// HsDate.h: interface for the CHsDate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSDATE_H__34F7E15D_766B_46F3_8A88_B4FB0160B4D1__INCLUDED_)
#define AFX_HSDATE_H__34F7E15D_766B_46F3_8A88_B4FB0160B4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>

using namespace std;

class CHsDate  
{
public:
	CHsDate();
	CHsDate(int dateInt);
	CHsDate(string dateStr);
	int DayOfYear();
	int GetDateDiff(CHsDate to);
	bool IsLeapYear();
	int GetMonthDays();
	int GetMonthDays(int year, int month);
	bool IsLeapYear(int year);
	bool IsValidDate();
  bool operator<(const CHsDate &date); 
	CHsDate(int year, int month, int day);
	virtual ~CHsDate();

private:
  int Day;
  int Month;
  int Year;

};

#endif // !defined(AFX_HSDATE_H__34F7E15D_766B_46F3_8A88_B4FB0160B4D1__INCLUDED_)
