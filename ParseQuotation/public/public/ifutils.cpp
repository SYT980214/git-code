#pragma warning(disable:4996)
#include "ifutils.h"
#include <iomanip>
#include <sstream>

int getTimeByInt()
{
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);

	return (t.tm_hour * 10000 + t.tm_min * 100 + t.tm_sec);
}

string getDateByStr()
{
	struct tm t;
	string RetDate;
	time_t now;
	int iyear, imonth, iday;
	string syear, smonth, sday;
	time(&now);
	localtime_s(&t, &now);
	iyear = t.tm_year + 1900;
	imonth = t.tm_mon + 1;
	iday = t.tm_mday;

	syear = inttostr(iyear);
	smonth = inttostr(imonth);
	if (smonth.length() == 1)
	{
		smonth = "0" + smonth;
	}
	sday = inttostr(iday);
	if (sday.length() == 1)
	{
		sday = "0" + sday;
	}
	RetDate = syear + smonth + sday;
	return RetDate;
}


string upper(string s) //�����ַ���s�Ĵ�д��ʽ
{
	string temp(s);
	for (int i = 0; i < (int)s.size(); i++)
	{
		temp[i] = toupper(s[i]);
	}
	return temp;
}

void trim(string& s)
{
	if (s.empty())
	{
		return;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}

string& gettoken(string &sSource, string &sTarget, string sToken)
{
	size_t ilen = sToken.length();
	if (ilen == 0)
	{
		return sSource;
	}

	sTarget.erase(sTarget.begin(), sTarget.end());//sTarget.clear();
	size_t ipos = sSource.find_first_of(sToken);
	sTarget = sSource.substr(0, ipos);
	sSource.erase(0, ipos + ilen);

	return sSource;
}

int strtoint(string sSource)
{
	trim(sSource);
	int iTmp = atoi(sSource.c_str());
	return iTmp;
}

double strtofloat(string sSource)
{
	trim(sSource);
	double dTmp = atof(sSource.c_str());
	return dTmp;
}

string inttostr(int iSource)
{
	stringstream ss;
	ss.str("");
	ss << iSource;
	return ss.str();
}

string uint32tostr(uint32 iSource)
{
	stringstream ss;
	ss.str("");
	ss << iSource;
	return ss.str();
}

string int64tostr(int64 iSource)
{
	stringstream ss;
	ss.str("");
	ss << iSource;
	return ss.str();
}

string doubletostr(double dSource)
{
	stringstream ss;
	ss.str("");
	ss << dSource;
	return ss.str();
}


string wordtostr(WORD &iSource)
{
	stringstream ss;
	ss.str("");
	ss << iSource;
	return ss.str();
}

SYSTEMTIME getfilelasttime(const char *filename)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(filename, &fd);
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&(fd.ftLastWriteTime), &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	FindClose(hFind);
	return stLocal;
}

int stcmp(SYSTEMTIME st1, SYSTEMTIME st2)
{
	int ret = 0;

	if (st1.wYear > st2.wYear)
	{
		ret = 1;
	}
	else if (st1.wYear < st2.wYear)
	{
		ret = -1;
	}
	else
	{
		if (st1.wMonth > st2.wMonth)
		{
			ret = 1;
		}
		else if (st1.wMonth < st2.wMonth)
		{
			ret = -1;
		}
		else
		{
			if (st1.wDay > st2.wDay)
			{
				ret = 1;
			}
			else if (st1.wDay < st2.wDay)
			{
				ret = -1;
			}
			else
			{
				if (st1.wHour > st2.wHour)
				{
					ret = 1;
				}
				else if (st1.wHour < st2.wHour)
				{
					ret = -1;
				}
				else
				{
					if (st1.wMinute > st2.wMinute)
					{
						ret = 1;
					}
					else if (st1.wMinute < st2.wMinute)
					{
						ret = -1;
					}
					else
					{
						if (st1.wSecond > st2.wSecond)
						{
							ret = 1;
						}
						else if (st1.wSecond < st2.wSecond)
						{
							ret = -1;
						}
						else
						{
							if (st1.wMilliseconds > st2.wMilliseconds)
							{
								ret = 1;
							}
							else if (st1.wMilliseconds < st2.wMilliseconds)
							{
								ret = -1;
							}
							else
							{
								ret = 0;
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

//����ָ�����ꡢ�¡�����һ���еĵڼ���
unsigned int day_of_year(unsigned int year, unsigned int month, unsigned int day)
{
	unsigned int days = 0;
	for (unsigned int i = 1; i < month; i++)
	{
		switch (i)
		{
		case 4:
		case 6:
		case 9:
		case 11:
			days += 30;
			break;
		case 2:
			days += (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
			break;
		default:
			days += 31;
		}
	}
	days += day;
	return days;
}

unsigned int day_count_of_year(unsigned int year)
{
	return ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 366 : 365);
}

int exception_filter(LPEXCEPTION_POINTERS p_exinfo)
{
	DWORD exceptcode = p_exinfo->ExceptionRecord->ExceptionCode;
	switch (exceptcode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "�洢�����쳣..." ) );
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "��0���쳣...") );
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "����Խ���쳣...") );
		break;
	case EXCEPTION_INT_OVERFLOW:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "��������쳣...") );
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "�Ƿ�ָ���쳣...") );
		break;
	case EXCEPTION_STACK_OVERFLOW:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "��ջ����쳣...") );
		break;
	case EXCEPTION_INVALID_HANDLE:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "��Ч����쳣...") );
		break;
	default:
		//CHSLog::GetHSLog()->hsWriteError( strcat( pinfo, "�����쳣,����:[%x]..." ), exceptcode );
		break;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

