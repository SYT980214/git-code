/******************************************************************************
*
* 文 件 名：
* 作者：renzc
* 创建时间：2019-12
* ===========================================================================
*
* N O T E:
*           1. Platform: WINDOWS
*			2. 宏定义THREAD_COUNT控制读的线程个数
*			3. 简介:多线程读取，将符合的代码输出到临时的以证券代码
*					命名的txt中，然后读txt文件的数据进行解析并落地
*					注：解析到的除了当天的第一条其余都为增量行情，
*					所以解析时会通过放到map里来自行排序，key为写入文件时间。
******************************************************************************/
#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <io.h>
#include "ParseFile.h"
#include "HQStock.pb.h"
#include "threadsafequeue.h"
#include <thread>
#include <Windows.h>
#include <string>
#include "StructDefine.h"
#include "PublicFunc.h"
using namespace HQStockPackage;
using namespace std;
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")

#define BUF_SIZE 2048
#define THREAD_COUNT 5

struct HQStruct
{
	char sRecordTime[6];//交易市场
	HQ1008* lpStruct;//这个变更为protoBuf的类对象指针
};

vector<string> FilePaths;
threadsafe_queue<HQStruct1000*> HQStruct1000Queue;
threadsafe_queue<HQStruct*> HQ1008Queue;
map<string, HQStruct*> HQ1008Map;
int iDealFileCount = 0;//处理完文件的个数
bool bSortEnd = false;//用于判断是否排序完
bool bTranEnd = false;//用于判断是否转换完
bool bWriteEnd = false;//用于判断是否写完

CRITICAL_SECTION cs;

void getFiles(string FilePath, vector<string>& FilePaths)
{
	intptr_t hFile = 0;//文件句柄，过会儿用来查找
	struct _finddata_t FileInfo;//文件信息
	string p;
	string FullFilePath = "";//路径+文件名
	string TmpFullPath = "";
	if ((hFile = _findfirst(p.assign(FilePath).append("\\*").c_str(), &FileInfo)) != -1)
		//如果查找到第一个文件
	{
		do
		{
			if ((FileInfo.attrib &  _A_ARCH))// Archive file
			{
				FullFilePath = p.assign(FilePath).append("\\").append(FileInfo.name);
				//.dat文件会比.hs早放入队列，先判断一下是否已经存在解压完后的文件，如果存在就不放入队列了。
				if (FullFilePath.find(".hs") != string::npos)
				{
					replace_all(FullFilePath, TmpFullPath, ".hs", ".dat");
					if (! file_exists(TmpFullPath))
					{
						FilePaths.push_back(FullFilePath);
					}
				}
				else
				{
					FilePaths.push_back(FullFilePath);
				}
			}
		} while (_findnext(hFile, &FileInfo) == 0); //能寻找到其他文件

		_findclose(hFile);  //结束查找，关闭句柄
	}
}

