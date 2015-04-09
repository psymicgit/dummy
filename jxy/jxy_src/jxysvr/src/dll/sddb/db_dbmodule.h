
#ifndef SGDP_DB_DBMODULE_H_20070613
#define SGDP_DB_DBMODULE_H_20070613

#include <set>
#include "sddb.h"
#include "sdsingleton.h"
#include "sdmutex.h"

#define SYNC_SN_MARK 0x8000
#define CONV_SYNC_SN(nDBSn) (0x8000 | nDBSn)

//
// 类名称   : CSDDBModule
// 功能     : <继承并实现了ISDDBModule类的接口，提供获取连接和连接池的相关操作>
//
class CSDDBModule : public SGDP::ISDDBModule
{
	CSDDBModule();
	virtual ~CSDDBModule();

	DECLARE_SINGLETON(CSDDBModule)
public:
	
	// Interface ISDBase
	void SDAPI AddRef();
	UINT32 SDAPI QueryRef();
	void SDAPI Release();
	SGDP::SSGDPVersion SDAPI GetVersion();
	const CHAR * SDAPI GetModuleName();

	// Interface ISDDBModule
	SGDP::ISDDBSession * SDAPI GetDBSession(const CHAR * pszConfigString);
	SGDP::ISDDBSession * SDAPI GetDBSession(SGDP::SDDBAccount *pstDBAccount);
	SGDP::ISDDBSession * SDAPI GetDBSession(SGDP::SDDBAccount *pstDBAccount, UINT32 coreSize, UINT32 maxSize);
	void SDAPI Close(SGDP::ISDDBSession * pDBSession);

	bool Init();
private:
	void Uninit();
	void ParseConfigString(const CHAR * pszConfigString, SGDP::SDDBAccount &stDBAccount, UINT32 &coreSize, UINT32 &maxSize);

private:
	typedef std::set<SGDP::ISDDBSession *> DBSessions;
	 volatile INT32 m_dwRef;
	 SGDP::CSDMutex m_Lock;
	 DBSessions m_oSessions;
	 
};

#endif

