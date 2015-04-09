#include "sddbconfig.h"
#include "sdstring.h"
#include "db_pool.h"
#include "db_connfactory.h"
#include "sdatomic.h"
#include "sdtime.h"

using namespace SGDP;
const INT32 CConnPool::SLEEP_INTERVAL = 1;

CConnPool::CConnPool() :
m_nServerPort(0),
m_nCoreSize(0),
m_nMaxSize(0),
m_nRemainConnCount(0),
m_bRun(false),
m_poThreadPool(NULL),
m_bCheckConn(true)
{
#if (defined(WIN32) || defined(WIN64))
	SGDP::SDZeroString(m_szServerAddr);
	SGDP::SDZeroString(m_szDBName);
	SGDP::SDZeroString(m_szLoginName);
	SGDP::SDZeroString(m_szPassword);
	SGDP::SDZeroString(m_szUnixSocket);
#else
	memset( m_szServerAddr, 0, sizeof(m_szServerAddr) );
	memset( m_szDBName, 0, sizeof(m_szDBName) );
	memset( m_szLoginName, 0, sizeof(m_szLoginName) );
	memset( m_szPassword, 0, sizeof(m_szPassword) );
	memset( m_szUnixSocket, 0, sizeof(m_szUnixSocket) );
#endif		

	SGDP::SDStrncpy(m_szCharactSet, "latin1", sizeof(m_szCharactSet));
	SGDP::SDAtomicSet32(&m_nConnCount, 0);
}


CConnPool::~CConnPool()
{
	this->m_bRun = false;
	if(NULL != m_poThreadPool)
	{
		m_poThreadPool->TerminateWaitJobs();
		delete m_poThreadPool;
		m_poThreadPool = NULL;
	}

	//退出前确保所有的请求都已执行完
	this->_ClearCommandQueue();

	m_PoolLock.Lock();
	for (ConnSet::iterator itr = m_ConnSet.begin(); itr != m_ConnSet.end(); ++itr)
	{
		IConn * pConn = *itr;
		_RemoveConnMap(pConn);
		pConn->Release();
		delete pConn;
	}
	m_ConnSet.clear();
	this->_ClearConnMap();
	m_PoolLock.Unlock();
}

void CConnPool::Run()
{
	SGDP::ISDDBCommand * poDBCommand = NULL;

	while(m_bRun)
	{
		m_CommandLock.Lock();
		if( !m_DBCommandList.empty() )
		{
			poDBCommand = m_DBCommandList.front();
			m_DBCommandList.pop_front();
		}
		m_CommandLock.Unlock();
		
		if(NULL == poDBCommand)
		{
			SGDP::SDSleep(10);
			continue;
		}

		while(m_bRun)
		{
			IConn * pConn = this->_GetAsynConnection(poDBCommand->GetGroupId());
			if(NULL == pConn)
			{
				SGDP::SDSleep(10);
				continue;
			}
			if(m_bCheckConn && !pConn->CheckConnection())
			{
				this->_Return(pConn);
				continue;
			}
			//执行DBCommand的处理
			this->_ExecuteDBCommand(pConn, poDBCommand);
			this->_Return(pConn);
			break;
		}
		poDBCommand = NULL;
	}
}

void CConnPool::_ExecuteDBCommand(IConn * pConn, SGDP::ISDDBCommand * poDBCommand)
{
	poDBCommand->OnExecuteSql(pConn);

	m_ExcutedCommandLock.Lock();
	this->m_ExcutedDBCommandList.push_back(poDBCommand);
	m_ExcutedCommandLock.Unlock();
}

