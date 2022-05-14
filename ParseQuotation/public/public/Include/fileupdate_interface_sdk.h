/** @file fileupdate_interface_sdk.h
 *  @brief 文件更新 SDK开发接口文件
 *  @author      majc
 *  @date        20140329
 *  @copyright   恒生电子股份有限公司
 *  @version     1.0.0.0
 *
 *  @par 修改记录 <日期> <版本> <修改人员> <修改内容>
 *  
 */
#ifndef _FILEUPDATE_INTERFACE_SDK_H_
#define _FILEUPDATE_INTERFACE_SDK_H_
#if defined(_WIN32)&& _MSC_VER < 1300
#pragma warning(disable: 4786)				//需加在被包含的STL头文件前
#pragma warning(disable: 4710)				//需加在被包含的STL头文件前
#endif
#include "plugin_interface.h"

#ifdef _WIN32
typedef unsigned int		uint32;
#else
#include <sys/types.h>
#if defined(__linux__)
#include <stdint.h>
#endif
typedef uint32_t			uint32;
#endif

/** 文件更新：以下宏为API库中文件更新功能与界面间交互的打包器中的字段 */
#define PACKER_INT_FILE_LIST            "file_list"     /**< 文件列表 */
#define PACKER_INT_FILE_ID              "file_id"       /**< 文件id */
#define PACKER_INT_FILE_SIZE            "file_size"     /**< 文件大小 */
#define PACKER_INT_TIME                 "time"          /**< 文件时间 */
#define PACKER_STRING_FILE_NAME         "file_name"     /**< 文件名 */
#define PACKER_STRING_FILE_PATH         "file_path"     /**< 文件路径 */
#define PACKER_STRING_LOCAL_PATH        "local_path"    /**< 本地文件路径 */
#define PACKER_STRING_MD5_CODE          "md5_code"      /**< 文件md5值 */
#define PACKER_STRING_FILE_FLAG         "file_flag"     /**< 文件标识 */
/**
 * 文件更新回调接口，上层应用通过这个接口，接收过来的消息
 */
class CFileUpdateCallbackInterface: public IKnown
{
public:

   /**
    * 收到更新文件列表的回调
    * @param lpData 返回二进制指针
    * @param nLength 二进制数据的长度，如果为负数则为错误码
	* @param lpErrorInfo 如果不为NULL则为错误信息，发生错误，应该先去看错误信息，如果没有再去拿错误码信息
    * @return 无
    */
    virtual void FUNCTION_CALL_MODE OnRecvFileList(const void *lpData, int nLength, const char * lpErrorInfo) = 0;

   /**
    * 收到通知文件更新进度
    * @param iOneFileProcessBar 当前文件进度
    * @param iTotalProcessBar 总进度
	* @param lpErrorInfo 如果不为NULL则为错误信息，发生错误，应该先去看错误信息，如果没有再去拿错误码信息
    * @return 无
    */
    virtual void FUNCTION_CALL_MODE OnShowProcessBar(int iOneFileProcessBar, int iTotalProcessBar, const char * lpErrorInfo) = 0;
	
	/**
    * 收到成功取消文件更新
    * @return 无
    */
    virtual void FUNCTION_CALL_MODE OnCancel() = 0;


};

class CFileUpdateInterface: public IKnown
{
public:

   /**
    * 根据前面回调显示的文件列表，由用户选择后下发需要更新的文件列表
    * @param lpPack 更新文件列表指针
    * @return 返回0表示成功，其他值表示失败.
    */
    virtual int FUNCTION_CALL_MODE UpdateFile(IF2Packer* lpPack) = 0;

   /**
    * 界面取消正在更新中的更新动作
    * @return 返回0表示成功，其他值表示失败
    */
    virtual int FUNCTION_CALL_MODE CancelUpdateFile() = 0;

   /**
    * 获取更新文件列表
    * @return 文件列表解包器，不允许外部操作，只能读，不能释放
	* @note 解包器里面的字段见该文件define字段,注意：在OnRecvFileList回调以后该方法有效
    */
    virtual IF2UnPacker* FUNCTION_CALL_MODE GetFileUpdateList() = 0;
};

#endif


