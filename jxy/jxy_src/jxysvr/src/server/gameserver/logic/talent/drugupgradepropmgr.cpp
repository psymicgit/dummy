
#include "drugupgradepropmgr.h"
#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>

#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

#define SQL_READ_DRUG_GRADE_PROP_STRING "select * from drugupgradeprop order by SrcLevel"

IMPLEMENT_SINGLETON_PROPMGR(CDrugUpradePropMgr)


CDrugUpradePropMgr::CDrugUpradePropMgr()
{

}

CDrugUpradePropMgr::~CDrugUpradePropMgr()
{
    CDrugUpradePropMgr::DestroyInstance();
}


BOOL CDrugUpradePropMgr::Init()
{
    return LoadFromDB();
}

VOID CDrugUpradePropMgr::UnInit()
{
	m_mapUpradePropDrug.clear();
	memset(m_byDrugNum, 0, sizeof(UINT8) * MAX_DRUG_LEVEL);
}

BOOL CDrugUpradePropMgr::LoadFromDB()
{
	return TRUE; //已经废弃
	UINT8	byTotal	=	0;
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("drugupgradeprop", " order by SrcLevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
    // 查询的表结果
	while(pRes->GetRecord() && byTotal < MAX_DRUG_LEVEL)// 当有行结果时
	{
		SDrugUpradeProp stDrugUpradeProp;
		memset(&stDrugUpradeProp, 0x00, sizeof(SDrugUpradeProp));
		stDrugUpradeProp.dwSrcLevel = SGDP::SDAtou(pRes->GetFieldValueByName("SrcLevel"));			//丹药等级
		stDrugUpradeProp.dwSrcNum = SGDP::SDAtou(pRes->GetFieldValueByName("SrcNum"));				//合成丹药的个数


		stDrugUpradeProp.dwLevel1Rate = SGDP::SDAtou(pRes->GetFieldValueByName("Level1Rate"));	
		stDrugUpradeProp.dwLevel2Rate = SGDP::SDAtou(pRes->GetFieldValueByName("Level2Rate"));			//
		stDrugUpradeProp.dwLevel3Rate = SGDP::SDAtou(pRes->GetFieldValueByName("Level3Rate"));			//
		stDrugUpradeProp.dwLevel4Rate = SGDP::SDAtou(pRes->GetFieldValueByName("Level4Rate"));			//
		stDrugUpradeProp.dwLevel5Rate = SGDP::SDAtou(pRes->GetFieldValueByName("Level5Rate"));			//
		stDrugUpradeProp.dwLevel6Rate = SGDP::SDAtou(pRes->GetFieldValueByName("Level6Rate"));			//
		m_mapUpradePropDrug[stDrugUpradeProp.dwSrcLevel] = stDrugUpradeProp;
		m_byDrugNum[byTotal] = static_cast<UINT8>(stDrugUpradeProp.dwSrcNum);
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;

}

VOID CDrugUpradePropMgr::GetDT_MIX_DRUG_NUM(UINT8 byDrugNum[MAX_DRUG_LEVEL])
{
	memcpy(byDrugNum, (void*)m_byDrugNum, sizeof(byDrugNum));
}

UINT8 CDrugUpradePropMgr::GetDT_MIX_DRUG_NUM(UINT8 byDrugLeave)
{
	CDrugUpradePropMapItr itr = m_mapUpradePropDrug.find(byDrugLeave);
	if (itr != m_mapUpradePropDrug.end())
	{
		return itr->second.dwSrcNum;
	}
	return 0;
}

VOID  CDrugUpradePropMgr::GetDT_MIX_DRUG_RATE(CRandomVec &vecRandom, UINT8 byDrugLeave)
{
	CDrugUpradePropMapItr itr = m_mapUpradePropDrug.find(byDrugLeave);
	if (itr != m_mapUpradePropDrug.end())
	{
		SDrugUpradeProp &stDrugUpradeProp = itr->second;
		vecRandom.push_back(stDrugUpradeProp.dwLevel1Rate);
		vecRandom.push_back(stDrugUpradeProp.dwLevel2Rate);	
		vecRandom.push_back(stDrugUpradeProp.dwLevel3Rate);	
		vecRandom.push_back(stDrugUpradeProp.dwLevel4Rate);	
		vecRandom.push_back(stDrugUpradeProp.dwLevel5Rate);	
		vecRandom.push_back(stDrugUpradeProp.dwLevel6Rate);	
	}
}


