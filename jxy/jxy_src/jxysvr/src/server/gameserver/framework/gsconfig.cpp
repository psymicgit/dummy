
#include "gsconfig.h"
#include <sdutil.h>
#include <sdstring.h>

#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define ATTRI_UNIXSOCKET		"UnixSocket"

IMPLEMENT_SINGLETON(CGSConfig)

CGSConfig::CGSConfig()
    : m_bPKServer(FALSE)
{
    memset(m_szDBConfigPath, 0, MAX_PATH);
    memset(m_szConfigFile, 0, MAX_PATH);

	m_dwRechargePort=0;
	m_nPayType=0;
	m_qwServerOpentTime = 0;
}

CGSConfig::~CGSConfig()
{
    UnInit() ;
}

BOOL CGSConfig::Init(const TCHAR* pszConfig)
{
    CSDXMLFile oSDXMLFile;
    if (FALSE == oSDXMLFile.Load(_SDTT2A(pszConfig)))
    {
        return FALSE;
    }
    SDStrcpy(m_szConfigFile,_SDTT2A(pszConfig));
    if(!_LoadCfg(&oSDXMLFile))
	{
		USR_INFO(_SDT("[%s: %d]: _LoadCfg failed!"), MSG_MARK);   
		return FALSE;
	}

	if(!_LoadGateListenerInfo(&oSDXMLFile))
	{
		USR_INFO(_SDT("[%s: %d]: _LoadGateListenerInfo failed!"), MSG_MARK);   
		return FALSE;
	}
	
	return TRUE;
}


VOID  CGSConfig::UnInit()
{	

}

BOOL CGSConfig::_LoadCfg(CSDXMLFile* poConfig)
{
    CSDXMLNode oRootNode = poConfig->GetRootNode() ;
    string strValue;

	m_dwOnlineSaveTimeInterval = oRootNode["TimerCfg"]("OnlineSaveTimeInterval").AsInteger();
	m_dwOfflineSaveCkInterval = oRootNode["TimerCfg"]("OfflineSaveCkInterval").AsInteger();
	m_dwOfflineCacheTime = oRootNode["TimerCfg"]("OfflineCacheTime").AsInteger();
	m_dwPvpSaveCkInterval = oRootNode["TimerCfg"]("PvpSaveCkInterval").AsInteger();
	m_dwPvpSaveNum = oRootNode["TimerCfg"]("PvpSaveNum").AsInteger();
	m_dwPlayerSaveInterval=oRootNode["SavePlayerCfg"]("SavePlayerInterval").AsInteger();
	m_dwFillePlayerQueueInterval=oRootNode["SavePlayerCfg"]( "FillePlayerQueueInterval" ).AsInteger();

	m_strRechargeIP=oRootNode["Recharge"]( "ip" ).AsString();
	m_dwRechargePort=oRootNode["Recharge"]( "port" ).AsInteger();
	m_nPayType=oRootNode["PayConfig"]("paytype").AsInteger();

	m_strAppPayUrl=oRootNode["APPPay"]("Url").AsString();
	m_strAppPayTableName=oRootNode["APPPay"]( "PayTableName" ).AsString();
	if((0 == m_dwOnlineSaveTimeInterval) || (0 == m_dwOfflineSaveCkInterval) || (0 == m_dwOfflineCacheTime)
		|| (0 == m_dwPvpSaveCkInterval) || (0 == m_dwPvpSaveNum)
		)
	{
		USR_INFO(_SDT("[%s: %d]: TimerCfg invalid!"), MSG_MARK);   
		return FALSE;
	}

	m_dwTimerNotice = oRootNode["TimerNotice"]("OnTimerNotice").AsInteger();
	if (m_dwTimerNotice < 0 || m_dwTimerNotice > 24)
	{
		USR_INFO(_SDT("[%s: %d]: TimerNotice invalid!"), MSG_MARK);   
		return FALSE;
	}

	m_dwChatSyncTime = oRootNode["Chat"]("SyncTimer").AsInteger();
	if (m_dwChatSyncTime < 60)
	{  
		m_dwChatSyncTime = 60;
	}

	m_dwChatSendNotice = oRootNode["Chat"]("SendTimer").AsInteger();
	if (m_dwChatSendNotice < 10)
	{  
		m_dwChatSendNotice = 10;
	}

	m_strSpeCfgTablePrefix = oRootNode["SpeCfgTablePrefix"]( "Prefix" ).AsString();
	
	CSDDateTime stDateTime;
	if(SDTimeFromString(oRootNode["ServerOpenTime"]( "Time" ).AsString(), stDateTime))
	{
		m_qwServerOpentTime = stDateTime.GetTimeValue();
	}	
	
	CSDXMLNode oSettingNode=oRootNode["DBMS"];
	LoadDBConnInfo( oSettingNode, m_oDBAccount );	
	m_dwPoolCoreSize = oSettingNode(ELEM_POOLCORESIZE).AsInteger();
	m_dwPoolMaxSize = oSettingNode(ELEM_POOLMAXSIZE).AsInteger();

	CSDXMLNode oRecordDBSettingNode=oRootNode["RecordDBMS"];
	LoadDBConnInfo( oRecordDBSettingNode, m_oRecordDBAccount );
	m_dwRecordPoolCoreSize = oRecordDBSettingNode(ELEM_POOLCORESIZE).AsInteger();
	m_dwRecordPoolMaxSize = oRecordDBSettingNode(ELEM_POOLMAXSIZE).AsInteger();

	CSDXMLNode oPayDBSettingNode=oRootNode["PayDBMS"];
	LoadDBConnInfo( oPayDBSettingNode, m_oPayDBAccount );
	m_dwPayPoolCoreSize = oPayDBSettingNode(ELEM_POOLCORESIZE).AsInteger();
	m_dwPayPoolMaxSize = oPayDBSettingNode(ELEM_POOLMAXSIZE).AsInteger();

	CSDXMLNode oCfgBSettingNode=oRootNode["ConfigureDBMS"];
	LoadDBConnInfo( oCfgBSettingNode, m_oCfgDBAccount );

    return TRUE;
}