bool CConnPool::Init(SGDP::SDDBAccount *pstDBAccount, INT32 nCoreSize, INT32 nMaxSize)
{
	if(m_bRun)
	{
		return true;
	}
	if(NULL == pstDBAccount)
	{
		return false;
	}
	if(nCoreSize <= 0 || nCoreSize > nMaxSize)
	{
		return false;
	}

	SGDP::SDStrncpy(this->m_szServerAddr, pstDBAccount->m_szHostName, sizeof(this->m_szServerAddr));
	SGDP::SDStrncpy(this->m_szDBName, pstDBAccount->m_szDBName, sizeof(this->m_szDBName));
	SGDP::SDStrncpy(this->m_szLoginName, pstDBAccount->m_szLoginName, sizeof(this->m_szLoginName));
	SGDP::SDStrncpy(this->m_szPassword, pstDBAccount->m_szLoginPwd, sizeof(this->m_szPassword));
	SGDP::SDStrncpy(this->m_szUnixSocket, pstDBAccount->m_szUnixSocket, sizeof(this->m_szUnixSocket));

	if ('\0' != pstDBAccount->m_szCharactSet[0])
	{
		SGDP::SDStrncpy(m_szCharactSet, pstDBAccount->m_szCharactSet, sizeof(m_szCharactSet));
		INFO(_SDT("Set DB connection character set as:%s\n"), _SDTA2T(m_szCharactSet));
	}
	else
	{
		INFO(_SDT("Cann't set DB connection character set as:%s\n"), _SDTA2T(m_szCharactSet));
	}

	m_nServerPort = pstDBAccount->m_wConnPort;
	m_wDBType = pstDBAccount->m_wDBType;
	m_nCoreSize = nCoreSize;
	m_nMaxSize = nMaxSize;

	INT32 i = SGDP::SDAtomicRead32(&m_nConnCount);
	for(; i < m_nCoreSize; i++)
	{
		IConn * pConn = this->_CreateConnection();
		if(NULL == pConn)
		{
			CRITICAL(_SDT("CConnPool::Init, create connection failed, alrealy created %d\n"), i);
			return false;
		}
		pConn->SetUsed(false);
		m_PoolLock.Lock();
		this->m_ConnSet.insert(pConn);
		SGDP::SDAtomicInc32(&m_nConnCount);
		m_nRemainConnCount++;
		m_PoolLock.Unlock();
	}

	
	m_bRun = true;
	if(NULL == this->m_poThreadPool)
	{
		this->m_poThreadPool = SDNew SGDP::CSDThreadPool();
		this->m_poThreadPool->Init(m_nCoreSize, m_nCoreSize, m_nMaxSize * 2);
		for(i = 0; i < m_nCoreSize; i++)
		{
			this->m_poThreadPool->ScheduleJob(this);
		}
	}

	return true;
}

//返回一个连接，如果当前没有连接，停等
IConn * CConnPool::_GetAsynConnection(int nGroupId)
{
	if(-1 == nGroupId)
	{
		return this->_GetConnection(0);
	}
	else
	{
		IConn * pSpecialConn = NULL;

		//执行特殊Group的DBCommand的处理
		m_MapLock.Lock();
		ConnMap::iterator i = m_ConnMap.find(nGroupId);
		if(m_ConnMap.end() == i)
		{
			pSpecialConn = this->_GetConnection(0);
			if(NULL != pSpecialConn)
			{
				m_ConnMap.insert(std::make_pair(nGroupId, pSpecialConn));
			}
		}
		else
		{
			pSpecialConn = i->second;
			pSpecialConn = this->_Borrow(pSpecialConn);
		}
		m_MapLock.Unlock();
		
		return pSpecialConn;
	}
}

SGDP::ISDDBConnection * CConnPool::GetConnection(INT32 timeout)
{
	return this->_GetConnection(timeout);
}

bool CConnPool::SelectDB(const CHAR *pcDBName)
{
	m_PoolLock.Lock();
	for (ConnSet::iterator itr = m_ConnSet.begin(); itr != m_ConnSet.end(); ++itr)
	{
		IConn * pConn = *itr;
		if(!pConn->SelectDB(pcDBName))
		{
			return false;
		}
	}
	m_PoolLock.Unlock();
	return true;
}

