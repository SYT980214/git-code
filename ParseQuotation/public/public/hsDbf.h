/************************************************
DBF�ļ���д��

DBF�ļ��������ʽ
	a.���ݼ�¼����,�����ֶνṹ��������,��ASCII�뷽ʽ˳����,������־Ϊ1AH(���ļ�������־)
		ÿ����¼�����ֽ�Ϊɾ����־,20H��ʾ��Ч,2AH��ʾ�ѱ�ɾ��.
		�ֶ�ֵ���޷ָ���,��¼���޽������.
	b.�ֶνṹ��������,λ���ļ��ײ�,�Զ����Ʒ�ʽ���,������־Ϊ0DH.
		����Ϊ32*(���ݿ���ֶθ���+1(������������))+1(0DH������־)

�ļ�ͷ����=�ļ�����ͷ����+�ֶ��������ȣ�32��*�ֶθ���


����ʵ��:
  ThsDBF *dbf;       //����
  dbf = new ThsDBF;  //��������
  if (!dbf->Open("d:\\name.dbf", fmRead|fmwrite|fmShare))   //�����д�ķ�ʽ���ļ�
    ShowMessage(dbf->ErrMsg);  //�����ʧ��,�򵯳���Ϣ
  dbf->First();      //��λ��һ����¼
  dbf->Edit();       //�޸ļ�¼
  if(!dbf->Post())
    ShowMessage(dbf->ErrMsg);  //����ύʧ��,�򵯳���Ϣ
  dbf->Close();      //�ر��ļ�
*************************************************/
#include <windows.h>

#define __FOX_COMPATIBLE

#ifdef __FOX_COMPATIBLE
  #define BASE_LOCK_ADDRESS  0x40000000
  #define FILE_LOCK_LEN  0x3FFFFFFF
#else
  #define  BASE_LOCK_ADDRESS  0x00000000
  #define  FILE_LOCK_LEN  0x00000020
#endif

#define UNLOCK_LEN  0x3FFFFFFF

#define LOCK_RETRY_TIMES  200
#define OPEN_RETRY_TIMES  100
#define RETRY_DELAY  50

//��¼���������Դ���
#define REC_LOCK_RETRY_TIMES  100
#define REC_RETRY_DELAY  5

#define ERR_OPENFILE 1
#define ERR_READHEADER 2
#define ERR_BADDBF 3
#define ERR_READFIELD 4
#define ERR_NOMEMORY 5
#define ERR_LOCKFAILED_EDIT 6
#define ERR_LOCKFAILED_APPEND 7
#define ERR_SETFILEPOINTER 8
#define ERR_WRITEFILE 9
#define ERR_UNLOCKREC 10
#define ERR_FLUSHFILEBUFFERS 11
#define ERR_UNLOCKAPPEND 12
#define ERR_READREC_OUTTIME 13
#define ERR_NOT_OPEN 14

#define ERR_OPENFILE_MSG "���ļ�ʧ�ܣ������ļ��Ƿ���ڻ������û���ס"
#define ERR_READHEADER_MSG "��DBF�ļ�ͷ��Ϣʧ�ܣ������ļ��Ƿ������û���ס"
#define ERR_BADDBF_MSG "DBF�ļ���ʽ����ȷ�������ļ��Ƿ�ΪFoxpro2.5��ʽ��DBF"
#define ERR_READFIELD_MSG "��ȡ�ļ�ʧ��, �����ļ��Ƿ������ļ���ס"
#define ERR_NOMEMORY_MSG "�����ڴ�ʧ��, �����ڴ�ʹ�����"
#define ERR_LOCKFAILED_EDIT_MSG "�޸ļ�¼ʱ,��¼����ʧ��"
#define ERR_LOCKFAILED_APPEND_MSG "������¼ʱ,��¼����ʧ��"
#define ERR_SETFILEPOINTER_MSG "�ƶ��ļ�ָ��ʧ��"
#define ERR_WRITEFILE_MSG "д��DBF�ļ�ʧ��, ����ļ��Ƿ�ֻ������ֻ����ʽ��"
#define ERR_UNLOCKREC_MSG "DBF�ļ��޸ļ�¼����ʧ��"
#define ERR_FLUSHFILEBUFFERS_MSG "����ļ�����ʧ��"
#define ERR_UNLOCKAPPEND_MSG "DBF�ļ�������¼����ʧ��"
#define ERR_NOT_OPEN_MSG "DBF�ļ�δ��"
#define ERR_READREC_OUTTIME_MSG "��ȡ��¼ʧ�ܣ����Գ�ʱ!"
#define ERR_DEFAULT_MSG "��дDBF�ļ�����δ֪����"

