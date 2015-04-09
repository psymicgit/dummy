
#include "instancemgr.h"
#include "instancepropmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/player/playermgr.h>
#include <net/db/dbpktbuilder.h>
#include <logic/other/singleprammgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/monster/monstergroup.h>
#include <common/client/errdef.h>
#include <logic/base/basedefine.h>
#include <logic/player/player.h>
#include "logic/other/fewparam.h"
#include "logic/other/locale.h"
#include "logic/monster/monstermgr.h"
#include "logic/pet/playerpet.h"
#include "logic/other/state.h"
IMPLEMENT_SINGLETON(CInstanceMgr)


CInstanceMgr::CInstanceMgr()
{

}

CInstanceMgr::~CInstanceMgr()
{
    CInstancePropMgr::DestroyInstance();
}

BOOL CInstanceMgr::Init()
{
    //读取配置文件

    // 初始化CInstancePropMgr管理器
    if(FALSE == CInstancePropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CInstancePropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }


    return TRUE;

}


VOID CInstanceMgr::UnInit()
{
    CInstancePropMgr::Instance()->UnInit();

}


CInstanceRecord* CInstanceMgr::CreatInstanceRecord(UINT32 dwPlayerID, UINT16 wSceneID, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    CInstanceRecord* poInstanceRecord = m_oInstanceRecordShmemObjMgr.Alloc();
    if(NULL == poInstanceRecord)
    {
        return NULL;
    }

    m_mapID2InstanceRecord[CLogicFunc::GetInstanceIdxKey(dwPlayerID, wSceneID, byTownIdx, byInstanceIdx)] = poInstanceRecord;

    return poInstanceRecord;
}


VOID CInstanceMgr::RemoveInsanceRecord(UINT32 dwPlayerID, UINT16 wSceneID, UINT8 byTownIdx, UINT8 byInstanceIdx)
{

    CID2InstanceRecordMapItr itr = m_mapID2InstanceRecord.find(CLogicFunc::GetInstanceIdxKey(dwPlayerID, wSceneID, byTownIdx, byInstanceIdx));
    if (m_mapID2InstanceRecord.end() != itr)
    {
        CInstanceRecord* poInstanceRecord = itr->second;
        m_oInstanceRecordShmemObjMgr.Free(poInstanceRecord);
        m_mapID2InstanceRecord.erase(itr);
    }
}