/*
 *��������doubleToStr
 *����: data - ��ת����double����ֵ
		buffet - ��Ž���Ļ�����
 *����: ������ָ��
 *����: ��double���͵���ֵת�����ַ�����������Ļ�����
 */
char * doubleToStr(const double data, char * buffer)
{
	if (!buffer)
	{
		return NULL;
	}
	sprintf(buffer, "%f", data);
	return buffer;
}

char *w2c(char *pcstr, const wchar_t *pwstr, size_t len)

{

	size_t nlength = wcslen(pwstr);

	//��ȡת����ĳ���

	size_t nbytes = WideCharToMultiByte(0, // specify the code page used to perform the conversion

		0,         // no special flags to handle unmapped characters

		pwstr,     // wide character string to convert

		nlength,   // the number of wide characters in that string

		NULL,      // no output buffer given, we just want to know how long it needs to be

		0,

		NULL,      // no replacement character given

		NULL);    // we don't want to know if a character didn't make it through the translation

	// make sure the buffer is big enough for this, making it larger if necessary

	if (nbytes > len)   nbytes = len;

	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�

	WideCharToMultiByte(0, // specify the code page used to perform the conversion

		0,         // no special flags to handle unmapped characters

		pwstr,   // wide character string to convert

		nlength,   // the number of wide characters in that string

		pcstr, // put the output ascii characters at the end of the buffer

		nbytes,                           // there is at least this much space there

		NULL,      // no replacement character given

		NULL);

	return pcstr;

}

//�����ν����������룬iPosition��Բ������λ����Ϊ1,10,100...
int RoundInt(int iData, int iPosition)
{
	if (iPosition <= 0 || iPosition % 10 != 0)
	{
		return iData;
	}
	return ((iData + (iPosition / 2)) / iPosition) * iPosition;
}

//�Ը�����������������Բ������iPosition��Բ������λ����Ϊ1,10,100...
int RoundInt(double dData, int iPosition)
{
	if (iPosition == 1)
	{
		return int(dData + 0.5);
	}
	return RoundInt(int(dData), iPosition);
}

//����С���������λ��iPositionΪ1,2,3...
double Round(double dData, int iPosition)
{
	stringstream ss;
	ss << fixed << setprecision(iPosition);
	ss << dData;
	double dReturn;
	ss >> dReturn;
	return dReturn;
}

/*====������������====*/
void exception_handler(PEXCEPTION_POINTERS AExceptionInfo)
{
	//addMyLog(0, "[exception_handler]ExceptionCode=%x,ExceptionInformation=%s",
	//AExceptionInfo->ExceptionRecord->ExceptionCode, AExceptionInfo->ExceptionRecord->ExceptionInformation);
}

/*__try...__except�쳣������,����ִ�г���
*/
int ex_handler_continue(int AExceptionCode, PEXCEPTION_POINTERS AExceptionInfo)
{
	exception_handler(AExceptionInfo);
	//CHSLog::GetHSLog()->hsWriteError("�����쳣���������ִ��");  
	return -1;
}

