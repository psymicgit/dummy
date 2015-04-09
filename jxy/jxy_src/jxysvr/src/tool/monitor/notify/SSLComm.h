
//SSLCOMM.H


#ifndef SSLCOMM_H
#define SSLCOMM_H

#ifdef linux
#include <assert.h>
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"

#include "errno.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include <arpa/inet.h>
#include <netdb.h>

#define APNS_DEV
#if defined(APNS_DEV)

#define CA_CERT_PATH    		"/pem"
#define RSA_CLIENT_CERT     	"/pem/apns.pem"
#define RSA_CLIENT_KEY      	"/pem/apns.pem"
/* Development Connection Infos */
#define APPLE_HOST          	"gateway.sandbox.push.apple.com"
#define APPLE_PORT          	2195

#define APPLE_FEEDBACK_HOST 	"feedback.sandbox.push.apple.com"
#define APPLE_FEEDBACK_PORT 	2196

#else
#define CA_CERT_PATH    		"/pem"
#define RSA_CLIENT_CERT     	"/pem/apns.pem"
#define RSA_CLIENT_KEY      	"/pem/apns.pem"
#define APPLE_HOST          	"gateway.push.apple.com"
#define APPLE_PORT          	2195
#define APPLE_FEEDBACK_HOST 	"feedback.push.apple.com"
#define APPLE_FEEDBACK_PORT 	2196
#endif


typedef unsigned	char		UINT8;
typedef unsigned  	short int 	UINT16;
typedef unsigned  	int			UINT32;

typedef struct _tagPUSHDATA
{
	UINT8	byCommend;
	UINT16	wLen;
	char	szToken[32];
	UINT16	wPayLoadLen;
	char	szPayload[256];
	_tagPUSHDATA()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagPUSHDATA));
	}
} CPUSHDATA, *pPUSHDATA;


typedef struct _tagPUSHDATAEX
{
	UINT8	byCommend;
	UINT32	dwIdentifier;
	UINT32	dwExpiry;
	UINT16	wLen;
	char	szToken[32];
	UINT16	wPayLoadLen;
	char	szPayload[256];
	_tagPUSHDATAEX()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagPUSHDATAEX));
	}
} CPUSHDATAEX, *pPUSHDATAEX;


typedef struct _tagResponce
{
	UINT8	byCommend;
	UINT8	byStatus;
	UINT32	dwIdentifier;
	_tagResponce()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagResponce));
	}
}CResponce, *pResponce;


class CSSLComm
{
public:
    CSSLComm();
    ~CSSLComm();

    bool connected();
    bool ssl_connect(const char *host, int port, const char *certfile, const char *keyfile, const char* capath);
    
	void PushNotification(const char *pToken,const char *pMsg);
    void GenPushData(const char *pToken);
    int  GenPayloadData(const char *pMsg = NULL);
	
	void PushNotificationEx(const char *pToken,const char *pMsg);
	void GenPushDataEx(const char *pToken);
    int  GenPayloadDataEx(const char *pMsg = NULL);

    private:
    void Reset();

    private:

    SSL_CTX         	*m_pctx;
    SSL             	*m_pssl;
    const SSL_METHOD    *m_pmeth;
    X509            	*m_pserver_cert;
    EVP_PKEY        	*m_pkey;

    /* Socket Communications */
    struct sockaddr_in   	m_server_addr;
    struct hostent      *	m_phost_info;

    int                  	m_sockfd;
	CPUSHDATA				m_data;
	CPUSHDATAEX				m_dataex;

};

#endif

#endif // SSLCOMM_H