UINT16 CInstanceMgr::InstanceBattle(CPlayer* poMyself, CMonsterGroup* poEnemy,  UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx,
                                    UINT8 byBattleIdx, DT_BATTLE_DATA5& stDT_BATTLE_DATA, UINT8& byPassFlag, DT_UNLOCK_DATA& stUnlockInfo,
                                    DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,
                                    DT_PASS_ENCOURAGE_DATA& stPassEncourage, DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage,
                                    DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bGuide)
{
    if(bGuide)
    {
        ChangeMonsterAttr(poEnemy);
    }

    //战斗后血量信息
    DT_BATTLE_OBJ_GROUP_HP stAfterMyselfInfo;

    if(!CBattleMgr::Instance()->Battle4(poMyself, poEnemy, stDT_BATTLE_DATA, FALSE, FALSE, &stAfterMyselfInfo))
    {
        if(bGuide)
        {
            RecoverMonsterAttr(poEnemy);
        }
        RETURN_OTHER_ERR;
    }
    if(bGuide)
    {
        stDT_BATTLE_DATA.byBattleResult = EBT_SUCCESS;
        RecoverMonsterAttr(poEnemy);
    }


    BOOL bInstancePass = FALSE; //是否通过战斗
    //得分。
    stDT_BATTLE_DATA.byScore = 0;
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(EBT_SUCCESS == stDT_BATTLE_DATA.byBattleResult)
    {
        //通过得一分
        stDT_BATTLE_DATA.byScore++;

        if(pstInstanceProp)
        {
            //检测是否通过副本
            if((byBattleIdx != 0) && (byBattleIdx == pstInstanceProp->vecWholeMonsterProp.size()))
            {
                bInstancePass = TRUE;
                stDT_BATTLE_DATA.byBattleResult = EBT_INSTANCE_PASS;//????
                byPassFlag = 1;
            }
            //检测战斗回合数
            if(stDT_BATTLE_DATA.byBattleRoundNum <= pstInstanceProp->byRoundNum)
            {
                stDT_BATTLE_DATA.byScore++;
            }
        }

        //计算血量百分比
        UINT8	byPercentageHp = 0;//血量百分比
        UINT32	dwBeforeHP = 0;
        for (UINT8 byObjNum = 0; byObjNum < MAX_FORMATION_IDX_NUM; byObjNum++)
        {
            dwBeforeHP += stDT_BATTLE_DATA.stMyselfInfo.astBattleObjInfo[byObjNum].stBattleAttr.dwHP;
        }

        UINT32 dwAfterHP = stAfterMyselfInfo.CalcTotalHP();        

        try
        {
            byPercentageHp = (UINT8)((dwAfterHP * 100) /  dwBeforeHP);
        }
        catch(...)
        {
            byPercentageHp = 0;
        }

        //检测血量百分比
        if(byPercentageHp >= pstInstanceProp->byPercentageHp)
        {
            stDT_BATTLE_DATA.byScore++;
        }
        //通过
        PassRecord(poMyself,  poEnemy, wSceneIdx, byTownIdx, byInstanceIdx, byBattleIdx, stDT_BATTLE_DATA.byScore, bInstancePass, stUnlockInfo, stBattleEncourage, stAfterBattleEncourage,
                   stPassEncourage, stAfterPassEncourage, stPassEncourageBox);
    }
    else if(EBT_FAILED == stDT_BATTLE_DATA.byBattleResult) //失败，也做失败记录
    {
            poMyself->GetInstance().AddFailedRecord(wSceneIdx, byTownIdx, byInstanceIdx);
    }

    return ERR_BATTLE::ID_SUCCESS;
}


UINT16 CInstanceMgr::InstanceBattle2(CPlayer* poMyself, CMonsterGroup* poEnemy,  UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx,
    UINT8 byBattleIdx, DT_BATTLE_DATA3& stDT_BATTLE_DATA, UINT8& byPassFlag, DT_UNLOCK_DATA& stUnlockInfo,
    DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,
    DT_PASS_ENCOURAGE_DATA& stPassEncourage, DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage,
    DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bGuide)
{
    if(bGuide)
    {
        ChangeMonsterAttr(poEnemy);
    }

    //战斗后血量信息
    DT_BATTLE_OBJ_GROUP_HP stAfterMyselfInfo;

    if(!CBattleMgr::Instance()->Battle2(poMyself, poEnemy, stDT_BATTLE_DATA, FALSE, FALSE, &stAfterMyselfInfo))
    {
        if(bGuide)
        {
            RecoverMonsterAttr(poEnemy);
        }
        RETURN_OTHER_ERR;
    }
    if(bGuide)
    {
        stDT_BATTLE_DATA.byBattleResult = EBT_SUCCESS;
        RecoverMonsterAttr(poEnemy);
    }


    BOOL bInstancePass = FALSE; //是否通过战斗
    //得分。
    stDT_BATTLE_DATA.byScore = 0;
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(EBT_SUCCESS == stDT_BATTLE_DATA.byBattleResult)
    {
        //通过得一分
        stDT_BATTLE_DATA.byScore++;

        if(pstInstanceProp)
        {
            //检测是否通过副本
            if((byBattleIdx != 0) && (byBattleIdx == pstInstanceProp->vecWholeMonsterProp.size()))
            {
                bInstancePass = TRUE;
                stDT_BATTLE_DATA.byBattleResult = EBT_INSTANCE_PASS;//????
                byPassFlag = 1;
            }
            //检测战斗回合数
            if(stDT_BATTLE_DATA.byBattleRoundNum <= pstInstanceProp->byRoundNum)
            {
                stDT_BATTLE_DATA.byScore++;
            }
        }

        //计算血量百分比
        UINT8	byPercentageHp = 0;//血量百分比
        UINT32	dwBeforeHP = 0;
        for (UINT8 byObjNum = 0; byObjNum < MAX_FORMATION_IDX_NUM; byObjNum++)
        {
            dwBeforeHP += stDT_BATTLE_DATA.stMyselfInfo.astBattleObjInfo[byObjNum].stBattleAttr.dwHP;
        }

        UINT32 dwAfterHP = stAfterMyselfInfo.CalcTotalHP();

        try
        {
            byPercentageHp = (UINT8)((dwAfterHP * 100) /  dwBeforeHP);
        }
        catch(...)
        {
            byPercentageHp = 0;
        }

        //检测血量百分比
        if(byPercentageHp >= pstInstanceProp->byPercentageHp)
        {
            stDT_BATTLE_DATA.byScore++;
        }
        //通过
        PassRecord(poMyself,  poEnemy, wSceneIdx, byTownIdx, byInstanceIdx, byBattleIdx, stDT_BATTLE_DATA.byScore, bInstancePass, stUnlockInfo, stBattleEncourage, stAfterBattleEncourage,
            stPassEncourage, stAfterPassEncourage, stPassEncourageBox);
    }
    else if(EBT_FAILED == stDT_BATTLE_DATA.byBattleResult) //失败，也做失败记录
    {
        poMyself->GetInstance().AddFailedRecord(wSceneIdx, byTownIdx, byInstanceIdx);
    }

    return ERR_BATTLE::ID_SUCCESS;
}

