#include "sdloggerimpl.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include "sdfile.h"
#include "sddir.h"
#include "sdutil.h"
#include "sdconfig.h"
#include "sdxmlfile.h"

#pragma warning( disable : 4267 )
#pragma warning( disable : 4996 )

using namespace std;

using namespace SGDP;

namespace SGDP
{



BOOL CSDLoggerImpl::m_bLogToFile = FALSE;
BOOL CSDLoggerImpl::m_bLogToDebugger = FALSE;
BOOL CSDLoggerImpl::m_bLogToUdp = FALSE;
BOOL CSDLoggerImpl::m_bLogToTcp = FALSE;

ISDFileLogger* CSDLoggerImpl::m_pSYSFileLogger = NULL;
ISDFileLogger* CSDLoggerImpl::m_pUsrFileLogger = NULL;
ISDFileLogger* CSDLoggerImpl::m_pDbgFileLogger = NULL;

ISDUdpLogger*  CSDLoggerImpl::m_pUdpLogger = NULL;
ISDTcpLogger*  CSDLoggerImpl::m_pTcpLogger = NULL;

CSDObjectPool<CLOGDATA, CSDMutex> CSDLoggerImpl::m_oLogDataPool;

BOOL CSDLoggerImpl::m_abLogLevel[LOGLVL_MAX] =
{
    TRUE,   //LOGLVL_SYS_CRITICAL
    TRUE,   //LOGLVL_SYS_IMPORTANT,
    FALSE,  //LOGLVL_SYS_INFO,
    TRUE,   //LOGLVL_USR_CRITICAL,
    TRUE,   //LOGLVL_USR_IMPORTANT,
    FALSE,  //LOGLVL_USR_INFO,
    FALSE,  //LOGLVL_DBG_INFO,
};

TCHAR * CSDLoggerImpl::m_aszLogPrefix[LOGLVL_MAX] =
{
    _SDT("[SYS_CRI]"),   //LOGLVL_SYS_CRITICAL
    _SDT("[SYS_IMP]"),   //LOGLVL_SYS_IMPORTANT,
    _SDT("[SYS_INF]"),  //LOGLVL_SYS_INFO,
    _SDT("[USR_CRI]"),   //LOGLVL_USR_CRITICAL,
    _SDT("[USR_IMP]"),   //LOGLVL_USR_IMPORTANT,
    _SDT("[USR_INF]"),  //LOGLVL_USR_INFO,
    _SDT("[DBG_INF]"),  //LOGLVL_DBG_INFO,
};

CHAR CSDLoggerImpl::m_szConfig[MAX_PATH] = {0};
TCHAR CSDLoggerImpl::m_szSysLogFileName[MAX_PATH] = {0};
TCHAR CSDLoggerImpl::m_szUsrLogFileName[MAX_PATH] = {0};
TCHAR CSDLoggerImpl::m_szDbgLogFileName[MAX_PATH] = {0};


static const CHAR* ELEM_CONFIG      = "config";
static const CHAR* ELEM_LOG         = "log";
static const CHAR* ELEM_LEVELS      = "Levels";
static const CHAR* ELEM_LEVEL       = "level";
static const CHAR* ATTR_ID          = "id";
static const CHAR* ATTR_LOG         = "Log";
static const CHAR* ELEM_TARGETS     = "Targets";
static const CHAR* ELEM_TARGET      = "target";
static const CHAR* VAL_SYS_CRITICAL = "SYS_CRITICAL";
static const CHAR* VAL_SYS_IMPORTANT = "SYS_IMPORTANT";
static const CHAR* VAL_SYS_INFO     = "SYS_INFO";
static const CHAR* VAL_USR_CRITICAL = "USR_CRITICAL";
static const CHAR* VAL_USR_IMPORTANT = "USR_IMPORTANT";
static const CHAR* VAL_USR_INFO     = "USR_INFO";
static const CHAR* VAL_DBG_INFO     = "DBG_INFO";
static const CHAR* VAL_CRITICAL		= "CRITICAL";
static const CHAR* VAL_WARN			= "WARN";
static const CHAR* VAL_INFO			= "INFO";
static const CHAR* VAL_DBG			= "DBG";
static const CHAR* VAL_FILE         = "FILE";
static const CHAR* VAL_DEBUGGER     = "DEBUGGER";


CSDLoggerImpl::CSDLoggerImpl()
{
}

CSDLoggerImpl::~CSDLoggerImpl()
{
}

BOOL CSDLoggerImpl::Init(const TCHAR *pszConfig)
{
    m_pSYSFileLogger = SDCreateFileLogger(&SDLOGGER_VERSION);
    if (NULL == m_pSYSFileLogger)
    {
        return FALSE;
    }

    m_pUsrFileLogger = SDCreateFileLogger(&SDLOGGER_VERSION);
    if (NULL == m_pUsrFileLogger)
    {
        return FALSE;
    }

    m_pDbgFileLogger = SDCreateFileLogger(&SDLOGGER_VERSION);
    if (NULL == m_pDbgFileLogger)
    {
        return FALSE;
    }

    TCHAR szPath[MAX_PATH];
    _SDTsnprintf(szPath, MAX_PATH, _SDT("%slog"), SDGetModulePath());

    SDCreateDirectory(szPath);
    strcpy(m_szConfig, _SDTT2A(pszConfig));
    if (FALSE == Reload())
    {
        return FALSE;
    }
    return m_pSYSFileLogger->Init(szPath, m_szSysLogFileName, LOG_MODE_DAY_DIVIDE) &&
           m_pUsrFileLogger->Init(szPath, m_szUsrLogFileName, LOG_MODE_DAY_DIVIDE) &&
           m_pDbgFileLogger->Init(szPath, m_szDbgLogFileName, LOG_MODE_DAY_DIVIDE);
}

BOOL CSDLoggerImpl::Reload()
{
    //SysImportant(_SDT("Begin Load Log Config File %s"), _SDTA2T(m_szConfig));
    CSDXMLFile  xml;
    if (FALSE == xml.Load((m_szConfig)))
    {
        return FALSE;
    }
    BOOL bLog = FALSE;
    string strLog = "";
    CSDXMLNode rootNode = xml.GetRootNode();
    _SDTStrcpy(m_szSysLogFileName, _SDTA2T(rootNode("servername").AsString().c_str()));
    _SDTStrcpy(m_szUsrLogFileName, _SDTA2T(rootNode("servername").AsString().c_str()));
    _SDTStrcpy(m_szDbgLogFileName, _SDTA2T(rootNode("servername").AsString().c_str()));

    _SDTStrcat(m_szSysLogFileName, "_sys");
    _SDTStrcat(m_szUsrLogFileName, "_usr");
    _SDTStrcat(m_szDbgLogFileName, "_dbg");

    CSDXMLNode levelNode = rootNode["log/levels/level"];
    while(levelNode != NULL)
    {
        string strId = levelNode("log").AsString();
        strLog = levelNode.AsString();
        if(strLog == "Y" || strLog == "y")
            bLog = TRUE;
        else
            bLog = FALSE;
        if (strcmp(strId.c_str(), VAL_SYS_CRITICAL) == 0 )
        {
            m_abLogLevel[LOGLVL_SYS_CRITICAL] = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_SYS_IMPORTANT) == 0)
        {
            m_abLogLevel[LOGLVL_SYS_IMPORTANT] = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_SYS_INFO) == 0)
        {
            m_abLogLevel[LOGLVL_SYS_INFO] = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_USR_CRITICAL) == 0)
        {
            m_abLogLevel[LOGLVL_USR_CRITICAL] = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_USR_IMPORTANT) == 0)
        {
            m_abLogLevel[LOGLVL_USR_IMPORTANT] = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_USR_INFO) == 0)
        {
            m_abLogLevel[LOGLVL_USR_INFO] = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_DBG_INFO) == 0)
        {
            m_abLogLevel[LOGLVL_DBG_INFO] = bLog;
        }
        levelNode = levelNode.Sibling();
    }

    CSDXMLNode targetNode = rootNode["log/targets/target"];
    while(targetNode != NULL)
    {
        string strId = targetNode("id").AsString();
        string strLog = targetNode("log").AsString();

        if(strLog == "Y" || strLog == "y")
            bLog = TRUE;
        else
            bLog = FALSE;
        if (strcmp(strId.c_str(), VAL_FILE) == 0 )
        {
            m_bLogToFile = bLog;
        }
        else if (strcmp(strId.c_str(), VAL_DEBUGGER) == 0)
        {
            m_bLogToDebugger = bLog;
        }
        targetNode = targetNode.Sibling();
    }

    //SysImportant(_SDT("Load Log Config File Success!"));
    return TRUE;
}

