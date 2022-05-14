/** @file mcapi_interface.h
 *  @brief MC2.0 SDK开发接口文件
 *  @author      xuxp
 *  @date        20120809
 *  @copyright   恒生电子股份有限公司
 *  @version     1.0.0.0
 *
 *  @par 修改记录 <日期> <版本> <修改人员> <修改内容>
 *  -# 2013-02-04 1.0.0.0 xuxp 发布初始版本
 *  20150209 majc 增加订阅者路由配置选项
 *	20180425 huxb 支持订阅项名称，补缺时按照订阅项名称进行补缺
 */
#ifndef _MC_INTERFACE_H_
#define _MC_INTERFACE_H_
#if defined(_WIN32)&& _MSC_VER < 1300
#pragma warning(disable: 4786)				//需加在被包含的STL头文件前
#pragma warning(disable: 4710)				//需加在被包含的STL头文件前
#endif
#include "plugin_interface.h"
#include <string.h>

#ifdef _WIN32
typedef unsigned int		uint32;
#else
#include <sys/types.h>
#if defined(__linux__)
#include <stdint.h>
#endif
typedef uint32_t			uint32;
#endif

#define IDENTITY_NAME_LENGTH    32  /**< 客户端名字长度 */
#define MAX_MACADDRESS_LEN	    18  /**< MAC 地址长度 */
#define MAX_RAND_LEN	        4   /**< 随机数长度 */

/** 客户标识长度 */
#define MAX_BIZNAME_LEN \
	IDENTITY_NAME_LENGTH+1+MAX_MACADDRESS_LEN+1+MAX_RAND_LEN+2

#define INIT_RECVQ_LEN 256          /**< 接收队列初始长度 */
#define STEP_RECVQ_LEN 512          /**< 接收队列扩展步长 */
#define SIMPLIFIED_CHINESE      0   /**< 错误信息语言:简体中文 */
#define ENGLISH                 1   /**< 错误信息语言:英文 */
#define MAX_FILTERFIELD_LEN 63      /**< 过滤字段长度 */
//20150209 majc  增加宏定义
#define T2SDK_MAX_BRANCH_NO_LEN  256 
#define T2SDK_MAX_COMPANY_NO_LEN 256 
#define T2SDK_MAX_OSPF_LEN       256
/** 主题可靠等级 */
enum ReliableLevel
{
    LEVEL_DOBEST            = 0,    /**< 尽力而为 */
    LEVEL_DOBEST_BYSEQ      = 1,    /**< 尽力有序 */
    LEVEL_MEM               = 2,    /**< 内存 */
    LEVEL_FILE              = 3,    /**< 文件 */
    LEVEL_SYSTEM            = 4     /**< 系统 */
};
//20150209 majc 增加创建订阅者时，指定路由参数结构体定义
typedef struct tagSubRouteInfo
{
	int iSystemNo;                       //系统号
	int iSubSystemNo;                    //子系统号
	char szBranchNo[T2SDK_MAX_BRANCH_NO_LEN+1];       //分支号
	char szCompanyNo[T2SDK_MAX_COMPANY_NO_LEN+1];        //公司编号
	char szOspfName[T2SDK_MAX_OSPF_LEN+1];         //OSPF
    tagSubRouteInfo()
    {
		memset(this,0,sizeof(tagSubRouteInfo));
    }
}SUB_ROUTER_INFO;
/** 
 * 过滤器接口
 */
class CFilterInterface:public IKnown
{
public:
   /**
    * 根据下标获取过滤条件的名字
    * @param index 对应的过滤条件下标
    * @return 返回对应的下标过滤条件的名字，否则返回NULL.
    */
    virtual char* FUNCTION_CALL_MODE GetFilterNameByIndex(int index) = 0;

   /**
    * 根据下标获取过滤条件的值
    * @param index 对应的过滤条件下标
    * @return 返回对应的下标过滤条件的值，否则返回NULL.
    */
    virtual char* FUNCTION_CALL_MODE GetFilterValueByIndex(int index)= 0;

