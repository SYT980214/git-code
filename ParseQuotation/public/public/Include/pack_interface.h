/*******************************************************
  源程序名称:pack_interface.h
  软件著作权:恒生电子股份有限公司
  系统名称:  FBASE2.0
  模块名称:  业务包打包解包
  功能说明:  业务包打包解包接口
             1、IF2PackSvr 服务组件接口
             2、IF2Packer 打包接口
			 3、IResultSet 解包接口(resultset.h定义)
             
  相关文档:  概要设计接口部分
             
  作    者:  zhangly
  开发日期:  20040223
  备    注:  平台无关，支持多种包格式
  修改人员： zhangly
  修改日期： 20051207
  修改说明： IUnpacker接口增加取记录条数方法 getRowCount();
             20130508 05909 新增结果集行记录游标接口：包括First()/Last()/Go()
			 20130809 xuxp  增加获取当前结果集名字的接口
			 20131115 xuxp  增加解包并且拷贝内存的接口
			 20140416 majc 增加根据名字获取字段类型，字段精度，字段最大长度
			 20141110 xuxp 新增加一种P类型，方便pack嵌套pack模式。
			 20150727 majc 添加int64接口
			 20181025 majc #103547 增加接口获取ReturnCode
*********************************************************/
#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include "plugin_interface.h"
#include "resultset.h"
#include "inttypes.h"
struct IF2Packer;
struct IF2UnPacker;

#define SID_F2PACKSVR				"com.hundsun.fbase.f2packsvr"
#define SID_F2PACKER				"com.hundsun.fbase.f2packer"
#define SID_F2UNPACKER				"com.hundsun.fbase.f2unpacker"

///打包解包服务接口 
struct IF2PackSvr : public IKnown
{
	///返回业务包格式版本
	/**根据业务包首字符判断业务包格式版本,业务包版本识别规则:
	 *
     *  V1版首部放的是字符串型的列数，所以第一个字节>=0x30;
     *
     *  V2版第一个字节放的是版本号，目前值为0x21，兼容性改升时该值最大升到0x2F
     *
	 *@param void * lpBuffer		业务包数据(必须是指向合法业务包的首字节)
	 *@return int 					业务包格式版本(1: V1版,0x21~0x2F V2版版本号)
	 */
    virtual int FUNCTION_CALL_MODE GetVersion(const void * lpBuffer) = 0;
	
	/** 获取打包器接口指针
	 *@param int iVersion			业务包格式版本(取值:1 字串版,其他值 0x20版)
	 *@return IPacker * 			打包器接口指针
	 */
    virtual IF2Packer * FUNCTION_CALL_MODE GetPacker(int iVersion) = 0;

	/** 解析指定的数据并返回解包器操作接口指针
	 *@param void * lpBuffer		要解析的数据缓冲指针
	 *@param unsigned int iLen		数据长度
	 *@return IUnPacker *			解包器操作接口指针, 若解析失败则返回NULL
	 */
	virtual IF2UnPacker *  FUNCTION_CALL_MODE GetUnPacker(void * lpBuffer, unsigned int iLen ) = 0;
	
	/** 获取解包器接口指针
	 *@param iVersion:				业务包格式版本(1: V1版,0x21~0x2F V2版版本号)
	 *@return IUnPacker *			解包器操作接口指针
	 */
	virtual IF2UnPacker *  FUNCTION_CALL_MODE GetEmptyUnPacker(int iVersion) = 0;
};

