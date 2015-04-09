
#ifndef SGDP_SDNET_H_20070614
#define SGDP_SDNET_H_20070614

#include "sdbase.h"
#include "config.h"
#include "sdlogger.h"

namespace SGDP{

/**
 * @brief SDNet组件模块版本
 */
static const SSGDPVersion SDNET_MODULE_VERSION = {3, 2, 0, 0};

/**
 * @brief SDNet module name
 */
const char SDNET_MODULENAME[] = "SDNet";

/**
 * @brief SDNet version
 */
const SSGDPVersion	SDNET_VERSION = SDNET_MODULE_VERSION;

/**
 * @brief Error code of SDNet
 */
enum ESDNetErrCode{
	NET_BIND_FAIL	= -7,
    NET_CONNECT_FAIL    = -6,
    NET_SYSTEM_ERROR    = -5, 
    NET_RECV_ERROR      = -4, 
    NET_SEND_ERROR      = -3, 
    NET_SEND_OVERFLOW   = -2,
    NET_PACKET_ERROR    = -1,
    NET_SUCCESS         = 0
};


//网络包头起始标志
#define PKTHEADMAK			0xCCDD
//最大网络包错误次数
#define MAX_NETPKT_ERR_NUM	100

/**
 * @brief 创建ISDConnector或ISDListener时，用户需要指定网络I/O模型。
 */


/**
 * @brief 完成端口
 */
const UINT32 NETIO_COMPLETIONPORT   = 0x0001; 

/**
 * @brief 完成端口，使用sdgate组件
 */
const UINT32 NETIO_COMPLETIONPORT_GATE   = 0x0002;

/**
 * @brief 每次收到数据时，在网络线程中回调给应用
 */
const UINT32 NET_THREADED   = 0x0004;   

/**
 * @brief 每次收到数据时，将在主线程中回调给应用
 */
const UINT32 NET_NONETHREADED   = 0x0008;          

/**
 * @brief 异步I/O的select网络模型，客户端用。
 */
const UINT32 NETIO_ASYNCSELECT      = 2;

/**
 * @brief linux下epoll网络模型
 */
const UINT32 NETIO_EPOLL            = 0x0010;
/**
 * @brief linux下epoll网络模型，选用sdgate组件
 */
const UINT32 NETIO_EPOLL_GATE       = 0x0020;      

const UINT32 INVALID_IO_TYPE        = 0xFFFFFFFF;

const UINT32 UNKNOWN_SIZE	= 0xFFFFFFFF;

class ISDPacketParser;
class ISDSession;
class ISDSessionFactory;

/**
 * @brief An ISDConnection object represent an abstraction of a TCP connection
 */
class ISDConnection
{
public:
    /**
     * @brief Check if the connection is connected
     * @return true means connected, otherwise, false
     */
    virtual bool SDAPI IsConnected(void) = 0;

    /**
     * @brief Send data on the connection
     * @param pBuf : data buffer need to send
     * @param dwLen : data buffer length
     */
    virtual void SDAPI Send(const char* pBuf,UINT32 dwLen) = 0;

    /**
     * @brief Set extension options
     * @param dwType : option type
     * @param pOpt : option value
     */
    virtual void SDAPI SetOpt(UINT32 dwType, void* pOpt) = 0;

    /**
     * @brief Close the TCP connection
     */
    virtual void SDAPI Disconnect(void) = 0;

    /**
     * @brief Retrieves remote IP address for the connection
     * @return IP address four bytes unsigned integer
     */
    virtual const UINT32 SDAPI GetRemoteIP(void) = 0;

    /**
     * @brief Retrieves remote IP address in text format for the connection
     * @return IP address string
     */
    virtual const char* SDAPI GetRemoteIPStr(void) = 0;

    /**
     * @brief Retrieves remote port number for the connection
     * @return port number
     */
    virtual UINT16 SDAPI GetRemotePort(void) = 0;

    /**
     * @brief Retrieves local IP address for the connection
     * @return IP address four bytes unsigned integer
     */
    virtual const UINT32 SDAPI GetLocalIP(void) = 0;