VOID CSDLoggerImpl::UnInit()
{
    if (NULL != m_pSYSFileLogger)
    {
        m_pSYSFileLogger->Release();
        m_pSYSFileLogger = NULL;
    }
    if (NULL != m_pUsrFileLogger)
    {
        m_pUsrFileLogger->Release();
        m_pUsrFileLogger = NULL;
    }
    if (NULL != m_pDbgFileLogger)
    {
        m_pDbgFileLogger->Release();
        m_pDbgFileLogger = NULL;
    }

    if (NULL != m_pUdpLogger)
    {
        m_pUdpLogger->Release();
        m_pUdpLogger = NULL;
    }
    if (NULL != m_pTcpLogger)
    {
        m_pTcpLogger->Release();
        m_pTcpLogger = NULL;
    }
}

BOOL CSDLoggerImpl::NeedLog(EExeLogLvl nLevel)
{
    if(nLevel < 0 || nLevel >= LOGLVL_MAX)
        return FALSE;

    return m_abLogLevel[nLevel];
}

VOID CSDLoggerImpl::_Log(EExeLogLvl nLevel, const TCHAR *format, va_list argptr)
{
    if(nLevel < 0 || nLevel >= LOGLVL_MAX)
        nLevel = LOGLVL_DBG_INFO;

	//TCHAR szMsg[1024] = {0};
	CLOGDATA * v_pLOGDATA = m_oLogDataPool.Alloc();;
	if (NULL == v_pLOGDATA)
		return;

	v_pLOGDATA->Init();

	INT32 n1 = _SDTsnprintf(v_pLOGDATA->szData, MAX_LOG_DATA_LEN, _SDT("%s "), m_aszLogPrefix[nLevel]);
	INT32 n2 = _SDTvsnprintf(v_pLOGDATA->szData + n1, MAX_LOG_DATA_LEN - n1, format, argptr);
	INT32 n3 = n1 + n2;
	if (n3 >= MAX_LOG_DATA_LEN)
		n3 = MAX_LOG_DATA_LEN - 1;

	v_pLOGDATA->szData[n3] = _SDT('\0');

    if(m_bLogToFile && m_pSYSFileLogger && m_pUsrFileLogger && m_pDbgFileLogger)
    {
        switch (nLevel)
        {
        case LOGLVL_SYS_CRITICAL:
        case LOGLVL_SYS_IMPORTANT:
        case LOGLVL_SYS_INFO:
            //if (NeedLog(LOGLVL_DBG_INFO))
            //{
            //    m_pDbgFileLogger->LogText(v_pLOGDATA->szData);
            //}
			m_pDbgFileLogger->LogText(v_pLOGDATA->szData);
            m_pSYSFileLogger->LogText(v_pLOGDATA->szData);
            break;
        case LOGLVL_USR_CRITICAL:
        case LOGLVL_USR_IMPORTANT:
        case LOGLVL_USR_INFO:
            //if (NeedLog(LOGLVL_DBG_INFO))
            //{
            //    m_pDbgFileLogger->LogText(v_pLOGDATA->szData);
            //}
			m_pDbgFileLogger->LogText(v_pLOGDATA->szData);
            m_pUsrFileLogger->LogText(v_pLOGDATA->szData);
            break;
        case LOGLVL_DBG_INFO:
            m_pDbgFileLogger->LogText(v_pLOGDATA->szData);
            break;
        default:
            m_pDbgFileLogger->LogText(v_pLOGDATA->szData);
            break;
        }
    }

    if (m_bLogToTcp && m_pTcpLogger)
    {
        m_pTcpLogger->LogText(v_pLOGDATA->szData);
    }
    if (m_bLogToUdp && m_pUdpLogger)
    {
        m_pUdpLogger->LogText(v_pLOGDATA->szData);
    }
	m_oLogDataPool.Free(v_pLOGDATA);
}

