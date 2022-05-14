#include "ParseFile.h"

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

void static str2Double(string sTmp, double& dTmp)//���֧��6λС������float�Ļ�ֻ��֧��3λС�����ϵ㿴���ھ��ȶ�ʧ���⣬��ʵ�ʴ�ӡ������������
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
	if (iSize > 0)//��ȡʧ�ܵģ��ⲿ��Ӧ���ٶ�ȡ
	{
		char* buf = new char[iSize + 1];
		if (buf == NULL)
		{
			printf("file read new char error \n");
			return -1;
		}
		memset(buf, '\0', iSize + 1);
		fin.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(iSize));//������ص��ڴ�ֻ��Ҫ0.005s
		fin.close();
		int cnt = 0;
		char * pch;
		pch = strtok(buf, "\n");//��ʱ0.015s���ɽ���
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
		//��Ϊ���ڶ�ȡ���Ǿ�̬�ļ����Կ��ܴ���isize��0��������Ͳ���ʾ����Ϣ��
		//printf("file read size error \n");//�ļ��滻ģʽ�£�������ʺܸߣ��Ӹ��жϣ��´ζ�ȡ������
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
		TempStr = DataStrList[CurrReadPos].substr(1, length -3);//���ݸ�ʽΪ[][][][][]���˴�ȥ��ͷ��β֮���á�][�����зָ� (Ϊʲô��length-3,��Ϊȥ����ͷ��[��length���-1���ַ�����β�и�\r��Ҳȥ����-2����ȥ��]�ͱ��-3)
		LastReadPos = CurrReadPos;
		CurrReadPos++;
	}
	else
	{
		TempStr = "";  //�����ļ���Χ�����ļ�β�����˳�
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
