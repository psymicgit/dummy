#include "onhook.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <logic/hero/hero.h>
#include <common/client/errdef.h>
#include <logic/monster/monstermgr.h>
#include <logic/task/taskmgr.h>
#include <logic/other/state.h>
#include <logic/instance/instance.h>
#include <sdloggerimpl.h>
#include "logic/other/phystrength.h"

IMPLEMENT_CLASS_POOL_MGR(COnhook)

COnhook::COnhook()
{
    memset(&m_stOnhookRcdData, 0 , sizeof(m_stOnhookRcdData));

    pvecWholeMonsterProp = NULL;
    m_mapOnhookEncourageItem.clear();
    m_mapOnhookUpgrageHero.clear();
    //m_listOnHookResultList.clear();
    m_wLastOnhookResultReqPassSec = 0;
}

COnhook::~COnhook()
{

}

BOOL COnhook::Init(DT_ONHOOK_RCD_DATA& stOnhookRcdData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
	memcpy(&m_stOnhookRcdData, &stOnhookRcdData, sizeof(DT_ONHOOK_RCD_DATA));
    //去掉挂机记录
    /*
    memcpy(&m_stOnhookRcdData, &stOnhookRcdData, sizeof(DT_ONHOOK_RCD_DATA));

    DT_INSTANCE_ID& stOnhookInstanceID = stOnhookRcdData.stOnhookInstanceID;

    DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT = stOnhookRcdData.stOnhookResult;
    if((1 == m_stOnhookRcdData.byOnhookFlag) &&
    	(stDT_ONHOOK_RESULT.byCurRunTimes < stDT_ONHOOK_RESULT.byAllRunTimes) &&
    	((0 != stOnhookInstanceID.wSceneIdx) && (0 != stOnhookInstanceID.byTownIdx) && (0 != stOnhookInstanceID.byInstanceIdx)))
    {
    	m_stOnhookRcdData.qwLastOnhookResultReqTime = SDTimeSecs();//时间重新开始
    	m_mapOnhookEncourageItem.clear();
    	UINT8 byItemNum = stDT_ONHOOK_RESULT.byItemNum;
    	for(UINT8 byIdx = 0; ((byIdx < byItemNum) && (byIdx < MAX_ENCOURAGE_ITEM_KIND_NUM)); byIdx++)
    	{
    		DT_ITEM_DATA* pstDT_ITEM_DATA = &stDT_ONHOOK_RESULT.astItemInfoList[byIdx];
    		m_mapOnhookEncourageItem[pstDT_ITEM_DATA->wKindID] = pstDT_ITEM_DATA;
    	}

    	InitHeroMonster();
    	//状态回调
    	m_poOwner->GetState().OnOnhook();
    }
    */

    return TRUE;
}


VOID COnhook::SetOnHookParam(UINT8 byRunTimes, DT_INSTANCE_ID* pstDT_INSTANCE_ID)
{
    memset(&m_stOnhookRcdData, 0, sizeof(m_stOnhookRcdData));
    DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT = m_stOnhookRcdData.stOnhookResult;
    stDT_ONHOOK_RESULT.byCurRunTimes = 0;
    stDT_ONHOOK_RESULT.byAllRunTimes = byRunTimes;
    m_mapOnhookEncourageItem.clear();
    memcpy(&m_stOnhookRcdData.stOnhookInstanceID, pstDT_INSTANCE_ID, sizeof(DT_INSTANCE_ID));
    m_stOnhookRcdData.byOnhookFlag = 1;
    m_stOnhookRcdData.qwLastOnhookResultReqTime = SDTimeSecs();//用于判断是否达到扫荡时间
    m_wLastOnhookResultReqPassSec = 0;
    InitHeroMonster();
}

