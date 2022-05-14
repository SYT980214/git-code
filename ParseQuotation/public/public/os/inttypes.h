/** 各种长度整型定义
源程序名称: inttypes.h
软件著作权: 恒生电子股份有限公司 版权所有
系统名称: 公用技术平台
模块名称: 跨平台适配层 
功能说明: 各种长度整型定义(8、16、32、64位)
	
相关文档: 
作者:
审核: (可选的)
	
修改记录
修改日期:
修改人员:
修改说明:
*******************************************************************/

#ifndef _INTTYPES_H_
#define _INTTYPES_H_

#ifdef _WIN32

typedef char				int8;
typedef short				int16;
typedef int					int32;
typedef __int64				int64;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned __int64	uint64;

#else

#include <sys/types.h>

#if defined(__linux__)
#include <stdint.h>
#endif

typedef int8_t				int8;
typedef int16_t				int16;
typedef int32_t				int32;
typedef int64_t				int64;
typedef uint8_t				uint8;
typedef uint16_t			uint16;
typedef uint32_t			uint32;
typedef uint64_t			uint64;

#endif

#if defined(__ia64) || defined(__sparc) || defined(_ARCH_PPC) || defined(__PPC)

#ifdef __HP_aCC
  #pragma pack 1
#elif defined(__IBMCPP__) || defined(__IBMC__)
  #pragma options align=packed
#elif defined(_MSC_VER)
  #pragma pack(push, 1)
#else
  #pragma pack(1)
#endif

typedef struct tagInt32Struct
{
	int32 Value;
} INT32_STRUCT, *LPINT32_STRUCT;

typedef struct tagUint32Struct
{
	uint32 Value;
} UINT32_STRUCT, *LPUINT32_STRUCT;

typedef struct tagInt16Struct
{
	int16 Value;
} INT16_STRUCT, *LPINT16_STRUCT;

typedef struct tagUint16Struct
{
	uint16 Value;
} UINT16_STRUCT, *LPUINT16_STRUCT;

#ifdef __HP_aCC
  #pragma pack
#elif defined(__IBMCPP__)
  #pragma options align=reset
#elif defined(_MSC_VER)
  #pragma pack(pop)
#else
  #pragma pack()
#endif

#endif

// 对于只能访问边界对齐的整数的系统, 注：ALIGNED_BUS 宏由程序员根据目标平台自定义
#ifdef ALIGNED_BUS
#pragma message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
#pragma message("Are you sure to build for ALIGNED_BUS?")
#pragma message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

template <typename T>
T __get_int_from_ptr__(const void * ptr)
{
	if (((uintptr_t)ptr % sizeof(T)) == 0)
	{
		return (*((T *)(ptr)));
	}
	else
	{
		T val;
		memcpy(&val, ptr, sizeof(T));
		return val;
	}
};
#define GET_INT_FROM_PTR(type, ptr) __get_int_from_ptr__<type>(ptr)

template <typename T>
void __put_int_to_ptr__(void * ptr, T val)
{
	if (((uintptr_t)ptr % sizeof(T)) == 0)
	{
		(*((T *)(ptr))) = val;
	}
	else
	{
		memcpy(ptr, &val, sizeof(T));
	}
};
#define PUT_INT_TO_PTR(type, ptr, val)		__put_int_to_ptr__<type>(ptr, val)

#else

#define GET_INT_FROM_PTR(type, ptr)			(*((type *)(ptr)))
#define PUT_INT_TO_PTR(type, ptr, val)		(*((type *)(ptr))) = val

#endif

#endif // _INTTYPES_H_