///多结果集打包接口(一个包可有多个异构结果集)
/**执行序列:
 *
 * 0、准备(可选, 若不进行该操作, 则由IF2Packer内部申请内存缓冲)： SetBuffer(),如果打包缓存区由调用者提供,则必须在BeginPack()之前准备;
 *
 * 1、开始:  BeginPack(),打包器复位;
 *
 * 2、第一个结果集打包：
 *
 *(a)添加字段名列表域：AddField()
 *
 *(b)按照结果集二维表顺序，逐字段，逐条记录按字段类型添加内容：AddStr() {AddInt(), AddDouble(), AddRaw()}
 *
 * 3、设置结果集的返回码(可选，若不设置, 则返回码为0) SetReturnCode()
 *
 * 4、打下一个结果集(可选) NewDataSet()，此处同时设置了该结果集的返回码；
 * 
 * 5、参考第2步实现下一个结果集打包(可选)；
 *
 * 6、结束：EndPack(),重复调用会导致加入空结果集;
 *
 * 7、取打包结果(缓存区，缓存区大小，数据长度)
 *    打包结果也可以直接解包UnPack()返回解包接口
 *
 *使用注意事项:IF2Packer所使用的内存缓存区，由调用者负责回收；
 *             结果集附带的返回码，只有在包格式版本0x21及以上时有效；
 */
struct IF2Packer : public IKnown
{
    ///打包器初始化(使用调用者的缓存区)
	/** 第一次使用打包器时，可先使用本方法设置好缓冲区(数据长度被置为iDataLen)
	 *@param  char * pBuf  缓冲区地址
 	 *@param  int iBufSize  缓冲区空间
 	 *@param  int iDataLen  已有数据长度，新增数据加在已有数据之后（只对V1.0格式的包有效） 	 
 	 */
	virtual void FUNCTION_CALL_MODE SetBuffer(void * pBuf, int iBufSize, int iDataLen = 0) = 0;

	///复位，重新开始打另一个包(字段数与记录数置为0行0例)
	/**
	 * 功能：开始打包，把包长度清零(重复使用已有的缓存区空间)
	 *@return 无
	 */
	virtual void FUNCTION_CALL_MODE BeginPack(void) = 0;

	///开始打一个结果集
	/**在打单结果集的包时，可以不调用本方法,均取默认值
	 *@param const char *szDatasetName 0x20版打包需要指明结果集名字
	 *@param int iReturnCode           0x20版打包需要为每个结果集指明返回值
	 */
	virtual int FUNCTION_CALL_MODE NewDataset(const char *szDatasetName, int iReturnCode = 0) = 0;

	//20141110 xuxp 增加新的字段类型
	/* 类型为P的数据，解包的时候可以根据GetUnpacker来获取 */
	/**
	 * 功能：向包添加字段, 二进制字段只在包格式版本0x21及以上有效；
	 *
	 *有执行次序要求:在 NewDataset()或Reset(),SetBuffer()之后,逐个字段按顺序添加;
	 *
	 *@param szFieldName：字段名
	 *@param cFieldType ：字段类型:I整数(Int32)，D浮点数，C字符，S字符串，R任意二进制数据，P表示pack数据，L表示Int64
	 *@param iFieldWidth ：字段宽度（所占最大字节数）
	 *@param iFieldScale ：字段精度,即cFieldType='D'时的小数位数(缺省为4位小数)
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddField(const char *szFieldName,char cFieldType = 'S', int iFieldWidth = 255, int iFieldScale = 4) = 0;

	/**
	 * 功能：向包添加字符串数据
     * 有执行次序要求:必须在所有字段增加完之后,逐个字段按顺序添加;
	 *@param       szValue：字符串数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddStr(const char *szValue) = 0;

	/**
     * 功能：向包添加整数数据
 	 *@param       iValue：整数数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddInt(int iValue) = 0;
	
    /**
	 * 功能：向包添加浮点数据
	 *@param       fValue：浮点数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddDouble(double fValue) = 0;

	/**
	 * 功能：向包添加一个字符
	 *@param		 cValue：字符
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddChar(char cValue) = 0;

	/**
	 * 功能：向包添加一个二进制字段, 包格式版本0x21及以上时有效；
	 *@param	void * lpBuff 数据区
	 *@param	int iLen  数据长度	 
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddRaw(void * lpBuff,int iLen) = 0;

    ///结束打包
	virtual void FUNCTION_CALL_MODE EndPack() = 0;
 
	/**
     * 功能：取打包结果指针
	 *@return 打包结果指针
     */
	virtual void * FUNCTION_CALL_MODE GetPackBuf(void) = 0;
	
