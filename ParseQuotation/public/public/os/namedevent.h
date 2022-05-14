#ifndef NAMEDEVENT_H_
#define NAMEDEVENT_H_

#include "os/config_env.h"

BEGIN_FBASE2_NAMESPACE

class API_EXPORT CNamedEvent
{
public:
    CNamedEvent();

    ~CNamedEvent();
    
    ///
    /**
     *
     *@raturn: 0 创建成功, 1 打开成功(已被创建), 其它初始化失败
     */
	int Init(const char* szName, bool bInit = false);

	int Wait(long dwMillsSecond = INFINITE);

	int Set(void);	

	int Reset();
	
	int Close();
private:	
	const char * m_szName;
	
#ifdef _WIN32
    ///事件对象句柄
    HANDLE  m_hEvent;
#else
	int		m_hSem;    
#endif
};



END_FBASE2_NAMESPACE

#endif
