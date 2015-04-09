#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>
#include "sendflowerpropmgr.h"
#include <logic/player/playerbasedatacachemgr.h>
#include "errdef.h"
#include <protocligs.h>
#include <db/dbmgr.h>
#include "logic/base/jsonencourage.h"

using namespace SGDP;


IMPLEMENT_SINGLETON_PROPMGR(CSendFlowerMgr);

CSendFlowerMgr::CSendFlowerMgr()
{

}

CSendFlowerMgr::~CSendFlowerMgr()
{
}

BOOL CSendFlowerMgr::Init()
{
    if(!LoadSendFlowerPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadSendFlowerPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CSendFlowerMgr::UnInit()
{
    m_mapSendFlowerPropMap.clear();
}

SSendFlowerEncourage *CSendFlowerMgr::GetSendFlowerEncourage(UINT32	dwFlowerNum)
{
    SSendFlowerEncourage *_p = NULL;
    CSendFlowerPropVectorItr itr;
    for( itr = m_vecSendFlowerProp.begin(); itr != m_vecSendFlowerProp.end(); itr++)
    {
        if (itr->dwFlowerNum == dwFlowerNum)
        {
            return &(*itr);
        }
        else
        {
            continue;
        }
    }
	if ( itr == m_vecSendFlowerProp.end())
	{
		SSendFlowerEncourage* poMax = GetMaxSendFlowerEncourage();
		if ( NULL == poMax)
		{
			return NULL;
		}
		UINT32 dwLeft = dwFlowerNum % poMax->dwFlowerNum;
		if ( 0 == dwLeft )
		{
			return poMax;
		}
	}
    return NULL;
}

SSendFlowerEncourage *CSendFlowerMgr::GetMaxSendFlowerEncourage()
{
    CSendFlowerPropVectorRItr itr = m_vecSendFlowerProp.rbegin();
    return &(*itr);
}

SSendFlowerEncourage *CSendFlowerMgr::GetNextSendFlowerEncourage(UINT32	dwFlowerNum, UINT32 &dwNextFlowerNum)
{
	
    CSendFlowerPropVectorItr itr;
    for( itr = m_vecSendFlowerProp.begin(); itr != m_vecSendFlowerProp.end(); itr++)
    {
        dwNextFlowerNum = itr->dwFlowerNum;
        if (itr->dwFlowerNum > dwFlowerNum)
        {
            return &(*itr);
        }
    }
	if ( itr == m_vecSendFlowerProp.end())
	{
		CSendFlowerPropVector::reverse_iterator reItr = m_vecSendFlowerProp.rbegin();
		dwNextFlowerNum = reItr->dwFlowerNum;
		return &(*reItr);
	}
    return NULL;
}

SRecvFlowerEncourage *CSendFlowerMgr::GetRecvFlowerEncourage(UINT16	wKindID)
{
    CRecvFlowerPropVectorItr itr;
    for( itr = m_vecRecvFlowerProp.begin(); itr != m_vecRecvFlowerProp.end(); itr++)
    {
        if (itr->wKindID == wKindID)
        {
            return &(*itr);
        }
    }
    return NULL;
}


BOOL CSendFlowerMgr::LoadSendFlowerPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;

    UINT32 dwErrID = 0;
    string strErr;

    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("sendflowerprop", " order by flowerNum ").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord()) // 当有行结果时
    {
        SSendFlowerProp stProp = {0};
        stProp.dwFlowerNum     = SGDP::SDAtou(pRes->GetFieldValueByName("flowerNum"));
        stProp.wSendMaxTimes   = SGDP::SDAtou(pRes->GetFieldValueByName("sendMaxTimes"));
        stProp.byNeedVipLvl    = SGDP::SDAtou(pRes->GetFieldValueByName("needVipLvl"));
        stProp.wCostKindID     = SGDP::SDAtou(pRes->GetFieldValueByName("costKindID"));
        stProp.dwCostKindIDNum = SGDP::SDAtou(pRes->GetFieldValueByName("costKindIDNum"));
        stProp.wGetKindID      = SGDP::SDAtou(pRes->GetFieldValueByName("getKindID"));
        stProp.dwGetKindIDNum  = SGDP::SDAtou(pRes->GetFieldValueByName("getKindIDNum"));

        m_mapSendFlowerPropMap[stProp.dwFlowerNum] = stProp;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("sendflowerencourageprop", " order by flowerNum ").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord()) // 当有行结果时
    {
        SSendFlowerEncourage stProp;
        stProp.dwFlowerNum    = SGDP::SDAtou(pRes->GetFieldValueByName("flowerNum"));
        string strJsonData	= pRes->GetFieldValueByName("jsoncouragedata");

        if(!JsonParseResItem(stProp.vecJsonEncourageItemInfo, strJsonData))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, strJsonData:%s!"), MSG_MARK, strJsonData.c_str());
            return FALSE;
        }

        m_vecSendFlowerProp.push_back(stProp);
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    //////////////////////////////////////////////////////////////////////////
    nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("recvflowerencourageprop", " order by KindID ").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord()) // 当有行结果时
    {
        SRecvFlowerEncourage stProp;
        stProp.wKindID		= SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        string strJsonData	= pRes->GetFieldValueByName("jsoncouragedata");
        if(!JsonParseResItem(stProp.vecJsonEncourageItemInfo, strJsonData))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, strJsonData:%s!"), MSG_MARK, strJsonData.c_str());
            return FALSE;
        }

        m_vecRecvFlowerProp.push_back(stProp);
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

// 获取送鲜花配置列表(for 打开送鲜花面板)
UINT16 CSendFlowerMgr::GetSendFlowerProp(DT_SEND_FLOWER_PROP_LIST_CLI& stSendFlowerPropList)
{
    stSendFlowerPropList.dwPropNum = m_mapSendFlowerPropMap.size();

    size_t i = 0;
    for(CSendFlowerPropMapIter iter = m_mapSendFlowerPropMap.begin(); iter != m_mapSendFlowerPropMap.end() && i < MAX_SEND_FLOWER_PROP_NUM; iter++, i++)
    {
        stSendFlowerPropList.astListData[i].dwFlowerNum = iter->second.dwFlowerNum;

        // 送鲜花花费的资源:1表铜钱，2表元宝
        if (1 == iter->second.wCostKindID)
        {
            stSendFlowerPropList.astListData[i].dwCoin = iter->second.dwCostKindIDNum;
            stSendFlowerPropList.astListData[i].dwGold = 0;
        }
        else
        {
            stSendFlowerPropList.astListData[i].dwCoin = 0;
            stSendFlowerPropList.astListData[i].dwGold = iter->second.dwCostKindIDNum;
        }

        stSendFlowerPropList.astListData[i].wGoodsKindID = iter->second.wGetKindID;
        stSendFlowerPropList.astListData[i].dwGoodsNum   = iter->second.dwGetKindIDNum;
        stSendFlowerPropList.astListData[i].byNeedVipLvl = iter->second.byNeedVipLvl;
    }

    return 0;
}



