/*
	修改记录
		maoyinjie 2011/04/19	增加遍历TAG的接口
		maoyinjie 2012/08/14	增加直接处理业务体的接口
*/

#ifndef ESB_MESSAGE_INTERFACE_H
#define ESB_MESSAGE_INTERFACE_H

#if defined(_WIN32)&& _MSC_VER < 1300
#pragma warning(disable: 4786)				//需加在被包含的STL头文件前
#pragma warning(disable: 4710)				//需加在被包含的STL头文件前
#endif


#include "plugin_interface.h"




//	优先级定义以及各种建议优先级(应用可以根据实际需要上下调整,绝对不允许其他应用使用>=14的优先级)
#define MAX_PRIORITYQ			16	//	总共支持16级优先级
#define PRIORITY_REAL			15	//	最高优先级,仅仅协议内部包使用
#define PRIORITY_OSPF			14	//	OSPF协议使用的优先级
#define PRIORITY_HIGHER			13	//	高优先级,适用紧急管理类指令
#define PRIORITY_HIGH			10	//	较高优先级,实时交易类指令适用,语音聊天应该也适用该优先级
#define PRIORITY_NORMAL			8	//	中优先级,实时的查询指令适用
#define PRIORITY_LOW			6	//	较低优先级,历史类查询适用
#define PRIORITY_LOWER			3	//	低优先级,文字聊天适用(档案中影像数据的传输)
#define PRIORITY_LAZY			0	//	最慢的发送(好象无应用?)

/// 在HSESB间传递的消息,消息在各个组件间传递,作为我们ESB内部的消息格式,其他通道的消息必须先转换成该消息格式
///	才能在组件间传递
///	存在多线程冲突,不允许多线程并发访问

/*
	获取消息中的域值时,如果该TAG并不存在于该消息中,则返回缺省值 ,缺省值如下:
		对于int型,		返回0
		对于string型	返回""
		对于rawdata型	返回"",并且长度为0
IESBMessage:
	ITagItems
		ITagItem
		...
		ITagItem
	...
	ITagItems
		ITagItem
		...
		ITagItem
*/


/// rawdata不允许出现在数组中
struct ITagItem : IKnown
{
	/// 获取值

	/**
	* 获得字串形式的值:
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	指向域值的指针
	**/
	virtual const char* FUNCTION_CALL_MODE GetString(int iIndex=0) =0;			//	""
	/**
	* 获得整数形式的值
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	域值
	**/
	virtual int			FUNCTION_CALL_MODE GetInt(int iIndex=0)=0;				//	0
	//	获得rawdata的值
	/**
	* 获得rawdata的值
	*@param int* ilpRawDataLen:	返回rawdata的长度
	*@param int iIndex:			获取数组内第几个
	*@return	指向域值的指针
	**/
	virtual const void* FUNCTION_CALL_MODE GetRawData(int* ilpRawDataLen=NULL, int iIndex=0)=0;
	/**
	* 获得rawdata的长度
	*@param int iIndex:			获取数组内第几个
	*@return	长度
	**/
	virtual int		FUNCTION_CALL_MODE 	GetRawDataLen(int iIndex=0)=0;		//	0
	

	///	设置值

	/**
	* 设置字串形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param const char* lpValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetString(const char* lpValue, int iIndex=0)=0;
	/**
	* 设置整数形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param int iValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetInt(int iValue, int iIndex=0)=0;
	/**
	* 设置rawdata形式的值
	*@param const void* lpRawData:	域值
	*@param int iRawDataLen:	域值长度
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetRawData(const void* lpRawData, int iRawDataLen)=0;

	/// 获得数组内成员个数 
	virtual int FUNCTION_CALL_MODE GetCount()=0;

	/// 是否存在第iIndex个成员
	virtual bool FUNCTION_CALL_MODE IsExist(int iIndex=0)=0;

	/**
	* 设置字串形式的值,加在当前数组的尾部,增加数组项
	*@param const char* lpValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetStringAtTail(const char* lpValue)=0;
};


/// 线程不安全
struct IESBMessage : IKnown
{
	/**
	*	获得某个域值的对象指针
	*@param int	iFieldTag:需要获得域值的TAG
	*@return	指向域值对象的指针(如果消息内不存在该TAG,则会返回一个缺省的对象,确保返回非空)
	**/
	virtual ITagItem*	FUNCTION_CALL_MODE GetItem(int iFieldTag)=0;
	///	根据报文生成消息对象内的各个域
	/**
	* 根据报文生成消息对象内的各个域
	*@param const void* lpMessageBuffer:	报文
	*@param int iMsgLen:					报文长度
	*@return
		0 成功
		其他失败
	**/
	virtual int			FUNCTION_CALL_MODE SetBuffer(const void* lpMessageBuffer, int iMsgLen) = 0;
	/**
	* 将本对象内的各个域值对象最终封装成报文返回,(不封装协议内部的域值)
	*@param int* ilpMsgLen:	封装后报文的长度
	*@return 报文
	**/
	virtual void*		FUNCTION_CALL_MODE GetBuffer(int* ilpMsgLen) = 0;
	/**
	* 将本对象内的各个域值对象最终封装成报文返回(封装所有域值)
	*@param int* ilpMsgLen:	封装后报文的长度
	*@return 报文
	**/
	virtual void*		FUNCTION_CALL_MODE GetInternalBuffer(int* ilpMsgLen) = 0;
	/// 检查消息中是否存在iTag域
	/**
		返回0:不存在
			非0存在
	**/
	virtual int			FUNCTION_CALL_MODE IsExist(int iTag) = 0;




