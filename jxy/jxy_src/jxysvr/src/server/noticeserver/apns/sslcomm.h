
#ifndef SSLCOMM_H
#define SSLCOMM_H

#include "sdtype.h"
#include <sdnetutils.h>
#include <sdutil.h>
#include <sdframework.h>

#include <assert.h>
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"

#if defined(__WINDOWS__)
#undef __WINDOWS__
#endif

#if (defined(WIN32) || defined(_WIN32) || defined(_WINDOWS) )
#define __WINDOWS__
#endif


#ifdef __WINDOWS__
#define SHUTDOWN_READ_WRITE   2         //winsock & winsock2
#else //Unix
#define SHUTDOWN_READ_WRITE   SHUT_RDWR
#endif


#if (defined(__WINDOWS__) || defined(__HP_aCC))
typedef int			socketlen_t;
typedef SOCKET		socket_t; 
#else
typedef socklen_t		socketlen_t;
typedef int			socket_t;
#endif

#ifdef __WINDOWS__
#define CLOSE_SOCKET(a) closesocket(a)
#else //Unix
#define CLOSE_SOCKET(a) close(a)
#endif


#define INVALID_SOCK               -1
#define UNDEFINED_PORT             0
#define BIND_TRY_NUM               10
#define CTP_OPTIMIZE_BUF_SIZE     65536     //接收、发送缓冲区优化大小





using namespace SGDP;


class CSSLComm
{
public:
    CSSLComm();
    ~CSSLComm();
	void		ssl_set(const char *host, const int port, const char *certfile, const char *keyfile, const char* capath);
	bool		ssl_connect();

	int		ssl_write(const char * data, int nLen); 
	int		ssl_read(const char * data, int nLen); 
	int		ssl_read_block(const char * data, int nLen); 
	bool		ssl_check();
private:
    void ssl_Reset();
	bool ssl_connected();
	bool ssl_connect(const char *host, const int port, const char *certfile, const char *keyfile, const char* capath);
	

private:
    SSL_CTX         	*m_pctx;
    SSL             	*m_pssl;
    const SSL_METHOD  *m_pmeth;
    X509            	*m_pserver_cert;
    EVP_PKEY        	*m_pkey;

    /* Socket Communications */
    struct sockaddr_in   		m_server_addr;
    SDHostent			*	m_phost_info;
    socket_t                 m_sockfd;

	//
	char		m_host[255];
	int		m_port;
	char 	m_certfile[255];
	char 	m_keyfile[255];
	char	 	m_capath[255];

	CSDMutex	m_oSDMutex;
};

#endif // SSLCOMM_H