UINT16	CInstanceMgr::InstanceBattle3(CPlayer* poMyself, CMonsterGroup* poEnemy,  UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, 
	UINT8 byBattleIdx, DT_BATTLE_DATA4& stDT_BATTLE_DATA, UINT8& byPassFlag, DT_UNLOCK_DATA& stUnlockInfo,
	DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,
	DT_PASS_ENCOURAGE_DATA& stPassEncourage,DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage, 
	DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bGuide )
{
	if(bGuide)
	{
		ChangeMonsterAttr(poEnemy);
	}

	//战斗后血量信息
	DT_BATTLE_OBJ_GROUP_HP stAfterMyselfInfo;

	if(!CBattleMgr::Instance()->Battle3(poMyself, poEnemy, stDT_BATTLE_DATA, FALSE, FALSE, &stAfterMyselfInfo))
	{
		if(bGuide)
		{
			RecoverMonsterAttr(poEnemy);
		}
		RETURN_OTHER_ERR;
	}
	if(bGuide)
	{
		stDT_BATTLE_DATA.byBattleResult = EBT_SUCCESS;
		RecoverMonsterAttr(poEnemy);
	}


	BOOL bInstancePass = FALSE; //是否通过战斗
	//得分。
	stDT_BATTLE_DATA.byScore = 0;
	SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
	if(EBT_SUCCESS == stDT_BATTLE_DATA.byBattleResult)
	{
		//通过得一分
		stDT_BATTLE_DATA.byScore++;

		if(pstInstanceProp)
		{
			//检测是否通过副本
			if((byBattleIdx != 0) && (byBattleIdx == pstInstanceProp->vecWholeMonsterProp.size()))
			{
				bInstancePass = TRUE;
				stDT_BATTLE_DATA.byBattleResult = EBT_INSTANCE_PASS;//????
				byPassFlag = 1;
			}
			//检测战斗回合数
			if(stDT_BATTLE_DATA.byBattleRoundNum <= pstInstanceProp->byRoundNum)
			{
				stDT_BATTLE_DATA.byScore++;
			}
		}

		//计算血量百分比
		UINT8	byPercentageHp = 0;//血量百分比
		UINT32	dwBeforeHP = 0;
		for (UINT8 byObjNum = 0; byObjNum < MAX_FORMATION_IDX_NUM; byObjNum++)
		{
			dwBeforeHP += stDT_BATTLE_DATA.stMyselfInfo.astBattleObjInfo[byObjNum].stBattleAttr.dwHP;
		}

		UINT32 dwAfterHP = stAfterMyselfInfo.CalcTotalHP();

		try
		{
			byPercentageHp = (UINT8)((dwAfterHP * 100) /  dwBeforeHP);
		}
		catch(...)
		{
			byPercentageHp = 0;
		}

		//检测血量百分比
		if(byPercentageHp >= pstInstanceProp->byPercentageHp)
		{
			stDT_BATTLE_DATA.byScore++;
		}
		//通过
		PassRecord(poMyself,  poEnemy, wSceneIdx, byTownIdx, byInstanceIdx, byBattleIdx, stDT_BATTLE_DATA.byScore, bInstancePass, stUnlockInfo, stBattleEncourage, stAfterBattleEncourage,
			stPassEncourage, stAfterPassEncourage, stPassEncourageBox);
	}
	else if(EBT_FAILED == stDT_BATTLE_DATA.byBattleResult) //失败，也做失败记录
	{
		poMyself->GetInstance().AddFailedRecord(wSceneIdx, byTownIdx, byInstanceIdx);
	}

	return ERR_BATTLE::ID_SUCCESS;
}
VOID CInstanceMgr::ChangeMonsterAttr(CMonsterGroup* poEnemy)
{
    m_mapOldMonsterAttr.clear();
    for(UINT8 byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM; byIdx++)
    {
        CBattleAttrObj* poObj = poEnemy->GetFormationObj(byIdx);
        if(poObj)
        {
            DT_BATTLE_BASE_ATTRIBUTE& stTmp = poObj->GetDT_BATTLE_ATTRIBUTE();
            m_mapOldMonsterAttr[byIdx] = stTmp;
            stTmp.qwHP = stTmp.qwHP << 1;
            stTmp.dwAttack = stTmp.dwAttack << 1;
        }
    }
}

