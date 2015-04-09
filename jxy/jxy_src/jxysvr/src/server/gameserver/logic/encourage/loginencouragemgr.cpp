#include "loginencouragemgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <sqlite3db.h>
#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <logic/item/goods/goodspropmgr.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CLoginEncourageMgr)


CLoginEncourageMgr::CLoginEncourageMgr()
{
	memset(&m_astItemInfo, 0, sizeof(m_astItemInfo));
	memset(&m_astLoginEncourageProp, 0, sizeof(m_astLoginEncourageProp));

}

CLoginEncourageMgr::~CLoginEncourageMgr()
{

}

BOOL CLoginEncourageMgr::Init()
{
	//读取配置文件
	if(FALSE == LoadFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);   
		return FALSE;
	}

	return TRUE;

}


VOID CLoginEncourageMgr::UnInit()
{
	memset(&m_astItemInfo, 0, sizeof(m_astItemInfo));
	memset(&m_astLoginEncourageProp, 0, sizeof(m_astLoginEncourageProp));
}

// 从数据源获取数据
BOOL CLoginEncourageMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("loginencourage").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	UINT8 byItemNum = 0;
	while((pRes->GetRecord()) && (byItemNum < MAX_LOGIN_ENCOURAGE_ITEM_NUM))// 当有行结果时
	{
		SLoginEncourageProp& stProp = m_astLoginEncourageProp[byItemNum];
		stProp.wItemKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
		stProp.wPileNum = SGDP::SDAtou(pRes->GetFieldValueByName("Num"));
		stProp.byRate = SGDP::SDAtou(pRes->GetFieldValueByName("Rate"));		
		DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = m_astItemInfo[byItemNum];
		stDT_ITEM_DATA_CLI.wKindID = stProp.wItemKindID;
		SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stDT_ITEM_DATA_CLI.wKindID);
		if(pstGoodsProp)
		{
			stDT_ITEM_DATA_CLI.byGoodsMainKindID = pstGoodsProp->byMainKindID;
		}

		stDT_ITEM_DATA_CLI.wPileNum = stProp.wPileNum;

		byItemNum++;
	}
	if(byItemNum != MAX_LOGIN_ENCOURAGE_ITEM_NUM)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoginEncourage item is not [%d]!"), MSG_MARK, MAX_LOGIN_ENCOURAGE_ITEM_NUM);    
		return FALSE;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	

	return TRUE;
}



UINT8 CLoginEncourageMgr::RateHitItem()
{
	CRandomVec vecRandom;
	for(UINT8 byItemNum = 0; byItemNum < MAX_LOGIN_ENCOURAGE_ITEM_NUM; byItemNum++)
	{
		vecRandom.push_back(m_astLoginEncourageProp[byItemNum].byRate);		
	}

	return GetRandomIdxMustHit(vecRandom) + 1;//使用必须命中算法,从1开始
}


SLoginEncourageProp* CLoginEncourageMgr::GetLoginEncourageProp(UINT8 byIdx)
{
	if((byIdx < 1) || (byIdx > MAX_LOGIN_ENCOURAGE_ITEM_NUM))
	{
		return NULL;
	}

	return &m_astLoginEncourageProp[byIdx-1];
}

