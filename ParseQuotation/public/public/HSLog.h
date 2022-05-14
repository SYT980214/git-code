// HSLog.h: interface for the CHSLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSLOG_H__DCF37C5D_37B1_44B9_8585_302B07AE4FCA__INCLUDED_)
#define AFX_HSLOG_H__DCF37C5D_37B1_44B9_8585_302B07AE4FCA__INCLUDED_

#pragma warning(disable:4786)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "INIParser.h"
using namespace std;

#define PHSLOG CHSLog::GetHSLog()

const unsigned long MAXLOGLEN = 1024 * 1024 * 100; //100M 

class CHSLog
{
public:
	void addLog(int iLogLevel, char * format, ...);
	static CHSLog* GetHSLog(string sFileName = "Default");
	virtual ~CHSLog();
private:
	int InitHsLog();
	void hsWriteLog(int LogLevel, string szMsg);
	void WriteFile(string inszLogLevel, string inszDate, string inszMsg, string inszFile);
	void ChangLogFileName(string inszFileName);
	unsigned long GetFileLen(string inszFileName);
	string GetDateTime();
	string GetExePath(void);
	int iLogLevel;
	unsigned long iTicks;
	static CHSLog *pHsLog;
	string sPath;//Â·¾¶£¬´øÐ±¸Ü
	string sFileName = "Default";
	CHSLog(string sFileName = "Default");
};

#define HQLog_MAX_LINE_LEN 40960
#endif // !defined(AFX_HSLOG_H__DCF37C5D_37B1_44B9_8585_302B07AE4FCA__INCLUDED_)
