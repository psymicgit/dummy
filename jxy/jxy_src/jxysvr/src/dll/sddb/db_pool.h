
#ifndef SGDP_DB_POOL_H_20100729
#define SGDP_DB_POOL_H_20100729

#include "sddb.h"
#include "db_conn.h"
#include <map>
#include <set>
#include <list>
#include "sdthreadpool.h"
#include "sdmutex.h"

class CConnPool : public SGDP::ISDRunable
{
public:
	CConnPool();
	~CConnPool();

	/**
	 * @desc 初始化数据库连接池
	 *
	 * @param pstDBAccount 连接的数据库服务器的参数
	 * @param coreSize 数据库连接池的核心连接数，数据库连接池至少拥有coreSize个连接
	 * @param maxSize 数据库连接池能够创建的最大连接数
	 *
	 * @return true为数据库连接池初始化成功，false为数据库连接池初始化不成功
	 **/
	bool Init(SGDP::SDDBAccount *pstDBAccount, INT32 coreSize, INT32 maxSize);

	/**
	 * @desc 获取连接
	 *
	 * @return 创建的连接，如果返回值为NULL，代表没有可用的连接
	 **/
	SGDP::ISDDBConnection * SDAPI GetConnection(INT32 timeout = 0);

	bool SDAPI SelectDB(const CHAR *pcDBName);

	void Return(SGDP::ISDDBConnection * pConnection);

	/**
	 * @desc 添加一个DB命令，异步执行DB命令
	 *
	 * @param poDBCommand 需要执行的DB命令
	 * 
	 * @return true添加命令成功，false为添加命令失败
	 **/
	bool SDAPI AddDBCommand(SGDP::ISDDBCommand *poDBCommand);

	/**
	 * @desc 快速添加一个DB命令，异步执行DB命令
	 *
	 * @param poDBCommand 需要执行的DB命令
	 *
	 * @return true添加命令成功，false为添加命令失败
	 **/
    bool SDAPI QuickAddDBCommand(SGDP::ISDDBCommand *poDBCommand);

	/**
	 * @desc 运行DB命令的逻辑部分
	 * 
	 * @param nCount 需要执行DB命令的数量
	 *
	 * @return 运行的结果，true代表运行完所有要求完成的DB命令，
	 * false代表没有完成
	 **/
	bool SDAPI Run(UINT32& dwCount);

	/**
	 * @desc 获取可执行db操作的DB命令的数量
	 * 
	 * @return 当前可执行db操作的DB命令的数量
	 **/
	UINT32 SDAPI GetExcuteCmdCount();

	/**
	 * @desc 获取已执行db操作的DB命令的数量
	 * 
	 * @return 当前可运行的DB命令的数量
	 **/
	UINT32 SDAPI GetExcutedCmdCount();

	/**
	 * @desc 获取连接的数量
	 *
	 * @return 获取连接的数量
	 **/
	INT32 SDAPI GetConnCount();

	/**
	 * @desc 获取可用连接数量
	 *
	 * @return 获取可用连接数量
	 **/
	INT32 SDAPI GetRemainConnCount();

	//线程函数
	void Run();

	typedef std::map<int, IConn *> ConnMap;
	typedef std::set<IConn *> ConnSet;
	typedef std::list<SGDP::ISDDBCommand *> CDBCommandList;

	void SDAPI SetCheckConn(bool bCheckConn);

private:
	//借取一个包装过的连接
	IConn * _Borrow();

	IConn * _Borrow(IConn * pConn);

	//返回一个包装过的连接
	void _Return(IConn * pConn);

	//创建一个连接
	IConn * _CreateConnection();

	//获取一个用于异步执行命令的连接，如果返回值为NULL，代表暂无可用的连接
	IConn * _GetAsynConnection(int nGroupId);

	//内部获取一个连接
	IConn * _GetConnection(INT32 timeout = 0);

	//清除队列中所有的DBCommand
	void _ClearCommandQueue();

	//清除特殊的连接Map中所有的连接
	void _ClearConnMap();

	//删除在ConnMap中的一个连接
	void _RemoveConnMap(IConn * pConn);

	//执行DBCommand
	void _ExecuteDBCommand(IConn * pConn, SGDP::ISDDBCommand * poDBCommand);

private:
	SGDP::CSDMutex m_PoolLock;
	SGDP::CSDMutex m_CommandLock;
	SGDP::CSDMutex m_ExcutedCommandLock;
	SGDP::CSDMutex m_MapLock;
	ConnMap m_ConnMap;
	ConnSet m_ConnSet;
	CDBCommandList m_DBCommandList;
	CDBCommandList m_ExcutedDBCommandList;

	//SGDP::ISDThreadCtrl * m_poThreadCtrl;
	SGDP::CSDThreadPool * m_poThreadPool;

	INT32 m_nCoreSize;
	INT32 m_nMaxSize;

	CHAR m_szServerAddr[SDDB_MAX_NAME_SIZE];
	CHAR m_szDBName[SDDB_MAX_NAME_SIZE];
	CHAR m_szLoginName[SDDB_MAX_NAME_SIZE];
	CHAR m_szPassword[SDDB_MAX_NAME_SIZE];
	CHAR m_szCharactSet[SDDB_MAX_CHARACTSET_SIZE];
	CHAR m_szUnixSocket[SDDB_MAX_NAME_SIZE];

	INT32 m_nServerPort;
	INT32 m_wDBType;

	volatile bool m_bCheckConn;

	volatile INT32 m_nConnCount;
	volatile INT32 m_nRemainConnCount;
	volatile bool m_bRun;

	const static INT32 SLEEP_INTERVAL;
};

#endif