void PackHqData(HQStruct1000* Struct1000, HQ1008* lpObj)
{
	memcpy(Struct1000->sMarketNo, lpObj->smarketno().c_str(), lpObj->smarketno().length());
	memcpy(Struct1000->sStockCode, lpObj->sstockcode().c_str(), lpObj->sstockcode().length());
	memcpy(Struct1000->sAbsoluteTimeStamp, lpObj->sabsolutetimestamp().c_str(),lpObj->sabsolutetimestamp().length());
	if (lpObj->has_dprecloseprice())
	{
		Struct1000->dPreclosePrice = lpObj->dprecloseprice();
	}
	if (lpObj->has_dopenprice())
	{
		Struct1000->dOpenPrice = lpObj->dopenprice();
	}
	if (lpObj->has_dlastprice())
	{
		Struct1000->dLastPrice = lpObj->dlastprice();
	}
	if (lpObj->has_dhighprice())
	{
		Struct1000->dHighPrice = lpObj->dhighprice();
	}
	if (lpObj->has_dlowprice())
	{
		Struct1000->dLowPrice = lpObj->dlowprice();
	}
	if (lpObj->has_dcloseprice())
	{
		Struct1000->dClosePrice = lpObj->dcloseprice();
	}
	if (lpObj->has_davgprice())
	{
		Struct1000->dAvgPrice = lpObj->davgprice();
	}
	Struct1000->dBusinessAmount = lpObj->dbusinessamount();
	if (lpObj->has_dbusinessbalance())
	{
		Struct1000->dBusinessBalance = lpObj->dbusinessbalance();
	}
	if (lpObj->has_dupprice())
	{
		Struct1000->dUpPrice = lpObj->dupprice();
	}
	if (lpObj->has_ddownprice())
	{
		Struct1000->dDownPrice = lpObj->ddownprice();
	}
	if (lpObj->has_dcurrentamount())
	{
		Struct1000->dCurrentAmount = lpObj->dcurrentamount();
	}
	if (lpObj->has_damount())
	{
		Struct1000->dAmount = lpObj->damount();
	}
	if (lpObj->has_dpreamount())
	{
		Struct1000->dPreAmount = lpObj->dpreamount();
	}
	if (lpObj->has_dpresettlement())
	{
		Struct1000->dPreSettlement = lpObj->dpresettlement();
	}
	if (lpObj->has_dsettlement())
	{
		Struct1000->dSettleMent = lpObj->dsettlement();
	}
	if (lpObj->has_diopv())
	{
		Struct1000->dIopv = lpObj->diopv();
	}
	if (lpObj->has_stradingphasecode())
	{
		memcpy(Struct1000->sTradingPhaseCode, lpObj->stradingphasecode().c_str(), lpObj->stradingphasecode().length());
	}
	if (lpObj->has_dauctionprice())
	{
		Struct1000->dAuctionPrice = lpObj->dauctionprice();
	}
	if (lpObj->has_dmatchedqty())
	{
		Struct1000->dMatchedQty = lpObj->dmatchedqty();
	}
	if (lpObj->has_dfundt1price())
	{
		Struct1000->dFundT1Price = lpObj->dfundt1price();
	}
	if (lpObj->has_sfundtype())
	{
		memcpy(Struct1000->sFundType, lpObj->sfundtype().c_str(), lpObj->sfundtype().length());
	}
	if (lpObj->has_dmarketdealamount())
	{
		Struct1000->dMarketDealAmount = lpObj->dmarketdealamount();
	}
	if (lpObj->has_sstopflag())
	{
		memcpy(Struct1000->sStopFlag, lpObj->sstopflag().c_str(), lpObj->sstopflag().length());
	}
	if (lpObj->has_sbsstatus())
	{
		memcpy(Struct1000->sBsStatus, lpObj->sbsstatus().c_str(), lpObj->sbsstatus().length());
	}
	if (lpObj->has_slatestenquirytime())
	{
		memcpy(Struct1000->sLatestEnquiryTime, lpObj->slatestenquirytime().c_str(), lpObj->slatestenquirytime().length());
	}
	if (lpObj->has_dbusinweight())
	{
		Struct1000->dBusinWeight = lpObj->dbusinweight();
	}
	if (lpObj->has_dbuyprice1())
	{
		Struct1000->dBuyPrice1 = lpObj->dbuyprice1();
	}
	if (lpObj->has_dbuyprice2())
	{
		Struct1000->dBuyPrice2 = lpObj->dbuyprice2();
	}
	if (lpObj->has_dbuyprice3())
	{
		Struct1000->dBuyPrice3 = lpObj->dbuyprice3();
	}
	if (lpObj->has_dbuyprice4())
	{
		Struct1000->dBuyPrice4 = lpObj->dbuyprice4();
	}
	if (lpObj->has_dbuyprice5())
	{
		Struct1000->dBuyPrice5 = lpObj->dbuyprice5();
	}
	if (lpObj->has_dsaleprice1())
	{
		Struct1000->dSalePrice1 = lpObj->dsaleprice1();
	}
	if (lpObj->has_dsaleprice2())
	{
		Struct1000->dSalePrice2 = lpObj->dsaleprice2();
	}
	if (lpObj->has_dsaleprice3())
	{
		Struct1000->dSalePrice3 = lpObj->dsaleprice3();
	}
	if (lpObj->has_dsaleprice4())
	{
		Struct1000->dSalePrice4 = lpObj->dsaleprice4();
	}
	if (lpObj->has_dsaleprice5())
	{
		Struct1000->dSalePrice5 = lpObj->dsaleprice5();
	}
	if (lpObj->has_dbuyqty1())
	{
		Struct1000->dBuyQty1 = lpObj->dbuyqty1();
	}
	if (lpObj->has_dbuyqty2())
	{
		Struct1000->dBuyQty2 = lpObj->dbuyqty2();
	}
	if (lpObj->has_dbuyqty3())
	{
		Struct1000->dBuyQty3 = lpObj->dbuyqty3();
	}
	if (lpObj->has_dbuyqty4())
	{
		Struct1000->dBuyQty4 = lpObj->dbuyqty4();
	}
	if (lpObj->has_dbuyqty5())
	{
		Struct1000->dBuyQty5 = lpObj->dbuyqty5();
	}
	if (lpObj->has_dsaleqty1())
	{
		Struct1000->dSaleQty1 = lpObj->dsaleqty1();
	}
	if (lpObj->has_dsaleqty2())
	{
		Struct1000->dSaleQty2 = lpObj->dsaleqty2();
	}
	if (lpObj->has_dsaleqty3())
	{
		Struct1000->dSaleQty3 = lpObj->dsaleqty3();
	}
	if (lpObj->has_dsaleqty4())
	{
		Struct1000->dSaleQty4 = lpObj->dsaleqty4();
	}
	if (lpObj->has_dsaleqty5())
	{
		Struct1000->dSaleQty5 = lpObj->dsaleqty5();
	}
	if (lpObj->has_sstockname())
	{
		memcpy(Struct1000->sStockName, lpObj->sstockname().c_str(), lpObj->sstockname().length());
	}
	if (lpObj->has_danountperhand())
	{
		Struct1000->dAnountPerHand = lpObj->danountperhand();
	}
	if (lpObj->has_dtotalbuyamount())
	{
		Struct1000->dTotalbuyAmount = lpObj->dtotalbuyamount();
	}
	if (lpObj->has_dtotalsaleamount())
	{
		Struct1000->dTotalSaleAmount = lpObj->dtotalsaleamount();
	}
	if (lpObj->has_dcjbishu())
	{
		Struct1000->dCJBiShu = lpObj->dcjbishu();
	}
	if (lpObj->has_drealavgrate())
	{
		Struct1000->dRealavgRate = lpObj->drealavgrate();
	}
	if (lpObj->has_dpreavgrate())
	{
		Struct1000->dPreavgRate = lpObj->dpreavgrate();
	}
	if (lpObj->has_iinitdate())
	{
		Struct1000->iInitDate = lpObj->iinitdate();
	}
	if (lpObj->has_dpe())
	{
		Struct1000->dPe = lpObj->dpe();
	}
	if (lpObj->has_dpe2())
	{
		Struct1000->dPe2 = lpObj->dpe2();
	}
	if (lpObj->has_ilayering())
	{
		Struct1000->iLayering = lpObj->ilayering();
	}
	if (lpObj->has_izsssl())
	{
		Struct1000->iZSSSL = lpObj->izsssl();
	}
	if (lpObj->has_sstocktype())
	{
		memcpy(Struct1000->sStockType, lpObj->sstocktype().c_str(), lpObj->sstocktype().length());
	}
	if (lpObj->has_scodetype())
	{
		memcpy(Struct1000->sCodeType, lpObj->scodetype().c_str(), lpObj->scodetype().length());
	}
	if (lpObj->has_istrikedate())
	{
		Struct1000->iStrikeDate = lpObj->istrikedate();
	}
	if (lpObj->has_iexpiredate())
	{
		Struct1000->iExpireDate = lpObj->iexpiredate();
	}
	if (lpObj->has_dmaxlimitedamount())
	{
		Struct1000->dMaxLimitedAmount = lpObj->dmaxlimitedamount();
	}
	if (lpObj->has_dmaxmarketamount())
	{
		Struct1000->dMaxMarketAmount = lpObj->dmaxmarketamount();
	}
	if (lpObj->has_dholdbalance())
	{
		Struct1000->dHoldBalance = lpObj->dholdbalance();
	}
	if (lpObj->has_ddelta())
	{
		Struct1000->dDelta = lpObj->ddelta();
	}
}