   /**
    * 根据过滤条件的名字获取过滤条件的值
    * @param fileName 对应的过滤条件名字
    * @return 返回对应的过滤条件名字的条件值，否则返回NULL.
    */
    virtual char* FUNCTION_CALL_MODE GetFilterValue(char*  fileName)= 0;

   /**
    * 获取过滤条件的个数
    * @return 返回对应过滤条件的个数，没有返回0
    */
    virtual int   FUNCTION_CALL_MODE GetCount() = 0;

   /**
    * 设置过滤条件，根据过滤条件名字和值
    * @param filterName 对应的过滤条件名字
    * @param filterValue 对应的过滤条件名字的值
    */
    virtual void FUNCTION_CALL_MODE  SetFilter(char* filterName,char* filterValue) =0;
};

/**
 * 订阅参数类接口
 */
class CSubscribeParamInterface:public IKnown
{
public:

   /**
    * 设置主题名字
    * @param szName 对应的主题名字
    */
    virtual void FUNCTION_CALL_MODE  SetTopicName(char* szName) =0;

   /**
    * 设置附加数据
    * @param lpData 附加数据的首地址
    * @param iLen 附加数据的长度
    */
    virtual void FUNCTION_CALL_MODE  SetAppData(void* lpData,int iLen)=0;
    
   /**
    * 添加过滤条件
    * @param filterName 过滤条件的名字
    * @param filterValue 过滤条件的值
    */
    virtual void FUNCTION_CALL_MODE  SetFilter(char* filterName,char* filterValue)=0;

   /**
    * 添加返回字段
    * @param filedName 需要添加的返回字段
    */
    virtual void FUNCTION_CALL_MODE  SetReturnFiled(char* filedName)=0;

   /**
    * 设置是否补缺标志
    * @param bFromNow true表示需要之前的数据，也就是补缺，false表示不需要补缺
    */
    virtual void FUNCTION_CALL_MODE  SetFromNow(bool bFromNow)=0;

   /**
    * 设置覆盖订阅标志
    * @param bReplace true表示覆盖订阅，取消之前的所有订阅，只保留当前的订阅，false表示追加订阅
    */
    virtual void FUNCTION_CALL_MODE  SetReplace(bool bReplace)=0;

   /**
    * 设置发送间隔
    * @param nSendInterval 单位是秒
    */
    virtual void FUNCTION_CALL_MODE  SetSendInterval(int nSendInterval)=0;

   /**
    * 获取主题名字
    * @return 返回主题名字信息
    */
    virtual char* FUNCTION_CALL_MODE  GetTopicName() =0;

   /**
    * 获取附加数据
    * @param iLen 出参，表示附加数据的长度
    * @return 返回附加数据首地址，没有返回NULL
    */
    virtual void* FUNCTION_CALL_MODE  GetAppData(int *iLen) =0;

   /**
    * 获取对应的过滤字段的名字
    * @param index 对应的过滤条件下标
    * @return 返回对应的下标过滤条件的名字，否则返回NULL.
    */
    virtual char* FUNCTION_CALL_MODE  GetFilterNameByIndex(int index) = 0;

   /**
    * 根据下标获取过滤条件的值
    * @param index 对应的过滤条件下标
    * @return 返回对应的下标过滤条件的值，否则返回NULL.
    */
    virtual char* FUNCTION_CALL_MODE  GetFilterValueByIndex(int index)= 0;

   /**
    * 根据过滤条件的名字获取过滤条件的值
    * @param fileName 对应的过滤条件名字
    * @return 返回对应的过滤条件名字的条件值，否则返回NULL.
    */
    virtual char* FUNCTION_CALL_MODE  GetFilterValue(char*  fileName)= 0;

   /**
    * 获取过滤条件的个数
    * @return 返回对应过滤条件的个数，没有返回0
    */
    virtual int   FUNCTION_CALL_MODE  GetFilterCount() = 0;