BOOL CGSConfig::LoadDBConnInfo( CSDXMLNode &oSettingNode, SGDP::SDDBAccount &oDBAccount )
{
	SDStrncpy(oDBAccount.m_szHostName, oSettingNode(ATTRI_REMOTE_IP).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
	oDBAccount.m_wConnPort = SDAtoi(oSettingNode(ATTRI_REMOTE_PORT).AsString("-1").c_str());
	SDStrncpy(oDBAccount.m_szLoginName, oSettingNode(ATTRI_USERNAME).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
	SDStrncpy(oDBAccount.m_szLoginPwd, oSettingNode(ATTRI_PWD).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
	SDStrncpy(oDBAccount.m_szDBName, oSettingNode(ATTRI_DBNAME0).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
	SDStrncpy(oDBAccount.m_szCharactSet, oSettingNode(ATTRI_CHARACTERSET).AsString("-1").c_str(), SDDB_MAX_CHARACTSET_SIZE);
	SDStrncpy(oDBAccount.m_szUnixSocket, oSettingNode(ATTRI_UNIXSOCKET).AsString("").c_str(), SDDB_MAX_NAME_SIZE);
	
	oDBAccount.m_wDBType = 0;

	return TRUE;
}


// 获取数据库连接信息
BOOL CGSConfig::GetDBConfig(CHAR* pszHost, UINT16 &wPort, CHAR* pszLoginName, CHAR* pszPassword, CHAR* pszDBName, CHAR* pszCharacterSet, CHAR* pszUnixSocket)
{
    SDStrcpy(pszHost, m_oDBAccount.m_szHostName);
    wPort = m_oDBAccount.m_wConnPort;
    SDStrcpy(pszLoginName, m_oDBAccount.m_szLoginName);
    SDStrcpy(pszPassword, m_oDBAccount.m_szLoginPwd);
    SDStrcpy(pszDBName, m_oDBAccount.m_szDBName);
    SDStrcpy(pszCharacterSet, m_oDBAccount.m_szCharactSet);
	SDStrcpy(pszUnixSocket, m_oDBAccount.m_szUnixSocket);
    return TRUE;
}

// 获取配置数据库文件地址
BOOL CGSConfig::GetConfigDataPath(CHAR* pszPath)
{
    SDStrcpy(pszPath, m_szDBConfigPath);
    return TRUE;
}

tstring CGSConfig::GetConfigFile()
{
    return _SDTA2T(m_szConfigFile);
}

UINT32 CGSConfig::GetPoolCoreSize()
{	
	return m_dwPoolCoreSize;
}

UINT32 CGSConfig::GetPoolMaxSize()
{
	return m_dwPoolMaxSize;
}

BOOL CGSConfig::CopyDBAccountInfo( SGDP::SDDBAccount &DBAccountDst, const SGDP::SDDBAccount &DBAccountSrc )
{
	SDStrncpy(DBAccountDst.m_szHostName,DBAccountSrc.m_szHostName, SDDB_MAX_NAME_SIZE);
	DBAccountDst.m_wConnPort = DBAccountSrc.m_wConnPort;
	SDStrncpy(DBAccountDst.m_szLoginName, DBAccountSrc.m_szLoginName, SDDB_MAX_NAME_SIZE);
	SDStrncpy(DBAccountDst.m_szLoginPwd,DBAccountSrc.m_szLoginPwd, SDDB_MAX_NAME_SIZE);
	SDStrncpy(DBAccountDst.m_szDBName,DBAccountSrc.m_szDBName, SDDB_MAX_NAME_SIZE);
	SDStrncpy(DBAccountDst.m_szCharactSet, DBAccountSrc.m_szCharactSet, SDDB_MAX_CHARACTSET_SIZE);
	SDStrncpy(DBAccountDst.m_szUnixSocket, m_oDBAccount.m_szUnixSocket, SDDB_MAX_NAME_SIZE);
	DBAccountDst.m_wDBType = DBAccountSrc.m_wDBType;

	return TRUE;
}

BOOL CGSConfig::GetDBAccount(SGDP::SDDBAccount& DBAccount)
{
	return CopyDBAccountInfo( DBAccount, m_oDBAccount );
}

BOOL CGSConfig::GetRecordDBAccount( SGDP::SDDBAccount &DBAccount )
{
	return CopyDBAccountInfo( DBAccount, m_oRecordDBAccount );
}

BOOL CGSConfig::GetCfgDBAccount( SGDP::SDDBAccount& DBAccount )
{
	return CopyDBAccountInfo( DBAccount, m_oCfgDBAccount );
}

UINT32 CGSConfig::GetRecordPoolCoreSize()
{
	return m_dwRecordPoolCoreSize;
}

UINT32 CGSConfig::GetRecordPoolMaxSize()
{
	return m_dwRecordPoolMaxSize;
}

BOOL	CGSConfig::GetPayDBAccount( SGDP::SDDBAccount& DBAccount )
{
	return CopyDBAccountInfo( DBAccount, m_oPayDBAccount );
}

UINT32	CGSConfig::GetPayPoolCoreSize()
{
	return m_dwPayPoolCoreSize;
}

UINT32	CGSConfig::GetPayPoolMaxSize()
{
	return m_dwPayPoolMaxSize;
}
BOOL CGSConfig::_LoadGateListenerInfo(CSDXMLFile* poSDXMLFile)
{
	CSDXMLNode rootNode = poSDXMLFile->GetRootNode();
	CSDXMLNode oListenXML=rootNode["RechargeListen"];
	if(oListenXML == NULL)
	{
		return FALSE;
	}
	m_stRechargeListenerInfo.m_strIP = oListenXML(ATTRI_IP).AsString().c_str();
	m_stRechargeListenerInfo.m_wPort = oListenXML(ATTRI_PORT).AsInteger();
	m_stRechargeListenerInfo.m_dwRecvBuf = oListenXML(ATRRI_RECVBUFF).AsInteger();
	m_stRechargeListenerInfo.m_dwSendBuf = oListenXML(ATRRI_SENDBUFF).AsInteger();


	return TRUE;
}