	/**
     * 功能：取打包结果长度
     *@return 打包结果长度
	 */
	virtual int FUNCTION_CALL_MODE GetPackLen(void) = 0;
	
	/**
	 * 功能：取打包结果缓冲区大小
     *@return 打包结果缓冲区大小
	 */
	virtual int FUNCTION_CALL_MODE GetPackBufSize(void) = 0;
	
	/**
	 * 功能：取打包格式版本
     *@return 版本
	 */
	virtual int FUNCTION_CALL_MODE GetVersion(void) = 0;
	
	///设置结果集的返回码(0x20版以上要求)，错误结果集需要设置
	/**返回码取缺省值0，则不设置，如果设置，则必须在EndPack()之前调用
     *@return 版本
	 */
	virtual void FUNCTION_CALL_MODE SetReturnCode(unsigned long dwRetCode) = 0;

	/**直接返回当前打包结果的解包接口,必须在EndPack()之后才能调用,在打包器释放时相应的解包器实例也释放
     *@return 解包器接口，此解包接口不能调用 destroy()来释放
	 */
	virtual IF2UnPacker * FUNCTION_CALL_MODE UnPack(void) = 0;

	/**
	 *
	 */
	virtual void FUNCTION_CALL_MODE FreeMem(void* lpBuf) = 0;

	/**
	 *
	 */
	virtual void FUNCTION_CALL_MODE ClearValue() = 0;

	//20110302 xuxp 增加一个接口函数，用来传递第一个结果集的名字
	///复位，重新开始打另一个包(字段数与记录数置为0行0例)
	/**
	 * 功能：开始打包，把包长度清零(重复使用已有的缓存区空间)
	 *@return 无
	 */
	virtual void FUNCTION_CALL_MODE BeginPackEx(char* szName = NULL) = 0;

	//20110324 dongpf 增加一个接口函数，用来复位当前结果集
	///复位当前结果集(字段数与记录数置为0行0例)，不影响其他结果集
	/**
	 * 功能：复位当前结果集
	 *@return 无
	 */
	virtual void FUNCTION_CALL_MODE ClearDataSet() = 0;

	//20130920 xuxp 增加一个新的接口，用于添加double型数据，提高效率
	/**
	 * 功能：向包添加浮点数据
	 *@param       fValue：浮点数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddDoubleEx(double fValue) = 0;


	//20141110 xuxp 增加一个直接添加pack数据的接口
	/**
	 * 功能：向包添加一个打包器数据
	 *@param       lpPacker：需要打入的pack数据指针
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddPacker(IF2Packer* lpPacker) = 0;

	//20150727 majc 添加int64接口
	/**
     * 功能：向包添加64位整数数据
 	 *@param       iValue：整数数据
	 *@return 负数表示失败，否则为目前包的长度
	 */
	virtual int FUNCTION_CALL_MODE AddInt64(int64 iValue) = 0;
};

///解包器接口
struct IF2UnPacker : public IF2ResultSet
{
	/** 取打包格式版本
     *@return 业务包格式版本
	 */
	virtual int FUNCTION_CALL_MODE GetVersion(void) = 0;

	/** 解包
	 *@param void * lpBuffer			要解包的数据
	 *@param unsigned int iLen			数据长度
     *@return							0 表示成功， 其它为失败
	 */
	virtual int FUNCTION_CALL_MODE Open(void * lpBuffer,unsigned int iLen) = 0;

    /// 取结果集个数 (0x21及以上版本支持)
    virtual int FUNCTION_CALL_MODE GetDatasetCount()=0;

