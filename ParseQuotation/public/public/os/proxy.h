/******************************************************************************
源文件名：proxy.h
软件版权：恒生电子股份有限公司
系统名称：
功能说明：代理接口
作    者：feisi@hundsun.com
创建时间：20080102
备    注：
修 改 者：
修改时间：
修改简介：
******************************************************************************/

#ifndef _PROXY_H_
#define _PROXY_H_



/******************************************************************************
*
*	头文件
*
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#if defined _WIN32
	#include <Winsock2.h>
	#include <Windows.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
#endif

#if defined _WIN32
	typedef int socklen_t;
#endif



/******************************************************************************
*
*	常量定义
*
******************************************************************************/
// 函数成功返回
const int SUCCESS = 1;
// select函数超时时间
const long SELECT_TIME = 5L;
// SOCKS5中使用的接收或发送UDP报文的数据缓冲区大小
const unsigned long SOCKS5_UDP_BUFSIZE = 65535UL;

const unsigned char SOCKS5_CMD_TCP  = 0x01;   // SOCKS5代理TCP连接请求
const unsigned char SOCKS5_CMD_UDP  = 0x03;   // SOCKS5代理UDP连接请求

const int ERR_SELECT_ERROR          = -101;   // select函数出错
const int ERR_SELECT_TIMEOUT        = -102;   // select函数超时
const int ERR_SOCKET_SEND_ERROR     = -103;   // 系统socket send函数出错
const int ERR_SOCKET_READ_ERROR     = -104;   // 系统socket recv函数出错
const int ERR_SOCKET_READ_EOF       = -105;   // 读到对方关闭套接字标志
const int ERR_CONNECT_PROXY_SERVER  = -106;   // 无法连接到代理服务器

/* HTTP error */
const int ERR_HTTP_PACK_CONNECT_REQUEST            = -301;   // 组织请求包出错
const int ERR_HTTP_SEND_CONNECT_REQUEST            = -302;   // 发送请求包出错
const int ERR_HTTP_RECV_CONNECT_REPLY              = -303;   // 接收服务器应答包出错
const int ERR_HTTP_VERIFY_CONNECT_REPLY            = -304;   // 验证服务器应答包出错

/* SOCKS4 error */
const int ERR_SOCKS4_PACK_CONNECT_REQUEST          = -401;   // 组织请求包出错
const int ERR_SOCKS4_SEND_CONNECT_REQUEST          = -402;   // 发送请求包出错
const int ERR_SOCKS4_RECV_CONNECT_REPLY            = -403;   // 接收服务器应答包出错
const int ERR_SOCKS4_VERIFY_CONNECT_REPLY          = -404;   // 验证服务器应答包出错

/* SOCKS5 error */
const int ERR_SOCKS5_SEND_METHOD_SELECTION_REQUEST = -501;   // "登录方式"请求报文发送失败
const int ERR_SOCKS5_RECV_METHOD_SELECTION_REPLY   = -502;   // "登录方式"应答报文接收失败
const int ERR_SOCKS5_VERIFY_METHOD_SELECTION_REPLY = -503;   // "登录方式"被服务器拒绝
const int ERR_SOCKS5_PACK_USR_PSWD_REQUEST         = -504;   // "用户名+密码"报文组包出错
const int ERR_SOCKS5_SEND_USR_PSWD_REQUEST         = -505;   // "用户名+密码"报文发送失败
const int ERR_SOCKS5_RECV_USR_PSWD_REPLY           = -506;   // "用户名+密码"应答报文接收失败
const int ERR_SOCKS5_VERIFY_USR_PSWD_REPLY         = -507;   // "用户名+密码"验证被服务器拒绝
const int ERR_SOCKS5_PACK_CONNECT_REQUEST          = -508;   // "连接请求"报文组包失败
const int ERR_SOCKS5_SEND_CONNECT_REQUEST          = -509;   // "连接请求"报文发送失败
const int ERR_SOCKS5_RECV_CONNECT_REPLY            = -510;   // "连接请求应答"报文接收失败
const int ERR_SOCKS5_VERIFY_CONNECT_REPLY          = -511;   // "连接请求应答"报文验证失败
const int ERR_SOCKS5_UDP_INIT                      = -512;   // SOCKS5 UDP连接初始化失败
const int ERR_SOCKS5_UDP_FRAGMENTATION             = -513;   // UDP不支持报文分片
const int ERR_SOCKS5_UDP_SEND                      = -514;   // UDP报文发送失败
const int ERR_SOCKS5_UDP_RECV                      = -515;   // UDP报文接收失败
const int ERR_SOCKS5_UDP_VERIFY_PACKET             = -516;   // SOCKS5 UDP报文头验证失败
const int ERR_SOCKS5_UDP_INBUF_INSUFFICIENT        = -517;   // 用户接收缓存不足
const int ERR_SOCKS5_UDP_NOT_IPv4                  = -518;   // 发现非IPv4地址

