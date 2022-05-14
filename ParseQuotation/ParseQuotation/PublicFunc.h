#ifndef PUBLICFUNC_H
#define PUBLICFUNC_H

#include<iostream>
#include<string>
#include <Windows.h>
#include <sstream>
#include <filesystem>
using namespace std;

//=============获取当前运行目录，需要再加\\=====================
string GetExePath(void);

// int转string
string inttostr(int iSource);

//============判断文件是否存在===========================
bool file_exists(std::string fn);

//字符串替换
void replace_all(string str, string& result_str, const string& old_value, const string& new_value);

//字符串转byte数组，就是实现如下功能：把一个字符串，比如“E4F1C3A81F”每两位作为一个十六进制数
//存到一个unsigned char 型的数组(buf)里，buf[0] = 0xE4, buf[1] = 0xF1, buf[2] = 0xC3, buf[3] = 0xA8......。
//我们知道字符串中的字符在计算机中大小是其ASCII码值，比如E的ASCII值是69，但是E如果作为一个十六进制数，
//在计算机中的大小是14，所以我们需要找到一种对应关系将字母E(69)->14，同样将第二位的数字字符4(ASCII:52)->4，
//并将转换后的14放到高4位，4放到低4位即可。
std::string hexStrToByte(const char* hexstr, int len);

//获取完整日期时间格式 YYYYMMDDHHMMSSsss
string GetLocalDateTime();
//获取日期格式 YYYYMMDD
string GetLocalDate();
string GetLocalDateByTime(time_t aDay);
//获取时间格式 HHMMSS
string GetLocalTime();
#endif