
#ifndef IAPNS_H
#define IAPNS_H

#include "sslcomm.h"

#include "sdtype.h"
#include <sdnetutils.h>
#include <sdthread.h>

class IAPNS
{
public:
	IAPNS();
	~IAPNS();

public:
	BOOL OnStart();
	BOOL OnStop();
	void SSL_Set(const char *host, const int port, const char *certfile, const char *keyfile, const char* capath);
	BOOL SSL_Connect();
	static SDTHREAD_DECLARE(SDThreadFuncRead) (VOID * arg); 
	static SDTHREAD_DECLARE(SDThreadFuncWrite) (VOID * arg); 

	virtual void Read() = 0;
	virtual void Write() = 0;
	
protected:
	BOOL			m_bShutDown;
	CSSLComm		m_oSSLComm;
	CSDThread	m_oThreadRead;
	CSDThread	m_oThreadWrite;
};


#endif