void ParseQuotation(vector<string> FilePaths,int iPos, string sSecurityID, string OutFile)
{
	int iFileCount = FilePaths.size();
	for (int i = iPos; i < iFileCount; i = i + THREAD_COUNT)
	{
		string sFileName = FilePaths[i];
		int iReadCount = 0;
		if (sFileName.find(".hs") != string::npos)//处理.hs文件
		{

		}
		else if (sFileName.find(".dat") != string::npos)//处理.dat文件
		{
			cout << GetLocalTime() << " 开始读取文件[" << sFileName << "]" << endl;			
			char buf[BUF_SIZE];
			memset(buf, '\0', BUF_SIZE);
			FILE *fp = fopen(sFileName.c_str(), "r");
			FILE *fp2 = fopen(OutFile.c_str(), "a+");
			
			while (fgets(buf, BUF_SIZE, fp) != NULL)
			{
				char *ptr = buf;
				ptr += 10;//避免取到时间				
				if (strstr(ptr, sSecurityID.c_str()))
				{
					EnterCriticalSection(&cs);
					fseek(fp2, 0, ios::end);
					fputs(buf, fp2);
					LeaveCriticalSection(&cs);
				}
				memset(buf, '\0', BUF_SIZE);
			}
			fclose(fp);
			fclose(fp2);
		}
		cout << GetLocalTime() << " 文件[" << sFileName << "]读取完毕" << endl;
		iDealFileCount++;
	}
}

