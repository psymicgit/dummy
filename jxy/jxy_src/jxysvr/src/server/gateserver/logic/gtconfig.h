

#ifndef _GATE_SERVER_SETTING_H_
#define _GATE_SERVER_SETTING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <string>
#include <list>
#include <vector>

using namespace std;

typedef struct tagGateListenerInfo
{
    CHAR					m_szIP[MAX_IP_LEN];
    UINT16					m_wPort;
    UINT32					m_dwRecvBuf;
    UINT32					m_dwSendBuf;
} SGateListenerInfo;

class CGTConfig
{
public:
    CGTConfig();
    ~CGTConfig();
public:
    virtual VOID				UnInit();
    virtual BOOL				Init(const TCHAR* pszFilename);
    SGateListenerInfo*		GetGateListenerInfo();
    UINT32					GetDealSessionNum();
    UINT32					GetCloseSessionElapse();
    UINT32					GetCkSessionTimerElapse();
    BOOL						GetEncryptFlag();
    string					GetLoginUrl()
    {
        return m_strLoginUrl;
    };
	string					GetNoticeUrl()
	{
		return m_strNoticeUrl;
	};
	string GetRegisterurl() {return m_strRegisterurl;}

	string GetLogoutUrl() { return m_strLogoutUrl; }

	string GetChangePwdUrl() { return m_strChangePwdUrl; }

	string GetUserExpandUrl() { return m_strGetUserExpandUrl; }

	string GetLoginExUrl() { return m_strLoginExUrl; }
    string                  GetBindMobileUrl()
    {
        return m_strBindMobileUrl;
    }
    string                  GetUnBindMobileUrl()
    {
        return m_strUnBindMobileUrl;
    }
    string                  GetBindChenckVerCode()
    {
        return m_strBindCheckVerCode;
    }
    string                  GetUnBindChenckVerCode()
    {
        return m_strUnBindCheckVerCode;
    }
protected:
    BOOL				_LoadGateListenerInfo(CSDXMLFile* poSDXMLFile);
private:
    SGateListenerInfo	m_stGateListenerInfo;
    UINT32           m_dwDealSessionNum;
    UINT32			m_dwCloseSessionElapse; //单位秒
    UINT32           m_dwCkSessionTimerElapse; //单位秒
    BOOL				m_bEncryptFlag;//是否加解密

	string	m_strLoginExUrl;
    string			m_strLoginUrl;
	string			m_strNoticeUrl;
	string			m_strRegisterurl;
	string m_strLogoutUrl;
	string m_strChangePwdUrl;
	string m_strGetUserExpandUrl;
    string          m_strBindMobileUrl;
    string          m_strUnBindMobileUrl;
    string          m_strBindCheckVerCode;
    string          m_strUnBindCheckVerCode;
};

#endif //#ifndef _GATE_SERVER_SETTING_H_

