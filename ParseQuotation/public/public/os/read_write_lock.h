/** 跨平台事件 CReadWriteLock 类定义
源程序名称: read_write_lock.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台读写锁封装类
相关文档: 
作者:
审核: (可选的)

修改记录
修改日期:
修改人员:
修改说明:

2014-03-13 liuzl 修改CReadWriteLock在Windows下的bug（释放锁的读者线程可能不是加锁的读者线程）
                 同时保持兼容性

*******************************************************************/
#ifndef READ_WRITE_LOCK_INC_
#define READ_WRITE_LOCK_INC_

#include "config_env.h"

BEGIN_FBASE2_NAMESPACE

///读写锁定义
class API_EXPORT CReadWriteLock
{
public:
	CReadWriteLock();
	~CReadWriteLock();

	///获得读锁
	/**
	 *@return 非零 表示失败，原因需检查系统错误， 0表示成功
	 */
	int ReadAcquire();

	///释放读锁
	/**
	 *@return 非零 表示失败，原因需检查系统错误， 0表示成功
	 */
	int ReadRelease();

	///获得写锁
	/**
	 *@return 非零 表示失败，原因需检查系统错误， 0表示成功
	 */
	int WriteAcquire();

	///释放写锁
	/**
	 *@return 非零 表示失败，原因需检查系统错误， 0表示成功
	 */
	int WriteRelease();
private:
#ifdef _WIN32
	///读线程数
	long				m_nReaderCount;
	///保护读者数的临界区
	CRITICAL_SECTION	m_Mutex;
	
    // 2014-03-13 liuzl 保证对象大小和对齐方式和以前一致
    CRITICAL_SECTION	m_hDataLock;
#else			//for posix unix
	///读写锁对象
	pthread_rwlock_t	m_Lock;
#endif
	///封闭掉下面函数
	CReadWriteLock(CReadWriteLock &lock);
	CReadWriteLock & operator=(CReadWriteLock &lock);
};

END_FBASE2_NAMESPACE

#endif
