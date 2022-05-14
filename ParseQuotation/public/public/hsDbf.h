/************************************************
DBF文件读写类

DBF文件的总体格式
	a.数据记录部分,紧随字段结构描述部分,以ASCII码方式顺序存放,结束标志为1AH(即文件结束标志)
		每条记录的首字节为删除标志,20H表示有效,2AH表示已被删除.
		字段值间无分隔符,记录亦无结束标记.
	b.字段结构描述部分,位于文件首部,以二进制方式存放,结束标志为0DH.
		长度为32*(数据库的字段个数+1(即库整体描述))+1(0DH结束标志)

文件头长度=文件描述头长度+字段描述长度（32）*字段个数


调用实例:
  ThsDBF *dbf;       //声明
  dbf = new ThsDBF;  //创建对象
  if (!dbf->Open("d:\\name.dbf", fmRead|fmwrite|fmShare))   //共享读写的方式打开文件
    ShowMessage(dbf->ErrMsg);  //如果打开失败,则弹出信息
  dbf->First();      //定位第一条记录
  dbf->Edit();       //修改记录
  if(!dbf->Post())
    ShowMessage(dbf->ErrMsg);  //如果提交失败,则弹出信息
  dbf->Close();      //关闭文件
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

//记录被锁，重试次数
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

#define ERR_OPENFILE_MSG "打开文件失败，请检查文件是否存在或被其他用户锁住"
#define ERR_READHEADER_MSG "读DBF文件头信息失败，请检查文件是否被其他用户锁住"
#define ERR_BADDBF_MSG "DBF文件格式不正确，请检查文件是否为Foxpro2.5格式的DBF"
#define ERR_READFIELD_MSG "读取文件失败, 请检查文件是否被其他文件锁住"
#define ERR_NOMEMORY_MSG "分配内存失败, 请检查内存使用情况"
#define ERR_LOCKFAILED_EDIT_MSG "修改记录时,记录加锁失败"
#define ERR_LOCKFAILED_APPEND_MSG "新增记录时,记录加锁失败"
#define ERR_SETFILEPOINTER_MSG "移动文件指针失败"
#define ERR_WRITEFILE_MSG "写入DBF文件失败, 检查文件是否只读或以只读方式打开"
#define ERR_UNLOCKREC_MSG "DBF文件修改记录解锁失败"
#define ERR_FLUSHFILEBUFFERS_MSG "清除文件缓存失败"
#define ERR_UNLOCKAPPEND_MSG "DBF文件新增记录解锁失败"
#define ERR_NOT_OPEN_MSG "DBF文件未打开"
#define ERR_READREC_OUTTIME_MSG "读取记录失败，重试超时!"
#define ERR_DEFAULT_MSG "读写DBF文件发生未知错误"

//DBF文件状态 读取，修改，追加
enum ThsDBFState {tsBrowse, tsEdit, tsAppend};


//DBF 文件头结构,即库整体描述表,共32个字节
typedef struct 
{
  char  cMono;						//0x03h 或 0x80h(开头字节，有MEMO字段)		0
  char  sLastUpdate[3];		//依次为年月日,二进制											1-3
  long  iRecordNum;				//总记录个数,低位字节在前									4-7
  short iHeadSize;				//文件头长度=第9字节值*256+第8字节值			8-9
  short iRecordLength;		//记录长度=第11字节值*256+第10字节值			10-11
  char  sReserved[20];		//保留（略去更细节的描述）								12-31
} TDBFHeader;


//DBF 字段结构,即字段描述表,32个字节/字段
typedef struct
{
  char  sFieldName[11];		//字段名称,ASCII码;保留字节		0-9 10
  char  cFieldType;				//字段类型,CDNL等 ASCII码			11
  int iFieldOffset;				//本字段在首记录中的位置			12-15
  unsigned char iLength;	//字段长度<=256								16
  unsigned char iDec;			//小数点的位数								17
  char sReserved[14];			//保留（略去更细节的描述）  18-31
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
		int iBufferSize; //读取的缓冲区大小，默认为1
		int iValidBufferRec; //已读取的有效的记录数大小
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
		bool ReadBuffer(int ARecOffset); //读取记录，参数为记录的偏移数量
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
		bool IsDeletedRec(void); //判断当前选中的记录是否已经删除
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
		int RecordCount(void); //获取DBF文件的记录数
		unsigned int RecordNo(void);
		int GetFieldCount(void);
		//   __property unsigned int ErrCode  = { read=iErrCode };
		//   __property char *ErrMsg = { read=GetErrMsg };
		//   __property int TestFlag = { read = FiTestFlag };
		//  unsigned int RecordNo(void);
		~ThsDBF();
		ThsDBF();
};