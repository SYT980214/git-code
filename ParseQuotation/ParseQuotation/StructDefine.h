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

#define Const_Market_sh  "1" //上海
#define Const_Market_shggt  "n"//沪港通
#define Const_Market_sz  "2" //深圳
#define Const_Market_szggt  "o"//深港通
#define Const_Market_SQ  "3"; //上期所
#define Const_Market_DS  "9"; //大商所
#define Const_Market_ZS  "4"; //郑商所
#define Const_Market_ZJ  "7"; //中金所
#define Const_Market_NY  "k"; //能交所
#define Const_Market_GZ  "j" //股转市场
#define Const_Market_ShGdsy  "1547" //上海固定收益

//无效价格定义
#define Const_Nil_ArbiPrice  -9999999999.0
#define Const_Nil_Price  0.0
#define Const_Up_NilArbiPrice  9999999999.0
#define Const_Down_NilArbiPrice  -9999999999.0
#define Const_Up_NilPrice  0.0000000001
#define Const_Down_NilPrice  -0.0000000001
//无效极大值定义
#define Const_Up_NilBalance  9999999999999999999.0
#define Const_Nil_Balance  0.0

#define Const_SnapshotQuotationPlatfrom "A"; //实时平台
#define Const_RequestQuotationPlatfrom "B"; //重传平台