#if defined(_WIN32) && !defined(SEPARATE_COMPILE)
	#ifdef DLL_API_EXPORT
		#define API_EXPORT __declspec(dllexport)
	#else
		#define API_EXPORT __declspec(dllimport)
	#endif
#else
	#define API_EXPORT 
#endif

/******************************************************************************
*
*	代理TCP相关接口
*
******************************************************************************/
/// 基于SOCKS4代理协议产生TCP连接套接字
/** @param serv_addr: 代理服务器地址
  * @param serv_port: 代理服务器端口
  * @param dest_addr: 目的主机地址
  * @param dest_port: 目的主机端口
  *
  * @return int:      成功 > 0, 可用套接字
  *                   失败 < 0, 错误号
  */
extern "C" int API_EXPORT socks4_connect(const char *serv_addr, unsigned short serv_port,
							  const char *dest_addr, unsigned short dest_port);



/// 基于HTTP代理协议产生TCP连接套接字
/** @param serv_addr: 代理服务器地址
  * @param serv_port: 代理服务器端口
  * @param dest_addr: 目的主机地址
  * @param dest_port: 目的主机端口
  * @param user_name: 代理登录用户(匿名登录时设为NULL)
  * @param password:  代理登录密码(匿名登录时设为NULL)
  *
  * @return int:      成功 > 0, 可用套接字
  *                   失败 < 0, 错误号
  */
extern "C" int API_EXPORT http_connect(const char *serv_addr, unsigned short serv_port,
							const char *dest_addr, unsigned short dest_port,
							const char *user_name, const char *password);



/// 基于SOCKS5代理协议产生TCP连接套接字
/** @param serv_addr: 代理服务器地址
  * @param serv_port: 代理服务器端口
  * @param dest_addr: 目的主机地址
  * @param dest_port: 目的主机端口
  * @param user_name: 代理登录用户(匿名登录时设为NULL)
  * @param password:  代理登录密码(匿名登录时设为NULL)
  *
  * @return int:      成功 > 0, 可用套接字
  *                   失败 < 0, 错误号
  */
extern "C" int API_EXPORT socks5_tcp_connect(const char *serv_addr, unsigned short serv_port,
								  const char *dest_addr, unsigned short dest_port,
								  const char *user_name, const char *password);



/******************************************************************************
*
*	代理UDP相关接口
*
******************************************************************************/
/*

注: 通过SOCKS5代理协议进行UDP报文的转发时, 在"客户端"和"代理服务器端"间发送UDP
报文时, 会有一个额外的报文头部, 如下所示:

      +----+------+------+----------+----------+----------+
      |RSV | FRAG | ATYP | DST.ADDR | DST.PORT |   DATA   |
      +----+------+------+----------+----------+----------+
      | 2  |  1   |  1   | Variable |    2     | Variable |
      +----+------+------+----------+----------+----------+

其中:	RSV  - 保留位, 必须全0
		FRAG - 分片标志位, 目前实现中不支持分片, 故值也为0
		ATYP - 地址类型, 目前实现只支持ipv4地址

		DST.ADDR - 发送时, 表明最终目的地址(而非代理服务地址)
		           接收时, 表示真正来源地址(而非代理服务地址)
		DST.PORT - 端口, 含义同上

		DATA - 用户真正的应用数据

本实现中: Socks5Udp::SendTo, Socks5Udp::RecvFrom - 已经在函数内部处理了头部信息
                                                   用户只需要关心自己的数据即可

!!!在使用上与系统socket函数中的sendto, recvfrom用法一致!!!

*/

