///基础服务组件演示服务的实现类 CDemoServiceImpl 定义
/*******************************************************
  源程序名称:DemoServiceImpl.h
  软件著作权:杭州恒生电子股份有限公司
  系统名称:  HZHSGS证券交易应用服务器V2.0
  模块名称:  服务处理模块
  功能说明:  
  
  相关文档:  概要设计接口部分
             
  作    者:  zhangly
  开发日期:  20050718
  //20140919 dongpf 增加设置发送/接收窗口大小函数
  //20150109 huxb 初始化时可以指定侦听网卡地址
*********************************************************/
#ifndef UDP_BROADCAST_H
#define UDP_BROADCAST_H

#include "os/config_env.h"
#include "os/thread.h"
#include "os/netinterface.h"

BEGIN_FBASE2_NAMESPACE

#define MAX_ADDRESS_COUNT 64

///UDP接收回调函数指针
#ifdef _WIN32
	typedef int (__stdcall * FOnUDPReceive ) (void * owner,void * lpData,int iDataLen);
#else
	typedef int (* FOnUDPReceive ) (void * owner,void * lpData,int iDataLen);
#endif

//20110524 dongpf 应王仁云要求，回调返回udp的发送者的ip和端口
///UDP接收回调函数指针
#ifdef _WIN32
	typedef int (__stdcall * FOnUDPReceiveEx ) (void * owner,void * lpData,int iDataLen, const char * szSenderIP, int iPort);
#else
	typedef int (* FOnUDPReceiveEx ) (void * owner,void * lpData,int iDataLen, const char * szSenderIP, int iPort);
#endif


//20110524 dongpf 应王仁云要求，回调返回udp的发送者的ip和端口(改成全局函数指针)
static FOnUDPReceiveEx g_pReceiveFunctionEx = NULL;   //接收回调函数

///UDP广播实现类
class API_EXPORT CUDPBroadcast: public CThread
{
public:
	CUDPBroadcast();
	~CUDPBroadcast();

	/**@ udp 广播接口 */
	//@{
		void Destroy(void);
		///
		/**
		 *
		 *
		 *
		 *@return: 0 成功  -1失败, 套接字初始化失败
		 */
		int Init( unsigned int iPort,FOnUDPReceive pFunction,void * lpOwner=NULL);
		
		void Start(void);
		int Broadcast(char * lpData,int iLen);
		
		//20140919 dongpf 增加设置发送窗口大小函数
		/**
		 *@int iSendBufLen: 窗口大小，单位是字节
		 *@return: 返回实际窗口大小，-1：设置发送窗口大小失败
		 */
		int SetSendBufLen(int iSendBufLen);
		
		//20140919 dongpf 增加设置接收窗口大小函数
		/**
		 *@int iRecvBufLen: 窗口大小，单位是字节
		 *@return: 返回实际窗口大小，-1：设置接收窗口大小失败
		 */
		int SetRecvBufLen(int iRecvBufLen);
	//@}
	
	/**@  */
	//@{
	   long Run();
	//@}
	
	//20110524 dongpf 应王仁云要求，回调返回udp的发送者的ip和端口
	int InitEx( unsigned int iPort,FOnUDPReceiveEx pFunction,void * lpOwner=NULL);
	
    ///20150109 huxb 初始化时可以指定网卡地址
    int InitByAddr(const char * szAddr, unsigned int iPort,FOnUDPReceiveEx pFunction,void * lpOwner=NULL);
private:
	///创建侦听sock
	int CreateSock();

    ///20150109 huxb 创建侦听sock时，可以指定侦听IP
    int CreateSockWithAddr(const char * szAddr);

private:
	
	unsigned int  m_iPort;             //端口号
	FOnUDPReceive m_pReceiveFunction;   //接收回调函数

	void *        m_lpOwner;            //回调处理者
	
	int m_iSock;         //sock
};

END_FBASE2_NAMESPACE

#endif
