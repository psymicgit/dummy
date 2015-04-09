

#ifndef _GATE_SERVER_SETTING_H_
#define _GATE_SERVER_SETTING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <string>
#include <list>
#include <vector>
#include <sddb.h>

using namespace std;

typedef struct tagGateListenerInfo
{
	CHAR					m_szIP[MAX_IP_LEN];
	UINT16				m_wPort;
	UINT32				m_dwRecvBuf;
	UINT32				m_dwSendBuf;
} SGateListenerInfo;

class CNSConfig
{
public:
    CNSConfig();
    ~CNSConfig();
public:
    virtual VOID			UnInit();
    virtual BOOL			Init(const TCHAR* pszFilename);
    SGateListenerInfo*	GetGateListenerInfo();
    UINT32				GetPoolCoreSize();
    UINT32				GetPoolMaxSize();
    BOOL					GetDBAccount(SGDP::SDDBAccount& DBAccount);
    BOOL					GetEncryptFlag(){return m_bEncryptFlag;}
	const CHAR		*	Get_CA_CERT_PATH();
	const CHAR		*	Get_RSA_CLIENT_CERT();
	const CHAR		*	Get_RSA_CLIENT_KEY();
	const UINT32 GetNoticeTime(){return m_dwTimerNotice;};
protected:
    BOOL					_LoadGateListenerInfo(CSDXMLFile* poSDXMLFile);
private:
    SGateListenerInfo		m_stGateListenerInfo;
    SGDP::SDDBAccount		m_oDBAccount;   ///< 数据库连接信息
    UINT32				m_dwPoolCoreSize;
    UINT32				m_dwPoolMaxSize;
    BOOL					m_bEncryptFlag;//是否加解密
	CHAR					m_szRSA_CLIENT_CERT[MAX_PATH];
	CHAR					m_szRSA_CLIENT_KEY[MAX_PATH];
	UINT32				m_dwTimerNotice;
};


#endif //#ifndef _GATE_SERVER_SETTING_H_

