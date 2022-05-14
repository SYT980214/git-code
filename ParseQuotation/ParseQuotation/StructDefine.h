#ifndef _STRUCT_DEFINE_H_
#define _STRUCT_DEFINE_H_

#include <string>
#include <cstdint>
#include <mutex>
using namespace std;


#ifdef _WIN32

typedef __int16 Int16;
typedef unsigned __int16 uInt16;
typedef unsigned __int32 uInt32;
typedef unsigned __int64 uint64;
typedef __int32	Int32;
typedef __int64 int64;

#else

typedef int16_t Int16;
typedef uint16_t uInt16;
typedef uint32_t uInt32;
typedef uint64_t uint64;
typedef int32_t	Int32;
typedef int64_t int64;
typedef unsigned char BYTE;

#endif

#define Const_Market_sh  "1" //�Ϻ�
#define Const_Market_shggt  "n"//����ͨ
#define Const_Market_sz  "2" //����
#define Const_Market_szggt  "o"//���ͨ
#define Const_Market_SQ  "3"; //������
#define Const_Market_DS  "9"; //������
#define Const_Market_ZS  "4"; //֣����
#define Const_Market_ZJ  "7"; //�н���
#define Const_Market_NY  "k"; //�ܽ���
#define Const_Market_GZ  "j" //��ת�г�
#define Const_Market_ShGdsy  "1547" //�Ϻ��̶�����

//��Ч�۸���
#define Const_Nil_ArbiPrice  -9999999999.0
#define Const_Nil_Price  0.0
#define Const_Up_NilArbiPrice  9999999999.0
#define Const_Down_NilArbiPrice  -9999999999.0
#define Const_Up_NilPrice  0.0000000001
#define Const_Down_NilPrice  -0.0000000001
//��Ч����ֵ����
#define Const_Up_NilBalance  9999999999999999999.0
#define Const_Nil_Balance  0.0

#define Const_SnapshotQuotationPlatfrom "A"; //ʵʱƽ̨
#define Const_RequestQuotationPlatfrom "B"; //�ش�ƽ̨


//�ṹ�嶨���У�������ʹ��string��Ϊ�������ͣ�������memset����zeromemory��ʱ�������ڴ�й©
struct HQStructTimeSection
{
	int iBeginTime;
	int iEndTime;
	//���漸�����ڻ���
	char cMarketNo[2 + 1];
	char sMarketName[100];//֤ȯ����
	int iIndex;
};

struct HQStructTaskData
{
	int iBeginTime;
	int iEndTime;
	int iBeginTime2;
	int iEndTime2;
	bool bNightRun;
};

//�н�����Ϻ�Լ�����Ⱥ�Լ����
struct HQStructContractDetail
{
	bool bFuture; 		//�Ƿ��ڻ�
	bool bOptionCall; 	//�Ƿ�����Ȩ
	char sKind[10];     //Ʒ������
	char sDate[10];
	int iDate;//ת��Ϊint
	char strikePrice[50];
	int iStrikePrice;//ת��Ϊint
};

//�ڻ�Ʒ��
struct HQStructFutureKind
{
	char C_MARKET_NO[2 + 1];
	char VC_KIND_CODE[10 + 1];
	int L_MULTIPLICATOR;
	char VC_TRADE_MONTH[64 + 1];
	char VC_CONTRACT_RULE[20 + 1];
	char C_STOCK_TYPE[2 + 1];
};

//֤ȯģ��
struct HQStructStockModel
{
	char sMainKey[100];//����������������Ϊ market_no+_+stockcode_Mask+_+stock_name_mask
	char c_market_no[1 + 1];
	char c_stock_type[2 + 1];
	char vc_stockcode_mask[20 + 1];
	char vc_stock_name_mask[40 + 1];
	double en_uplimited_ratio;
	double en_downlimited_ratio;
	char vc_contractid_id_mask[19 + 1];
	char c_is_issuecode[1 + 1];
	int l_amount_per_hand;
	double en_mindifference_price;
};

//�Ϻ���Ʒ��Ϣcpxx
struct HQStructShCpxx
{
	char sMainKey[30];//����������������Ϊ �г�+_+����,���� "1_600570"
	char sMarketNo[6];//�����г� 
	char sStockCode[20];//֤ȯ����
	char sStockName[100];//֤ȯ����
	double dUpPrice;//��ͣ��   
	double dDownPrice;//��ͣ�� 
	char sFundType[2];//��������
	char sStockType[2];//֤ȯ����
	char sCodeType[7];//�������
	int iAmountPerHand;//ÿ�ֹ��� //����ֶ��Ǵ�ģ����ƥ��ģ��״�Ϊ0//��cpxx�л�ȡ, ծȯд��Ϊ10
	double dPreclosePrice;//������   һ�㲻��cpxx�е�������
};