VOID CSDLoggerImpl::SysCritical(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_SYS_CRITICAL, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::SysImportant(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_SYS_IMPORTANT, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::SysInfo(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_SYS_INFO, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::UsrCritical(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_USR_CRITICAL, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::UsrImportant(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_USR_IMPORTANT, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::UsrInfo(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_USR_INFO, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::DbgInfo(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LOGLVL_DBG_INFO, format, args);
    va_end(args);
}

VOID CSDLoggerImpl::NoHeadLog(const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    TCHAR szMsg[1024];
    _SDTvsnprintf(szMsg, 1024, format, args);
    if(m_bLogToFile && m_pSYSFileLogger)
    {
        m_pSYSFileLogger->LogText(szMsg);
    }
    if (m_bLogToTcp && m_pTcpLogger)
    {
        m_pTcpLogger->LogText(szMsg);
    }
    if (m_bLogToUdp && m_pUdpLogger)
    {
        m_pUdpLogger->LogText(szMsg);
    }
}

VOID CSDLoggerImpl::SetLogLevel(INT32 nIndex, BOOL bVal)
{
    if(nIndex >= LOGLVL_MAX || nIndex < 0)
        return;

    m_abLogLevel[nIndex] = bVal;
}

BOOL CSDLoggerImpl::GetLevelValue(INT32 nIndex)
{
    if(nIndex >= LOGLVL_MAX || nIndex < 0)
        return FALSE;

    return m_abLogLevel[nIndex];
}

bool CSDLoggerImpl::LogText(const TCHAR* pszLog)
{
    NoHeadLog(_SDT("%s"), pszLog);
    return true;
}

bool CSDLoggerImpl::LogBinary(const UINT8* pLog, UINT32 dwLen)
{
    return true;
}

CSDComLoggerImpl::CSDComLoggerImpl()
{
    m_szPrefix[0] = _SDT('\0');
}

CSDComLoggerImpl::~CSDComLoggerImpl()
{

}

void CSDComLoggerImpl::Init(const TCHAR* pszPrefix)
{
    _SDTStrncpy(m_szPrefix, pszPrefix, 16);
}

bool CSDComLoggerImpl::LogText(const TCHAR *pszLog)
{
    CSDLoggerImpl::NoHeadLog(_SDT("%s %s"), m_szPrefix, pszLog);
    return true;
}

bool CSDComLoggerImpl::LogBinary(const UINT8 *pLog, UINT32 dwLen)
{
    return true;
}

}