class API_EXPORT Socks5Udp
{
	public:
		/// 基于SOCKS5代理的UDP操作初始化
		/** @param serv_addr: 代理服务器地址
		  * @param serv_port: 代理服务器端口
		  * @param user_name: 代理登录用户(匿名登录时设为NULL)
		  * @param password:  代理登录密码(匿名登录时设为NULL)
		  *
		  * @return int:      成功 == SUCCESS
		  *                   失败 < 0, 错误号
		  */
		int UdpInit(const char *serv_addr, unsigned short serv_port, const char *user_name, const char *password);

		/// 基于SOCKS5代理的UDP数据报发送函数(目前实现只支持IPv4地址)
		/** @param buf:		待发送数据缓冲区首地址
		  * @param len:		待发送数据长度
		  * @param flags:	发送选项(其含义与系统socket中sendto函数意义一致), 一般可传入0
		  * @param to:		数据要发往的目的地址结构体(其含义与系统socket中sendto函数意义一致)
		  * @param tolen:	数据要发往的目的地址结构体的长度(其含义与系统socket中sendto函数意义一致)
		  *
		  * @return int:      成功返回实际发送的字节数
		  *                   失败 < 0, 错误号
		  */
		int SendTo(const char *buf, int len, int flags, const struct sockaddr *to, int tolen);

		/// 基于SOCKS5代理的UDP数据报接收函数(目前实现只支持IPv4地址)
		/** @param buf:		接收数据缓冲区首地址
		  * @param len:		接收数据缓冲区长度
		  * @param flags:	接收选项(其含义与系统socket中recvfrom函数意义一致), 一般可传入0
		  * @param to:		数据真正来源地址结构体(其含义与系统socket中recvfrom函数意义一致)
		  * @param tolen:	数据真正来源地址结构体的长度(其含义与系统socket中recvfrom函数意义一致)
		  *
		  * @return int:      成功返回实际接收的字节数
		  *                   失败 < 0, 错误号
		  *                   注: 错误号为"ERR_SOCKS5_UDP_INBUF_INSUFFICIENT"时, 表明用户提供接收
		  *                       缓存不足, 可扩大缓存再取回数据
		  */
		int RecvFrom(char *buf, int len, int flags, struct sockaddr *from, int *fromlen);

		/// 关闭内部维护的2个套接字(1.TCP - 控制用 2.UDP - 转发报文用)
		/**
		  * @return int: SUCCESS, 目前总是返回成功
		  *
		  */
		int CloseSocket(void);

		/// 获得内部维护的UDP套接字
		/**
		  * @return int: UDP套接字
		  *
		  */
		int GetUdpHandle(void) const;

		/// 默认构造函数
		Socks5Udp();

		/// 析构函数
		~Socks5Udp();

	private:
		Socks5Udp(const Socks5Udp &);   // 禁止拷贝构造函数
		Socks5Udp & operator=(const Socks5Udp &);   // 禁止"="号赋值

	private:
		int tcp_fd;                    // TCP套接字, 初期的协商, 请求, 应答 ...; 控制用
		int udp_fd;                    // UDP套接字, 发送UDP报文用
		struct sockaddr_in udp_serv;   // SOCKS5代理服务器用来转发UDP报文使用的地址信息
		bool m_bAlreadyReaded;                  // 用于控制是否需要从底层读取报文
		char m_szRecvBuf[SOCKS5_UDP_BUFSIZE];   // 接收UDP包的内部缓存
		int m_iPackSize;                        // 接收到包的大小
};



#endif   // _PROXY_H_