VOID CInstanceMgr::RecoverMonsterAttr(CMonsterGroup* poEnemy)
{
    for(map<UINT8, DT_BATTLE_BASE_ATTRIBUTE>::iterator itr = m_mapOldMonsterAttr.begin(); itr != m_mapOldMonsterAttr.end(); itr++)
    {
        CBattleAttrObj* poObj = poEnemy->GetFormationObj(itr->first);
        if(poObj)
        {
            DT_BATTLE_BASE_ATTRIBUTE& stTmp = poObj->GetDT_BATTLE_ATTRIBUTE();
            stTmp.qwHP = itr->second.qwHP;
            stTmp.dwAttack = itr->second.dwAttack;
        }
    }

    m_mapOldMonsterAttr.clear();
}


VOID CInstanceMgr::PassRecord(CPlayer* poMyself, CMonsterGroup* poEnemy, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIdx, UINT8 byScore, BOOL bInstancePass,
                              DT_UNLOCK_DATA& stUnlockInfo, DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage, DT_PASS_ENCOURAGE_DATA& stPassEncourage,
                              DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage, DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox)
{
    if((NULL == poMyself) || (NULL == poEnemy))
    {
        return;
    }

    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        return;
    }

    //开始战斗 ，同时计算获取的奖励。
    poMyself->GetInstance().BattlePass(poEnemy, byBattleIdx);
    //获取奖励信息
    poMyself->GetBattleEncourage(stBattleEncourage, &stAfterBattleEncourage);

    //是否通过
    if(bInstancePass)
    {
		if ( EIT_OUT_BOUND != wSceneIdx)
		{
			//记录通关副本
			poMyself->GetInstance().ExitInstanceWhenPass(wSceneIdx, byTownIdx, byInstanceIdx, byScore);
		}
        
        //获取通关奖励
        poMyself->GetInstance().PassEncourage(wSceneIdx, byTownIdx, byInstanceIdx, 1, &stPassEncourage, &stAfterPassEncourage);
        //打开宝箱
        poMyself->GetInstance().RatePassBoxEnc(wSceneIdx, byTownIdx, byInstanceIdx, stPassEncourageBox);

        //普通副本解锁相关信息
        if(EIT_COMMON == wSceneIdx)
        {
            poMyself->GetInstance().DoUnlock(wSceneIdx, byTownIdx, byInstanceIdx, stUnlockInfo.byUnlockBuildKindID, stUnlockInfo.awUnlockHeroID);
        }
    }
}