   /**
    * 获取返回字段
    * @return 返回对应的返回字段信息
    */
    virtual char* FUNCTION_CALL_MODE  GetReturnFiled()=0;

   /**
    * 获取是否补缺的标志
    * @return 返回对应的补缺标志
    */
    virtual bool  FUNCTION_CALL_MODE  GetFromNow()=0 ;

   /**
    * 获取是否覆盖订阅的标志
    * @return 返回对应的覆盖订阅标志
    */
    virtual bool  FUNCTION_CALL_MODE  GetReplace() =0;

   /**
    * 获取对应的发送频率
    * @return 返回对应的发送间隔
    */
    virtual int   FUNCTION_CALL_MODE  GetSendInterval()=0;

	/**
    * 设置是否更新消息序号
    * @param bUpdate true表示需要更新消息序号，false表示不更新
    */
    virtual void FUNCTION_CALL_MODE  SetUpdateMsgNo(bool bUpdate)=0;

	/**
    * 获取是否更新消息序号
    * @return 返回是否更新消息序号
    */
    virtual bool  FUNCTION_CALL_MODE  GetUpdateMsgNo()=0;

	//20171129 majc 增加订阅参数接口
	//添加过滤条件
	/**
	* @param filterName 过滤条件的名字
	* @param iFileterValue 过滤条件的Int值
	**/
	virtual void FUNCTION_CALL_MODE SetFilterInt(char* filterName, int filterValue) = 0;
    //根据过滤条件的名字获取过滤条件的Int值
	/**
	* @param filterName 对应的过滤条件名字
	* @return 返回对应的过滤条件名字的条件Int值，否则返回0。
	**/
	virtual int FUNCTION_CALL_MODE GetFilterIntValue(char* filterName) = 0;
	//根据下标获取过滤条件的值
	/**
	* @param index 对应的过滤条件下标
	* @return 返回对应的下标过滤条件的值，否则返回0。
	**/
	virtual int FUNCTION_CALL_MODE GetFilterIntValueByIndex(int index) = 0;
	///20180425 huxb 订阅项名称，补缺时按照订阅项名称进行补缺
	/**
	* 设置订阅名称
	* @param subinfoName 订阅项名称
	*/
	virtual void FUNCTION_CALL_MODE  SetSubInfoName(char * subinfoName) = 0;

	/**
	* 获取订阅名称
	* @return 返回订阅名称
	*/
	virtual char*  FUNCTION_CALL_MODE  GetSubInfoName() = 0;
};

class CSubscribeInterface;

/**
 * @brief 订阅回调接口返回的数据定义，除了订阅需要的业务体之外，还需要返回的数据
 */
typedef struct tagSubscribeRecvData
{
	char* lpFilterData;     /**< 过滤字段的数据头指针，用解包器解包 */
	int iFilterDataLen;     /**< 过滤字段的数据长度 */
	char* lpAppData;        /**< 附加数据的数据头指针 */
	int iAppDataLen;        /**< 附加数据的长度 */
	char szTopicName[260];  /**< 主题名字 */
	
   /**
    * tagSubscribeRecvData 构造函数
    */
	tagSubscribeRecvData()
	{
		memset(this,0,sizeof(tagSubscribeRecvData));
	}
}SUBSCRIBE_RECVDATA, *LPSUBSCRIBE_RECVDATA;

/**
 * 订阅回调接口，上层应用通过这个接口，接收主推过来的消息
 */
class CSubCallbackInterface: public IKnown
{
public:

   /**
    * 收到发布消息的回调
    * @param lpSub 回调的订阅指针
    * @param subscribeIndex 消息对应的订阅标识，这个标识来自于SubscribeTopic函数的返回
    * @param lpData 返回消息的二进制指针，一般是消息的业务体打包内容
    * @param nLength 二进制数据的长度
	* @param lpRecvData 主推消息的其他字段返回，主要包含了附加数据，过滤信息，主题名字，详细参看前面结构体定义
    * @return 无
    */
    virtual void FUNCTION_CALL_MODE OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData) = 0;

   /**
    * 收到剔除订阅项的消息回调，一般在拥有踢人策略的主题下会回调这个接口,这个回调里面不需要取消订阅，底层已经取消这个订阅，只是一个通知接口
    * @param lpSub 回调的订阅指针
    * @param subscribeIndex 消息对应的订阅标识，这个标识来自于SubscribeTopic函数的返回
    * @param TickMsgInfo 踢人的错误信息，主要是包含具体重复的订阅项位置信息
    * @return 无
    */
    virtual void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo) = 0;
};


