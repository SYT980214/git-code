/** @file
* 业务消息的接口文件
* @author  许欣芃
* @author  恒生电子股份有限公司
* @version 1.0
* @date    20130625
* 20150114 xuxp 增加公司编号获取设置接口
*/
#ifndef BIZ_MESSAGE_INTERFACE_H
#define BIZ_MESSAGE_INTERFACE_H

#if defined(_WIN32)&& _MSC_VER < 1300
#pragma warning(disable: 4786)				//需加在被包含的STL头文件前
#pragma warning(disable: 4710)				//需加在被包含的STL头文件前
#endif
#include <string.h>
#include "plugin_interface.h"
#include "tag_def.h"

///ESB组名长度，名字为可见字符，不能包含实例分隔符、空格、分号;
#define IDENTITY_NAME_LENGTH	32
///实例编号最大占位长度
#define ID_LENGTH               4 
///节点名全长,定义时使用char sName[ID_STR_LEN+1]
#define ID_STR_LEN		       (IDENTITY_NAME_LENGTH + ID_LENGTH + 1)


//	插件接口名的最大长度,定义时使用char sName[PLUGINID_LENGTH+1]
#define PLUGINID_LENGTH	256
//	插件实例名的最大长度,定义时使用char sName[PLUGIN_NAME_LENGTH+1]
#define PLUGIN_NAME_LENGTH	(PLUGINID_LENGTH+ID_LENGTH+1)
//	进程名最大长度.定义时使用char sName[SVR_NAME_LENGTH+1]
#define SVR_NAME_LENGTH	256
//	进程实例名最大长度.定义时使用char sName[PLUGINID_NAME_LENGTH+1]
#define SVRINSTANCE_NAME_LENGTH	(SVR_NAME_LENGTH+ID_LENGTH+1)

typedef struct tagBizRouteInfo
{
	char ospfName[ID_STR_LEN+1];//路由目标节点中间件名字
	char nbrName[ID_STR_LEN+1];//中间件节点的邻居名字
	char svrName[SVRINSTANCE_NAME_LENGTH+1];//中间件的进程名字
	char pluginID[PLUGIN_NAME_LENGTH+1];//中间件插件名
	int connectID;//连接号
	int memberNO;//成员编号
	
	tagBizRouteInfo()
	{
		memset(this,0,sizeof(tagBizRouteInfo));
	}
}BIZROUTE_INFO;


struct IBizMessage : public IKnown
{
	//设置功能号
	virtual void FUNCTION_CALL_MODE SetFunction(const int nFUnctionNo) = 0;
	//获取功能号
	virtual int FUNCTION_CALL_MODE GetFunction() = 0;

	//设置包类型
	virtual void FUNCTION_CALL_MODE SetPacketType(const int nPacketType) = 0;
	//获取包类型
	virtual int FUNCTION_CALL_MODE GetPacketType() = 0;

	//设置营业部号
	virtual void FUNCTION_CALL_MODE SetBranchNo(const int nBranchNo) = 0;
	//获取营业部号
	virtual int FUNCTION_CALL_MODE GetBranchNo() = 0;

	//设置系统号
	virtual void FUNCTION_CALL_MODE SetSystemNo(const int nSystemNo) = 0;
	//获取系统号
	virtual int FUNCTION_CALL_MODE GetSystemNo() = 0;

	//设置子系统号
	virtual void FUNCTION_CALL_MODE SetSubSystemNo(const int nSubSystemNo) = 0;
	//获取子系统号
	virtual int FUNCTION_CALL_MODE GetSubSystemNo() = 0;

	//设置发送者编号
	virtual void FUNCTION_CALL_MODE SetSenderId(const int nSenderId) = 0;
	//获取发送者编号
	virtual int FUNCTION_CALL_MODE GetSenderId() = 0;

	//设置包序号
	virtual void FUNCTION_CALL_MODE SetPacketId(const int nPacketId) = 0;
	//获取包序号
	virtual int FUNCTION_CALL_MODE GetPacketId() = 0;