void UnZipFiles(vector<string>& FilePaths, string Quotation_Path)
{
	string sExePath = GetExePath();//获取当前exe所在路径，rar.exe和他同目录
	int FileCount = FilePaths.size();
	for (int i = 0; i < FileCount; i++)
	{
		if (FilePaths[i].find(".hs") != string::npos)//处理.hs文件
		{
			string str = "\"" + sExePath + "\\Rar.exe\" e -y \"" + FilePaths[i] + "\" \"" + Quotation_Path + "\"";
			string sRarBat = Quotation_Path + "\\unzip.bat";
			ofstream f1(sRarBat);
			if (!f1)
				return;
			f1 << str << endl;
			f1.close();
			string tmp = "cmd /c " + sRarBat;
			WinExec(tmp.c_str(), SW_HIDE);
			int length = FilePaths[i].length();
			string sFilename = FilePaths[i].substr(0, length - 3) + ".dat";
			while (true)
			{
				if ((_access(sFilename.c_str(), 0)) == 0)//.dat文件存在说明解压成功
				{
					remove(sRarBat.c_str());//删除bat文件
					FilePaths[i] = sFilename;
					break;
				}
				Sleep(1000);
			}
		}
	}
}

void TranQuotation()//解析行情线程
{
	HQStruct1000* oldHqStock = new HQStruct1000;
	memset(oldHqStock, '\0', sizeof(HQStruct1000));
	cout << GetLocalTime() <<" 行情解析开始" << endl;
	while (true)
	{
		HQStruct* hqstock = HQ1008Queue.try_pop();
		if (NULL != hqstock)
		{
			HQStruct1000* newHqStock = new HQStruct1000;
			memset(newHqStock, '\0', sizeof(HQStruct1000));
			memcpy(newHqStock, oldHqStock, sizeof(HQStruct1000));//先拷贝一份全量的
			memcpy(newHqStock->sRecordTime, hqstock->sRecordTime, sizeof(hqstock->sRecordTime));
			PackHqData(newHqStock, hqstock->lpStruct);
			memcpy(oldHqStock, newHqStock, sizeof(HQStruct1000));//将变更后的值再拷到老的那一个结构体里
			HQStruct1000Queue.push(newHqStock);
			delete hqstock->lpStruct;
			hqstock->lpStruct = NULL;
			delete hqstock;
			hqstock = NULL;
		}
		else
		{
			if (bSortEnd)
			{
				delete oldHqStock;
				oldHqStock = NULL;
				bTranEnd = true;
				cout << GetLocalTime() <<" 数据解析完毕" << endl;
				break;
			}
		}
		Sleep(1);
	}
}

