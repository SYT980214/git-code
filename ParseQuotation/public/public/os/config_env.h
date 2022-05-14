/**跨平台环境配置头文件定义
源程序名称: config.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 根据各平台开发环境下已定义的宏包括常用头文件，统一定义常用宏.
	      各个跨平台封装对象应首先包含本文件，本文件引用config_win32.h，config_posix.h。
	      可引用的编译器环境宏如下：
	         _MSC_VER     MS VC编译器版本
	         __cplusplus  c++编译器
	         __GNUC__     gcc/g++编译器
	         __HP_aCC     HP aCC编译器
	         __IBMCPP__	  IBM VACPP编译器
	      
	      可引用的操作系统环境宏如下：	         
	         _WIN32       Win32类操作系统
	         __linux__    linux类操作系统
	         __hpux       HPUX类操作系统
	         _AIX         IBM AIX类操作系统
	         __sparc__    SUN Solaris操作系统
	         sun          SUN Solaris操作系统,建议用__sparc__来识别
	    
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:

20140612 liuzl  消除linux下TRACE编译警告
*******************************************************************/

#ifndef _CONFIG_INC_
#define _CONFIG_INC_

/// the all config files include

#ifndef __cplusplus
#error "the fbase lib only be used in c++"
#endif

#if defined (_MSC_VER) && _MSC_VER < 1200
#error "please use this header only over MSVC 6.0 if using vc compiler"
#endif


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

///本项目命名空间定义
#define FBASE_NAMESPACE			FBASE
#define FBASE2_NAMESPACE		FBASE2
///define the namesapce
#define NAMESPACE_BEGIN(x)		namespace x {
#define NAMESPACE_END			}
#define USING_NAMESPACE(x)		using namespace x;

#define BEGIN_FBASE_NAMESPACE	NAMESPACE_BEGIN(FBASE_NAMESPACE)
#define END_FBASE_NAMESPACE		NAMESPACE_END
#define USING_FBASE_NAMESPACE	USING_NAMESPACE(FBASE_NAMESPACE)

#define BEGIN_FBASE2_NAMESPACE	NAMESPACE_BEGIN(FBASE2_NAMESPACE)
#define END_FBASE2_NAMESPACE	NAMESPACE_END
#define USING_FBASE2_NAMESPACE	USING_NAMESPACE(FBASE2_NAMESPACE)

BEGIN_FBASE_NAMESPACE
END_FBASE_NAMESPACE

BEGIN_FBASE2_NAMESPACE
END_FBASE2_NAMESPACE

typedef unsigned char BYTE;         ///sizeof(BYTE) = 1
typedef unsigned short WORD;        ///sizeof(WORD) = 2

///无效的句柄
#define InvalidHandle 0L

#ifdef _WIN32
#pragma message("_WIN32 macro activated!")

#if defined(_WIN32)&& _MSC_VER < 1300
#pragma warning(disable: 4786)				// 需加在被包含的STL头文件前
#pragma warning(disable: 4976)
#pragma warning(disable: 4291)				// 忽略使用 new(nothrow)的异常
#endif


#include "config_win32.h"
#else  ///for unix or linux
#include "config_posix.h"
#endif ///end of _WIN32

//TRACE宏定义，在_DEBUG下输出调试信息
#if defined (_DEBUG) && !defined(TRACE)
#define TRACE			printf
#else
// 20140612 liuzl 消除linux下编译警告
#ifdef _WIN32
#define TRACE			(void)
#else
#define TRACE(...)  ((void)0)
#endif
#endif

//当编译器支持C++模板分离模式（本项目所用编译器均不支持）时，定义USE_EXPORT
/*alog.h,alog.cpp等用到，不支持分离模式时，注意此类cpp（模板类的实现代码）不应直接编译cpp*/

#if defined(USE_EXPORT)
#define TEMPLATE_EXPORT		export
#else
#define TEMPLATE_EXPORT
#endif

//
// 需要对该目录下的源文件单独编译时, 需定义 SEPARATE_COMPILE 宏
#if defined(_WIN32) && !defined(SEPARATE_COMPILE)
	#ifdef DLL_API_EXPORT
		#define API_EXPORT __declspec(dllexport)
	#else
		#define API_EXPORT __declspec(dllimport)
	#endif
#else
	#define API_EXPORT
#endif

#include <new>

#define FBASE_NEW new(std::nothrow)

#endif
