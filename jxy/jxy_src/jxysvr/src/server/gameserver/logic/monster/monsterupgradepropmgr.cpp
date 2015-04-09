#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <framework/gsapi.h>
#include "monsterupgradepropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>

#include <db/dbmgr.h>

#define SQL_READ_MONSTER_STRING "select * from monsterupgradeprop order by level"


IMPLEMENT_SINGLETON_PROPMGR(CMonsterUpgradePropMgr);

CMonsterUpgradePropMgr::CMonsterUpgradePropMgr()
{
}

CMonsterUpgradePropMgr::~CMonsterUpgradePropMgr()
{
}

BOOL CMonsterUpgradePropMgr::Init()
{
    return LoadFromDB();
}

VOID CMonsterUpgradePropMgr::UnInit()
{
	m_mapMonsterUpgradeProp.clear();

}


// 从数据源获取数据
BOOL CMonsterUpgradePropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("monsterupgradeprop", " order by level").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SMonsterUpgradeProp stMonsterProp;
		memset(&stMonsterProp, 0, sizeof(SMonsterUpgradeProp));
		stMonsterProp.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
		stMonsterProp.byType = SGDP::SDAtou(pRes->GetFieldValueByName("Type"));
		//stMonsterProp.byCareerID = SGDP::SDAtou(pRes->GetFieldValueByName("CareerID"));		
		//stMonsterProp.dwHP = SGDP::SDAtou(pRes->GetFieldValueByName("HP"));
		//stMonsterProp.dwAttack = SGDP::SDAtou(pRes->GetFieldValueByName("Attack"));
		stMonsterProp.dwEncourageExp = SGDP::SDAtou(pRes->GetFieldValueByName("EncourageExp"));
		stMonsterProp.dwEncourageCoin = SGDP::SDAtou(pRes->GetFieldValueByName("EncourageCoin"));
		stMonsterProp.dwEncourageStory = SGDP::SDAtou(pRes->GetFieldValueByName("EncourageStory"));
		stMonsterProp.dwEncourageJingJie = SGDP::SDAtou(pRes->GetFieldValueByName("EncourageJingJie"));

		m_mapMonsterUpgradeProp[CLogicFunc::Get1616Key(stMonsterProp.wLevel,stMonsterProp.byType)] = stMonsterProp;
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

SMonsterUpgradeProp* CMonsterUpgradePropMgr::GetMonsterUpgradeProp(UINT16 wLevel, UINT8	byType)
{
	CMonsterUpgradePropMapItr itr = m_mapMonsterUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, byType));
	if(itr != m_mapMonsterUpgradeProp.end())
	{
		return &(itr->second);
	}

	return NULL;

}