/*__try...__except�쳣�����������쳣�׸���һ��
*/
int ex_handler_return_parent(int AExceptionCode, PEXCEPTION_POINTERS AExceptionInfo)
{
	exception_handler(AExceptionInfo);
	//CHSLog::GetHSLog()->hsWriteError("�����쳣���쳣���׵���һ��");
	return 0;
}

/*__try...__except�쳣������,��������
*/
int ex_handler_kill(int AExceptionCode, PEXCEPTION_POINTERS AExceptionInfo)
{
	exception_handler(AExceptionInfo);
	//CHSLog::GetHSLog()->hsWriteError("�����쳣����ֹ����");
	return 1;
}


string char2str(char AChar)
{
	char s[2];
	s[0] = AChar;
	s[1] = 0;
	return s;
}


time_t MakeTime(int AYear, int AMonth, int ADay, int AHour, int AMiniute, int ASecond)
{
	struct tm  timeptr;
	time_t  time_now;

	memset(&timeptr, 0, sizeof(struct tm));
	timeptr.tm_year = AYear - 1900;
	timeptr.tm_mon = AMonth - 1;
	timeptr.tm_mday = ADay;
	timeptr.tm_hour = AHour;
	timeptr.tm_min = AMiniute;
	timeptr.tm_sec = ASecond;

	time_now = mktime(&timeptr);
	return time_now;
}

//iDate �ĸ�ʽΪ20130716
time_t MakeTime(int iDate)
{
	struct tm  timeptr;
	time_t  time_now;
	memset(&timeptr, 0, sizeof(struct tm));
	timeptr.tm_year = iDate / 10000 - 1900;
	timeptr.tm_mon = (iDate % 10000) / 100 - 1;
	timeptr.tm_mday = (iDate % 10000) % 100;
	timeptr.tm_hour = 0;
	timeptr.tm_min = 0;
	timeptr.tm_sec = 0;

	time_now = mktime(&timeptr);
	return time_now;
}

time_t AddMonth(time_t ts, int month)
{
	struct tm temp;
	localtime_s(&temp, &ts);
	temp.tm_mon = temp.tm_mon + month;
	return mktime(&temp);
}

time_t AddDay(time_t ts, int day)
{
	struct tm temp;
	localtime_s(&temp, &ts);
	temp.tm_mday = temp.tm_mday + day;
	return mktime(&temp);
}

const char* GetFormatedDate()
{
	const int D_LEN = 9;
	static char st_FormatedDate[D_LEN];
	struct tm t;
	time_t now;
	int year;

	time(&now);
	t = *localtime(&now);
	year = t.tm_year;
	if (year < 50)
		year += 2000;
	else
		year += 1900;
	sprintf(st_FormatedDate, "%04d%02d%02d", year, t.tm_mon + 1, t.tm_mday);
	st_FormatedDate[D_LEN - 1] = 0;
	return st_FormatedDate;
}

const char* GetFormatedTime()
{
	const int T_LEN = 7;
	static char st_FormatedTime[T_LEN];
	struct tm t;
	time_t now;
	int year;

	time(&now);
	t = *localtime(&now);
	year = t.tm_year;
	if (year < 50)
		year += 2000;
	else
		year += 1900;
	sprintf(st_FormatedTime, "%02d%02d%02d", t.tm_hour, t.tm_min, t.tm_sec);
	st_FormatedTime[T_LEN - 1] = 0;
	return st_FormatedTime;
}

/*
  GetColString: ȡ��ָ���ַ����а���ָ���ָ����ָ���ָ������,���ַ�������
  ASourceStr: Դ��
  AColNo: ȡ�ڼ�����,��0��ʼ
  AChar : �ָ��ַ�
*/
string hsGetCol(const char* ASourceStr, int AColNo, char AChar)
{
	int curCol = 0;
	char const *beginStr = ASourceStr;
	char const *endStr = ASourceStr;
	while (beginStr)
	{
		endStr = strchr(beginStr, AChar);
		if (endStr)
		{
			if (curCol == AColNo)
			{
				break;
			}
			else
			{
				curCol++;
				endStr++;
				beginStr = endStr;
			}
		}
		else
		{
			if (curCol == AColNo)
			{
				endStr = ASourceStr + strlen(ASourceStr);
			}
			else
			{
				beginStr = NULL;
			}
			break;
		}

	}

	string retStr;
	if (beginStr)
	{
		for (char const *p = beginStr; p < endStr; ++p)
		{
			retStr.push_back(*p);
		}
	}
	return retStr;
}