    /// 设置当前结果集 (0x21及以上版本支持)
    /**
	 *@param  int nIndex				结果集编号
	 *@return int						非0 表示成功，否则为失败
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDatasetByIndex(int nIndex)=0;

    /// 设置当前结果集 (0x21及以上版本支持)
    /**
	 *@param  const char *szDatasetName	结果集名称
	 *@return int						非0 表示成功，否则为失败
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDataset(const char *szDatasetName)=0;

	/** 取解包数据缓冲指针
	 *@return 数据缓冲指针
     */
	virtual void * FUNCTION_CALL_MODE GetPackBuf(void) = 0;

	/** 取解包数据长度
     *@return 解包数据长度
	 */
	virtual unsigned int FUNCTION_CALL_MODE GetPackLen(void) = 0;

	/**取解包数据记录条数,20051207以后版本支持
     *@return 记录条数
	 */
	virtual unsigned int FUNCTION_CALL_MODE GetRowCount(void) = 0;

    /* 20130508 05909 新增结果集行记录游标接口 start */
    ///结果集行记录游标接口：取结果集的首条记录
    virtual void FUNCTION_CALL_MODE First() = 0;

    ///结果集行记录游标接口：取结果集的最后一条记录
    virtual void FUNCTION_CALL_MODE Last() = 0;

    ///结果集行记录游标接口：取结果集的第n条记录，取值范围[1, GetRowCount()]
    virtual void FUNCTION_CALL_MODE Go(int nRow) = 0;
    /* 20130508 05909 新增结果集行记录游标接口 end */

	//20130809 xuxp  增加获取当前结果集名字的接口,没有名字返回""
	virtual const char* FUNCTION_CALL_MODE GetDatasetName() = 0;


	//20131115 xuxp  增加解包并且拷贝的接口
	/** 解包并且拷贝缓存到自己的内部空间
	 *@param void * lpBuffer			要解包的数据
	 *@param unsigned int iLen			数据长度
     *@return							0 表示成功， 其它为失败
	 */
	virtual int FUNCTION_CALL_MODE OpenAndCopy(void * lpBuffer,unsigned int iLen) = 0;
	//20140416 majc 增加根据名字获取字段类型，字段精度，字段最大长度
	
	//取字段数据类型
    /** @param columnName:字段名
      * @return:见上面常量定义;列名不存在 默认返回'S'
      */
    virtual char FUNCTION_CALL_MODE GetColTypeByName(const char * columnName)=0;


   ///取数字型字段小数位数
   /** @param columnName:字段名
     * @return int 返回字段数据小数位数 列名不存在 返回0 
     */
   virtual int FUNCTION_CALL_MODE GetColScaleByName(const char * columnName)=0;
  
   //取字段允许存放数据的最大宽度.
   /** @param columnName:字段名
     * @return int 返回字段宽度 列名不存在 返回1
     */
   virtual int FUNCTION_CALL_MODE GetColWidthByName(const char * columnName) = 0;




   //20141110 xuxp 增加获取UnPacker接口
   //按字段序号，取字段值(packer类型)
    /**@param column:字段序号(以0为基数)
	  *@param   lpUnpacker: [out]解包器指针，由外部传入。解包器最后由外部释放。
	  *可以传入一个NULL解包器，也可以是重复利用解包器指针。
      *@return 对应的字段名字不存在，或者对应的内容不是一个解包器，返回值为负数，成功返回0；
	  *具体使用说明如下，代码编写的例子：
		IF2UnPacker* lpUnpacker = NULL;//或者IF2UnPacker* lpUnpacker是已经存在解包器，重复使用。
		int iResult = GetUnpacker(indx,&lpUnpacker);
		if(iResult==0)//返回成功
		{
			lpUnpacker->Release();//需要释放解包器,如果外面要重复使用就不用释放。
		}
		else//失败
		{
		}
      */
    virtual int FUNCTION_CALL_MODE GetUnpackerByIndex(int column,IF2UnPacker** lpUnpacker)=0;

