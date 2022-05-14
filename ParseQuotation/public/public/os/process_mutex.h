/** 跨平台进程互斥锁CProcessMutex类定义
源程序名称: process_mutex.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台进程互斥锁封装类
	      在windows下使用互斥子实现;
	      在linux/unix下使用有名信号量实现，有posix与sys v二类,
	      需要在编译时用USE_POSIX_SEMAPHORES来指定是否用POSIX标准的信号量,
          使用有名信号量实现，引用semaphore.cpp。
          
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
*******************************************************************/
#ifndef _PROCESS_MUTEX_INC_
#define _PROCESS_MUTEX_INC_

#include <os/config_env.h>
#include <os/semaphore.h>  //用于摸拟进程锁的信号量

BEGIN_FBASE2_NAMESPACE
/// 进程级互斥量
/*
 * 对于UNIX/LINUX平台，进程锁是基于信号量来实现的。而由于POSIX的信号量
 * 在进程异常退出时没有自动重新置位的功能，故为了保证程序逻辑正确性，推
 * 荐使用SYSV信号量。此版本的进程锁及信号量默认使用SYSV信号量来实现。若
 * 用户需要使用POSIX信号量，必须在编译时定义 USE_POSIX_SEMAPHORES 宏
 */
class API_EXPORT CProcessMutex
{
public:
	///构造函数
    CProcessMutex();
    
    ///构造函数
    /**
     *@param lpName, 锁名称，必须全局唯一
     */
    CProcessMutex(const char lpName[]);

    ///初始化函数
    /**
     *@param name, 锁名称，用于区分不同的锁实例
     *@return 返回非0表示失败
     */
    int Init(const char name[]);
    
    ///析构函数
    ~CProcessMutex();

    ///锁定
    /**
        *@return 0表示成功，
    */
    int Acquire();
    ///尝试获得锁，如果得不到立即返回
    /**
     * @return 0表示成功得到锁，其实失败
     */

    int TryAcquire();
    ///释放
    /**
        *@return 0成功，其他失败
    */
    int Release();

private:
	///内部对象句柄，不同平台不一样
#ifdef _WIN32
    HANDLE     m_hMutex;   
#else

    CSemaphore * m_lpSem;
#endif

};

class CProcAutoLock
{
public:
    CProcAutoLock( CProcessMutex *pm )
    {
        lpMut = pm;
        lpMut->Acquire();
    }
    ~CProcAutoLock()
    {
        lpMut->Release();
    }
private:
    CProcessMutex *lpMut;
};


END_FBASE2_NAMESPACE

#endif

