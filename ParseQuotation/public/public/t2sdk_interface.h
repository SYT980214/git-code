
#ifndef _T2SDK_INTERNAL_INTERFACE_H
#define _T2SDK_INTERNAL_INTERFACE_H
//ͷ�ļ�
#include "Include/connection_interface.h"
#include "Include/esb_message_interface.h"
#include "Include/bizmsg_interface.h"
#include "Include/pack_interface.h"
#include "Include/tag_def.h"
#include "Include/fileupdate_interface_sdk.h"

//T2_SDK����������
extern "C"
{
     /**
	* ��ȡT2_SDK�İ汾��
	* @return ��ǰT2_SDK�İ汾��
	*/
int FUNCTION_CALL_MODE GetVersionInfo();

    /**
	* ��ȡһ��ʵ��CConfigInterface�ӿڵ����ָ��
	* @return ʵ��CConfigInterface�ӿڵ����ָ��
	*/
CConfigInterface* FUNCTION_CALL_MODE NewConfig();

    /**
	* ��ȡһ��ʵ��CConnectionInterface�ӿڵ����ָ��
	* @param CConfigInterface ʵ��CConfigInterface�ӿڵ����ָ��
	* @returnʵ��CConnectionInterface�ӿڵ����ָ��
	*/
CConnectionInterface* FUNCTION_CALL_MODE NewConnection(CConfigInterface *lpConfig);

    /**
	* ����һ���µ�ESBMessage����ָ��
	* @param int nCompressIDָ����ESBMessage���õ�ѹ���㷨��Ŀǰ֧��1����0��ʾ��ѹ��
	* @return �ӿڵ����ָ��
	*/
IESBMessage*  FUNCTION_CALL_MODE NewESBMessage(int nCompressID = 1);
    /**
	* ����һ���µ�ESBMessage����ָ�룬���Ұ���ز������������
	* @param int iFunID ���ܺ�
	* @param IF2Packer *lpPacker ҵ����
	* @param int iSystemNo ϵͳ��
	* @param int nCompressIDָ����ESBMessage���õ�ѹ���㷨��Ŀǰ֧��1����0��ʾ��ѹ��
	* @return �ӿڵ����ָ��
	*/
IESBMessage * FUNCTION_CALL_MODE NewESBMessageEx(int iFunID, IF2Packer *lpPacker, int iSystemNo = 0, int nCompressID = 1);
/**
* @param int iVersion ҵ�����ʽ�汾(ȡֵ:1 �ִ���,����ֵ 0x20��)
* �Ƽ�ʹ��0x20��
* @return IPacker * ������ӿ�ָ��
*/
IF2Packer * FUNCTION_CALL_MODE NewPacker(int iVersion);

/**
* @param void * lpBuffer Ҫ��������ݣ�����ARͨ�Ű�ͷ��
* @param unsigned int iLen ���ݳ���
* @return IUnPacker * �汾2����������ӿ�ָ��
*/
IF2UnPacker * FUNCTION_CALL_MODE NewUnPacker(void * lpBuffer, unsigned int iLen);

/**
* @param void * lpBuffer Ҫ��������ݣ�����ARͨ�Ű�ͷ��
* @param unsigned int iLen ���ݳ���
* @return IUnPacker * �汾1����������ӿ�ָ��
*/
IF2UnPacker * FUNCTION_CALL_MODE NewUnPackerV1(void * lpBuffer, unsigned int iLen);

/**
* @param void * lpBuffer Ҫ��������ݣ�����ARͨ�Ű�ͷ��
* @return 1��ʾ�汾1�Ľ�������ݣ�0x21~0x2F �汾2�Ľ��������
*/
int FUNCTION_CALL_MODE GetPackVersion(const void *lpBuffer);

/**
* @param char *EncodePass ������ɢ�н�����ַ��������Ȳ�����16������'\0'��
* @param const char* Password ����Ĵ�ɢ�е�����
* @param int nReserve ��������������Ĭ��ֵ
* @return char * ������ɢ�н����ַ��ͬEncodePass
*/
char * FUNCTION_CALL_MODE Encode(char *EncodePass, const char* Password, int nReserve = 0 );

/**
* @return SSLģʽ��SSL֤��ID
*/
const char* FUNCTION_CALL_MODE GetCertID();

 /**
* @return SSLģʽ��SSL֤����Ч��
*/
int FUNCTION_CALL_MODE GetCertValid();

/**
* ����һ���������ӿ�ָ��
* @return ����NULL��ʾʧ��.
*/
CFilterInterface* FUNCTION_CALL_MODE NewFilter();
    
/**
* ����һ���������Խӿ�ָ��
* @return ����NULL��ʾʧ��.
*/
CSubscribeParamInterface* FUNCTION_CALL_MODE NewSubscribeParam();

//20130625 xuxp ����ҵ����Ϣ
IBizMessage* FUNCTION_CALL_MODE NewBizMessage();

//20160317 zhouwh ����ҵ����Ϣ�����ѹ����־
IBizMessage* FUNCTION_CALL_MODE NewBizMessageEx(int nCompressID = 1); 
//����
int FUNCTION_CALL_MODE GetNetWorkSpeed(char * lpAddress, int port, IF2Packer * lpResult);   
//���������ٶ�
int FUNCTION_CALL_MODE GetMultiNetWorkSpeed(IF2UnPacker * lpRequest, IF2Packer * lpResult, unsigned int uiTimeOut);   
//��ȡ���ͺͽ������ֽ���
int FUNCTION_CALL_MODE GetRateBytes(int * iSend, int * iRecv);
//�����ֽ���
void FUNCTION_CALL_MODE ReSetRateBytes();
//Newһ���ս����
IF2UnPacker * FUNCTION_CALL_MODE NewEmptyUnPacker(int iVersion);
 
};



#endif