//�Ϻ��۹�ͨreff
struct HQStructShGgtReff04
{
	char sMainKey[30];//����������������Ϊ �г�+_+����,���� "1_600570"
	char sMarketNo[6];//�����г� 
	char sStockCode[20];//֤ȯ����
	char sStockName[100];//֤ȯ����
	double RoundLot; //������λ
	double PreClosePx; //ǰ���̼۸�
	//
	char sBsStatus[2];//����״̬����trades04�ļ��и���
};

//�Ϻ���Ʊ��Ȩreff
struct HQStructShGpqqReff03
{
	char sMainKey[30];//����������������Ϊ �г�+_+����,���� "1_600570"
	char sMarketNo[6];//�����г� 
	char sStockCode[20];//֤ȯ����
	char sStockName[100];//֤ȯ����
	double SecurityClosePx; //��Լǰ���̼�
	double DailyPriceUpLimit; //�Ƿ����޼۸�
	double DailyPriceDownLimit; //�������޼۸�
	double ContractMultiplierUnit; //��Լ��λ
};

//�Ϻ���Ʊ��Ȩclpr
struct HQStructShGpqqClpr03
{
	char sMainKey[30];//����������������Ϊ �г�+_+����,���� "1_600570"
	char sMarketNo[6];//�����г� 
	char sStockCode[20];//֤ȯ����
	double SecurityClosePx; //��Ȩ���̼�
	double SettlPrice; //��Ȩ�ο������
	double LeaveQty; //δƽ�ֺ�Լ��
};

//���ڳɽ���ͳ��ָ������
struct HQStructDealAmount
{
	char sStockCode[20];//֤ȯ����
	double dBusinessAmount;//�ܳɽ���     
	double dBusinessBalance;//�ܳɽ����  
};

//����Ƶ���б�
struct HQStructChannelData
{
	int iChannelNo;
	int64 iApplLastSeqNum;
	mutex m_lock;//�������map���м���
	map<int64, int64>* lpMapRecNum;
};

struct HQStructFullFileData//����ȫ���б�ṹ�壬�Ӹ����ļ��ж�ȡ
{
	char sMarketNo[6];//�����г� 
	char sStockCode[50];//֤ȯ����
	char sStockName[100];//֤ȯ���ƣ�ȫ�ֶ��ж������Ƶģ�ȫҪ
	char sStopFlag[2];//ͣ�Ʊ�־��ȫҪ
	char sFundType[2];//�������ͣ�ȫҪ
	double dPreclosePrice;//�����̣�ȡֵ��ʱ������ж����Ƿ�Ϊ0
	char sCodeType[7];//������࣬ȫҪ
	//
	double dAnountPerHand;//ÿ�ֹ�����Ŀǰ����Ȩ�У��ֻ�Ҳ����
	int iBsStatus;//������־��ֻ�и۹���
	int iLimitType;//1=���ȣ��ٷֱȣ�,2=�۸񣨾���ֵ��
	double dLimitUpRate;//���Ƿ���
	double dLimitDownRate;//�µ�����
	double dLimitUpAbsolute;//�����޼�
	double dLimitDownAbsolute;//�µ��޼�
};

//==============================������proto��Ľṹ�壬���𶯣���proto������ȫһ��=========================================================
//�ṹ�嶨���У�������ʹ��string��Ϊ�������ͣ�������memset����zeromemory��ʱ�������ڴ�й©
enum TaskKind
{
	ptTaskDefault,//Ĭ��ֵ
	ptTaskSHStock,//�Ϻ�֤ȯ
	ptTaskSHGPQQ,//�Ϻ���Ʊ��Ȩ
	ptTaskSHGGT,//�Ϻ��۹�ͨ
	ptTaskSzBin,//����binary
	//ptTaskSzStep,//����step
	//ptTaskFCQH,//�ֲ��ڻ�
	ptTaskSQCTP,//�����ڻ�
	ptTaskSHBigTrade,//�Ϻ�����
	ptTaskSHPhdj,//�Ϻ��̺󶨼�
	ptTaskSHGzyfx,//�Ϻ���ծԤ����
	ptTaskNQHQ,//��ת����
	ptTaskSHGdsy//�Ϻ��̶�����
};

