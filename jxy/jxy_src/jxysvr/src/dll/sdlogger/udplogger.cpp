#include <time.h>
#include "vercheck.h"
#include "udplogger.h"
#include <sdstring.h>
#include "sddebug.h"
#if (defined(WIN32) || defined(WIN64))
#define snprintf _snprintf
#else 
#include <fcntl.h>
#endif

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
ISDUdpLogger* SDAPI SGDP::SDCreateUdpLogger(const SSGDPVersion *pstVersion)
{
	if (FALSE == SGDP::VerCheck( pstVersion, &SDLOGGER_VERSION))
	{       
		return NULL;
	}
	return SDNew CSDUdpLogger;
}

CSDUdpLogger::CSDUdpLogger()
:m_pErrorfile(NULL),  m_nBufSize(0)
{
#if (defined(WIN32) || defined(WIN64))
	m_hSocket = NULL;
	m_bWSA = false;
#else
	m_nFd=-1;
#endif
	
}

CSDUdpLogger::~CSDUdpLogger()
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

bool CSDUdpLogger::Init(const TCHAR *pszIp, const UINT16 wPort, UINT8 byServerType, const TCHAR* pszErrorfile)
{
#if (defined(WIN32) || defined(WIN64))
	WSADATA      wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}       
	m_bWSA = true;

	//创建socket
	m_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
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
	m_stSockAddr.sin_family = AF_INET;
	m_stSockAddr.sin_addr.s_addr = inet_addr(_SDTT2A(pszIp));
	m_stSockAddr.sin_port = htons(wPort);

	m_pErrorfile  = fopen(_SDTT2A(pszErrorfile), "a");
	if (m_pErrorfile == NULL)
	{
		return false;
	}

	
	return true;
}

void CSDUdpLogger::Release()
{
	delete this;
}
#ifdef UNICODE

bool CSDUdpLogger::LogText(const WCHAR *pszLog)
{
	std::string strData = SDW2A(pszLog) ;
	return LogBinary((const UINT8*)strData.c_str(), (INT32)strData.size());
}

#else
bool CSDUdpLogger::LogText(const char *pszLog)
{
	return LogBinary((const UINT8*)pszLog, (INT32)strlen(pszLog));
}
#endif



//////////////////////////////////////////////////////////////////////////

bool CSDUdpLogger::LogBinary(const UINT8 *pLog, UINT32 dwLen)
{
	if (pLog == NULL || dwLen == 0 || dwLen > SEND_BUFSIZE)
	{
		return false;
	}

	char sztime[20];
	GetTimeStamp(sztime, 20);
	m_logMutex.Lock();
	m_nBufSize = sizeof(UINT32);
	memcpy(m_aSendBuffer + m_nBufSize, sztime, 20);
	m_nBufSize += 20;

	UINT32 buflen = SEND_BUFSIZE - 20 - sizeof(UINT32);
	buflen = dwLen < buflen ? dwLen : buflen;
	memcpy(m_aSendBuffer + 20 + sizeof(UINT32), pLog, buflen);
	m_nBufSize += buflen;
	*((UINT32*)m_aSendBuffer) = m_nBufSize;

	if (!SendBuf())
	{
		WriteFile(m_aSendBuffer, m_nBufSize);
		m_logMutex.Unlock();
		return false;
	}

	m_logMutex.Unlock();
	return true;
}

bool CSDUdpLogger::SendBuf()
{
	int totalsend = 0;
	int len = 0;

	while(totalsend < m_nBufSize)
	{
#if (defined(WIN32) || defined(WIN64))
		len = sendto(m_hSocket, m_aSendBuffer + totalsend, 
			m_nBufSize - totalsend, 0, (sockaddr*)&m_stSockAddr, sizeof(sockaddr_in));
		if (len == SOCKET_ERROR)
#else
		len = sendto(m_nFd, m_aSendBuffer + totalsend, 
			m_nBufSize - totalsend, 0, (struct sockaddr*)&m_stSockAddr, sizeof(struct sockaddr_in));
		if (len < 0)
#endif
		{
			return false;
		}
		totalsend += len;
	}

	return true;
}