//结构体定义中，不允许使用string作为变量类型，否则在memset或者zeromemory的时候会存在内存泄漏
struct HQStructTimeSection
{
	int iBeginTime;
	int iEndTime;
	//下面几个给期货用
	char cMarketNo[2 + 1];
	char sMarketName[100];//证券名称
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

//中金所组合合约，单腿合约定义
struct HQStructContractDetail
{
	bool bFuture; 		//是否期货
	bool bOptionCall; 	//是否看涨期权
	char sKind[10];     //品种类型
	char sDate[10];
	int iDate;//转换为int
	char strikePrice[50];
	int iStrikePrice;//转换为int
};

//期货品种
struct HQStructFutureKind
{
	char C_MARKET_NO[2 + 1];
	char VC_KIND_CODE[10 + 1];
	int L_MULTIPLICATOR;
	char VC_TRADE_MONTH[64 + 1];
	char VC_CONTRACT_RULE[20 + 1];
	char C_STOCK_TYPE[2 + 1];
};

//证券模版
struct HQStructStockModel
{
	char sMainKey[100];//主键，该主题主键为 market_no+_+stockcode_Mask+_+stock_name_mask
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

//上海产品信息cpxx
struct HQStructShCpxx
{
	char sMainKey[30];//主键，该主题主键为 市场+_+代码,例如 "1_600570"
	char sMarketNo[6];//交易市场 
	char sStockCode[20];//证券代码
	char sStockName[100];//证券名称
	double dUpPrice;//涨停价   
	double dDownPrice;//跌停价 
	char sFundType[2];//基金类型
	char sStockType[2];//证券类型
	char sCodeType[7];//代码分类
	int iAmountPerHand;//每手股数 //这个字段是从模版中匹配的，首次为0//从cpxx中获取, 债券写死为10
	double dPreclosePrice;//昨收盘   一般不用cpxx中的昨收盘
};

//上海港股通reff
struct HQStructShGgtReff04
{
	char sMainKey[30];//主键，该主题主键为 市场+_+代码,例如 "1_600570"
	char sMarketNo[6];//交易市场 
	char sStockCode[20];//证券代码
	char sStockName[100];//证券名称
	double RoundLot; //买卖单位
	double PreClosePx; //前收盘价格
	//
	char sBsStatus[2];//买卖状态，从trades04文件中更新
};

//上海股票期权reff
struct HQStructShGpqqReff03
{
	char sMainKey[30];//主键，该主题主键为 市场+_+代码,例如 "1_600570"
	char sMarketNo[6];//交易市场 
	char sStockCode[20];//证券代码
	char sStockName[100];//证券名称
	double SecurityClosePx; //合约前收盘价
	double DailyPriceUpLimit; //涨幅上限价格
	double DailyPriceDownLimit; //跌幅下限价格
	double ContractMultiplierUnit; //合约单位
};

//上海股票期权clpr
struct HQStructShGpqqClpr03
{
	char sMainKey[30];//主键，该主题主键为 市场+_+代码,例如 "1_600570"
	char sMarketNo[6];//交易市场 
	char sStockCode[20];//证券代码
	double SecurityClosePx; //期权收盘价
	double SettlPrice; //期权参考结算价
	double LeaveQty; //未平仓合约数
};

//深圳成交量统计指标行情
struct HQStructDealAmount
{
	char sStockCode[20];//证券代码
	double dBusinessAmount;//总成交量     
	double dBusinessBalance;//总成交金额  
};

//深圳频道列表
struct HQStructChannelData
{
	int iChannelNo;
	int64 iApplLastSeqNum;
	mutex m_lock;//对下面的map进行加锁
	map<int64, int64>* lpMapRecNum;
};

struct HQStructFullFileData//深圳全局列表结构体，从各个文件中读取
{
	char sMarketNo[6];//交易市场 
	char sStockCode[50];//证券代码
	char sStockName[100];//证券名称，全局队列都有名称的，全要
	char sStopFlag[2];//停牌标志，全要
	char sFundType[2];//基金类型，全要
	double dPreclosePrice;//昨收盘，取值的时候最好判断下是否为0
	char sCodeType[7];//代码分类，全要
	//
	double dAnountPerHand;//每手股数，目前仅期权有，现货也用了
	int iBsStatus;//买卖标志，只有港股有
	int iLimitType;//1=幅度（百分比）,2=价格（绝对值）
	double dLimitUpRate;//上涨幅度
	double dLimitDownRate;//下跌幅度
	double dLimitUpAbsolute;//上涨限价
	double dLimitDownAbsolute;//下跌限价
};

//==============================下面是proto类的结构体，勿动勿动，跟proto保持完全一致=========================================================
//结构体定义中，不允许使用string作为变量类型，否则在memset或者zeromemory的时候会存在内存泄漏
enum TaskKind
{
	ptTaskDefault,//默认值
	ptTaskSHStock,//上海证券
	ptTaskSHGPQQ,//上海股票期权
	ptTaskSHGGT,//上海港股通
	ptTaskSzBin,//深圳binary
	//ptTaskSzStep,//深圳step
	//ptTaskFCQH,//分仓期货
	ptTaskSQCTP,//上期期货
	ptTaskSHBigTrade,//上海大宗
	ptTaskSHPhdj,//上海盘后定价
	ptTaskSHGzyfx,//上海国债预发行
	ptTaskNQHQ,//股转行情
	ptTaskSHGdsy//上海固定收益
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
//sMainKey字段必须，下面结构体用于自动生成CProtoPublic代码文件
//普通行情
struct HQStruct1000
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];//交易市场 
	char sStockCode[50];//证券代码期货代码目前长度有超过30的，期货定义的是31，我们再取大点
	char sAbsoluteTimeStamp[20];//绝对时间戳（格式hhnnsszzz） 
	double dPreclosePrice;//昨收盘         
	double dOpenPrice;//今开盘  
	double dLastPrice;//最新价  
	double dHighPrice;//最高价  
	double dLowPrice;//最低价   
	double dClosePrice;//今收盘 
	double dAvgPrice;//均价    
	double dBusinessAmount;//总成交量     
	double dBusinessBalance;//总成交金额  
	double dUpPrice;//涨停价   
	double dDownPrice;//跌停价 
	double dCurrentAmount;//现手，公共函数中算了      
	double dAmount;//持仓量    
	double dPreAmount;//昨持仓量          
	double dPreSettlement;//昨结算        
	double dSettleMent;//结算价
	double dIopv;//iopv        
	char sTradingPhaseCode[10];//产品实施标志(市场交易状态)       
	double dAuctionPrice;//动态参考价格   
	double dMatchedQty;//虚拟匹配数量     
	double dFundT1Price;//T-1日基金净值   
	char sFundType[2];//基金类型
	double dMarketDealAmount;//市场成交量（计算得出）
	char sStopFlag[2];//停牌标志
	char sBsStatus[2];//买卖标志
	char sLatestEnquiryTime[8];//最近询价时间（格式HHMMSS）      
	double dBusinWeight;//成交重量（双边）
	double dBuyPrice1;//买一价 
	double dBuyPrice2;//买二价 
	double dBuyPrice3;//买三价 
	double dBuyPrice4;//买四价 
	double dBuyPrice5;//买五价 
	double dSalePrice1;//卖一价
	double dSalePrice2;//卖二价
	double dSalePrice3;//卖三价
	double dSalePrice4;//卖四价
	double dSalePrice5;//卖五价
	double dBuyQty1;//买一量   
	double dBuyQty2;//买二量   
	double dBuyQty3;//买三量   
	double dBuyQty4;//买四量   
	double dBuyQty5;//买五量   
	double dSaleQty1;//卖一量  
	double dSaleQty2;//卖二量  
	double dSaleQty3;//卖三量  
	double dSaleQty4;//卖四量  
	double dSaleQty5;//卖五量  
	char sStockName[100];//证券名称
	double dAnountPerHand;//每手股数       
	double dTotalbuyAmount;//总买量 组合期货使用     
	double dTotalSaleAmount;//总卖量 组合期货使用    
	double dCJBiShu;//成交笔数 组合期货使用,深圳也有
	double dRealavgRate;//实时加权平均利率 深圳协议回购使用      
	double dPreavgRate;//昨收盘加权平均利率 深圳协议回购使用     
	int  iInitDate;//初始化日期          
	double dPe;//静态市盈率
	double dPe2;//动态市盈率          
	int iLayering;//股转分层信息，0-基础层，1-创新层，2-精选层      
	int iZSSSL;//股转做市商数量     
	char sStockType[2];//证券类别   
	char sCodeType[7];//代码分类   
	int iStrikeDate;  //开始行权日   
	int iExpireDate;  //行权到期日
	double dMaxLimitedAmount;  //限价申报上限
	double dMaxMarketAmount;  //市价申报上限 
	double dHoldBalance;  //开仓最小保证金，商品期权 上期所使用 
	double dDelta; // 风险值，商品期权 上期所使用，计算保证金时用
	char sRecordTime[7];//文件中记录的时间
};

//港股VCM
struct HQStruct1001
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];// 1 ; //交易市场
	char sStockCode[50];// 2 ; //证券代码
	char sTimeStamp[20];// 3 ; //市调行情时间（格式HHMMSSZZZ）
	double dReferencePrice;// 4 ; //市调参考价格
	double dUpPrice;// 5 ; //市调上限价格
	double dDownPrice;// 6 ; //市调下限价格
	char sBeginTime[7];// 7 ; //市调开始时间（格式HHMMSS）
	char sEndTime[7];// 8 ; //市调结束时间（格式HHMMSS）
};