IConn * CConnPool::_GetConnection(INT32 timeout)
{
	IConn * pConn = NULL;
	if(0 == timeout)
	{
		//直接返回结果，如果没有连接，返回NULL
		return this->_Borrow();
	}
	else if(0 > timeout)
	{
		//返回一个连接，如果当前没有连接，停等
		while(m_bRun)
		{
			pConn = this->_Borrow();
			if(NULL == pConn)
			{
				SGDP::SDSleep(SLEEP_INTERVAL);
				continue;
			}
			else
			{
				return pConn;
			}
		}
	}
	else
	{
		INT32 oncetime = timeout > SLEEP_INTERVAL ? SLEEP_INTERVAL : timeout;
		INT32 costtime = 0;
		while(m_bRun && costtime < timeout)
		{
			pConn = this->_Borrow();
			if(NULL == pConn)
			{
				SGDP::SDSleep(oncetime);
				costtime += oncetime;
				if(timeout - costtime < SLEEP_INTERVAL)
				{
					oncetime = timeout - costtime;
				}
				continue;
			}
			else
			{
				return pConn;
			}
		}
	}
	
	return pConn;
}

bool CConnPool::AddDBCommand(SGDP::ISDDBCommand *poDBCommand)
{
	if(NULL == poDBCommand)
	{
		return false;
	}
	m_CommandLock.Lock();
	m_DBCommandList.push_back(poDBCommand);
	m_CommandLock.Unlock();
	return true;
}

bool CConnPool::QuickAddDBCommand(SGDP::ISDDBCommand *poDBCommand)
{
	if(NULL == poDBCommand)
	{
		return false;
	}
	m_CommandLock.Lock();
	m_DBCommandList.push_front(poDBCommand);
	m_CommandLock.Unlock();
	return true;
}

bool CConnPool::Run(UINT32& dwCount)
{
	UINT32 dwLoopNum = dwCount;
	dwCount = 0;
	while(dwLoopNum-- > 0) 
	{
		SGDP::ISDDBCommand *poDBCommand = NULL;

		m_ExcutedCommandLock.Lock();
		if( !m_ExcutedDBCommandList.empty() )
		{
			poDBCommand = m_ExcutedDBCommandList.front();
			m_ExcutedDBCommandList.pop_front();
			dwCount++;
		}
		m_ExcutedCommandLock.Unlock();

		if(NULL == poDBCommand)
		{
			return false;
		}

		poDBCommand->OnExecuted();					
		poDBCommand->Release();
	}
	return true;
}

UINT32 CConnPool::GetExcuteCmdCount()
{
	UINT32 dwCount = 0;
	m_CommandLock.Lock();
	dwCount = (UINT32)m_DBCommandList.size();
	m_CommandLock.Unlock();
	return dwCount;
}

UINT32 CConnPool::GetExcutedCmdCount()
{
	UINT32 dwCount = 0;
	m_ExcutedCommandLock.Lock();
	dwCount = (UINT32)m_ExcutedDBCommandList.size();
	m_ExcutedCommandLock.Unlock();
	return dwCount;
}

IConn * CConnPool::_CreateConnection()
{
	IConn * pConn = CConnFactory::GetConnection(this->m_wDBType, this->m_szServerAddr, this->m_nServerPort,
		this->m_szDBName, this->m_szLoginName, this->m_szPassword, this->m_szCharactSet, this->m_szUnixSocket);
	return pConn;
}

void CConnPool::_ClearCommandQueue()
{
	if(m_bRun)
	{
		return;
	}
	SGDP::ISDDBCommand * poDBCommand = NULL;
	SGDP::ISDDBConnection * pConn = NULL;
	bool bFinished = false;
	while(!bFinished)
	{
		pConn = this->_Borrow();
		if(NULL == pConn)
		{
			continue;
		}
		while(!bFinished)
		{
			if(m_bCheckConn && !pConn->CheckConnection())
			{
				pConn->Release();
				pConn = NULL;
				break;
			}
			m_CommandLock.Lock();
			if( !m_DBCommandList.empty() )
			{
				poDBCommand = m_DBCommandList.front();
				m_DBCommandList.pop_front();
			}
			m_CommandLock.Unlock();
			if(poDBCommand == NULL)
			{
				pConn->Release();
				bFinished = true;
				break;
			}

			poDBCommand->OnExecuteSql(pConn);
			this->m_ExcutedDBCommandList.push_back(poDBCommand);
			poDBCommand = NULL;
		}
	}
}

