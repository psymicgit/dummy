#ifndef TCPLOGGER_H
#define TCPLOGGER_H

#include "sdlogger.h"
#include "logproto.h"
#include "sdmutex.h"
#include "sdnetutils.h"
using namespace SGDP;

class CSDTCPLogger : public ISDTcpLogger
{
public:
	CSDTCPLogger();
	virtual ~CSDTCPLogger();
	virtual bool SDAPI LogText(const TCHAR* pszLog) ;

	virtual bool SDAPI LogBinary(const UINT8 *pLog, UINT32 dwLen);
	virtual bool SDAPI Init(const TCHAR *pszIp, const UINT16 wPort, UINT8 byServerType, const TCHAR* pszErrorfile = NULL);
	virtual void SDAPI Release();

private:
	bool SendBuf();

	bool ReConnect();

	void CloseConnect()
	{
#if (defined(WIN32) || defined(WIN64))
		closesocket(m_hSocket);
#else
		close(m_nFd);
#endif		
	}

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

	const UINT16 GetPort()  { return ntohs(m_stSockAddr.sin_port);}

private:
#if (defined(WIN32) || defined(WIN64))
	SOCKADDR_IN m_stSockAddr;
	SOCKET      m_hSocket;
	bool		m_bWSA;
#else
	struct sockaddr_in m_stSockAddr;
	int m_nFd;
#endif

	char		m_aSendBuffer[SEND_BUFSIZE];
	UINT32		m_nBufferPos;
	FILE*		m_pErrorfile;
	bool		m_bConnect;
	CSDMutex	m_logMutex;
};

#endif

