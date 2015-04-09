#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include <db/updatecdkeycmd.h>
#include "activityencouragemgr.h"
#include <db/dbcommoperate.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <sdloggerimpl.h>
#include <json/json.h>



IMPLEMENT_SINGLETON(CActivityEncourageMgr);

CActivityEncourageMgr::CActivityEncourageMgr()
{
    m_wZoneID = 0;
}

CActivityEncourageMgr::~CActivityEncourageMgr()
{

}

BOOL CActivityEncourageMgr::Init()
{
    return LoadFromDBCfg() && LoadFromDBPay();
}

VOID CActivityEncourageMgr::UnInit()
{
    m_mapActivityEncourageProp.clear();
}

SActivityEncouragePro  * CActivityEncourageMgr::GetActivityEncouragePro(UINT64	qwActivityID)
{
    CActivityEncouragePropMapItr itr = m_mapActivityEncourageProp.find(qwActivityID);
    if (itr != m_mapActivityEncourageProp.end())
    {
        return &itr->second;
    }
    return NULL;
}

BOOL CActivityEncourageMgr::LoadFromDBCfg()
{
    //获取GAMESERVER的活动奖励配置
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    CHAR szSql[512] = {0};
    sprintf(szSql, "select * from activityencouragepro");
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    //查询的表结果
    while(pRecordSet->GetRecord())// 当有行结果时
    {
        UINT16 wActivityType	= SGDP::SDAtou(pRecordSet->GetFieldValueByName("ActivityType"));
        string strJsonValues = pRecordSet->GetFieldValueByName("JsonValues");
        if (ACTE_RECHARGE == wActivityType)
        {
            //解析Json值
            if(!JsonParseResItem(m_vecRechargeActivityEncourageItemInfo, strJsonValues))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, JsonValues:%s!"), MSG_MARK, strJsonValues.c_str());
                return FALSE;
            }
        }
        else if (ACTE_SCORE == wActivityType)
        {
            //解析Json值
            if(!JsonParseResItem(m_vecScoreActivityEncourageItemInfo, strJsonValues))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, JsonValues:%s!"), MSG_MARK, strJsonValues.c_str());
                return FALSE;
            }
        }
    }

    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    if (0 == m_vecRechargeActivityEncourageItemInfo.vecJsonResItem.size() ||
            0 == m_vecScoreActivityEncourageItemInfo.vecJsonResItem.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data Score or Recharge!"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}




BOOL CActivityEncourageMgr::LoadFromDBPay()
{
    //获取GAMESERVER的活动奖励配置
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    m_wZoneID = gsapi::GetZoneID();
    m_stChangeActivityEncouragePro.wZoneID = m_wZoneID;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    CHAR szSql[512] = {0};
    sprintf(szSql, "select * from activityencouragepro where (ZoneID = %u or ZoneID = 0) ORDER BY ActivityID", m_wZoneID);
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return TRUE;
    }

    //查询的表结果
    while(pRecordSet->GetRecord())// 当有行结果时
    {
        SActivityEncouragePro stProp;
        stProp.qwActivityID	= SGDP::SDAtou64(pRecordSet->GetFieldValueByName("ActivityID"));
        stProp.byNeedCdKey	= SGDP::SDAtou(pRecordSet->GetFieldValueByName("IsNeedCdKey"));
        UINT32	dwLen		= pRecordSet->GetFieldLengthByName("ActivityName");
        SDStrncpy(stProp.szActivityName, pRecordSet->GetFieldValueByName("ActivityName"), dwLen);
        dwLen				= pRecordSet->GetFieldLengthByName("ActivityDesc");
        SDStrncpy(stProp.szActivityDesc, pRecordSet->GetFieldValueByName("ActivityDesc"), dwLen);
        string strJsonValues	= pRecordSet->GetFieldValueByName("JsonValues");

        GetDateTime2Stamp(stProp.qwStartTime, "StartTime");
        GetDateTime2Stamp(stProp.qwEndTime, "EndTime");

        if (stProp.qwStartTime > stProp.qwEndTime)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: time limit!!"), MSG_MARK);
            return FALSE;
        }

        //解析Json值
        if(!JsonParseResItem(stProp.vecActivityEncourageItemInfo, strJsonValues))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, JsonValues:%s!"), MSG_MARK, strJsonValues.c_str());
            return FALSE;
        }

        m_mapActivityEncourageProp[stProp.qwActivityID] = stProp;

    }

    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }


    nRet = pDBSession->ExecuteSqlRs("select * from activityencourageplayer order by ActivityID, StartPlayerID;", &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        //查询的表结果
        while(pRecordSet->GetRecord())// 当有行结果时
        {
            UINT64 qwActivityID		= SGDP::SDAtou64(pRecordSet->GetFieldValueByName("ActivityID"));
            UINT32 dwStartPlayerID		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("StartPlayerID"));
            UINT32 dwEndPlayerID		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("EndPlayerID"));
            if(dwEndPlayerID < dwStartPlayerID)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EndPlayerID[%u] < StartPlayerID[%u]!"), MSG_MARK, dwEndPlayerID, dwStartPlayerID);
                return FALSE;
            }
            CActivityEncouragePropMapItr itr = m_mapActivityEncourageProp.find(qwActivityID);
            if (itr != m_mapActivityEncourageProp.end())
            {
                SActivityEncouragePro & stProp = itr->second;
                stProp.mapActivityEncourageStartPlayerID[dwStartPlayerID] = dwEndPlayerID;
                stProp.mapActivityEncourageEndPlayerID[dwEndPlayerID] = dwStartPlayerID;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


UINT16 CActivityEncourageMgr::GetHeroID()
{
    CJsonResItemVectorItr itr = m_vecRechargeActivityEncourageItemInfo.vecJsonResItem.begin();
    for (itr; itr != m_vecRechargeActivityEncourageItemInfo.vecJsonResItem.end(); itr++)
    {
        if (itr->byKindType == ACTE_HERO)
        {
            return itr->dwKindValue;
        }
    }
    return 0;
}

BOOL CActivityEncourageMgr::OnEvent()
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_ACTIVITYENCOURAGE);
    if(NULL != poCMD)
    {
        poCMD->SetUserData(&m_stChangeActivityEncouragePro);
        gsapi::GetDBMgr()->AddPayCommand(poCMD);
    }
    return TRUE;
}

