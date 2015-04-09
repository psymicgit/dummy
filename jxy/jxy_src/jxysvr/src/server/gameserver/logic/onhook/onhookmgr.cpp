#include <sdutil.h>
#include <common/client/commondef.h>
#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <common/client/errdef.h>
#include <logic/other/singleprammgr.h>
#include <logic/event/timereventmgr.h>
#include <logic/other/phystrengthmgr.h>
#include <logic/onhook/onhook.h>
#include <logic/bag/bag.h>
#include <logic/vip/vippropmgr.h>
#include "onhookmgr.h"
#include <logic/record/recordmgr.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <logic/player/player.h>

IMPLEMENT_SINGLETON(COnhookMgr);

COnhookMgr::COnhookMgr()
{
    memset(&m_stOnHookProp, 0, sizeof(m_stOnHookProp));
}

COnhookMgr::~COnhookMgr()
{
}

BOOL COnhookMgr::Init()
{
    return LoadFromDB();
}

VOID COnhookMgr::UnInit()
{

}


// 从数据源获取数据
BOOL COnhookMgr::LoadFromDB()
{
    return TRUE;//已经废弃
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("onhook").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    if(pRes->GetRecord())// 当有行结果时
    {
        m_stOnHookProp.wPassSecondPerInstance = SGDP::SDAtou(pRes->GetFieldValueByName("PassSecondPerInstance"));
        m_stOnHookProp.wNoCDGoldPerInstance = SGDP::SDAtou(pRes->GetFieldValueByName("NoCDGoldPerInstance"));
    }
    else
    {
        return FALSE;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

UINT16 COnhookMgr::Onhook(CPlayer* poPlayer, UINT8 byRunTimes, DT_INSTANCE_ID stDT_INSTANCE_ID, DT_ONHOOK_RESULT& stOnhookResult, DT_DATA_AFTER_ONHOOK& stAfterOnhookData)
{
    UINT16 wErrCode = ERR_ONHOOK::ID_SUCCESS;

    CInstanceRecord *pInstanceRecord = NULL;
    pInstanceRecord = poPlayer->GetInstance().GetSingleInstaceRecord(stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    if(NULL == pInstanceRecord)
    {
        return ERR_ONHOOK::ID_INSTANCE_NOT_BATTLED;
    }

    if(EIT_COMMON != stDT_INSTANCE_ID.wSceneIdx)
    {
        return ERR_ONHOOK::ID_ONHOOK_UNLOCK;
    }
	//外传挂机独立协议
	//if ( EIT_OUT_BOUND == stDT_INSTANCE_ID.wSceneIdx)
	//{
	//	if ( 1 != byRunTimes)
	//	{
	//		return ERR_ONHOOK::ID_OUT_BOUND_ERR;
	//	}

	//	SInstanceProp* poOutBoundProp = CInstancePropMgr::Instance()->GetInstanceProp(stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
	//	if ( NULL != poOutBoundProp)
	//	{
	//		if ( pInstanceRecord->GetDT_INSTANCE_DATA().dwSuccessNum >= poOutBoundProp->byBattleNum )
	//		{
	//			return ERR_ONHOOK::ID_NOT_TIME_OUT_BOUND;
	//		}
	//		
	//	}

	//}

    if(byRunTimes > MAX_INSTANCE_RUN_TIMES)
    {
        return ERR_ONHOOK::ID_OUT_MAX_RUN_RANG;
    }
    if(0 == byRunTimes)
    {
        return ERR_ONHOOK::ID_RUN_TIMES_0;
    }

    //背包判断
    CBag& oBag = poPlayer->GetBag();
    if(0 == oBag.GetIdleSize())
    {
        return ERR_ONHOOK::ID_BAG_FULL;
    }


    //先回复体力(体力由相关网络请求触发，非定时器，则先尝试回复体力)
    poPlayer->GetPhystrength().PhyStrengthRecover();//

    //体力判断
    UINT16 wCostPhyStrenth = byRunTimes * CPhystrengthMgr::Instance()->GetCommonPhyStrength();
    if(poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength < wCostPhyStrenth)
    {
        return ERR_ONHOOK::ID_PHYSTRENGTH_NOT_ENOUGH;
    }

	COnhook* poOnhook = poPlayer->GetOnhook();
	if(NULL == poOnhook)
	{
		return ERR_COMMON::ID_OTHER_ERR;
	}
    poOnhook->SetOnHookParam(byRunTimes, &stDT_INSTANCE_ID);
    DT_ONHOOK_RCD_DATA& stDT_ONHOOK_RCD_DATA = poOnhook->GetDT_ONHOOK_RCD_DATA();


    //获取奖励
    if(poOnhook->OnHookEncourage(byRunTimes))
    {
        wErrCode = ERR_ONHOOK::ID_BAG_FULL;
    }
    //扣除体力
    wCostPhyStrenth = byRunTimes * CPhystrengthMgr::Instance()->GetCommonPhyStrength();
    UINT16 wPhyStrength = poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength;
    //poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength = poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength < wCostPhyStrenth ? 0 : (poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength - wCostPhyStrenth);
    if(wPhyStrength < wCostPhyStrenth)
    {
        poPlayer->DecPhyStrength( poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength, CRecordMgr::EDPST_ONHOOK, stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    }
    else
    {
        poPlayer->DecPhyStrength( wCostPhyStrenth, CRecordMgr::EDPST_ONHOOK, stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    }
    //poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength = wPhyStrength;

    //获取结果及玩家信息
    poOnhook->GetOnhookResult(stOnhookResult, stAfterOnhookData);

    //一次性返回了，挂机结束
    stDT_ONHOOK_RCD_DATA.byOnhookFlag = 0;//挂机结束
    //poPlayer->GetLocale().OnOnhookEnd();

    //如果是爬塔，要记录最后时间，用来限制次数
    if (NULL != pInstanceRecord)
    {
        //pInstanceRecord->GetDT_INSTANCE_DATA().wPassTimes += byIdx;
        pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
		//if ( EIT_OUT_BOUND == stDT_INSTANCE_ID.wSceneIdx)
		//{
		//	 pInstanceRecord->GetDT_INSTANCE_DATA().dwSuccessNum += 1;
		//}
    }
    return wErrCode;
}

UINT16	COnhookMgr::OnhookTower(CPlayer* poPlayer, DT_INSTANCE_ID stDT_INSTANCE_ID, UINT8 byMaxInstanceIdx, DT_ONHOOK_RESULT& stOnhookResult, DT_DATA_AFTER_ONHOOK& stAfterOnhookData)
{
    UINT16 wErrCode = ERR_CLIMB_TOWER_ONHOOK::ID_SUCCESS;
    CInstanceRecord *pInstanceRecord = NULL;
    pInstanceRecord = poPlayer->GetInstance().GetSingleInstaceRecord(stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    if(NULL == pInstanceRecord)
    {
        return ERR_CLIMB_TOWER_ONHOOK::ID_INSTANCE_NOT_BATTLED;
    }

    if(EIT_TOWER != stDT_INSTANCE_ID.wSceneIdx)
    {
        return ERR_CLIMB_TOWER_ONHOOK::ID_ONHOOK_UNLOCK;
    }

    if (IsToday(pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime))
    {
        return ERR_CLIMB_TOWER_ONHOOK::ID_CLIMBTOWER_ONE_PER_DAY_ERR;
    }

    //背包判断
    CBag& oBag = poPlayer->GetBag();
    if(0 == oBag.GetIdleSize())
    {
        return ERR_CLIMB_TOWER_ONHOOK::ID_BAG_FULL;
    }

    COnhook* poOnhook = poPlayer->GetOnhook();
	if (NULL == poOnhook)
	{
		return ERR_COMMON::ID_OTHER_ERR;
	}


    poOnhook->SetOnHookParam(1, &stDT_INSTANCE_ID);
    DT_ONHOOK_RCD_DATA& stDT_ONHOOK_RCD_DATA = poOnhook->GetDT_ONHOOK_RCD_DATA();

    //获取奖励
    UINT8 byRunTimes = 1;
    if(poOnhook->OnHookEncourage(byRunTimes))
    {
        wErrCode = ERR_CLIMB_TOWER_ONHOOK::ID_BAG_FULL;
    }

    //获取结果及玩家信息
    poOnhook->GetOnhookResult(stOnhookResult, stAfterOnhookData);

    //一次性返回了，挂机结束
    stDT_ONHOOK_RCD_DATA.byOnhookFlag = 0;//挂机结束
    //poPlayer->GetLocale().OnOnhookEnd();

    //如果是爬塔，要记录最后时间，用来限制次数
    if (NULL != pInstanceRecord && EIT_TOWER == stDT_INSTANCE_ID.wSceneIdx &&  ERR_CLIMB_TOWER_ONHOOK::ID_BAG_FULL != wErrCode)
    {
        //pInstanceRecord->GetDT_INSTANCE_DATA().wPassTimes ++;
        pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
    }
    stOnhookResult.byCurRunTimes = stDT_INSTANCE_ID.byInstanceIdx;
    stOnhookResult.byAllRunTimes = byMaxInstanceIdx;
    return wErrCode;
}


UINT16	COnhookMgr::OnhookTower(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 &byOnhookResultNum, DT_ONHOOK_RESULT astOnhookResult[MAX_INSTANCE_CLIMB_TOWER_NUM], DT_DATA_AFTER_ONHOOK &stAfterOnhookData)
{
    UINT16 wErrCode = ERR_CLIMB_TOWER_ONHOOK::ID_SUCCESS;
    STownInstanceProp *pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(wSceneIdx, byTownIdx);
    if (NULL == pstTownInstanceProp)
    {
        return ERR_CLIMB_TOWER_ONHOOK::ID_NOT_TOWER;
    }

    //计算当前塔副本处在第几层
    UINT64			qwLastPassTime	= 1;
    UINT8			byCurInstanceIdx	= 1;
    UINT8			byMaxInstanceIdx	= 1;
    UINT8			byInstanceIdx		= 1;
    BOOL				bPassInstance		=	FALSE;
    byOnhookResultNum					=	0;
    for (byInstanceIdx = 1; byInstanceIdx <= pstTownInstanceProp->m_mapInstance.size() && byInstanceIdx < MAX_INSTANCE_CLIMB_TOWER_NUM; byInstanceIdx++)
    {
        CInstanceRecord *pInstanceRecord = NULL;
        pInstanceRecord = poPlayer->GetInstance().GetSingleInstaceRecord(wSceneIdx, byTownIdx, byInstanceIdx);
        if(NULL == pInstanceRecord)
        {
            break;
        }
        if(pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime >= qwLastPassTime)
        {
            byCurInstanceIdx = byInstanceIdx;
            qwLastPassTime = pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime;
        }
        bPassInstance = TRUE;
        byMaxInstanceIdx = byInstanceIdx;
    }

    //对于处在第一层的玩家可以特殊处理。
    if (bPassInstance && (byCurInstanceIdx != byMaxInstanceIdx || 1 == byCurInstanceIdx))
    {
        //保证玩家从第一层开始
        if (0 == byCurInstanceIdx )
        {
            byCurInstanceIdx = 1;
        }

        for (byInstanceIdx = byCurInstanceIdx; byInstanceIdx <= byMaxInstanceIdx && byOnhookResultNum < MAX_INSTANCE_CLIMB_TOWER_NUM; byInstanceIdx++)
        {
            CIndex2InstancePropMapItr itr = pstTownInstanceProp->m_mapInstance.find(byInstanceIdx);
            if (itr == pstTownInstanceProp->m_mapInstance.end())
            {
                break;
            }

            DT_INSTANCE_ID stDT_INSTANCE_ID;
            stDT_INSTANCE_ID.wSceneIdx = wSceneIdx;
            stDT_INSTANCE_ID.byTownIdx = byTownIdx;
            stDT_INSTANCE_ID.byInstanceIdx = byInstanceIdx;

            DT_DATA_AFTER_ONHOOK stDT_DATA_AFTER_ONHOOK; //扫荡玩家后信息
            memset(&stDT_DATA_AFTER_ONHOOK, 0x00, sizeof(stDT_DATA_AFTER_ONHOOK));
            wErrCode = COnhookMgr::Instance()->OnhookTower(poPlayer, stDT_INSTANCE_ID, byMaxInstanceIdx, astOnhookResult[byOnhookResultNum], stDT_DATA_AFTER_ONHOOK);


            if (ERR_CLIMB_TOWER_ONHOOK::ID_CLIMBTOWER_ONE_PER_DAY_ERR == wErrCode)
            {
                continue;//如果当天已经打过刚检测下一层是否可挂。错误处理
            }

            if (ERR_CLIMB_TOWER_ONHOOK::ID_BAG_FULL == wErrCode)
            {
                byMaxInstanceIdx = byInstanceIdx - 1;//如果检测到背包满了，要修改最大的挂机层数
                break;
            }

            //对于有错误的，直接中止挂机退出
            if(ERR_CLIMB_TOWER_ONHOOK::ID_SUCCESS != wErrCode)
            {
                break;
            }
            byOnhookResultNum++;
            memcpy(&stAfterOnhookData, &stDT_DATA_AFTER_ONHOOK, sizeof(DT_DATA_AFTER_ONHOOK));
        }
    }
    else
    {
        wErrCode = ERR_CLIMB_TOWER_ONHOOK::ID_INSTANCE_NOT_BATTLED;
    }

    //如果检测到背包满了，要修改最大的挂机层数
    if (byOnhookResultNum > 0 && ERR_CLIMB_TOWER_ONHOOK::ID_BAG_FULL == wErrCode)
    {
        UINT8 byOnhookResultNum = 0;
        for (byOnhookResultNum = 0; byOnhookResultNum < byOnhookResultNum; byOnhookResultNum++)
        {
            astOnhookResult[byOnhookResultNum].byAllRunTimes = byMaxInstanceIdx;
        }
    }

    //检测当前错误码，如果有符合挂机次数的，当错误码“爬塔每天只能爬一次”，“该副本没有打过”
    if (byOnhookResultNum > 0 && (ERR_CLIMB_TOWER_ONHOOK::ID_CLIMBTOWER_ONE_PER_DAY_ERR == wErrCode || ERR_CLIMB_TOWER_ONHOOK::ID_INSTANCE_NOT_BATTLED == wErrCode))
    {
        wErrCode = ERR_CLIMB_TOWER_ONHOOK::ID_SUCCESS;
    }
    return wErrCode;
}


UINT16  COnhookMgr::OnhookElitInstance( CPlayer* poPlayer, DT_INSTANCE_ID stDT_INSTANCE_ID, UINT8 &byMaxOnHookNum,
                                        DT_ONHOOK_RESULT& stOnhookResult, DT_DATA_AFTER_ONHOOK& stAfterOnhookData )
{
    UINT16 wErrCode = ERR_ELITE_INSTANCE_ON_HOOK::ID_SUCCESS;
    CInstanceRecord *pInstanceRecord = NULL;
    pInstanceRecord = poPlayer->GetInstance().GetSingleInstaceRecord(stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    if(NULL == pInstanceRecord)
    {
        return ERR_ELITE_INSTANCE_ON_HOOK::ID_INSTANCE_NOT_BATTLED;
    }

    if(EIT_ELITE != stDT_INSTANCE_ID.wSceneIdx)
    {
        return ERR_ELITE_INSTANCE_ON_HOOK::ID_ONHOOK_UNLOCK;
    }

    //背包判断
    CBag& oBag = poPlayer->GetBag();
    if(0 == oBag.GetIdleSize())
    {
        return ERR_ELITE_INSTANCE_ON_HOOK::ID_BAG_FULL;
    }

    if( IsToday( pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime ) )
    {
        return ERR_ELITE_INSTANCE_ON_HOOK::ID_TODAY_ALREADY_BATTLED;
    }

    byMaxOnHookNum++;



	COnhook* poOnhook = poPlayer->GetOnhook();
	if (NULL == poOnhook)
	{
		return ERR_COMMON::ID_OTHER_ERR;
	}

    poOnhook->SetOnHookParam(1, &stDT_INSTANCE_ID);
    DT_ONHOOK_RCD_DATA& stDT_ONHOOK_RCD_DATA = poOnhook->GetDT_ONHOOK_RCD_DATA();

    //获取奖励
    UINT8 byRunTimes = 1;
    if(poOnhook->OnHookEncourage(byRunTimes))
    {
        wErrCode = ERR_ELITE_INSTANCE_ON_HOOK::ID_BAG_FULL;
    }

    //获取结果及玩家信息
    poOnhook->GetOnhookResult(stOnhookResult, stAfterOnhookData);

    //一次性返回了，挂机结束
    stDT_ONHOOK_RCD_DATA.byOnhookFlag = 0;//挂机结束
    //poPlayer->GetLocale().OnOnhookEnd();

    //要记录最后时间，用来限制次数
    if (NULL != pInstanceRecord && EIT_ELITE == stDT_INSTANCE_ID.wSceneIdx &&  ERR_ELITE_INSTANCE_ON_HOOK::ID_BAG_FULL != wErrCode)
    {
        //pInstanceRecord->GetDT_INSTANCE_DATA().wPassTimes ++;
        pInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
    }

    return wErrCode;
}

UINT16 COnhookMgr::OnhookElitInstanceByTownIndex( CPlayer *poPlayer, UINT8 byTownIndex, PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK *pstAck )
{
    UINT16   wErrCode = ERR_ELITE_INSTANCE_ON_HOOK::ID_SUCCESS; //错误码，非0为错误
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    UINT8 byMaxInstanceIdx = poPlayer->GetInstance().GetAlreadyBattleMaxIndex( byTownIndex );
    UINT8 byMaxCanHookInstance = 0;

    map<UINT32, DT_HERO_UPGRADE_DATA2> mapHeroUpgrade;
    DT_DATA_AFTER_ONHOOK stAfterOnhookData; //扫荡玩家后信息

    STownInstanceProp *pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_ELITE, byTownIndex);
    if (NULL == pstTownInstanceProp)
    {
        return ERR_ELITE_INSTANCE_ON_HOOK::ID_NOT_TOWER;
    }


    //玩家等级判断,vip等级判断
    if((poPlayer->GetLevel() < pstTownInstanceProp->wOnhookLevel) && (!CVipPropMgr::Instance()->CkOpenFunc(EVF_ONHOOKETELITEINSTANCE, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel)))
    {
        return ERR_ELITE_INSTANCE_ON_HOOK::ID_ONHOOK_UNLOCK;
    }

    for (UINT8 byInstanceIdx = 1; byInstanceIdx <= byMaxInstanceIdx && pstAck->byOnhookResultNum < MAX_ELITE_INSTANCE_TOWN_NUM; byInstanceIdx++)
    {
        memset(&stAfterOnhookData, 0x00, sizeof(stAfterOnhookData));

        DT_INSTANCE_ID stDT_INSTANCE_ID;
        stDT_INSTANCE_ID.wSceneIdx = EIT_ELITE;
        stDT_INSTANCE_ID.byTownIdx = byTownIndex;
        stDT_INSTANCE_ID.byInstanceIdx = byInstanceIdx;

        wErrCode = COnhookMgr::Instance()->OnhookElitInstance(poPlayer, stDT_INSTANCE_ID, byMaxCanHookInstance,
                   pstAck->astOnhookResult[pstAck->byOnhookResultNum], stAfterOnhookData);

        //已经不可打，跳出
        if(ERR_ELITE_INSTANCE_ON_HOOK::ID_INSTANCE_NOT_BATTLED == wErrCode)
        {
            break;
        }

        //跳过已经打过的
        if (ERR_ELITE_INSTANCE_ON_HOOK::ID_TODAY_ALREADY_BATTLED == wErrCode)
        {
            continue;
        }

        //其他错误，直接中止挂机退出
        if( ERR_ELITE_INSTANCE_ON_HOOK::ID_SUCCESS != wErrCode )
        {
            break;
        }
        pstAck->byOnhookResultNum++;

        //记录伙伴升级信息
        for( INT32 nIndex = 0; nIndex < stAfterOnhookData.byUpgradeHeroNum; ++nIndex )
        {
            UINT16 wHeroID = stAfterOnhookData.astHeroUpgradeInfo[nIndex].wHeroKindID;
            map<UINT32, DT_HERO_UPGRADE_DATA2>::iterator itrFind = mapHeroUpgrade.find( wHeroID );
            if( itrFind != mapHeroUpgrade.end() )
            {
                //记录升级起始信息，因为在挂机过程中可能会升N次级
                stAfterOnhookData.astHeroUpgradeInfo[nIndex].wStartLevel = itrFind->second.wStartLevel;
            }

            mapHeroUpgrade[ wHeroID ] = stAfterOnhookData.astHeroUpgradeInfo[nIndex];

            //记录起始等级
// 			if( mapHeroStartLevel.find( wHeroID )!=mapHeroStartLevel.end() )
// 			{
// 				continue;
// 			}
//
// 			mapHeroStartLevel[wHeroID]=stAfterOnhookData.astHeroUpgradeInfo[nIndex].wStartLevel;
        }

        memcpy(&pstAck->stAfterOnhookData, &stAfterOnhookData, sizeof(stAfterOnhookData));
    }

    //DBG_INFO( _SDT( "elite instance onhook result:" ) );
    for( map<UINT32, DT_HERO_UPGRADE_DATA2>::iterator itr = mapHeroUpgrade.begin(); itr != mapHeroUpgrade.end(); ++itr )
    {
        DT_HERO_UPGRADE_DATA2 &data = pstAck->stAfterOnhookData.astHeroUpgradeInfo[pstAck->stAfterOnhookData.byUpgradeHeroNum];

        data = itr->second;
        pstAck->stAfterOnhookData.byUpgradeHeroNum++;

        //输出用于调试
        //DBG_INFO( _SDT( "hero:%u, start level:%u, cur level:%u" ), data.wHeroKindID, data.wStartLevel, data.wCurLevel );

        //pstAck->stAfterOnhookData.astHeroUpgradeInfo[pstAck->stAfterOnhookData.byUpgradeHeroNum++]=itr->second;
    }

    pstAck->wAllOnhookNum = byMaxCanHookInstance;
    poPlayer->GetInstance().GetEliteInstanceRecordData( pstAck->stInstanceData, pstAck->stInstanceScoreData, byTownIndex);

    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    pstAck->stInstanceData.wRemainResetNum = poPlayer->GetVipExtData().GetRemainResetEliteInstanceNum( byTownIndex );

    //有些挂机后的数据在多次挂机后不正确，需要在此重新赋值供客户端使用
    pstAck->stAfterOnhookData.qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->stAfterOnhookData.qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->stAfterOnhookData.qwCurStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
    poPlayer->GetPhystrength().GetPhyStrengthInfo( pstAck->stAfterOnhookData.stCurPhyStrengthInfo );
    poPlayer->GetExperienceInfo( pstAck->stAfterOnhookData.stExperienceInfo );

    //如果不是背包满，并且有成功的次数，那么则设为成功
    if( wErrCode != ERR_ELITE_INSTANCE_ON_HOOK::ID_BAG_FULL && wErrCode != ERR_ELITE_INSTANCE_ON_HOOK::ID_SUCCESS && pstAck->byOnhookResultNum )
    {
        wErrCode =  ERR_ELITE_INSTANCE_ON_HOOK::ID_SUCCESS;
    }

    return wErrCode;
}

