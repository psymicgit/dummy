#include <time.h>
#include "sdnetutils.h"
#include "loggerutils.h"
#include "vercheck.h"
#include "tcplogger.h"
#include <sdstring.h>
using namespace SGDP ;
#define TIMESTAMP_SIZE	20
#define HEAD_SIZE (TIMESTAMP_SIZE + sizeof(UINT32))

#if (defined(WIN32) || defined(WIN64))
#define snprintf _snprintf
#else 
#include <fcntl.h>
#endif
#include "sddebug.h"
//////////////////////////////////////////////////////////////////////////////
inline void GetTimeStamp(char* timebuf, int len)
{
	time_t now_time;
	time(&now_time);
	struct tm* p_now_tm = localtime(&now_time);
	snprintf(timebuf, len, "%04d-%02d-%02d %02d:%02d:%02d", 
		p_now_tm->tm_year+1900, p_now_tm->tm_mon+1, 
		p_now_tm->tm_mday, p_now_tm->tm_hour, 
		p_now_tm->tm_min, p_now_tm->tm_sec);
}
//////////////////////////////////////////////////////////////////////////////


ISDTcpLogger* SDAPI SGDP::SDCreateTcpLogger(const SSGDPVersion *pstVersion)
{
	if (FALSE == SGDP::VerCheck( pstVersion, &SDLOGGER_VERSION))
	{       
		return NULL;
	}
	return SDNew CSDTCPLogger;
}

CSDTCPLogger::CSDTCPLogger()
:m_nBufferPos(0), m_pErrorfile(NULL), 
  m_bConnect(false)
{
#if (defined(WIN32) || defined(WIN64))
	m_hSocket = NULL;
	m_bWSA = false;
#else
	m_nFd=-1;
#endif

}

CSDTCPLogger::~CSDTCPLogger()
{
#if (defined(WIN32) || defined(WIN64))
	if (m_bWSA)
	{
		WSACleanup();
	}
#else
	if(-1!=m_nFd){
		close(m_nFd);
	}
#endif

	if (m_pErrorfile != NULL)
	{
		fclose(m_pErrorfile);
		m_pErrorfile = NULL;
	}

}

bool CSDTCPLogger::Init(const TCHAR *pszIp, const UINT16 wPort, UINT8 byServerType, const TCHAR* pszErrorfile)
{
#if (defined(WIN32) || defined(WIN64))
	WSADATA      wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}       
	m_bWSA = true;
#endif

	m_stSockAddr.sin_family = AF_INET;
	m_stSockAddr.sin_addr.s_addr = inet_addr(_SDTT2A(pszIp));
	m_stSockAddr.sin_port = htons(wPort);
	if (!ReConnect())
	{
		return false;
	}

	// open file to write error sending log
	m_pErrorfile  = fopen(_SDTT2A(pszErrorfile), "a");
	if (m_pErrorfile == NULL)
	{
		return false;
	}

	return true;
}

void CSDTCPLogger::Release()
{
	delete this;
}
#ifdef UNICODE
bool CSDTCPLogger::LogText(const TCHAR *pszLog)
{
	std::string strData = SDW2A((WCHAR*)pszLog) ;
	return LogBinary((const UINT8*)strData.c_str(), (INT32)strData.size());
}
#else
bool CSDTCPLogger::LogText(const TCHAR *pszLog)
{
	return LogBinary((const UINT8*)pszLog, (INT32)strlen(pszLog));
}
#endif




//////////////////////////////////////////////////////////////////////////

bool CSDTCPLogger::LogBinary(const UINT8 *pLog, UINT32 dwLen)
{
	if (pLog == NULL || dwLen == 0 || dwLen + HEAD_SIZE > SEND_BUFSIZE)
	{
		return false;
	}

	
	char sztime[TIMESTAMP_SIZE];
	GetTimeStamp(sztime, TIMESTAMP_SIZE);

	m_logMutex.Lock();

	// send log when buf is not enought
	if (m_nBufferPos + dwLen + HEAD_SIZE > SEND_BUFSIZE)
	{
		if (!SendBuf())
		{
			// send error, set flag not connected
			m_bConnect = false;
			WriteFile(m_aSendBuffer, m_nBufferPos);
		}
		m_nBufferPos = 0;
	}

	m_nBufferPos = sizeof(UINT32);
	memcpy(m_aSendBuffer + m_nBufferPos, sztime, TIMESTAMP_SIZE);
	m_nBufferPos += TIMESTAMP_SIZE;

	UINT32 buflen = SEND_BUFSIZE - HEAD_SIZE;
	buflen = dwLen < buflen ? dwLen : buflen;
	memcpy(m_aSendBuffer + 20 + sizeof(UINT32), pLog, buflen);
	m_nBufferPos += buflen;
	*((UINT32*)m_aSendBuffer) = m_nBufferPos;

	m_logMutex.Unlock();

	return true;
}

bool CSDTCPLogger::SendBuf()
{
	// 如果连接断开了，并且重联失败，不尝试发送
	if (m_bConnect == false && !ReConnect())
	{
		return false;
	}

	UINT32 totalsend = 0;
	int len = 0;

	while(totalsend < m_nBufferPos)
	{
#if (defined(WIN32) || defined(WIN64))
		len = send(m_hSocket, m_aSendBuffer + totalsend, m_nBufferPos - totalsend, 0);
		if (len == SOCKET_ERROR)
#else
		len = send(m_nFd, m_aSendBuffer + totalsend, m_nBufferPos - totalsend, 0);
		if (len < 0)
#endif
		{
			return false;
		}
		totalsend += len;
	}

	return true;
}

bool CSDTCPLogger::ReConnect()
{
#if (defined(WIN32) || defined(WIN64))
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{		
		return false;
	}   

	//nonblock mode
	u_long on = 1;
	ioctlsocket(m_hSocket, FIONBIO, &on );

	//创建sockaddr_in
	memset(&m_stSockAddr, 0, sizeof(SOCKADDR_IN));
#else
	m_nFd= socket(AF_INET, SOCK_DGRAM, 0);
	if (-1==m_nFd)
	{
		return false;
	}   

	//nonblock mode
	fcntl(m_nFd, F_SETFL, O_NONBLOCK|fcntl(m_nFd, F_GETFL));

	memset(&m_stSockAddr, 0, sizeof m_stSockAddr);
#endif

#if (defined(WIN32) || defined(WIN64))
	if (connect(m_hSocket, (sockaddr*)&m_stSockAddr, sizeof(m_stSockAddr)) == SOCKET_ERROR) 
	{
		closesocket(m_hSocket);
		return false;
	}
#else
	if (connect(m_nFd, (struct sockaddr*)&m_stSockAddr, sizeof(m_stSockAddr)) < 0) 
	{
		close(m_nFd);
		return false;
	}
#endif
	
	return true;
}


