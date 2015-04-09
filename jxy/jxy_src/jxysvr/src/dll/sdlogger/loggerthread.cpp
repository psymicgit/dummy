#include "loggerutils.h"
#include "loggerthread.h"
#include "sddebug.h"
#include "sdstring.h"
using namespace SGDP ;
IMPLEMENT_SINGLETON(CLoggerThread)

CLoggerThread::CLoggerThread()
{
    m_bTerminate	= false;
	m_dwFlushTime	= 0;
	m_oLoopBuffer.Init(LOGTHRD_BUF_SIZE);
}

CLoggerThread::~CLoggerThread()
{
}

BOOL SDAPI CLoggerThread::Init()
{
	return this->Start(); 
}

void SDAPI CLoggerThread::Uninit()
{
	//this->Terminate();
	this->Wait(); 
}

void  CLoggerThread::ThrdProc()
{
	char Buf[10240];
	SLogHead stHead;

    while(true)
	{
		while(m_oLoopBuffer.PopFront((char*)&stHead, sizeof(stHead)))
		{
			SDASSERT(stHead.wLen <= sizeof(Buf)-sizeof(SLogHead));

			switch(stHead.wLogMsgID)
			{
			case LOGID_TEXT_LOG:
				memset(Buf,0,10240);
				if(m_oLoopBuffer.PopFront(Buf, stHead.wLen))
				{
					CBasicLogger* poLogger = _FindLogger(stHead.dwLoggerID);
					if(poLogger != NULL)
					{
						poLogger->Log((TCHAR*)(Buf), stHead.wLen);
					}
				}
				else
				{
					SDASSERT(false);
				}
				break;
			case LOGID_BIN_LOG:
				if(m_oLoopBuffer.PopFront(Buf, stHead.wLen))
				{
					CBasicLogger* poLogger = _FindLogger(stHead.dwLoggerID);
					if(poLogger != NULL)
					{
						poLogger->LogEx(_SDTA2T(Buf), stHead.wLen);
					}
				}
				else
				{
					SDASSERT(false);
				}
				break;
			case LOGID_CREATE:
				if(m_oLoopBuffer.PopFront(Buf, stHead.wLen))
				{
					SCreateLog* pstCreat = (SCreateLog*)Buf;
					if(_DeleteLogger(stHead.dwLoggerID))
					{
						SDASSERT(false);
					}

					CBasicLogger* poLogger = SDNew CBasicLogger;
					if(NULL == poLogger)
						continue;

					if(false == poLogger->Init(pstCreat->nLogMode, stHead.dwLoggerID, pstCreat->szDir, pstCreat->szName))
					{
						delete poLogger;
						continue;
					}

					m_oLoggerMap[stHead.dwLoggerID] = poLogger;
				}
				else
				{
					SDASSERT(false);
				}
				break;
			case LOGID_DELETE:
				{
					if(false == _DeleteLogger(stHead.dwLoggerID))
					{
						SDASSERT(false);
					}
				}
				break;
			default:
				SDASSERT(false);
				break;
			}
		}

        if(!m_bstart)
		{				
			for(CBasicLoggerMap::iterator it = m_oLoggerMap.begin();
			it != m_oLoggerMap.end(); ++it)
			{
				it->second->Flush();
			}
			break;
		}
			
		m_dwFlushTime += LOGTHRD_SLEEP_TIME;
		if(m_dwFlushTime >= 3000)
		{
			m_dwFlushTime = 0;
			for(CBasicLoggerMap::iterator it = m_oLoggerMap.begin();
				it != m_oLoggerMap.end(); ++it)
			{
				it->second->Flush();
			}
		}

		SDSleep(LOGTHRD_SLEEP_TIME);
	}

	_ClearLogger();
}

void CLoggerThread::OnTerminated()
{
    m_bTerminate = true;
}

BOOL SDAPI CLoggerThread::WriteToBuf(char* pContent, INT32 nLen)
{
	// 由于可能有多个线程在写，所以写入时要加锁
	m_logMutex.Lock();
	BOOL bRet = m_oLoopBuffer.PushBack(pContent, nLen);
	m_logMutex.Unlock();
	return bRet;
}

CBasicLogger* CLoggerThread::_FindLogger(UINT32 dwID)
{
	CBasicLoggerMap::iterator it = m_oLoggerMap.find(dwID);
	if(it == m_oLoggerMap.end())
		return NULL;

	return it->second;
}

bool CLoggerThread::_DeleteLogger(UINT32 dwID)
{
	CBasicLoggerMap::iterator it = m_oLoggerMap.find(dwID);
	if(it != m_oLoggerMap.end())
	{
		delete it->second;
		m_oLoggerMap.erase(it);
		return true;
	}

	return false;
}

void CLoggerThread::_ClearLogger()
{
	for(CBasicLoggerMap::iterator it = m_oLoggerMap.begin(); it != m_oLoggerMap.end(); it++)
	{
		delete it->second;
	}
	m_oLoggerMap.clear();
}

