/******************************************************************************
            Copyright (C) Shanda Corporation. All rights reserved.

 sdpipe.h - Interfaces, classes, macros, types and definitions for pipe module.
            Include file for sdcore.dll. This library is based on net module
            and provide much user friendly API. It provides message-oriented 
            communication as well as connection reestablishing functionality.

 Class    - ISDPipeModule
          - ISDPipeReporter
          - ISDPipe
          - ISDPipeSink
          
 Struct   - ISDPipeId


 Remarks  - SDPipe module is based on SDNet module and provides an easy to use
            network programming interface. It reliefs users from network 
            programming details such as connection establishment, partial data
            sending/receiving, reconnecting etc, so users only need to concentr-
            ate on their business logics.

            SDPipe is pure virtual C++ function based. It contains a set of pure
            virtual C++ class, include both interfaces implemented inside SDPipe
            and callback interfaces which to be implemented by user.
            
******************************************************************************/

#ifndef SGDP_SDPIPE_H_20070613
#define SGDP_SDPIPE_H_20070613

#include "sdbase.h"
#include "sdnet.h"
namespace SGDP{


/**
* @brief SDPipe module name
*/
const char SDPIPE_MODULENAME[] = "SDPipe";

/**
* @brief SDPipe version
*/
const SSGDPVersion	SDPIPE_VERSION = SDNET_MODULE_VERSION;

/**
* @brief Error code of SDPipe
*/
enum ESDPipeCode{
    /**
    * @brief Remote pipe id error
    */
    PIPE_REMOTEID_ERR   = -3,

    /**
    * @brief Duplicated pipe connection
    */
    PIPE_REPEAT_CONN    = -2,  

    /**
    * @brief Pipe disconnected
    */
    PIPE_DISCONNECT     = -1,  

    /**
    * @brief Pipe connected
    */
    PIPE_SUCCESS        = 0     
};

/**
* @brief Default parameters¡£
*/

/**
* @brief Default send buffer size
*/
#define PIPE_DFLT_SENDBUF_SIZE        (0x00000001<<12)

/**
* @brief Default receive buffer size
*/
#define PIPE_DFLT_RECVBUF_SIZE       PIPE_DFLT_SENDBUF_SIZE

/**
* @brief Pipe id elements
*/
struct SSDPipeId
{
    UINT8 byRegion;      ///< Region
    UINT8 byGroup;       ///< Group
    UINT8 byType;        ///< Type
    UINT8 byIndex;       ///< Index
};

/**
* @brief Connection level callback interface.
*/
class ISDPipeSink
{
public:
    /**
    * @brief Data arriving callback.
    */
    virtual void SDAPI OnRecv(UINT16 wBusinessID, const char* pData, UINT32 dwLen) = 0;

    /**
    * @brief Pipe status changed callback.
    */
    virtual void SDAPI OnReport(UINT16 wBusinessID, INT32 nErrCode) = 0;

};

/**
* @brief Represent a remote pipe.
*/
class ISDPipe
{
public:
    /**
    * @brief Get remote pipe id.
    */
    virtual UINT32 SDAPI GetID(void) = 0;

    /**
    * @brief Send data to remote pipe.
    */
    virtual bool SDAPI Send(UINT16 wBusinessID, const char* pData, UINT32 dwLen) = 0;

    /**
    * @brief Associate user data for business id.
    */
    virtual void SDAPI SetUserData(UINT16 wBusinessID, UINT32 dwData) = 0;

    /**
    * @brief Get user data for business id.
    */
    virtual bool SDAPI GetUserData(UINT16 wBusinessID, UINT32* pdwData) = 0;

    /**
    * @brief Set connection level callback for business id.
    */
    virtual bool SDAPI SetSink(UINT16 wBusinessID, ISDPipeSink* pSink) = 0;

    /**
    * @brief Get connection level callback for business id.
    */
    virtual ISDPipeSink* SDAPI GetSink(UINT16 wBusinessID) = 0;

    /**
    * @briefGet ip for remote ip.
    */
    virtual UINT32 SDAPI GetIP(void) = 0;

};

/**
* @brief Module level callback.
*/
class ISDPipeReporter
{
public:
    /**
    * @brief Pipe status changed callback.
    */
    virtual void SDAPI OnReport(INT32 nErrCode, UINT32 dwID) = 0;

};

/**
* @brief Represent a pipe module(a local pipe).
*/
class ISDPipeModule : public ISDBase
{
public:
    /**
    * @brief Init pipe module.
    */
    virtual bool SDAPI Init(const char* pszConfFile, const char* pszIPListFile, ISDPipeReporter* pReporter, ISDNet* pNetModule) = 0;

    /**
    * @brief Find remote pipe by id.
    */
    virtual ISDPipe* SDAPI GetPipe(UINT32 dwID) = 0;

    /**
    * @brief Process pipe event.
    */
    virtual bool SDAPI Run(INT32 nCount = -1) = 0;

    /**
    * @brief Add a actively connecting pipe.
    */
    virtual bool SDAPI AddConn(UINT32 dwID, const char* pszRemoteIP, UINT16 wRemotePort, UINT32 dwRecvBuf=PIPE_DFLT_RECVBUF_SIZE, UINT32 dwSendBuf=PIPE_DFLT_SENDBUF_SIZE) = 0;

    /**
    * @brief Add a passively connecting pipe.
    */
    virtual bool SDAPI AddListen(const char* pszLocalIP, UINT16 wLocalPort, UINT32 dwRecvBuf=PIPE_DFLT_RECVBUF_SIZE, UINT32 dwSendBuf=PIPE_DFLT_SENDBUF_SIZE) = 0;

    /**
    * @brief Get local pipe id.
    */
    virtual UINT32 SDAPI GetLocalID(void) = 0;

    /**
    * @brief Reload ip list.
    */
    virtual bool SDAPI ReloadIPList(const char* pszIPListFile) = 0;
};

/**
* @brief Function prototype for SDPipeGetModule
*/
typedef ISDPipeModule* (SDAPI *PFN_SDPipeGetModule)(const SSGDPVersion* pstVersion);

/**
* @brief Get an SDPipe object.
*/
ISDPipeModule* SDAPI SDPipeGetModule(const SSGDPVersion* pstVersion);

/**
* @brief Function prototype for SDPipeSetLogger
*/
typedef bool (SDAPI *PFN_SDPipeSetLogger)(ISDLogger* poLogger, UINT32 dwLevel);

/**
* @brief Set ISDLogger object for the SDPipe. The log of SDPipe will export
*            to the ISDLogger object.
*/
bool SDAPI SDPipeSetLogger(ISDLogger* poLogger, UINT32 dwLevel);


/*
//
// Name     : idtoa
// Function : Convert numberic id to string id.
//
const char * SDAPI idtoa(UINT32 dwID, char *pszID);

//
// Name     : atoid
// Function : Convert string id to numberic id.
//
UINT32 SDAPI atoid(const char *pszID);
*/

} // namespace SGDP

#endif