    /**
     * @brief Retrieves local IP address in text format for the connection
     * @return IP address string
     */
    virtual const char* SDAPI GetLocalIPStr(void) = 0;

    /**
     * @brief Retrieves local port number for the connection
     * @return port number
     */
    virtual UINT16 SDAPI GetLocalPort(void) = 0;

    /**
     * @brief Retrieves local send buffer's free size
     * @return local send buffer's free size in bytes
     */
	virtual UINT32 SDAPI GetSendBufFree(void) = 0;
};

/**
 * @brief The interface provides functionality for TCP listening
 */
class ISDListener
{
public:
    /**
     * @brief Set an user implemented ISDPacketParser object, which process
     *        packet parse logic of the connection accepted by the ISDListener
     * @param poPacketParser ISDPacketParser instance pointer
     */
    virtual void SDAPI SetPacketParser(ISDPacketParser* poPacketParser) = 0;

    /**
     * @brief Set the user implemented ISDSessionFactory object to ISDListener
     *
     * When a TCP connection is accepted by ISDListener,
     * ISDSessionFactory object will be asked to create a ISDSession
     * @param poSessionFactory ISDSessionFactory instance factory
     */
    virtual void SDAPI SetSessionFactory(ISDSessionFactory* poSessionFactory) = 0;

    /**
     * @brief Set the send and receive buffer size of the connection accepted by the ISDListener object
     * @param dwRecvBufSize : receiving buffer size in bytes
     * @param dwSendBufSize : sending buffer size in bytes
     */
     virtual void SDAPI SetBufferSize(UINT32 dwRecvBufSize, UINT32 dwSendBufSize) = 0;

    /**
     * @brief Set extension options
     * @param dwType : option type
     * @param pOpt : option value
     */
    virtual void SDAPI SetOpt(UINT32 dwType, void* pOpt) = 0;

    /**
     * @brief Listen at specified IP and port
     * @param pszIP : IP string
     * @param wPort : port number
     * @param bReUseAddr : the flag for re-using same address
     * @return true means success, false means failure
     */
    virtual bool SDAPI Start(const char* pszIP, UINT16 wPort, bool bReUseAddr = true) = 0;

    /**
     * @brief Stop listening
     * @return true means success, false means failure
     */
    virtual bool SDAPI Stop(void) = 0;

    /**
     * @brief Release the ISDListener object
     */
    virtual void SDAPI Release(void) = 0;

};

/**
 * @brief The interface provides functionality for initiative TCP connection
 */
class ISDConnector
{
public:
    /**
     * @brief Set the user implemented ISDPacketParser object, which process
     *        packet parse logic of the connection connected by the ISDConnector
     *        object
     * @param poPakcetParser ISDPacketParser instance pointer
     */
    virtual void SDAPI SetPacketParser(ISDPacketParser* poPakcetParser) = 0;

    /**
     * @brief Set the user implemented ISDSession object, which will be bind
     *        to ISDConnection object when connection establishes
     * @param poSession ISDSession instance pointer
     */
     virtual void SDAPI SetSession(ISDSession* poSession) = 0;

    /**
     * @brief Set the send and receive buffer size for the connection connected
     *        by the ISDConnector object
     * @param dwRecvBufSize : receiving buffer size in bytes
     * @param dwSendBufSize : sending buffer size in bytes
     */
    virtual void SDAPI SetBufferSize(UINT32 dwRecvBufSize, UINT32 dwSendBufSize) = 0;

    /**
     * @brief Connect to the specified address
     * @param pszIP : IP string
     * @param wPort : port number
     * @return true means success, false means failure
     */
    virtual bool SDAPI Connect(const char* pszIP, UINT16 wPort) = 0;

    /**
     * @brief Reconnect to the address which is specified by the last Connect call
     * @return true means success, false means failure
     */
    virtual bool SDAPI ReConnect(void) = 0;

    /**
     * @brief Release the ISDConnector object
     */
    virtual void SDAPI Release(void) = 0;

