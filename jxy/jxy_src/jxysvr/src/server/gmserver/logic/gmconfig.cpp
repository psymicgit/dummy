
#include "gmconfig.h"
#include <sdutil.h>

#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define ATTRI_UNIXSOCKET		"UnixSocket"


CGMConfig::CGMConfig()
{
	m_dwKeepAlive = 0;
    memset(&m_stExecInfoAreaList, 0x00, sizeof(m_stExecInfoAreaList));
}

CGMConfig::~CGMConfig()
{
    UnInit();
}

BOOL CGMConfig::Init(const TCHAR* pszFilename)
{
    CSDXMLFile oSDXMLFile;
    if (FALSE == oSDXMLFile.Load(_SDTT2A(pszFilename)))
    {
        return FALSE;
    }

    if (!_LoadExecInfo(&oSDXMLFile))
    {
        SYS_CRITICAL(_SDT("[%s:%d]--Load exec info failed."), MSG_MARK);
        return FALSE;
    }

	if (!_LoadUserInfo(&oSDXMLFile))
	{
		SYS_CRITICAL(_SDT("[%s:%d]--Load user info failed."), MSG_MARK);
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
	SDStrncpy(m_oDBAccount.m_szUnixSocket, oSettingNode(ATTRI_UNIXSOCKET).AsString("").c_str(), SDDB_MAX_NAME_SIZE);
	m_oDBAccount.m_wDBType = 0;
	m_dwPoolCoreSize = oSettingNode(ELEM_POOLCORESIZE).AsInteger();
	m_dwPoolMaxSize = oSettingNode(ELEM_POOLMAXSIZE).AsInteger();

	m_dwKeepAlive = rootNode["KeepAlive"]("value").AsInteger();

	if (0 == m_dwKeepAlive)
	{
		m_dwKeepAlive = 5;
	}
    return TRUE;
}


VOID  CGMConfig::UnInit()
{

}

BOOL	CGMConfig::_LoadExecInfo(CSDXMLFile* poSDXMLFile)
{
    CSDXMLNode rootNode = poSDXMLFile->GetRootNode();
    CSDXMLNode areaNode = rootNode["area"];
    while (areaNode != NULL)
    {
        SExecInfoArea	 &	stExecInfoArea = m_stExecInfoAreaList.stExecInfoArea[m_stExecInfoAreaList.byAreaNum];
        stExecInfoArea.dwAreaID = areaNode("AreaID").AsInteger();

		BOOL				bRead	=	FALSE;
        CSDXMLNode		execNode =	areaNode["exec"];
        while (execNode != NULL)
        {
            SExecInfo	 & stExecInfo		= stExecInfoArea.stExecInfo[stExecInfoArea.byExecNum];
            SDStrcpy(stExecInfo.szPath, execNode("path").AsString().c_str());
            if (0 == SDStrlen(stExecInfo.szPath))
            {
                SDStrcpy(stExecInfo.szPath, SDGetModulePath());
            }

            SDStrcpy(stExecInfo.szName, execNode("name").AsString().c_str());
            if (0 == SDStrlen(stExecInfo.szName))
            {
                return FALSE;
            }
			bRead = TRUE;
            execNode = execNode.Sibling();
            stExecInfoArea.byExecNum++;
        }
		if (bRead)
		{ 
			m_stExecInfoAreaList.byAreaNum++;
		}
        areaNode = areaNode.Sibling();
    }
    return TRUE;
}


BOOL	CGMConfig::_LoadUserInfo(CSDXMLFile* poSDXMLFile)
{
	CSDXMLNode rootNode = poSDXMLFile->GetRootNode();
	CSDXMLNode useNode = rootNode["user"];
	while (useNode != NULL)
	{
		SUserInfo	 &	stUserInfo = m_stUserInfoList.stUserInfo[m_stUserInfoList.byNum];
		SDStrcpy(stUserInfo.szUserName, useNode("username").AsString().c_str());
		SDStrcpy(stUserInfo.szUserPwd, useNode("userpwd").AsString().c_str());
		useNode = useNode.Sibling();
		m_stUserInfoList.byNum++;
	}
	return TRUE;
}

char	*CGMConfig::GetUserPwd(char *szUserName)
{
	for (UINT8 byNum = 0; byNum < m_stUserInfoList.byNum; byNum++)
	{
		SUserInfo	 &	stUserInfo = m_stUserInfoList.stUserInfo[byNum];
		if (0 == strcmp(stUserInfo.szUserName, szUserName))
		{
			return stUserInfo.szUserPwd;
		}
	}
	return NULL;
}

UINT32 CGMConfig::GetPoolCoreSize()
{	
	return m_dwPoolCoreSize;
}

UINT32 CGMConfig::GetPoolMaxSize()
{
	return m_dwPoolMaxSize;
}

BOOL CGMConfig::GetDBAccount(SGDP::SDDBAccount& DBAccount)
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