   //按字段名，取字段值(packer类型)
    /**@param columnName: 字段名
	  *@param   lpUnpacker: [out]解包器指针，由外部传入。解包器最后由外部释放。
	  *可以传入一个NULL解包器，也可以是重复利用解包器指针。
      *@return 对应的字段名字不存在，或者对应的内容不是一个解包器，返回值为负数，成功返回0；
	  *具体使用说明如下，代码编写的例子：
		IF2UnPacker* lpUnpacker = NULL;//或者IF2UnPacker* lpUnpacker是已经存在解包器，重复使用。
		int iResult = GetUnpacker(strName,&lpUnpacker);
		if(iResult==0)//返回成功
		{
			lpUnpacker->Release();//需要释放解包器,如果外面要重复使用就不用释放。
		}
		else//失败
		{
		}
      */
    virtual int FUNCTION_CALL_MODE GetUnpacker(const char * columnName,IF2UnPacker** lpUnpacker)=0;

	//20150727 majc 添加int64接口
	///按字段序号，取字段值
    /**@param columnIndex:字段序号(以0为基数)
      *@return int64型字段值,下标越界返回0
	  *如果对应的列类型是char型的，返回的是字符对应的ascII码；如果对应的列类型是string型的，就会做类似atoll的转换
      */
    virtual int64  FUNCTION_CALL_MODE GetInt64ByIndex(int columnIndex)=0;

    ///按字段名，取字段值
    /**@param columnName:字段名
      *@return int型字段值,不存在返回0
	  *如果对应的列类型是char型的，返回的是字符对应的ascII码；如果对应的列类型是string型的，就会做类似atoll的转换
      */
    virtual int64 FUNCTION_CALL_MODE GetInt64(const char * columnName)=0;

	//20161013 zhouwh 增加L类型(Int64)支持
	//取字段数据类型
    /** @param columnName:字段名
      * @return:见上面常量定义;列名不存在 默认返回'S'
      */
    virtual char FUNCTION_CALL_MODE GetColTypeByNameEx(const char * columnName)=0;

	//20161013 zhouwh 取字段数据类型,支持int64增加
    /** @param column:字段序号(以0为基数)
      * @return:见上面常量定义;下标越界 返回-1
      */
    virtual char FUNCTION_CALL_MODE GetColTypeEx(int column)=0;

	//20181025 majc #103547 增加接口获取ReturnCode
	/**返回结果集的错误码(0x20版以上要求)
	*@return 返回码默认值0，不设置返回默认值
	*/
	virtual int FUNCTION_CALL_MODE GetReturnCode() = 0;

};

//----------作为可动态加载的基础服务组件，必须定义以下引出函数---------

#ifdef __cplusplus
extern "C" { 
#endif

	///组件初始化方法
	/**
	 *@return int 版本号 yyyymmdd形式
	 */
	int  FUNCTION_CALL_MODE PackSvrInit(void *lpInst, PFOnSetTimeOut lpFunc= NULL);

	///取服务接口句柄
	/**
	 *@return void * 基础服务句柄 
	 */
	IKnown * FUNCTION_CALL_MODE GetPackSvrInstance(void);

	///取服务接口依赖服务
	/**
	 *@param iIndex 
	 *@return IKnown * 依赖服务标识 
	 */
	char * FUNCTION_CALL_MODE GetPackSvrDependService(void *);

	///返回统一的基础服务插件查询接口
	/**当基础服务组件动态加载时，通过本接口或以获取服务信息(包括xxxxInit(),getxxxxInterface()函数指针)
	 *@param int iIndex		
	 *@param void ** ppv    基础服务组件信息(输出参数)，
	 *@return 返回0表示查到所要的接口，否则返回负数
	 */
	int FUNCTION_CALL_MODE GetPackSvrInfo(int iIndex, tagBaseServiceInfo * ppv );

#ifdef __cplusplus
  }
#endif
 
#endif
