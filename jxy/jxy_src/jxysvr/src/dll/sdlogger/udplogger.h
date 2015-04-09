#ifndef NETLOGGER_H
#define NETLOGGER_H

#include <stdio.h>
#include "sdmutex.h"
#include "sdnetutils.h"
#include "loggerutils.h"

#include "sdlogger.h"
using namespace SGDP;

class CSDUdpLogger : public ISDUdpLogger
{
public:
	CSDUdpLogger();
	virtual ~CSDUdpLogger();
	virtual bool SDAPI LogText(const TCHAR* pszLog) ;
	virtual bool SDAPI LogBinary(const UINT8 *pLog, UINT32 dwLen);
	virtual bool SDAPI Init(const TCHAR *pszIp, const UINT16 wPort, UINT8 byServerType, const TCHAR* pszErrorfile = NULL);
	virtual void SDAPI Release();

private:
	bool SendBuf();

	void WriteFile(const char* buf, int len)
	{
		if (m_pErrorfile == NULL)
		{
			return;
		}

		fwrite(buf, len, 1, m_pErrorfile);
	}

#if (defined(WIN32) || defined(WIN64))
	const UINT32 GetIp()    { return m_stSockAddr.sin_addr.S_un.S_addr; }
#else
	const UINT32 GetIp()    { return m_stSockAddr.sin_addr.s_addr; }
#endif

	const UINT16 GetPort()  { return SDNtohs(m_stSockAddr.sin_port);}

private:
#if (defined(WIN32) || defined(WIN64))
	SOCKADDR_IN m_stSockAddr;
	SOCKET      m_hSocket;
	bool		m_bWSA;
#else
	struct sockaddr_in m_stSockAddr;
	int m_nFd;
#endif

	char	m_aSendBuffer[SEND_BUFSIZE];
	int		m_nBufSize;
	FILE*	m_pErrorfile;
	CSDMutex m_logMutex;
};

#endif

