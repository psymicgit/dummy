#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>
#include "begbluepropmgr.h"
#include <logic/other/errmsgmgr.h>
#include <db/dbmgr.h>

using namespace SGDP;

//const int MAX_BEGBLUEPROP_ROW = 7;

IMPLEMENT_SINGLETON_PROPMGR(CBegBluePropMgr);

CBegBluePropMgr::CBegBluePropMgr()
{
    
}

CBegBluePropMgr::~CBegBluePropMgr()
{
}

BOOL CBegBluePropMgr::Init()
{
	if(!LoadBegBluePropFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadBegBluePropFromDB failed!"), MSG_MARK); 
		return FALSE;
	}
    if (!LoadBegBlueCouragePropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadBegBlueCouragePropFromDB failed!"), MSG_MARK); 
        return FALSE;
    }
	return TRUE;
}

VOID CBegBluePropMgr::UnInit()
{
    m_mapBegBluePropMap.clear();
    m_mapBegBlueCourageProp.clear();
}


BOOL CBegBluePropMgr::LoadBegBluePropFromDB()
{	
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

    UINT32 dwErrID = 0;
	string strErr;
    
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("begblueprop", " order by CurrSmileCnt ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    
	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
	    SBegBlueProp stProp = {0};
        stProp.byCurrSmileCnt = SGDP::SDAtou(pRes->GetFieldValueByName("CurrSmileCnt"));  

        // to del elem
        stProp.by0SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("ZeroSmileOdds"));  
        stProp.by1SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("OneSmileOdds"));   
        stProp.by2SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("TwoSmileOdds"));   
        stProp.by3SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("ThreeSmileOdds")); 
        stProp.by4SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("FourSmileOdds"));  
        stProp.by5SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("FiveSmileOdds"));  
        stProp.by6SmileOdds   = SGDP::SDAtou(pRes->GetFieldValueByName("SixSmileOdds"));   
        
        stProp.byX1Odds       = SGDP::SDAtou(pRes->GetFieldValueByName("x1Odds"));         
        stProp.byX2Odds       = SGDP::SDAtou(pRes->GetFieldValueByName("x2Odds"));         
        stProp.byX3Odds       = SGDP::SDAtou(pRes->GetFieldValueByName("x3Odds"));         
        stProp.byX4Odds       = SGDP::SDAtou(pRes->GetFieldValueByName("x4Odds"));         
        stProp.byX5Odds       = SGDP::SDAtou(pRes->GetFieldValueByName("x5Odds"));         
        stProp.byX6Odds       = SGDP::SDAtou(pRes->GetFieldValueByName("x6Odds"));   
        
        stProp.dwBlueUnit     = SGDP::SDAtou(pRes->GetFieldValueByName("BlueUnit"));       
        stProp.dwStoryUnit    = SGDP::SDAtou(pRes->GetFieldValueByName("StoryUnit"));   

        SDStrcpy(stProp.aszLuckPrompt, pRes->GetFieldValueByName("LuckPrompt"));
        
        stProp.vecSmileRandom.push_back(stProp.by0SmileOdds);
        stProp.vecSmileRandom.push_back(stProp.by1SmileOdds);
        stProp.vecSmileRandom.push_back(stProp.by2SmileOdds);
        stProp.vecSmileRandom.push_back(stProp.by3SmileOdds);
        stProp.vecSmileRandom.push_back(stProp.by4SmileOdds);
        stProp.vecSmileRandom.push_back(stProp.by5SmileOdds);
        stProp.vecSmileRandom.push_back(stProp.by6SmileOdds);

        stProp.vecXnMultipleRandom.push_back(stProp.byX1Odds);
        stProp.vecXnMultipleRandom.push_back(stProp.byX2Odds);
        stProp.vecXnMultipleRandom.push_back(stProp.byX3Odds);
        stProp.vecXnMultipleRandom.push_back(stProp.byX4Odds);
        stProp.vecXnMultipleRandom.push_back(stProp.byX5Odds);
        stProp.vecXnMultipleRandom.push_back(stProp.byX6Odds);

        m_mapBegBluePropMap[stProp.byCurrSmileCnt] = stProp;
        
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
    /*
	if(byRowNum != MAX_BEGBLUEPROP_ROW)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: begblueprop row[%d] not match[%d] !"), MSG_MARK, byRowNum, MAX_BEGBLUEPROP_ROW); 
		return FALSE;
	}
    */
    
	return TRUE;
}


