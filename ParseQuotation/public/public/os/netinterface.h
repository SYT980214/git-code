// NetInterface.h: interface for the CNetInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETINTERFACE_H__222C2493_7EBB_440C_9DBA_0E77042930D7__INCLUDED_)
#define AFX_NETINTERFACE_H__222C2493_7EBB_440C_9DBA_0E77042930D7__INCLUDED_

//#define _WS2TCPIP_H_
#include <os/config_env.h>
#include <os/inttypes.h>

#ifdef WIN32
	#if _MSC_VER > 1000
	#pragma once
	#endif // _MSC_VER > 1000

	#define s6_addr
	#include <Include/IPHlpApi.h>

#else
	#include <netinet/in.h>
	#include <net/if.h>


	#if defined(__linux__)	//	LINUX
		#include <linux/sockios.h>
	#elif defined(__hpux)	//	HP
		#include <netinet/ip_var.h>
	#elif defined(sun)		//	SUN
		#include <netinet/ip_var.h>
		#include <sys/sockio.h>
	#endif
#endif

BEGIN_FBASE2_NAMESPACE

typedef struct tagIPAddress
{
	char	szIP[32];
	uint32	dwMask;
	tagIPAddress()
	{
		memset(this, 0, sizeof(tagIPAddress));
	}
}IP_ADDRESS, *LPIP_ADDRESS;

typedef struct tagMacAddress
{
	uint8	szNet[4];
	uint8	szNode[6];
	tagMacAddress()
	{
		memset(this, 0, sizeof(tagMacAddress));
	}
}MAC_ADDRESS, *LPMAC_ADDRESS;

class CNetInterface  
{
public:
	CNetInterface();
	virtual ~CNetInterface();
	//	»°IPµÿ÷∑
	bool GetIpAddress (LPIP_ADDRESS  lpAddress, int &iIPs);
	bool GetMacAddress(LPMAC_ADDRESS lpAddress, int &iMacss);
};

END_FBASE2_NAMESPACE

#endif // !defined(AFX_NETINTERFACE_H__222C2493_7EBB_440C_9DBA_0E77042930D7__INCLUDED_)
