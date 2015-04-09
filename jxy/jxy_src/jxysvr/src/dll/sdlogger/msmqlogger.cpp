#include "msmqlogger.h"
#include "msmqwrapper.h"
#include "vercheck.h"

#ifdef SGDP_HAS_MSMQ_LOGGER 
#include <Winsock2.h>

#pragma comment(lib,"mqrt.lib")

ISDMsmqLogger* SDAPI SGDP::SDCreateMsmqLogger(const SSGDPVersion *pstVersion)
{
	if (FALSE == SGDP::VerCheck( pstVersion, &SDMSMQLOGGER_VERSION))
	{       
		return NULL;
	}
	return SDNew CSDMsmqLogger;
}

//////////////////////////////////////////////////////////////////////////
//CSDMsmqLogger
//////////////////////////////////////////////////////////////////////////

CSDMsmqLogger::CSDMsmqLogger()
:m_pMsmqWarpper(NULL)
{

}

CSDMsmqLogger::~CSDMsmqLogger()
{
	if (m_pMsmqWarpper != NULL)
	{
		delete m_pMsmqWarpper;
		m_pMsmqWarpper = NULL;
	}
}

bool CSDMsmqLogger::Init(const char* szQueuePath, int sendtype)
{
	if (NULL == szQueuePath)
		return false;

	m_pMsmqWarpper = SDNew CMsmqWarpper;
	if (m_pMsmqWarpper == NULL)
		return false;

	if (!m_pMsmqWarpper->Init(szQueuePath, sendtype))
	{
		return false;
	}

	return true;
}

bool CSDMsmqLogger::LogText(const char *pszLog)
{
	if (m_pMsmqWarpper == NULL)
		return false;

	return m_pMsmqWarpper->SendMessage(pszLog, strlen(pszLog) + 1);
}

bool CSDMsmqLogger::LogTextW(const WCHAR *pszLog)
{
	if (m_pMsmqWarpper == NULL)
		return false;
	std::string strData = SDW2A(pszLog) ;

	return m_pMsmqWarpper->SendMessage(strData.c_str(), strData.size());
}

bool CSDMsmqLogger::LogBinary(const UINT8* pLog, UINT32 dwLen)
{
	if (m_pMsmqWarpper == NULL)
		return false;

	return m_pMsmqWarpper->SendMessage((const char*)pLog, dwLen);
}

void CSDMsmqLogger::Release()
{
	delete this;
}


#else 

void * SDAPI SGDP::SDCreateMsmqLogger(const SSGDPVersion *pstVersion)
{
	return NULL;
}
#endif

