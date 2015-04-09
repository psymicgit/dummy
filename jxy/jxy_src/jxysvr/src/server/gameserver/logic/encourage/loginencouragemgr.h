
#ifndef _LOGINENCOURAGEMGR_H_
#define _LOGINENCOURAGEMGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <sddb.h>
#include <logic/instance/instance.h>
#include <logic/base/burstprop.h>
#include <logic/base/basepropmgr.h>
#include "loginencouragerecord.h"



#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>

using namespace std;
using namespace SGDP;

//登录奖励，可抽奖次数为连续登录天数(<=3天)，抽奖没有CD
struct SLoginEncourageProp
{
	UINT16	wItemKindID;//道具ID
	UINT16	wPileNum;//叠加数
	UINT8	byRate;//概率
};

class CLoginEncourageMgr : public CBasePopMgr
{
public:

	DECLARE_SINGLETON_PROPMGR(CLoginEncourageMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_LOGIN_ENC ; }
	BOOL Init();
	VOID UnInit();

public:	
	VOID GetEncourageItem(DT_ITEM_DATA_CLI* pstItemInfo) { memcpy(pstItemInfo, &m_astItemInfo, sizeof(m_astItemInfo)); }
	UINT8 RateHitItem();
	SLoginEncourageProp* GetLoginEncourageProp(UINT8 byIdx);
public:
	CLoginEncourageMgr();
	virtual ~CLoginEncourageMgr();

protected:
	BOOL LoadFromDB();	

public:
    //CLoginEncourageRecord* CreateLoginEncourageRecord();
    //VOID  RemoveLoginEncourageRecord(CLoginEncourageRecord*);

private:
    //CSDObjectPool<CLoginEncourageRecord, CSDMutex>  m_oLoginEncourageRecordPool;

private:
	DT_ITEM_DATA_CLI	m_astItemInfo[MAX_LOGIN_ENCOURAGE_ITEM_NUM];//奖励的物品，用于给客户端
	SLoginEncourageProp	m_astLoginEncourageProp[MAX_LOGIN_ENCOURAGE_ITEM_NUM];//奖励配置
};

#endif 