enum HQKind
{
	ptHQ1000,
	ptHQ1001,
	ptHQ1002,
	ptHQ1006,
	ptHQ1011,
	ptHQ1013,
	ptHQ1055,
	ptHQ1057,
	ptHQ1058,
	ptHQ1059,
	ptHQ1060,
	ptHQ1061,
	ptHQ1069,
	ptHQ1070,
	ptHQ1071
};

//================================
//sMainKey�ֶα��룬����ṹ�������Զ�����CProtoPublic�����ļ�
//��ͨ����
struct HQStruct1000
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];//�����г� 
	char sStockCode[50];//֤ȯ�����ڻ�����Ŀǰ�����г���30�ģ��ڻ��������31��������ȡ���
	char sAbsoluteTimeStamp[20];//����ʱ�������ʽhhnnsszzz�� 
	double dPreclosePrice;//������         
	double dOpenPrice;//����  
	double dLastPrice;//���¼�  
	double dHighPrice;//��߼�  
	double dLowPrice;//��ͼ�   
	double dClosePrice;//������ 
	double dAvgPrice;//����    
	double dBusinessAmount;//�ܳɽ���     
	double dBusinessBalance;//�ܳɽ����  
	double dUpPrice;//��ͣ��   
	double dDownPrice;//��ͣ�� 
	double dCurrentAmount;//���֣���������������      
	double dAmount;//�ֲ���    
	double dPreAmount;//��ֲ���          
	double dPreSettlement;//�����        
	double dSettleMent;//�����
	double dIopv;//iopv        
	char sTradingPhaseCode[10];//��Ʒʵʩ��־(�г�����״̬)       
	double dAuctionPrice;//��̬�ο��۸�   
	double dMatchedQty;//����ƥ������     
	double dFundT1Price;//T-1�ջ���ֵ   
	char sFundType[2];//��������
	double dMarketDealAmount;//�г��ɽ���������ó���
	char sStopFlag[2];//ͣ�Ʊ�־
	char sBsStatus[2];//������־
	char sLatestEnquiryTime[8];//���ѯ��ʱ�䣨��ʽHHMMSS��      
	double dBusinWeight;//�ɽ�������˫�ߣ�
	double dBuyPrice1;//��һ�� 
	double dBuyPrice2;//����� 
	double dBuyPrice3;//������ 
	double dBuyPrice4;//���ļ� 
	double dBuyPrice5;//����� 
	double dSalePrice1;//��һ��
	double dSalePrice2;//������
	double dSalePrice3;//������
	double dSalePrice4;//���ļ�
	double dSalePrice5;//�����
	double dBuyQty1;//��һ��   
	double dBuyQty2;//�����   
	double dBuyQty3;//������   
	double dBuyQty4;//������   
	double dBuyQty5;//������   
	double dSaleQty1;//��һ��  
	double dSaleQty2;//������  
	double dSaleQty3;//������  
	double dSaleQty4;//������  
	double dSaleQty5;//������  
	char sStockName[100];//֤ȯ����
	double dAnountPerHand;//ÿ�ֹ���       
	double dTotalbuyAmount;//������ ����ڻ�ʹ��     
	double dTotalSaleAmount;//������ ����ڻ�ʹ��    
	double dCJBiShu;//�ɽ����� ����ڻ�ʹ��,����Ҳ��
	double dRealavgRate;//ʵʱ��Ȩƽ������ ����Э��ع�ʹ��      
	double dPreavgRate;//�����̼�Ȩƽ������ ����Э��ع�ʹ��     
	int  iInitDate;//��ʼ������          
	double dPe;//��̬��ӯ��
	double dPe2;//��̬��ӯ��          
	int iLayering;//��ת�ֲ���Ϣ��0-�����㣬1-���²㣬2-��ѡ��      
	int iZSSSL;//��ת����������     
	char sStockType[2];//֤ȯ���   
	char sCodeType[7];//�������   
	int iStrikeDate;  //��ʼ��Ȩ��   
	int iExpireDate;  //��Ȩ������
	double dMaxLimitedAmount;  //�޼��걨����
	double dMaxMarketAmount;  //�м��걨���� 
	double dHoldBalance;  //������С��֤����Ʒ��Ȩ ������ʹ�� 
	double dDelta; // ����ֵ����Ʒ��Ȩ ������ʹ�ã����㱣֤��ʱ��
	char sRecordTime[7];//�ļ��м�¼��ʱ��
};