int CompareStr(const char * lstr, const char * rstr, const size_t &count, const DWORD &AReplaceFlags)
{
	unsigned int i = 0;

	for (i = 0; i < count; i++)
	{
		if (AReplaceFlags & RF_REPLACE_IGNORECASE)
		{
			if (toupper(*(lstr + i)) != toupper(*(rstr + i)))
				break;
		}
		else
		{
			if (*(lstr + i) != *(rstr + i))
				break;
		}
	}

	return (i < count ? 1 : 0);
}

/*ʹ��ANewPattern���滻��ASourceStr�е�AOldPattern
  AReplaceFlags:
	RF_REPLACE_ALL-ȫ���滻
	RF_REPLACE_IGNORECASE-���Դ�Сд
  �磺RF_REPLACE_ALL|RF_REPLACE_IGNORECASE
*/

string StringReplace(const string &ASourceStr, const string &AOldPattern, const string &ANewPattern, DWORD AReplaceFlags)
{
	size_t curPos = 0;
	size_t oldPatLen = AOldPattern.length();
	size_t newPatLen = ANewPattern.length();
	size_t sourceLen = ASourceStr.length();
	size_t searchLen = 0;
	string result = "";
	result.reserve(sourceLen);

	if (sourceLen < oldPatLen)
		return ASourceStr;
	else
		searchLen = sourceLen - oldPatLen;

	while (curPos <= searchLen)
	{
		if (0 != CompareStr(&ASourceStr[curPos], AOldPattern.c_str(), oldPatLen, AReplaceFlags))
		{
			result.push_back(ASourceStr[curPos]);
			curPos++;
		}
		else
		{
			for (unsigned int i = 0; i < newPatLen; i++)
				result.push_back(ANewPattern[i]);
			curPos += oldPatLen;

			if (!(AReplaceFlags & RF_REPLACE_ALL))
				break;
		}
	}

	while (curPos < sourceLen)
	{
		result.push_back(ASourceStr[curPos]);
		curPos++;
	}
	return result;
}

/*�жϸ�����d�Ƿ�Ϊ0�����Ϊ0����1����0����0
*/
int NumberIsZero(double d)
{
	return ((d > -0.0000001) && (d < 0.0000001) ? 1 : 0);
}



/*
 ��AValue����Բ��
 AUnit��Բ��������λ
 AType��
   1-��������
   2-ֱ����
   3-ֱ����
*/

int RoundByType(int AValue, int AUnit, char AType)
{
	int retValue = AValue;
	switch (AType)
	{
	case '1':
		retValue = RoundByUnit(AValue, AUnit);
		break;
	case '2':
		retValue = RoundUpByUnit(AValue, AUnit);
		break;
	case '3':
		retValue = RoundDownByUnit(AValue, AUnit);
		break;
	}
	return retValue;
}

int RoundByUnit(int AValue, int AUnit)
{
	int retVal = AValue;
	double d = AValue * 1.0 / AUnit;
	if (int(d) != int(d + 0.5))
	{
		retVal = RoundUpByUnit(AValue, AUnit);
	}
	else
	{
		retVal = RoundDownByUnit(AValue, AUnit);
	}
	return retVal;
}

int RoundUpByUnit(int AValue, int AUnit)
{
	return ((AValue / AUnit) * AUnit + ((AValue%AUnit) > 0 ? AUnit : 0));
}

int RoundDownByUnit(int AValue, int AUnit)
{
	return ((AValue / AUnit) * AUnit);
}

string charTostr(char cSource)
{
	stringstream ss;
	ss.str("");
	ss << cSource;
	return ss.str();
}

int ifstrstrex(char *MainStr, char *SearchStr)
{
	char s[103];
	memset(s, 0, sizeof(s));
	_snprintf(s, sizeof(s), ",%s,", SearchStr);
	if (strstr(MainStr, SearchStr) == NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}

}


