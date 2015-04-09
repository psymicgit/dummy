//
//#include "settings.h"
#include "dbconfig.h"
#include <sdstring.h>
#include <sdserverid.h>
#include <sdnetutils.h>
#include "framework/lsapplication.h"
#include <sdtime.h>
#include "sdutil.h"
#include "sdstring.h"


#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define ATTRI_UNIXSOCKET		"UnixSocket"



CDBConfig::CDBConfig()
{
    m_dwSaveDBInterval = 0;
    m_dwPoolCoreSize = 0;
    m_dwPoolMaxSize = 0;
}

CDBConfig::~CDBConfig()
{
}

UINT32 CDBConfig::GetIntervalSaveTime()
{
	return m_dwSaveDBInterval;
}

BOOL CDBConfig::Init(const TCHAR* pConfigFile)
{	
    CSDXMLFile oSDXMLFile;
    if (FALSE == oSDXMLFile.Load(_SDTT2A(pConfigFile)))
    {
        return FALSE;
    }
    CSDXMLNode rootNode = oSDXMLFile.GetRootNode();
	m_dwSaveDBInterval = rootNode["Setting"]["Interval"]("time").AsInteger();

	
    CSDXMLNode oSettingNode = rootNode["Setting/Item"];

    SDStrncpy(m_oDBAccount.m_szHostName, oSettingNode(ATTRI_REMOTE_IP).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    m_oDBAccount.m_wConnPort = SDAtoi(oSettingNode(ATTRI_REMOTE_PORT).AsString("-1").c_str());
    SDStrncpy(m_oDBAccount.m_szLoginName, oSettingNode(ATTRI_USERNAME).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szLoginPwd, oSettingNode(ATTRI_PWD).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szDBName, oSettingNode(ATTRI_DBNAME0).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
    SDStrncpy(m_oDBAccount.m_szCharactSet, oSettingNode(ATTRI_CHARACTERSET).AsString("-1").c_str(), SDDB_MAX_CHARACTSET_SIZE);
	SDStrncpy(m_oDBAccount.m_szUnixSocket, oSettingNode(ATTRI_UNIXSOCKET).AsString("").c_str(), SDDB_MAX_NAME_SIZE);
    m_oDBAccount.m_wDBType = 0;

    CSDXMLNode node = rootNode["Setting/Item"];
    m_dwPoolCoreSize = node(ELEM_POOLCORESIZE).AsInteger();
    m_dwPoolMaxSize = node(ELEM_POOLMAXSIZE).AsInteger();
	return TRUE;
}

VOID CDBConfig::UnInit()
{
}

BOOL CDBConfig::GetDBAccount(SGDP::SDDBAccount& DBAccount)
{
	SDStrncpy(DBAccount.m_szHostName,m_oDBAccount.m_szHostName, SDDB_MAX_NAME_SIZE);
	DBAccount.m_wConnPort = m_oDBAccount.m_wConnPort;
	SDStrncpy(DBAccount.m_szLoginName, m_oDBAccount.m_szLoginName, SDDB_MAX_NAME_SIZE);
	SDStrncpy(DBAccount.m_szLoginPwd,m_oDBAccount.m_szLoginPwd, SDDB_MAX_NAME_SIZE);
	SDStrncpy(DBAccount.m_szDBName,m_oDBAccount.m_szDBName, SDDB_MAX_NAME_SIZE);
	SDStrncpy(DBAccount.m_szCharactSet, m_oDBAccount.m_szCharactSet, SDDB_MAX_CHARACTSET_SIZE);
	SDStrncpy(DBAccount.m_szUnixSocket, m_oDBAccount.m_szUnixSocket, SDDB_MAX_NAME_SIZE);
	DBAccount.m_wDBType = 0;
	return TRUE;
}


UINT32 CDBConfig::GetPoolCoreSize()
{	
	return m_dwPoolCoreSize;
}

UINT32 CDBConfig::GetPoolMaxSize()
{
	return m_dwPoolMaxSize;
}

