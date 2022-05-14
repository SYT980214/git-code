/**跨平台文件锁 CFileLock 类定义
源程序名称: filelock.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台文件锁 

相关文档: 
作者:
审核: (可选的)
*******************************************************************/


#ifndef _FILELOCK_INC_
#define _FILELOCK_INC_

#if defined(_MSC_VER)
#pragma warning(disable:4284)
#endif

#include <os/config_env.h>
#ifndef _WIN32
#include <sys/file.h>
#endif

BEGIN_FBASE2_NAMESPACE

/// 文件锁定义，可用于模拟进程锁
class API_EXPORT CFileLock
{
public:
    ///构造函数，建立或者打开文件，如果已经被创建，则打开
    /**
     * @param lpFileName 文件名
     */
    CFileLock(const char * lpFileName);

	///析构函数，释放占用的资源
	~CFileLock();

	///尝试加排他锁，非阻塞模式
	/**
     * @return 如果成功则true，否则返回false。
    */
	bool tryLockExclusive();

	///加排他锁，阻塞模式
	/**
     * @return 如果成功则true，否则返回false。
    */
	bool lockExclusive();
	
	///尝试加共享锁，非阻塞模式
	/**
     * @return 如果成功则true，否则返回false。
    */
	bool tryLockShare();

	///加读锁，阻塞模式
	/**
     * @return 如果成功则true，否则返回false。
    */
	bool lockShare();

	///解共享锁或者排他锁
	/**
     * @return 如果成功则true，否则返回false。
    */
	bool unlock();

	///是否可用
	bool isAvailable();
private:
#ifdef _WIN32
	//文件锁句柄
	HANDLE m_pHandle;
	OVERLAPPED m_Oapped;
#else
	///内核对象的句柄
	int m_iLockfd;
#endif
};

END_FBASE2_NAMESPACE

#endif