vector<char *> splite(char *str, char* fstr)//ʡ�ڴ淨  �Ѱ汾
{
	vector<char *> tmparr;
	if (str == NULL || fstr == NULL)
		return tmparr;

	char* copystr = (char *)malloc(strlen(str) + 1);
	memset(copystr, 0, sizeof(copystr));
	strcpy(copystr, str);//���ԭ
	char* i = copystr;//ǰָ��
	char* freefirst = copystr;
	//char **tmparr;

	char* chnext = 0;
	for (; chnext = strstr(i, fstr);)
	{
		//if (chnext == 0)//��һ��Ϊ�ҵ���
		//	continue;
		int size = chnext - i + 1;//string  end \0 so +1
		char *tmpstr = (char *)calloc(size, sizeof(char));

		memcpy_s(tmpstr, size - 1, i, size - 1);
		tmparr.push_back(tmpstr);
		//int chnext = strstr(str, fstr);
		i = chnext + strlen(fstr);
	}
	size_t size = copystr + strlen(copystr) - i + 1;//last
	char *tmpstr = (char *)calloc(size, sizeof(char));

	memcpy_s(tmpstr, size - 1, i, size - 1);
	tmparr.push_back(tmpstr);
	free(freefirst);//free copystr
	//str = copystr;
	return tmparr;

}

//�ָ����ַ�������vec   ���ָ���ַ���str 
int destroy_splite(vector<char *> vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		free(vec[i]);
	}
	return 0;
}

char *replace(char *str, char *oldstr, char *newstr)
{
	if (NULL == str || NULL == oldstr || NULL == newstr)
		return NULL;
	if (!strcmp(oldstr, newstr))
	{
		return str;
	}
	size_t len = strlen(str);
	char * tmp = strstr(str, oldstr);
	if (NULL == tmp)
	{
		return NULL;
	}
	size_t oldlen = strlen(oldstr);
	size_t newlen = strlen(newstr);
	if (oldlen == newlen)
	{
		while (oldlen--)
			*tmp++ = *newstr++;
	}
	else
	{
		//char after[len + 1];
		char *behind = (char *)malloc(len + 1);//������ܻ�������ڴ�����Ż�
		strcpy(behind, tmp + strlen(oldstr));

		*tmp = '\0';
		strcat(str, newstr);
		strcpy(str + strlen(str), behind);
		free(behind);
	}
	return str;
}


void PrintUnPack(IF2UnPacker* lpUnPack)
{
	int ii;
	for (ii = 0; ii < lpUnPack->GetDatasetCount(); ii++)
	{
		lpUnPack->SetCurrentDatasetByIndex(ii);
		//CHSLog::GetHSLog()->hsWriteDebug("=====================================================================");
		//CHSLog::GetHSLog()->hsWriteDebug("�������       %d",ii);
		//CHSLog::GetHSLog()->hsWriteDebug("��¼������     %d",lpUnPack->GetRowCount());
		//CHSLog::GetHSLog()->hsWriteDebug("��������       %d",lpUnPack->GetColCount());
		while (!lpUnPack->IsEOF())
		{
			for (int i = 0; i < lpUnPack->GetColCount(); i++)
			{
				char* colName = (char*)lpUnPack->GetColName(i);
				char colType = lpUnPack->GetColType(i);
				if (colType == 's' || colType == 'S')
				{
					char* colValue = (char*)lpUnPack->GetStrByIndex(i);
					//CHSLog::GetHSLog()->hsWriteDebug("%s : %s",colName,colValue);
				}
				else if (colType == 'R' || colType == 'r')
				{
					int colLength = 0;
					char* colValue = (char*)lpUnPack->GetRawByIndex(i, &colLength);
					//CHSLog::GetHSLog()->hsWriteDebug("%s  : %s    len:%d     ",colName,colValue,colLength);
				}
				else if (colType == 'i' || colType == 'I')
				{
					int colValue = atoi((char*)lpUnPack->GetStrByIndex(i));
					//CHSLog::GetHSLog()->hsWriteDebug("%s : %d",colName,colValue);
				}
				else if (colType == 'd' || colType == 'D')
				{
					double colValue = atof((char*)lpUnPack->GetStrByIndex(i));
					//CHSLog::GetHSLog()->hsWriteDebug("%s : %f",colName,colValue);
				}
				else if (colType == 'f' || colType == 'F')
				{
					float colValue = atof((char*)lpUnPack->GetStrByIndex(i));
					//CHSLog::GetHSLog()->hsWriteDebug("%s : %f",colName,colValue);
				}
				else {
					//CHSLog::GetHSLog()->hsWriteDebug("colType[%c] cant recognize", colType);
				}
			}
			//CHSLog::GetHSLog()->hsWriteDebug("******************************NextRow************************************");
			lpUnPack->Next();
		}
	}
	lpUnPack->First();
}

