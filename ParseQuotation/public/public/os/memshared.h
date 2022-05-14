/**跨平台进程间共享内存 CSharedMem 类定义
源程序名称: memshared.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台进程间共享内存类
	      可用于WIN32和支持posix或sys v标准的linux/unix平台
	      如果系统不支持posix具名的内存映射则使用 sys v 地实现，如早期的linux
	      在linux/unix下，使用sys v标准，则需要定义 USE_SYSV_SHAREMEM

相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期: 20140121
修改人员: dongpf
修改说明:	增加接口createEx，用以创建超过unsigned长度的共享内存
*******************************************************************/


#ifndef _MEMSHARED_INC_
#define _MEMSHARED_INC_

#if defined(_MSC_VER)
#pragma warning(disable:4284)
#endif

#include <os/config_env.h>

//#define TRADER_API_EXPORT

BEGIN_FBASE2_NAMESPACE

/// 共享内存定义, 该类旨在进程间共享内存，所以使用具名的内存映射机制
/**
 *可用于WIN32和支持posix标准的unix平台或sys v
 */
class API_EXPORT CSharedMem
{
public:
    ///构造函数，初始化资源
    CSharedMem();
    
    ///构造函数，建立或者打开一个共享内存，如果已经被创建，则打开
    /**
     * @param filename 文件名
     * @param size 共享内存的大小
     */
    CSharedMem(const char* filename, unsigned size = 1024 * 1024);
    
    ///析构函数，释放占用的资源
    ~CSharedMem();

    ///初始化一段共享内存
    /**
     * @param filename 文件名
     * @param size 共享内存的大小
     * @return 如果成功则返回共享内存首地址，失败返回NULL
     */
    void *Create(const char* filename, unsigned size = 1024 * 1024);

    ///打开一段共享内存
    /**
     * @param filename 文件名
     * @return 如果成功则返回共享内存首地址，失败返回NULL
     */
    void *Open(const char* filename);

    ///获取共享内存的大小
    const size_t Size() const;

    ///关闭共享内存区
    /**
     * @return 返回0表示失败，非零表示成功
     */
    int Close();


    int UnLink();


    ///获取共享内存的首地址
    const void * operator*() const; 

    ///判断是否有效
    bool operator!() const;

    ///扩展共享内存的大小，并搬移已有数据, 扩展共享内存大小必须在独占模式下才能成功
    /**
     * @param appendsize 扩展的长度
     * @return 返回新的首地址句柄，失败返回NULL
     */
    const void* Resize(size_t appendsize);
	
	///初始化一段共享内存
	/**
	 * @param filename 文件名
	 * @param size 共享内存的大小
	 * @return 如果成功则返回共享内存首地址，失败返回NULL
	 */
	void * CreateEx(const char* filename, size_t size = 1024 * 1024);
	
private:
    
    ///映射的文件名
    char  m_szName[255];
    
    ///内存区首址
    void *  m_pStart;
    
    ///长度
    size_t  m_iSize;
    
#if !defined(_WIN32)
    ///内核对象的句柄
    int m_lFd;
    
    // liangyong 删除 20080221
//    ///是否为创建者（posix下创建者负责删除文件shm_unlink() ）
//    bool m_bCreater;
#else
    ///映射文件句柄
    HANDLE  m_hFile;
    ///映射句柄
    HANDLE m_hMapHandle;
#endif
};

END_FBASE2_NAMESPACE

#endif
