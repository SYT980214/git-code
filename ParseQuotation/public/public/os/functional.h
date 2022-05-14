/**��ƽ̨���ߺ�������(SleepX,Ŀ¼��ز���)
Դ��������: functional.h
�������Ȩ: �������ӹɷ����޹�˾ ��Ȩ����
ϵͳ����: ���ü���ƽ̨
ģ������: ��ƽ̨����� 
����˵��: ��ƽ̨���ߺ���
	      SleepX()     ����ָ���ĺ����������Գ���1��
	      ConvertDir() ת��Ŀ¼�ָ���
		  IsDir()	   �ж�������Ŀ¼·���Ƿ�Ϸ�
	      CreateDir()  �������Ŀ¼
	      OpenDir(),ReadDir(),CloseDir() һ�ױ���Ŀ¼��ȡ�ļ���Ϣ�ĺ�����
����ĵ�: 
����:
���: (��ѡ��)
	
�޸ļ�¼
�޸�����:
�޸���Ա:
�޸�˵��:
	maoyinjie 20110921����CreateDir����ֵ˵��
    liuzl     20140108 ����GetTimespecMonotonic��GetTimespecRealtime������Ŀǰthread.cpp,event.cpp��ʹ��
*******************************************************************/

#ifndef _FUNCTIONAL_INC_
#define _FUNCTIONAL_INC_

#include <os/config_env.h>
#include <os/inttypes.h>

BEGIN_FBASE2_NAMESPACE

///���߲���ָ���ĺ�����
extern "C" API_EXPORT void SleepX(unsigned long dwMilliseconds);

///������·���ַ����Ĳ��Ϲ��Ŀ¼�ָ���ֱ���滻����Ӧƽ̨�ĸ�ʽ
/**ֱ���������ַ������滻
 *@param char dir_[] Ŀ¼·���������Ȳ��ܳ��� MAX_PATH
 */
extern "C" API_EXPORT const char *ConvertDir(char dir_[]);

///�ж�������Ŀ¼·���Ƿ�Ϸ�
extern "C" API_EXPORT bool IsDir(char pathname[]);


//	maoyinjie 20110921����CreateDir����ֵ˵��
//	����0�ɹ� ��0ʧ��
///�������Ŀ¼��ע:��linux/unix��Ŀ¼Ȩ��Ϊ��ߵ�0x777
/**
 *@param char dir_[] Ŀ¼·���������Ȳ��ܳ��� MAX_PATH
 */
extern "C" API_EXPORT int CreateDir(char dir_[]);  

///����һ�׺��������ڱ���Ŀ¼����ȡĿ¼�µ��ļ���Ϣ
//@{
///����ļ���Ϣ�Ľṹ
typedef struct tag_FileInfo
{
	char sFileName[MAX_PATH]; //�ļ���������·��
	int  iSize;               //�ļ���С
	int  iTime;               //�ļ��޸�ʱ��(DOSʱ���)
	bool bIsDir;              //�ļ����Ƿ�Ϊ��Ŀ¼��
} API_EXPORT FILE_INFO;

///��Ŀ¼,����Ŀ¼���
/**
 *@param char * dir Ŀ¼·�����ļ���ƥ�䴮���� "../appcom/lib*.so"; Ҳ��ָֻ��Ŀ¼, ��"../appcom"
 *@return ���ص�Ŀ¼�����ʹ����ϱ������CloseDir()�ͷ������Դ;
 */
extern "C" API_EXPORT void * OpenDir(char * dir);

///ȡĿ¼�е���һ���ļ���Ϣ
/**����Ŀ¼ʱѭ�����ñ�������ֱ������false; �ҵ����ļ���Ϣ����lpFileInfo��ָ�ṹ�С�
 *@param void * handle Ŀ¼���,��OpenDir()���ص�Ŀ¼���
 *@param FILE_INFO lpFileInfo ����ҵ����ļ���Ϣ�Ľṹ�����������
 *@return ture �ҵ�һ��ƥ����ļ���false ��û��ƥ����ļ�
 */
extern "C" API_EXPORT bool   ReadDir(void * handle,FILE_INFO * lpFileInfo);

///�ͷ�Ŀ¼���
extern "C" API_EXPORT void   CloseDir(void * handle);

extern "C" API_EXPORT int	GetRandNum(void);

extern "C" API_EXPORT uint32 GetTimeSinceStart();

extern "C" API_EXPORT int32 GetErrorNo(void);

// ���� > 2 ΢��
extern "C" API_EXPORT uint64 GetTime(void);

extern "C" API_EXPORT int GetTimeStr(char* time);

#ifndef _WIN32
///���ݳ�ʱʱ���ȡ��ʱʱ�̣����������ĵ�������ʱ�䣬������ʵʱ��
/**
 * @param iTimeout ��ʱʱ��ms
 * @param ts [out] ���س�ʱʱ��
 */
extern "C" API_EXPORT void GetTimespecMonotonic(long iTimeout, timespec & ts);

///���ݳ�ʱʱ���ȡ��ʱʱ�̣���ʵʱ��
/**
 * @param iTimeout ��ʱʱ��ms
 * @param ts [out] ���س�ʱʱ��
 */
extern "C" API_EXPORT void GetTimespecRealtime(long iTimeout, timespec & ts);
#endif

#ifdef _WIN32
extern "C" API_EXPORT char *strtok_r(char *s, const char *delim, char **save_ptr) ;
extern "C" API_EXPORT struct tm *localtime_r(const time_t * timer, struct tm * result);	//	���ص�tm�ṹ��,û��tm_ydayֵ
#endif

//@}

END_FBASE2_NAMESPACE
#endif