//=================�ַ����ָ�=========================
vector<string> split2(string str, char del) //���ԡ���λ����::192:168:ABC::416��->��192 168 ABC 416��
{
	string tmp = str;
	stringstream ss(tmp);
	string tok;
	vector<string> ret;
	while (getline(ss, tok, del))
		ret.push_back(tok);
	return ret;
}
//=============��ȡ��ǰ����Ŀ¼����Ҫ�ټ�\\=====================
string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�
	string path = szFilePath;
	return path;
}
//============�ж��ļ��Ƿ����===========================
bool file_exists(std::string fn)
{
	return	std::experimental::filesystem::exists(fn);
}

//====��¼��־=====Ŀǰ��������
void addMyLog(int iLogLevel, string sMsg, string sFileName)
{
	return;//���ͬ����¼���ܲ���, �е���
	char* sTmp = (char*)malloc(sMsg.length() + 1);
	memset(sTmp, '\0', sMsg.length() + 1);
	memcpy(sTmp, sMsg.c_str(), sMsg.length());
	CHSLog::GetHSLog(sFileName)->addLog(iLogLevel, sTmp);//�����־��¼��ͬ���ģ���Ҫ�޸�Ϊ�첽��
	free(sTmp);
}

/**
* ʹ���øú������߳̽���˯��
* @param dwMilliseconds ��Ҫ˯�ߵĺ�����
* @return ��
*/
void sleepX(unsigned long dwMilliseconds)
{
#ifdef _WIN32
	Sleep(dwMilliseconds);
#else
	sleep(dwMilliseconds / 1000);
	usleep(dwMilliseconds % 1000 * 1000);
#endif
}

string GetKey(string str1, string str2)
{
	return str1 + "_" + str2;
}

// host long 64 to network
uint64 hl64ton(uint64 host)
{
	uint64 ret = 0;
	unsigned long high, low;
	low = (unsigned long)(host & 0xFFFFFFFF);
	high = (host >> 32) & 0xFFFFFFFF;
	low = htonl(low);
	high = htonl(high);
	ret = low;
	ret <<= 32;
	ret |= high;
	return   ret;
}

//network to host long 64
uint64 ntohl64(uint64 host)
{
	uint64   ret = 0;
	unsigned long high, low;
	low = host & 0xFFFFFFFF;
	high = (host >> 32) & 0xFFFFFFFF;
	low = ntohl(low);
	high = ntohl(high);
	ret = low;
	ret <<= 32;
	ret |= high;
	return   ret;
}

double GetBinPrice(uint64 iPrice)   //�۸�N13(4)
{
	return (double)iPrice / 10000;
}
double GetBinPriceItem(uint64 iPrice)  //�۸�N13(6)
{
	return (double)iPrice / 1000000;
}
double GetBinQty(uint64 iQty)//������N15(2)
{
	return (double)iQty / 100;
}

double GetBinAmt(uint64 iAmt) //��N18(4)
{
	return (double)iAmt / 10000;
}

string GETBINARY_STR(char* sourceData, int iSourceSize)
{
	char* destData = (char *)malloc(iSourceSize + 1);
	memset(destData, 0, iSourceSize + 1);
	memcpy(destData, sourceData, iSourceSize);
	string tmp = destData;
	free(destData);
	//ȥǰ��Ŀո�
	if (tmp.empty())
	{
		return tmp;
	}
	tmp.erase(0, tmp.find_first_not_of(" "));
	tmp.erase(tmp.find_last_not_of(" ") + 1);
	return tmp;
}

//�ַ����滻��ע�����12212�滻12Ϊ21������Ϊ21221
string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}
//��ȡ��������ʱ���ʽ YYYYMMDDHHMMSSsss
string GetLocalDateTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	int year, month, day, hour, min, sec;
	year = 1900 + timeinfo->tm_year;
	month = 1 + timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	hour = timeinfo->tm_hour;
	min = timeinfo->tm_min;
	sec = timeinfo->tm_sec;
	//printf("%4d%02d%02d-%02d:%02d:%02d.000", year, month, day, hour, min, sec);
	char buffer[128] = { 0 };
	sprintf(buffer, "%4d%02d%02d%02d%02d%02d000", year, month, day, hour, min, sec);
	return string(buffer);
}

