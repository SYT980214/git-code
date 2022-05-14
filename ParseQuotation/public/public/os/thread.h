/**CThread跨平台线程类定义
源程序名称: thread.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台线程封装类
相关文档: 
作者:
审核: (可选的)
	

修改记录
修改日期:
修改人员:
修改说明:

2013-11-15 liuzl 优化CThreadEx在linux下的实现
2013-12-18 liuzl CThreadEx增加StartEx方法，可以设置线程栈大小

*******************************************************************/

#ifndef _THREAD_INC_
#define _THREAD_INC_

#include "config_env.h"
#include "thread_mutex.h"  
#include "inttypes.h"

//#define TRADER_API_EXPORT

BEGIN_FBASE2_NAMESPACE

///一个简单的跨平台线程类定义
/**
 * 使用方法：class myThread:public CThread{...}
 * 注意重载Run函数来完成执行体，可以通过重载构造函数或者增加其他方法来传递参数
 * 实例化对象后，如实例为Obj, 则调用Obj.Start()来启动线程
 * Run()
 * {
 *     ....
 *   }
 */
/**
*可以运行于_win32及支持POSIX标准的unix平台
*/
class API_EXPORT CThread
{
public:
#ifndef _WIN32
    typedef     pthread_t THREAD;
#else
    typedef     HANDLE    THREAD;
#endif
	CThread();
	///启动线程
	int Start();		

	///析构函数，终止线程并释放资源
	virtual ~CThread();

	///取得线程内部句柄
	CThread::THREAD GetId() const
	{
		return m_hThread;
	}	

	///线程是否已经启动
	/**
		*如果状态为true则表示线程启动
	*/
	bool IsRunning()const
	{ 
		return m_bStarted;
	};

	void SetStatus(bool bStatus);
	///检查线程终止状态
	/**
		*如果状态为true则表示终止过程中，可以检测该值以结束线程运行体
	*/
	bool IsTerminated()const
	{
		return m_bTerminateRequest;
	}
	

	/// 增加线程体的循环执行次数
	void IncreaseLoops()
	{
		m_dwLoopTimes++;
		return;
	}
	/// 获得线程循环次数
	uint32 GetLoops()
	{
		return m_dwLoopTimes;
	}

	/// 检测线程是否工作正常(老的循环次数和当前的循环次数比较，两者不一致则说明线程工作
	/// 一切正常，检测的时间间隔必须足够长，否则就会得出错误的结论)
	bool IsActive()
	{
		bool bResult;
		bResult = (m_dwOldLoopTimes != m_dwLoopTimes);
		m_dwOldLoopTimes = m_dwLoopTimes;
		return bResult;
	}

	///线程终止
	virtual void Stop(long dwWait = INFINITE);

	virtual void Signal();
#ifdef _WIN32
	friend static unsigned WINAPI thread_function(void* pArg);
#else
	friend void * thread_function(void *pArg);
#endif

protected:
	///线程执行体，使用者需重载该函数，加入线程执行体
	virtual long Run() = 0;
	///是否中止线程
	bool m_bTerminateRequest;
private:
	///禁止复制和赋值
	CThread( const CThread& );
	CThread& operator=(const CThread& );

	bool m_bStarted;
	///内部线程句柄
    THREAD  m_hThread;
protected:
	//	线程经过的循环次数
	uint32			m_dwLoopTimes;
	uint32			m_dwOldLoopTimes;
	CThreadMutex 	m_Mutex;  //互斥子
};

//---------------------------------------------------------------------------------------------

class API_EXPORT CThreadEx
{
public:
#ifndef _WIN32
    typedef     pthread_t THREAD;
#else
    typedef     HANDLE    THREAD;
#endif

	CThreadEx();
	///启动线程
	int Start();		

    ///2013-12-18 liuzl CThreadEx增加StartEx方法，可以设置线程栈大小
    ///创建并启动线程
    /**
     * @param iThreadStkSize(单位为kb) 线程栈大小
     * @return 0成功 其他失败
     * @note uThreadStkSize只对linux有效，
     *       uThreadStkSize > 0，则取uThreadStkSize
     *       否则取环境变量FBASE_STACK_SIZE，否则为10M
     */
    int StartEx(size_t uThreadStkSize = 0);

	///析构函数，终止线程并释放资源
	virtual ~CThreadEx();

	///取得线程内部句柄
	CThreadEx::THREAD GetId() const
	{
		return m_hThread;
	}	

	///线程是否已经启动
	/**
		*如果状态为true则表示线程启动
	*/
	bool IsRunning()
	{
#if defined(_WIN32)
	    EnterCriticalSection(&m_hMutex);
		bool ret = m_bStarted;
		LeaveCriticalSection(&m_hMutex);
		return ret;
#else
	   	pthread_mutex_lock(&m_CondMutex);
	 	bool ret = m_bStarted;
		pthread_mutex_unlock(&m_CondMutex);
		return ret;
#endif
	};

	void SetStatus(bool bStatus);
	///检查线程终止状态
	/**
		*如果状态为true则表示终止过程中，可以检测该值以结束线程运行体
	*/
	bool IsTerminated() const
	{
		return m_bTerminateRequest;
	}

	/// 增加线程体的循环执行次数
	void IncreaseLoops()
	{
		m_dwLoopTimes++;
		return;
	}
	/// 获得线程循环次数
	uint32 GetLoops()
	{
		return m_dwLoopTimes;
	}

	/// 检测线程是否工作正常(老的循环次数和当前的循环次数比较，两者不一致则说明线程工作
	/// 一切正常，检测的时间间隔必须足够长，否则就会得出错误的结论)
	bool IsActive()
	{
		bool bResult;
		bResult = (m_dwOldLoopTimes != m_dwLoopTimes);
		m_dwOldLoopTimes = m_dwLoopTimes;
		return bResult;
	}
    
    /// 等待一段超时时间
    int WaitCond(long iWait);
	
    // 线程终止, 在目前的实现中, 如果在线程函数中调用自身的Stop, 会造成死锁
	virtual void Stop(long dwWait = INFINITE);

	virtual void Signal();

#if defined(_WIN32)
	friend static unsigned WINAPI thread_function_ex(void* pArg);
#else
    friend void OnThreadExit(void* pArg);
	friend void * thread_function_ex(void *pArg);
#endif

protected:
	///线程执行体，使用者需重载该函数，加入线程执行体
	virtual long Run() = 0;
	///是否中止线程
	bool m_bTerminateRequest;
private:
	///禁止复制和赋值
	CThreadEx( const CThreadEx& );
	CThreadEx& operator=(const CThreadEx& );

	bool m_bStarted;

    THREAD  m_hThread;
protected:
	//	线程经过的循环次数
	uint32			m_dwLoopTimes;
	uint32			m_dwOldLoopTimes;

#if defined(_WIN32)
	CRITICAL_SECTION	m_hMutex;
#else
    pthread_mutex_t		m_CondMutex;
    pthread_cond_t		m_Cond;
    pthread_mutex_t		m_ExitMutex;    // 2013-12-18 liuzl 为兼容保留无用变量
#endif
};

END_FBASE2_NAMESPACE

#endif