void CConnPool::_ClearConnMap()
{
	m_MapLock.Lock();
	ConnMap::iterator i = m_ConnMap.begin();
	ConnMap::iterator end = m_ConnMap.end();
	for(; i != end; ++i)
	{
		i->second->Release();
	}
	m_ConnMap.clear();
	m_MapLock.Unlock();
}

//删除在ConnMap中的一个连接
void CConnPool::_RemoveConnMap(IConn * pConn)
{
	m_MapLock.Lock();
	ConnMap::iterator i = m_ConnMap.begin();
	ConnMap::iterator end = m_ConnMap.end();
	for(; i != end; ++i)
	{
		if(pConn == i->second)
		{
			int nGroupId = i->first;
			m_ConnMap.erase(nGroupId);
			break;
		}
	}
	m_MapLock.Unlock();
}

void SDAPI CConnPool::SetCheckConn(bool bCheckConn)
{
	m_bCheckConn = bCheckConn;
}

IConn * CConnPool::_Borrow(IConn * pConn)
{
	m_PoolLock.Lock();
	if(pConn->IsUsed())
	{
		m_PoolLock.Unlock();
		return NULL;
	}
	ConnSet::iterator i = m_ConnSet.find(pConn);
	if(i != m_ConnSet.end())
	{
		m_ConnSet.erase(i);
	}
	pConn->SetUsed(true);
	m_PoolLock.Unlock();
	return pConn;
}

//借出连接，这里需要锁
IConn * CConnPool::_Borrow()
{
	IConn * pConn = NULL;
	m_PoolLock.Lock();
	if(m_ConnSet.empty())
	{
		if(SGDP::SDAtomicRead32(&m_nConnCount) >= m_nMaxSize)
		{
			m_PoolLock.Unlock();
			return NULL;
		}
		pConn = this->_CreateConnection();
		if(NULL == pConn)
		{
			m_PoolLock.Unlock();
			CRITICAL(_SDT("CConnPool::_Borrow, create connection failed, alrealy created %d\n"), SGDP::SDAtomicRead32(&m_nConnCount));
			return NULL;
		}
		this->m_ConnSet.insert(pConn);
		SGDP::SDAtomicInc32(&m_nConnCount);
		m_nRemainConnCount++;
	}
	ConnSet::iterator i = m_ConnSet.begin();
	pConn = *i;
	m_ConnSet.erase(i);
	m_nRemainConnCount--;
	pConn->SetUsed(true);
	m_PoolLock.Unlock();
	return pConn;
}

void CConnPool::Return(SGDP::ISDDBConnection * pConn)
{
	this->_Return((IConn *)pConn);
}

//归还连接，这里需要锁
//归还连接时，
//1.如果该连接异常，直接关闭该连接
//2.如果连接数大于核心连接数，直接关闭该连接
void CConnPool::_Return(IConn * pConn)
{
	if(m_bCheckConn && !pConn->CheckConnection())
	{
		_RemoveConnMap(pConn);
		pConn->Release();
		delete pConn;
		SGDP::SDAtomicDec32(&m_nConnCount);
		
		return;
	}
	m_PoolLock.Lock();
	if(SGDP::SDAtomicRead32(&m_nConnCount) > m_nCoreSize)
	{
		m_PoolLock.Unlock();
		_RemoveConnMap(pConn);
		pConn->Release();
		delete pConn;
		SGDP::SDAtomicDec32(&m_nConnCount);
		return;
	}
	m_ConnSet.insert(pConn);
	pConn->SetUsed(false);
	m_nRemainConnCount++;
	m_PoolLock.Unlock();
}

