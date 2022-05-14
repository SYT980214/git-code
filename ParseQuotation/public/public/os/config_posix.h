/**Linux/Unixƽ̨��posix��׼�Ļ�������ͷ�ļ�����
Դ��������: config_posix.h
�������Ȩ: �������ӹɷ����޹�˾ ��Ȩ����
ϵͳ����: ���ü���ƽ̨
ģ������: ��ƽ̨����� 
����˵��: ���ļ���config_env.h����������Ӧֱ�Ӱ�����ͷ�ļ���
	      
����ĵ�: 
����:
���: (��ѡ��)
	
�޸ļ�¼
�޸�����:
�޸���Ա:
�޸�˵��:

2013-11-15 liuzl add �޸�64λ�±��뱨���Ҳ���stropts.h
2013-12-24 liuzl ����HAVE_PTHREAD_CONDATTR_SETCLOCK�꿪��
20140402 liuzl ȥ��HAVE_PTHREAD_CONDATTR_SETCLOCK

*******************************************************************/

#if !defined _CONFIG_POSIX_INC_ && !defined _WIN32
#define _CONFIG_POSIX_INC_

/* 20140402 liuzl add Ϊ�����������Ҳ�������
// 2013-12-24 liuzl add �򵥸���glibc�汾�ж��Ƿ�֧��pthread_condattr_setclock
#if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 4)
#include <pthread.h>
#define HAVE_PTHREAD_CONDATTR_SETCLOCK
#elif (__GLIBC__ == 2 && __GLIBC_MINOR__ == 3 && !defined(__x86_64__)) // && kernel >= 2.6
// 64λ���뻷����û��nptl�߳̿�
#include <nptl/pthread.h>   // �����ִ�г���ʱ����Ҫ�����ض����߳̿�-L /usr/lib/nptl/ -lpthread -lrt
#define HAVE_PTHREAD_CONDATTR_SETCLOCK
#else
#include <pthread.h>
#endif
*/

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <sys/socket.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>

// 2013-11-15 liuzl add �ο�ace/config-linux-common.h
#if defined _XOPEN_STREAMS && _XOPEN_STREAMS == -1
// no stropts.h
#else
#include <stropts.h>
#endif

#include <errno.h>
#include <dirent.h>
#include <fnmatch.h>		// liangyong 081022 add

#if !defined(OS_UNIX)
#define OS_UNIX
#endif
///��hp aCC������
#if defined(__HP_aCC)
///����gcc ������
#elif defined(__GNUC__)
#include <ctype.h>
#endif

///ƽ̨���
///���hpunix
#if defined(__hpux)
#include <poll.h>
#define	SEM_FAILED		((sem_t *)(-1))
///���redhat linux
#elif defined(__linux__)
//#define USE_IPX			//	LINUX��һ�㲻֧��IPXЭ��
#include <sys/poll.h>
#include <netipx/ipx.h>
#elif defined(_AIX)
#include <sys/poll.h>
///���solaris
#elif defined(sun)
#include <poll.h>
#endif

#define __stdcall

///��socket��غ궨�壬��windows����һ��
//@{
	#define closesocket				close

	#ifndef SOCKET
	#define SOCKET					int
	#endif

	#ifndef INVALID_SOCKET
	#define INVALID_SOCKET			(SOCKET)(~0)
	#endif

	#ifndef SOCKET_ERROR
	#define SOCKET_ERROR			(-1)
	#endif

	#ifndef INADDR_NONE
	#define INADDR_NONE				(0xFFFFFFFFL)
	#endif

	#ifndef INADDR_ANY
	#define INADDR_ANY				((uint32)0x00000000L)
	#endif
	
	#ifndef SOCKET_LEN_TYPE
		#if defined(__hpux)				//	HP UXIX
			#define SOCKET_LEN_TYPE		int
		#elif defined(__linux__)		//	LINUX
			#define SOCKET_LEN_TYPE		socklen_t
		#elif defined(sun)				//	SUN
			#define SOCKET_LEN_TYPE		socklen_t
		#elif defined(_AIX)
		        //20091124 dongpf aix�޸�SOCKET_LEN_TYPEΪsocklen_t
			//#define SOCKET_LEN_TYPE		unsigned long
			#define SOCKET_LEN_TYPE		socklen_t
		#endif
	#endif

	#define SD_RECEIVE      0x00				///ֹͣ����
	#define SD_SEND         0x01				///ֹͣ����
	#define SD_BOTH         0x02				///˫��ֹͣ
//@}

///����linux/unix��Ŀ¼�ָ���
#define DIR_SLASH			'/'
///����linux/unix��Ŀ¼·���ַ�������󳤶�
#define MAX_PATH			256

//typedef unsigned int  DWORD;        ///sizeof(DWORD) = 4
typedef long unsigned int DWORD;			/// 20090105 ���� �޸�

//#define OS_UNIX

static const int INFINITE = (-1);
typedef int     BOOL;


#define INVALID_HANDLE_VALUE    -1


///��̬���ļ���׺
#define LIB_POSTFIX		".so"
///��̬���ļ�ǰ׺
#define LIB_PREFIX		"lib"

///�����Ļ��ϵͳ����
#define CLEAR_CMD			"clear"

#define SEM_NAME_LEN			255

#ifndef MAXLONG
#define MAXLONG ((1 << (sizeof(long) * 8 - 1)) - 1)			//~(1 << (sizeof(long) * 8 - 1))
#endif

#ifndef MAXULONG
#define MAXULONG ((1 << (sizeof(unsigned long) * 8)) - 1)
#endif

///��windowsͬ�壬���������Ķ����Ѿ���������
#define ERROR_ALREADY_EXISTS        183L

#if !defined( PLUGINS_CALL )
#define PLUGINS_CALL	
#endif

#endif
