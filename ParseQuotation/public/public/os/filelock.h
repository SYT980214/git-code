/**��ƽ̨�ļ��� CFileLock �ඨ��
Դ��������: filelock.h
�������Ȩ: �������ӹɷ����޹�˾ ��Ȩ����
ϵͳ����: ���ü���ƽ̨
ģ������: ��ƽ̨����� 
����˵��: ��ƽ̨�ļ��� 

����ĵ�: 
����:
���: (��ѡ��)
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

/// �ļ������壬������ģ�������
class API_EXPORT CFileLock
{
public:
    ///���캯�����������ߴ��ļ�������Ѿ������������
    /**
     * @param lpFileName �ļ���
     */
    CFileLock(const char * lpFileName);

	///�����������ͷ�ռ�õ���Դ
	~CFileLock();

	///���Լ���������������ģʽ
	/**
     * @return ����ɹ���true�����򷵻�false��
    */
	bool tryLockExclusive();

	///��������������ģʽ
	/**
     * @return ����ɹ���true�����򷵻�false��
    */
	bool lockExclusive();
	
	///���Լӹ�������������ģʽ
	/**
     * @return ����ɹ���true�����򷵻�false��
    */
	bool tryLockShare();

	///�Ӷ���������ģʽ
	/**
     * @return ����ɹ���true�����򷵻�false��
    */
	bool lockShare();

	///�⹲��������������
	/**
     * @return ����ɹ���true�����򷵻�false��
    */
	bool unlock();

	///�Ƿ����
	bool isAvailable();
private:
#ifdef _WIN32
	//�ļ������
	HANDLE m_pHandle;
	OVERLAPPED m_Oapped;
#else
	///�ں˶���ľ��
	int m_iLockfd;
#endif
};

END_FBASE2_NAMESPACE

#endif
