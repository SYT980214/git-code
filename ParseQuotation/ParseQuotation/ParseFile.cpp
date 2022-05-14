#include "ParseFile.h"

//=================字符串分割=========================
vector<string> split2(string str, char del) //忽略“空位”（::192:168:ABC::416）->（192 168 ABC 416）
{
	string tmp = str;
	stringstream ss(tmp);
	string tok;
	vector<string> ret;
	while (getline(ss, tok, del))
		ret.push_back(tok);
	return ret;
}

void static str2Double(string sTmp, double& dTmp)//最多支持6位小数，用float的话只能支持3位小数，断点看存在精度丢失问题，但实际打印出来是正常的
{
	sscanf(sTmp.c_str(), "%lf", &dTmp);
}


ParseFile::ParseFile(string sFilePath)
{
	this->sFilePath = sFilePath;
}

ParseFile::~ParseFile()
{
}

int ParseFile::Open()
{
	ifstream fin(sFilePath, std::ios::binary);
	streamoff iSize = fin.seekg(0, std::ios::end).tellg();
	if (iSize > 0)//读取失败的，外部不应该再读取
	{
		char* buf = new char[iSize + 1];
		if (buf == NULL)
		{
			printf("file read new char error \n");
			return -1;
		}
		memset(buf, '\0', iSize + 1);
		fin.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(iSize));//这个加载到内存只需要0.005s
		fin.close();
		int cnt = 0;
		char * pch;
		pch = strtok(buf, "\n");//耗时0.015s，可接受
		while (pch != NULL)
		{
			string tmp = pch;
			map<int, string>::iterator iter = DataStrList.find(cnt);
			if (iter != DataStrList.end())
				iter->second = tmp;
			else
				DataStrList.insert(pair<int, string>(cnt, tmp));
			cnt++;
			pch = strtok(NULL, "\n");
		}
		if (cnt <= 0)
		{
			printf("file read none error \n");
			return -1;
		}
		delete[] buf;
		CurrReadPos = 0;
		return 0;
	}
	else
	{
		//因为现在读取的是静态文件所以可能存在isize是0的情况，就不提示该信息了
		//printf("file read size error \n");//文件替换模式下，这个概率很高，加个判断，下次读取就正常
		return -1;
	}
		
}

void ParseFile::Close()
{
	DataStrList.erase(DataStrList.begin(), DataStrList.end());
	CurrReadPos = 0;
}

int ParseFile::ReadLn()
{
	int LastReadPos = 0;
	string TempStr = "";
	string LastStr = "";
	if ((CurrReadPos >= 0) && (CurrReadPos <= DataStrList.size() - 1))
	{
		int length = DataStrList[CurrReadPos].length();
		TempStr = DataStrList[CurrReadPos].substr(1, length -3);//数据格式为[][][][][]，此处去掉头和尾之后用“][”进行分割 (为什么是length-3,因为去掉了头的[，length变成-1，字符串结尾有个\r，也去掉就-2，再去掉]就变成-3)
		LastReadPos = CurrReadPos;
		CurrReadPos++;
	}
	else
	{
		TempStr = "";  //超出文件范围返回文件尾，并退出
		return 2;
	}
	if (TempStr != "")
	{
		vector<string> aHqlist = split2(TempStr, '[');
		if (aHqlist.size() <= 1)
		{
			aHqlist.clear();
			return 2;
		}
		memset(&CurrReadData, '\0', sizeof(CurrentData));
		memcpy(CurrReadData.LocalTime, aHqlist[1].c_str(), aHqlist[1].length());
		if (!(aHqlist.size() >= 7))
		{
			aHqlist.clear();
			return 3;
		}
		try
		{
			memcpy(CurrReadData.Topic, aHqlist[2].c_str(), aHqlist[2].length() - 1);
			memcpy(CurrReadData.Market, aHqlist[3].c_str(), aHqlist[3].length() - 1);
			memcpy(CurrReadData.SecurityID, aHqlist[4].c_str(), aHqlist[4].length() - 1);
			str2Double(aHqlist[5], CurrReadData.DataLength);
			memcpy(CurrReadData.HexData, aHqlist[6].c_str(), aHqlist[6].length());
		}
		catch (...)
		{
			aHqlist.clear();
			return 3;
		}
		aHqlist.clear();
		return 0;
	}
}