/**
 * 订阅接口的定义
 */
class CSubscribeInterface: public IKnown
{
public:

   /**
    * 订阅主题
    * @param lpSubscribeParamInter 上面定义的订阅参数结构
    * @param uiTimeout 超时时间
    * @param lppBizUnPack 业务校验时，失败返回的业务错误信息，如果订阅成功没有返回，输出参数，需要外面调用Release释放
                          如果接受业务校验的错误信息，写法如下：
                          IF2UnPacker* lpBizUnPack =NULL;
                          SubscribeTopic(...,&lpBizUnPack);
                          最后根据返回值，如果是失败的就判断 lpBizUnPack 是不是NULL.
                          最后错误信息获取完之后,释放
                          lpBizUnPack->Release();
    * @param lpBizPack 业务校验需要增加的业务字段以及值，没有就根据过滤属性作为业务校验字段
    * @return 返回值大于0，表示当前订阅成功的订阅标识，外面要记住这个标识和订阅项之间的映射关系，这个标识需要用于取消订阅和接收消息的回调里面.
	*		  返回其他值，根据错误号获取错误信息.
    */
    virtual int FUNCTION_CALL_MODE SubscribeTopic(CSubscribeParamInterface* lpSubscribeParamInter ,unsigned int uiTimeout,IF2UnPacker** lppBizUnPack=NULL,IF2Packer* lpBizPack=NULL) = 0;

   /**
    * 取消订阅主题
    * @param subscribeIndex 消息对应的订阅标识，这个标识来自于SubscribeTopic函数的返回
    * @return 返回0表示取消订阅成功，返回其他值，根据错误号获取错误信息.
    */
    virtual int FUNCTION_CALL_MODE CancelSubscribeTopic(int subscribeIndex) = 0;


	/**
    * 取消订阅主题
    * @param topicName 主题名字
    * @param lpFilterInterface 对应的过滤条件
    * @return 返回0表示取消订阅成功，返回其他值，根据错误号获取错误信息.
    */
    virtual int FUNCTION_CALL_MODE CancelSubscribeTopicEx(char* topicName,CFilterInterface* lpFilterInterface) = 0;



   /**
    * 获取当前订阅接口已经订阅的所有主题以及过滤条件信息
    * @param lpPack 外面传入的打包器
    * @note packer报文字段
        - SubcribeIndex
        - IsBornTopic
        - TopicName
        - TopicNo
        - FilterRaw
        - Appdata
        - SendInterval
        - ReturnFileds
        - isReplace
        - isFromNow
        - Stutas
        - QueueCount
    */
    virtual void FUNCTION_CALL_MODE GetSubcribeTopic(IF2Packer* lpPack)=0;

   /**
    * 取服务器地址
    * @param lpPort 输出的服务器端口，可以为NULL
    * @return 返回服务器地址
    */
    virtual const char * FUNCTION_CALL_MODE GetServerAddress(int *lpPort) = 0;
	 /**
    * 获取服务端的所有主题信息
    * @param byForce 是否强制从后台获取
    * @param iTimeOut 超时时间
    * @return 成功就返回所有主题信息
    * @note 解包器外面需要调用release接口进行释放.
    * @note packer返回字段
    * - TopicName
    * - TopicNo
    * - ReliableLevel
    * - IssuePriority
    * - MsgLifetime
    * - Stutas 
    * - TickStrategy 
    * - BusinessVerify
    * - Local
    * - FilterField1 
    * - FilterField2 
    * - FilterField3 
    * - FilterField4 
    * - FilterField5 
    * - FilterField6
	* - FilterField7
	* - FilterField8
	* - FilterField9
	* - FilterField10
	* - FilterField11
	* - FilterField12
	* - FilterField13
	* - FilterField14
	* - FilterField15
	* - FilterField16
    * - SubscribeStr 
    * - PublishStr
    */
    virtual IF2UnPacker* FUNCTION_CALL_MODE GetTopic(bool byForce,int iTimeOut) = 0;