UINT16 CInstanceMgr::OpenNeiDanInstance(CPlayer* poPlayer, PKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_OPEN_NEIDAN_ALL_INSTANCE::ID_IS_NOT_OPEN;
	}
	if ( NULL == poPlayer->GetBuild(EBK_NEIDAN_INSTANCE) )
	{
		return ERR_OPEN_NEIDAN_ALL_INSTANCE::ID_IS_NOT_OPEN;
	}
	CInstance& oInstance = poPlayer->GetInstance();
	//资源信息
	UINT8 byResNum = 0;
	stAck.stResInfo.astPlayerRes[byResNum].byResKind = ACTE_COIN;
	stAck.stResInfo.astPlayerRes[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	byResNum++;
	stAck.stResInfo.astPlayerRes[byResNum].byResKind = ACTE_GOLD;
	stAck.stResInfo.astPlayerRes[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	byResNum++;
	stAck.stResInfo.byResNum = byResNum;
	//今天已挑战次数
	stAck.wCanBattleNum = oInstance.GetNeiDanCanBattleNum();
	//锁定副本
	stAck.stSelectInstance = oInstance.GetMaxNaiDanInstanceID();
	if ( 0 == stAck.stSelectInstance.wSceneIdx)
	{
		stAck.stSelectInstance.wSceneIdx = EIT_NEIDAN;
		stAck.stSelectInstance.byTownIdx = 1;
	}
	stAck.stSelectInstance.byInstanceIdx = 1;
	
	//副本信息
	CInstancePropMgr::Instance()->GetNeiDanAllInstance(poPlayer, stAck.stInstanceLst);
	return ERR_OPEN_NEIDAN_ALL_INSTANCE::ID_SUCCESS;
}

UINT16 CInstanceMgr::NeiDanBattle(CPlayer* poPlayer, PKT_CLIGS_NEIDAN_BATTLE_REQ& stReq, PKT_CLIGS_NEIDAN_BATTLE_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_NEIDAN_BATTLE::ID_IS_NOT_OPEN;
	}

	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ERR_NEIDAN_BATTLE::ID_IS_NOT_OPEN;
	}

	if ( !poPlayer->GetBuild(EBK_NEIDAN_INSTANCE))
	{
		return ERR_NEIDAN_BATTLE::ID_IS_NOT_OPEN;
	}
	

	if ( EIT_NEIDAN != stReq.stInstanceID.wSceneIdx)
	{
		return ERR_NEIDAN_BATTLE::ID_IS_NOT_OPEN;
	}
	CInstance& oInstance = poPlayer->GetInstance();
	UINT16 wErrCode = oInstance.CkCanEnter( stReq.stInstanceID.wSceneIdx, stReq.stInstanceID.byTownIdx, stReq.stInstanceID.byInstanceIdx, FALSE);
	if ( ERR_ENTER_INSTANCE::ID_NOT_UNLOCK_ERR == wErrCode)
	{
		return ERR_NEIDAN_BATTLE::ID_PREVIOUS_INSTANCE_NOT_PASS;
	}
	else if ( ERR_ENTER_INSTANCE::ID_LEVEL_ERR == wErrCode)
	{
		return ERR_NEIDAN_BATTLE::ID_LEVEL_NOT_ENOUGH;
	}
	else if (ERR_ENTER_INSTANCE::ID_NOT_TIMES_OUT_BOUND == wErrCode)
	{
		return ERR_NEIDAN_BATTLE::ID_NOT_TIME_BATTLE_NUM;
	}
	else if ( ERR_ENTER_INSTANCE::ID_INDEX_ERR == wErrCode)
	{
		return ERR_NEIDAN_BATTLE::ID_PREVIOUS_INSTANCE_NOT_PASS;
	}
	//打第一个副本时，重新赋值满血量
	if ( 1 == stReq.stInstanceID.byInstanceIdx)
	{
		poPlayerPet->ResetNeiDanHeroHP();
	}

	//记录副本位置
	oInstance.EnterInstance(stReq.stInstanceID.wSceneIdx, stReq.stInstanceID.byTownIdx, stReq.stInstanceID.byInstanceIdx);
	
	NeiDanBattle(poPlayer, stReq.stInstanceID, stAck.stPassEncourage, &stAck.stBattleInfo);

	return ERR_NEIDAN_BATTLE::ID_SUCCESS;
}

