#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "sdlogger.h"
#include "logproto.h"
#include "loggerthread.h"

using namespace SGDP;

class CSDFileLogger : public ISDFileLogger
{
public:
	CSDFileLogger();
	virtual ~CSDFileLogger();

	virtual bool SDAPI LogText(const TCHAR* pszLog) ;
	virtual bool SDAPI LogBinary(const UINT8 *pLog, UINT32 dwLen);
	virtual bool SDAPI Init(const TCHAR* pszDir, const TCHAR* pszFileName, INT32 nMode);
	virtual void SDAPI Release();

	void SetID(UINT32 dwID) { m_dwID = dwID; }
	UINT32 GetID() { return m_dwID; }

private:
	UINT32			m_dwID;
	CLoggerThread*  m_pThread;
	static UINT32   m_dwRef;
};

#endif

