#ifndef _LOGGER_THREAD_
#define _LOGGER_THREAD_

#include "sdhashmap.h"

#include "sdloopbuffer.h"
#include "sdsingleton.h"
#include "basiclogger.h"
#include "sdthread.h"
#include <stdio.h>
#include "sdmutex.h"
#include "sddebug.h"
using namespace SGDP;

typedef HashMap<UINT32, CBasicLogger*> CBasicLoggerMap;

//////////////////////////////////////////////////////////////////////////

#define LOGID_CREATE	1
#define LOGID_DELETE	2
#define LOGID_TEXT_LOG	3
#define LOGID_BIN_LOG	4

struct SLogHead
{
	UINT16	wLen;
	UINT16	wLogMsgID;
	UINT32	dwLoggerID;
};

struct SCreateLog
{
	INT32	nLogMode;
	CHAR	szDir[1024];
	CHAR	szName[128];
};

class CLoggerThread : public CSDThread
{
	CLoggerThread();
	virtual ~CLoggerThread();
	DECLARE_SINGLETON(CLoggerThread)

public:
    virtual void ThrdProc();
    virtual void OnTerminated();

	BOOL SDAPI Init();
	void SDAPI Uninit();

    BOOL SDAPI WriteToBuf(char* pContent, INT32 nLen);

protected:
	CBasicLogger* _FindLogger(UINT32 dwID);
	bool _DeleteLogger(UINT32 dwID);
	void _ClearLogger();

protected:
    bool			m_bTerminate;
    CSDLoopBuffer	m_oLoopBuffer;
	CBasicLoggerMap	m_oLoggerMap;
	CSDMutex        m_logMutex; 
	UINT32			m_dwFlushTime;
};

#endif