//�۹�VCM
struct HQStruct1001
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];// 1 ; //�����г�
	char sStockCode[50];// 2 ; //֤ȯ����
	char sTimeStamp[20];// 3 ; //�е�����ʱ�䣨��ʽHHMMSSZZZ��
	double dReferencePrice;// 4 ; //�е��ο��۸�
	double dUpPrice;// 5 ; //�е����޼۸�
	double dDownPrice;// 6 ; //�е����޼۸�
	char sBeginTime[7];// 7 ; //�е���ʼʱ�䣨��ʽHHMMSS��
	char sEndTime[7];// 8 ; //�е�����ʱ�䣨��ʽHHMMSS��
};

//�۹�CAS
struct HQStruct1002
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];// 1 ; //�����г�
	char sStockCode[50];// 2 ; //֤ȯ����
	double dPopcPrice;// 3 ; //�ο�ƽ��۸�iep��
	double dSvol;// 4 ; //�ο�ƽ������iev��
	double dReferencePrice;// 5 ; //���о��۵Ĳο��۸�
	double dUpPrice;// 6 ; //���о��۵����޼۸�
	double dDownPrice;// 7 ; //���о��۵����޼۸�
	double dAmount;// 8 ; //δ����������̵���
	char sOrdImbDirection[2];// 9 ; //δ����������̵ķ���,���庬����ֶζ���
	char sTimeStamp[20];// 10 ; //�е�����ʱ�䣨��ʽHHMMSS000��
};

//�۹ɸ߼����飬��ʷ�������⻹����Ҫʵ�ֵ�
struct HQStruct1006
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];// 1; //�����г�
	char sStockCode[50];// 2; //֤ȯ����
	char sTradeStatus[9];// 3; //����״̬
	double dNominalPrice;// 4; //���̼�
};

//֤ȯ����
struct HQStruct1011
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];// 1; //�����г�
	char sStockCode[50];// 2; //֤ȯ����
	char sTimeStamp[20];// 3 ; //����ʱ�������ʽHHMMSSZZZ��
	int dSpecialMark;// 4 ; //������־
};

//����Э������
struct HQStruct1013
{
	char sMainKey[100];//�������������г�����Ϣ��¼�ţ�ϯλ�����룬�����ڲ���Ƶ������Ϣ��¼��
	char sMarketNo[6];// 1;	//�����г���1-�Ϻ���2-����
	char sRecordNo[20];// 2;	//��Ϣ��¼�ţ����յ��г���Ψһ
	char sReportSeat[10];// 3;	//ϯλ�����Ϻ���
	char sStockCode[50];// 4;	//֤ȯ����
	double dEntrustPrice;// 5;	//�۸�
	double dEntrustAmount;// 6;	//����
	char sSide[2];// 7;			//��������1-��2-��
	char sContactInfo[100];// 8;	//��ϵ����Ϣ
	char sTransactDate[10];// 9;	//��¼���ڣ���ʽYYYYMMDD��
	char sTransactTime[7];// 10;	//��¼ʱ�䣨��ʽHHMMSS��
	char sTransType[2];// 11;	//����������ͣ�1-Э������2-Э�鶨�ۡ��Ϻ�ֻ��Э������
	char sConfirmID[10];// 12;	//��������Լ���ţ���������
	char sBulkunderweightFlag[2];// 13;	//��ɶ����ֱ�־�ֶΣ�0-��1-�ǡ����Ϻ���
	char sExecType[2];// 14;				//ִ�����ͣ�0-δ�ɽ���4-�ѳ�����F-�ѳɽ�
};

//�Ͻ����̺󶨼�����
struct HQStruct1055
{
	char sMainKey[100];//������֤ȯ����
	char sMarketNo[6]; 	//�����г�--
	char sStockCode[50]; //֤ȯ����--
	char sStockName[100];// ֤ȯ����--
	double dBusinessAmount;//�ܳɽ���    -- 
	double dBusinessBalance;//�ܳɽ����  --
	double dClosePrice;//������--
	double dBuyVolume;//��������--
	double dSellVolume;//��������--
	char sTradingPhaseCode[10];// ��Ʒʵʱ״̬
	char sAbsoluteTimeStamp[20];// ����ʱ�������ʽhhnnsszzz��--
	int iInitDate;// ��ʼ������YYYYMMDD
};

