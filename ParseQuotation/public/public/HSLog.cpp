// HSLog.cpp: implementation of the CHSLog class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4996)
#include <fstream>
#include <vector>
#include <stdarg.h>//�ú�ʽ�ܹ����ղ���������
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
CHSLog *CHSLog::pHsLog = NULL;	//�ж������Ƿ�����־�ļ�
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

int CHSLog::InitHsLog()	//������־�ļ������趨��־����
{
	try
	{
		//setlocale(LC_CTYPE, "");//�������·������
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
		//hsWriteError("[CHSLog::InitHsLog]��ʼ��CHSLog�쳣,��־����Ĭ����Ϊ3��");
		return 0;
	}
}

void CHSLog::hsWriteLog(int LogLevel, string szMsg)
{
	try
	{
		long iTemp = GetTickCount() - iTicks;
		if (iTemp >= 10000)//ʮ���һ�������ļ�
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
		//��ȡʱ��
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
		////����ȼ�
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
		cout << "[CHSLog::hsWriteLog]���������쳣��";
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
			// ���ļ����󣬿����ļ���ɾ����������
			return;
		}

		fout << inszLogLevel << inszDate << "  " << inszMsg << endl;
		fout.close();
	}
	catch (...)
	{
		cout << "[CHSLog::WriteFile]���������쳣��";
	}
}

void CHSLog::addLog(int iLogLevel, char * format, ...)
{
	try
	{
		va_list args; // ���ñ����������б�
		char tmp[HQLog_MAX_LINE_LEN + 1]; // ��̬���飬���ڲ����б����ַ����Ļ���
		tmp[HQLog_MAX_LINE_LEN] = 0;
		va_start(args, format); // ������ȡ����
		vsnprintf(tmp, HQLog_MAX_LINE_LEN, format, args);  // �������������sz_format��
		va_end(args); // ���������δ���

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
			addLog(3, "[ChangLogFileName]�ļ�������100M,�޸��ļ�����ʧ�ܣ�");
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
	(strrchr(szFilePath, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�
	string path = szFilePath;
	return path;
}

