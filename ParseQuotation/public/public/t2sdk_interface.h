
#ifndef _T2SDK_INTERNAL_INTERFACE_H
#define _T2SDK_INTERNAL_INTERFACE_H
//头文件
#include "Include/connection_interface.h"
#include "Include/esb_message_interface.h"
#include "Include/bizmsg_interface.h"
#include "Include/pack_interface.h"
#include "Include/tag_def.h"
#include "Include/fileupdate_interface_sdk.h"

//T2_SDK的引出函数
extern "C"
{
     /**
	* 获取T2_SDK的版本号
	* @return 当前T2_SDK的版本号
	*/
int FUNCTION_CALL_MODE GetVersionInfo();

    /**
	* 获取一个实现CConfigInterface接口的类的指针
	* @return 实现CConfigInterface接口的类的指针
	*/
CConfigInterface* FUNCTION_CALL_MODE NewConfig();

    /**
	* 获取一个实现CConnectionInterface接口的类的指针
	* @param CConfigInterface 实现CConfigInterface接口的类的指针
	* @return实现CConnectionInterface接口的类的指针
	*/
CConnectionInterface* FUNCTION_CALL_MODE NewConnection(CConfigInterface *lpConfig);

    /**
	* 返回一个新的ESBMessage对象指针
	* @param int nCompressID指定对ESBMessage采用的压缩算法（目前支持1），0表示不压缩
	* @return 接口的类的指针
	*/
IESBMessage*  FUNCTION_CALL_MODE NewESBMessage(int nCompressID = 1);
    /**
	* 返回一个新的ESBMessage对象指针，并且把相关参数放入对象中
	* @param int iFunID 功能号
	* @param IF2Packer *lpPacker 业务体
	* @param int iSystemNo 系统号
	* @param int nCompressID指定对ESBMessage采用的压缩算法（目前支持1），0表示不压缩
	* @return 接口的类的指针
	*/
IESBMessage * FUNCTION_CALL_MODE NewESBMessageEx(int iFunID, IF2Packer *lpPacker, int iSystemNo = 0, int nCompressID = 1);
/**
* @param int iVersion 业务包格式版本(取值:1 字串版,其他值 0x20版)
* 推荐使用0x20版
* @return IPacker * 打包器接口指针
*/
IF2Packer * FUNCTION_CALL_MODE NewPacker(int iVersion);

/**
* @param void * lpBuffer 要解包的数据（不含AR通信包头）
* @param unsigned int iLen 数据长度
* @return IUnPacker * 版本2结果集操作接口指针
*/
IF2UnPacker * FUNCTION_CALL_MODE NewUnPacker(void * lpBuffer, unsigned int iLen);

/**
* @param void * lpBuffer 要解包的数据（不含AR通信包头）
* @param unsigned int iLen 数据长度
* @return IUnPacker * 版本1结果集操作接口指针
*/
IF2UnPacker * FUNCTION_CALL_MODE NewUnPackerV1(void * lpBuffer, unsigned int iLen);

/**
* @param void * lpBuffer 要解包的数据（不含AR通信包头）
* @return 1表示版本1的结果集数据，0x21~0x2F 版本2的结果集数据
*/
int FUNCTION_CALL_MODE GetPackVersion(const void *lpBuffer);

/**
* @param char *EncodePass 传出的散列结果，字符串，长度不超过16（包括'\0'）
* @param const char* Password 传入的待散列的密码
* @param int nReserve 保留参数，采用默认值
* @return char * 传出的散列结果地址，同EncodePass
*/
char * FUNCTION_CALL_MODE Encode(char *EncodePass, const char* Password, int nReserve = 0 );

/**
* @return SSL模式下SSL证书ID
*/
const char* FUNCTION_CALL_MODE GetCertID();

 /**
* @return SSL模式下SSL证书有效期
*/
int FUNCTION_CALL_MODE GetCertValid();

/**
* 构造一个过滤器接口指针
* @return 返回NULL表示失败.
*/
CFilterInterface* FUNCTION_CALL_MODE NewFilter();
    
/**
* 构造一个订阅属性接口指针
* @return 返回NULL表示失败.
*/
CSubscribeParamInterface* FUNCTION_CALL_MODE NewSubscribeParam();

//20130625 xuxp 构造业务消息
IBizMessage* FUNCTION_CALL_MODE NewBizMessage();

//20160317 zhouwh 构造业务消息，添加压缩标志
IBizMessage* FUNCTION_CALL_MODE NewBizMessageEx(int nCompressID = 1); 
//测速
int FUNCTION_CALL_MODE GetNetWorkSpeed(char * lpAddress, int port, IF2Packer * lpResult);   
//批量测试速度
int FUNCTION_CALL_MODE GetMultiNetWorkSpeed(IF2UnPacker * lpRequest, IF2Packer * lpResult, unsigned int uiTimeOut);   
//获取发送和接收总字节数
int FUNCTION_CALL_MODE GetRateBytes(int * iSend, int * iRecv);
//重置字节数
void FUNCTION_CALL_MODE ReSetRateBytes();
//New一个空解包器
IF2UnPacker * FUNCTION_CALL_MODE NewEmptyUnPacker(int iVersion);
 
};



#endif