UINT16 CInstanceMgr::RecvNeiDanAward(CPlayer* poPlayer, PKT_CLIGS_RECV_NEIDAN_AWARD_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_RECV_NEIDAN_AWARD::ID_IS_NOT_OPEN;
	}
	if ( !poPlayer->GetBuild(EBK_NEIDAN_INSTANCE))
	{
		return ERR_RECV_NEIDAN_AWARD::ID_IS_NOT_OPEN;
	}

	CInstance& oInstance = poPlayer->GetInstance();
	UINT16 wErrCode = oInstance.PassBoxEnc(oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx, stAck.stPassEncourage);
	if(SUCCESS == wErrCode)
	{
		poPlayer->GetState().OnOpenEncourageBox();

		SInstanceProp* poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp( oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx + 1);
		if ( NULL == poInstanceProp)
		{
			CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
			if ( NULL != poPlayerPet)
			{
				poPlayerPet->ResetNeiDanHeroHP();
			}
			//打开宝箱后退出副本
			poPlayer->ExitInstance();
		}
		else
		{
			stAck.stNextInstance.wSceneIdx = oInstance.GetCurBattleInstance().wSceneIdx;
			stAck.stNextInstance.byTownIdx = oInstance.GetCurBattleInstance().byTownIdx;
			stAck.stNextInstance.byInstanceIdx = oInstance.GetCurBattleInstance().byInstanceIdx + 1;
			
			oInstance.EnterInstance(stAck.stNextInstance.wSceneIdx, stAck.stNextInstance.byTownIdx, stAck.stNextInstance.byInstanceIdx);
		}
	}
	else
	{
		poPlayer->ExitInstance();
	}
	
	return ERR_RECV_NEIDAN_AWARD::ID_SUCCESS;
}

UINT16 CInstanceMgr::SkipNeiDanBattle(CPlayer* poPlayer, PKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ& stReq, PKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_SKIP_NEIDAN_BATTLE::ID_IS_NOT_OPEN;
	}

	if ( !poPlayer->GetBuild(EBK_NEIDAN_INSTANCE))
	{
		return ERR_SKIP_NEIDAN_BATTLE::ID_IS_NOT_OPEN;
	}
	CInstance& oInstance = poPlayer->GetInstance();
	DT_INSTANCE_ID stInstanceID = stReq.stInstanceID;
	stInstanceID.byInstanceIdx = 1;
	SInstanceProp* poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp( stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx);
	
	while(poInstanceProp)
	{
		DT_PASS_ENCOURAGE_BOX_DATA stEncourageBoxData = {0};
		NeiDanBattle(poPlayer, stInstanceID, stEncourageBoxData, NULL, TRUE);
		oInstance.PassBoxEnc(stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx, stEncourageBoxData);
		UINT8 byItemNum = 0;
		for ( UINT8 byIdx = 0; byIdx < stEncourageBoxData.byGroupNum; byIdx++)
		{
			UINT8 byHitItemIdx = stEncourageBoxData.astBoxGroupInfo[byIdx].byHitItemIdx;
			if ( 0 != byHitItemIdx)
			{
				stAck.stAwardInfo.astAwardItemInfoLst[stInstanceID.byInstanceIdx - 1].astItemInfo[byItemNum].wItemID = stEncourageBoxData.astBoxGroupInfo[byIdx].stItemInfo.astItemList[byHitItemIdx - 1].wKindID;
				stAck.stAwardInfo.astAwardItemInfoLst[stInstanceID.byInstanceIdx - 1].astItemInfo[byItemNum].wItemNum = stEncourageBoxData.astBoxGroupInfo[byIdx].stItemInfo.astItemList[byHitItemIdx - 1].wPileNum;
			}
		}
		stAck.stAwardInfo.astAwardItemInfoLst[stInstanceID.byInstanceIdx - 1].byNum = byItemNum;
		poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp( stInstanceID.wSceneIdx, stInstanceID.byTownIdx, ++stInstanceID.byInstanceIdx);
	}
	poPlayer->ExitInstance();
	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL != poPlayerPet)
	{
		poPlayerPet->ResetNeiDanHeroHP();
	}
	return ERR_SKIP_NEIDAN_BATTLE::ID_SUCCESS;
}

