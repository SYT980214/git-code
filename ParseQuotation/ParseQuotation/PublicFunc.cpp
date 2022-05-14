#include "PublicFunc.h"


//=============��ȡ��ǰ����Ŀ¼����Ҫ�ټ�\\=====================
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

//============�ж��ļ��Ƿ����===========================
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

//�ַ���תbyte���飬����ʵ�����¹��ܣ���һ���ַ��������硰E4F1C3A81F��ÿ��λ��Ϊһ��ʮ��������
//�浽һ��unsigned char �͵�����(buf)�buf[0] = 0xE4, buf[1] = 0xF1, buf[2] = 0xC3, buf[3] = 0xA8......��
//����֪���ַ����е��ַ��ڼ�����д�С����ASCII��ֵ������E��ASCIIֵ��69������E�����Ϊһ��ʮ����������
//�ڼ�����еĴ�С��14������������Ҫ�ҵ�һ�ֶ�Ӧ��ϵ����ĸE(69)->14��ͬ�����ڶ�λ�������ַ�4(ASCII:52)->4��
//����ת�����14�ŵ���4λ��4�ŵ���4λ���ɡ�
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