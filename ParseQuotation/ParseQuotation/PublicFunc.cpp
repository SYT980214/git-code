#include "PublicFunc.h"


//=============获取当前运行目录，需要再加\\=====================
string GetExePath(void)
{
	char exeFullPath[MAX_PATH]; // Full path   
	string strPath = "";
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = (string)exeFullPath;    // Get full path of the file   
	int pos = strPath.find_last_of('\\', strPath.length());

	return strPath.substr(0, pos);  // Return the directory without the file name   
}

string inttostr(int iSource)
{
	stringstream ss;
	ss.str("");
	ss << iSource;
	return ss.str();
}

//============判断文件是否存在===========================
bool file_exists(std::string fn)
{
	return	std::experimental::filesystem::exists(fn);
}

void replace_all(string str, string& result_str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	result_str = str;
}

//字符串转byte数组，就是实现如下功能：把一个字符串，比如“E4F1C3A81F”每两位作为一个十六进制数
//存到一个unsigned char 型的数组(buf)里，buf[0] = 0xE4, buf[1] = 0xF1, buf[2] = 0xC3, buf[3] = 0xA8......。
//我们知道字符串中的字符在计算机中大小是其ASCII码值，比如E的ASCII值是69，但是E如果作为一个十六进制数，
//在计算机中的大小是14，所以我们需要找到一种对应关系将字母E(69)->14，同样将第二位的数字字符4(ASCII:52)->4，
//并将转换后的14放到高4位，4放到低4位即可。
std::string hexStrToByte(const char* hexstr, int len)
{
	std::string byteString;
	unsigned char bits;
	for (int i = 0; nullptr != hexstr && i < len; i += 2)
	{
		if (hexstr[i] >= 'A' && hexstr[i] <= 'F')
			bits = hexstr[i] - 'A' + 10;
		else
			bits = hexstr[i] - '0';
		if (hexstr[i + 1] >= 'A' && hexstr[i + 1] <= 'F')
			bits = (bits << 4) | (hexstr[i + 1] - 'A' + 10);
		else
			bits = (bits << 4) | (hexstr[i + 1] - '0');
		byteString.push_back(bits);
	}
	return byteString;
}
//获取完整日期时间格式 YYYYMMDDHHMMSSsss
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

//获取日期格式 YYYYMMDD
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

//获取时间格式 HHMMSS
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