VOID COnhook::InitHeroMonster()
{
    pvecWholeMonsterProp = NULL;
    //初始化武将升级信息
    m_mapOnhookUpgrageHero.clear();
    CFormationHeroShmemMap& mapFormationHero = m_poOwner->GetFormationHeroMap();
    CHero* poHero = mapFormationHero.GetFistData();
    while(poHero)
    {
        SOnhookHeroUpgrage stOnhookHeroUpgrage;
        stOnhookHeroUpgrage.stDT_HERO_UPGRADE_DATA2.wHeroKindID = poHero->GetDT_HERO_BASE_DATA().wKindID;
        stOnhookHeroUpgrage.stDT_HERO_UPGRADE_DATA2.wStartLevel = poHero->GetDT_HERO_BASE_DATA().wLevel;
        m_mapOnhookUpgrageHero[stOnhookHeroUpgrage.stDT_HERO_UPGRADE_DATA2.wHeroKindID] = stOnhookHeroUpgrage;

        poHero = mapFormationHero.GetNextData();
    }

    DT_INSTANCE_ID& stDT_INSTANCE_ID = m_stOnhookRcdData.stOnhookInstanceID;
    SInstanceProp* pstOnhookInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stDT_INSTANCE_ID.wSceneIdx, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    if(pstOnhookInstanceProp)
    {
        pvecWholeMonsterProp = &(pstOnhookInstanceProp->vecWholeMonsterProp);
    }
}



//副本挂机奖励回调，跑一次副本回调一次
BOOL COnhook::OnHookEncourage(UINT8 &byRunTimes )
{
    DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT = m_stOnhookRcdData.stOnhookResult;
    stDT_ONHOOK_RESULT.byCurRunTimes = byRunTimes;
    if(stDT_ONHOOK_RESULT.byCurRunTimes >= stDT_ONHOOK_RESULT.byAllRunTimes)
    {
        m_stOnhookRcdData.byOnhookFlag = 0;
    }

    DT_INSTANCE_ID& stOnhookInstanceID = m_stOnhookRcdData.stOnhookInstanceID;

    //奖励
    BOOL bBagFull = GetHookEncourage(byRunTimes);

    stDT_ONHOOK_RESULT.byCurRunTimes = byRunTimes;

    //任务杀怪回调

    for (UINT8 byIdx = 0; byIdx < byRunTimes; byIdx++)
    {
        if(NULL == pvecWholeMonsterProp)
        {
            break;
        }
        UINT8 bySize = pvecWholeMonsterProp->size();
        for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
        {
            SWholeMonsterProp* pstWholeMonsterProp = (*pvecWholeMonsterProp)[byIdx];
            if(NULL == pstWholeMonsterProp)
            {
                break;
            }
            if(pstWholeMonsterProp->vecMonster.size() > 0)
            {
                CTaskMgr::Instance()->OnKillMonsterInstance(m_poOwner, stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx, pstWholeMonsterProp->vecMonster[0].wMonsterID);
            }
        }

        //通关任务回调
        CTaskMgr::Instance()->OnPassInstance(m_poOwner, stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx);
    }

    return bBagFull;
}

