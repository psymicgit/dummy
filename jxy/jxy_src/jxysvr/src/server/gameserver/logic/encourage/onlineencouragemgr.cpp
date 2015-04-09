#include "onlineencouragemgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <sqlite3db.h>
#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <logic/item/goods/goodspropmgr.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(COnlineEncourageMgr)


COnlineEncourageMgr::COnlineEncourageMgr()
{

}

COnlineEncourageMgr::~COnlineEncourageMgr()
{

}

BOOL COnlineEncourageMgr::Init()
{
	//读取配置文件
	if(FALSE == LoadFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);   
		return FALSE;
	}

	return TRUE;

}


VOID COnlineEncourageMgr::UnInit()
{
	m_vecOnlineEncourageProp.clear();
	m_byMaxPerDay = 0;
}

// 从数据源获取数据
BOOL COnlineEncourageMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("onlineencourage", " order by Idx asc").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	UINT8 byEncIdx = 1;
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SOnlineEncourageProp stOnlineEncourageProp;
		//idx必须按顺序，从1开始
		if(byEncIdx++ != SGDP::SDAtou(pRes->GetFieldValueByName("Idx")))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Idx[%d] invalid!"), MSG_MARK, byEncIdx);   
			return FALSE;
		}
		stOnlineEncourageProp.wRecvCD = SGDP::SDAtou(pRes->GetFieldValueByName("RecvCD"));
		DT_ONLINE_ENCOURAGE_DATA& stDT_ONLINE_ENCOURAGE_DATA = stOnlineEncourageProp.stDT_ONLINE_ENCOURAGE_DATA;
		stDT_ONLINE_ENCOURAGE_DATA.dwCoin = SGDP::SDAtou(pRes->GetFieldValueByName("Coin"));
		stDT_ONLINE_ENCOURAGE_DATA.dwGold = SGDP::SDAtou(pRes->GetFieldValueByName("Gold"));
		stDT_ONLINE_ENCOURAGE_DATA.dwScience = SGDP::SDAtou(pRes->GetFieldValueByName("Science"));
		stDT_ONLINE_ENCOURAGE_DATA.dwExperience = SGDP::SDAtou(pRes->GetFieldValueByName("Experience"));
		stDT_ONLINE_ENCOURAGE_DATA.wPhyStrength = SGDP::SDAtou(pRes->GetFieldValueByName("PhyStrength"));				
		UINT8 byItemNum = 0;
		for(UINT8 byIdx = 1; byIdx <= 3; byIdx++) //3个物品
		{
			CHAR szTmp[32] = {0};
			sprintf(szTmp, "ItemKindID%d", byIdx);
			DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stDT_ONLINE_ENCOURAGE_DATA.astItemInfoList[byItemNum];
			stDT_ITEM_DATA_CLI.wKindID = SGDP::SDAtou(pRes->GetFieldValueByName(szTmp));	
			if(0 == stDT_ITEM_DATA_CLI.wKindID)
			{
				continue;
			}
			sprintf(szTmp, "ItemPileNum%d", byIdx);
			stDT_ITEM_DATA_CLI.wPileNum = SGDP::SDAtou(pRes->GetFieldValueByName(szTmp));	
			SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stDT_ITEM_DATA_CLI.wKindID);
			if(pstProp)
			{
				stDT_ONLINE_ENCOURAGE_DATA.astItemInfoList[byItemNum].byGoodsMainKindID = pstProp->byMainKindID;
			}	
			byItemNum++;
		}
		stDT_ONLINE_ENCOURAGE_DATA.byItemNum = byItemNum;

		m_vecOnlineEncourageProp.push_back(stOnlineEncourageProp);
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	m_byMaxPerDay = m_vecOnlineEncourageProp.size();

	return TRUE;
}


SOnlineEncourageProp* COnlineEncourageMgr::GetEncourage(UINT8 byIdx)
{
	if((0 == byIdx) || (byIdx > m_vecOnlineEncourageProp.size()))
	{
		return NULL;
	}
	return &m_vecOnlineEncourageProp[byIdx-1];
}