//港股CAS
struct HQStruct1002
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];// 1 ; //交易市场
	char sStockCode[50];// 2 ; //证券代码
	double dPopcPrice;// 3 ; //参考平衡价格（iep）
	double dSvol;// 4 ; //参考平衡量（iev）
	double dReferencePrice;// 5 ; //收市竞价的参考价格
	double dUpPrice;// 6 ; //收市竞价的上限价格
	double dDownPrice;// 7 ; //收市竞价的下限价格
	double dAmount;// 8 ; //未能配对买卖盘的量
	char sOrdImbDirection[2];// 9 ; //未能配对买卖盘的方向,具体含义见字段定义
	char sTimeStamp[20];// 10 ; //市调行情时间（格式HHMMSS000）
};

//港股高级行情，历史遗留问题还是需要实现的
struct HQStruct1006
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];// 1; //交易市场
	char sStockCode[50];// 2; //证券代码
	char sTradeStatus[9];// 3; //交易状态
	double dNominalPrice;// 4; //按盘价
};

//证券开关
struct HQStruct1011
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];// 1; //交易市场
	char sStockCode[50];// 2; //证券代码
	char sTimeStamp[20];// 3 ; //绝对时间戳（格式HHMMSSZZZ）
	int dSpecialMark;// 4 ; //买卖标志
};

//大宗协议行情
struct HQStruct1013
{
	char sMainKey[100];//主键，主键，市场，消息记录号，席位，代码，深圳内部用频道加消息记录号
	char sMarketNo[6];// 1;	//交易市场，1-上海，2-深圳
	char sRecordNo[20];// 2;	//消息记录号，当日单市场下唯一
	char sReportSeat[10];// 3;	//席位。仅上海有
	char sStockCode[50];// 4;	//证券代码
	double dEntrustPrice;// 5;	//价格
	double dEntrustAmount;// 6;	//数量
	char sSide[2];// 7;			//买卖方向：1-买，2-卖
	char sContactInfo[100];// 8;	//联系人信息
	char sTransactDate[10];// 9;	//记录日期（格式YYYYMMDD）
	char sTransactTime[7];// 10;	//记录时间（格式HHMMSS）
	char sTransType[2];// 11;	//逐笔数据类型，1-协议意向，2-协议定价。上海只有协议意向
	char sConfirmID[10];// 12;	//定价行情约定号，仅深圳有
	char sBulkunderweightFlag[2];// 13;	//大股东减持标志字段，0-否，1-是。仅上海有
	char sExecType[2];// 14;				//执行类型，0-未成交，4-已撤销，F-已成交
};