void WriteExcel(string sSecurityID)//落EXCEL线程
{
	cout << GetLocalTime() << " 行情落地开始" << endl;
	ofstream oFile;
	string sFullFileName = GetExePath();
	string sFileName = sSecurityID + ".csv";
	sFullFileName = sFullFileName + "\\" + sFileName;
	oFile.open(sFullFileName, ios::out | ios::trunc);
	oFile  << "文件时间戳" << "," << "交易市场" << "," << "证券代码" << "," << "证券名称" << "," << "绝对时间戳" << "," << "昨收盘" << "," << "今开盘" << "," << "最新价" << "," << "最高价" << "," << "最低价" << "," << "今收盘" << "," << "均价" << "," << "总成交量" << "," << "总成交金额" << "," << "涨停价" << "," << "跌停价" << "," << "现手" << "," << "持仓量" << "," << "昨持仓量" << "," << "昨结算" << "," << "结算价" << "," << "iopv" << "," << "产品实施标志(市场交易状态)" << "," << "动态参考价格" << "," << "虚拟匹配数量" << "," << "T-1日基金净值" << "," << "基金类型" << "," << "市场成交量" << "," << "停牌标志" << "," << "买卖标志" << "," << "最近询价时间" << "," << "成交重量（双边）" << "," << "买一价" << "," << "买二价" << "," << "买三价" << "," << "买四价" << "," << "买五价" << "," << "卖一价" << "," << "卖二价" << "," << "卖三价" << "," << "卖四价" << "," << "卖五价" << "," << "买一量" << "," << "买二量" << "," << "买三量" << "," << "买四量" << "," << "买五量" << "," << "卖一量" << "," << "卖二量" << "," << "卖三量" << "," << "卖四量" << "," << "卖五量" << "," << "每手股数" << "," << "总买量" << "," << "总卖量" << "," << "成交笔数" << "," << "实时加权平均利率" << "," << "昨收盘加权平均利率" << "," << "初始化日期" << "," << "静态市盈率" << "," << "动态市盈率" << "," << "股转分层信息" << "," << "股转做市商数量" << "," << "证券类别" << "," << "代码分类" << "," << "开始行权日" << "," << "行权到期日" << "," << "限价申报上限" << "," << "市价申报上限" << "," << "开仓最小保证金" << "," << "风险值" << endl;
	oFile.close();
	while (true)
	{
		HQStruct1000* newHQStruct1000 = HQStruct1000Queue.try_pop();
		if (NULL != newHQStruct1000)
		{
			oFile.open(sFullFileName, ios::out | ios::app);//追加内容
			oFile << newHQStruct1000->sRecordTime << "," <<newHQStruct1000->sMarketNo << "," << newHQStruct1000->sStockCode << "," << newHQStruct1000->sStockName << "," << newHQStruct1000->sAbsoluteTimeStamp << "," << newHQStruct1000->dPreclosePrice << "," << newHQStruct1000->dOpenPrice\
				<< "," << newHQStruct1000->dLastPrice << "," << newHQStruct1000->dHighPrice << "," << newHQStruct1000->dLowPrice << "," << newHQStruct1000->dClosePrice << "," << newHQStruct1000->dAvgPrice << "," << newHQStruct1000->dBusinessAmount\
				<< "," << newHQStruct1000->dBusinessBalance << "," << newHQStruct1000->dUpPrice << "," << newHQStruct1000->dDownPrice << "," << newHQStruct1000->dCurrentAmount << "," << newHQStruct1000->dAmount << "," << newHQStruct1000->dPreAmount\
				<< "," << newHQStruct1000->dPreSettlement << "," << newHQStruct1000->dSettleMent << "," << newHQStruct1000->dIopv << "," << newHQStruct1000->sTradingPhaseCode << "," << newHQStruct1000->dAuctionPrice << "," << newHQStruct1000->dMatchedQty\
				<< "," << newHQStruct1000->dFundT1Price << "," << newHQStruct1000->sFundType << "," << newHQStruct1000->dMarketDealAmount << "," << newHQStruct1000->sStopFlag << "," << newHQStruct1000->sBsStatus << "," << newHQStruct1000->sLatestEnquiryTime\
				<< "," << newHQStruct1000->dBusinWeight << "," << newHQStruct1000->dBuyPrice1 << "," << newHQStruct1000->dBuyPrice2 << "," << newHQStruct1000->dBuyPrice3 << "," << newHQStruct1000->dBuyPrice4 << "," << newHQStruct1000->dBuyPrice5\
				<< "," << newHQStruct1000->dSalePrice1 << "," << newHQStruct1000->dSalePrice2 << "," << newHQStruct1000->dSalePrice3 << "," << newHQStruct1000->dSalePrice4 << "," << newHQStruct1000->dSalePrice5 << "," << newHQStruct1000->dBuyQty1\
				<< "," << newHQStruct1000->dBuyQty2 << "," << newHQStruct1000->dBuyQty3 << "," << newHQStruct1000->dBuyQty4 << "," << newHQStruct1000->dBuyQty5 << "," << newHQStruct1000->dSaleQty1 << "," << newHQStruct1000->dSaleQty2 << "," << newHQStruct1000->dSaleQty3\
				<< "," << newHQStruct1000->dSaleQty4 << "," << newHQStruct1000->dSaleQty5 << "," << newHQStruct1000->dAnountPerHand << "," << newHQStruct1000->dTotalbuyAmount << "," << newHQStruct1000->dTotalSaleAmount\
				<< "," << newHQStruct1000->dCJBiShu << "," << newHQStruct1000->dRealavgRate << "," << newHQStruct1000->dPreavgRate << "," << newHQStruct1000->iInitDate << "," << newHQStruct1000->dPe << "," << newHQStruct1000->dPe2 << "," << newHQStruct1000->iLayering\
				<< "," << newHQStruct1000->iZSSSL << "," << newHQStruct1000->sStockType << "," << newHQStruct1000->sCodeType << "," << newHQStruct1000->iStrikeDate << "," << newHQStruct1000->iExpireDate << "," << newHQStruct1000->dMaxLimitedAmount\
				<< "," << newHQStruct1000->dMaxMarketAmount << "," << newHQStruct1000->dHoldBalance << "," << newHQStruct1000->dDelta << endl;
			oFile.close();
			delete newHQStruct1000;
			newHQStruct1000 = NULL;
		}
		else
		{
			if (bTranEnd)
			{
				cout << GetLocalTime() <<" 数据落地完毕" << endl;
				bWriteEnd = true;
				break;
			}
				
		}
		Sleep(1);
	}
}

