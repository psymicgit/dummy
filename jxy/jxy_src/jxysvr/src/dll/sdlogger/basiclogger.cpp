#include "loggerutils.h"
#include "basiclogger.h"
#include "sdstring.h"
#include "sdlogger.h"
#include "sdtime.h"
#include "sddir.h"
#include "sdfile.h"

using namespace SGDP;

#define MAX_LOGFILE_SIZE	(1024*1024*1024)

CBasicLogger::CBasicLogger()
{
	m_nLogMode	= LOG_MODE_SINGLE_FILE;;
	m_dwID		= 0;
	memset(m_szDir, 0, sizeof(m_szDir));
	memset(m_szName, 0, sizeof(m_szName));
	m_fpFile	= NULL;
	m_bDirty	= false;
	m_nSubFile	= 0;
}

CBasicLogger::~CBasicLogger()
{
	if(m_fpFile != NULL)
		fclose(m_fpFile);
}

bool CBasicLogger::Init(INT32 nMode, UINT32 dwID, const CHAR* pszDir, const CHAR* pszName)
{
	m_nLogMode	= nMode;
	m_dwID		= dwID;
	strncpy(m_szDir, pszDir,sizeof(m_szDir));
	strncpy(m_szName, pszName,sizeof(m_szName));

	CHAR* p = m_szDir;
	while(*p)
	{
#if defined(__linux__)
		if('\\' == *p) *p = '/';
#else
		if('/' == *p) *p = '\\';
#endif
		p++;
	}

	size_t nLastPos = strlen(m_szDir)-1;
	if( ('/' == m_szDir[nLastPos]) || ('\\' == m_szDir[nLastPos]) )
	{
		m_szDir[nLastPos] = '\0';
	}
	SDCreateDirectory(_SDTA2T(m_szDir));

	m_stTime = SDNow();
	CHAR szFile[MAX_PATH]={0};
	sprintf(szFile, ("%s/%s_%04d_%02d_%02d.log"), 
		m_szDir, m_szName, m_stTime.GetYear(), m_stTime.GetMonth(), m_stTime.GetDay());
	if (SDFileExists(_SDTA2T(szFile)))
	{
		m_fpFile = fopen((szFile), "ab");
	}
	else  //  如果第一次打开，在文件头写unicode文件标识 0xff ,0xfe
	{
		m_fpFile = fopen((szFile), "ab");	
#ifndef __linux__
		if ( m_fpFile )
		{
            CHAR szBOM[4] = {(CHAR)0xEF, (CHAR)0xBB, (CHAR)0xBF, 0};  
            fwrite(szBOM,1,4,m_fpFile) ;
            fflush(m_fpFile) ;
		}
#endif
	}
	
	m_nSubFile = 0;

	return true;
}

bool CBasicLogger::Log(const TCHAR* pData, INT32 nLen)
{
	_UpdateFilePointer();

	if(NULL == m_fpFile)
		return false;
	CHAR* pszData = (CHAR*)pData ;

	CHAR wpData[1024]={0} ;
	sprintf(wpData,"%04d-%02d-%02d %02d:%02d:%02d ", 
		m_stTime.GetYear(), m_stTime.GetMonth(), m_stTime.GetDay(),
		m_stTime.GetHour(), m_stTime.GetMinute(), m_stTime.GetSecond()) ;
	fprintf(m_fpFile, wpData);
	fprintf(m_fpFile, pData);
	fprintf(m_fpFile, "\n");
	m_bDirty = true;
#ifdef _LOGERFALG
	fflush(m_fpFile);
#endif
	return true;
}

