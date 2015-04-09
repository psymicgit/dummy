


#include "sslcomm.h"
#include "defapns.h"
#include <sdutil.h>
#include <sdframework.h>


CSSLComm::CSSLComm()
{
    //ctor
    m_sockfd = -1;
    m_pctx = NULL;
    m_pssl = NULL;
    m_pmeth = NULL;
    m_pserver_cert = NULL;
    m_pkey = NULL;

    m_port = 0;
    memset(m_host, 0x00, sizeof(m_host));
    memset(m_certfile, 0x00, sizeof(m_certfile));
    memset(m_keyfile, 0x00, sizeof(m_keyfile));
    memset(m_capath, 0x00, sizeof(m_capath));
#ifdef __WINDOWS__
#pragma comment(lib, "Ws2_32.lib")
    WSADATA	wsd;
    //启动socket库
    WSAStartup(MAKEWORD(2, 2), &wsd);
#endif

}

CSSLComm::~CSSLComm()
{
    //dtor
    ssl_Reset();
#ifdef __WINDOWS__
#pragma comment(lib, "Ws2_32.lib")
    WSACleanup();
#endif

}
void CSSLComm::ssl_Reset()
{
    if(m_pssl)
    {
        SSL_shutdown(m_pssl);
        SSL_free(m_pssl);
        m_pssl = NULL;
    }
    if(m_pctx)
    {
        SSL_CTX_free(m_pctx);
        m_pctx = NULL;
    }
    if(m_sockfd > 2)
    {
        CLOSE_SOCKET(m_sockfd);
        m_sockfd = -1;
    }
}

void CSSLComm::ssl_set(const char *host, const int port, const char *certfile, const char *keyfile, const char* capath)
{
    m_port = port;
    SDStrcpy(m_host, host);
    SDStrcpy(m_certfile, certfile);
    SDStrcpy(m_keyfile, keyfile);
    SDStrcpy(m_capath, capath);
}


bool CSSLComm::ssl_connected()
{
    if(m_sockfd < 2)
        return false;

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    fd_set fdwrite;
    fd_set fdexcept;
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcept);
    FD_SET(m_sockfd, &fdwrite);
    FD_SET(m_sockfd, &fdexcept);
    int ret = select(m_sockfd + 1, NULL, &fdwrite, &fdexcept, &timeout);
    if(ret == -1)
    {
        return false;
    }
    else if (ret == 0)
    {
        return true;
    }
    else if(ret > 0)
    {
        if(FD_ISSET(m_sockfd, &fdexcept))
        {
            return false;
        }
        else if(FD_ISSET(m_sockfd, &fdwrite))
        {
            int err = 0;
            socketlen_t len = sizeof(err);
            int result = getsockopt(m_sockfd, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
            if(result < 0 || err != 0)
                return false;
            return true;
        }
    }
    return false;
}

bool CSSLComm::ssl_check()
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    fd_set fdread;
    fd_set fdexcept;
    FD_ZERO(&fdread);
    FD_ZERO(&fdexcept);
    FD_SET(m_sockfd, &fdread);
    FD_SET(m_sockfd, &fdexcept);
    int ret = select(m_sockfd + 1, &fdread, NULL, &fdexcept, &timeout);
    if(ret == -1)
    {
        return false;
    }
    else if (ret == 0)
    {
        return false;
    }
    else if(ret > 0)
    {
        if(FD_ISSET(m_sockfd, &fdexcept))
        {
            return false;
        }
        else if(FD_ISSET(m_sockfd, &fdread))
        {
            return true;
        }
    }
    return false;
}

bool CSSLComm::ssl_connect()
{
    return ssl_connect(m_host, m_port, m_certfile, m_keyfile, m_capath);
}