//��Ȩ����ѯ������
struct HQStruct1057
{
	char sMainKey[200];//������֤ȯ���룬�����գ�ѯ�۱��
	char sMarketNo[6]; 	//�����г�
	char sStockCode[50]; 	//֤ȯ����
	char sTradingDay[10];// ������YYYYMMDD
	char sForquoteSysId[100];// ѯ�۱��
	char sForquoteTime[7];// ѯ��ʱ��(hhnnss)
	char sActionDay[10];// ҵ������YYYYMMDD
	char sAbsoluteTimeStamp[20];// ����ʱ�������ʽhhnnsszzz��
	int iInitDate;// ��ʼ������YYYYMMDD
};

//�۹�ͨʵʱ�������
struct HQStruct1058
{
	char sMainKey[6];//����������������Ϊ �г�
	char sMarketNo[6];// 1�����г�,����
	double dThresholdAmount;// 2ÿ�ճ�ʼ��ȣ���λ�����Ԫ
	double dPosAmt;// 3����ʣ���ȣ���λ�����Ԫ
	int iAmountStatus;// 4���״̬ 1��������������ԭ��ȫ�г���ֹ���룻2����ȿ���
	char sTradeStatus[9];// 5�۹������г�״̬
};

//ת��֤ͨȯ��������
struct HQStruct1059
{
	char sMainKey[100];//�������������г�����Ϣ��¼�š�ϯλ�����룬�����ڲ���Ƶ������Ϣ��¼��
	char sMarketNo[6];// 1;	//�����г���1-�Ϻ���2-���ڣ�Ŀǰ������֧��
	char sRecordNo[20];// 2;	//��Ϣ��¼�ţ��г�+��Ϣ��¼��+ϯλ+��¼���� ��Ϊ����
	char sReportSeat[10];// 3;	//ϯλ�����Ϻ��У����ڴ���
	char sStockCode[50];// 4;	//֤ȯ����
	double dEntrustPrice;// 5;	//�۸�
	double dEntrustAmount;// 6;	//����
	char sSide[2];// 7;	//��������G-���룬F-����
	char sTransactDate[10];// 8;	//��¼���ڣ���ʽYYYYMMDD��
	char sTransactTime[7];// 9;	//��¼ʱ�䣨��ʽHHMMSS��
	int iExpirationDays;// 10;	//���ޣ���λΪ������ת��֤ͨȯ����ҵ����д
	int iExpirationType;// 11;	//�������ͣ�1=�̶����ޡ�ת��֤ͨȯ����ҵ����д
	char sExecType[2];// 12; //ִ�����ͣ�0-δ�ɽ���4-�ѳ�����F-�ѳɽ���Ŀǰ�Ͻ�������ʾ�ѳɽ��������Fֵ��
};

//��������̼۽��׵��̺󶨼۽�������
struct HQStruct1060
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];// 1�����г�---
	char sStockCode[50];// 2֤ȯ����---
	char sStockName[100];// 3֤ȯ����
	char sTradingPhaseCode[10];// 4��Ʒʵʱ״̬---
	double dPrevClosePx;// 5���ռ�---
	double dNumTrades;// 6�ɽ�����---
	double dTotalVolumeTrade;// 7�ɽ�����---
	double dTotalValueTrade;// 8�ɽ��ܽ��---
	double dReferPrice;// 9�������̼۸�---
	double dBuyAmount;// 10����ʣ������---
	double dSaleAmount;// 11����ʣ������---
	char sTimeStamp[20];// 12����ʱ�������ʽhhnnsszzz��---
	int iInitDate;// 13��ʼ������---
};

//����Գɽ�����Ȩƽ���۽��׵��̺󶨼۽�������
struct HQStruct1061
{
	char sMainKey[100];//�������������Ϊ�����ֶ�,���ƥ�䷽��
	char sMarketNo[6];// 1�����г�
	char sStockCode[50];// 2֤ȯ����
	char sStockName[100];// 3֤ȯ����
	char sTradingPhaseCode[10];// 4��Ʒʵʱ״̬
	double dPrevClosePx;// 5���ռ�
	double dNumTrades;// 6�ɽ�����
	double dTotalVolumeTrade;// 7�ɽ�����
	double dTotalValueTrade;// 8�ɽ��ܽ��
	double dReferPrice;// 9�������̼۸�
	double dBuyAmount;// 10����ʣ������
	double dSaleAmount;// 11����ʣ������
	char sTimeStamp[20];// 12����ʱ�������ʽhhnnsszzz��
	int iInitDate;// 13��ʼ������
};