VOID CInstanceMgr::NeiDanBattle(CPlayer* poPlayer, DT_INSTANCE_ID& stInstanceID, DT_PASS_ENCOURAGE_BOX_DATA& stEncourageBoxData, DT_BATTLE_DATA5* pstBattleData , BOOL bIsSkip )
{
	CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx, 1);
	if ( NULL == poMonsterGroup)
	{
		return ;
	}
	DT_BATTLE_OBJ_GROUP_HP stAfterMyself;
	DT_BATTLE_OBJ_GROUP_HP stInitMyself;
	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ;
	}
	BOOL bPass = FALSE;
	CInstance& oInstance = poPlayer->GetInstance();
	if ( !bIsSkip && NULL != pstBattleData)
	{
		poPlayerPet->SetInitHeroHP( stInitMyself );
		//战斗
		CBattleMgr::Instance()->Battle4( poPlayer, poMonsterGroup, *pstBattleData, FALSE, FALSE, &stAfterMyself, NULL, &stInitMyself, NULL );
		bPass = (0 == pstBattleData->byBattleResult);
		pstBattleData->byBattleResult = bPass ? 2 : 1;
		poPlayerPet->SetAfterHeroHP( stAfterMyself );
	}
	else
	{
		bPass = TRUE;
	}

	//记录副本信息
	if ( bPass )
	{
		//记录通关副本
		CInstanceRecord* poInstanceRecord = oInstance.GetSingleInstaceRecord(oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx, FALSE);
		//没有记录或通关赢的记录
		if((NULL == poInstanceRecord) )
		{
			poInstanceRecord = CInstanceMgr::Instance()->CreatInstanceRecord(poPlayer->GetID(), oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx);
			if(NULL == poInstanceRecord)
			{
				USR_INFO("[%s:%d] PlayerID=%d, CreateRecord failed", MSG_MARK, poPlayer->GetID());
				return ;
			}

			DT_INSTANCE_DATA stDT_INSTANCE_DATA = {0};
			stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = oInstance.GetCurBattleInstance().wSceneIdx;
			stDT_INSTANCE_DATA.stInstanceID.byTownIdx = oInstance.GetCurBattleInstance().byTownIdx;
			stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx = oInstance.GetCurBattleInstance().byInstanceIdx;
			stDT_INSTANCE_DATA.byScore = 0;
			stDT_INSTANCE_DATA.qwLastPassTime = SDTimeSecs();
			stDT_INSTANCE_DATA.dwSuccessNum = 1;
			if(FALSE == poInstanceRecord->Init(&stDT_INSTANCE_DATA, poPlayer))
			{
				SYS_CRITICAL(_SDT("[%s: %d]:  InstanceRecord init failed!"), MSG_MARK);
				CInstanceMgr::Instance()->RemoveInsanceRecord(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx);
				return ;
			}

			oInstance.GetNeiDanInstanceRecord().AddData(CLogicFunc::GetInstanceIdxKey(oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx), poInstanceRecord);
		}
		else
		{
			poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
			poInstanceRecord->GetDT_INSTANCE_DATA().dwSuccessNum++;
			poInstanceRecord->GetDT_INSTANCE_DATA().wLastConFailedNum = 0;
		}
		//打开宝箱
		oInstance.RatePassBoxEnc(oInstance.GetCurBattleInstance().wSceneIdx, oInstance.GetCurBattleInstance().byTownIdx, oInstance.GetCurBattleInstance().byInstanceIdx, stEncourageBoxData);
	}
	else
	{
		poPlayerPet->ResetNeiDanHeroHP();
		poPlayer->ExitInstance();
	}
}