	///20180425 huxb 新增SubInfoName字段，传入订阅项名称
	/**
	*批量订阅的接口
	*@param lpSubReqUnpack 批量参数的集合，采用unpack的形式传入
	*传入的解包器必须包含的字段如下：
	*TopicName SendInterval  ReturnFileds isFromNow AppData SubInfoName
	*FilterField1 FilterValue1 FilterField2 FilterValue2 FilterField3 FilterValue3 FilterField4 FilterValue4
	*FilterField5 FilterValue5 FilterField6 FilterValue6 FilterField7 FilterValue7 FilterField8 FilterValue8
	*FilterField9 FilterValue9 FilterField10 FilterValue10 FilterField11 FilterValue11 FilterField12 FilterValue12
	*FilterField13 FilterValue13 FilterField14 FilterValue14 FilterField15 FilterValue15 FilterField16 FilterValue16
	*BizCheckPack(用于订阅相应主题的业务校验unpack，二进制格式存放)
	*@param lpSubAnsPacker 订阅结果的返回，由外部申请一个空的打包器，函数内部把每个订阅结果打入打包器。
	*传出的打包器包含的字段如下：
	*TopicName SendInterval  ReturnFileds isFromNow AppData SubInfoName
	*FilterField1 FilterValue1 FilterField2 FilterValue2 FilterField3 FilterValue3 FilterField4 FilterValue4
	*FilterField5 FilterValue5 FilterField6 FilterValue6 FilterField7 FilterValue7 FilterField8 FilterValue8
	*FilterField9 FilterValue9 FilterField10 FilterValue10 FilterField11 FilterValue11 FilterField12 FilterValue12
	*FilterField13 FilterValue13 FilterField14 FilterValue14 FilterField15 FilterValue15 FilterField16 FilterValue16
	*BizCheckPack SubscribeIndex NotifyStatus ErrorNo ErrorInfo 
	*@param uiTimeout 订阅的超时时间,单位毫秒
	*@return 成功就返回0，其他值表示失败，通过错误号获取错误信息，具体每个订阅的失败原因可以查看lpSubAnsPacker内容。
	//////////////////////////////////////////////////////////////////////////////
	*调用实例：
	*IF2Packer* lpSubReqPack =NewPacker(2);
	*lpSubReqPack->AddRef();
	*添加多天需要订阅的参数，按照参数说面添加。
	*IF2Packer* lpSubAnsPack =NewPacker(2);
	*lpSubAnsPack->AddRef();
	*lpSubAnsPack->BeginPack();
	*BatchSubscribeTopic(lpSubReqPack->Unpack(),lpSubAnsPack,10000);
	*lpSubAnsPack->EndPack();
	*IF2UnPacker* lpAnsUnPack=lpSubAnsPack->Unpack();
	*根据返回参数获取对应的订阅应答
	*lpSubReqPack->FreeMem(lpSubReqPack->GetPackBuf());
	*lpSubReqPack->Release();
	*lpSubAnsPack->FreeMem(lpSubAnsPack->GetPackBuf());
	*lpSubAnsPack->Release();
	//////////////////////////////////////////////////////////////////////////////
	*/
	virtual int FUNCTION_CALL_MODE BatchSubscribeTopic(IF2UnPacker* lpSubReqUnpack,IF2Packer* lpSubAnsPacker,unsigned int uiTimeout) = 0;

	/**
    * 获取订阅会话状态
    * @return 0		 与消息中心连接断开 
			  1		 正在与消息中心建立连接 
			  2		 与消息中心连接已建立 
    */
    virtual int FUNCTION_CALL_MODE GetSessionStatus() = 0;
};

