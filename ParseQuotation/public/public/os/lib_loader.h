/** 跨平台动态库动态加载器CLib类定义
源程序名称: lib_loader.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台动态库动态加载器
	      同时提供一个库名转换工具函数ConvertFileName()，
	      在不同平台下动态库（共享库）名字可以统一采用不带前后缀的名字，由此函数转成实际文件名；
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
*******************************************************************/

#ifndef _LIB_LOADER_INC_
#define _LIB_LOADER_INC_

#include <os/config_env.h>

BEGIN_FBASE2_NAMESPACE

#ifndef _WIN32

///动态库句柄
#define HMODULE		void *

///函数句柄
#define FARPROC		void *

///打开动态库
#define LoadLibrary		dlopen

///取函数地址
#define GetProcAddress	dlsym

///关闭动态库
#define FreeLibrary		dlclose

#endif /* end of no  _win32 */

///动态库加载器
/**
*@可用于win32及支持posix标准的unix平台
*/

class API_EXPORT CLib
{
public:
	///在不同平台下把一个无前后缀的动态库名转换成实际文件名
	/**
	 *@param char * lpNew 输出参数，转换后的实际文件名存放区，调用者准备空间
	 *@param const char *libfile 输入参数，一个无前后缀的动态库名
	 */
	static void ConvertFileName(char * lpNew,const char *libfile);
public:	
	CLib();
	
	/**构造函数
	 *@param _libname  动态库名，不含前后缀
	 */
	CLib(const char * _libname );
	
	///加载一个动态库模块
	/**
	 *@param _libname  动态库名，不含前后缀
	 *@return 加载失败返回NULL， 其他返回句柄
	 */
	HMODULE Load(const char *libname = NULL);
	
	///加载一个动态库中的的函数
	/**
	 *@param hModule  动态库句柄
	 *@param lpProcName 加载的函数名称
	 *@return  加载失败返回NULL， 其他返回句柄
	 */
	FARPROC LoadFunc(const char *ProcName);
	
	///卸载动态库
	/**
	 *@return 0 成功，其他失败
	 */
	int Free();
	
	///取得动态库的句柄
	const HMODULE GetHandle() const;
	
	///取动态加载失败后的错误信息
	/**在windows下返回错误号字符串，在其他平台下返回错误信息；
	 *@retrun 错误信息
	 */
	char * GetLastErrorInfo();
	
	///取得动态库文件名
	const char* GetName() const;
	
	const char* GetPath();
	///是否有效
	bool operator!();
	
	//20110817 dongpf 在加载的时候，采用立即检查符合表，即用RTLD_NOW
	HMODULE LoadEx(const char *libname = NULL);
	
private:
#if defined(_WIN32)
	char *m_szErrInfo;
#endif

	//20110817 dongpf 封装为统一函数
	HMODULE mf_Load(const char *libname, bool bIsLoadLazy);
	
	HMODULE m_hLib;   //动态库句柄
	char  m_szLibName[256];  //动态库名
	char  m_szPath[256];
};

END_FBASE2_NAMESPACE

#endif