//��ȡ���ڸ�ʽ YYYYMMDD
string GetLocalDate()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	int year, month, day;
	year = 1900 + timeinfo->tm_year;
	month = 1 + timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	char buffer[128] = { 0 };
	sprintf(buffer, "%4d%02d%02d", year, month, day);
	return string(buffer);
}

string GetLocalDateByTime(time_t aDay)
{
	struct tm * timeinfo;
	timeinfo = localtime(&aDay);
	int year, month, day;
	year = 1900 + timeinfo->tm_year;
	month = 1 + timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	char buffer[128] = { 0 };
	sprintf(buffer, "%4d%02d%02d", year, month, day);
	return string(buffer);
}

//��ȡʱ���ʽ HHMMSS
string GetLocalTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	int year, month, day, hour, min, sec;
	year = 1900 + timeinfo->tm_year;
	month = 1 + timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	hour = timeinfo->tm_hour;
	min = timeinfo->tm_min;
	sec = timeinfo->tm_sec;
	char buffer[128] = { 0 };
	sprintf(buffer, "%02d%02d%02d", hour, min, sec);
	return string(buffer);
}
// ������ϵͳ���������ĺ�������tick��
unsigned long GetMYTickCount()
{
#ifdef _WIN32
	return GetTickCount();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

void mytolower(char *s)
{
	size_t len = strlen(s);
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= 'A'&&s[i] <= 'Z')
		{
			s[i] = tolower(s[i]);
		}
	}
}
void mytoupper(char *s)
{
	size_t len = strlen(s);
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= 'a'&&s[i] <= 'z')
		{
			s[i] = toupper(s[i]);
		}
	}
}

std::string UnicodeToUTF8(const std::wstring & wstr)
{
	std::string ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.to_bytes(wstr);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return ret;
}

std::wstring UTF8ToUnicode(const std::string & str)
{
	std::wstring ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.from_bytes(str);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return ret;
}
std::wstring Utf8ToUnicode(const std::string &strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	if (len == 0)
	{
		return L"";
	}

	wchar_t *pRes = new wchar_t[len];
	if (pRes == NULL)
	{
		return L"";
	}

	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;

	return result;
}

std::string UnicodeToUtf8(const std::wstring &strUnicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		return "";
	}

	char *pRes = new char[len];
	if (pRes == NULL)
	{
		return "";
	}

	WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

std::wstring StringToWString(const std::string &str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (len == 0)
	{
		return L"";
	}

	wchar_t *pRes = new wchar_t[len];
	if (pRes == NULL)
	{
		return L"";
	}

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;

	return result;
}

std::string WStringToString(const std::wstring &wstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		return "";
	}

	char *pRes = new char[len];
	if (pRes == NULL)
	{
		return "";
	}

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

//UTF-8תUnicode 
std::wstring Utf82Unicode(const std::string& utf8string) {
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION) {
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0) {
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize) {
		throw std::exception("La falla!");
	}
	return std::wstring(&resultstring[0]);
}
//unicode תΪ ascii 
string WideByte2Acsi(wstring& wstrcode) {
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION) {
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0) {
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
	if (convresult != asciisize) {
		throw std::exception("La falla!");
	}
	return std::string(&resultstring[0]);
}
//utf-8 ת ascii 
string UTF_82ASCII(string& strUtf8Code) {
	string strRet("");
	//�Ȱ� utf8 תΪ unicode 
	wstring wstr = Utf82Unicode(strUtf8Code);
	//���� unicode תΪ ascii 
	strRet = WideByte2Acsi(wstr);
	return strRet;
}
//ascii ת Unicode 
wstring Acsi2WideByte(string& strascii) {
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION) {
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0) {
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize) {
		throw std::exception("La falla!");
	}
	return std::wstring(&resultstring[0]);
}
//Unicode ת Utf8 
std::string Unicode2Utf8(const std::wstring& widestring) {
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0) {
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(utf8size);
	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);
	if (convresult != utf8size) {
		throw std::exception("La falla!");
	}
	return std::string(&resultstring[0]);
}
//ascii ת Utf8 
string ASCII2UTF8(string& strAsciiCode) {
	string strRet("");
	//�Ȱ� ascii תΪ unicode 
	wstring wstr = Acsi2WideByte(strAsciiCode);
	//���� unicode תΪ utf8 
	strRet = Unicode2Utf8(wstr);
	return strRet;
}

