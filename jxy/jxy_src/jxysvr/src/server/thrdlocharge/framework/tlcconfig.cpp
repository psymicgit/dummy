#include "tlcconfig.h"
#include <sdutil.h>
#include <sdstring.h>
#include <set>

#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define ATTRI_UNIXSOCKET		"UnixSocket"
#define ELEM_TIMEOUT			"timeout"
#define ATTR_OPEN 				"open"
#define ATTR_NAME				"name"
#define ATTR_DLL				"dll"
#define ATTR_GET_MODULE_FUNC			"getmodulefunc"
#define DEFAULT_IMPORT_AUTH_FUNC		"GetAuthModule"
#define DEFAULT_IMPORT_RECHARGE_FUNC	"GetRechargeModule"


IMPLEMENT_SINGLETON(CTLCConfig)

CTLCConfig::CTLCConfig()
{

}

CTLCConfig::~CTLCConfig()
{
 
}

BOOL CTLCConfig::Init(const TCHAR* pszFilename)
{
	if(!_LoadCfg(pszFilename))
	{
		USR_INFO(_SDT("[%s: %d]:_LoadCfg failed!"), MSG_MARK);
		return FALSE;
	}

	return TRUE;
}

BOOL CTLCConfig::_LoadCfg(const TCHAR* pszConfig)
{
	CSDXMLFile oSDXMLFile;
	if (FALSE == oSDXMLFile.Load(_SDTT2A(pszConfig)))
	{
		USR_INFO(_SDT("[%s: %d]:Load cfg[%s] failed!"), MSG_MARK);
		return FALSE;
	}
	CSDXMLNode rootNode = oSDXMLFile.GetRootNode();
	m_dwTimeOut = rootNode[ELEM_TIMEOUT].AsInteger();
		
	if(!_LoadAuthModule(rootNode))
	{
		USR_INFO(_SDT("[%s: %d]:_LoadAuthModule failed!"), MSG_MARK);
		return FALSE;
	}
	
	if(!_LoadRechargeModule(rootNode))
	{
		USR_INFO(_SDT("[%s: %d]:_LoadRechargeModule failed!"), MSG_MARK);
		return FALSE;
	}

	if(!_LoadCliListen(rootNode))
	{
		USR_INFO(_SDT("[%s: %d]:_LoadCliListen failed!"), MSG_MARK);
		return FALSE;
	}

	if(!_LoadHttpListen(rootNode))
	{
		USR_INFO(_SDT("[%s: %d]:_LoadHttpListen failed!"), MSG_MARK);
		return FALSE;
	}

	if(!_LoadDbInfo(rootNode))
	{
		USR_INFO(_SDT("[%s: %d]:_LoadDbInfo failed!"), MSG_MARK);
		return FALSE;
	}

	return TRUE;
}


BOOL CTLCConfig::_LoadAuthModule(CSDXMLNode rootNode)
{
	CSDXMLNode authModelNode = rootNode["AuthModules/AuthModule"];
	set<string> setExistName;
	while (authModelNode!= NULL)
	{
		string strIsOpen = authModelNode(ATTR_OPEN).AsString();
		if (!(strcmp(strIsOpen.c_str(),"y") == 0 || strcmp(strIsOpen.c_str(),"Y") == 0))
		{
			authModelNode = authModelNode.Sibling();
			continue;
		}
		SModuleConfig sTmp;
		string strTemp = authModelNode(ATTR_NAME).AsString();
		set<string>::iterator itSetExistName = setExistName.find(strTemp);
		if (itSetExistName != setExistName.end())
		{            
			return FALSE;
		}
		setExistName.insert(strTemp);
		sTmp.strName = strTemp;
		sTmp.strDll = authModelNode(ATTR_DLL).AsString();
		sTmp.strGetModuleFunc = authModelNode(ATTR_GET_MODULE_FUNC).AsString(DEFAULT_IMPORT_AUTH_FUNC);
		sTmp.strConfFile = authModelNode("file").AsString();

		m_vecAuthModuleConfigs.push_back(sTmp);
		authModelNode = authModelNode.Sibling();
	}

	return TRUE;
}