//上交所盘后定价行情
struct HQStruct1055
{
	char sMainKey[100];//主键，证券代码
	char sMarketNo[6]; 	//交易市场--
	char sStockCode[50]; //证券代码--
	char sStockName[100];// 证券名称--
	double dBusinessAmount;//总成交量    -- 
	double dBusinessBalance;//总成交金额  --
	double dClosePrice;//今收盘--
	double dBuyVolume;//买入数量--
	double dSellVolume;//卖出数量--
	char sTradingPhaseCode[10];// 产品实时状态
	char sAbsoluteTimeStamp[20];// 绝对时间戳（格式hhnnsszzz）--
	int iInitDate;// 初始化日期YYYYMMDD
};

//期权做市询价行情
struct HQStruct1057
{
	char sMainKey[200];//主键，证券代码，交易日，询价编号
	char sMarketNo[6]; 	//交易市场
	char sStockCode[50]; 	//证券代码
	char sTradingDay[10];// 交易日YYYYMMDD
	char sForquoteSysId[100];// 询价编号
	char sForquoteTime[7];// 询价时间(hhnnss)
	char sActionDay[10];// 业务日期YYYYMMDD
	char sAbsoluteTimeStamp[20];// 绝对时间戳（格式hhnnsszzz）
	int iInitDate;// 初始化日期YYYYMMDD
};

//港股通实时额度行情
struct HQStruct1058
{
	char sMainKey[6];//主键，该主题主键为 市场
	char sMarketNo[6];// 1交易市场,主键
	double dThresholdAmount;// 2每日初始额度，单位人民币元
	double dPosAmt;// 3日中剩余额度，单位人民币元
	int iAmountStatus;// 4额度状态 1：额度用完或其他原因全市场禁止买入；2：额度可用
	char sTradeStatus[9];// 5港股行情市场状态
};

//转融通证券出借行情
struct HQStruct1059
{
	char sMainKey[100];//主键，主键，市场、消息记录号、席位、代码，深圳内部用频道加消息记录号
	char sMarketNo[6];// 1;	//交易市场，1-上海，2-深圳，目前仅深圳支持
	char sRecordNo[20];// 2;	//消息记录号，市场+消息记录号+席位+记录日期 作为主键
	char sReportSeat[10];// 3;	//席位。仅上海有，深圳传空
	char sStockCode[50];// 4;	//证券代码
	double dEntrustPrice;// 5;	//价格
	double dEntrustAmount;// 6;	//数量
	char sSide[2];// 7;	//买卖方向：G-借入，F-出借
	char sTransactDate[10];// 8;	//记录日期（格式YYYYMMDD）
	char sTransactTime[7];// 9;	//记录时间（格式HHMMSS）
	int iExpirationDays;// 10;	//期限，单位为天数。转融通证券出借业务填写
	int iExpirationType;// 11;	//期限类型，1=固定期限。转融通证券出借业务填写
	char sExecType[2];// 12; //执行类型，0-未成交，4-已撤销，F-已成交。目前上交所不揭示已成交，因此无F值域
};

//深交所以收盘价交易的盘后定价交易行情
struct HQStruct1060
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];// 1交易市场---
	char sStockCode[50];// 2证券代码---
	char sStockName[100];// 3证券名称
	char sTradingPhaseCode[10];// 4产品实时状态---
	double dPrevClosePx;// 5昨收价---
	double dNumTrades;// 6成交笔数---
	double dTotalVolumeTrade;// 7成交总量---
	double dTotalValueTrade;// 8成交总金额---
	double dReferPrice;// 9当日收盘价格---
	double dBuyAmount;// 10买盘剩余数量---
	double dSaleAmount;// 11卖盘剩余数量---
	char sTimeStamp[20];// 12绝对时间戳（格式hhnnsszzz）---
	int iInitDate;// 13初始化日期---
};

