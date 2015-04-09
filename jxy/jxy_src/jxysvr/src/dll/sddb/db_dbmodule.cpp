#include "sddbconfig.h"
#include "db_dbmodule.h"
#include "db_pool.h"
#include "sdatomic.h"
#include "db_connfactory.h"
#include "db_session.h"
#include "sdstring.h"

IMPLEMENT_SINGLETON(CSDDBModule)

SGDP::ISDDBModule * SDAPI SGDP::SDDBGetModule()
{
	if(false == CSDDBModule::CreateInstance())
	{
		return NULL;
	}
	CSDDBModule::Instance()->AddRef();

	if(false == CSDDBModule::Instance()->Init())
	{
		CRITICAL(_SDT("SDDBGetModule, Init CSDDBModule failed"));
		CSDDBModule::DestroyInstance();
		return NULL;
	}

	return CSDDBModule::Instance();
}

CSDDBModule::CSDDBModule()
{
	SGDP::SDAtomicSet32(&m_dwRef, 0);
}

CSDDBModule::~CSDDBModule()
{
}

void CSDDBModule::AddRef()
{
	SGDP::SDAtomicInc32(&m_dwRef);
}

UINT32 CSDDBModule::QueryRef()
{
	return SGDP::SDAtomicRead32(&m_dwRef);
}

void CSDDBModule::Release()
{
	if(SGDP::SDAtomicDec32(&m_dwRef) == 1)
	{
		Uninit();
		DestroyInstance();
	}
}

void CSDDBModule::Close(SGDP::ISDDBSession * pDBSession)
{
	if(NULL != pDBSession)
	{
		m_Lock.Lock();
		DBSessions::iterator i = m_oSessions.find(pDBSession);
		if(i != m_oSessions.end())
		{
			m_oSessions.erase(i);
			delete pDBSession;
		}
		m_Lock.Unlock();
	}
}

SGDP::SSGDPVersion CSDDBModule::GetVersion()
{
	return SGDP::SDDB_VERSION;
}

const CHAR * CSDDBModule::GetModuleName()
{
	return SGDP::SDDB_MODULENAME;
}

SGDP::ISDDBSession * SDAPI CSDDBModule::GetDBSession(const CHAR * pszConfigString)
{
	SGDP::SDDBAccount stDBAccount;
	UINT32 coreSize = 1;
	UINT32 maxSize = 1;

	ParseConfigString(pszConfigString, stDBAccount, coreSize, maxSize);

	CConnPool * pPool = SDNew CConnPool();
	if(pPool->Init(&stDBAccount, coreSize, maxSize))
	{
		CDBSession * pDBSession = SDNew CDBSession(pPool);
		m_Lock.Lock();
		m_oSessions.insert(pDBSession);
		m_Lock.Unlock();
		return pDBSession;
	}

	return NULL;
}

SGDP::ISDDBSession * CSDDBModule::GetDBSession(SGDP::SDDBAccount *pstDBAccount)
{
	CConnPool * pPool = SDNew CConnPool();
	if(pPool->Init(pstDBAccount, 1, 1))
	{
		CDBSession * pDBSession = SDNew CDBSession(pPool);
		this->m_Lock.Lock();
		this->m_oSessions.insert(pDBSession);
		this->m_Lock.Unlock();
		return pDBSession;
	}

	return NULL;
}

SGDP::ISDDBSession * CSDDBModule::GetDBSession(SGDP::SDDBAccount *pstDBAccount, UINT32 coreSize, UINT32 maxSize)
{
	CConnPool * pPool = SDNew CConnPool();
	if(pPool->Init(pstDBAccount, coreSize, maxSize))
	{
		CDBSession * pDBSession = SDNew CDBSession(pPool);
		this->m_Lock.Lock();
		this->m_oSessions.insert(pDBSession);
		this->m_Lock.Unlock();
		return pDBSession;
	}
	else
	{
		return NULL;
	}
}

bool CSDDBModule::Init()
{
	return true;
}

void CSDDBModule::Uninit()
{
	this->m_Lock.Lock();
	for (DBSessions::iterator itr = m_oSessions.begin(); itr != m_oSessions.end(); ++itr)
	{
		delete (*itr); 
	}
	m_oSessions.clear(); 
	this->m_Lock.Unlock();
}

void CSDDBModule::ParseConfigString(const CHAR * pszConfigString, 
									SGDP::SDDBAccount &stDBAccount, UINT32 &coreSize, UINT32 &maxSize)
{
	std::map<std::string, std::string> ConfigMap;
	CHAR * keys[] = {
		"HostName=",
		"DBName=",
		"LoginName=",
		"LoginPwd=",
		"CharacterSet=",
		"Port=",
		"DBType=",
		"CoreSize=",
		"MaxSize="
	};
	std::string ConfigString = pszConfigString;
	for(int i=0; i<sizeof(keys)/4; ++i)
	{
		std::string::size_type key_end_pos = ConfigString.find(keys[i]);
		if(std::string::npos == key_end_pos)
		{
			continue;
		}
		std::string::size_type value_end_pos = ConfigString.find(";", key_end_pos + SGDP::SDStrlen(keys[i]));
		if(std::string::npos == value_end_pos)
		{
			continue;
		}
		std::string key = ConfigString.substr(key_end_pos, SGDP::SDStrlen(keys[i]) - 1);
		std::string value = ConfigString.substr(key_end_pos + SGDP::SDStrlen(keys[i]), value_end_pos - key_end_pos - SGDP::SDStrlen(keys[i]));
		if(0 == value.length())
		{
			continue;
		}
		ConfigMap.insert(std::make_pair(key, value));
	}
	SGDP::SDStrncpy(stDBAccount.m_szHostName, ConfigMap["HostName"].c_str(), SDDB_MAX_NAME_SIZE);
	SGDP::SDStrncpy(stDBAccount.m_szLoginName, ConfigMap["LoginName"].c_str(), SDDB_MAX_NAME_SIZE);
	SGDP::SDStrncpy(stDBAccount.m_szLoginPwd, ConfigMap["LoginPwd"].c_str(), SDDB_MAX_NAME_SIZE);
	SGDP::SDStrncpy(stDBAccount.m_szDBName, ConfigMap["DBName"].c_str(), SDDB_MAX_NAME_SIZE);
	SGDP::SDStrncpy(stDBAccount.m_szCharactSet, ConfigMap["CharacterSet"].c_str(), SDDB_MAX_CHARACTSET_SIZE);
	
	stDBAccount.m_wConnPort = atoi(ConfigMap["Port"].c_str());
	stDBAccount.m_wDBType = atoi(ConfigMap["DBType"].c_str());
	//如果没有设置端口号,设置端口号的缺省值
	if(SGDP::SDDB_DBTYPE_MYSQL == stDBAccount.m_wDBType &&
		0 == stDBAccount.m_wConnPort)
	{
		stDBAccount.m_wConnPort = 3306;
	}
	else if(SGDP::SDDB_DBTYPE_ADO == stDBAccount.m_wDBType &&
		0 == stDBAccount.m_wConnPort)
	{
		stDBAccount.m_wConnPort = 1433;
	}

	coreSize = atoi(ConfigMap["CoreSize"].c_str());
	//如果没有设置CoreSize,设置CoreSize的缺省值
	if(coreSize == 0)
	{
		coreSize = 1;
	}
	maxSize = atoi(ConfigMap["MaxSize"].c_str());
	//如果没有设置MaxSize,设置MaxSize的缺省值
	if(maxSize == 0)
	{
		maxSize = 1;
	}
	ConfigMap.clear();
}

