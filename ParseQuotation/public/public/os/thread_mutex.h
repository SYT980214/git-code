/** 跨平台线程互斥锁CThreadMutex及自动锁CAutoMutex类定义
源程序名称: thread_mutex.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台线程互斥锁封装类，在互斥锁基础上包装了一个自动锁类
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
*******************************************************************/


#ifndef _THREAD_MUTEX_INC_
#define _THREAD_MUTEX_INC_

#include "config_env.h"

BEGIN_FBASE2_NAMESPACE

///跨平台的线程互斥锁定义
class API_EXPORT CThreadMutex
{
public:
#ifndef _WIN32
    typedef pthread_mutex_t     THREADMUTEX;
#else
    typedef CRITICAL_SECTION    THREADMUTEX;
#endif

	///构造函数，初始化临界区
	CThreadMutex();
	///析构函数， 清除临界区
	~CThreadMutex();

	///进入临界区
	void Acquire();
	
	///离开临界区
	void Release();

#if (defined (_WIN32_WINNT) && (_WIN32_WINNT > 0x0400)) || !defined(_WIN32)
	///尝试进入临界区，失败返回非零
	int TryAcquire();
#endif

private:
	///核心互斥量句柄
	THREADMUTEX  m_hMutex;
};

///自动锁类
/**定义一个局部包装类对象，定义是加读锁，超出范围时局部对象释放，读锁也自动释放；
 * 当一个函数内加锁后，确保此函数任何退出点锁都会释放，以避免死锁 
 * 注意：要防止有些编译器把未被引用的局部对象优化掉.
 */
class API_EXPORT CAutoMutex
{
public:
	CAutoMutex(CThreadMutex * pMutex)
	{
		m_pMutex=pMutex;
		pMutex->Acquire();
	};
	~CAutoMutex()
	{
		m_pMutex->Release();
	};
private:
	CThreadMutex * m_pMutex;
};

END_FBASE2_NAMESPACE

#endif