VOID CActivityEncourageMgr::SetActivityEncouragePropMap(CActivityEncouragePropMap &mapActivityEncourageProp)
{
    //检查所有符合条件的玩家并下发通知
    const CID2PlayerMap& mapPlayer = CPlayerMgr::Instance()->GetID2PlayerMap();
    for(CID2PlayerMapConstItr itr = mapPlayer.begin(); itr != mapPlayer.end(); itr++)
    {
        CPlayer* poPlayer = itr->second;
        if((NULL == poPlayer) || (!poPlayer->CkLogin()))
        {
            continue;
        }

        //检测一下奖励建筑是否开放。
        CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY_ENCOURAGE);
        if(NULL == poBuild)
        {
            continue;
        }

        PKT_CLIGS_ACTIVITY_ENC_NTF stNtf;
        UINT8 buLastNum = poPlayer->GetActivityEncourage().GetLastActivityEncourageNum();
        stNtf.byEncNum = poPlayer->GetActivityEncourage().GetActivityEncourageNum();
		if ( 0 != poPlayer->GetDT_PLAYER_BASE_DATA().byBagOpenNum )
		{
			stNtf.byEncNum += 1;
		}
        if((stNtf.byEncNum > 0) || ((0 == stNtf.byEncNum) && (0 != buLastNum)))
        {
            poPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_ACTIVITY_ENC_NTF);
        }
    }
    m_mapActivityEncourageProp.clear();
    m_mapActivityEncourageProp = mapActivityEncourageProp;
}



UINT8 CActivityEncourageMgr::IsNeedCdKey(UINT64 qwActivityID)
{
    SActivityEncouragePro  * poActivityEncouragePro = CActivityEncourageMgr::Instance()->GetActivityEncouragePro(qwActivityID);
    if (NULL == poActivityEncouragePro)
    {
        return 0;
    }
    return poActivityEncouragePro->byNeedCdKey;
}

BOOL CActivityEncourageMgr::CkActivityCdKeyValid(UINT32 dwPlayerID, UINT64 qwActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM])
{
    //CDKEY属于全服唯一，则去全服pay表中操作
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_UPDATE_CK_KEY);
    if(NULL != poCMD)
    {
        UPDATE_CK_KEY stUPDATE_CK_KEY;
        stUPDATE_CK_KEY.qwActivityID = qwActivityID;
        SDStrcpy(stUPDATE_CK_KEY.aszSeriesNo, aszSeriesNo);
        poCMD->SetUserData((VOID*)&stUPDATE_CK_KEY, dwPlayerID);
        return gsapi::GetDBMgr()->AddPayCommand(poCMD);
    }
    return FALSE;
}


VOID CActivityEncourageMgr::AddPlayerActivity(UINT64 qwActivityID, UINT32 dwStartPlayerID, UINT32 dwEndPlayerID)
{
    CActivityEncouragePropMapItr itr = m_mapActivityEncourageProp.find(qwActivityID);
    if(itr == m_mapActivityEncourageProp.end())
    {
        return;
    }
    itr->second.mapActivityEncourageStartPlayerID[dwStartPlayerID] = dwEndPlayerID;
    itr->second.mapActivityEncourageEndPlayerID[dwEndPlayerID] = dwStartPlayerID;
}



