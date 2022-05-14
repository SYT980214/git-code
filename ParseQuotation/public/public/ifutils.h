#ifndef IFUTILS_H
#define IFUTILS_H
//#define WIN32_LEAN_AND_MEAN // 防止windows.h 包含 winsock.h//这个放到预处理器中定义
#include "HSLog.h"
#include <string>
#include <time.h>
#include <iostream>
#include<vector>
#include"Include/pack_interface.h"
#include <fstream>
#include <map>
#include <stdio.h>
#include<sstream>
#include<algorithm>
#include <vector>
#include <experimental/filesystem>
//#include "HSLog.h"
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>//不能引用winsock.h, 会冲突
#else
#include <unistd.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#pragma comment(lib,"ws2_32.lib")

using namespace std;

//判断double类型的是否小于等于0
#define MIN_VALUE 1e-8
#define IS_DOUBLE_ZERO(d)  (abs(d) < MIN_VALUE)

//获取的是HHMMSS
int getTimeByInt();

string getDateByStr();

//返回字符串s的大写形式
string upper(string s) ;

// 字符串过滤空格函数
void trim(string &);

// 字符串截取函数
string& gettoken(string &sSource, string &sTarget, string sToken);

// 字符串转整型
int strtoint(string sSource);

// 字符串转浮点型
double strtofloat(string sSource);

string charTostr(char );

// 整型转字符串
string inttostr(int iSource);
string uint32tostr(uint32 iSource);
string int64tostr(int64 iSource);
string doubletostr(double dSource);

string wordtostr(WORD &iSource);

char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);

// 获取文件最近修改时间,系统时间表示
SYSTEMTIME getfilelasttime(const char *filename);

// 比较两个系统时间
int stcmp(SYSTEMTIME st1, SYSTEMTIME st2);

//返回指定的年、月、日是一年中的第几天
unsigned int day_of_year(unsigned int year, unsigned int month, unsigned int day);
//返回一年总天数
unsigned int day_count_of_year(unsigned int year);

// 异常处理函数
int exception_filter(LPEXCEPTION_POINTERS p_exinfo);


// 浮点数转string
char * doubleToStr(const double data, char * buffer);

int RoundInt(int iData, int iPosition);
int RoundInt(double dData, int iPosition);
double Round(double dData, int iPosition); 

#define MIN(a,b) (a < b)? a:b

/*根据年iYear、月iMonth、iDay，返回对应的时间类型*/
time_t MakeTime(int AYear,int AMonth,int ADay,int AHour = 0, int AMiniute = 0, int ASecond = 0);

/*iDate 的格式为20130716*/
time_t MakeTime(int iDate);

time_t AddMonth(time_t ts,int month);

time_t AddDay(time_t ts,int day);

vector<char *> splite(char *str, char* fstr);//省内存法  堆版本
int destroy_splite(vector<char *> vec);

//不能传静态区字符串,用户自己要防止溢出问题
char *replace(char *str, char *oldstr, char *newstr);

/*====公共基础函数====*/
int ex_handler_continue(int AExceptionCode,PEXCEPTION_POINTERS AExceptionInfo);
int ex_handler_return_parent(int AExceptionCode,PEXCEPTION_POINTERS AExceptionInfo);
int ex_handler_kill(int AExceptionCode,PEXCEPTION_POINTERS AExceptionInfo);
string char2str(char AChar);
const char* GetFormatedDate();
const char* GetFormatedTime();
string hsGetCol(const char* ASourceStr,int AColNo,char AChar);
#define RF_REPLACE_ALL           0x00000001
#define RF_REPLACE_IGNORECASE    0x00000002
string StringReplace(const string &ASourceStr ,const string &AOldPattern,const string &ANewPattern,DWORD AReplaceFlags);
int RoundByType(int AValue, int AUnit,char AType);
int RoundByUnit(int AValue, int AUnit);
int RoundUpByUnit(int AValue, int AUnit);
int RoundDownByUnit(int AValue, int AUnit);
int NumberIsZero(double d);
  
int ifstrstrex(char *MainStr,char *SearchStr);
void PrintUnPack(IF2UnPacker* lpUnPack);

// 其他类型 -> string                                               
//template <class Type>
//void static TypeTostring(Type TypeTmp, string& strTmp)
//{
//	stringstream stream;
//	stream << TypeTmp;
//	stream >> strTmp;
//	stream.clear();
//	stream.str("");
//}
void static str2Double(string sTmp, double& dTmp)//最多支持6位小数，用float的话只能支持3位小数，断点看存在精度丢失问题，但实际打印出来是正常的
{
	sscanf(sTmp.c_str(), "%lf", &dTmp);
}
// string -> 为其他类型   ,转换为double时存在精度丢失问题,断点看存在精度丢失问题，但实际打印出来是正常的                              
template <class Type>
void static stringToType(string strTmp, Type& TypeTmp)
{
	stringstream stream;
	stream << strTmp;
	stream >> TypeTmp;
	stream.clear();
	stream.str("");
}
//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
//template <class Type>
//void static stringToNum(const string& str, Type& num)
//{
//	istringstream iss(str);
//	iss >> num;
//}

//=================字符串分割=========================
vector<string> split2(string str, char del); //忽略“空位”（::192:168:ABC::416）->（192 168 ABC 416）
//=============获取当前运行目录，需要再加\\=====================
string GetExePath(void);
//============判断文件是否存在===========================
bool file_exists(std::string fn);
//====记录日志=====
void addMyLog(int iLogLevel, string sMsg, string sFileName = "");//0=normal,1=hint,2=warning,3=error
void sleepX(unsigned long dwMilliseconds);
//========深圳bin行情用到的一些公共函数========
string GetKey(string str1, string str2);
// host long 64 to network
uint64 hl64ton(uint64 host);
//network to host long 64
uint64 ntohl64(uint64 host);
double GetBinPrice(uint64 iPrice);  //价格，N13(4)
double GetBinPriceItem(uint64 iPrice);  //价格，N13(4)
double GetBinQty(uint64 iQty);//数量，N15(2)
double GetBinAmt(uint64 iAmt); //金额，N18(4)
string GETBINARY_STR(char* sourceData, int iSourceSize);

//字符串替换，注意如果12212替换12为21，最终为21221
string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value);
//获取完整日期时间格式 YYYYMMDDHHMMSSsss
string GetLocalDateTime();
//获取日期格式 YYYYMMDD
string GetLocalDate();
string GetLocalDateByTime(time_t aDay);
//获取时间格式 HHMMSS
string GetLocalTime();
// 返回自系统开机以来的毫秒数（tick）
unsigned long GetMYTickCount();
void mytolower(char *s);
void mytoupper(char *s);
std::string UnicodeToUTF8(const std::wstring & wstr);
std::wstring UTF8ToUnicode(const std::string & str);
std::wstring Utf8ToUnicode(const std::string &strUTF8);
std::string UnicodeToUtf8(const std::wstring &strUnicode);
std::wstring StringToWString(const std::string &str);
std::string WStringToString(const std::wstring &wstr);

//UTF-8转Unicode 
std::wstring Utf82Unicode(const std::string& utf8string);
//unicode 转为 ascii 
string WideByte2Acsi(wstring& wstrcode);
//utf-8 转 ascii 
string UTF_82ASCII(string& strUtf8Code);
//ascii 转 Unicode 
wstring Acsi2WideByte(string& strascii);
//Unicode 转 Utf8 
std::string Unicode2Utf8(const std::wstring& widestring);
//ascii 转 Utf8 
string ASCII2UTF8(string& strAsciiCode);

#endif //IFUTILS_H