BOOL CTLCConfig::_LoadRechargeModule(CSDXMLNode rootNode)
{
	CSDXMLNode rechargeModelNode = rootNode["RechargeModules/RechargeModule"];
	set<string> setExistName;
	while (rechargeModelNode!= NULL)
	{
		string strIsOpen = rechargeModelNode(ATTR_OPEN).AsString();
		if (!(strcmp(strIsOpen.c_str(),"y") == 0 || strcmp(strIsOpen.c_str(),"Y") == 0))
		{
			rechargeModelNode = rechargeModelNode.Sibling();
			continue;
		}
		SModuleConfig sTmp;
		string strTemp = rechargeModelNode(ATTR_NAME).AsString();
		set<string>::iterator itSetExistName = setExistName.find(strTemp);
		if (itSetExistName != setExistName.end())
		{            
			return FALSE;
		}
		setExistName.insert(strTemp);
		sTmp.strName = strTemp;
		sTmp.strDll = rechargeModelNode(ATTR_DLL).AsString();
		sTmp.strGetModuleFunc = rechargeModelNode(ATTR_GET_MODULE_FUNC).AsString(DEFAULT_IMPORT_RECHARGE_FUNC);
		sTmp.strConfFile = rechargeModelNode("file").AsString();

		m_vecRechargeModuleConfigs.push_back(sTmp);
		rechargeModelNode = rechargeModelNode.Sibling();
	}

	return TRUE;
}

BOOL CTLCConfig::_LoadCliListen(CSDXMLNode rootNode)
{
	CSDXMLNode gmListenNode	= rootNode["CliListen"];
	if (gmListenNode != NULL)
	{
		SDStrcpy(m_stCliListen.m_szIP, gmListenNode(ATTRI_LOCAL_IP).AsString().c_str());
		m_stCliListen.m_wPort		= gmListenNode(ATTRI_LOCAL_PORT).AsInteger();
		m_stCliListen.m_dwRecvBuf	= gmListenNode(ATRRI_RECVBUFF).AsInteger();
		m_stCliListen.m_dwSendBuf	= gmListenNode(ATRRI_SENDBUFF).AsInteger();
	}
	return TRUE;
}	

BOOL CTLCConfig::_LoadHttpListen(CSDXMLNode rootNode)
{
	CSDXMLNode gmListenNode	= rootNode["HttpListen"];
	if (gmListenNode != NULL)
	{
		SDStrcpy(m_stCliListen.m_szIP, gmListenNode(ATTRI_LOCAL_IP).AsString().c_str());
		m_stHttpListen.m_wPort		= gmListenNode(ATTRI_LOCAL_PORT).AsInteger();
		m_stHttpListen.m_dwRecvBuf	= gmListenNode(ATRRI_RECVBUFF).AsInteger();
		m_stHttpListen.m_dwSendBuf	= gmListenNode(ATRRI_SENDBUFF).AsInteger();
	}
	return TRUE;
}

BOOL CTLCConfig::_LoadDbInfo(CSDXMLNode rootNode)
{
	CSDXMLNode oSettingNode=rootNode["DBMS"];
	if( oSettingNode != NULL)
	{
		SDStrncpy(m_stDBAccount.m_szHostName, oSettingNode(ATTRI_REMOTE_IP).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
		m_stDBAccount.m_wConnPort = SDAtoi(oSettingNode(ATTRI_REMOTE_PORT).AsString("-1").c_str());
		SDStrncpy(m_stDBAccount.m_szLoginName, oSettingNode(ATTRI_USERNAME).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
		SDStrncpy(m_stDBAccount.m_szLoginPwd, oSettingNode(ATTRI_PWD).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
		SDStrncpy(m_stDBAccount.m_szDBName, oSettingNode(ATTRI_DBNAME0).AsString("-1").c_str(), SDDB_MAX_NAME_SIZE);
		SDStrncpy(m_stDBAccount.m_szCharactSet, oSettingNode(ATTRI_CHARACTERSET).AsString("-1").c_str(), SDDB_MAX_CHARACTSET_SIZE);
		SDStrncpy(m_stDBAccount.m_szUnixSocket, oSettingNode(ATTRI_UNIXSOCKET).AsString("").c_str(), SDDB_MAX_NAME_SIZE);
		m_stDBAccount.m_wDBType = 0;
		m_dwPoolCoreSize = oSettingNode(ELEM_POOLCORESIZE).AsInteger();
		m_dwPoolMaxSize = oSettingNode(ELEM_POOLMAXSIZE).AsInteger();

		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
