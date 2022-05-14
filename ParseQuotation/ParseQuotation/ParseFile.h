/******************************************************************************
 *
 * �� �� ����
 * ģ�����ƣ��ļ���ȡ��װ
 * ���ߣ�renzc
 * ����ʱ�䣺2019-12
 * ===========================================================================
 *
 * N O T E:
 *           1. Platform: WINDOWS
 *
******************************************************************************/
#ifndef _C_ParseFile_H_
#define _C_ParseFile_H_

#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <map>
#include <stdio.h>
#include<sstream>
#include<algorithm>
#include <vector>

using namespace std;

//char str[20] = "0123456789"; 
//int a = strlen(str); //a=10;strlen �����ַ����ĳ��ȣ���\0'Ϊ�ַ���������ǡ� 
//int b=sizeof(str); //b=20;sizeof ��������Ƿ��������str[20] ��ռ���ڴ�ռ�Ĵ�С����������洢������Ӱ��

//strucrt �ṹ�嶨���������������string

struct CurrentData
{
	char LocalTime[6 + 1];//ʱ��
	char Topic[12 + 1];//��������
	char Market[6];//�г�
	char SecurityID[6+1];//��Ʒ����
	double DataLength;//������
	char HexData[2056];//ʮ����������
};

class ParseFile
{
private:
	string sFilePath;
	map<int, string> DataStrList;
	int CurrReadPos;
	bool bQtEnd;
public:
	ParseFile(string sFilePath);
	~ParseFile();
public:
	CurrentData CurrReadData;
	int Open();
	void Close();
	int ReadLn();
};

#endif