    /**
     * @brief Set extension options
     * @param dwType : option type
     * @param pOpt : option value
     */
	virtual void SDAPI SetOpt(UINT32 dwType, void* pOpt) = 0;
};

/**
 * @brief Interface of SDNet module
 */
class ISDNet : public ISDBase
{
public:
    /**
     * @brief Create an ISDConnector object
     * @param dwNetIOType : Network I/O type
     * @return ISDConnector instance pointer
     */
    virtual ISDConnector* SDAPI CreateConnector(UINT32 dwNetIOType) = 0;

    /**
     * @brief Create an ISDListener object
     * @param dwNetIOType : Network I/O type
     * @return ISDListener instance pointer
     */
    virtual ISDListener* SDAPI CreateListener(UINT32 dwNetIOType)  = 0;

    /**
     * @brief Process network event
     * @param nCount : the count of loop
     * @return true means success, false means failure
     */
    virtual bool SDAPI Run(INT32 nCount = -1) = 0;
};

/**
 * @brief Break packet from TCP data stream. The interface need to be implemented by user
 */
class ISDPacketParser
{
public:
    /**
     * @brief Return the length in bytes of the packet in TCP data stream
     * @param pBuf : the data buffer need to parse
     * @param dwLen : the data buffer length
     */
    virtual INT32 SDAPI ParsePacket(const char* pBuf, UINT32 dwLen) = 0;
};

/**
 * @brief When network event happens, the related function of ISDSession
 *        will be callback. User need to implement ISDSession
 */
class ISDSession
{
public:
    /**
     * @brief Set the ISDConnection object to the ISDSession object
     * @param poConnection : ISDConnection instance pointer
     */
    virtual void SDAPI SetConnection(ISDConnection* poConnection) = 0;

    /**
     * @brief Callback when the connection is established
     */
    virtual void SDAPI OnEstablish(void) = 0;

    /**
     * @brief Callback when the connection is closed
     */
    virtual void SDAPI OnTerminate(void) = 0;

    /**
     * @brief Callback when TCP connection has error
     */
    virtual bool SDAPI OnError(INT32 nModuleErr, INT32 nSysErr) = 0;

    /**
     * @brief Callback when receive a packet, which is parsed by ISDPacketParser
     * @param pBuf : data buffer pointer which hold data
     * @param dwLen : buffer length
     */
    virtual void SDAPI OnRecv(const char* pBuf, UINT32 dwLen) = 0;

#ifdef SDNET_HAS_SEND_REPORT

    /**
     * @brief Callback when send  success, used in asynch mode
     * @param pBuf : left data in buffer
     * @param left : data length  in buffer
     * @warning don't send the data again
     */
	virtual void SDAPI OnSend(const char* pBuf, UINT32 dwLen) 
	{
	}
#endif

    /**
     * @brief Release ISDSession object
     */
    virtual void SDAPI Release(void) = 0;
};

/**
 * @brief Create user implemented ISDSession object. This interface need to be implemented by user too
 */
class ISDSessionFactory
{
public:
    /**
     * @brief Create an user implemented ISDSession object
     * @param poConnection : ISDConnection instance pointer
     * @return ISDSession instance pointer
     */
    virtual ISDSession* SDAPI CreateSession(ISDConnection* poConnection) = 0;
};

/**
 * @brief Get an SDNet object
 * @param pstVersion : the version of SDNet
 * @return ISDNet instance pointer
 */
ISDNet* SDAPI SDNetGetModule(const SSGDPVersion* pstVersion);

/**
 * @brief Function pointer definition of SDNetGetModule
 */
typedef ISDNet* (SDAPI *PFN_SDNetGetModule)(const SSGDPVersion* pstVersion);

/**
 * @brief Set ISDLogger object to the SDNet. The log of SDNet will export to the ISDLogger object
 * @param poLogger : ISDLogger instance pointer
 * @param dwLevel : log level
 * @return true means success, false means failure
 */
bool SDAPI SDNetSetLogger(ISDLogger* poLogger, UINT32 dwLevel);

/**
 * @brief Function pointer definition of SDNetSetLogger
 */
typedef bool (SDAPI *PFN_SDNetSetLogger)(ISDLogger* poLogger, UINT32 dwLevel);

/**
 * @brief Set extension options
 * @param dwType : option type
 * @param pOpt : option value
 */
void SDAPI SDNetSetOpt(UINT32 dwType, void* pOpt);

}

#endif