BOOL CBegBluePropMgr::GetBegBlueRes(UINT16 wLastSmileCnt, UINT16& wNewSmileCnt, UINT16& wXnMultiple, 
                   UINT32& dwBlueUnit, UINT32& dwStoryUnit, TCHAR aszLuckPrompt[BEGBLUE_LUCK_PROMPT], UINT32 dwCliver)
{
	CBegBluePropMapItr itr = m_mapBegBluePropMap.find(wLastSmileCnt);
	if(itr == m_mapBegBluePropMap.end())
	{
		return FALSE;
	}
    SBegBlueProp& stProp = itr->second;

	//随机
	INT32 iSmileRanIdx = GetRandomIdxMustHit(stProp.vecSmileRandom);
    INT32 iMultipleRanIdx = GetRandomIdxMustHit(stProp.vecXnMultipleRandom);
    
	if(RANDOM_UNHIT == iSmileRanIdx || RANDOM_UNHIT == iMultipleRanIdx )
	{
		return FALSE;
	}

    wNewSmileCnt = iSmileRanIdx;

    //新版本都为1倍
    if (dwCliver > CLIENT_VER)
    {
        wXnMultiple = 1;
    }
    else
    {
        wXnMultiple = iMultipleRanIdx + 1;
    }
   
    // 根据新才笑脸数量取新的值
	itr = m_mapBegBluePropMap.find(wNewSmileCnt);
	if(itr == m_mapBegBluePropMap.end())
	{
		return FALSE;
	}

    SBegBlueProp& stNewProp = itr->second;
    
    dwBlueUnit  = stNewProp.dwBlueUnit;
    dwStoryUnit = stNewProp.dwStoryUnit;
    
    SDStrcpy(aszLuckPrompt, stNewProp.aszLuckPrompt);
    
    return TRUE;
}

BOOL CBegBluePropMgr::LoadBegBlueCouragePropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

    UINT32 dwErrID = 0;
	string strErr;
    
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("begblueencourageprop", " order by SmileNum ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    
	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
	    SBegBlueCourageProp stProp = {0};
        stProp.dwSmileNum = SGDP::SDAtou(pRes->GetFieldValueByName("SmileNum"));  
        stProp.dwBlue   = SGDP::SDAtou(pRes->GetFieldValueByName("Blue"));    
        m_mapBegBlueCourageProp[stProp.dwSmileNum] = stProp;      
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
    /*
	if(byRowNum != MAX_BEGBLUEPROP_ROW)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: begblueprop row[%d] not match[%d] !"), MSG_MARK, byRowNum, MAX_BEGBLUEPROP_ROW); 
		return FALSE;
	}
    */
    
	return TRUE;
}

SBegBlueCourageProp* CBegBluePropMgr::GetBegBlueCourageProp( UINT32 dwSmileNum )
{
    CBegBlueCouragePropMapItr itr = m_mapBegBlueCourageProp.find(dwSmileNum);
    if (itr != m_mapBegBlueCourageProp.end())
    {
        return &itr->second;
    }
    return NULL;
}

VOID CBegBluePropMgr::GetEncouragePrompt(UINT32 dwSmileNum, TCHAR aszEncouragePrompt[BEGBLUE_ENCOURAGE_PROMPT] )
{
    CBegBlueCouragePropMapItr itr = m_mapBegBlueCourageProp.begin();
    while(itr != m_mapBegBlueCourageProp.end())
    {
        if (itr->first > dwSmileNum)
        {
            break;
        }
        itr++;
    }
    if (itr == m_mapBegBlueCourageProp.end())
    {
        return;
    }
    UINT32 dwNum = itr->first > dwSmileNum ? itr->first - dwSmileNum : 0;
    vector<string> vecFillMsg;
    CHAR szBuff[21] = {0};
    sprintf(szBuff, "%u", dwNum);
    vecFillMsg.push_back(szBuff);
    sprintf(szBuff, "%u", itr->second.dwBlue);
    vecFillMsg.push_back(szBuff);
    memset(aszEncouragePrompt, 0, BEGBLUE_ENCOURAGE_PROMPT);
    SGDP::SDStrncpy(aszEncouragePrompt, CMsgDefMgr::Instance()->GetErrMsg("BB_COURAGE_MS", &vecFillMsg).c_str(), BEGBLUE_ENCOURAGE_PROMPT - 1);
    
}