	//设置目的地路由
	virtual void FUNCTION_CALL_MODE SetTargetInfo(const BIZROUTE_INFO targetInfo) = 0;
	//获取目的地路由
	virtual void FUNCTION_CALL_MODE GetTargetInfo(BIZROUTE_INFO& targetInfo) = 0;
	
	//设置发送者路由
	virtual void FUNCTION_CALL_MODE SetSendInfo(const BIZROUTE_INFO sendInfo) = 0;
	//获取发送者路由
	virtual void FUNCTION_CALL_MODE GetSendInfo(BIZROUTE_INFO& sendInfo) = 0;

	//设置错误号
	virtual void FUNCTION_CALL_MODE SetErrorNo(const int nErrorNo) = 0;
	//获取错误号
	virtual int FUNCTION_CALL_MODE GetErrorNo() = 0;
	
	//设置错误信息
	virtual void FUNCTION_CALL_MODE SetErrorInfo(const char* strErrorInfo) = 0;
	//获取错误信息
	virtual const char* FUNCTION_CALL_MODE GetErrorInfo() = 0;
	
	//设置返回码
	virtual void FUNCTION_CALL_MODE SetReturnCode(const int nReturnCode) = 0;
	//获取返回码
	virtual int FUNCTION_CALL_MODE GetReturnCode() = 0;

	//设置业务内容
	virtual void FUNCTION_CALL_MODE SetContent(void* lpContent,int iLen) = 0;
	//获取业务内容
	virtual const void* FUNCTION_CALL_MODE GetContent(int& iLen) = 0;

	//以下接口用于消息中心1.0的订阅
	//设置订阅类型
	virtual void FUNCTION_CALL_MODE SetIssueType(const int nIssueType) = 0;
	//获取订阅类型
	virtual int FUNCTION_CALL_MODE GetIssueType() = 0;

	//设置序号
	virtual void FUNCTION_CALL_MODE SetSequeceNo(const int nSequeceNo) = 0;
	//获取序号
	virtual int FUNCTION_CALL_MODE GetSequeceNo() = 0;

	//设置关键字段信息
	virtual void FUNCTION_CALL_MODE SetKeyInfo(void* lpKeyData,int iLen) = 0;
	//获取关键字段信息
	virtual const void* FUNCTION_CALL_MODE GetKeyInfo(int& iLen) = 0;

	//设置附加数据，订阅推送时原样返回
	virtual void FUNCTION_CALL_MODE SetAppData(const void* lpAppdata,int nAppLen) = 0;
	//获取附加数据，订阅推送时原样返回
	virtual const void* FUNCTION_CALL_MODE GetAppData(int& nAppLen) = 0;

	//请求转应答
	virtual int	FUNCTION_CALL_MODE ChangeReq2AnsMessage() = 0;

	//获取二进制
	virtual void* FUNCTION_CALL_MODE GetBuff(int& nBuffLen) = 0;
	//解析二进制
	virtual int	FUNCTION_CALL_MODE SetBuff(const void* lpBuff,int nBuffLen) = 0;

	//20131227 xuxp 增加Reset接口，确保消息可以重复使用
	//清除消息内的字段，可以下次复用。
	virtual void FUNCTION_CALL_MODE ReSet() = 0;

	//20150114 xuxp 增加公司编号的获取和增加接口
	//设置公司编号
	virtual void FUNCTION_CALL_MODE SetCompanyID(const int nCompanyID) = 0;
	//获取公司编号
	virtual int FUNCTION_CALL_MODE GetCompanyID() = 0;

	//20150320 xuxp 增加发送者机构编号的获取和增加接口
	//设置发送者公司编号
	virtual void FUNCTION_CALL_MODE SetSenderCompanyID(const int nSenderCompanyID) = 0;
	//获取发送者公司编号
	virtual int FUNCTION_CALL_MODE GetSenderCompanyID() = 0;
};


#endif
