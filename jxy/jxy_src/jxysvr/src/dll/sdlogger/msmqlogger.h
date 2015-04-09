#ifndef MSMQLOGGER_H
#define MSMQLOGGER_H


#include "sdlogger.h"
#ifdef SGDP_HAS_MSMQ_LOGGER 

#include "logproto.h"
using namespace SGDP;
class CMsmqWarpper;

class CSDMsmqLogger : public ISDMsmqLogger
{
public:
	CSDMsmqLogger();
	virtual ~CSDMsmqLogger();

	virtual bool SDAPI LogText(const char *pszLog);
	virtual bool SDAPI LogTextW(const WCHAR* pszLog) ;
	virtual bool SDAPI LogBinary(const UINT8 *pLog, UINT32 dwLen);
	virtual bool SDAPI Init(const char* szQueuePath, int sendtype);
	virtual void SDAPI Release();

private:
	CMsmqWarpper* m_pMsmqWarpper;
};
#endif // SGDP_HAS_MSMQ_LOGGER

#endif