void SortFileData(string sFileName)
{
	ParseFile lpParseFile(sFileName);
	int iRet = lpParseFile.Open();
	if (iRet == 0)
	{
		while (true)
		{
			try
			{
				iRet = lpParseFile.ReadLn();
				if (iRet == 0)
				{
					string msg = hexStrToByte(lpParseFile.CurrReadData.HexData, sizeof(lpParseFile.CurrReadData.HexData));
					HQ1008* hqstock = new HQ1008;
					hqstock->ParseFromArray(msg.c_str(), msg.length());
					HQStruct* newStruct = new HQStruct;
					memset(newStruct, '\0', sizeof(HQStruct));
					memcpy(newStruct->sRecordTime, lpParseFile.CurrReadData.LocalTime, sizeof(lpParseFile.CurrReadData.LocalTime));
					newStruct->lpStruct = hqstock;
					EnterCriticalSection(&cs);
					HQ1008Map.insert(pair<string, HQStruct*>(newStruct->sRecordTime, newStruct));
					LeaveCriticalSection(&cs);				
				}
				else if(iRet == 2)//读到最后一行才退出
					break;
			}
			catch (exception& e)
			{
				cout << e.what() << endl;
			}
		}
	}
	lpParseFile.Close();
	map<string, HQStruct*>::iterator iterHQ1008Map = HQ1008Map.begin();
	while (iterHQ1008Map != HQ1008Map.end())
	{
		HQ1008Queue.push(iterHQ1008Map->second);
		iterHQ1008Map++;
	}
	bSortEnd = true;
}


