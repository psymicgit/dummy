
#include "zsconfig.h"
#include <sdutil.h>

#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define ATTRI_UNIXSOCKET		"UnixSocket"

CZSConfig::CZSConfig()
{

}

CZSConfig::~CZSConfig()
{
    UnInit();
}

BOOL CZSConfig::Init(const TCHAR* pszFilename)
{
    CSDXMLFile oSDXMLFile;
    if (FALSE == oSDXMLFile.Load(_SDTT2A(pszFilename)))
    {
        return FALSE;
    }
    // GM Listener
    if (!_LoadGMListenerInfo(&oSDXMLFile))
    {
        SYS_CRITICAL(_SDT("[%s:%d]--Load client listener info failed."), MSG_MARK);
        return FALSE;
    }

    CSDXMLNode rootNode = oSDXMLFile.GetRootNode();
    CSDXMLNode oDBMSNode = rootNode["DBMS"];

    SDStrncpy(m_oDBAccount.m_szHostName,	oDBMSNode(ATTRI_REMOTE_IP).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    m_oDBAccount.m_wConnPort = SDAtoi(oDBMSNode(ATTRI_REMOTE_PORT).AsString("-1").c_str());
    SDStrncpy(m_oDBAccount.m_szLoginName,	oDBMSNode(ATTRI_USERNAME).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szLoginPwd,	oDBMSNode(ATTRI_PWD).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szDBName,		oDBMSNode(ATTRI_DBNAME0).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szCharactSet,	oDBMSNode(ATTRI_CHARACTERSET).AsString("-1").c_str(), SDDB_MAX_CHARACTSET_SIZE);
    SDStrncpy(m_oDBAccount.m_szUnixSocket,	oDBMSNode(ATTRI_UNIXSOCKET).AsString("").c_str(), SDDB_MAX_NAME_SIZE);
    m_oDBAccount.m_wDBType = 0;
    m_dwPoolCoreSize = oDBMSNode(ELEM_POOLCORESIZE).AsInteger();
    m_dwPoolMaxSize = oDBMSNode(ELEM_POOLMAXSIZE).AsInteger();
    m_bEncryptFlag = rootNode["Encrypt"]("value").AsBoolean();

    m_strRegist				= rootNode["regist"].AsString();
    m_strfindpwd				= rootNode["findpwd"].AsString();
    m_strfindpwd_sendsmscode	= rootNode["findpwd_sendsmscode"].AsString();
    m_strfindpwd_checkcode		= rootNode["findpwd_checkcode"].AsString();
    m_strreportuserdeviceinfo	= rootNode["reportuserdeviceinfo"].AsString();
    m_strgetversionnotice		= rootNode["getversionnotice"].AsString();
    m_strunbinddevice			= rootNode["unbinddevice"].AsString();
    m_strlogin				= rootNode["login"].AsString();
    m_strquickstart			= rootNode["quickstart"].AsString();

    return TRUE;
}

VOID  CZSConfig::UnInit()
{
}


SGateListenerInfo* CZSConfig::GetGMListenerInfo()
{
    return &m_stGMListenerInfo;
}

BOOL CZSConfig::_LoadGMListenerInfo(CSDXMLFile* poSDXMLFile)
{
    CSDXMLNode rootNode	= poSDXMLFile->GetRootNode();
    CSDXMLNode gmListenNode	= rootNode["gm"];
    if (gmListenNode != NULL)
    {
        SDStrncpy(m_stGMListenerInfo.m_szIP, gmListenNode(ATTRI_LOCAL_IP).AsString().c_str(), MAX_IP_LEN);
        m_stGMListenerInfo.m_wPort		= gmListenNode(ATTRI_LOCAL_PORT).AsInteger();
        m_stGMListenerInfo.m_dwRecvBuf	= gmListenNode(ATRRI_RECVBUFF).AsInteger();
        m_stGMListenerInfo.m_dwSendBuf	= gmListenNode(ATRRI_SENDBUFF).AsInteger();
        gmListenNode = gmListenNode.Sibling();
    }
    return TRUE;
}

UINT32 CZSConfig::GetPoolCoreSize()
{
    return m_dwPoolCoreSize;
}

UINT32 CZSConfig::GetPoolMaxSize()
{
    return m_dwPoolMaxSize;
}

BOOL CZSConfig::GetDBAccount(SGDP::SDDBAccount& DBAccount)
{
    SDStrncpy(DBAccount.m_szHostName, m_oDBAccount.m_szHostName, SDDB_MAX_NAME_SIZE);
    DBAccount.m_wConnPort = m_oDBAccount.m_wConnPort;
    SDStrncpy(DBAccount.m_szLoginName, m_oDBAccount.m_szLoginName, SDDB_MAX_NAME_SIZE);
    SDStrncpy(DBAccount.m_szLoginPwd, m_oDBAccount.m_szLoginPwd, SDDB_MAX_NAME_SIZE);
    SDStrncpy(DBAccount.m_szDBName, m_oDBAccount.m_szDBName, SDDB_MAX_NAME_SIZE);
    SDStrncpy(DBAccount.m_szCharactSet, m_oDBAccount.m_szCharactSet, SDDB_MAX_CHARACTSET_SIZE);
    SDStrncpy(DBAccount.m_szUnixSocket, m_oDBAccount.m_szUnixSocket, SDDB_MAX_NAME_SIZE);
    DBAccount.m_wDBType = 0;
    return TRUE;
}