//获取当前挂机结果
VOID COnhook::GetOnhookResult(DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT, DT_DATA_AFTER_ONHOOK& stAfterOnhookData)
{
    memset(&stDT_ONHOOK_RESULT, 0, sizeof(stDT_ONHOOK_RESULT));
    memset(&stAfterOnhookData, 0, sizeof(stAfterOnhookData));

    //其他信息
    memcpy(&stDT_ONHOOK_RESULT, &m_stOnhookRcdData.stOnhookResult, sizeof(DT_ONHOOK_RESULT));

    //玩家当前信息

    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = m_poOwner->GetDT_PLAYER_BASE_DATA();
    stAfterOnhookData.qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    stAfterOnhookData.qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    stAfterOnhookData.qwCurStory = stDT_PLAYER_BASE_DATA.qwStory;
    stAfterOnhookData.qwCurBlueGas = stDT_PLAYER_BASE_DATA.qwBlueGas;

    m_poOwner->GetPhystrength().GetPhyStrengthInfo(stAfterOnhookData.stCurPhyStrengthInfo);
    //赋值道具武将升级信息
    CFormationHeroShmemMap& mapFormationHero = m_poOwner->GetFormationHeroMap();
    CHero* poHero = mapFormationHero.GetFistData();
    while(poHero)
    {
        SOnhookHeroUpgrage& stOnhookHeroUpgrage = m_mapOnhookUpgrageHero[poHero->GetDT_HERO_BASE_DATA().wKindID];
        DT_HERO_UPGRADE_DATA2& stOld  = stOnhookHeroUpgrage.stDT_HERO_UPGRADE_DATA2;
        DT_HERO_UPGRADE_DATA2 stNow;
        poHero->GetUpgrageInfo2(stNow);
        //升级，重新赋值升级信息
        if(1 == stNow.byUpgradeFlag)
        {
            stOnhookHeroUpgrage.bHaveUpgrageFlag = TRUE;
            stNow.wStartLevel = stOld.wStartLevel; //使用最开始的等级
            memcpy(&stOld, &stNow, sizeof(stOld));
        }
        if((TRUE == stOnhookHeroUpgrage.bHaveUpgrageFlag) && (stOld.wCurLevel > stOld.wStartLevel)) //
        {
            memcpy(&(stAfterOnhookData.astHeroUpgradeInfo[stAfterOnhookData.byUpgradeHeroNum++]), &stOld, sizeof(DT_HERO_UPGRADE_DATA2));
            stOld.byUpgradeFlag = 0;//清除升级标志
        }

        poHero = mapFormationHero.GetNextData();
    }
    m_poOwner->GetExperienceInfo(stAfterOnhookData.stExperienceInfo);
    stAfterOnhookData.wUnlockEquipLevel = m_poOwner->GetState().GetDT_STATE_DATA().wUnlockEquipLevel;
    stAfterOnhookData.stUnlockScienceList = m_poOwner->GetState().GetDT_STATE_DATA().stUnlockScienceList;
    m_poOwner->GetState().GetDT_STATE_DATA().wUnlockEquipLevel = 0;
    memset( &stAfterOnhookData.stUnlockScienceList, 0, sizeof( stAfterOnhookData.stUnlockScienceList ) );
    m_poOwner->GetUnlockNewInstance(stAfterOnhookData.byUnlockNewInstanceFlag, stAfterOnhookData.byHaveNewInstanceFlag, stAfterOnhookData.astUnlockCommonInstanceID[0], stAfterOnhookData.astNewCommonInstanceID[0]);
}

