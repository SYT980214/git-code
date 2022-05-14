#ifndef IFUTILS_H
#define IFUTILS_H
//#define WIN32_LEAN_AND_MEAN // ��ֹwindows.h ���� winsock.h//����ŵ�Ԥ�������ж���
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
#include <winsock2.h>//��������winsock.h, ���ͻ
#else
#include <unistd.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#pragma comment(lib,"ws2_32.lib")

using namespace std;

//�ж�double���͵��Ƿ�С�ڵ���0
#define MIN_VALUE 1e-8
#define IS_DOUBLE_ZERO(d)  (abs(d) < MIN_VALUE)

//��ȡ����HHMMSS
int getTimeByInt();

string getDateByStr();

//�����ַ���s�Ĵ�д��ʽ
string upper(string s) ;

// �ַ������˿ո���
void trim(string &);

// �ַ�����ȡ����
string& gettoken(string &sSource, string &sTarget, string sToken);

// �ַ���ת����
int strtoint(string sSource);

// �ַ���ת������
double strtofloat(string sSource);

string charTostr(char );

// ����ת�ַ���
string inttostr(int iSource);
string uint32tostr(uint32 iSource);
string int64tostr(int64 iSource);
string doubletostr(double dSource);

string wordtostr(WORD &iSource);

char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);

// ��ȡ�ļ�����޸�ʱ��,ϵͳʱ���ʾ
SYSTEMTIME getfilelasttime(const char *filename);

// �Ƚ�����ϵͳʱ��
int stcmp(SYSTEMTIME st1, SYSTEMTIME st2);

//����ָ�����ꡢ�¡�����һ���еĵڼ���
unsigned int day_of_year(unsigned int year, unsigned int month, unsigned int day);
//����һ��������
unsigned int day_count_of_year(unsigned int year);

// �쳣������
int exception_filter(LPEXCEPTION_POINTERS p_exinfo);


// ������תstring
char * doubleToStr(const double data, char * buffer);

int RoundInt(int iData, int iPosition);
int RoundInt(double dData, int iPosition);
double Round(double dData, int iPosition); 

#define MIN(a,b) (a < b)? a:b

/*������iYear����iMonth��iDay�����ض�Ӧ��ʱ������*/
time_t MakeTime(int AYear,int AMonth,int ADay,int AHour = 0, int AMiniute = 0, int ASecond = 0);

/*iDate �ĸ�ʽΪ20130716*/
time_t MakeTime(int iDate);

time_t AddMonth(time_t ts,int month);

time_t AddDay(time_t ts,int day);

vector<char *> splite(char *str, char* fstr);//ʡ�ڴ淨  �Ѱ汾
int destroy_splite(vector<char *> vec);

//���ܴ���̬���ַ���,�û��Լ�Ҫ��ֹ�������
char *replace(char *str, char *oldstr, char *newstr);

/*====������������====*/
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

// �������� -> string                                               
//template <class Type>
//void static TypeTostring(Type TypeTmp, string& strTmp)
//{
//	stringstream stream;
//	stream << TypeTmp;
//	stream >> strTmp;
//	stream.clear();
//	stream.str("");
//}
void static str2Double(string sTmp, double& dTmp)//���֧��6λС������float�Ļ�ֻ��֧��3λС�����ϵ㿴���ھ��ȶ�ʧ���⣬��ʵ�ʴ�ӡ������������
{
	sscanf(sTmp.c_str(), "%lf", &dTmp);
}
// string -> Ϊ��������   ,ת��Ϊdoubleʱ���ھ��ȶ�ʧ����,�ϵ㿴���ھ��ȶ�ʧ���⣬��ʵ�ʴ�ӡ������������                              
template <class Type>
void static stringToType(string strTmp, Type& TypeTmp)
{
	stringstream stream;
	stream << strTmp;
	stream >> TypeTmp;
	stream.clear();
	stream.str("");
}
//ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ���ͣ��˷��������ձ������ԣ�
//template <class Type>
//void static stringToNum(const string& str, Type& num)
//{
//	istringstream iss(str);
//	iss >> num;
//}

//=================�ַ����ָ�=========================
vector<string> split2(string str, char del); //���ԡ���λ����::192:168:ABC::416��->��192 168 ABC 416��
//=============��ȡ��ǰ����Ŀ¼����Ҫ�ټ�\\=====================
string GetExePath(void);
//============�ж��ļ��Ƿ����===========================
bool file_exists(std::string fn);
//====��¼��־=====
void addMyLog(int iLogLevel, string sMsg, string sFileName = "");//0=normal,1=hint,2=warning,3=error
void sleepX(unsigned long dwMilliseconds);
//========����bin�����õ���һЩ��������========
string GetKey(string str1, string str2);
// host long 64 to network
uint64 hl64ton(uint64 host);
//network to host long 64
uint64 ntohl64(uint64 host);
double GetBinPrice(uint64 iPrice);  //�۸�N13(4)
double GetBinPriceItem(uint64 iPrice);  //�۸�N13(4)
double GetBinQty(uint64 iQty);//������N15(2)
double GetBinAmt(uint64 iAmt); //��N18(4)
string GETBINARY_STR(char* sourceData, int iSourceSize);

//�ַ����滻��ע�����12212�滻12Ϊ21������Ϊ21221
string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value);
//��ȡ��������ʱ���ʽ YYYYMMDDHHMMSSsss
string GetLocalDateTime();
//��ȡ���ڸ�ʽ YYYYMMDD
string GetLocalDate();
string GetLocalDateByTime(time_t aDay);
//��ȡʱ���ʽ HHMMSS
string GetLocalTime();
// ������ϵͳ���������ĺ�������tick��
unsigned long GetMYTickCount();
void mytolower(char *s);
void mytoupper(char *s);
std::string UnicodeToUTF8(const std::wstring & wstr);
std::wstring UTF8ToUnicode(const std::string & str);
std::wstring Utf8ToUnicode(const std::string &strUTF8);
std::string UnicodeToUtf8(const std::wstring &strUnicode);
std::wstring StringToWString(const std::string &str);
std::string WStringToString(const std::wstring &wstr);

//UTF-8תUnicode 
std::wstring Utf82Unicode(const std::string& utf8string);
//unicode תΪ ascii 
string WideByte2Acsi(wstring& wstrcode);
//utf-8 ת ascii 
string UTF_82ASCII(string& strUtf8Code);
//ascii ת Unicode 
wstring Acsi2WideByte(string& strascii);
//Unicode ת Utf8 
std::string Unicode2Utf8(const std::wstring& widestring);
//ascii ת Utf8 
string ASCII2UTF8(string& strAsciiCode);

#endif //IFUTILS_H