void CBasicLogger::_UpdateFilePointer()
{
	char szFile[MAX_PATH];
	CSDDateTime stCurTime;
	stCurTime = SDNow();

	//
	// 如果文件超过1G，需要分割文件，否则可能会导致程序崩溃
	//
	if(m_fpFile != NULL)
	{
		if(ftell(m_fpFile) > MAX_LOGFILE_SIZE)
		{
			m_nSubFile++;

			memset(szFile, 0, sizeof(szFile));
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d_%d.log", 
				m_szDir, m_szName, stCurTime.GetYear(), stCurTime.GetMonth(), stCurTime.GetDay(), m_nSubFile);

			fclose(m_fpFile);
			if (SDFileExists(_SDTA2T(szFile)))
			{
				m_fpFile = fopen(szFile, "ab");
			}
			else  //  如果第一次打开，在文件头写unicode文件标识 0xff ,0xfe
			{
				m_fpFile = fopen(szFile, "ab");	
				if ( m_fpFile )
				{
	                CHAR szBOM[4] = {(CHAR)0xEF, (CHAR)0xBB, (CHAR)0xBF, 0};  
                    fwrite(szBOM,1,4,m_fpFile) ;
     				fflush(m_fpFile) ;
				}
			}
		}
	}

	if(LOG_MODE_DAY_DIVIDE == m_nLogMode)
	{
		if(stCurTime.GetYear() != m_stTime.GetYear()
			|| stCurTime.GetMonth() != m_stTime.GetMonth() 
			|| stCurTime.GetDay() != m_stTime.GetDay())
		{			
			memset(szFile, 0, sizeof(szFile));
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d.log", 
				m_szDir, m_szName,
				stCurTime.GetYear(), stCurTime.GetMonth(), stCurTime.GetDay());

			if(m_fpFile != NULL)fclose(m_fpFile);
			if (SDFileExists(_SDTA2T(szFile)))
			{
				m_fpFile = fopen(szFile, "ab");
			}
			else  //  如果第一次打开，在文件头写unicode文件标识 0xff ,0xfe
			{
				m_fpFile = fopen(szFile, "ab");	
#ifndef __linux__ 
				if ( m_fpFile )
				{

					CHAR szBOM[4] = {(CHAR)0xEF, (CHAR)0xBB, (CHAR)0xBF, 0};  
					fwrite(szBOM,1,4,m_fpFile) ;
					fflush(m_fpFile) ;
				}
#endif
			}
			m_nSubFile = 0;
		}
	}
	else if(LOG_MODE_MONTH_DIVIDE == m_nLogMode)
	{
		if(stCurTime.GetYear() != m_stTime.GetYear() 
			|| stCurTime.GetMonth() != m_stTime.GetMonth())
		{			
			memset(szFile, 0, sizeof(szFile));
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d.log", 
				m_szDir, m_szName,
				stCurTime.GetYear(), stCurTime.GetMonth() , stCurTime.GetDay());

			if(m_fpFile != NULL)fclose(m_fpFile);
			if (SDFileExists(_SDTA2T(szFile)))
			{
				m_fpFile = fopen(szFile, "ab");
			}
			else  //  如果第一次打开，在文件头写unicode文件标识 0xff ,0xfe
			{
				m_fpFile = fopen(szFile, "ab");	
#ifndef __linux__ 
				if ( m_fpFile )
				{
                    CHAR szBOM[4] = {(CHAR)0xEF, (CHAR)0xBB, (CHAR)0xBF, 0};  
                    fwrite(szBOM,1,4,m_fpFile) ;
                    fflush(m_fpFile) ;
				}
#endif
			}
			m_nSubFile = 0;
		}
	}

	m_stTime = stCurTime;
}

void CBasicLogger::Flush()
{
	if(m_bDirty)
	{
		fflush(m_fpFile);
		m_bDirty = false;
	}
}

bool CBasicLogger::LogEx( const TCHAR* pData, INT32 nLen )
{
    _UpdateFilePointer();

    if(NULL == m_fpFile)
        return false;

    fwprintf(m_fpFile, L"%04d-%02d-%02d %02d:%02d:%02d ", 
        m_stTime.GetYear() , m_stTime.GetMonth() , m_stTime.GetDay(),
        m_stTime.GetHour(), m_stTime.GetMinute(), m_stTime.GetSecond());//, pData);
    for (int i = 0; i < nLen; i++)
    {
		fwprintf(m_fpFile, L"%x ", pData[i]);
   
    }
    //fwrite(pData, sizeof(char), nLen, m_fpFile);
    fwprintf(m_fpFile, L"\n");
    m_bDirty = true;

#ifdef _LOGERFALG
	fflush(m_fpFile);
#endif

    return true;
}


