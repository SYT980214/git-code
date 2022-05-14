/******************************************************************************
  源文件名：as_ResultSet.h
  软件版权：杭州恒生电子股份有限公司
  系统名称：金融基础件安全服务
  功能说明：解包器接口
  作    者：魏晓刚
  创建时间：20040223
  备    注：20071115 梁勇
  			将重命名重载函数，避免出现重载
  修 改 者：
  修改时间：
  修改简介：

  20141110 xuxp 新增加一种P类型，方便pack嵌套pack模式。
  20151216 huxb 新增IF2ResultSetEx，用于新增GetInt64，GetInt64ByIndex接口
  20160511 huxb 新增接口，获取字段默认值
  20160519 huxb 新增接口，获取字段定义宽度
********************************************************************************/
#ifndef ResultSetH
#define ResultSetH
#include "plugin_interface.h"

///20151216 huxb 增加头文件引用（int64定义）
#include "inttypes.h"

const char HSQL_DATATYPE_STRING                  = 'S';  //参数类型为string C标准的字符串 
const char HSQL_DATATYPE_CHAR                    = 'C';  //参数类型为char
const char HSQL_DATATYPE_INT                     = 'I';  //参数类型为int
const char HSQL_DATATYPE_DOUBLE                  = 'D';  //参数类型为DOUBLE
const char HSQL_DATATYPE_BINARY                  = 'R';  //任意二进制数据 
//20141110 xuxp 增加P类型
const char HSQL_DATATYPE_PACKER                  = 'P';  //PACKER格式的二进制数据
//20151216 huxb 增加L类型，表示int64
const char HSQL_DATATYPE_INT64                   = 'L';  //参数类型为int64

///解包器接口
struct IF2ResultSet : public IKnown
{

	///取字段数
    /**@return 返回字段数.
      */
    virtual int FUNCTION_CALL_MODE GetColCount()=0;

    ///取字段名
    /** @param column:字段序号(以0为基数)
      * @return 返回字段名  下标越界 返回NULL
      */
    virtual const char * FUNCTION_CALL_MODE GetColName(int column)=0;

    //取字段数据类型
    /** @param column:字段序号(以0为基数)
      * @return:见上面常量定义;下标越界 返回-1
      */
    virtual char FUNCTION_CALL_MODE GetColType(int column)=0;


   ///取数字型字段小数位数
   /** @param column:字段序号(以0为基数)
     * @return int 返回字段数据小数位数 下标越界 返回-1
     */
   virtual int FUNCTION_CALL_MODE GetColScale(int column)=0;
  
   //取字段允许存放数据的最大宽度.
   /** @param column:字段序号(以0为基数)
     * @return int 返回字段宽度 下标越界 返回-1
     */
   virtual int FUNCTION_CALL_MODE GetColWidth(int column) = 0;

   ///取字段名对应的字段序号
   /**@param columnName: 字段名
     *@return 返回字段序号. 不存在返回-1
     */
   virtual int  FUNCTION_CALL_MODE FindColIndex(const char * columnName)=0;

    // (2009-3-9孔祥云) 从hsdb的结果集中取时，保留前导和后导空格(按trade2.0要求)
    //
    //按字段序号(以0为基数)，取字段值(字符串)
    /**@param column:字段序号(以0为基数)
      *@return 字符串型字段值,下标越界返回NULL
      */
    virtual const char *  FUNCTION_CALL_MODE GetStrByIndex(int column)=0;

    //按字段名，取字段值(字符串)
    /**@param columnName: 字段名
      *@return 字符串型字段值,不存在返回NULL
      */
    virtual const char *  FUNCTION_CALL_MODE GetStr(const char * columnName)=0;

    //按字段序号(以0为基数)，取字段值
    /**@param column:字段序号(以0为基数)
      *@return 字符型字段值,下标越界返回'\0'.
      */
     virtual char  FUNCTION_CALL_MODE  GetCharByIndex(int column)=0;

