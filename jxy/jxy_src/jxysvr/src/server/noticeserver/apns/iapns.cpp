#include "iapns.h"
#include <framework/nsapplication.h>


extern CSDApplication* SDGetApp();

IAPNS::IAPNS()
{
	m_bShutDown = FALSE;
}

IAPNS::~IAPNS()
{

}

BOOL IAPNS::OnStart()
{
    return m_oThreadRead.Start(SDThreadFuncRead, this) && m_oThreadWrite.Start(SDThreadFuncWrite, this);
}

BOOL IAPNS::OnStop()
{
	m_bShutDown = TRUE;
	return TRUE;
}

void IAPNS::SSL_Set(const char *host, const int port, const char *certfile, const char *keyfile, const char* capath)
{
	m_oSSLComm.ssl_set(host, port, certfile, keyfile, capath);
}

BOOL IAPNS::SSL_Connect()
{
	return m_oSSLComm.ssl_connect();
}

SDTHREAD_IMP IAPNS::SDThreadFuncRead (VOID * arg)
{
    IAPNS *pIAPNS = (IAPNS*)arg;
    if (NULL != pIAPNS)
    {
        pIAPNS->Read();
    }
    SDTHREAD_RETURN(NULL);
}

SDTHREAD_IMP IAPNS::SDThreadFuncWrite (VOID * arg)
{
    IAPNS *pIAPNS = (IAPNS*)arg;
    if (NULL != pIAPNS)
    {
        pIAPNS->Write();
    }
    SDTHREAD_RETURN(NULL);
}
