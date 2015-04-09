#ifndef _MOCKSESSION_H_
#define _MOCKSESSION_H_

#include "sdnet.h"
#include "clidefine.h"
#include "mgencrypt.h"

class CCliSession : public SGDP::ISDSession
{
public:
    /**
     * @brief 构建网络头
     * @return 返回报文大小
     */
    INT32 BuildNetHeader(STNetMsgHeader* pstHead, UINT16 wMsgID, UINT32 dwDataLen);
    
    /**
     * @brief 获取连接
     * @return 返回连接接口指针
     */
    SGDP::ISDConnection* GetConnection();

    /**
     * @brief 发送协议报文
     * @param wMsgID : 协议ID
     * @param pBuf : 缓冲区地址
     * @param nLen : 缓冲区大小
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL SendMsg(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen);

public:
    //
    // Name     : SetConnection
    // Function : Set the ISDConnection object to the ISDSession object.
    //
    virtual VOID SDAPI SetConnection(SGDP::ISDConnection* poConnection);

    //
    // Name     : OnEstablish
    // Function : Callback when the connection is established.
    //
    virtual VOID SDAPI OnEstablish();

    //
    // Name     : OnTerminate
    // Function : Callback when the connection is closed.
    //
    virtual VOID SDAPI OnTerminate();

    //
    // Name     : OnError
    // Function : Callback when TCP connection has error.
    //
    virtual bool SDAPI OnError(INT32 nModuleErr, INT32 nSysErr);

    //
    // Name     : OnRecv
    // Function : Callback when receive a packet, which is parsed by ISDPacketParser.
    //            
    //
    virtual VOID SDAPI OnRecv(const char* pBuf, UINT32 dwLen);

    //
    // Name     : Release
    // Function : Release ISDSession object. It will be callback when SDNet no
    //            more use it.
    //
    virtual VOID SDAPI Release();

	MGEncrypt& GetMGEncrypt() { return m_oMGEncrypt; }

public:
	VOID SetKey(const unsigned char * pEncryptKey, unsigned int keyLength){ m_oMGEncrypt.setKey(pEncryptKey, keyLength); }
	VOID ClrKey() { m_oMGEncrypt.clear(); }

protected:
    SGDP::ISDConnection*  m_poConnection;         ///< TCP连接，必须成员，存放发送报文接口

	//加解密
	MGEncrypt	m_oMGEncrypt;
};

#endif // #ifndef _MOCKSESSION_H_