int main()
{
	string sFuncNo;
	char cFuncNo;
	string Quotation_Path = "";
	string sSecurityID = "";
	InitializeCriticalSection(&cs);
	while (1)
	{
		cout << "请选择:" << endl
			<< "1:行情解析(目前只支持1000主题的)" << endl
			<< "q:退出" << endl;
		cin >> sFuncNo;
		cFuncNo = sFuncNo[0];
		if (cFuncNo == '1')
		{
			//设置让用户输入路径，然后进去后先获取行情的主题
			cout << "请输入要解析行情的路径:";
			cin >> Quotation_Path;
			cout << endl;
			cout << "请输入要解析的证券代码:";
			cin >> sSecurityID;
			string stmpSecurityID = "[" + sSecurityID + "]";
			string sFullFileName = GetExePath();
			string stmpFileName = sSecurityID + ".txt";
			sFullFileName = sFullFileName + "\\" + stmpFileName;
			try
			{
				std::thread threads[1024];//先定义一个大一点的线程数组
				getFiles(Quotation_Path, FilePaths);
				UnZipFiles(FilePaths, Quotation_Path);
				int iFileCount = FilePaths.size();
				for (int i = 0; i < THREAD_COUNT; i++)
				{
					threads[i] = std::thread(ParseQuotation, FilePaths, i, stmpSecurityID, sFullFileName);
					//Sleep(10);
				}
				while (true)
				{
					if (iDealFileCount == iFileCount)
					{
						iDealFileCount = 0;
						break; 
					}
					Sleep(1000);
				}
				for (int i = 0; i < THREAD_COUNT; i++)
				{
					if (threads[i].joinable())
					{
						threads[i].join();
					}
				}
				std::thread threadSort = std::thread(SortFileData, sFullFileName);
				std::thread threadTran = std::thread(TranQuotation);
				Sleep(10);
				std::thread threadWrite = std::thread(WriteExcel, sSecurityID);
				if(threadSort.joinable())
					threadSort.join();
				if(threadTran.joinable())
					threadTran.join();
				if(threadWrite.joinable())
					threadWrite.join();
				//执行写库
				FilePaths.clear();
				HQ1008Map.clear();
				while (!bWriteEnd)
				{
					Sleep(100);
				}
				bSortEnd = false;
				bTranEnd = false;
				bWriteEnd = false;
				if (file_exists(sFullFileName))
				{
					remove(sFullFileName.c_str());
				}
			}
			catch (exception& e)
			{
				cout << e.what() << endl;
			}
		}
		else if (cFuncNo == 'q' || cFuncNo == 'Q')
		{
			break;
		}
	}
	DeleteCriticalSection(&cs);
	//system("pause>nul");
    return 0;
}

