/******************************************************************************
 *
 * 文 件 名：
 * 模块名称：文件读取封装
 * 作者：renzc
 * 创建时间：2019-12
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
//int a = strlen(str); //a=10;strlen 计算字符串的长度，以\0'为字符串结束标记。 
//int b=sizeof(str); //b=20;sizeof 计算的则是分配的数组str[20] 所占的内存空间的大小，不受里面存储的内容影响

//strucrt 结构体定义变量都不允许用string

struct CurrentData
{
	char LocalTime[6 + 1];//时间
	char Topic[12 + 1];//行情主题
	char Market[6];//市场
	char SecurityID[6+1];//产品代码
	double DataLength;//包长度
	char HexData[2056];//十六进制数据
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

