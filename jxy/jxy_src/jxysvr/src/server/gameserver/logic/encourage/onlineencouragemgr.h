
#ifndef _ONLINEENCOURAGEMGR_H_
#define _ONLINEENCOURAGEMGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <sddb.h>
#include <logic/instance/instance.h>
#include <logic/base/burstprop.h>
#include <logic/base/basepropmgr.h>

#include "onlineencouragerecord.h"

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>

using namespace std;
using namespace SGDP;

//在线奖励，只做前3天（可配），每次领取都有个CD时间
struct SOnlineEncourageProp
{
	SOnlineEncourageProp() { wRecvCD = 0; memset(&stDT_ONLINE_ENCOURAGE_DATA, 0, sizeof(stDT_ONLINE_ENCOURAGE_DATA)); }
		
	DT_ONLINE_ENCOURAGE_DATA stDT_ONLINE_ENCOURAGE_DATA;//奖励
	UINT16 wRecvCD;//本次奖励领取CD(单位秒)
};

typedef vector <SOnlineEncourageProp> COnlineEncouragePropVec;
typedef COnlineEncouragePropVec::iterator COnlineEncouragePropVecItr;

class COnlineEncourageMgr : public CBasePopMgr
{
public:

	DECLARE_SINGLETON_PROPMGR(COnlineEncourageMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_ONLINE_ENC ; }
	BOOL Init();
	VOID UnInit();

public:	
	SOnlineEncourageProp* GetEncourage(UINT8 byIdx);
	UINT8 GetMaxPerDay() { return m_byMaxPerDay; }

public:
	COnlineEncourageMgr();
	virtual ~COnlineEncourageMgr();

protected:
	BOOL LoadFromDB();
	BOOL ParseItem(UINT8& byItemNum, DT_ITEM_DATA_CLI astItemInfoList[MAX_ENCOURAGE_ITEM_KIND_NUM], const CHAR* szInput);

//public:
//    COnlineEncourageRecord* CreateOnlineEncourageRecord();
//    VOID  RemoveOnlineEncourageRecord(COnlineEncourageRecord*);
//
//private:
//    CSDObjectPool<COnlineEncourageRecord, CSDMutex>  m_oOnlineEncourageRecordPool;

private:
	COnlineEncouragePropVec	m_vecOnlineEncourageProp;//在线奖励，数组下标
	UINT8					m_byMaxPerDay;//每天最大领取次数, 目前每天是一样的
};

#endif 
