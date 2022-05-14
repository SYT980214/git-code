// HSLog.cpp: implementation of the CHSLog class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4996)
#include <fstream>
#include <vector>
#include <stdarg.h>//让函式能够接收不定量参数
#include "HSLog.h"
#include <time.h>
#include <windows.h>
#include <iostream>
#include <io.h>
#include <sys/stat.h>
using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHSLog *CHSLog::pHsLog = NULL;	//判断现在是否有日志文件
CHSLog::CHSLog(string sFileName)
{
	iLogLevel = 3;
	this->sFileName = sFileName;
	sPath = GetExePath() + "\\";
	InitHsLog();
	iTicks = GetTickCount();
}

CHSLog::~CHSLog()
{

}

CHSLog* CHSLog::GetHSLog(string sFileName)
{
	if (pHsLog == NULL)
	{
		pHsLog = new CHSLog(sFileName);
	}
	return pHsLog;
}

int CHSLog::InitHsLog()	//创建日志文件，并设定日志级别
{
	try
	{
		//setlocale(LC_CTYPE, "");//解决中文路径问题
		//file<> fileDoc(szPath.c_str());
		//setlocale(LC_CTYPE, 0);
		INIParser ini_parser;
		ini_parser.ReadINI(sPath + "HsServerInfo.ini");
		iLogLevel = atoi(ini_parser.GetValue("writelog", "loglevel", "3").c_str());
		ini_parser.Clear();
		return 0;
	}
	catch (...)
	{
		iLogLevel = 3;
		//hsWriteError("[CHSLog::InitHsLog]初始化CHSLog异常,日志级别默认置为3！");
		return 0;
	}
}

void CHSLog::hsWriteLog(int LogLevel, string szMsg)
{
	try
	{
		long iTemp = GetTickCount() - iTicks;
		if (iTemp >= 10000)//十秒读一次配置文件
		{
			InitHsLog();
			iTicks = GetTickCount();
		}
		/*
			if(!fgInit)
			{
			  return;
			}
		*/
		if (LogLevel > iLogLevel)
		{
			return;
		}
		//获取时间
		struct tm t;
		time_t now;
		int year;
		char szDate[100] = "";
		char szLogFile[50] = "";
		time(&now);
		t = *localtime(&now);
		year = t.tm_year;
		if (year < 50)
		{
			year += 2000;
		}
		else
		{
			year += 1900;
		}
		sprintf(szDate, "%02d%02d%02d:", t.tm_hour, t.tm_min, t.tm_sec);
		////错误等级
		string szLevel;
		switch (LogLevel)
		{
		case 1:
			szLevel = "[FAULT]";
			break;
		case 3:
			szLevel = "[ERROR]";
			break;
		case 5:
			szLevel = "[WARN]";
			break;
		case 7:
			szLevel = "[INFO]";
			break;
		case 9:
			szLevel = "[DEBUG]";
			break;
		default:
			szLevel = "[UNKOWN]";
		}
		sprintf(szLogFile, "%sLogs\\%s_%04d%02d%02d.log", sPath.c_str(), sFileName.c_str(), year, t.tm_mon + 1, t.tm_mday);
		WriteFile(szLevel, szDate, szMsg, szLogFile);
	}
	catch (...)
	{
		cout << "[CHSLog::hsWriteLog]出现严重异常！";
	}
}


void CHSLog::WriteFile(string inszLogLevel, string inszDate, string inszMsg, string inszFile)
{
	try
	{
		ChangLogFileName(inszFile);
		ofstream fout(inszFile.c_str(), ios::out | ios::app);
		if (!fout)
		{
			// 打开文件错误，可能文件被删除或重命名
			return;
		}

		fout << inszLogLevel << inszDate << "  " << inszMsg << endl;
		fout.close();
	}
	catch (...)
	{
		cout << "[CHSLog::WriteFile]出现严重异常！";
	}
}

void CHSLog::addLog(int iLogLevel, char * format, ...)
{
	try
	{
		va_list args; // 内置变量：参数列表
		char tmp[HQLog_MAX_LINE_LEN + 1]; // 静态数组，用于参数列表传入字符串的缓冲
		tmp[HQLog_MAX_LINE_LEN] = 0;
		va_start(args, format); // 分析提取参数
		vsnprintf(tmp, HQLog_MAX_LINE_LEN, format, args);  // 将传入参数填入sz_format中
		va_end(args); // 结束不定参处理

		hsWriteLog(iLogLevel, tmp);
	}
	catch (...)
	{

	}
}

string CHSLog::GetDateTime()
{
	struct tm t;
	time_t now;
	int year;
	char szDate[100] = "";
	time(&now);
	t = *localtime(&now);
	year = t.tm_year;
	if (year < 50)
	{
		year += 2000;
	}
	else
	{
		year += 1900;
	}
	sprintf(szDate, "%04d-%02d-%02d %02d:%02d:%02d ", year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	return szDate;
}

void CHSLog::ChangLogFileName(string inszFileName)
{
	int i = 1;
	char sNewFile[200] = "";
	if (GetFileLen(inszFileName) > MAXLOGLEN)
	{
		sprintf(sNewFile, "%s_%d", inszFileName.c_str(), i);
		while (((_access(sNewFile, 0)) != -1))
		{
			i++;
			sprintf(sNewFile, "%s_%d", inszFileName.c_str(), i);
		}
		int iRet = rename(inszFileName.c_str(), sNewFile);
		if (iRet != 0)
		{
			addLog(3, "[ChangLogFileName]文件超过了100M,修改文件名称失败！");
		}
	}
}

unsigned long CHSLog::GetFileLen(string inszFileName)
{
	struct _stat buf;
	if (_stat(inszFileName.c_str(), &buf) == 0)
	{
		return buf.st_size;
	}
	else
		return 0;
}

string CHSLog::GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
	string path = szFilePath;
	return path;
}