	///	操纵消息的一些辅助函数
	/**
	* 重置请求消息,
	* 内部操作:
	*	一:清空消息内所有域值
	*	二:将消息类型设置为iPacketType,功能号设置为iFuncID
	*	三:如果是请求,则缺省设置需要对方应答(如果应用者不需要对方的响应,则后续应该显式的调用ExpectResponse,
	*		将其重新设置成不需要对方响应)
	*		如果是应答,则缺省设置成不需要对方响应(如果应用者实际需要对方的响应,则后续应该显式的调用ExpectResponse,
	*		将其重新设置成需要对方响应)
	*
	*	不需要对方响应的报文将比需要对方响应的报文要小.

	*@param int iPacketType:	报文类型
	*@param int iFuncID:		功能号
	*@return
		0 成功
		其他失败
	**/
	virtual int			FUNCTION_CALL_MODE Prepare(int iPacketType, int iFuncID) = 0;
	/**
	* 将消息从请求形式转化成应答形式
	*	该函数执行如下操作:
	*		一:将消息类型从请求转换成应答
	*		二:从消息中取出发送者信息,将其设置成路由信息.
	*		三:将该消息设置成不需要对方的响应(如果应用者需要获知对方的响应,则需要显式的调用ExpectResponse,将其重新设置成需要对方响应)
	*		四:将   !!!!所有其他的域值!!!!!!  重置为空
	*
	*	本操作将破坏原始消息,如果应用者需要保留原始消息的话,不能用本函数操作

	*@return
			0 成功
			其他失败,失败的原因有如下几种:
				一:本消息不是请求报文
				二:在消息中并不包含发送者信息

		说明:
			
	**/
	virtual int			FUNCTION_CALL_MODE ChangeReq2AnsMessage() = 0;
	/// 在创建完一条完整消息之后,在发送之前,应用可以设置自己是否需要对方的响应
	/**
		通过调用本函数可以,应用显示的通知router,针对该消息,自己是否期望从对方收到响应消息
		一般来说,请求者总是需要对方响应
		而应答者一般不希望对方有响应
	**/
	virtual int			FUNCTION_CALL_MODE ExpectResponse(bool bNeedResponse) = 0;
	/// 在消息中删除一个域
	virtual int			FUNCTION_CALL_MODE RemoveTag(int iTag) = 0;



	//修改人:xuqq
	//修改时间:2008-11-11
	//修改目的:增加接口为了能自由的设置压缩功能
	//以下为新增加的代码

	///成功返回0,失败返回1
	///默认使用1号压缩功能，也就是minilzo压缩算法
	virtual int			FUNCTION_CALL_MODE SetCompressID(int iCompressID = 1) = 0;

	virtual void		FUNCTION_CALL_MODE SetOwner(void* lpOwner) = 0;
	virtual void*       FUNCTION_CALL_MODE GetOwner() = 0;

	//得到引用计数
	virtual int			FUNCTION_CALL_MODE GetRef() = 0;
	//--------------------------------------xuqq的代码到此为止



	virtual int			FUNCTION_CALL_MODE AddTimeStamp(char* lpText) = 0;

	//	maoyinjie 2011/04/19	增加遍历TAG的接口
	virtual int FUNCTION_CALL_MODE BeginTour() = 0;		//	开始遍历
	//	返回<0	说明已经遍历完毕
	virtual int FUNCTION_CALL_MODE GetItemTag() = 0;	//	获得当前TAG
	virtual int FUNCTION_CALL_MODE NextItem() = 0;		//	到下一个TAG

	
	//	maoyinjie 2012/08/14	增加直接处理业务体的接口
	//	业务体数据	业务体数据长度	iNoCompressInForce=1:强制不压缩,  0:根据配置,可能压缩,也可能不压缩
	virtual int FUNCTION_CALL_MODE SetBizData(char* lpBizData, int iBizDataLen, int iNoCompressInForce=0) = 0;
	//	返回业务体数据	
	//	参数:	业务体数据长度	iNoCompressInForce=1:强制不解压缩,  0:根据配置,可能解压缩,也可能不解压缩
	virtual const void*	FUNCTION_CALL_MODE GetBizData(int* ilpRawDataLen, int iNoCompressInForce=0) = 0;
};


#endif