    //按字段名，取字段值
    /**@param columnName: 字段名
     *@return 字符型字段值,不存在返回'\0'
     */
    virtual char   FUNCTION_CALL_MODE GetChar(const char * columnName)=0;

    //按字段序号，取字段值
    /**@param column:字段序号(以0为基数)
      *@return double型字段值,下标越界返回0
      */
    virtual double  FUNCTION_CALL_MODE GetDoubleByIndex( int column)=0;
    
	///按字段名，取字段值
    /**@param columnName:字段名
      *@return double型字段值,不存在返回0
      */
    virtual double  FUNCTION_CALL_MODE GetDouble(const char * columnName)=0;

    ///按字段序号，取字段值
    /**@param column:字段序号(以0为基数)
      *@return int型字段值,下标越界返回0
	  *如果对应的列类型是char型的，返回的是字符对应的ascII码；如果对应的列类型是string型的，就会做类似atoi的转换
      */
    virtual int  FUNCTION_CALL_MODE GetIntByIndex(int column)=0;

    ///按字段名，取字段值
    /**@param columnName:字段名
      *@return int型字段值,不存在返回0
	  *如果对应的列类型是char型的，返回的是字符对应的ascII码；如果对应的列类型是string型的，就会做类似atoi的转换
      */
    virtual int FUNCTION_CALL_MODE GetInt(const char * columnName)=0;

	///按字段序号获得字段值,二进制数据
	/**@param column: 字段序号(以0为基数)
	*@param   lpRawLen: [out]数据长度
	*@return : 数据首地址
	*/
	virtual void *  FUNCTION_CALL_MODE GetRawByIndex(int column,int * lpRawLen) = 0;

	///按字段名，取字段值
	/**@param columnName:字段名
	*@param   lpRawLen: [out]数据长度
	*@return : 数据首地址
	*/
	virtual void *  FUNCTION_CALL_MODE GetRaw(const char * columnName,int * lpRawLen) = 0;


	///最后一次取的字段值是否为NULL
   /**@return 0 是， 1不是
   */
    virtual int  FUNCTION_CALL_MODE WasNull()=0;

    ///取下一条记录
    virtual void  FUNCTION_CALL_MODE Next()=0;

    ///判断是否为结尾
    /**@return 1 是，0 不是;
      */
    virtual int  FUNCTION_CALL_MODE IsEOF()=0;
   
	///判断是否为空
    /**@return 1 是，0 不是;
      */
    virtual int  FUNCTION_CALL_MODE IsEmpty()=0;
    
    virtual void * FUNCTION_CALL_MODE Destroy() = 0;
};

///解包器接口（新增GetInt64，GetInt64ByIndex接口）
struct IF2ResultSetEx : public IF2ResultSet
{
   ///按字段序号，取字段值
    /**@param column:字段序号(以0为基数)
      *@return int64型字段值,下标越界返回0
	  *如果对应的列类型是char型的，返回的是字符对应的ascII码；如果对应的列类型是string型的，则转换为int64类型
      */
    virtual int64  FUNCTION_CALL_MODE GetInt64ByIndex(int column)=0;

    ///按字段名，取字段值
    /**@param columnName:字段名
      *@return int64型字段值,不存在返回0
	  *如果对应的列类型是char型的，返回的是字符对应的ascII码；如果对应的列类型是string型的，则转换为int64类型
      */
    virtual int64  FUNCTION_CALL_MODE GetInt64(const char * columnName)=0;

	//20160510 huxb 新增接口
    //按字段名，取字段默认值(字符串)
    /**@param column:字段序号(以0为基数)
	   @param lpLen:输出字段，返回默认值长度
      *@return 默认值,下标越界或者没有默认值返回NULL
      */
    virtual const char *  FUNCTION_CALL_MODE GetColDefValue(int column, int * lpLen)=0;

	//20160519 huxb 取字段宽度.
   /** @param column:字段序号(以0为基数)
     * @return int 返回字段宽度 下标越界 返回-1
     */
   virtual int FUNCTION_CALL_MODE GetColWidthEx(int column) = 0;
};

#endif