//�ڻ��н�����ϴ��뵼��
struct HQStruct1069
{
	char sMainKey[100];//������ �г�,��Լ����,��������
	int iInitDate;//��ʼ������
	int iMarginlegID;//��֤���Ⱥţ���̯���ڼ��ȣ�Ĭ�ϵ�һ��
	char sMarketNo[6];//�����г�
	char sEntrustBs[2];//��������, 1 - ��, 2 - ��
	char sKindType[2];//��������, 1 - ͬ�ڻ�Ʒ��, 2 - ͬ��ȨƷ��, 3 - ���ڻ�Ʒ��, 4 - ͬ����ڻ���Ȩ���
	char sRuleType[2];//��������
	char sStockCode[50];//��Լ����
	char sFirstCode[50];//��һ�ȴ���
	char sFirstBs[2];//��һ����������, 1 - ��, 2 - ��
	int iFirstRation;//��һ�ȱ���, �����һ����ڶ��ȱ���Ϊ1:3�����һ�ȱ���Ϊ1���ڶ��ȱ���Ϊ3
	char sSecondCode[50];//�ڶ��ȴ���
	char sSecondBs[2];//�ڶ�����������, 1 - ��, 2 - ��
	int iSecondRation;//�ڶ��ȱ���
};

//�Ͻ����̶�����ƽ̨ȷ����������
struct HQStruct1070
{
	char sMainKey[100];//����Ϊ����+���붩�����+�����������
	int iInitDate;//��ʼ������
	char sMarketNo[6];//�����г�
	char sStockCode[50];//��Լ����
	char sStockName[100];//֤ȯ����
	char sBuyOrderNo[10 + 1];//���붩�����
	int iBuyQuoteTime;//���뱨��ʱ��, HHMMSS
	char sBuyQuoter[10 + 1];//���뱨�۷�, �����̼��
	double dBuyNetPrice;//����ۣ����ۣ�, ��λ����
	double dBuyQty;//��������, ��λ����
	double dBuyAllPrice;//����ȫ��, ��λ����
	double dBuyProfitRatio;//"���뵽��������,�ٷ�����4λС�����磺2.1234
	char sSaleOrderNo[10 + 1];//�����������
	int iSaleQuoteTime;//��������ʱ��, HHMMSS
	char sSaleQuoter[10 + 1];//�������۷�, �����̼��
	double dSaleNetPrice;//�����ۣ����ۣ�, ��λ����
	double dSaleQty;//��������, ��λ����
	double dSaleAllPrice;//����ȫ��, ��λ����
	double dSaleProfitRatio;//��������������, �ٷ�����4λС�����磺2.1234
	double dAccruedInterest;//Ӧ����Ϣ, ��λ��0.1��
	char sRecordStatus[1+1];//��¼״̬��0��Ч��1��Ч��
};

//�Ͻ����̶�����ƽ̨�ɽ�����
struct HQStruct1071
{
	char sMainKey[100];//����Ϊ����
	int iInitDate;//��ʼ������
	char sMarketNo[6];//�����г�
	char sStockCode[50];//֤ȯ����
	char sStockName[100];//֤ȯ���
	int iTimeStamp;//ʱ��, HHMMSS
	double dYesterdayClosePrice;//�������̼�
	double dYesterdayWeightedAvgPrice;//���ռ�Ȩƽ����
	double dOpenPrice;//���տ��̼�
	double dHighPrice;//������߼�
	double dLowPrice;//������ͼ�
	double dLastPrice;//�������¼�
	double dWeightedAvgPrice;//���ռ�Ȩƽ����
	double dTotalQty;//�����ܳɽ���
	double dTotalAmount;//�����ܳɽ����
	double dTotalNum;//�����ܳɽ�����
	double dYesterdayCloseYield;//��������������
	double dYesterdayWeightedAvgYield;//���ռ�Ȩƽ��������
	double dOpenYield;//���տ���������
	double dHighYield;//�������������
	double dLowYield;//�������������
	double dLastYield;//��������������
	double dWeightedAvgYield;//���ռ�Ȩƽ��������
	char sCodeType[2 + 1];//���ײ�Ʒ����, 05 : ծȯ��ѺʽЭ��ع�, 10 : �����ع�, ����
};

#endif