

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

class CZSConfig
{
public:
    CZSConfig();
    ~CZSConfig();
public:
    virtual VOID			UnInit();
    virtual BOOL			Init(const TCHAR* pszFilename);
    SGateListenerInfo*	GetGMListenerInfo();
    UINT32				GetPoolCoreSize();
    UINT32				GetPoolMaxSize();
    BOOL					GetDBAccount(SGDP::SDDBAccount& DBAccount);
    BOOL					GetEncryptFlag()
    {
        return m_bEncryptFlag;
    }
	//http地址
	string				GetRegist(){return m_strRegist;}
	string				Getfindpwd(){return m_strfindpwd;}
	string				Getfindpwd_sendsmscode(){return m_strfindpwd_sendsmscode;}
	string				Getfindpwd_checkcode(){return m_strfindpwd_checkcode;}
	string				Getreportuserdeviceinfo(){return m_strreportuserdeviceinfo;}
	string				Getversionnotice(){return m_strgetversionnotice;}
	string				Getunbinddevice(){return m_strunbinddevice;}
	string				Getlogin(){return m_strlogin;}
	string				Getquickstart(){return m_strquickstart;};
protected:
    BOOL					_LoadGMListenerInfo(CSDXMLFile* poSDXMLFile);
private:
    SGateListenerInfo		m_stGMListenerInfo;
    SGDP::SDDBAccount		m_oDBAccount;   ///< 数据库连接信息
    UINT32				m_dwPoolCoreSize;
    UINT32				m_dwPoolMaxSize;
    BOOL					m_bEncryptFlag;//是否加解密

	string				m_strRegist;
	string				m_strfindpwd;
	string				m_strfindpwd_sendsmscode;
	string				m_strfindpwd_checkcode;
	string				m_strreportuserdeviceinfo;
	string				m_strgetversionnotice;
	string				m_strunbinddevice;
	string				m_strlogin;
	string				m_strquickstart;
};

#endif //#ifndef _GATE_SERVER_SETTING_H_

