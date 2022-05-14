/**跨平台信号量 CSemaphore 类定义
源程序名称: semaphore.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台信号量类(有名，无名)
	      在linux/unix下信号量有posix与sys v二类实现，需要在编译时用 USE_SYSV_SEMAPHORES来指定.	      
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
*******************************************************************/

#ifndef SEMAPHORE_TYPE_INC_
#define SEMAPHORE_TYPE_INC_

#include <os/config_env.h>

#if !defined(_WIN32)
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
		/* union semun is defined by including <sys/sem.h> */
	#else
		/* according to X/OPEN we have to define it ourselves */
		union semun {
			int val;					/* value for SETVAL */
			struct semid_ds *buf;		/* buffer for IPC_STAT, IPC_SET */
			unsigned short *array;		/* array for GETALL, SETALL */
										/* Linux specific part: */
			struct seminfo *__buf;		/* buffer for IPC_INFO */
		};
	#endif

#endif

BEGIN_FBASE2_NAMESPACE

///有名信号量名字长度
#define SEM_NAME_LEN 255

///信号量类
/**
 * 可用于win32和支持posix标准的unix平台, 提供线程间或者进程间同步的机制
 *
 *
 * unix平台sysv及posix两种实现的主要区别为
 * sysv 
 * (1) 信号量提供undo特性, 即进程在退出时(包括异常退出), 所有设置undo的操作都会被反转, 
 *     故可避免因为进程崩溃而产生的死锁. 
 * (2) 其次, sysv不提供匿名信号量
 * (3) sysv信号量效率较低, 但性能平稳
 *
 * posix
 * (1) posix无undo特性, 用于多进程同步时, 存在因进程崩溃而导致其它进程死锁的可能
 * (2) posix提供有名(基于文件) 和 匿名(基于内存)
 * (3) 性能较sysv为优, 但不平稳
 *
 */

enum SemStyle{SYSV_STYLE, POSIX_STYLE};

class API_EXPORT CSemaphore
{
public:
	///构造函数
	CSemaphore(SemStyle style = SYSV_STYLE);

	///构造函数，初始化信号量
	/**一般使用这个构造,如果已存在则Open
	 * @param init 初始信号量值
	 * @param sem_name 如果用于进程间同步,则需要使用具名的信号灯,该参数就是指明信号灯的名称
	 * @param style 实现标准, win32平台忽略此参数 
	 				0 sysv 
					1 posix
	 */
	CSemaphore( int init , const char *sem_name = "", SemStyle style = SYSV_STYLE);

	///创建信号量
	/**
	 * @param init 初始信号量值
	 * @param sem_name 如果用于进程间同步，则需要使用具名的信号灯，该参数就是指明信号灯的名称
	 * @return int 成功返回0
	 */
	int Create( int init , const char *sem_name = "");

	/**一般先Create,如果已存在则Open
	 * @param sem_name 如果用于进程间同步，则需要使用具名的信号灯，该参数就是指明信号灯的名称
	 * @return 0,成功，其他失败
	 */
	int Open(const char* sem_name);
 
	///析构函数， 关闭信号量
	~CSemaphore();

	///等待一个信号量,返回时信号量值-1
	int Wait(int iTimeOut = INFINITE) const;

	///尝试等待一个信号,不阻塞
	/**
	 * @return 0表示成功(有信号量,且返回时信号量值-1)，其他失败
	 */
	int TryWait() const;

	///发送一个信号，即信号量值+1
	/**
	 * @return 0表示成功，其他失败
	 */
	int Post() const;

	///重新初始化信号量
	/**
	 * @param init 信号量的初始值
	 */
	void Reset( int init = 0 );

	///测试信号等对象无效
	bool operator!();

	///取信号灯名称
	const char* Name() const;

	///信号灯对象是否有效
	bool IsValid();

protected:
    ///当前信号量的值， 因为目前windows上的实现不能正确取到该值，所以未开放
	int Value() const;
private:
	///信号灯名称
	char m_szName[SEM_NAME_LEN + 1];

#ifdef _WIN32
	HANDLE	m_hWin32_Sem;
#else
	int		m_hSYSV_Sem;
	sem_t*	m_hPOSIX_Sem;

	SemStyle	m_iStyle;
#endif

	CSemaphore(CSemaphore &);
	CSemaphore &operator=(CSemaphore &);
};

END_FBASE2_NAMESPACE
#endif