/**
 * 发布接口
 */
class CPublishInterface: public IKnown
{
public:

   /**
    * 业务打包格式的内容发布接口
    * @param topicName 主题名字，不知道名字就传NULL
    * @param lpUnPacker 具体的内容
    * @param iTimeOut 超时时间
    * @param lppBizUnPack 业务校验时，失败返回的业务错误信息，如果发布成功没有返回，输出参数，需要外面调用Release释放
                            如果接受业务校验的错误信息，写法如下：
                            IF2UnPacker* lpBizUnPack =NULL;
                            PubMsgByPacker(...,&lpBizUnPack);
                            最后根据返回值，如果是失败的就判断 lpBizUnPack 是不是NULL.
                            最后错误信息获取完之后,释放
                            lpBizUnPack->Release();
    * @param bAddTimeStamp 是否添加时间戳，配合单笔性能查找
    * @return 返回0表示成功，返回其他值，根据错误号获取错误信息.
    */
    virtual int FUNCTION_CALL_MODE PubMsgByPacker(char* topicName ,IF2UnPacker* lpUnPacker,int iTimeOut=-1,
        IF2UnPacker** lppBizUnPack=NULL,bool bAddTimeStamp=false) = 0;
    
   /**
    * 非业务打包格式的内容发布接口，一般二进制格式报文发布
    * @param topicName 主题名字，不知道名字就传NULL
    * @param lpFilterInterface 过滤条件，需要上层自己指定，否则默认没有过滤条件
    * @param lpData 具体的内容
    * @param nLength 内容长度
    * @param iTimeOut 超时时间
    * @param lppBizUnPack 业务校验时，失败返回的业务错误信息，如果发布成功没有返回，输出参数，需要外面调用Release释放
                            如果接受业务校验的错误信息，写法如下：
                            IF2UnPacker* lpBizUnPack =NULL;
                            PubMsgByPacker(...,&lpBizUnPack);
                            最后根据返回值，如果是失败的就判断 lpBizUnPack 是不是NULL.
                            最后错误信息获取完之后,释放
                            lpBizUnPack->Release();
    * @param bAddTimeStamp 是否添加时间戳，配合单笔性能查找
    * @return 返回0表示成功，返回其他值，根据错误号获取错误信息.
    */
    virtual int FUNCTION_CALL_MODE PubMsg(char* topicName, CFilterInterface* lpFilterInterface, const void *lpData, int nLength,int iTimeOut=-1,
        IF2UnPacker** lppBizUnPack=NULL,bool bAddTimeStamp=false) = 0;

   /**
    * 返回当前主题的发布序号
    * @param topicName 主题名字
    * @return 返回0表示没有对应的主题，返回其他值表示成功
    */
    virtual uint32 FUNCTION_CALL_MODE GetMsgNoByTopicName(char* topicName)=0;

   /**
    * 取服务器地址
    * @param lpPort 输出的服务器端口，可以为NULL
    * @return 返回服务器地址
    */
    virtual const char * FUNCTION_CALL_MODE GetServerAddress(int *lpPort) = 0;
	 /**
    * 获取服务端的所有主题信息
    * @param byForce 是否强制从后台获取
    * @param iTimeOut 超时时间
    * @return 成功就返回所有主题信息
    * @note 解包器外面需要调用release接口进行释放.
    * @note packer返回字段
    * - TopicName
    * - TopicNo
    * - ReliableLevel
    * - IssuePriority
    * - MsgLifetime
    * - Stutas 
    * - TickStrategy 
    * - BusinessVerify
    * - Local
    * - FilterField1 
    * - FilterField2 
    * - FilterField3 
    * - FilterField4 
    * - FilterField5 
    * - FilterField6
    * - SubscribeStr 
    * - PublishStr
    */
    virtual IF2UnPacker* FUNCTION_CALL_MODE GetTopic(bool byForce,int iTimeOut) = 0;
};


#endif


