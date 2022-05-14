// HsDate.cpp: implementation of the CHsDate class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4996)
#include "HsDate.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHsDate::CHsDate(int year, int month, int day)
{

}

CHsDate::~CHsDate()
{

}

bool CHsDate::IsValidDate()
{
  if(Year < 1 || Month < 1 || Month > 12 || Day < 1 || Day > GetMonthDays())
  {
    return false;
  }
  return true;
}

bool CHsDate::IsLeapYear(int year)
{
  return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

int CHsDate::GetMonthDays(int year, int month)
{
  int n = 0;
  char adom[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(month > 0 && month <= 12)
  {
    n = adom[month - 1];
    if(month == 2 && IsLeapYear(year)) 
    {
      n ++;
    }
  }
  return n;

}

int CHsDate::GetMonthDays()
{
  return GetMonthDays(Year, Month);
}

bool CHsDate::IsLeapYear()
{
  return IsLeapYear(Year);
}

int CHsDate::GetDateDiff(CHsDate to)
{
  if(to < *this)
  {
    return to.GetDateDiff(*this);
  }
  int days = 0;
  for(int year = Year; year < to.Year; year++)
  {
    if(IsLeapYear(year))
    {
      days += 366;
    }
    else
    {
      days +=365;
    }
  }
  days += to.DayOfYear();
  days -= DayOfYear();
  return days;
}

int CHsDate::DayOfYear()
{
  short adoy[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  int n = adoy[Month - 1] + Day;
  if(Month > 2 && IsLeapYear())
  {
    n ++;
  }
  return n;
}

bool CHsDate::operator<(const CHsDate &date)
{
  if(Year < date.Year)
  {
    return true;
  }
  if(Year > date.Year)
  {
    return false;
  }
  if(Month < date.Month)
  {
    return true;
  }
  if(Month > date.Month)
  {
    return false;
  }
  if(Day < date.Day)
  {
    return true;
  }
  else
  {
    return false;
  }
}

CHsDate::CHsDate(string dateStr)
{
  char year[5];
  char month[3];
  char day[3];
  strncpy(year, dateStr.c_str(), 4);
  strncpy(month, dateStr.substr(4,5).c_str(), 2);
  strncpy(day, dateStr.substr(6,7).c_str(), 2);

  Year = atoi(year);
  Month = atoi(month);
  Day = atoi(day);
  if(!IsValidDate())
  {
    Year = 0;
    Month = 0;
    Day = 0;
  }
}

CHsDate::CHsDate(int dateInt)
{
  Year = dateInt / 10000;
  Month = (dateInt % 10000) / 100;
  Day = dateInt % 100;
}

CHsDate::CHsDate()
{
  struct tm t;
  time_t now;
  time(&now);
  t = *localtime(&now);
  Year=t.tm_year;
  if (Year<50)
  {
    Year+=2000;
  }
  else
  {
    Year+=1900;
  }
  Month = t.tm_mon + 1;
  Day = t.tm_mday;
}