bool CSSLComm::ssl_connect(const char *host, const int port, const char *certfile, const char *keyfile, const char* capath)
{
    ssl_Reset();

    int err;

    /* Load encryption & hashing algorithms for the SSL program */
    SSL_library_init();

    /* Load the error strings for SSL & CRYPTO APIs */
    SSL_load_error_strings();

    /* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
    m_pmeth = SSLv3_method();

    /* Create an SSL_CTX structure */
    m_pctx = SSL_CTX_new((SSL_METHOD*)m_pmeth);
    if(!m_pctx)
    {
        SYS_CRITICAL("Could not get SSL Context\n");
        return false;
    }

    /* Load the CA from the Path */
    if(SSL_CTX_load_verify_locations(m_pctx, NULL, capath) <= 0)
    {
        /* Handle failed load here */
        SYS_CRITICAL("Failed to set CA location...\n");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /* Load the client certificate into the SSL_CTX structure */
    if (SSL_CTX_use_certificate_file(m_pctx, certfile, SSL_FILETYPE_PEM) <= 0)
    {
        SYS_CRITICAL("Cannot use Certificate File\n");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /* Load the private-key corresponding to the client certificate */
    if (SSL_CTX_use_PrivateKey_file(m_pctx, keyfile, SSL_FILETYPE_PEM) <= 0)
    {
        SYS_CRITICAL("Cannot use Private Key\n");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /* Check if the client certificate and private-key matches */
    if (!SSL_CTX_check_private_key(m_pctx))
    {
        SYS_CRITICAL("Private key does not match the certificate public key\n");
        return false;
    }

    /* Set up a TCP socket */
    m_sockfd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_sockfd == -1)
    {
        SYS_CRITICAL("Could not get Socket:%u\n", SDGetThreadId());
        return false;
    }

    memset (&m_server_addr, '\0', sizeof(m_server_addr));
    m_server_addr.sin_family      = AF_INET;
    m_server_addr.sin_port        = htons(port);       /* Server Port number */
    m_phost_info = SDGetHostByName(host);
    if(m_phost_info)
    {
        /* Take the first IP */
        struct in_addr *address = (struct in_addr*)m_phost_info->h_addr_list[0];
        m_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*address)); /* Server IP */
    }
    else
    {
        SYS_CRITICAL("Could not resolve hostname %s\n", host);
        return false;
    }

    /* Establish a TCP/IP connection to the SSL client */
    err = connect(m_sockfd, (struct sockaddr*) &m_server_addr, sizeof(m_server_addr));
    if(err == -1)
    {
        SYS_CRITICAL("Could not connect\n");
        return false;
    }

    /* An SSL structure is created */
    m_pssl = SSL_new(m_pctx);
    if(!m_pssl)
    {
        SYS_CRITICAL("Could not get SSL Socket\n");
        return false;
    }

    /* Assign the socket into the SSL structure (SSL and socket without BIO) */
    SSL_set_fd(m_pssl, m_sockfd);

    /* Perform SSL Handshake on the SSL client */
    err = SSL_connect(m_pssl);
    if(err == -1)
    {
        SYS_CRITICAL("Could not connect to SSL Server\n");
        return false;
    }
	//SetSocketOpt(m_sockfd, TRUE);
    //SYS_CRITICAL("ssl_connect %d\n", m_sockfd);
    return true;
}

int CSSLComm::ssl_write(const char * data, int nLen)
{
    CSDLockEx<CSDMutex> v_lock(m_oSDMutex);
    if(!ssl_connected())
    {
        //SYS_CRITICAL(_SDT("[%s:%d] 网络重连 %u\n"), MSG_MARK, errno);
        if(!ssl_connect(m_host, m_port, m_certfile, m_keyfile, m_capath))
            return -1;
    }
    if (NULL == m_pssl)
    {
        return -1;
    }

    int ret	= SSL_write(m_pssl, (const void*)data, nLen);
    if (ret <= 0)
    {
        ssl_Reset();
        return ret;
    }
    return ret;
}

int CSSLComm::ssl_read(const char * data, int nLen)
{
    CSDLockEx<CSDMutex> v_lock(m_oSDMutex);
    if(!ssl_connected())
    {
        SYS_CRITICAL(_SDT("[%s:%d] 网络重连 \n"), MSG_MARK);
        if(!ssl_connect(m_host, m_port, m_certfile, m_keyfile, m_capath))
            return -1;
    }

	if(!ssl_check())
	{
		return -1;
	}
	if (NULL == m_pssl)
	{
		return -1;
	}

    int ret = SSL_read(m_pssl, (void*)data, nLen);
    if (ret <= 0)
    {
        ssl_Reset();
        return ret;
    }
    return ret;
}


int CSSLComm::ssl_read_block(const char * data, int nLen)
{
    CSDLockEx<CSDMutex> v_lock(m_oSDMutex);
    if(!ssl_connected())
    {
        //SYS_CRITICAL(_SDT("[%s:%d] 网络重连 \n"), MSG_MARK);
        if(!ssl_connect(m_host, m_port, m_certfile, m_keyfile, m_capath))
            return -1;
    }
    int ret = 0;
    while(1)
    {
        ret = SSL_read(m_pssl, (void*)data, nLen);
        int nRes = SSL_get_error(m_pssl, ret);
        if(SSL_ERROR_WANT_READ == nRes)
        {
            continue;
        }
        if (ret <= 0)
        {
            ssl_Reset();
            break;
        }
        break;
    }
    return ret;
}
