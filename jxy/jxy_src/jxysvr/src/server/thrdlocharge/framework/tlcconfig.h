#ifndef _TLC_SERVER_SETTING_H_
#define _TLC_SERVER_SETTING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <sdsingleton.h>
#include <sdxmlfile.h>
#include <sddb.h>
#include <vector>

using namespace std;
using namespace SGDP;

struct SModuleConfig
{
	string strName;
	string strDll;
	string strGetModuleFunc;
	string strConfFile;
};

typedef vector<SModuleConfig> CModuleConfigsVect;
typedef CModuleConfigsVect::const_iterator CModuleConfigsVectItr;

struct SListenerInfo
{
	CHAR				m_szIP[MAX_IP_LEN];
	UINT16				m_wPort;
	UINT32				m_dwRecvBuf;
	UINT32				m_dwSendBuf;
};


class CTLCConfig
{
public:
    DECLARE_SINGLETON(CTLCConfig);

public:
    BOOL    Init(const TCHAR* pszConfig);

public:
	inline UINT32							GetTimeOut() { return m_dwTimeOut; }
	inline const CModuleConfigsVect&		GetAuthModuleConfigs(VOID) const { return m_vecAuthModuleConfigs; }  
	inline const CModuleConfigsVect&		GetRechargeModuleConfigs(VOID) const { return m_vecRechargeModuleConfigs; }  
	inline const SListenerInfo&				GetCliListen() { return m_stCliListen; }
	inline const SListenerInfo&				GetHttpListen() { return m_stHttpListen; }
	inline const SDDBAccount&				GetDBAccount() { return m_stDBAccount; }
	inline const UINT32						GetPoolCoreSize() {	return m_dwPoolCoreSize; }
	inline const UINT32						GetPoolMaxSize() { return m_dwPoolMaxSize; }

protected:
    CTLCConfig();
    ~CTLCConfig();

    BOOL    _LoadCfg(const TCHAR* pszConfig);
	BOOL	_LoadAuthModule(CSDXMLNode rootNode);
	BOOL	_LoadRechargeModule(CSDXMLNode rootNode);
	BOOL	_LoadCliListen(CSDXMLNode rootNode);
	BOOL	_LoadHttpListen(CSDXMLNode rootNode);
	BOOL	_LoadDbInfo(CSDXMLNode rootNode);
private:

	UINT32					m_dwTimeOut;
	CModuleConfigsVect		m_vecAuthModuleConfigs;  
	CModuleConfigsVect		m_vecRechargeModuleConfigs;  
	SListenerInfo			m_stCliListen;
	SListenerInfo			m_stHttpListen;
	SDDBAccount				m_stDBAccount; 
	UINT32					m_dwPoolCoreSize;
	UINT32					m_dwPoolMaxSize;
};

#endif