BOOL COnhook::GetHookEncourage(UINT8 &byRunTimes)
{
    BOOL bBagFull = FALSE;
    DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT = m_stOnhookRcdData.stOnhookResult;
    DT_INSTANCE_ID& stOnhookInstanceID = m_stOnhookRcdData.stOnhookInstanceID;
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        return bBagFull;
    }

    //打开宝箱的次数
    for (UINT8 byIdx = 0; byIdx < byRunTimes; byIdx++)
    {
        //副本通关宝箱奖励
        DT_PASS_ENCOURAGE_BOX_DATA stPassEncourageBox;
        memset(&stPassEncourageBox, 0, sizeof(stPassEncourageBox));
        m_poOwner->GetInstance().RatePassBoxEnc(stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx, stPassEncourageBox, TRUE);
        DT_PASS_ENCOURAGE_BOX_DATA stDT_PASS_ENCOURAGE_BOX_DATA;
        memset(&stDT_PASS_ENCOURAGE_BOX_DATA, 0, sizeof(stDT_PASS_ENCOURAGE_BOX_DATA));
        UINT16 wErrCode = m_poOwner->GetInstance().PassBoxEnc(stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx, stDT_PASS_ENCOURAGE_BOX_DATA);
        if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_BAG_FULL == wErrCode)
        {
            bBagFull = TRUE;
            byRunTimes = byIdx;
            break;
        }
        if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_SUCCESS == wErrCode)
        {
            for(UINT8 byGroupNum = 0; byGroupNum < MAX_ENCOURAGE_BOX_GROUP_NUM; byGroupNum++)
            {
                DT_PASS_ENCOURAGE_BOX_GROUP_DATA& stBoxGroupInfo = stDT_PASS_ENCOURAGE_BOX_DATA.astBoxGroupInfo[byGroupNum];
                stDT_ONHOOK_RESULT.dwCoin += stBoxGroupInfo.dwCoin;
                stDT_ONHOOK_RESULT.dwGold += stBoxGroupInfo.dwGold;
                stDT_ONHOOK_RESULT.dwBlueGas += stBoxGroupInfo.dwBlueGas;
                if((0 != stBoxGroupInfo.byHitItemIdx) && (stBoxGroupInfo.stItemInfo.byItemNum >= 1))
                {
                    DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stBoxGroupInfo.stItemInfo.astItemList[0];
                    CKindID2ItemDataMapItr itr = m_mapOnhookEncourageItem.find(stDT_ITEM_DATA_CLI.wKindID);
                    if(m_mapOnhookEncourageItem.end() != itr)
                    {
                        DT_ITEM_DATA* pstItem = itr->second;
                        pstItem->wNum += stDT_ITEM_DATA_CLI.wPileNum;
                    }
                    else
                    {
                        DT_ITEM_DATA* pstItem = &(stDT_ONHOOK_RESULT.astItemInfoList[stDT_ONHOOK_RESULT.byItemNum++]);
                        pstItem->wKindID = stDT_ITEM_DATA_CLI.wKindID;
                        pstItem->wNum = stDT_ITEM_DATA_CLI.wPileNum;
                        m_mapOnhookEncourageItem[stDT_ITEM_DATA_CLI.wKindID] = pstItem;
                    }
                }
            }
        }
    }

    //副本通关奖励
	//DT_PASS_ENCOURAGE_DATA stDT_PASS_ENCOURAGE_DATA = {0};
 //   m_poOwner->GetInstance().PassEncourage(stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx, byRunTimes, &stDT_PASS_ENCOURAGE_DATA);

 //   //记录奖励
 //   stDT_ONHOOK_RESULT.dwCoin += stDT_PASS_ENCOURAGE_DATA.dwCoin;
 //   stDT_ONHOOK_RESULT.dwStory += stDT_PASS_ENCOURAGE_DATA.dwStory;

 //   //如果是爬塔，没有经验奖励
 //   if (EIT_TOWER != stOnhookInstanceID.wSceneIdx)
 //   {
 //       stDT_ONHOOK_RESULT.dwExperience += stDT_PASS_ENCOURAGE_DATA.dwExperience;
 //   }

    UINT8 byBattleNum = pstInstanceProp->vecWholeMonsterProp.size();
    DT_BATTLE_ENCOURAGE_DATA stBattleEncourage;
    memset(&stBattleEncourage, 0, sizeof(stBattleEncourage));
    for(UINT8 byIdx = 1; byIdx <= byBattleNum; byIdx++)
    {
        CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stOnhookInstanceID.wSceneIdx, stOnhookInstanceID.byTownIdx, stOnhookInstanceID.byInstanceIdx, byIdx);
        if(NULL != poMonsterGroup)
        {
            //如果是爬塔，没有奖励
            if (EIT_TOWER != stOnhookInstanceID.wSceneIdx)
            {
                poMonsterGroup->Encourage(m_poOwner, FALSE, byRunTimes);
            }
        }
        else
        {
            //异常
            SYS_CRITICAL(_SDT("[%s: %d]: poMonsterGroup is NULL!"), MSG_MARK);
        }
    }
    m_poOwner->GetBattleEncourage(stBattleEncourage);
    //记录奖励
    stDT_ONHOOK_RESULT.dwCoin += stBattleEncourage.dwCoin;
    stDT_ONHOOK_RESULT.dwStory += stBattleEncourage.dwStory;
    stDT_ONHOOK_RESULT.dwExperience += stBattleEncourage.dwExperience;

    return bBagFull;
}