//DBF�ļ�״̬ ��ȡ���޸ģ�׷��
enum ThsDBFState {tsBrowse, tsEdit, tsAppend};


//DBF �ļ�ͷ�ṹ,��������������,��32���ֽ�
typedef struct 
{
  char  cMono;						//0x03h �� 0x80h(��ͷ�ֽڣ���MEMO�ֶ�)		0
  char  sLastUpdate[3];		//����Ϊ������,������											1-3
  long  iRecordNum;				//�ܼ�¼����,��λ�ֽ���ǰ									4-7
  short iHeadSize;				//�ļ�ͷ����=��9�ֽ�ֵ*256+��8�ֽ�ֵ			8-9
  short iRecordLength;		//��¼����=��11�ֽ�ֵ*256+��10�ֽ�ֵ			10-11
  char  sReserved[20];		//��������ȥ��ϸ�ڵ�������								12-31
} TDBFHeader;


//DBF �ֶνṹ,���ֶ�������,32���ֽ�/�ֶ�
typedef struct
{
  char  sFieldName[11];		//�ֶ�����,ASCII��;�����ֽ�		0-9 10
  char  cFieldType;				//�ֶ�����,CDNL�� ASCII��			11
  int iFieldOffset;				//���ֶ����׼�¼�е�λ��			12-15
  unsigned char iLength;	//�ֶγ���<=256								16
  unsigned char iDec;			//С�����λ��								17
  char sReserved[14];			//��������ȥ��ϸ�ڵ�������  18-31
}ThsFieldInfo;


const unsigned short fmRead = 1;
const unsigned short fmWrite = 2;
const unsigned short fmShare = 4;


class ThsDBF
{
	private:
		bool bReadOnly;
		bool bOpened;
		char *sFileName;
		char *sBuffer;
		char sErrMsg[10*1024];
		int FiTestFlag;		
		int iBufferOffSet;
		int iBufferInterOffSet;
		int iBufferSize; //��ȡ�Ļ�������С��Ĭ��Ϊ1
		int iValidBufferRec; //�Ѷ�ȡ����Ч�ļ�¼����С
		unsigned short iFieldCount;
		unsigned int iAppendLockStart;		
		unsigned int iRecordCount;
		unsigned int iErrCode;		
		HANDLE fh;
		ThsDBFState FState;
		TDBFHeader dbfHeader;
		ThsFieldInfo *fieldList;
		
		void ResetError();
		void DbfError(unsigned int iErrorCode, char *sErrMsgEx);
		bool ReadBuffer(int ARecOffset); //��ȡ��¼������Ϊ��¼��ƫ������
		bool LockDBF(void);
		bool UnlockDBF(void);
		bool LockRec(unsigned int ARecNo, bool ALockHeader=false);
		bool UnlockRec(unsigned int ARecNo, bool AUnlockHeader=false);
		bool OpenInternal(const char* AFileName, unsigned short AMode);
		bool CheckOpen(char *sErrCode);
		char *GetErrMsg();
		ThsFieldInfo *GetFieldInfo(char *AFieldName);
	
	public:
		bool Deleted;

		void Close(void);
		void First(void);
		void Prior(void);
		void Next(void);
		void Last(void);
		void Go(unsigned int RecNo);
		void Cancel(void);
		bool IsDeletedRec(void); //�жϵ�ǰѡ�еļ�¼�Ƿ��Ѿ�ɾ��
		bool LockAppend(void);
		bool UnlockAppend(void);
		bool Open(const char* AFileName, unsigned short AMode);
		bool GetField(char *AFieldName, char *AValue);
		bool GetField(char *AFieldName, int &AValue);
		bool GetField(char *AFieldName, __int64 &AValue);
		bool GetField(char *AFieldName, double &AValue);
		bool GetField(char *AFieldName, bool &AValue);
		bool SetField(char *AFieldName, const char *AValue);
		bool SetField(char *AFieldName, int AValue);
		bool SetField(char *AFieldName, double AValue);
		bool SetField(char *AFieldName, bool AValue);
		bool GetFieldName(int AIndex, char *AName);
		bool SetBufferSize(int ARecCount);
		bool Append(void);
		bool Edit(void);
		bool Post(void);
		bool Eof(void);
		bool Bof(void);
		bool BufferIsOk(void);
		bool Opened(void);
		bool Zap(void);
		int RecordCount(void); //��ȡDBF�ļ��ļ�¼��
		unsigned int RecordNo(void);
		int GetFieldCount(void);
		//   __property unsigned int ErrCode  = { read=iErrCode };
		//   __property char *ErrMsg = { read=GetErrMsg };
		//   __property int TestFlag = { read = FiTestFlag };
		//  unsigned int RecordNo(void);
		~ThsDBF();
		ThsDBF();
};