//深交所以成交量加权平均价交易的盘后定价交易行情
struct HQStruct1061
{
	char sMainKey[100];//主键，主键变更为代码字段,监控匹配方便
	char sMarketNo[6];// 1交易市场
	char sStockCode[50];// 2证券代码
	char sStockName[100];// 3证券名称
	char sTradingPhaseCode[10];// 4产品实时状态
	double dPrevClosePx;// 5昨收价
	double dNumTrades;// 6成交笔数
	double dTotalVolumeTrade;// 7成交总量
	double dTotalValueTrade;// 8成交总金额
	double dReferPrice;// 9当日收盘价格
	double dBuyAmount;// 10买盘剩余数量
	double dSaleAmount;// 11卖盘剩余数量
	char sTimeStamp[20];// 12绝对时间戳（格式hhnnsszzz）
	int iInitDate;// 13初始化日期
};

//期货中金所组合代码导入
struct HQStruct1069
{
	char sMainKey[100];//主键， 市场,合约代码,买卖方向
	int iInitDate;//初始化日期
	int iMarginlegID;//保证金腿号，分摊到第几腿，默认第一腿
	char sMarketNo[6];//交易市场
	char sEntrustBs[2];//买入卖出, 1 - 买, 2 - 卖
	char sKindType[2];//策略类型, 1 - 同期货品种, 2 - 同期权品种, 3 - 跨期货品种, 4 - 同标的期货期权组合
	char sRuleType[2];//策略名称
	char sStockCode[50];//合约代码
	char sFirstCode[50];//第一腿代码
	char sFirstBs[2];//第一腿买卖方向, 1 - 买, 2 - 卖
	int iFirstRation;//第一腿比例, 如果第一腿与第二腿比例为1:3，则第一腿比例为1，第二腿比例为3
	char sSecondCode[50];//第二腿代码
	char sSecondBs[2];//第二腿买卖方向, 1 - 买, 2 - 卖
	int iSecondRation;//第二腿比例
};

//上交所固定收益平台确定报价行情
struct HQStruct1070
{
	char sMainKey[100];//主键为代码+买入订单编号+卖出订单编号
	int iInitDate;//初始化日期
	char sMarketNo[6];//交易市场
	char sStockCode[50];//合约代码
	char sStockName[100];//证券名称
	char sBuyOrderNo[10 + 1];//买入订单编号
	int iBuyQuoteTime;//买入报价时间, HHMMSS
	char sBuyQuoter[10 + 1];//买入报价方, 交易商简称
	double dBuyNetPrice;//买入价（净价）, 单位：厘
	double dBuyQty;//买入数量, 单位：手
	double dBuyAllPrice;//买入全价, 单位：厘
	double dBuyProfitRatio;//"买入到期收益率,百分数（4位小数）如：2.1234
	char sSaleOrderNo[10 + 1];//卖出订单编号
	int iSaleQuoteTime;//卖出报价时间, HHMMSS
	char sSaleQuoter[10 + 1];//卖出报价方, 交易商简称
	double dSaleNetPrice;//卖出价（净价）, 单位：厘
	double dSaleQty;//卖出数量, 单位：手
	double dSaleAllPrice;//卖出全价, 单位：厘
	double dSaleProfitRatio;//卖出到期收益率, 百分数（4位小数）如：2.1234
	double dAccruedInterest;//应计利息, 单位：0.1厘
	char sRecordStatus[1+1];//记录状态（0无效，1有效）
};

//上交所固定收益平台成交行情
struct HQStruct1071
{
	char sMainKey[100];//主键为代码
	int iInitDate;//初始化日期
	char sMarketNo[6];//交易市场
	char sStockCode[50];//证券代码
	char sStockName[100];//证券简称
	int iTimeStamp;//时间, HHMMSS
	double dYesterdayClosePrice;//昨日收盘价
	double dYesterdayWeightedAvgPrice;//昨日加权平均价
	double dOpenPrice;//当日开盘价
	double dHighPrice;//当日最高价
	double dLowPrice;//当日最低价
	double dLastPrice;//当日最新价
	double dWeightedAvgPrice;//当日加权平均价
	double dTotalQty;//当日总成交量
	double dTotalAmount;//当日总成交金额
	double dTotalNum;//当日总成交笔数
	double dYesterdayCloseYield;//昨日收盘收益率
	double dYesterdayWeightedAvgYield;//昨日加权平均收益率
	double dOpenYield;//当日开盘收益率
	double dHighYield;//当日最高收益率
	double dLowYield;//当日最低收益率
	double dLastYield;//当日最新收益率
	double dWeightedAvgYield;//当日加权平均收益率
	char sCodeType[2 + 1];//交易产品类型, 05 : 债券质押式协议回购, 10 : 三方回购, 其它
};

#endif