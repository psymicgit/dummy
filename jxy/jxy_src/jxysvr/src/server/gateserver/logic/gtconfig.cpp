
#include "gtconfig.h"
#include <sdutil.h>

#define   ELEM_CKSESSIONTIMERELAPSE	"CkSessionTimerElapse"
#define   ELEM_DEALSESSIONNUMR		"DealSessionNum"
#define   ELEM_CLOSESESSIONELAPSE	"CloseSessionElapse"
#define   ELEM_ENCRYPT				"Encrypt"
#define	 LOGIN_URL				"LoginUrl"
#define	 NOTICE_URL				"NoticeUrl"
#define  REGISTER_URL			"RegisterUrl"
#define  LOGOUT_URL				"LogoutUrl"
#define  CHANGE_PWD_URL			"ChangePwdUrl"
#define  GET_USER_EXPAND        "GetUserExpand"
#define  LOGINEX_URL			"LoginExUrl"
#define  BIND_MOBILE_URL        "BindMobileUrl"
#define  UN_BIND_MOBILE_URL     "UnBindMobileUrl"
#define  BIND_CHECK_VER_CODE    "BindCheckVerCode"
#define  UN_BIND_CHECK_VER_CODE    "UnBindCheckVerCode"
CGTConfig::CGTConfig()
{
    m_dwDealSessionNum = 0;
    m_dwCloseSessionElapse = 0;
    m_dwCkSessionTimerElapse = 0;
}

CGTConfig::~CGTConfig()
{
	UnInit();
}

BOOL CGTConfig::Init(const TCHAR* pszFilename)
{
    CSDXMLFile oSDXMLFile;
    if (FALSE == oSDXMLFile.Load(_SDTT2A(pszFilename)))
    {
        return FALSE;
    }
    // Gate Listener
    if (!_LoadGateListenerInfo(&oSDXMLFile))
    {
        SYS_CRITICAL(_SDT("[%s:%d]--Load client listener info failed."), MSG_MARK);
        return FALSE;
    }
    CSDXMLNode rootNode = oSDXMLFile.GetRootNode();
    m_dwDealSessionNum = rootNode[ELEM_DEALSESSIONNUMR].AsInteger();
    m_dwCloseSessionElapse = rootNode[ELEM_CLOSESESSIONELAPSE].AsInteger();
    m_dwCkSessionTimerElapse = rootNode[ELEM_CKSESSIONTIMERELAPSE].AsInteger();
	m_bEncryptFlag = rootNode[ELEM_ENCRYPT]("value").AsBoolean();

	m_strLoginUrl = rootNode[LOGIN_URL].AsString();
	m_strNoticeUrl = rootNode[NOTICE_URL].AsString();
	m_strRegisterurl = rootNode[REGISTER_URL].AsString();
	m_strLogoutUrl = rootNode[LOGOUT_URL].AsString();
	m_strChangePwdUrl = rootNode[CHANGE_PWD_URL].AsString();
	m_strGetUserExpandUrl = rootNode[GET_USER_EXPAND].AsString();
	m_strLoginExUrl = rootNode[LOGINEX_URL].AsString();
	m_strBindMobileUrl = rootNode[BIND_MOBILE_URL].AsString();
    m_strUnBindMobileUrl = rootNode[UN_BIND_MOBILE_URL].AsString();
    m_strBindCheckVerCode = rootNode[BIND_CHECK_VER_CODE].AsString();
    m_strUnBindCheckVerCode = rootNode[UN_BIND_CHECK_VER_CODE].AsString();
	return TRUE;
}


VOID  CGTConfig::UnInit()
{	
}

SGateListenerInfo* CGTConfig::GetGateListenerInfo()
{
    return &m_stGateListenerInfo;
}


BOOL CGTConfig::_LoadGateListenerInfo(CSDXMLFile* poSDXMLFile)
{
    CSDXMLNode rootNode = poSDXMLFile->GetRootNode();
    CSDXMLNode listenNode = rootNode["Listeners/Listener"];
    while (listenNode!=NULL)
    {
        SDStrcpy(m_stGateListenerInfo.m_szIP,listenNode(ATTRI_LOCAL_IP).AsString().c_str());
        m_stGateListenerInfo.m_wPort = listenNode(ATTRI_LOCAL_PORT).AsInteger();
        m_stGateListenerInfo.m_dwRecvBuf = listenNode(ATRRI_RECVBUFF).AsInteger();
        m_stGateListenerInfo.m_dwSendBuf = listenNode(ATRRI_SENDBUFF).AsInteger();
        listenNode = listenNode.Sibling();
    }
    return TRUE;
}

UINT32 CGTConfig::GetDealSessionNum()
{
    return m_dwDealSessionNum;
}

UINT32 CGTConfig::GetCloseSessionElapse()
{
    return m_dwCloseSessionElapse;
}

UINT32 CGTConfig::GetCkSessionTimerElapse()
{
    return m_dwCkSessionTimerElapse;
}

BOOL CGTConfig::GetEncryptFlag()
{
	return m_bEncryptFlag;
}
