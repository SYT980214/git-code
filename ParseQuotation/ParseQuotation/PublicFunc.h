#ifndef PUBLICFUNC_H
#define PUBLICFUNC_H

#include<iostream>
#include<string>
#include <Windows.h>
#include <sstream>
#include <filesystem>
using namespace std;

//=============��ȡ��ǰ����Ŀ¼����Ҫ�ټ�\\=====================
string GetExePath(void);

// intתstring
string inttostr(int iSource);

//============�ж��ļ��Ƿ����===========================
bool file_exists(std::string fn);

//�ַ����滻
void replace_all(string str, string& result_str, const string& old_value, const string& new_value);

//�ַ���תbyte���飬����ʵ�����¹��ܣ���һ���ַ��������硰E4F1C3A81F��ÿ��λ��Ϊһ��ʮ��������
//�浽һ��unsigned char �͵�����(buf)�buf[0] = 0xE4, buf[1] = 0xF1, buf[2] = 0xC3, buf[3] = 0xA8......��
//����֪���ַ����е��ַ��ڼ�����д�С����ASCII��ֵ������E��ASCIIֵ��69������E�����Ϊһ��ʮ����������
//�ڼ�����еĴ�С��14������������Ҫ�ҵ�һ�ֶ�Ӧ��ϵ����ĸE(69)->14��ͬ�����ڶ�λ�������ַ�4(ASCII:52)->4��
//����ת�����14�ŵ���4λ��4�ŵ���4λ���ɡ�
std::string hexStrToByte(const char* hexstr, int len);

//��ȡ��������ʱ���ʽ YYYYMMDDHHMMSSsss
string GetLocalDateTime();
//��ȡ���ڸ�ʽ YYYYMMDD
string GetLocalDate();
string GetLocalDateByTime(time_t aDay);
//��ȡʱ���ʽ HHMMSS
string GetLocalTime();
#endif