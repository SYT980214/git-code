/**跨平台工具函数定义(SleepX,目录相关操作)
源程序名称: functional.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 跨平台工具函数
	      SleepX()     休眠指定的毫秒数，可以超过1秒
	      ConvertDir() 转换目录分隔符
		  IsDir()	   判断所给的目录路径是否合法
	      CreateDir()  创建多层目录
	      OpenDir(),ReadDir(),CloseDir() 一套遍历目录，取文件信息的函数；
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
	maoyinjie 20110921增加CreateDir返回值说明
    liuzl     20140108 增加GetTimespecMonotonic和GetTimespecRealtime函数，目前thread.cpp,event.cpp在使用
*******************************************************************/

#ifndef _FUNCTIONAL_INC_
#define _FUNCTIONAL_INC_

#include <os/config_env.h>
#include <os/inttypes.h>

BEGIN_FBASE2_NAMESPACE

///休眠参数指定的毫秒数
extern "C" API_EXPORT void SleepX(unsigned long dwMilliseconds);

///将输入路径字符串的不合规的目录分隔符直接替换成相应平台的格式
/**直接在输入字符串上替换
 *@param char dir_[] 目录路径串，长度不能超过 MAX_PATH
 */
extern "C" API_EXPORT const char *ConvertDir(char dir_[]);

///判断所给的目录路径是否合法
extern "C" API_EXPORT bool IsDir(char pathname[]);


//	maoyinjie 20110921增加CreateDir返回值说明
//	返回0成功 非0失败
///创建多层目录，注:在linux/unix下目录权限为最高的0x777
/**
 *@param char dir_[] 目录路径串，长度不能超过 MAX_PATH
 */
extern "C" API_EXPORT int CreateDir(char dir_[]);  

///以下一套函数，用于遍历目录，获取目录下的文件信息
//@{
///存放文件信息的结构
typedef struct tag_FileInfo
{
	char sFileName[MAX_PATH]; //文件名，不带路径
	int  iSize;               //文件大小
	int  iTime;               //文件修改时间(DOS时间戳)
	bool bIsDir;              //文件名是否为子目录名
} API_EXPORT FILE_INFO;

///打开目录,返回目录句柄
/**
 *@param char * dir 目录路径及文件名匹配串，如 "../appcom/lib*.so"; 也可只指定目录, 如"../appcom"
 *@return 返回的目录句柄，使用完毕必须调用CloseDir()释放相关资源;
 */
extern "C" API_EXPORT void * OpenDir(char * dir);

///取目录中的下一个文件信息
/**遍历目录时循环调用本函数，直到返回false; 找到的文件信息放在lpFileInfo所指结构中。
 *@param void * handle 目录句柄,即OpenDir()返回的目录句柄
 *@param FILE_INFO lpFileInfo 存放找到的文件信息的结构（输出参数）
 *@return ture 找到一个匹配的文件，false 已没有匹配的文件
 */
extern "C" API_EXPORT bool   ReadDir(void * handle,FILE_INFO * lpFileInfo);

///释放目录句柄
extern "C" API_EXPORT void   CloseDir(void * handle);

extern "C" API_EXPORT int	GetRandNum(void);

extern "C" API_EXPORT uint32 GetTimeSinceStart();

extern "C" API_EXPORT int32 GetErrorNo(void);

// 精度 > 2 微秒
extern "C" API_EXPORT uint64 GetTime(void);

extern "C" API_EXPORT int GetTimeStr(char* time);

#ifndef _WIN32
///根据超时时间获取超时时刻，开机以来的单调递增时间，不是真实时间
/**
 * @param iTimeout 超时时间ms
 * @param ts [out] 返回超时时刻
 */
extern "C" API_EXPORT void GetTimespecMonotonic(long iTimeout, timespec & ts);

///根据超时时间获取超时时刻，真实时间
/**
 * @param iTimeout 超时时间ms
 * @param ts [out] 返回超时时刻
 */
extern "C" API_EXPORT void GetTimespecRealtime(long iTimeout, timespec & ts);
#endif

#ifdef _WIN32
extern "C" API_EXPORT char *strtok_r(char *s, const char *delim, char **save_ptr) ;
extern "C" API_EXPORT struct tm *localtime_r(const time_t * timer, struct tm * result);	//	返回的tm结构中,没有tm_yday值
#endif

//@}

END_FBASE2_NAMESPACE
#endif

