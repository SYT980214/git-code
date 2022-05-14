/**Win32平台环境配置头文件定义
源程序名称: config_win32.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 此文件被config_env.h所包含，不应直接包括此头文件。
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
	20110901
	maoyinjie	
		增加 __FUNCTION__ 的定义
		增加 localtime_r strtok_r的定义
    20140805 liuzl 增加unlink宏

*******************************************************************/

#if !defined _CONFIG_WIN32_INC_ && defined _WIN32
#define _CONFIG_POSIX_INC_

#define _WINSOCKAPI_ // 禁止windows.h包含winsock.h
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <direct.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>

#define USE_IPX			//	支持IPX协议
#include <wsipx.h>

///socket相关定义
#ifndef SOCKET_LEN_TYPE
#define SOCKET_LEN_TYPE			int
#endif

//	20110901 maoyinjie	增加 __FUNCTION__ 的定义
#ifndef __FUNCTION__
#define __FUNCTION__ "null"
#endif
#define __func__ __FUNCTION__

typedef int		socklen_t;

///定义一组与标准c不同名的函数别名
#define strcasecmp			  _stricmp
#define strncasecmp			  _strnicmp
#define snprintf			  _snprintf
#define stat				  _stat
#define access				  _access
#define mkdir                 _mkdir
#define splitpath             _splitpath
#define bzero				  ZeroMemory
#define unlink                _unlink   // 20140805 编译警告

///文件存在
#define F_OK					0
///文件可写
#define W_OK					2
///文件可读
#define R_OK					4
#define DIR_SLASH			'\\'


///清除屏幕的系统调用
#define CLEAR_CMD			"cls"

///动态库文件后缀
#define LIB_POSTFIX		".DLL"
///动态库文件前缀
#define LIB_PREFIX		""

#ifdef _CRT_ ///使用c运行库
# ifdef _USE_BEGINTHREAD
#   define THREAD_CALL                __cdecl
#   define THREAD_HANDLE              uintptr_t
#   define THREAD_RET_T               void
#   define CREATE_THREAD_FAILED       (-1L)
#   define CREATE_THREAD_ERROR        (errno)
#   define CREATE_THREAD(_S,_F,_P)    ((Handle)_beginthread((void (__cdecl *)(void *))_F,_S,(void *)_P))
#   define EXIT_THREAD                _endthread()
#   define CLOSE_HANDLE(x)            1
#   define THREAD_RETURN(x)           return
# else
#   define THREAD_CALL                WINAPI
#   define THREAD_HANDLE              HANDLE
#   define THREAD_RET_T               UINT
#   define CREATE_THREAD_FAILED       (0L)
#   define CREATE_THREAD_ERROR        (errno)
#   define CREATE_THREAD(_S,_F,_P)    ((Handle)_beginthreadex(0,_S,(UINT (WINAPI *)(void *))_F,(void *)_P,0,0))
#   define EXIT_THREAD                _endthreadex(0)
#   define CLOSE_HANDLE(x)            CloseHandle(x)
#   define THREAD_RETURN(x)           return(x)
# endif
#else                ///使用MFC运行库
# define THREAD_CALL                WINAPI
# define THREAD_HANDLE              HANDLE
# define THREAD_RET_T               long
# define CREATE_THREAD_FAILED       (0L)
# define CREATE_THREAD_ERROR        GetLastError()
# define CREATE_THREAD(_S,_F,_P)    ((Handle)CreateThread(0,_S,(long (WINAPI *)(void *))_F,(void *)_P,0,0))
# define EXIT_THREAD                ExitThread(0)
# define CLOSE_HANDLE(x)            CloseHandle(x)
# define THREAD_RETURN(x)           return(x)
#endif

#if !defined( PLUGINS_CALL )
#define PLUGINS_CALL		__stdcall
#endif


#endif
