
#include "nsconfig.h"
#include <sdutil.h>



#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define CA_CERT_PATH			"pem"
#define RSA_CLIENT_CERT		"rsa_client_cert"
#define RSA_CLIENT_KEY			"rsa_client_key"


CNSConfig::CNSConfig()
{
	m_dwTimerNotice = 0;
	memset(m_szRSA_CLIENT_CERT, 0x00, sizeof(m_szRSA_CLIENT_CERT));
	memset(m_szRSA_CLIENT_KEY, 0x00, sizeof(m_szRSA_CLIENT_KEY));
}

CNSConfig::~CNSConfig()
{
    UnInit();
}

BOOL CNSConfig::Init(const TCHAR* pszFilename)
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
    CSDXMLNode oSettingNode = rootNode["DBMS"];

    SDStrncpy(m_oDBAccount.m_szHostName, oSettingNode(ATTRI_REMOTE_IP).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    m_oDBAccount.m_wConnPort = SDAtoi(oSettingNode(ATTRI_REMOTE_PORT).AsString("-1").c_str());
    SDStrncpy(m_oDBAccount.m_szLoginName, oSettingNode(ATTRI_USERNAME).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szLoginPwd, oSettingNode(ATTRI_PWD).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szDBName, oSettingNode(ATTRI_DBNAME0).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szCharactSet, oSettingNode(ATTRI_CHARACTERSET).AsString("-1").c_str(), SDDB_MAX_CHARACTSET_SIZE);
    m_oDBAccount.m_wDBType = 0;
    m_dwPoolCoreSize = oSettingNode(ELEM_POOLCORESIZE).AsInteger();
    m_dwPoolMaxSize = oSettingNode(ELEM_POOLMAXSIZE).AsInteger();
    m_bEncryptFlag = rootNode["Encrypt"]("value").AsBoolean();

	//oSettingNode = rootNode["CA_RSA"];
	SDStrncpy(m_szRSA_CLIENT_CERT, rootNode["CA_RSA"](RSA_CLIENT_CERT).AsString("-1").c_str(), MAX_PATH);
	SDStrncpy(m_szRSA_CLIENT_KEY, rootNode["CA_RSA"](RSA_CLIENT_KEY).AsString("-1").c_str(), MAX_PATH);
	
	//////////////////////////////////////////////////////////////////////////
	m_dwTimerNotice = rootNode["TimerNotice"]("OnTimerNotice").AsInteger();
	if (m_dwTimerNotice < 0 || m_dwTimerNotice > 24)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: TimerNotice invalid!"), MSG_MARK);   
		return FALSE;
	}
    return TRUE;
}


VOID  CNSConfig::UnInit()
{
}

SGateListenerInfo* CNSConfig::GetGateListenerInfo()
{
    return &m_stGateListenerInfo;
}


BOOL CNSConfig::_LoadGateListenerInfo(CSDXMLFile* poSDXMLFile)
{
    CSDXMLNode rootNode = poSDXMLFile->GetRootNode();
    CSDXMLNode listenNode = rootNode["Listeners/Listener"];
    while (listenNode != NULL)
    {
        SDStrcpy(m_stGateListenerInfo.m_szIP, listenNode(ATTRI_LOCAL_IP).AsString().c_str());
        m_stGateListenerInfo.m_wPort = listenNode(ATTRI_LOCAL_PORT).AsInteger();
        m_stGateListenerInfo.m_dwRecvBuf = listenNode(ATRRI_RECVBUFF).AsInteger();
        m_stGateListenerInfo.m_dwSendBuf = listenNode(ATRRI_SENDBUFF).AsInteger();
        listenNode = listenNode.Sibling();
    }
    return TRUE;
}


UINT32 CNSConfig::GetPoolCoreSize()
{
    return m_dwPoolCoreSize;
}

UINT32 CNSConfig::GetPoolMaxSize()
{
    return m_dwPoolMaxSize;
}

BOOL CNSConfig::GetDBAccount(SGDP::SDDBAccount& DBAccount)
{
    SDStrncpy(DBAccount.m_szHostName, m_oDBAccount.m_szHostName, SDDB_MAX_NAME_SIZE);
    DBAccount.m_wConnPort = m_oDBAccount.m_wConnPort;
    SDStrncpy(DBAccount.m_szLoginName, m_oDBAccount.m_szLoginName, SDDB_MAX_NAME_SIZE);
    SDStrncpy(DBAccount.m_szLoginPwd, m_oDBAccount.m_szLoginPwd, SDDB_MAX_NAME_SIZE);
    SDStrncpy(DBAccount.m_szDBName, m_oDBAccount.m_szDBName, SDDB_MAX_NAME_SIZE);
    SDStrncpy(DBAccount.m_szCharactSet, m_oDBAccount.m_szCharactSet, SDDB_MAX_CHARACTSET_SIZE);
    DBAccount.m_wDBType = 0;
    return TRUE;
}

const CHAR	*CNSConfig::Get_CA_CERT_PATH()
{
	static CHAR s_szTemp[MAX_PATH] = {0};
	memset(s_szTemp, 0x00, sizeof(s_szTemp));
	_SDTsnprintf(s_szTemp, MAX_PATH, _SDT("%s%s"), SDGetModulePath(), "pem");
	return s_szTemp;
}

const CHAR	*CNSConfig::Get_RSA_CLIENT_CERT()
{
	static CHAR s_szTemp[MAX_PATH] = {0};
	memset(s_szTemp, 0x00, sizeof(s_szTemp));
	_SDTsnprintf(s_szTemp, MAX_PATH, _SDT("%s/%s"), Get_CA_CERT_PATH(), m_szRSA_CLIENT_CERT);
	return s_szTemp;
}

const CHAR	*CNSConfig::Get_RSA_CLIENT_KEY()
{
	static CHAR s_szTemp[MAX_PATH] = {0};
	memset(s_szTemp, 0x00, sizeof(s_szTemp));
	_SDTsnprintf(s_szTemp, MAX_PATH, _SDT("%s/%s"), Get_CA_CERT_PATH(), m_szRSA_CLIENT_KEY);
	return s_szTemp;
};

