

#include "filelogger.h"
#include "loggerutils.h"
#include "vercheck.h"
#include "sdstring.h"
#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#else
#include <dirent.h>
#endif
#include "sddebug.h"

#define MAX_BUFFER_LEN 8196

ISDFileLogger* SDAPI SGDP::SDCreateFileLogger(const SSGDPVersion *pstVersion)
{
	if (FALSE == SGDP::VerCheck( pstVersion, &SDLOGGER_VERSION))
    {       
        return NULL;
    }

	if (FALSE == CLoggerThread::CreateInstance())
		return NULL;

	if (FALSE == CLoggerThread::Instance()->Init())
		return NULL;

    return SDNew CSDFileLogger;
}

//////////////////////////////////////////////////////////////////////////
//CSDFileLogger
//////////////////////////////////////////////////////////////////////////

UINT32 CSDFileLogger::m_dwRef = 0;

CSDFileLogger::CSDFileLogger()
{
    ++m_dwRef;

	static UINT32 s_dwNextID = 0;
	m_dwID = ++s_dwNextID;
}

CSDFileLogger::~CSDFileLogger()
{
}

bool CSDFileLogger::Init(const TCHAR* pszDir, const TCHAR* pszFileName, INT32 nMode)
{
	if(NULL == pszDir || NULL == pszFileName)
		return false;

	if(nMode < 0 || nMode > LOG_MODE_MONTH_DIVIDE)
		return false;

	char Buf[MAX_BUFFER_LEN] = {0};
	SLogHead* pstHead = (SLogHead*)Buf;
	pstHead->wLen		= sizeof(SCreateLog);
	pstHead->wLogMsgID	= LOGID_CREATE;
	pstHead->dwLoggerID	= m_dwID;

	SCreateLog* pstCreate = (SCreateLog*)(Buf+sizeof(SLogHead));
	strcpy(pstCreate->szDir, _SDTT2A(pszDir));
	strcpy(pstCreate->szName, _SDTT2A(pszFileName));
	pstCreate->nLogMode = nMode;

    return CLoggerThread::Instance()->WriteToBuf(Buf, sizeof(SLogHead)+sizeof(SCreateLog)) == TRUE ? true : false;
}


#ifdef UNICODE
bool CSDFileLogger::LogText(const WCHAR *pszLog)
{
	if(NULL == pszLog)
		return false;

	char Buf[MAX_BUFFER_LEN]={0};
	SLogHead* pstHead = (SLogHead*)Buf;
	pstHead->wLogMsgID	= LOGID_TEXT_LOG;
	pstHead->wLen		= (UINT16)SDWcslen(pszLog)*sizeof(WCHAR);
	if(pstHead->wLen > sizeof(Buf) - sizeof(SLogHead))
		pstHead->wLen = sizeof(Buf) - sizeof(SLogHead);

	pstHead->dwLoggerID	= m_dwID;
	memcpy(Buf+sizeof(SLogHead), pszLog, pstHead->wLen);
	return CLoggerThread::Instance()->WriteToBuf(Buf, sizeof(SLogHead)+pstHead->wLen);
}

#else
bool CSDFileLogger::LogText(const char *pszLog)
{
	if(NULL == pszLog)
		return false;

	std::string strData = SDLocal2UTF8(pszLog) ;
	char Buf[MAX_BUFFER_LEN] = {0};
	SLogHead* pstHead = (SLogHead*)Buf;
	pstHead->wLogMsgID	= LOGID_TEXT_LOG;
	pstHead->wLen		= ((UINT16)SDStrnlen(strData.c_str(), sizeof(Buf)-sizeof(SLogHead)-1) + 1);
	
	if(pstHead->wLen > sizeof(Buf) - sizeof(SLogHead))
		pstHead->wLen = sizeof(Buf) - sizeof(SLogHead);

	pstHead->dwLoggerID	= m_dwID;
	memcpy(Buf+sizeof(SLogHead), strData.c_str(), pstHead->wLen-1);
	Buf[sizeof(SLogHead)+pstHead->wLen-1] = '\0';

    return CLoggerThread::Instance()->WriteToBuf(Buf, sizeof(SLogHead)+pstHead->wLen) == TRUE ?true:false;
}
#endif

bool CSDFileLogger::LogBinary(const UINT8* pLog, UINT32 dwLen)
{
	if (NULL == pLog || 0 == dwLen)
	{
		return false;
	}
    
	char Buf[MAX_BUFFER_LEN] = {0};
	SLogHead* pstHead = (SLogHead*)Buf;
	pstHead->wLogMsgID	= LOGID_BIN_LOG;
	pstHead->wLen		= (UINT16)dwLen;	//cwy modified.
	if(pstHead->wLen	 > sizeof(Buf)-sizeof(SLogHead))
		pstHead->wLen	 = sizeof(Buf)-sizeof(SLogHead);

	pstHead->dwLoggerID	= m_dwID;
	memcpy(Buf+sizeof(SLogHead), pLog, dwLen);

    return CLoggerThread::Instance()->WriteToBuf(Buf, sizeof(SLogHead)+pstHead->wLen)==TRUE?true:false;
}

void CSDFileLogger::Release()
{
	char Buf[MAX_BUFFER_LEN] = {0};
	SLogHead* pstHead = (SLogHead*)Buf;
	pstHead->wLogMsgID	= LOGID_DELETE;
	pstHead->wLen		= 0;
	pstHead->dwLoggerID	= m_dwID;

	CLoggerThread::Instance()->WriteToBuf(Buf, sizeof(SLogHead));

	--m_dwRef;
	if (m_dwRef == 0)
	{
		CLoggerThread::Instance()->Uninit();
		CLoggerThread::DestroyInstance();
	}

    delete this;
}

