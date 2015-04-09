#include "battlemgr.h"
#include <logic/player/player.h>
#include <logic/base/logicfunc.h>
#include <sdrandom.h>
#include <logic/monster/monstermgr.h>
#include <logic/race/racemgr.h>
#include <logic/race/racepropmgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/client/errdef.h>
#include <logic/plunder/plundermgr.h>
#include <logic/other/hpgridmgr.h>
#include <logic/player/playersavemgr.h>
#include <logic/player/playermgr.h>
#include <logic/hero/heromgr.h>
#include <logic/hero/heropropmgr.h>
#include <utility.h>
#include <logic/skill/skillpropmgr.h>
#include "logic/formation/formationmgr.h"
#include <dll/sdframework/sdloggerimpl.h>
#include "logic/specialeffects/specialeffects.h"
#include "logic/pet/pet.h"
#include "logic/pet/petskill.h"

IMPLEMENT_SINGLETON(CBattleMgr)

namespace battle
{
    void CopyHPGridNum(DT_BATTLE_OBJ_GROUP_DATA3 &stObjGroup, const DT_BATTLE_OBJ_GROUP_DATA3 &stObjGroupWithHPGrid)
    {
        for (UINT16 i = 0; i < stObjGroup.byObjNum; i++)
        {
            DT_BATTLE_OBJ_DATA2 &stObj = stObjGroup.astBattleObjInfo[i];

            for(UINT16 wObjIdx = 0; wObjIdx < stObjGroupWithHPGrid.byObjNum; wObjIdx++)
            {
                const DT_BATTLE_OBJ_DATA2 &stObjWithHPGrid = stObjGroupWithHPGrid.astBattleObjInfo[wObjIdx];
                if(stObj.byFomationIdx == stObjWithHPGrid.byFomationIdx)
                {
                    stObj.byHPGridNum = stObjWithHPGrid.byHPGridNum;
                    break;
                }
            }
        }
    }

	void CopyHPGridNum(DT_BATTLE_OBJ_GROUP_DATA2 &stObjGroup, const DT_BATTLE_OBJ_GROUP_DATA3 &stObjGroupWithHPGrid)
	{
		for (UINT16 i = 0; i < stObjGroup.byObjNum; i++)
		{
			DT_BATTLE_OBJ_DATA2 &stObj = stObjGroup.astBattleObjInfo[i];

			for(UINT16 wObjIdx = 0; wObjIdx < stObjGroupWithHPGrid.byObjNum; wObjIdx++)
			{
				const DT_BATTLE_OBJ_DATA2 &stObjWithHPGrid = stObjGroupWithHPGrid.astBattleObjInfo[wObjIdx];
				if(stObj.byFomationIdx == stObjWithHPGrid.byFomationIdx)
				{
					stObj.byHPGridNum = stObjWithHPGrid.byHPGridNum;
					break;
				}
			}
		}
	}
}

CBattleMgr::CBattleMgr()
{

}

CBattleMgr::~CBattleMgr()
{

}

BOOL CBattleMgr::Init()
{
    return TRUE;
}

VOID CBattleMgr::UnInit()
{
}


BOOL CBattleMgr::Battle(CBattleObj* poMyself, CBattleObj* poEnemy, DT_BATTLE_DATA2& stDT_BATTLE_DATA, BOOL bPVP, DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfInfo, BOOL bChallengFirst)
{
	DT_BATTLE_DATA4 stBattle4 = {0};
	BOOL bRet = Battle3(poMyself, poEnemy, stBattle4, bPVP, bChallengFirst, pstAfterMyselfInfo);
	Convert4To2(stBattle4, stDT_BATTLE_DATA);
	return bRet;
}

BOOL CBattleMgr::Battle2(
    CBattleObj* poMyself, 
    CBattleObj* poEnemy, 
    DT_BATTLE_DATA3& stDT_BATTLE_DATA, 
    BOOL bPVP, 
    BOOL bChallengFirst, 

    DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfHP, 
    DT_BATTLE_OBJ_GROUP_HP *pstAfterEnemyHP,
    DT_BATTLE_OBJ_GROUP_HP *pstInitMyselfHP, 
    DT_BATTLE_OBJ_GROUP_HP *pstInitEnemyHP)
{
	DT_BATTLE_DATA4 stBattle4 = {0};
	BOOL bRet = Battle3(poMyself, poEnemy, stBattle4, bPVP, bChallengFirst, pstAfterMyselfHP, pstAfterEnemyHP, pstInitMyselfHP, pstInitEnemyHP);
	Convert4To3(stBattle4, stDT_BATTLE_DATA);
	return bRet;
}

BOOL CBattleMgr::Battle3(
	CBattleObj* poMyself, CBattleObj* poEnemy, DT_BATTLE_DATA4& stDT_BATTLE_DATA, 
	BOOL bPVP, BOOL bChallengFirst, 
	DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfHP, 
	DT_BATTLE_OBJ_GROUP_HP *pstAfterEnemyHP,
	DT_BATTLE_OBJ_GROUP_HP *pstInitMyselfHP, 
	DT_BATTLE_OBJ_GROUP_HP *pstInitEnemyHP,
	UINT8 byMyselfStrongPercent,
	UINT8 byEnemyStrongPercent)
{
	if((NULL == poMyself) || (NULL == poEnemy))
	{
		return FALSE;
	}
	//memset(&stDT_BATTLE_DATA, 0, sizeof(stDT_BATTLE_DATA));
	CPlayer* poPlayer = dynamic_cast<CPlayer*>(poMyself);
	if ( NULL != poPlayer)
	{
		poPlayer->GetFormation().CheckFormation();
	}
	memset(&stDT_BATTLE_DATA, 0, sizeof(stDT_BATTLE_DATA));

	poMyself->StartBattle(poEnemy);	//确认敌人，清空战斗数据
	poEnemy->StartBattle(poMyself);

	DT_BATTLE_OBJ_GROUP_DATA3 stMyselfInfo = {0};
	DT_BATTLE_OBJ_GROUP_DATA3 stEnemyInfo = {0};

	//赋值战斗者属性
	poMyself->GetDT_BATTLE_OBJ_GROUP_DATA(stMyselfInfo);
	poEnemy->GetDT_BATTLE_OBJ_GROUP_DATA(stEnemyInfo);

	// 初始化己方和敌方的当前血量
	if(pstInitMyselfHP)
	{
		poMyself->InitCurBattleAttr(pstInitMyselfHP);
		// pstInitMyselfHP->CopyHPTo(stDT_BATTLE_DATA.stMyselfInfo);
	}
	if(pstInitEnemyHP)
	{
		poEnemy->InitCurBattleAttr(pstInitEnemyHP);
		// pstInitEnemyHP->CopyHPTo(stDT_BATTLE_DATA.stEnemyInfo);
	}

	if (100 != byMyselfStrongPercent)
	{
		poMyself->BeStronger(byMyselfStrongPercent);
	}

	if (100 != byEnemyStrongPercent)
	{
		poEnemy->BeStronger(byEnemyStrongPercent);
	}        

	//赋值战斗者属性

	memcpy(&stDT_BATTLE_DATA.stMyselfInfo, &stMyselfInfo, sizeof(DT_BATTLE_OBJ_GROUP_DATA2));
	memcpy(&stDT_BATTLE_DATA.stEnemyInfo, &stEnemyInfo, sizeof(DT_BATTLE_OBJ_GROUP_DATA2));

	//当敌人是怪物组
	map<UINT16, UINT16> mapBossMonsterID;//存放非普通怪
	if(dynamic_cast<CMonsterGroup*>(poEnemy))
	{
		UINT8 byIdx = 0;
		UINT8 byObjNum = stDT_BATTLE_DATA.stEnemyInfo.byObjNum;
		for(byIdx = 0; byIdx < byObjNum; byIdx++)
		{
			UINT16 wMonsterKindID = stDT_BATTLE_DATA.stEnemyInfo.astBattleObjInfo[byIdx].wKindID;
			SMonsterProp* pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(stDT_BATTLE_DATA.stEnemyInfo.astBattleObjInfo[byIdx].wKindID);
			if(pstMonsterProp && (pstMonsterProp->byType > EMT_NORMAL))
			{
				mapBossMonsterID[wMonsterKindID] = wMonsterKindID;//非普通怪
			}
		}
	}

	CHPGridMgr::Instance()->SetHPGridNum(stMyselfInfo, stEnemyInfo, mapBossMonsterID);		//设置血条（格子数）

	battle::CopyHPGridNum(stDT_BATTLE_DATA.stMyselfInfo, stMyselfInfo);
	battle::CopyHPGridNum(stDT_BATTLE_DATA.stEnemyInfo, stEnemyInfo);

	EAttackerFlag		eAttackerFlag = EAF_MYSELF; //攻击者标志
	CBattleObj*		poAttacker = poMyself;
	CBattleObj*		poBeattacker = poEnemy;
	EAttackerFlag		eStartAttackerFlag = EAF_MYSELF;
	CBattleObj*		poStartAttacker = poMyself;
	CBattleObj*		poStartBeattacker = poEnemy;
	if((!bChallengFirst) && (bPVP) && (poAttacker->GetFirstAttackValue() < poEnemy->GetFirstAttackValue()))//PVP时比较先攻值
	{//攻守转换
		eAttackerFlag = EAF_ENEMY;
		poAttacker = poEnemy;
		poBeattacker = poMyself;
		eStartAttackerFlag = EAF_ENEMY;
		poStartAttacker = poEnemy;
		poStartBeattacker = poMyself;
	}

	UINT32 wIdx = 0;
	ERoundResult eRoundResult = ERR_CONTINUE;
	BOOL bEndBigRound = FALSE;
	CBattleAttrObj* poCurEnemy = NULL;

	while((ERR_DEAD != (eRoundResult = OnceRound3(poAttacker, poBeattacker, eAttackerFlag, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx], poCurEnemy)))
		&& (wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2))
	{
		poAttacker->OnRoundEnd(TRUE);
		poBeattacker->OnRoundEnd(FALSE);

		if((!poAttacker->RoundHaveNextObj()) && (!poBeattacker->RoundHaveNextObj()))
		{
			bEndBigRound = TRUE;
		}

		//buff回合减少
		BuffRoundDec(poMyself, bEndBigRound);
		BuffRoundDec(poEnemy, bEndBigRound);
		bEndBigRound = FALSE;
		//获取攻击后回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);

		//防守者反击
		if ( (poCurEnemy) && (!poCurEnemy->IsDead()) && (poCurEnemy->GetBeatBack()))
		{
			wIdx++;
			EAttackerFlag eTmpAttackFlag = (eAttackerFlag == EAF_ENEMY ? EAF_MYSELF : EAF_ENEMY);
			if ( ERR_DEAD != (eRoundResult = SecondAttack(poBeattacker, poCurEnemy, poAttacker, poAttacker->GetCurAttacker(), eTmpAttackFlag, poCurEnemy->GetBeatBack(), stDT_BATTLE_DATA.astBattleRoundInfo[wIdx])) && (wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2))
			{
				poBeattacker->OnRoundEnd(TRUE);
				poAttacker->OnRoundEnd(FALSE);
				poCurEnemy->SetBeatBack(NULL);
				//获取攻击后回合数据
				GetAttackData3(poBeattacker, poAttacker , eTmpAttackFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
			}
			else
			{
				break;
			}

		}
		CBattleAttrObj* poCurAttack = poAttacker->GetCurAttacker();
		//进攻者连击
		if ( (poCurAttack) && (!poCurAttack->IsDead()) && (poCurAttack->GetContinueAttack()) )
		{
			wIdx++;
			if ( ERR_DEAD != (eRoundResult = SecondAttack( poAttacker, poAttacker->GetCurAttacker(),poBeattacker, poCurEnemy, eAttackerFlag, poCurAttack->GetContinueAttack(), stDT_BATTLE_DATA.astBattleRoundInfo[wIdx])) && (wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2))
			{
				poAttacker->GetCurAttacker()->SetContinueAttack(NULL);
				poAttacker->OnRoundEnd(TRUE);
				poBeattacker->OnRoundEnd(FALSE);
				//获取攻击后回合数据
				GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
			}
			else
			{
				break;
			}
		}

		//本回合内对方还有出手者，则交换攻击方
		if(poBeattacker->RoundHaveNextObj())
		{
			if(EAF_MYSELF == eAttackerFlag)
			{
				eAttackerFlag = EAF_ENEMY;
				poAttacker = poEnemy;
				poBeattacker = poMyself;
			}
			else
			{
				eAttackerFlag = EAF_MYSELF;
				poAttacker = poMyself;
				poBeattacker = poEnemy;
			}
		}
		//双方都没有出手者了，则重置出手者
		else if(!poAttacker->RoundHaveNextObj())
		{
			poAttacker = poStartAttacker;
			poBeattacker = poStartBeattacker;
			eAttackerFlag = eStartAttackerFlag; //攻击者标志
			poAttacker->ResetCurAttackObjIdx();
			poBeattacker->ResetCurAttackObjIdx();
		}
		//否则攻击方继续出手,不行变换出手方
		else
		{
			//nothing
		}

		wIdx++;
	}
	



	//死亡了，wIdx++执行不到，则回合数少+1
	if(wIdx != MAX_NEW_BATTLE_ROUND_NUM - 1)
	{
		//获取攻击后回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
		stDT_BATTLE_DATA.byBattleRoundNum = wIdx + 1;
	}
	else
	{
		stDT_BATTLE_DATA.byBattleRoundNum = wIdx;
		//DBG_INFO( _SDT("***回合满***"));

	}


	///若最后双方未有死亡，竞技场按战力高的赢；其他的按挑战者输
	if(ERR_DEAD != eRoundResult)
	{
		if(bPVP)
		{
			CPlayer* poMyselfPlayer = dynamic_cast<CPlayer*>(poMyself);
			CPlayer* poEnemyPlayer = dynamic_cast<CPlayer*>(poEnemy);
			if((NULL == poMyselfPlayer) || (NULL == poEnemyPlayer))//异常
			{
				stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
			}
			else
			{
				//if(poMyselfPlayer->GetDT_PLAYER_BASE_DATA().dwPower > poEnemyPlayer->GetDT_PLAYER_BASE_DATA().dwPower)
				//{
				//    stDT_BATTLE_DATA.byBattleResult = EBT_SUCCESS;
				//}
				//else
				//{
				//    stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
				//}
				stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
			}
		}
		else
		{
			stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
		}
	}
	else
	{
		if(!poMyself->IsDead())
		{
			stDT_BATTLE_DATA.byBattleResult = EBT_SUCCESS;
		}
		else
		{
			stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
		}
	}

	//获取玩家战斗后数据
	if (pstAfterMyselfHP)
	{
		poMyself->GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(*pstAfterMyselfHP);
	}

	if (pstAfterEnemyHP)
	{
		poEnemy->GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(*pstAfterEnemyHP);
	}

	poMyself->EndBattle();
	poEnemy->EndBattle();

	CPlayer *pPlayerMySelf = dynamic_cast<CPlayer*>( poMyself );
	if( pPlayerMySelf )
	{
		strcpy( stDT_BATTLE_DATA.stMyselfInfo.aszDispName, pPlayerMySelf->GetDT_PLAYER_BASE_DATA().aszDispName );
		stDT_BATTLE_DATA.stMyselfInfo.dwPlayerID = pPlayerMySelf->GetID();
	}

	CPlayer *pPlayerEnemy = dynamic_cast<CPlayer*>( poEnemy );
	if( pPlayerEnemy )
	{
		strcpy( stDT_BATTLE_DATA.stEnemyInfo.aszDispName, pPlayerEnemy->GetDT_PLAYER_BASE_DATA().aszDispName );
		stDT_BATTLE_DATA.stEnemyInfo.dwPlayerID = pPlayerEnemy->GetID();
	}

	//// VerifyBattle(stDT_BATTLE_DATA);
	return TRUE;
}

BOOL CBattleMgr::Battle4(
	CBattleObj* poMyself, CBattleObj* poEnemy, DT_BATTLE_DATA5& stDT_BATTLE_DATA, 
	BOOL bPVP, BOOL bChallengFirst, 
    DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfHP, 
	DT_BATTLE_OBJ_GROUP_HP *pstAfterEnemyHP,
	DT_BATTLE_OBJ_GROUP_HP *pstInitMyselfHP, 
	DT_BATTLE_OBJ_GROUP_HP *pstInitEnemyHP,
    UINT8 byMyselfStrongPercent,
    UINT8 byEnemyStrongPercent)
{
	if((NULL == poMyself) || (NULL == poEnemy))
	{
		return FALSE;
	}
	//memset(&stDT_BATTLE_DATA, 0, sizeof(stDT_BATTLE_DATA));
	CPlayer* poPlayer = dynamic_cast<CPlayer*>(poMyself);
	if ( NULL != poPlayer)
	{
		poPlayer->GetFormation().CheckFormation();
	}
	memset(&stDT_BATTLE_DATA, 0, sizeof(stDT_BATTLE_DATA));

	poMyself->StartBattle(poEnemy);	//确认敌人，清空战斗数据
	poEnemy->StartBattle(poMyself);

	DT_BATTLE_OBJ_GROUP_DATA3 stMyselfInfo;
	DT_BATTLE_OBJ_GROUP_DATA3 stEnemyInfo;

	memset(&stMyselfInfo, 0, sizeof(stMyselfInfo));
	memset(&stEnemyInfo, 0, sizeof(stEnemyInfo));

	//赋值战斗者属性
	poMyself->GetDT_BATTLE_OBJ_GROUP_DATA(stMyselfInfo);
	poEnemy->GetDT_BATTLE_OBJ_GROUP_DATA(stEnemyInfo);

	// 初始化己方和敌方的当前血量
	if(pstInitMyselfHP)
	{
		poMyself->InitCurBattleAttr(pstInitMyselfHP);
		// pstInitMyselfHP->CopyHPTo(stDT_BATTLE_DATA.stMyselfInfo);
	}
	if(pstInitEnemyHP)
	{
		poEnemy->InitCurBattleAttr(pstInitEnemyHP);
		// pstInitEnemyHP->CopyHPTo(stDT_BATTLE_DATA.stEnemyInfo);
	}

    if (100 != byMyselfStrongPercent)
    {
        poMyself->BeStronger(byMyselfStrongPercent);
    }

    if (100 != byEnemyStrongPercent)
    {
        poEnemy->BeStronger(byEnemyStrongPercent);
    }        

	//赋值战斗者属性
	poMyself->GetDT_BATTLE_OBJ_GROUP_DATA(stDT_BATTLE_DATA.stMyselfInfo);
	poEnemy->GetDT_BATTLE_OBJ_GROUP_DATA(stDT_BATTLE_DATA.stEnemyInfo);

	//当敌人是怪物组
	map<UINT16, UINT16> mapBossMonsterID;//存放非普通怪
	if(dynamic_cast<CMonsterGroup*>(poEnemy))
	{
		UINT8 byIdx = 0;
		UINT8 byObjNum = stDT_BATTLE_DATA.stEnemyInfo.byObjNum;
		for(byIdx = 0; byIdx < byObjNum; byIdx++)
		{
			UINT16 wMonsterKindID = stDT_BATTLE_DATA.stEnemyInfo.astBattleObjInfo[byIdx].wKindID;
			SMonsterProp* pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(stDT_BATTLE_DATA.stEnemyInfo.astBattleObjInfo[byIdx].wKindID);
			if(pstMonsterProp && (pstMonsterProp->byType > EMT_NORMAL))
			{
				mapBossMonsterID[wMonsterKindID] = wMonsterKindID;//非普通怪
			}
		}
	}

	CHPGridMgr::Instance()->SetHPGridNum(stMyselfInfo, stEnemyInfo, mapBossMonsterID);		//设置血条（格子数）

	battle::CopyHPGridNum(stDT_BATTLE_DATA.stMyselfInfo, stMyselfInfo);
	battle::CopyHPGridNum(stDT_BATTLE_DATA.stEnemyInfo, stEnemyInfo);

	EAttackerFlag		eAttackerFlag = EAF_MYSELF; //攻击者标志
	CBattleObj*		poAttacker = poMyself;
	CBattleObj*		poBeattacker = poEnemy;
	EAttackerFlag		eStartAttackerFlag = EAF_MYSELF;
	CBattleObj*		poStartAttacker = poMyself;
	CBattleObj*		poStartBeattacker = poEnemy;
	if((!bChallengFirst) && (bPVP) && (poAttacker->GetFirstAttackValue() < poEnemy->GetFirstAttackValue()))//PVP时比较先攻值
	{//攻守转换
		eAttackerFlag = EAF_ENEMY;
		poAttacker = poEnemy;
		poBeattacker = poMyself;
		eStartAttackerFlag = EAF_ENEMY;
		poStartAttacker = poEnemy;
		poStartBeattacker = poMyself;
	}

	UINT32 wIdx = 0;
	ERoundResult eRoundResult = ERR_CONTINUE;
	BOOL bEndBigRound = FALSE;
	CBattleAttrObj* poCurEnemy = NULL;

	while ( wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2 )
	{
		poAttacker->SetPetBattleState( FALSE );
		poBeattacker->SetPetBattleState( FALSE );
		EAttackerFlag ePetFlag = eAttackerFlag;

		vector<CBattleAttrObj*> vecTargetObj;
		eRoundResult = PetBattle( poAttacker, poBeattacker, ePetFlag, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx], vecTargetObj);
		//攻击方宠物战斗
		if ( ERR_CONTINUE == eRoundResult)
		{
			//获取攻击后回合数据
			GetAttackData3(poAttacker, poBeattacker , ePetFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
			wIdx++;
		}
		else if ( ERR_DEAD == eRoundResult)
		{
			break;
		}

		eRoundResult = PetSecondBattle( poAttacker, poBeattacker, ePetFlag, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx], vecTargetObj);
		//攻击方宠物战斗
		if ( ERR_CONTINUE == eRoundResult)
		{
			//获取攻击后回合数据
			GetAttackData3(poAttacker, poBeattacker , ePetFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
			wIdx++;
		}
		else if ( ERR_DEAD == eRoundResult)
		{
			break;
		}

		//攻守转换
		ePetFlag = (ePetFlag == EAF_ENEMY) ? EAF_MYSELF : EAF_ENEMY;
		eRoundResult = PetBattle( poBeattacker, poAttacker, ePetFlag, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx], vecTargetObj);
		//防守方宠物战斗
		if ( ERR_CONTINUE == eRoundResult)
		{
			//获取攻击后回合数据
			GetAttackData3(poBeattacker, poAttacker , ePetFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
			wIdx++;
		}
		else if ( eRoundResult == ERR_DEAD)
		{
			break;
		}

		eRoundResult = PetSecondBattle( poBeattacker, poAttacker, ePetFlag, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx], vecTargetObj);
		//攻击方宠物战斗
		if ( ERR_CONTINUE == eRoundResult)
		{
			//获取攻击后回合数据
			GetAttackData3(poBeattacker, poAttacker , ePetFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
			wIdx++;
		}
		else if ( ERR_DEAD == eRoundResult)
		{
			break;
		}

		while((ERR_DEAD != (eRoundResult = OnceRound4(poAttacker, poBeattacker, eAttackerFlag, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx], poCurEnemy)))
			&& (wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2))
		{
			poAttacker->OnRoundEnd(TRUE);
			poBeattacker->OnRoundEnd(FALSE);

			if((!poAttacker->RoundHaveNextObj()) && (!poBeattacker->RoundHaveNextObj()))
			{
				bEndBigRound = TRUE;
			}
			
			//获取攻击后回合数据
			GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);

			//防守者反击
			if ( (poCurEnemy) && (!poCurEnemy->IsDead()) && (poCurEnemy->GetBeatBack()))
			{
				wIdx++;
				EAttackerFlag eTmpAttackFlag = (eAttackerFlag == EAF_ENEMY ? EAF_MYSELF : EAF_ENEMY);
				if ( ERR_DEAD != (eRoundResult = SecondAttack(poBeattacker, poCurEnemy, poAttacker, poAttacker->GetCurAttacker(), eTmpAttackFlag, poCurEnemy->GetBeatBack(), stDT_BATTLE_DATA.astBattleRoundInfo[wIdx])) && (wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2))
				{
					poBeattacker->OnRoundEnd(TRUE);
					poAttacker->OnRoundEnd(FALSE);
					poCurEnemy->SetBeatBack(NULL);
					//获取攻击后回合数据
					GetAttackData3(poBeattacker, poAttacker , eTmpAttackFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
				}
				else
				{
					break;
				}

			}
			CBattleAttrObj* poCurAttack = poAttacker->GetCurAttacker();
			//进攻者连击
			if ( (poCurAttack) && (!poCurAttack->IsDead()) && (poCurAttack->GetContinueAttack()) )
			{
				wIdx++;
				if ( ERR_DEAD != (eRoundResult = SecondAttack( poAttacker, poAttacker->GetCurAttacker(),poBeattacker, poCurEnemy, eAttackerFlag, poCurAttack->GetContinueAttack(), stDT_BATTLE_DATA.astBattleRoundInfo[wIdx])) && (wIdx < MAX_NEW_BATTLE_ROUND_NUM - 2))
				{
					poAttacker->GetCurAttacker()->SetContinueAttack(NULL);
					poAttacker->OnRoundEnd(TRUE);
					poBeattacker->OnRoundEnd(FALSE);
					//获取攻击后回合数据
					GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
				}
				else
				{
					break;
				}
			}

			//buff回合减少
			BuffRoundDec(poMyself, bEndBigRound);
			BuffRoundDec(poEnemy, bEndBigRound);

			//本回合内对方还有出手者，则交换攻击方
			if(poBeattacker->RoundHaveNextObj())
			{
				if(EAF_MYSELF == eAttackerFlag)
				{
					eAttackerFlag = EAF_ENEMY;
					poAttacker = poEnemy;
					poBeattacker = poMyself;
				}
				else
				{
					eAttackerFlag = EAF_MYSELF;
					poAttacker = poMyself;
					poBeattacker = poEnemy;
				}
			}
			//双方都没有出手者了，则重置出手者
			else if(!poAttacker->RoundHaveNextObj())
			{
				poAttacker = poStartAttacker;
				poBeattacker = poStartBeattacker;
				eAttackerFlag = eStartAttackerFlag; //攻击者标志
				poAttacker->ResetCurAttackObjIdx();
				poBeattacker->ResetCurAttackObjIdx();
			}
			//否则攻击方继续出手,不行变换出手方
			else
			{
				//nothing
			}

			wIdx++;
			if ( bEndBigRound )
			{
				break;
			}
		}
		bEndBigRound = FALSE;

		//死亡判断 (中毒buff等影响)
		if(JudgeDead(poAttacker, poBeattacker)  )
		{
			break;
		}
		
	}
	

	//死亡了，wIdx++执行不到，则回合数少+1
	if(wIdx != MAX_NEW_BATTLE_ROUND_NUM - 1)
	{
		//获取攻击后回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, FALSE, stDT_BATTLE_DATA.astBattleRoundInfo[wIdx].stAttackData.stAfterAttack);
		stDT_BATTLE_DATA.byBattleRoundNum = wIdx + 1;
	}
	else
	{
		stDT_BATTLE_DATA.byBattleRoundNum = wIdx;
		//DBG_INFO( _SDT("***回合满***"));

	}


	///若最后双方未有死亡，竞技场按战力高的赢；其他的按挑战者输
	if(ERR_DEAD != eRoundResult)
	{
		if(bPVP)
		{
			CPlayer* poMyselfPlayer = dynamic_cast<CPlayer*>(poMyself);
			CPlayer* poEnemyPlayer = dynamic_cast<CPlayer*>(poEnemy);
			if((NULL == poMyselfPlayer) || (NULL == poEnemyPlayer))//异常
			{
				stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
			}
			else
			{
				stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
			}
		}
		else
		{
			stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
		}
	}
	else
	{
		if(!poMyself->IsDead())
		{
			stDT_BATTLE_DATA.byBattleResult = EBT_SUCCESS;
		}
		else
		{
			stDT_BATTLE_DATA.byBattleResult = EBT_FAILED;
		}
	}

	//获取玩家战斗后数据
	if (pstAfterMyselfHP)
	{
		poMyself->GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(*pstAfterMyselfHP);
	}

	if (pstAfterEnemyHP)
	{
		poEnemy->GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(*pstAfterEnemyHP);
	}

	poMyself->EndBattle();
	poEnemy->EndBattle();

	CPlayer *pPlayerMySelf = dynamic_cast<CPlayer*>( poMyself );
	if( pPlayerMySelf )
	{
		strcpy( stDT_BATTLE_DATA.stMyselfInfo.aszDispName, pPlayerMySelf->GetDT_PLAYER_BASE_DATA().aszDispName );
		stDT_BATTLE_DATA.stMyselfInfo.dwPlayerID = pPlayerMySelf->GetID();
	}

	CPlayer *pPlayerEnemy = dynamic_cast<CPlayer*>( poEnemy );
	if( pPlayerEnemy )
	{
		strcpy( stDT_BATTLE_DATA.stEnemyInfo.aszDispName, pPlayerEnemy->GetDT_PLAYER_BASE_DATA().aszDispName );
		stDT_BATTLE_DATA.stEnemyInfo.dwPlayerID = pPlayerEnemy->GetID();
	}
#ifdef MAXNET_DEBUG
	VerifyBattle(stDT_BATTLE_DATA);
#endif
	return TRUE;
}

VOID CBattleMgr::VerifyBattle(const DT_BATTLE_DATA5& stBattleData4)
{
	UINT8 byIdx = 0;
	USR_INFO("Result : %d", stBattleData4.byBattleResult );
	
	map<INT32, INT32> allMyselfHPChange;
	map<INT32, INT32> allEnemyHPChange;
	for ( byIdx = 0; byIdx < stBattleData4.byBattleRoundNum; byIdx++)
	{
		UINT8 byFlag = stBattleData4.astBattleRoundInfo[byIdx].byAttackerFlag;
		USR_INFO("Attack Flag = %s", byFlag == 0 ? "Myself" : "Enemy");
		USR_INFO("MyselfObj HP :");
		UINT8 byHPIdx = 0;
		map<INT32, INT32> HPMap;
		const DT_ATTACK_OBJ_DATA3& stMyselfBefer = stBattleData4.astBattleRoundInfo[byIdx].stAttackData.stBeforeAttack.stMyselfObjInfo;
		for( byHPIdx = 0; byHPIdx < stMyselfBefer.byHpChangeNum; byHPIdx++)
		{
			HPMap[stMyselfBefer.astHpChangeInfo[byHPIdx].byFormationIdx] += stMyselfBefer.astHpChangeInfo[byHPIdx].nHP;
			allMyselfHPChange[stMyselfBefer.astHpChangeInfo[byHPIdx].byFormationIdx] += stMyselfBefer.astHpChangeInfo[byHPIdx].nHP;
		}

		for ( UINT8 buffIdx = 0; buffIdx < stMyselfBefer.byBuffChangeNum; buffIdx++)
		{
			USR_INFO("[%s:%d] Myself Befer BuffFlag = %s， BUFFID = %u", MSG_MARK, stMyselfBefer.astBuffChangeInfo[buffIdx].wAddDelFlag == 0 ? "新增" : "删除", stMyselfBefer.astBuffChangeInfo[buffIdx].byBuffID);
		}

		for ( map<INT32, INT32>::iterator itr = HPMap.begin(); itr != HPMap.end(); itr++)
		{
			USR_INFO("Befer Myself FormationIdx=%u, HPChange=%d", itr->first, itr->second);
		}
		HPMap.clear();
		const DT_ATTACK_OBJ_DATA3& stMyselfAfter = stBattleData4.astBattleRoundInfo[byIdx].stAttackData.stAfterAttack.stMyselfObjInfo;
		for( byHPIdx = 0; byHPIdx < stMyselfAfter.byHpChangeNum; byHPIdx++)
		{
			HPMap[stMyselfAfter.astHpChangeInfo[byHPIdx].byFormationIdx] += stMyselfAfter.astHpChangeInfo[byHPIdx].nHP;
			allMyselfHPChange[stMyselfAfter.astHpChangeInfo[byHPIdx].byFormationIdx] += stMyselfAfter.astHpChangeInfo[byHPIdx].nHP;
		}
		for ( UINT8 buffIdx = 0; buffIdx < stMyselfAfter.byBuffChangeNum; buffIdx++)
		{
			USR_INFO("[%s:%d] Myself After BuffFlag = %s， BUFFID = %u", MSG_MARK, stMyselfAfter.astBuffChangeInfo[buffIdx].wAddDelFlag == 0 ? "新增" : "删除", stMyselfAfter.astBuffChangeInfo[buffIdx].byBuffID);
		}

		for ( map<INT32, INT32>::iterator itr = HPMap.begin(); itr != HPMap.end(); itr++)
		{
			USR_INFO("After Myself FormationIdx=%u, HPChange=%d", itr->first, itr->second);
		}

		HPMap.clear();
		const DT_ATTACK_OBJ_DATA3& stEmenyfBefer = stBattleData4.astBattleRoundInfo[byIdx].stAttackData.stBeforeAttack.stEnemyObjInfo;
		for( byHPIdx = 0; byHPIdx < stEmenyfBefer.byHpChangeNum; byHPIdx++)
		{
			HPMap[stEmenyfBefer.astHpChangeInfo[byHPIdx].byFormationIdx] += stEmenyfBefer.astHpChangeInfo[byHPIdx].nHP;
			allEnemyHPChange[stEmenyfBefer.astHpChangeInfo[byHPIdx].byFormationIdx] += stEmenyfBefer.astHpChangeInfo[byHPIdx].nHP;
		}
		for ( map<INT32, INT32>::iterator itr = HPMap.begin(); itr != HPMap.end(); itr++)
		{
			USR_INFO("Befer Enemy FormationIdx=%u, HPChange=%d", itr->first, itr->second);
		}
		for ( UINT8 buffIdx = 0; buffIdx < stEmenyfBefer.byBuffChangeNum; buffIdx++)
		{
			USR_INFO("[%s:%d] EnenyBefer BuffFlag = %s， BUFFID = %u", MSG_MARK, stEmenyfBefer.astBuffChangeInfo[buffIdx].wAddDelFlag == 0 ? "新增" : "删除", stEmenyfBefer.astBuffChangeInfo[buffIdx].byBuffID);
		}
		HPMap.clear();
		const DT_ATTACK_OBJ_DATA3& stEmenyffAfter = stBattleData4.astBattleRoundInfo[byIdx].stAttackData.stAfterAttack.stEnemyObjInfo;
		for( byHPIdx = 0; byHPIdx < stEmenyffAfter.byHpChangeNum; byHPIdx++)
		{
			HPMap[stEmenyffAfter.astHpChangeInfo[byHPIdx].byFormationIdx] += stEmenyffAfter.astHpChangeInfo[byHPIdx].nHP;
			allEnemyHPChange[stEmenyffAfter.astHpChangeInfo[byHPIdx].byFormationIdx] += stEmenyffAfter.astHpChangeInfo[byHPIdx].nHP;
		}
		for ( UINT8 buffIdx = 0; buffIdx < stEmenyffAfter.byBuffChangeNum; buffIdx++)
		{
			USR_INFO("[%s:%d] EnenyAfter BuffFlag = %s， BUFFID = %u", MSG_MARK, stEmenyffAfter.astBuffChangeInfo[buffIdx].wAddDelFlag == 0 ? "新增" : "删除", stEmenyffAfter.astBuffChangeInfo[buffIdx].byBuffID);
		}
		for ( map<INT32, INT32>::iterator itr = HPMap.begin(); itr != HPMap.end(); itr++)
		{
			USR_INFO("After Enemy FormationIdx=%u, HPChange=%d", itr->first, itr->second);
		}
		USR_INFO( "Flag : %s, GodHurtValue = %u ", byFlag == 0 ? "Myself" : "Enemy", stBattleData4.astBattleRoundInfo[byIdx].stAttackData.stGodweaponAttackInfo.dwAttackHurtValue);
		USR_INFO( "AngryChangeNum = %u ",stBattleData4.astBattleRoundInfo[byIdx].stAttackData.stGodweaponAttackInfo.byAngryChangeNum);
	}

	USR_INFO("MyselfInfo:");
	for ( byIdx = 0; byIdx < stBattleData4.stMyselfInfo.byObjNum; byIdx++)
	{
		USR_INFO("Idx = %u, MaxHP=%u", stBattleData4.stMyselfInfo.astBattleObjInfo[byIdx].byFomationIdx, stBattleData4.stMyselfInfo.astBattleObjInfo[byIdx].stBattleAttr.dwHP);
	}
	USR_INFO("EmenyInfo:");
	for ( byIdx = 0; byIdx < stBattleData4.stEnemyInfo.byObjNum; byIdx++)
	{
		USR_INFO("Idx = %u, MaxHP=%u", stBattleData4.stEnemyInfo.astBattleObjInfo[byIdx].byFomationIdx, stBattleData4.stEnemyInfo.astBattleObjInfo[byIdx].stBattleAttr.dwHP);
	}
	USR_INFO("RoundNum = %u", stBattleData4.byBattleRoundNum);


	for ( map<INT32, INT32>::iterator itr = allMyselfHPChange.begin(); itr != allMyselfHPChange.end(); itr++)
	{
		USR_INFO("All Myself FormationIdx=%u, HPChange=%d", itr->first, itr->second);
	}

	for ( map<INT32, INT32>::iterator itr = allEnemyHPChange.begin(); itr != allEnemyHPChange.end(); itr++)
	{
		USR_INFO("All Enemy FormationIdx=%u, HPChange=%d", itr->first, itr->second);
	}
}


VOID CBattleMgr::ReplaceBattleData(const vector<UINT16>& vecMonsterID, DT_BATTLE_DATA2& stDT_BATTLE_DATA)
{
	stDT_BATTLE_DATA.stEnemyInfo.byObjNum = vecMonsterID.size();
	stDT_BATTLE_DATA.stEnemyInfo.astBattleObjInfo;
}

ERoundResult CBattleMgr::OnceRound3(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA4& stBattleRoundInfo, CBattleAttrObj*& poEnemy)
{
	//0 初始化
	memset(&stBattleRoundInfo, 0, sizeof(stBattleRoundInfo));

	DT_ATTACK_ALL_DATA4& stAttackData = stBattleRoundInfo.stAttackData;
	if(EAF_MYSELF == eAttackerFlag)
	{
		stBattleRoundInfo.byAttackerFlag = 0;//攻击者标志，0为我方
	}
	else
	{
		stBattleRoundInfo.byAttackerFlag = 1;//攻击者标志，1为敌方
	}

	CBattleAttrObj* poCurAttacker = poAttacker->GetRoundNextObj(&stAttackData.byAttackerIdx);
	if(NULL == poCurAttacker)//位置已经轮过一次
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		//DBG_INFO( _SDT("***GetRoundNextObj***"));
		return ERR_DEAD;
	}

	//1 buff作用
	BuffAffect(EBAP_ROUND_BEGIN, poAttacker, poBeattacker);

	//死亡判断 (中毒buff等影响)
	if(JudgeDead(poAttacker, poBeattacker))
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		//DBG_INFO( _SDT("***JudgeDead***"));
		return ERR_DEAD;
	}

	//2 动作判定
	ERoundResult eResult = ActionJudge(poAttacker);
	if(ERR_CONTINUE != eResult)
	{
		stAttackData.bySkipFlag = 1;//1跳过攻击
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

		//死亡判断
		if(JudgeDead(poAttacker, poBeattacker))
		{
			//DBG_INFO( _SDT("***JudgeDead2***"));
			eResult = ERR_DEAD;
		}
		//DBG_INFO( _SDT("***ActionJudge***"));
		return eResult;
	}

	vector<CBattleAttrObj*> vecTargetObj;
	//3 攻击者触发技能
	CSkill* poAttackerSkill = poAttacker->TrigerSkill(poCurAttacker->GetFormationIdx(), vecTargetObj);
	if(0 == vecTargetObj.size())//异常
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

		//DBG_INFO( _SDT("***TrigerSkill***"));
		return ERR_DEAD;
	}

	//4 技能影响
	if(poAttackerSkill)
	{
		stAttackData.wAttackSkillID = poCurAttacker->GetSkillActionID();
		poAttackerSkill->Use(ESAP_BEFORE_COUNT_HURT, poAttacker, poAttacker, poBeattacker, poCurAttacker, vecTargetObj);
	}
	else
	{
		//当处于反击或连击时神器攻击放在其后面
		if ( !(poCurAttacker->GetContinueAttack() || poCurAttacker->GetBeatBack()) )
		{
			//若有神器，普通攻击，则+怒气
			COtherLife* poOtherLife = poCurAttacker->GetOtherLife();
			if(poOtherLife)
			{
				poOtherLife->AddAngryChange(ROUND_ANGRY);
			}
		}
		
	}

	//获取攻击前回合数据
	GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
	CHero* poHero = dynamic_cast<CHero*>(poCurAttacker);
	if ( poHero )
	{
		poCurAttacker->AddExtHur(poHero->GetJewelryAttr(EJEA_NORMAL_ATTACK));//普通攻击伤害
	}
	//临时保存攻击数据，用于攻击多个玩家数据
	UINT16 wAttackHurtRate = poCurAttacker->GetAttackHurtRate();
	UINT32 dwExtHur = poCurAttacker->GetExtHur();
	UINT16 wSkillHurtRate = poCurAttacker->GetSkillHurtRate();
	UINT32 dwSkillExtHur = poCurAttacker->GetSkillExtHur();
	
	poEnemy = vecTargetObj[0];
	//6 格挡判断及伤害,
	
	for(vector<CBattleAttrObj*>::iterator itr = vecTargetObj.begin(); itr != vecTargetObj.end(); )
	{
		poCurAttacker->GetExtHur() = dwExtHur;
		poCurAttacker->GetAttackHurtRate() = wAttackHurtRate;
		poCurAttacker->GetSkillExtHur() = dwSkillExtHur;
		poCurAttacker->GetSkillHurtRate() = wSkillHurtRate;

		CBattleAttrObj* poTarget = *itr;
		//poCurAttacker->InitAttack();//初始化攻击
		poCurAttacker->CKParryHurt(poTarget);
		BOOL bMagicFlag = (*itr)->GetMagicImmumity();

		//74 计算伤害
		CountHurt(poCurAttacker, poTarget, &stAttackData.byAttackSpecialEffectFlag, &stAttackData.byEnemySpecialEffectFlag);

		DT_PARRY_DATA2& stDT_PARRY_DATA = stAttackData.astBeAttackerIdxInfo[stAttackData.byBeAttackerIdxNum++];
		stDT_PARRY_DATA.byFormationIdx = poTarget->GetFormationIdx();
		stDT_PARRY_DATA.byParryFlag = poTarget->GetParryFlag();
		stDT_PARRY_DATA.byBeCritFlag = poCurAttacker->GetCritFlag();
		stDT_PARRY_DATA.byDodge = poTarget->GetDodge();
		//闪避
		if ( 1 == stDT_PARRY_DATA.byDodge || (bMagicFlag && !(*itr)->GetMagicImmumity()) )
		{
			itr = vecTargetObj.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	//4 技能影响
	if(poAttackerSkill)
	{
		poAttackerSkill->Use(ESAP_HIT_HURT, poAttacker, poAttacker, poBeattacker, poCurAttacker, vecTargetObj);
	}

	//8 死亡判断
	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}
	//未死，判断是否需神器攻击
	if(ERR_DEAD != eResult && !poCurAttacker->GetContinueAttack())
	{
		if(poEnemy)
		{
			CkGodweaponAttack(poCurAttacker, poEnemy, stAttackData.stGodweaponAttackInfo);
		}
	}
	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}

	return eResult;
}

ERoundResult CBattleMgr::OnceRound4(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo, CBattleAttrObj*& poEnemy)
{
	//0 初始化
	memset(&stBattleRoundInfo, 0, sizeof(stBattleRoundInfo));

	DT_ATTACK_ALL_DATA5& stAttackData = stBattleRoundInfo.stAttackData;
	if(EAF_MYSELF == eAttackerFlag)
	{
		stBattleRoundInfo.byAttackerFlag = 0;//攻击者标志，0为我方
	}
	else
	{
		stBattleRoundInfo.byAttackerFlag = 1;//攻击者标志，1为敌方
	}

	CBattleAttrObj* poCurAttacker = poAttacker->GetRoundNextObj(&stAttackData.byAttackerIdx);
	if(NULL == poCurAttacker)//位置已经轮过一次
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		//DBG_INFO( _SDT("***GetRoundNextObj***"));
		return ERR_DEAD;
	}

	//1 buff作用
	BuffAffect(EBAP_ROUND_BEGIN, poAttacker, poBeattacker);

	//死亡判断 (中毒buff等影响)
	if(JudgeDead(poAttacker, poBeattacker))
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		//DBG_INFO( _SDT("***JudgeDead***"));
		return ERR_DEAD;
	}

	//2 动作判定
	ERoundResult eResult = ActionJudge(poAttacker);
	if(ERR_CONTINUE != eResult)
	{
		stAttackData.bySkipFlag = 1;//1跳过攻击
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

		//死亡判断
		if(JudgeDead(poAttacker, poBeattacker))
		{
			//DBG_INFO( _SDT("***JudgeDead2***"));
			eResult = ERR_DEAD;
		}
		//DBG_INFO( _SDT("***ActionJudge***"));
		return eResult;
	}

	vector<CBattleAttrObj*> vecTargetObj;
	//3 攻击者触发技能
	CSkill* poAttackerSkill = poAttacker->TrigerSkill(poCurAttacker->GetFormationIdx(), vecTargetObj);
	//有可能幻镜被沉睡 动作判定
	eResult = ActionJudge(poAttacker);
	if(ERR_CONTINUE != eResult)
	{
		stAttackData.bySkipFlag = 1;//1跳过攻击
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

		//死亡判断
		if(JudgeDead(poAttacker, poBeattacker))
		{
			//DBG_INFO( _SDT("***JudgeDead2***"));
			eResult = ERR_DEAD;
		}
		//DBG_INFO( _SDT("***ActionJudge***"));
		return eResult;
	}

	if(0 == vecTargetObj.size())//异常
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

		//DBG_INFO( _SDT("***TrigerSkill***"));
		return ERR_DEAD;
	}

	if ( (NULL == poAttackerSkill) && !poCurAttacker->GetCanNormalAttack())
	{
		stAttackData.bySkipFlag = 1;//1跳过攻击
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

		//死亡判断
		if(JudgeDead(poAttacker, poBeattacker))
		{
			eResult = ERR_DEAD;
		}
		return eResult;
	}
	if ( EBK_NONE == poCurAttacker->GetChaos() )
	{
		stAttackData.byAttackObj = EAT_ENEMY;
	}
	else if ( EBK_CHAOS == poCurAttacker->GetChaos())
	{
		BOOL bMyself = FALSE;
		for ( UINT8 byIdx = 1; byIdx <= MAX_FORMATION_IDX_NUM; byIdx++)
		{
			CBattleAttrObj* poMyself = poAttacker->GetFormationObj(byIdx);
			if ( poMyself == vecTargetObj[0])
			{
				bMyself = TRUE;
				break;
			}
		}
		stAttackData.byAttackObj = bMyself ? EAT_MYSELF : EAT_ENEMY;
	}
	else if ( EBK_DREAMLAND == poCurAttacker->GetChaos() )
	{
		stAttackData.byAttackObj = EAT_MYSELF;
	}
	else
	{
		stAttackData.byAttackObj = EAT_ENEMY;
	}

	//4 技能影响
	if(poAttackerSkill)
	{
		stAttackData.wAttackSkillID = poCurAttacker->GetSkillActionID();
		poAttackerSkill->Use(ESAP_BEFORE_COUNT_HURT, poAttacker, poAttacker, poBeattacker, poCurAttacker, vecTargetObj);
	}
	else
	{
		//当处于反击或连击时神器攻击放在其后面
		if ( !(poCurAttacker->GetContinueAttack() || poCurAttacker->GetBeatBack()) )
		{
			//若有神器，普通攻击，则+怒气
			COtherLife* poOtherLife = poCurAttacker->GetOtherLife();
			if(poOtherLife)
			{
				poOtherLife->AddAngryChange(ROUND_ANGRY);
			}
		}

	}

	//获取攻击前回合数据
	GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
	CHero* poHero = dynamic_cast<CHero*>(poCurAttacker);
	if ( poHero )
	{
		poCurAttacker->AddExtHur(poHero->GetJewelryAttr(EJEA_NORMAL_ATTACK));//普通攻击伤害
	}
	//临时保存攻击数据，用于攻击多个玩家数据
	UINT16 wAttackHurtRate = poCurAttacker->GetAttackHurtRate();
	UINT32 dwExtHur = poCurAttacker->GetExtHur();
	UINT16 wSkillHurtRate = poCurAttacker->GetSkillHurtRate();
	UINT32 dwSkillExtHur = poCurAttacker->GetSkillExtHur();

	poEnemy = vecTargetObj[0];
	//6 格挡判断及伤害,

	for(vector<CBattleAttrObj*>::iterator itr = vecTargetObj.begin(); itr != vecTargetObj.end(); )
	{
		poCurAttacker->GetExtHur() = dwExtHur;
		poCurAttacker->GetAttackHurtRate() = wAttackHurtRate;
		poCurAttacker->GetSkillExtHur() = dwSkillExtHur;
		poCurAttacker->GetSkillHurtRate() = wSkillHurtRate;

		CBattleAttrObj* poTarget = *itr;
		//poCurAttacker->InitAttack();//初始化攻击
		poCurAttacker->CKParryHurt(poTarget);
		BOOL bMagicFlag = (*itr)->GetMagicImmumity();

		//74 计算伤害
		CountHurt(poCurAttacker, poTarget, &stAttackData.byAttackSpecialEffectFlag, &stAttackData.byEnemySpecialEffectFlag);

		DT_PARRY_DATA2& stDT_PARRY_DATA = stAttackData.astBeAttackerIdxInfo[stAttackData.byBeAttackerIdxNum++];
		stDT_PARRY_DATA.byFormationIdx = poTarget->GetFormationIdx();
		stDT_PARRY_DATA.byParryFlag = poTarget->GetParryFlag();
		stDT_PARRY_DATA.byBeCritFlag = poCurAttacker->GetCritFlag();
		stDT_PARRY_DATA.byDodge = poTarget->GetDodge();
		//闪避
		if ( 1 == stDT_PARRY_DATA.byDodge || (bMagicFlag && !(*itr)->GetMagicImmumity()) )
		{
			itr = vecTargetObj.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	//4 技能影响
	if(poAttackerSkill)
	{
		poAttackerSkill->Use(ESAP_HIT_HURT, poAttacker, poAttacker, poBeattacker, poCurAttacker, vecTargetObj);
	}

	//8 死亡判断
	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}
	//未死，判断是否需神器攻击
	if(ERR_DEAD != eResult && !poCurAttacker->GetContinueAttack())
	{
		if(poEnemy)
		{
			CkGodweaponAttack(poCurAttacker, poEnemy, stAttackData.stGodweaponAttackInfo);
		}
	}
	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}

	return eResult;
}

VOID CBattleMgr::PetCountHurt(CPet* poAttack, CBattleAttrObj* poBeattacker)
{
	UINT32 dwHurtValue = poAttack->GetLingli() * poAttack->GetSkillHurtRate() / 100 + poAttack->GetSkillExtHur();

	dwHurtValue = dwHurtValue * poAttack->GetFinalAddHurt() / 100;
	CPetSkill* poPetSkill = poAttack->GetCurSkill();
	if ( NULL != poPetSkill)
	{
		if( (ESEK_SECOND_ATTACK == poPetSkill->GetSkillType()) && (poBeattacker->GetCurDT_BATTLE_ATTRIBUTE().qwHP < dwHurtValue ))
		{
			dwHurtValue = static_cast<UINT32>(poBeattacker->GetCurDT_BATTLE_ATTRIBUTE().qwHP - 1);
		}
	}

	poBeattacker->AddHPChange( -(INT32)dwHurtValue, ET_PET_SKILL, EHPCR_NORMAL_SKILL, 0);
	
	poAttack->GetLastHurt() = dwHurtValue;
}

ERoundResult CBattleMgr::PetSecondBattle(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo, vector<CBattleAttrObj*>& vecTargetObj)
{
	//0 初始化
	memset(&stBattleRoundInfo, 0, sizeof(stBattleRoundInfo));

	DT_ATTACK_ALL_DATA5& stAttackData = stBattleRoundInfo.stAttackData;

	//获取宠物
	CPet* poPet = poAttacker->GetBattlePet();
	if ( NULL == poPet)
	{
		return ERR_SKIP;
	}
	CPetSkill* poPetSkill = poPet->GetSecondHrut();
	if( NULL == poPetSkill)
	{
		return ERR_SKIP;
	}

	poPet->GetSecondHrut() = NULL;
	poAttacker->SetPetBattleState( TRUE );
	

	if(EAF_MYSELF == eAttackerFlag)
	{
		stBattleRoundInfo.byAttackerFlag = 0;//攻击者标志，0为我方
	}
	else
	{
		stBattleRoundInfo.byAttackerFlag = 1;//攻击者标志，1为敌方
	}

	//1 buff作用
	BuffAffect(EBAP_ROUND_BEGIN, poAttacker, poBeattacker);

	stAttackData.byPetAttack = ESM_YES;
	stAttackData.wPetSkillID = poPetSkill->GetID();

	if (vecTargetObj.size() == 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Pet Not Skill vecTargetObj size == 0!"), MSG_MARK);
		poAttacker->SetPetBattleState( FALSE );
		//获取攻击前回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		return ERR_SKIP;
	}

	//获取攻击前回合数据
	GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

	//技能BUFF增加
	poPetSkill->Use(ESAP_SECOND_ATTACK, poAttacker, poAttacker, poBeattacker, poPet, vecTargetObj);

	//判定死亡
	//8 死亡判断
	if(JudgeDead(poAttacker, poBeattacker))
	{
		poAttacker->SetPetBattleState( FALSE );
		return ERR_DEAD;
	}
	poAttacker->SetPetBattleState( FALSE );
	return ERR_CONTINUE;
}

ERoundResult CBattleMgr::PetBattle(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo, vector<CBattleAttrObj*>& vecTargetObj)
{
	//0 初始化
	memset(&stBattleRoundInfo, 0, sizeof(stBattleRoundInfo));

	DT_ATTACK_ALL_DATA5& stAttackData = stBattleRoundInfo.stAttackData;
	
	//获取宠物
	CPet* poPet = poAttacker->GetBattlePet();
	if ( NULL == poPet)
	{
		return ERR_SKIP;
	}
	poAttacker->SetPetBattleState( TRUE );
	
	//技能对象选定
	CPetSkill* poPetSkill = poPet->GetCurSkill();
	if ( NULL == poPetSkill)
	{
		poAttacker->SetPetBattleState( FALSE );
		return ERR_SKIP;
	}

	if(EAF_MYSELF == eAttackerFlag)
	{
		stBattleRoundInfo.byAttackerFlag = 0;//攻击者标志，0为我方
	}
	else
	{
		stBattleRoundInfo.byAttackerFlag = 1;//攻击者标志，1为敌方
	}

	//1 buff作用
	BuffAffect(EBAP_ROUND_BEGIN, poAttacker, poBeattacker);
	vecTargetObj.clear();

	
	//赋值配置（上古灵符随机，进行确认）
	poPetSkill->SetPetSkillProp();

	poPetSkill->GetTriger(vecTargetObj, poBeattacker);

	
	stAttackData.byPetAttack = ESM_YES;
	stAttackData.wPetSkillID = poPetSkill->GetID();
	//4 技能影响
	poPetSkill->Use(ESAP_BEFORE_COUNT_HURT, poAttacker, poAttacker, poBeattacker, poPet, vecTargetObj);

	if (vecTargetObj.size() == 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Pet Not Skill vecTargetObj size == 0!"), MSG_MARK);
		poAttacker->SetPetBattleState( FALSE );
		//获取攻击前回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		return ERR_SKIP;
	}

	//获取攻击前回合数据
	GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

	for (vector<CBattleAttrObj*>::iterator itr = vecTargetObj.begin(); itr != vecTargetObj.end(); itr++ )
	{
		CBattleAttrObj* poTarget = *itr;
		if ( NULL == poTarget)
		{
			continue;
		}
		DT_PARRY_DATA2& stDT_PARRY_DATA = stAttackData.astBeAttackerIdxInfo[stAttackData.byBeAttackerIdxNum++];
		stDT_PARRY_DATA.byFormationIdx = poTarget->GetFormationIdx();
		stDT_PARRY_DATA.byBeCritFlag = poPet->GetSkillCritical();
		//技能伤害确认
		PetCountHurt(poPet, *itr);
	}
	
	//技能BUFF增加
	poPetSkill->Use(ESAP_HIT_HURT, poAttacker, poAttacker, poBeattacker, poPet, vecTargetObj);

	poPet->EnterNextSkill();
	//判定死亡
	//8 死亡判断
	if(JudgeDead(poAttacker, poBeattacker))
	{
		poAttacker->SetPetBattleState( FALSE );
		return ERR_DEAD;
	}
	poAttacker->SetPetBattleState( FALSE );
	return ERR_CONTINUE;
}

//两次攻击
ERoundResult CBattleMgr::SecondAttack(CBattleObj* poAttacker, CBattleAttrObj* poCurAttacker, CBattleObj* poBeattacker, CBattleAttrObj* poTarget, EAttackerFlag& eAttackerFlag, CSpecialEffect* poSpecialEffect, DT_BATTLE_ROUND_DATA4& stBattleRoundInfo)
{
	//0 初始化
	memset(&stBattleRoundInfo, 0, sizeof(stBattleRoundInfo));

	DT_ATTACK_ALL_DATA4& stAttackData = stBattleRoundInfo.stAttackData;
	if(EAF_MYSELF == eAttackerFlag)
	{
		stBattleRoundInfo.byAttackerFlag = 0;//攻击者标志，0为我方
	}
	else
	{
		stBattleRoundInfo.byAttackerFlag = 1;//攻击者标志，1为敌方
	}

	if(NULL == poCurAttacker || NULL == poBeattacker)//位置已经轮过一次
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		//DBG_INFO( _SDT("***GetRoundNextObj***"));
		return ERR_DEAD;
	}

	stAttackData.byAttackerIdx = poCurAttacker->GetFormationIdx();

	//死掉换下一个目标
	if ( (NULL == poTarget) || (poTarget->IsDead()))
	{
		poTarget = poBeattacker->GetTarget(poCurAttacker->GetFormationIdx());
		if ( NULL == poTarget)
		{
			return ERR_DEAD;
		}
	}

	//获取攻击前回合数据
	GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);

	
	CHero* poHero = dynamic_cast<CHero*>(poCurAttacker);
	if ( poHero )
	{
		poCurAttacker->AddExtHur(poHero->GetJewelryAttr(EJEA_NORMAL_ATTACK));//普通攻击伤害
	}
	
	poCurAttacker->CKParryHurt(poTarget);

	//74 计算伤害
	SecondCountHurt(poCurAttacker, poTarget, poSpecialEffect, &stAttackData.byAttackSpecialEffectFlag, &stAttackData.byEnemySpecialEffectFlag);
	
	DT_PARRY_DATA2& stDT_PARRY_DATA = stAttackData.astBeAttackerIdxInfo[stAttackData.byBeAttackerIdxNum++];
	stDT_PARRY_DATA.byFormationIdx = poTarget->GetFormationIdx();
	stDT_PARRY_DATA.byParryFlag = poTarget->GetParryFlag();
	stDT_PARRY_DATA.byBeCritFlag = poCurAttacker->GetCritFlag();
	stDT_PARRY_DATA.byDodge = poTarget->GetDodge();

	ERoundResult eResult = ERR_CONTINUE;

	//8 死亡判断
	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}
	//未死，判断是否需神器攻击
	if(ERR_DEAD != eResult)
	{
		if(poTarget)
		{
			CkGodweaponAttack(poCurAttacker, poTarget, stAttackData.stGodweaponAttackInfo);
		}
	}

	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}

	return eResult;
}

//两次攻击
ERoundResult CBattleMgr::SecondAttack(CBattleObj* poAttacker, CBattleAttrObj* poCurAttacker, CBattleObj* poBeattacker, CBattleAttrObj* poTarget, EAttackerFlag& eAttackerFlag, CSpecialEffect* poSpecialEffect, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo)
{
	//0 初始化
	memset(&stBattleRoundInfo, 0, sizeof(stBattleRoundInfo));

	DT_ATTACK_ALL_DATA5& stAttackData = stBattleRoundInfo.stAttackData;
	if(EAF_MYSELF == eAttackerFlag)
	{
		stBattleRoundInfo.byAttackerFlag = 0;//攻击者标志，0为我方
	}
	else
	{
		stBattleRoundInfo.byAttackerFlag = 1;//攻击者标志，1为敌方
	}

	if(NULL == poCurAttacker || NULL == poBeattacker)//位置已经轮过一次
	{
		//获取回合数据
		GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);
		//DBG_INFO( _SDT("***GetRoundNextObj***"));
		return ERR_DEAD;
	}

	stAttackData.byAttackerIdx = poCurAttacker->GetFormationIdx();

	//死掉换下一个目标
	if ( (NULL == poTarget) || (poTarget->IsDead()))
	{
		poTarget = poBeattacker->GetTarget(poCurAttacker->GetFormationIdx());
		if ( NULL == poTarget)
		{
			return ERR_DEAD;
		}
	}

	//获取攻击前回合数据
	GetAttackData3(poAttacker, poBeattacker, eAttackerFlag, TRUE, stAttackData.stBeforeAttack);


	CHero* poHero = dynamic_cast<CHero*>(poCurAttacker);
	if ( poHero )
	{
		poCurAttacker->AddExtHur(poHero->GetJewelryAttr(EJEA_NORMAL_ATTACK));//普通攻击伤害
	}

	poCurAttacker->CKParryHurt(poTarget);

	//74 计算伤害
	SecondCountHurt(poCurAttacker, poTarget, poSpecialEffect, &stAttackData.byAttackSpecialEffectFlag, &stAttackData.byEnemySpecialEffectFlag);

	DT_PARRY_DATA2& stDT_PARRY_DATA = stAttackData.astBeAttackerIdxInfo[stAttackData.byBeAttackerIdxNum++];
	stDT_PARRY_DATA.byFormationIdx = poTarget->GetFormationIdx();
	stDT_PARRY_DATA.byParryFlag = poTarget->GetParryFlag();
	stDT_PARRY_DATA.byBeCritFlag = poCurAttacker->GetCritFlag();
	stDT_PARRY_DATA.byDodge = poTarget->GetDodge();

	ERoundResult eResult = ERR_CONTINUE;

	//8 死亡判断
	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}
	//未死，判断是否需神器攻击
	if(ERR_DEAD != eResult)
	{
		if(poTarget)
		{
			CkGodweaponAttack(poCurAttacker, poTarget, stAttackData.stGodweaponAttackInfo);
		}
	}

	if(JudgeDead(poAttacker, poBeattacker))
	{
		eResult = ERR_DEAD;
	}

	return eResult;
}

VOID CBattleMgr::CkGodweaponAttack(CBattleAttrObj* poAttacker, CBattleAttrObj* poBeattacker, DT_GODWEAPON_ATTACK_DATA& stGodweaponAttackInfo)
{
    memset(&stGodweaponAttackInfo, 0, sizeof(stGodweaponAttackInfo));
    //若有神器且满怒气，则神器攻击
    COtherLife* poOtherLife = poAttacker->GetOtherLife();
    if(poOtherLife)
    {
        if(poOtherLife->GetCurAngry() >= FULL_ANGRY)
        {
            poOtherLife->AddAngryChange(-poOtherLife->GetCurAngry());
            stGodweaponAttackInfo.byAttackFlag = 1;
			
            stGodweaponAttackInfo.dwAttackHurtValue = poOtherLife->GetHurtValue();
            poBeattacker->AddHPChange(-(INT32)(stGodweaponAttackInfo.dwAttackHurtValue), ET_NORMAL, EHPCR_NORMAL_SKILL, 0, FALSE);		//
        }
        vector<DT_ANGRY_DATA>& vecAngryChange = poOtherLife->GetAngryChange();
        UINT8 bySize = vecAngryChange.size();
        for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
        {
            memcpy(&stGodweaponAttackInfo.astAngryChangeInfo[stGodweaponAttackInfo.byAngryChangeNum++], &vecAngryChange[byIdx], sizeof(DT_ANGRY_DATA));
        }
        vecAngryChange.clear();
    }
}

VOID CBattleMgr::CkGodweaponAttack(CBattleAttrObj* poAttacker, CBattleAttrObj* poBeattacker, DT_GODWEAPON_ATTACK_DATA2& stGodweaponAttackInfo)
{
	memset(&stGodweaponAttackInfo, 0, sizeof(stGodweaponAttackInfo));
	//若有神器且满怒气，则神器攻击
	COtherLife* poOtherLife = poAttacker->GetOtherLife();
	if(poOtherLife)
	{
		if(poOtherLife->GetCurAngry() >= FULL_ANGRY)
		{
			poOtherLife->AddAngryChange(-poOtherLife->GetCurAngry());
			stGodweaponAttackInfo.byAttackFlag = 1;

			stGodweaponAttackInfo.dwAttackHurtValue = poOtherLife->GetHurtValue();
			stGodweaponAttackInfo.byEnemyFormationIdx = poBeattacker->GetFormationIdx();
			poBeattacker->AddHPChange(-(INT32)(stGodweaponAttackInfo.dwAttackHurtValue), ET_NORMAL, EHPCR_NORMAL_SKILL, 0, FALSE);		//
		}
		vector<DT_ANGRY_DATA>& vecAngryChange = poOtherLife->GetAngryChange();
		UINT8 bySize = vecAngryChange.size();
		for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
		{
			memcpy(&stGodweaponAttackInfo.astAngryChangeInfo[stGodweaponAttackInfo.byAngryChangeNum++], &vecAngryChange[byIdx], sizeof(DT_ANGRY_DATA));
		}
		vecAngryChange.clear();
	}
}


VOID CBattleMgr::BuffAffect(EBuffAffectPhase eBuffAffectPhase, CBattleObj* poAttacker, CBattleObj* poBeattacker)
{
    CBattleAttrObj* poObj = NULL;
    CBattleAttrObj* poCurAttacker = poAttacker->GetCurAttacker();
    CBattleAttrObj* poNormalAttackTarget = poBeattacker->GetTarget(poCurAttacker->GetFormationIdx());
    for(UINT8 byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        poObj = poAttacker->GetFormationObj(byIdx);
        if(poObj)
        {
            BuffAffect(eBuffAffectPhase, TRUE, poObj, poCurAttacker, poNormalAttackTarget);
        }
    }

    for(UINT8 byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        poObj = poBeattacker->GetFormationObj(byIdx);
        if(poObj)
        {
            BuffAffect(eBuffAffectPhase, FALSE, poObj, poCurAttacker, poNormalAttackTarget);
        }
    }
}

VOID CBattleMgr::BuffAffect(EBuffAffectPhase eBuffAffectPhase, BOOL bMyAttack, CBattleAttrObj* poAttrObj, CBattleAttrObj* poAttacker, CBattleAttrObj* poNormalAttackTarget)
{
    CBuffID2BuffMap& mapBuff = poAttrObj->GetAllBuff();
    for(CBuffID2BuffMapItr itr = mapBuff.begin(); itr != mapBuff.end(); itr++)
    {
        CBuff* poBuf = itr->second;
        if(NULL == poBuf)
        {
            continue;
        }

        poBuf->Affect(eBuffAffectPhase, bMyAttack, poAttacker, poNormalAttackTarget);
    }
}

//buff回合减少
VOID CBattleMgr::BuffRoundDec(CBattleObj* poBattleObj, BOOL bEndBigRound)
{
    CBattleAttrObj* poObj = NULL;
    for(UINT8 byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        poObj = poBattleObj->GetFormationObj(byIdx);
        if(poObj)
        {
            poObj->BuffRoundDec(bEndBigRound);
        }
    }
}

ERoundResult CBattleMgr::ActionJudge(CBattleObj* poAttacker)
{
    CBattleAttrObj* poObj = poAttacker->GetCurAttacker();
    if(NULL == poObj)//异常
    {
        return ERR_SKIP;
    }

    if(poObj->IsDead())
    {
        return ERR_SKIP;
    }

    if((!poObj->GetCanNormalAttack()) && (!poObj->GetCanSkillAttack()))
    {
        return ERR_SKIP;
    }

    return ERR_CONTINUE;
}

UINT32 CBattleMgr::CountHurt(CBattleAttrObj* poCurAttacker, CBattleAttrObj* poBeattacker, UINT8* pbyAttackSpecialEffectFlag, UINT8* pbyEnemySpecialEffectFlag)
{
    /*
    0906新的战斗计算公式为：
		基本攻击 = （攻击＊增伤－防御）
		技能额外攻击 = 基本攻击 * 技能伤害倍率＋技能固定伤害
    	最终伤害 = （基本攻击 + 技能额外攻击）＊（是否格挡＊０．５）＊（１－减伤）*（是否暴击＊１．５）
    */

    UINT8 byCritRate = 100;
    UINT8 byParryRate = 100;
	//暴击状态
    if(1 == poCurAttacker->GetCritFlag())
    {
        byCritRate = 150;
    }
	//抗暴状态
    if(1 == poBeattacker->GetParryFlag())
    {
        byParryRate = 50;
    }

	//减伤率计算
    UINT16 wDecHurtRate = poBeattacker->GetDecHurtRate();
    wDecHurtRate += poBeattacker->GetDT_EXT_BATTLE_ATTRIBUTE().wDecHurtRate;
    wDecHurtRate = wDecHurtRate > 99 ? 99 : wDecHurtRate;
    UINT8 byDecHRate = 100 - (UINT8)wDecHurtRate;

	//攻击计算
	//基本攻击
	UINT64 qwNormalHurt = poCurAttacker->GetCurAttack(poBeattacker) ;//（攻击＊增伤－防御)
	//技能加强攻击
	UINT64 qwSkillHurt = qwNormalHurt * ( poCurAttacker->GetSkillHurtRate() - 100) / 100 + poCurAttacker->GetSkillExtHur();

	UINT64 qwFinalHurt = (qwNormalHurt + qwSkillHurt) * byParryRate * byDecHRate *  byCritRate/ (100 * 100 * 100);
	
    qwFinalHurt = qwFinalHurt > poBeattacker->GetDefenseHP() ? qwFinalHurt - poBeattacker->GetDefenseHP() : 1;

	BOOL bIsSkill = 0 != qwSkillHurt;
	if ( !bIsSkill)
	{
		//普通攻击时加上基本额外伤害
		qwFinalHurt += poCurAttacker->GetExtHur();
	}
	CSpecialEffect* poSpecialEffect = poCurAttacker->GetSpecialEffectsByAttack( bIsSkill);
	CSpecialEffect* poDefendSpecialEffect = poBeattacker->GetSpecialEffectsByDefend(bIsSkill);
	if ( NULL == poSpecialEffect)
	{
		if ( NULL != pbyAttackSpecialEffectFlag)
		{
			*pbyAttackSpecialEffectFlag = EBSE_NONE;
		}
	}
	else
	{
		poSpecialEffect->Effect(qwFinalHurt, bIsSkill, poBeattacker, poDefendSpecialEffect, pbyAttackSpecialEffectFlag);
	}

	if ( NULL == poDefendSpecialEffect)
	{
		if ( NULL != pbyEnemySpecialEffectFlag)
		{
			*pbyEnemySpecialEffectFlag = EBSE_NONE;
		}
		
		//正常伤害
		poBeattacker->AddHPChange(-(INT32)qwFinalHurt, bIsSkill ? ET_SKILL : ET_NORMAL, EHPCR_NORMAL_SKILL, poCurAttacker->GetFormationIdx());
	}
	else
	{
		poDefendSpecialEffect->Effect(qwFinalHurt, bIsSkill, poCurAttacker, NULL, pbyEnemySpecialEffectFlag);
	}
	
    //SYS_CRITICAL( _SDT("Attacker Address:%0x, Beattacker Address:%0x AttackHurt:%u"), poCurAttacker, poBeattacker, dwAttackHurt);

    return (UINT32)qwFinalHurt;
}

UINT32 CBattleMgr::SecondCountHurt(CBattleAttrObj* poCurAttacker, CBattleAttrObj* poBeattacker, CSpecialEffect* poSpecialEffect, UINT8* pbyAttackSpecialEffectFlag , UINT8* pbyEnemySpecialEffectFlag)
{
	if ( NULL == poSpecialEffect)
	{
		return 0;
	}
	 /*
    0906新的战斗计算公式为：
		基本攻击 = （攻击＊增伤－防御）
		技能额外攻击 = 基本攻击 * 技能伤害倍率＋技能固定伤害
    	最终伤害 = （基本攻击 + 技能额外攻击）＊（是否格挡＊０．５）＊（１－减伤）*（是否暴击＊１．５）
    */

    UINT8 byCritRate = 100;
    UINT8 byParryRate = 100;
	//暴击状态
    if(1 == poCurAttacker->GetCritFlag())
    {
        byCritRate = 150;
    }
	//抗暴状态
    if(1 == poBeattacker->GetParryFlag())
    {
        byParryRate = 50;
    }

	//减伤率计算
    UINT16 wDecHurtRate = poBeattacker->GetDecHurtRate();
    wDecHurtRate += poBeattacker->GetDT_EXT_BATTLE_ATTRIBUTE().wDecHurtRate;
    wDecHurtRate = wDecHurtRate > 99 ? 99 : wDecHurtRate;
    UINT8 byDecHRate = 100 - (UINT8)wDecHurtRate;

	//攻击计算
	//基本攻击
	//
	UINT64 qwNormalHurt = poCurAttacker->GetCurAttack(poBeattacker, poSpecialEffect->GetEffectPercent()) ;//（攻击＊增伤－防御)
	//技能加强攻击
	UINT64 qwSkillHurt = qwNormalHurt * ( poCurAttacker->GetSkillHurtRate() - 100) / 100 + poCurAttacker->GetSkillExtHur();

	UINT64 qwFinalHurt = (qwNormalHurt + qwSkillHurt) * byParryRate * byDecHRate *  byCritRate/ (100 * 100 * 100);
	
    qwFinalHurt = qwFinalHurt > poBeattacker->GetDefenseHP() ? qwFinalHurt - poBeattacker->GetDefenseHP() : 1;

	BOOL bIsSkill = 0 != qwSkillHurt;

	if ( !bIsSkill)
	{
		//普通攻击时加上基本额外伤害
		qwFinalHurt += poCurAttacker->GetExtHur();
	}


	if ( NULL != pbyAttackSpecialEffectFlag)
	{
		*pbyAttackSpecialEffectFlag = poSpecialEffect->GetEffectSmallType();
	}

	CSpecialEffect* poDefendSpecialEffect = poBeattacker->GetSpecialEffectsByDefend(bIsSkill);

	if ( NULL == poDefendSpecialEffect)
	{
		if ( NULL != pbyEnemySpecialEffectFlag)
		{
			*pbyEnemySpecialEffectFlag = EBSE_NONE;
		}
		
		//正常伤害
		poBeattacker->AddHPChange(-(INT32)qwFinalHurt, ET_NORMAL, EHPCR_NORMAL_SKILL, poCurAttacker->GetFormationIdx());
	}
	else
	{
		poDefendSpecialEffect->Effect(qwFinalHurt, bIsSkill, poBeattacker, NULL, pbyEnemySpecialEffectFlag);
	}
	
    //SYS_CRITICAL( _SDT("Attacker Address:%0x, Beattacker Address:%0x AttackHurt:%u"), poCurAttacker, poBeattacker, dwAttackHurt);

    return (UINT32)qwFinalHurt;
}

/*
VOID CBattleMgr::CountAttackBackHurt(CBattleAttrObj* poAttacker, CBattleAttrObj* poBeattacker)
{
	UINT32 dwAttackHurt = poAttacker->m_dwAttackBackHurtValue;
	DT_BATTLE_ATTRIBUTE& stDT_BATTLE_ATTRIBUTE = poBeattacker->GetCurDT_BATTLE_ATTRIBUTE();
	if(stDT_BATTLE_ATTRIBUTE.dwHP >= dwAttackHurt)
	{
		stDT_BATTLE_ATTRIBUTE.dwHP -= dwAttackHurt;
	}
	else
	{
		stDT_BATTLE_ATTRIBUTE.dwHP = 0;
	}
}
*/

BOOL CBattleMgr::JudgeDead(CBattleObj* poAttacker, CBattleObj* poBeattacker)
{
    CBattleAttrObj* poObj = NULL;
    BOOL bDead = TRUE;
    for(UINT8 byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        poObj = poAttacker->GetFormationObj(byIdx);
        if(poObj)
        {
            if(!poObj->IsDead())
            {
                bDead = FALSE;
                break;
            }
        }
    }
    if(bDead)
    {
        return TRUE;
    }

    bDead = TRUE;
    for(UINT8 byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        poObj = poBeattacker->GetFormationObj(byIdx);
        if(poObj)
        {
            if(!poObj->IsDead())
            {
                bDead = FALSE;
                break;
            }
        }
    }

    return bDead;
}

VOID CBattleMgr::GetAttackData(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag eAttackerFlag, BOOL bBeforeAttack, DT_ATTACK_DATA& stDT_ATTACK_DATA)
{
    UINT8 byIdx = 0;
    for(byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = poAttacker->GetFormationObj(byIdx);
        if(poObj)
        {
            if(poObj->CanGetData())
            {
                DT_ATTACK_OBJ_DATA* pstDT_ATTACK_OBJ_DATA = NULL;
                if(EAF_MYSELF == eAttackerFlag)
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stMyselfObjInfo);
                }
                else
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stEnemyObjInfo);
                }

                GetAttackData(poObj, bBeforeAttack, *pstDT_ATTACK_OBJ_DATA);               
            }
        }
    }

    for(byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = poBeattacker->GetFormationObj(byIdx);
        if(poObj)
        {
            if(poObj->CanGetData())
            {
                DT_ATTACK_OBJ_DATA* pstDT_ATTACK_OBJ_DATA = NULL;
                if(EAF_MYSELF != eAttackerFlag)
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stMyselfObjInfo);
                }
                else
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stEnemyObjInfo);
                }

                GetAttackData(poObj, bBeforeAttack, *pstDT_ATTACK_OBJ_DATA);
            }
        }
    }
}

VOID CBattleMgr::GetAttackData2(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag eAttackerFlag, BOOL bBeforeAttack, DT_ATTACK_DATA2& stDT_ATTACK_DATA)
{
    UINT8 byIdx = 0;
    for(byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = poAttacker->GetFormationObj(byIdx);
        if(poObj)
        {
            if(poObj->CanGetData())
            {
                DT_ATTACK_OBJ_DATA2* pstDT_ATTACK_OBJ_DATA = NULL;
                if(EAF_MYSELF == eAttackerFlag)
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stMyselfObjInfo);
                }
                else
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stEnemyObjInfo);
                }
                
                GetAttackData2(poObj, bBeforeAttack, *pstDT_ATTACK_OBJ_DATA);                
            }
        }
    }

    for(byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = poBeattacker->GetFormationObj(byIdx);
        if(poObj)
        {
            if(poObj->CanGetData())
            {
                DT_ATTACK_OBJ_DATA2* pstDT_ATTACK_OBJ_DATA = NULL;
                if(EAF_MYSELF != eAttackerFlag)
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stMyselfObjInfo);
                }
                else
                {
                    pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stEnemyObjInfo);
                }
                
                GetAttackData2(poObj, bBeforeAttack, *pstDT_ATTACK_OBJ_DATA);                
            }
        }
    }
}

VOID CBattleMgr::GetAttackData3(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag eAttackerFlag, BOOL bBeforeAttack, DT_ATTACK_DATA3& stDT_ATTACK_DATA)
{
	UINT8 byIdx = 0;
	for(byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
	{
		CBattleAttrObj* poObj = poAttacker->GetFormationObj(byIdx);
		if(poObj)
		{
			if(poObj->CanGetData())
			{
				DT_ATTACK_OBJ_DATA3* pstDT_ATTACK_OBJ_DATA = NULL;
				if(EAF_MYSELF == eAttackerFlag)
				{
					pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stMyselfObjInfo);
				}
				else
				{
					pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stEnemyObjInfo);
				}

				GetAttackData3(poObj, bBeforeAttack, *pstDT_ATTACK_OBJ_DATA);                
			}
		}
	}

	for(byIdx = 1; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
	{
		CBattleAttrObj* poObj = poBeattacker->GetFormationObj(byIdx);
		if(poObj)
		{
			if(poObj->CanGetData())
			{
				DT_ATTACK_OBJ_DATA3* pstDT_ATTACK_OBJ_DATA = NULL;
				if(EAF_MYSELF != eAttackerFlag)
				{
					pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stMyselfObjInfo);
				}
				else
				{
					pstDT_ATTACK_OBJ_DATA = &(stDT_ATTACK_DATA.stEnemyObjInfo);
				}

				GetAttackData3(poObj, bBeforeAttack, *pstDT_ATTACK_OBJ_DATA);                
			}
		}
	}
}

VOID CBattleMgr::GetAttackData(CBattleAttrObj* poObj, BOOL bBeforeAttack, DT_ATTACK_OBJ_DATA& stDT_ATTACK_OBJ_DATA)
{
    vector<DT_BUFF_DATA>& vecBuffChange = poObj->GetBuffChange();
    UINT8 bySize = vecBuffChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byBuffChangeNum < MAX_BUFF_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astBuffChangeInfo[stDT_ATTACK_OBJ_DATA.byBuffChangeNum++], &vecBuffChange[byIdx], sizeof(DT_BUFF_DATA));
    }

    vector<DT_SKILL_EFFECT_ID_DATA>& vecSkillEffectIDChange = poObj->GetSkillEffectIDChange();
    bySize = vecSkillEffectIDChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.bySkillEffectIDChangeNum < MAX_BUFF_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astSkillEffectIDChangeInfo[stDT_ATTACK_OBJ_DATA.bySkillEffectIDChangeNum++], &vecSkillEffectIDChange[byIdx], sizeof(DT_SKILL_EFFECT_ID_DATA));
    }

    vector<DT_HP_DATA>& vecHPChange = poObj->GetHPChange();
    bySize = vecHPChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byHpChangeNum < MAX_HP_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astHpChangeInfo[stDT_ATTACK_OBJ_DATA.byHpChangeNum++], &vecHPChange[byIdx], sizeof(DT_HP_DATA));
    }

    vector<DT_ANGRY_DATA>& vecAngryChange = poObj->GetAngryChange();
    bySize = vecAngryChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byAngryChangeNum < MAX_ANGRY_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astAngryChangeInfo[stDT_ATTACK_OBJ_DATA.byAngryChangeNum++], &vecAngryChange[byIdx], sizeof(DT_ANGRY_DATA));
    }

    //清除变化
    vecBuffChange.clear();
    vecHPChange.clear();
    vecAngryChange.clear();
    vecSkillEffectIDChange.clear();

    //已死，下次不可再取它的数据
    if(poObj->IsDead() && (!bBeforeAttack))
    {
        poObj->CanGetData() = FALSE;
    }
}


VOID CBattleMgr::SetChangeHP(UINT8& byNum, INT32 anHpChangeInfo[], vector<INT32>& vecHP, BOOL bReverse)
{
    byNum = 0;
    UINT8 bySize = vecHP.size();
    //需反序
    if(bReverse)
    {
        INT8 cIdx = (INT8)bySize - 1;
        for(; cIdx >= 0; cIdx--)
        {
            if(0 == vecHP[cIdx])
            {
                continue;
            }
            anHpChangeInfo[byNum++] = vecHP[cIdx];
        }
    }
    else
    {
        for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
        {
            if(0 == vecHP[byIdx])
            {
                continue;
            }
            anHpChangeInfo[byNum++] = vecHP[byIdx];
        }
    }
}


INT32 CBattleMgr::GetTotalChangeHP(vector<INT32>& vecHP)
{
    INT32 nTotal = 0;
    UINT16 wSize = vecHP.size();
    for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
    {
        nTotal += vecHP[wIdx];
    }

    return nTotal;
}


VOID CBattleMgr::BattleData221(DT_BATTLE_DATA2& stBattleInfo2, DT_BATTLE_DATA& stBattleInfo1)
{
    stBattleInfo1.stMyselfInfo.byObjNum = stBattleInfo2.stMyselfInfo.byObjNum;
    SDStrcpy(stBattleInfo1.stMyselfInfo.aszDispName, stBattleInfo2.stMyselfInfo.aszDispName);
    stBattleInfo1.stMyselfInfo.dwPlayerID = stBattleInfo2.stMyselfInfo.dwPlayerID;
    UINT8 byObjNum = stBattleInfo1.stMyselfInfo.byObjNum;
    for(UINT8 byIdx = 0; byIdx < byObjNum; byIdx++)
    {
        DT_BATTLE_OBJ_DATA& stDT_BATTLE_OBJ_DATA1 = stBattleInfo1.stMyselfInfo.astBattleObjInfo[byIdx];
        DT_BATTLE_OBJ_DATA2& stDT_BATTLE_OBJ_DATA2 = stBattleInfo2.stMyselfInfo.astBattleObjInfo[byIdx];
        memcpy(&stDT_BATTLE_OBJ_DATA1, &stDT_BATTLE_OBJ_DATA2, sizeof(DT_BATTLE_OBJ_DATA));
    }

    stBattleInfo1.stEnemyInfo.byObjNum = stBattleInfo2.stEnemyInfo.byObjNum;
    SDStrcpy(stBattleInfo1.stEnemyInfo.aszDispName, stBattleInfo2.stEnemyInfo.aszDispName);
    stBattleInfo1.stEnemyInfo.dwPlayerID = stBattleInfo2.stEnemyInfo.dwPlayerID;
    byObjNum = stBattleInfo1.stEnemyInfo.byObjNum;
    for(UINT8 byIdx = 0; byIdx < byObjNum; byIdx++)
    {
        DT_BATTLE_OBJ_DATA& stDT_BATTLE_OBJ_DATA1 = stBattleInfo1.stEnemyInfo.astBattleObjInfo[byIdx];
        DT_BATTLE_OBJ_DATA2& stDT_BATTLE_OBJ_DATA2 = stBattleInfo2.stEnemyInfo.astBattleObjInfo[byIdx];
        memcpy(&stDT_BATTLE_OBJ_DATA1, &stDT_BATTLE_OBJ_DATA2, sizeof(DT_BATTLE_OBJ_DATA));
    }

    stBattleInfo1.byBattleResult = stBattleInfo2.byBattleResult;
    stBattleInfo1.byScore = stBattleInfo2.byScore;
    stBattleInfo1.byBattleRoundNum = stBattleInfo2.byBattleRoundNum;
    memcpy(&stBattleInfo1.stMyselfInfoAfterBattle, &stBattleInfo2.stMyselfInfoAfterBattle, sizeof(stBattleInfo2.stMyselfInfoAfterBattle));

    UINT8 byBattleRoundNum = stBattleInfo2.byBattleRoundNum;
    for(UINT8 byIdx = 0; byIdx < byBattleRoundNum; byIdx++)
    {
        DT_BATTLE_ROUND_DATA& stDT_BATTLE_ROUND_DATA = stBattleInfo1.astBattleRoundInfo[byIdx];
        DT_BATTLE_ROUND_DATA2& stDT_BATTLE_ROUND_DATA2 = stBattleInfo2.astBattleRoundInfo[byIdx];
        stDT_BATTLE_ROUND_DATA.byAttackerFlag = stDT_BATTLE_ROUND_DATA2.byAttackerFlag;
        memcpy(&stDT_BATTLE_ROUND_DATA.stAttackData, &stDT_BATTLE_ROUND_DATA2.stAttackData, sizeof(DT_BATTLE_ROUND_DATA));
        DT_GODWEAPON_ATTACK_DATA& stGodweaponAttackInfo =  stDT_BATTLE_ROUND_DATA2.stAttackData.stGodweaponAttackInfo;
        DT_ATTACK_OBJ_DATA* pstObjInfo = NULL;
        if((1 == stGodweaponAttackInfo.byAttackFlag) && (0 != stGodweaponAttackInfo.dwAttackHurtValue))//神器攻击，则将攻击力加到第一个伤害值上面
        {
            if(0 == stDT_BATTLE_ROUND_DATA2.byAttackerFlag) //攻击者标志，0为我方
            {
                pstObjInfo = &stBattleInfo1.astBattleRoundInfo[byIdx].stAttackData.stAfterAttack.stEnemyObjInfo;
            }
            else
            {
                pstObjInfo = &stBattleInfo1.astBattleRoundInfo[byIdx].stAttackData.stAfterAttack.stMyselfObjInfo;
            }

            pstObjInfo->astHpChangeInfo[0].nHP += -(INT32)(stGodweaponAttackInfo.dwAttackHurtValue);
            pstObjInfo->byHpChangeNum = pstObjInfo->byHpChangeNum == 0 ? 1 : pstObjInfo->byHpChangeNum;
        }
    }

}

UINT16 CBattleMgr::BattleMyself(CPlayer* poMyself, UINT8 byCareerID, DT_BATTLE_DATA5& stDT_BATTLE_DATA, DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfInfo)
{
    if(NULL == poMyself)
    {
        RETURN_OTHER_ERR;
    }
    DT_PLAYER_DATA stDT_PLAYER_DATA = {0};
    if(!CPlayerSaveMgr::Instance()->SavePlayerData(poMyself, ESDBT_GAME, &stDT_PLAYER_DATA))
    {
        RETURN_OTHER_ERR;
    }

    UINT8 bySrcCareerID = (UINT8)(poMyself->GetCoachHeroKindID());
    //修改数据//
    //修改整型中的位置
	DT_FORMATION_DATA stFormationInfo = {0};
	CFormationMgr::Instance()->ConvertFormation(stFormationInfo, stDT_PLAYER_DATA.stPlayerNoPvpData.stFormationInfo, poMyself);
    for(UINT8 byFormationKindNum = 0; byFormationKindNum < stFormationInfo.stFormationList.byFormationKindNum; byFormationKindNum++)
    {
        DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationInfo.stFormationList.astFormationInfo[byFormationKindNum];
        for(UINT8 byOpenIdxNum = 0; byOpenIdxNum < stDT_FORMATION_IDX_DATA.byOpenIdxNum; byOpenIdxNum++)
        {
            DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdxNum];
            if(stDT_FORMATION_HERO_DATA.wHeroID == bySrcCareerID)
            {
                stDT_FORMATION_HERO_DATA.wHeroID = byCareerID;
            }
        }
    }
    //修改主伙伴
    DT_HERO_DATA& stHeroData = stDT_PLAYER_DATA.stPlayerNoPvpData.stHeroData.astHeroInfoLst[0];
    stHeroData.stHeroBaseData.wKindID = byCareerID;
    //修改穿戴装备的HeroID
    for(UINT8 byIdx = 0; byIdx < stHeroData.stEquipDataInfo.wEquipNum; byIdx++)
    {
        DT_EQUIP_DATA& stDT_EQUIP_DATA = stHeroData.stEquipDataInfo.astEquipList[byIdx];
        if(stDT_EQUIP_DATA.wHeroKindID == bySrcCareerID)
        {
            stDT_EQUIP_DATA.wHeroKindID = byCareerID;
        }
    }
    //修改神器
    stHeroData.stGodweaponInfo.wHeroKindID = byCareerID;

    //修改技能
    //目前只有1个技能
    DT_SKILL_DATA& stDT_SKILL_DATA = stHeroData.stSkillInfo.astSkillInfo[0];
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(byCareerID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, byCareerID);
        return ERR_CHALLENGE_MYSELF_INFO::ID_CAREERID_NOT_EXIST;
    }


    SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(pstHeroProp->wSkillActionID, stDT_SKILL_DATA.wActionLevel);
    if(NULL == pstSkillActionProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstSkillActionProp[%d.%d] is NULL!"), MSG_MARK, pstHeroProp->wSkillActionID, stDT_SKILL_DATA.wActionLevel);
        RETURN_OTHER_ERR;
    }
    stHeroData.stSkillInfo.astSkillInfo[0].wActionID = pstHeroProp->wSkillActionID;

    CPlayer* poEnemy = CPlayerMgr::Instance()->CreatePlayer(0);
    if(NULL == poEnemy)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreatePlayer failed!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }

    DT_PLAYER_BASE_DATA& stBaseData = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData;
    stBaseData.dwID = 0;
    SDStrcpy(stBaseData.aszDispName, SDGBK2UTF8("myself to die!").c_str());
    //设置临时玩家对象
    if(!poEnemy->Init(stDT_PLAYER_DATA))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Player Init failed!PlayerID[%d]."), MSG_MARK, 0);
        CPlayerMgr::Instance()->RemovePlayer(0);
        RETURN_OTHER_ERR;
    }

    if(!Battle4(poMyself, poEnemy, stDT_BATTLE_DATA, TRUE, TRUE, pstAfterMyselfInfo))
    {
        CPlayerMgr::Instance()->RemovePlayer(0);//清除临时对象
        RETURN_OTHER_ERR;
    }


    return ERR_CHALLENGE_MYSELF_INFO::ID_SUCCESS;
}

DT_BATTLE_ATTRIBUTE& CBattleMgr::NewToOldBattleAttribute(const DT_BATTLE_BASE_ATTRIBUTE& stAttr2)
{
	static DT_BATTLE_ATTRIBUTE stAttr;
	memset(&stAttr, 0, sizeof(DT_BATTLE_ATTRIBUTE));

    stAttr.dwAttack = stAttr2.dwAttack;
    stAttr.dwHP = (UINT32)stAttr2.qwHP;
	return stAttr;
}

DT_BATTLE_ATTRIBUTE& CBattleMgr::NewToOldBattleAttribute(const DT_BATTLE_ATTRIBUTE2& stAttr2)
{
	static DT_BATTLE_ATTRIBUTE stAttr;
	memset(&stAttr, 0, sizeof(DT_BATTLE_ATTRIBUTE));

	stAttr.dwAttack = stAttr2.dwAttack;
	stAttr.dwHP = stAttr2.dwHP;
	return stAttr;
}

DT_BATTLE_ATTRIBUTE2 CBattleMgr::ConvertAttribute2ToBase(const DT_BATTLE_BASE_ATTRIBUTE& stBaseAttr)
{
	static DT_BATTLE_ATTRIBUTE2 stAttr;
	memset(&stAttr, 0, sizeof(stAttr));

	stAttr.dwAttack = stBaseAttr.dwAttack;
	stAttr.dwHP = (UINT32)stBaseAttr.qwHP;
	stAttr.dwDefend = stBaseAttr.dwDefend;
	return stAttr;
}

VOID CBattleMgr::GetAttackData2( CBattleAttrObj* poObj, BOOL bBeforeAttack, DT_ATTACK_OBJ_DATA2& stDT_ATTACK_OBJ_DATA)
{
    vector<DT_BUFF_DATA>& vecBuffChange = poObj->GetBuffChange();
    UINT8 bySize = vecBuffChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byBuffChangeNum < MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astBuffChangeInfo[stDT_ATTACK_OBJ_DATA.byBuffChangeNum++], &vecBuffChange[byIdx], sizeof(DT_BUFF_DATA));
    }

    vector<DT_SKILL_EFFECT_ID_DATA>& vecSkillEffectIDChange = poObj->GetSkillEffectIDChange();
    bySize = vecSkillEffectIDChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.bySkillEffectIDChangeNum < MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astSkillEffectIDChangeInfo[stDT_ATTACK_OBJ_DATA.bySkillEffectIDChangeNum++], &vecSkillEffectIDChange[byIdx], sizeof(DT_SKILL_EFFECT_ID_DATA));
    }

    vector<DT_HP_DATA>& vecHPChange = poObj->GetHPChange();
    bySize = vecHPChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byHpChangeNum < MAX_HP_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astHpChangeInfo[stDT_ATTACK_OBJ_DATA.byHpChangeNum++], &vecHPChange[byIdx], sizeof(DT_HP_DATA));
    }

    vector<DT_ANGRY_DATA>& vecAngryChange = poObj->GetAngryChange();
    bySize = vecAngryChange.size();
    for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byAngryChangeNum < MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND; byIdx++)
    {
        memcpy(&stDT_ATTACK_OBJ_DATA.astAngryChangeInfo[stDT_ATTACK_OBJ_DATA.byAngryChangeNum++], &vecAngryChange[byIdx], sizeof(DT_ANGRY_DATA));
    }

    //清除变化
    vecBuffChange.clear();
    vecHPChange.clear();
    vecAngryChange.clear();
    vecSkillEffectIDChange.clear();

    //已死，下次不可再取它的数据
    if(poObj->IsDead() && (!bBeforeAttack))
    {
        poObj->CanGetData() = FALSE;
    }
}

VOID CBattleMgr::GetAttackData3(CBattleAttrObj* poObj, BOOL bBeforeAttack, DT_ATTACK_OBJ_DATA3& stDT_ATTACK_OBJ_DATA)
{
	vector<DT_BUFF_DATA>& vecBuffChange = poObj->GetBuffChange();
	UINT8 bySize = vecBuffChange.size();
	for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byBuffChangeNum < MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND; byIdx++)
	{
		memcpy(&stDT_ATTACK_OBJ_DATA.astBuffChangeInfo[stDT_ATTACK_OBJ_DATA.byBuffChangeNum++], &vecBuffChange[byIdx], sizeof(DT_BUFF_DATA));
	}

	vector<DT_SKILL_EFFECT_ID_DATA>& vecSkillEffectIDChange = poObj->GetSkillEffectIDChange();
	bySize = vecSkillEffectIDChange.size();
	for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.bySkillEffectIDChangeNum < MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND; byIdx++)
	{
		memcpy(&stDT_ATTACK_OBJ_DATA.astSkillEffectIDChangeInfo[stDT_ATTACK_OBJ_DATA.bySkillEffectIDChangeNum++], &vecSkillEffectIDChange[byIdx], sizeof(DT_SKILL_EFFECT_ID_DATA));
	}

	vector<DT_HP_DATA2>& vecHPChange = poObj->GetHPChange2();
	bySize = vecHPChange.size();
	for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byHpChangeNum < MAX_HP_CHANGE_TIMES_PER_ROUND; byIdx++)
	{
		memcpy(&stDT_ATTACK_OBJ_DATA.astHpChangeInfo[stDT_ATTACK_OBJ_DATA.byHpChangeNum++], &vecHPChange[byIdx], sizeof(DT_HP_DATA2));
	}

	vector<DT_ANGRY_DATA>& vecAngryChange = poObj->GetAngryChange();
	bySize = vecAngryChange.size();
	for(UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byAngryChangeNum < MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND; byIdx++)
	{
		memcpy(&stDT_ATTACK_OBJ_DATA.astAngryChangeInfo[stDT_ATTACK_OBJ_DATA.byAngryChangeNum++], &vecAngryChange[byIdx], sizeof(DT_ANGRY_DATA));
	}

	vector<DT_SHIELD_DATA>& vecShieldChange = poObj->GetShiledChange();
	bySize = vecShieldChange.size();
	for ( UINT8 byIdx = 0; byIdx < bySize && stDT_ATTACK_OBJ_DATA.byShieldChangeNum < MAX_FORMATION_IDX_NUM; byIdx++)
	{
		memcpy(&stDT_ATTACK_OBJ_DATA.astShieldChangeInfo[stDT_ATTACK_OBJ_DATA.byShieldChangeNum++], &vecShieldChange[byIdx], sizeof(DT_SHIELD_DATA));
	}

	//清除变化
	vecBuffChange.clear();
	vecHPChange.clear();
	poObj->GetHPChange().clear();
	vecAngryChange.clear();
	vecSkillEffectIDChange.clear();
	vecShieldChange.clear();
	//已死，下次不可再取它的数据
	if(poObj->IsDead() && (!bBeforeAttack))
	{
		poObj->CanGetData() = FALSE;
	}
}
VOID CBattleMgr::Convert5To3(const DT_BATTLE_DATA5& stBattleData5, DT_BATTLE_DATA3& stBattleData3)
{
	DT_BATTLE_DATA4 stBattleData4 = {0};
	Convert5To4(stBattleData5, stBattleData4);
	Convert4To3(stBattleData4, stBattleData3);
}

VOID CBattleMgr::Convert5To4(const DT_BATTLE_DATA5& stBattleData5, DT_BATTLE_DATA4& stBattleData4)
{
	stBattleData4.byBattleResult = stBattleData5.byBattleResult;
	stBattleData4.byBattleRoundNum = stBattleData5.byBattleRoundNum;
	stBattleData4.byScore = stBattleData5.byScore;
	stBattleData4.stMyselfInfoAfterBattle = stBattleData5.stMyselfInfoAfterBattle;
	//敌方信息
	stBattleData4.stEnemyInfo.dwPlayerID = stBattleData5.stEnemyInfo.dwPlayerID;
	stBattleData4.stEnemyInfo.byObjNum = stBattleData5.stEnemyInfo.byObjNum;
	SDStrncpy(stBattleData4.stEnemyInfo.aszDispName, stBattleData5.stEnemyInfo.aszDispName, USERNAME_LEN - 1);
	memcpy(stBattleData4.stEnemyInfo.astBattleObjInfo, stBattleData5.stEnemyInfo.astBattleObjInfo, sizeof(stBattleData4.stEnemyInfo.astBattleObjInfo));
	//我方信息
	stBattleData4.stMyselfInfo.dwPlayerID = stBattleData5.stMyselfInfo.dwPlayerID;
	stBattleData4.stMyselfInfo.byObjNum = stBattleData5.stMyselfInfo.byObjNum;
	SDStrncpy(stBattleData4.stMyselfInfo.aszDispName, stBattleData5.stMyselfInfo.aszDispName, USERNAME_LEN - 1);
	memcpy(stBattleData4.stMyselfInfo.astBattleObjInfo, stBattleData5.stMyselfInfo.astBattleObjInfo, sizeof(stBattleData4.stMyselfInfo.astBattleObjInfo));
	UINT8 byBattleRoundNum = 0;
	for (UINT8 byIdx = 0; byIdx < stBattleData5.byBattleRoundNum; byIdx++)
	{
		memcpy(&stBattleData4.astBattleRoundInfo[byIdx], &stBattleData5.astBattleRoundInfo[byIdx], sizeof(stBattleData4.astBattleRoundInfo[byIdx]));
	}
}

VOID CBattleMgr::Convert4To3(const DT_BATTLE_DATA4& stData4, DT_BATTLE_DATA3& stData3)
{
	memcpy(&stData3.stMyselfInfo, &stData4.stMyselfInfo, sizeof(stData4.stMyselfInfo));
	memcpy(&stData3.stMyselfInfoAfterBattle, &stData4.stMyselfInfoAfterBattle, sizeof(stData3.stMyselfInfoAfterBattle));
	memcpy(&stData3.stEnemyInfo, &stData4.stEnemyInfo, sizeof(stData3.stEnemyInfo));
	stData3.byBattleResult = stData4.byBattleResult;
	stData3.byBattleRoundNum = stData4.byBattleRoundNum;
	stData3.byScore = stData4.byScore;
	for (UINT8 byIndex =0 ; byIndex < stData3.byBattleRoundNum && byIndex < MAX_BATTLE_ROUND_NUM; byIndex++)
	{
		//memcpy(&stBataData3.astBattleRoundInfo[byIndex],&stBataData2.astBattleRoundInfo[byIndex],sizeof(DT_BATTLE_ROUND_DATA2));
		stData3.astBattleRoundInfo[byIndex].byAttackerFlag = stData4.astBattleRoundInfo[byIndex].byAttackerFlag;
		stData3.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx = stData4.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx;
		stData3.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID = stData4.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID;
		stData3.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag = stData4.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag;
		stData3.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum = stData4.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum;

		memcpy(&stData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,&stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,
			sizeof(stData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo));
		vector<DT_HP_DATA> vecChange;
		vecChange.clear();
		for(UINT8 byIdx = 0; byIdx < stData3.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum; byIdx++)
		{
			stData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag;
			stData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx;
			stData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag;
			if ( stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byDodge )
			{
				DT_HP_DATA stHPData = {0};
				stHPData.byFormationIdx = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx;
				stHPData.nHP = 0;
				vecChange.push_back(stHPData);
			}
		}

		//攻击前
		DT_ATTACK_DATA2 &stDtData3 = stData3.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;
		const DT_ATTACK_DATA3 &stDtData4 = stData4.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;


		stDtData3.stEnemyObjInfo.byBuffChangeNum = stDtData4.stEnemyObjInfo.byBuffChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astBuffChangeInfo, stDtData4.stEnemyObjInfo.astBuffChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astBuffChangeInfo));


		stDtData3.stEnemyObjInfo.byHpChangeNum = stDtData4.stEnemyObjInfo.byHpChangeNum;
		UINT8 byIdx = 0;
		for ( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}

		stDtData3.stEnemyObjInfo.byAngryChangeNum = stDtData4.stEnemyObjInfo.byAngryChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astAngryChangeInfo, stDtData4.stEnemyObjInfo.astAngryChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astAngryChangeInfo));
		
		stDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtData4.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stDtData4.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));
		


		stDtData3.stMyselfObjInfo.byBuffChangeNum = stDtData4.stMyselfObjInfo.byBuffChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astBuffChangeInfo, stDtData4.stMyselfObjInfo.astBuffChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astBuffChangeInfo));


		stDtData3.stMyselfObjInfo.byHpChangeNum = stDtData4.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}

		stDtData3.stMyselfObjInfo.byAngryChangeNum = stDtData4.stMyselfObjInfo.byAngryChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astAngryChangeInfo, stDtData4.stMyselfObjInfo.astAngryChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astAngryChangeInfo));

		stDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtData4.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stDtData4.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));


		//攻击后
		DT_ATTACK_DATA2 &stAfterDtData3 = stData3.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;
		const DT_ATTACK_DATA3 &stAfterDtData4 = stData4.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;

		UINT8 byShieldChangeNum = 0;
		const DT_SHIELD_DATA* poShield = NULL;
		if( EAF_MYSELF == stData4.astBattleRoundInfo[byIndex].byAttackerFlag )
		{
			byShieldChangeNum = stAfterDtData4.stEnemyObjInfo.byShieldChangeNum;
			poShield = stAfterDtData4.stEnemyObjInfo.astShieldChangeInfo;
		}
		else
		{
			byShieldChangeNum = stAfterDtData4.stMyselfObjInfo.byShieldChangeNum;
			poShield = stAfterDtData4.stMyselfObjInfo.astShieldChangeInfo;
		}
		if ( NULL != poShield)
		{
			for ( byIdx = 0; byIdx < byShieldChangeNum; byIdx++)
			{
				if ( poShield[byIdx].byAddDelFlag )
				{
					DT_HP_DATA stHPData = {0};
					stHPData.byFormationIdx = poShield[byIdx].byFormationIdx;
					stHPData.nHP = 0;
					vecChange.push_back(stHPData);
				}
			}
		}
		stAfterDtData3.stEnemyObjInfo.byBuffChangeNum = stAfterDtData4.stEnemyObjInfo.byBuffChangeNum;
		memcpy(stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo, stAfterDtData4.stEnemyObjInfo.astBuffChangeInfo, sizeof(stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo));


		stAfterDtData3.stEnemyObjInfo.byHpChangeNum = stAfterDtData4.stEnemyObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}

		stAfterDtData3.stEnemyObjInfo.byAngryChangeNum = stAfterDtData4.stEnemyObjInfo.byAngryChangeNum;
		memcpy(stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo, stAfterDtData4.stEnemyObjInfo.astAngryChangeInfo, sizeof(stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo));

		stAfterDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stAfterDtData4.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stAfterDtData4.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));



		stAfterDtData3.stMyselfObjInfo.byBuffChangeNum = stAfterDtData4.stMyselfObjInfo.byBuffChangeNum;
		memcpy(stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo, stAfterDtData4.stMyselfObjInfo.astBuffChangeInfo, sizeof(stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo));


		stAfterDtData3.stMyselfObjInfo.byHpChangeNum = stAfterDtData4.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}

		stAfterDtData3.stMyselfObjInfo.byAngryChangeNum = stAfterDtData4.stMyselfObjInfo.byAngryChangeNum;
		memcpy(stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo, stAfterDtData4.stMyselfObjInfo.astAngryChangeInfo, sizeof(stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo));

		stAfterDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stAfterDtData4.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stAfterDtData4.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));

		for(vector<DT_HP_DATA>::iterator itr = vecChange.begin(); itr != vecChange.end(); itr++)
		{
			if( EAF_MYSELF == stData4.astBattleRoundInfo[byIndex].byAttackerFlag )
			{
				stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[stAfterDtData4.stEnemyObjInfo.byHpChangeNum] = *itr;
				stAfterDtData3.stEnemyObjInfo.byHpChangeNum++;
			}
			else
			{
				stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[stAfterDtData4.stMyselfObjInfo.byHpChangeNum] = *itr;
				stAfterDtData3.stMyselfObjInfo.byHpChangeNum++;
			}
		}
	}
}

VOID CBattleMgr::Convert4To2(const DT_BATTLE_DATA4& stData4, DT_BATTLE_DATA2& stData2)
{
	memcpy(&stData2.stMyselfInfo, &stData4.stMyselfInfo, sizeof(stData2.stMyselfInfo));
	memcpy(&stData2.stMyselfInfoAfterBattle, &stData4.stMyselfInfoAfterBattle, sizeof(stData2.stMyselfInfoAfterBattle));
	memcpy(&stData2.stEnemyInfo, &stData4.stEnemyInfo, sizeof(stData2.stEnemyInfo));
	stData2.byBattleResult = stData4.byBattleResult;
	stData2.byBattleRoundNum = stData4.byBattleRoundNum > MAX_BATTLE_ROUND_NUM ? MAX_BATTLE_ROUND_NUM : stData4.byBattleRoundNum;
	stData2.byScore = stData4.byScore;
	for (UINT8 byIndex =0 ; byIndex < stData2.byBattleRoundNum && byIndex < MAX_BATTLE_ROUND_NUM; byIndex++)
	{
		//memcpy(&stBataData3.astBattleRoundInfo[byIndex],&stBataData2.astBattleRoundInfo[byIndex],sizeof(DT_BATTLE_ROUND_DATA2));
		stData2.astBattleRoundInfo[byIndex].byAttackerFlag = stData4.astBattleRoundInfo[byIndex].byAttackerFlag;
		stData2.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx = stData4.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx;
		stData2.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID = stData4.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID;
		stData2.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag = stData4.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag;
		stData2.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum = stData4.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum;

		memcpy(&stData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,&stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,
			sizeof(stData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo));
		vector<DT_HP_DATA> vecChange;
		vecChange.clear();
		UINT8 byIdx = 0;
		for( byIdx = 0; (byIdx < stData2.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum) && (byIdx < MAX_FORMATION_IDX_NUM); byIdx++)
		{
			stData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag;
			stData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx;
			stData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag;
			if ( stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byDodge )
			{
				DT_HP_DATA stHPData = {0};
				stHPData.byFormationIdx = stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx;
				stHPData.nHP = 0;
				vecChange.push_back(stHPData);
			}
		}

		//攻击前
		DT_ATTACK_DATA &stDtData3 = stData2.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;
		const DT_ATTACK_DATA3 &stDtData4 = stData4.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;


		stDtData3.stEnemyObjInfo.byBuffChangeNum = stDtData4.stEnemyObjInfo.byBuffChangeNum > MAX_BUFF_CHANGE_TIMES_PER_ROUND ? MAX_BUFF_CHANGE_TIMES_PER_ROUND : stDtData4.stEnemyObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stEnemyObjInfo.astBuffChangeInfo[byIdx], &stDtData4.stEnemyObjInfo.astBuffChangeInfo[byIdx], sizeof(stDtData3.stEnemyObjInfo.astBuffChangeInfo[0]));
		}
		

		//乱方
		stDtData3.stEnemyObjInfo.byHpChangeNum = stDtData4.stEnemyObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}
		

		stDtData3.stEnemyObjInfo.byAngryChangeNum = stDtData4.stEnemyObjInfo.byAngryChangeNum > MAX_ANGRY_CHANGE_TIMES_PER_ROUND ? MAX_ANGRY_CHANGE_TIMES_PER_ROUND : stDtData4.stEnemyObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stEnemyObjInfo.astAngryChangeInfo[byIdx], &stDtData4.stEnemyObjInfo.astAngryChangeInfo[byIdx], sizeof(stDtData3.stEnemyObjInfo.astAngryChangeInfo[0]));
		}
		

		stDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtData4.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stDtData4.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));


		//我方
		stDtData3.stMyselfObjInfo.byBuffChangeNum = stDtData4.stMyselfObjInfo.byBuffChangeNum > MAX_BUFF_CHANGE_TIMES_PER_ROUND ? MAX_BUFF_CHANGE_TIMES_PER_ROUND : stDtData4.stMyselfObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stMyselfObjInfo.astBuffChangeInfo[byIdx], &stDtData4.stMyselfObjInfo.astBuffChangeInfo[byIdx], sizeof(stDtData3.stMyselfObjInfo.astBuffChangeInfo[0]));
		}
		

		stDtData3.stMyselfObjInfo.byHpChangeNum = stDtData4.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}

		stDtData3.stMyselfObjInfo.byAngryChangeNum = stDtData4.stMyselfObjInfo.byAngryChangeNum > MAX_ANGRY_CHANGE_TIMES_PER_ROUND ? MAX_ANGRY_CHANGE_TIMES_PER_ROUND : stDtData4.stMyselfObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stMyselfObjInfo.astAngryChangeInfo[byIdx], &stDtData4.stMyselfObjInfo.astAngryChangeInfo[byIdx], sizeof(stDtData3.stMyselfObjInfo.astAngryChangeInfo[0]));
		}

		stDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtData4.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stDtData4.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));


		//攻击后
		DT_ATTACK_DATA &stAfterDtData3 = stData2.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;
		const DT_ATTACK_DATA3 &stAfterDtData4 = stData4.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;


		stAfterDtData3.stEnemyObjInfo.byBuffChangeNum = stAfterDtData4.stEnemyObjInfo.byBuffChangeNum > MAX_BUFF_CHANGE_TIMES_PER_ROUND ? MAX_BUFF_CHANGE_TIMES_PER_ROUND : stDtData4.stEnemyObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo[byIdx], &stAfterDtData4.stEnemyObjInfo.astBuffChangeInfo[byIdx], sizeof(stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo[0]));
		}
		UINT8 byShieldChangeNum = 0;
		const DT_SHIELD_DATA* poShield = NULL;
		if( EAF_MYSELF == stData4.astBattleRoundInfo[byIndex].byAttackerFlag )
		{
			byShieldChangeNum = stAfterDtData4.stEnemyObjInfo.byShieldChangeNum;
			poShield = stAfterDtData4.stEnemyObjInfo.astShieldChangeInfo;
		}
		else
		{
			byShieldChangeNum = stAfterDtData4.stMyselfObjInfo.byShieldChangeNum;
			poShield = stAfterDtData4.stMyselfObjInfo.astShieldChangeInfo;
		}
		if ( NULL != poShield)
		{
			for ( byIdx = 0; byIdx < byShieldChangeNum; byIdx++)
			{
				if ( poShield[byIdx].byAddDelFlag )
				{
					DT_HP_DATA stHPData = {0};
					stHPData.byFormationIdx = poShield[byIdx].byFormationIdx;
					stHPData.nHP = 0;
					vecChange.push_back(stHPData);
				}
			}
		}

		//乱方
		stAfterDtData3.stEnemyObjInfo.byHpChangeNum = stAfterDtData4.stEnemyObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData4.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}


		stAfterDtData3.stEnemyObjInfo.byAngryChangeNum = stAfterDtData4.stEnemyObjInfo.byAngryChangeNum > MAX_ANGRY_CHANGE_TIMES_PER_ROUND ? MAX_ANGRY_CHANGE_TIMES_PER_ROUND : stAfterDtData4.stEnemyObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo[byIdx], &stAfterDtData4.stEnemyObjInfo.astAngryChangeInfo[byIdx], sizeof(stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo[0]));
		}


		stAfterDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stAfterDtData4.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stAfterDtData4.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));


		//我方
		stAfterDtData3.stMyselfObjInfo.byBuffChangeNum = stAfterDtData4.stMyselfObjInfo.byBuffChangeNum > MAX_BUFF_CHANGE_TIMES_PER_ROUND ? MAX_BUFF_CHANGE_TIMES_PER_ROUND : stAfterDtData4.stMyselfObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo[byIdx], &stAfterDtData4.stMyselfObjInfo.astBuffChangeInfo[byIdx], sizeof(stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo[0]));
		}


		stAfterDtData3.stMyselfObjInfo.byHpChangeNum = stAfterDtData4.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData4.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
		}

		stAfterDtData3.stMyselfObjInfo.byAngryChangeNum = stAfterDtData4.stMyselfObjInfo.byAngryChangeNum > MAX_ANGRY_CHANGE_TIMES_PER_ROUND ? MAX_ANGRY_CHANGE_TIMES_PER_ROUND : stAfterDtData4.stMyselfObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo[byIdx], &stAfterDtData4.stMyselfObjInfo.astAngryChangeInfo[byIdx], sizeof(stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo[0]));
		}

		stAfterDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stAfterDtData4.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stAfterDtData4.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));

		for(vector<DT_HP_DATA>::iterator itr = vecChange.begin(); itr != vecChange.end(); itr++)
		{
			if( EAF_MYSELF == stData4.astBattleRoundInfo[byIndex].byAttackerFlag )
			{
				stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[stAfterDtData4.stEnemyObjInfo.byHpChangeNum] = *itr;
				stAfterDtData3.stEnemyObjInfo.byHpChangeNum++;
			}
			else
			{
				stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[stAfterDtData4.stMyselfObjInfo.byHpChangeNum] = *itr;
				stAfterDtData3.stMyselfObjInfo.byHpChangeNum++;
			}
		}
	}
}

VOID CBattleMgr::Convert2To4(const DT_BATTLE_DATA2& stData2, DT_BATTLE_DATA4& stData4)
{
	memset(&stData4, 0, sizeof(DT_BATTLE_DATA4));
	memcpy(&stData4.stMyselfInfo, &stData2.stMyselfInfo, sizeof(stData4.stMyselfInfo));
	memcpy(&stData4.stMyselfInfoAfterBattle, &stData2.stMyselfInfoAfterBattle, sizeof(stData4.stMyselfInfoAfterBattle));
	memcpy(&stData4.stEnemyInfo, &stData2.stEnemyInfo, sizeof(stData4.stEnemyInfo));
	stData4.byBattleResult = stData2.byBattleResult;
	stData4.byBattleRoundNum = stData2.byBattleRoundNum;
	stData4.byScore = stData2.byScore;
	for (UINT8 byIndex =0 ; byIndex < stData4.byBattleRoundNum ; byIndex++)
	{
		stData4.astBattleRoundInfo[byIndex].byAttackerFlag = stData2.astBattleRoundInfo[byIndex].byAttackerFlag;
		stData4.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx = stData2.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx;
		stData4.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID = stData2.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID;
		stData4.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag = stData2.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag;
		stData4.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum = stData2.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum;

		stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAttackFlag = stData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAttackFlag;
		stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.dwAttackHurtValue = stData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.dwAttackHurtValue;
		stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAngryChangeNum = stData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAngryChangeNum;
		memcpy(stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.astAngryChangeInfo, stData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.astAngryChangeInfo,
			sizeof(stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.astAngryChangeInfo));
	
		UINT8 byIdx = 0;
		for( byIdx = 0; byIdx < stData4.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum; byIdx++)
		{
			stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag = stData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag;
			stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx = stData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx;
			stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag = stData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag;
		}

		//攻击前
		DT_ATTACK_DATA3 &stDtData3 = stData4.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;
		const DT_ATTACK_DATA &stDtData = stData2.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;


		stDtData3.stEnemyObjInfo.byBuffChangeNum = stDtData.stEnemyObjInfo.byBuffChangeNum;;
		for( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stEnemyObjInfo.astBuffChangeInfo[byIdx], &stDtData.stEnemyObjInfo.astBuffChangeInfo[byIdx], sizeof(stDtData3.stEnemyObjInfo.astBuffChangeInfo[0]));
		}

		stDtData3.stEnemyObjInfo.byHpChangeNum = stDtData.stEnemyObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stDtData.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
		}
		

		stDtData3.stEnemyObjInfo.byAngryChangeNum = stDtData.stEnemyObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stEnemyObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stEnemyObjInfo.astAngryChangeInfo[byIdx], &stDtData.stEnemyObjInfo.astAngryChangeInfo[byIdx], sizeof(stDtData3.stEnemyObjInfo.astAngryChangeInfo[0]));
		}


		stDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtData.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stDtData.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));


		//我方
		stDtData3.stMyselfObjInfo.byBuffChangeNum = stDtData.stMyselfObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stMyselfObjInfo.astBuffChangeInfo[byIdx], &stDtData.stMyselfObjInfo.astBuffChangeInfo[byIdx], sizeof(stDtData3.stMyselfObjInfo.astBuffChangeInfo[0]));
		}


		stDtData3.stMyselfObjInfo.byHpChangeNum = stDtData.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stDtData.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
		}

		stDtData3.stMyselfObjInfo.byAngryChangeNum = stDtData.stMyselfObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stDtData3.stMyselfObjInfo.astAngryChangeInfo[byIdx], &stDtData.stMyselfObjInfo.astAngryChangeInfo[byIdx], sizeof(stDtData3.stMyselfObjInfo.astAngryChangeInfo[0]));
		}

		stDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtData.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stDtData.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));

		//攻击后
		DT_ATTACK_DATA3 &stAfterDtData3 = stData4.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;
		const DT_ATTACK_DATA &stAfterDtData = stData2.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;


		stAfterDtData3.stEnemyObjInfo.byBuffChangeNum = stAfterDtData.stEnemyObjInfo.byBuffChangeNum;;
		for( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo[byIdx], &stAfterDtData.stEnemyObjInfo.astBuffChangeInfo[byIdx], sizeof(stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo[0]));
		}

		stAfterDtData3.stEnemyObjInfo.byHpChangeNum = stAfterDtData.stEnemyObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
		}


		stAfterDtData3.stEnemyObjInfo.byAngryChangeNum = stAfterDtData.stEnemyObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo[byIdx], &stAfterDtData.stEnemyObjInfo.astAngryChangeInfo[byIdx], sizeof(stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo[0]));
		}


		stAfterDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stAfterDtData.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stAfterDtData.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));


		//我方
		stAfterDtData3.stMyselfObjInfo.byBuffChangeNum = stAfterDtData.stMyselfObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo[byIdx], &stAfterDtData.stMyselfObjInfo.astBuffChangeInfo[byIdx], sizeof(stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo[0]));
		}


		stAfterDtData3.stMyselfObjInfo.byHpChangeNum = stAfterDtData.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
		}

		stAfterDtData3.stMyselfObjInfo.byAngryChangeNum = stAfterDtData.stMyselfObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo[byIdx], &stAfterDtData.stMyselfObjInfo.astAngryChangeInfo[byIdx], sizeof(stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo[0]));
		}

		stAfterDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stAfterDtData.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stAfterDtData.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));
	}

	//VerifyBattle(stData4);
}

VOID CBattleMgr::Convert3To4(const DT_BATTLE_DATA3& stData3, DT_BATTLE_DATA4& stData4)
{
	memset(&stData4, 0, sizeof(DT_BATTLE_DATA4));
	memcpy(&stData4.stMyselfInfo, &stData3.stMyselfInfo, sizeof(stData4.stMyselfInfo));
	memcpy(&stData4.stMyselfInfoAfterBattle, &stData3.stMyselfInfoAfterBattle, sizeof(stData4.stMyselfInfoAfterBattle));
	memcpy(&stData4.stEnemyInfo, &stData3.stEnemyInfo, sizeof(stData4.stEnemyInfo));
	stData4.byBattleResult = stData3.byBattleResult;
	stData4.byBattleRoundNum = stData3.byBattleRoundNum;
	stData4.byScore = stData3.byScore;
	for (UINT8 byIndex =0 ; byIndex < stData4.byBattleRoundNum; byIndex++)
	{
		
		stData4.astBattleRoundInfo[byIndex].byAttackerFlag = stData3.astBattleRoundInfo[byIndex].byAttackerFlag;
		stData4.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx = stData3.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx;
		stData4.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID = stData3.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID;
		stData4.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag = stData3.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag;
		stData4.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum = stData3.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum;

		stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAttackFlag = stData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAttackFlag,
		stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.dwAttackHurtValue = stData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.dwAttackHurtValue,
		stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAngryChangeNum = stData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.byAngryChangeNum,
		memcpy(stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.astAngryChangeInfo, stData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.astAngryChangeInfo,
			sizeof(stData4.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo.astAngryChangeInfo));

		for(UINT8 byIdx = 0; byIdx < stData4.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum; byIdx++)
		{
			stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag = stData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byBeCritFlag;
			stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx = stData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byFormationIdx;
			stData4.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag = stData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo[byIdx].byParryFlag;
		}

		//攻击前
		DT_ATTACK_DATA3 &stDtData3 = stData4.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;
		const DT_ATTACK_DATA2 &stDtData2 = stData3.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;


		stDtData3.stEnemyObjInfo.byBuffChangeNum = stDtData2.stEnemyObjInfo.byBuffChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astBuffChangeInfo, stDtData2.stEnemyObjInfo.astBuffChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astBuffChangeInfo));


		stDtData3.stEnemyObjInfo.byHpChangeNum = stDtData2.stEnemyObjInfo.byHpChangeNum;
		UINT8 byIdx = 0;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData2.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stDtData2.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
		}

		stDtData3.stEnemyObjInfo.byAngryChangeNum = stDtData2.stEnemyObjInfo.byAngryChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astAngryChangeInfo, stDtData2.stEnemyObjInfo.astAngryChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astAngryChangeInfo));

		stDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtData2.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stDtData2.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));



		stDtData3.stMyselfObjInfo.byBuffChangeNum = stDtData2.stMyselfObjInfo.byBuffChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astBuffChangeInfo, stDtData2.stMyselfObjInfo.astBuffChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astBuffChangeInfo));


		stDtData3.stMyselfObjInfo.byHpChangeNum = stDtData2.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stDtData2.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stDtData2.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
		}
		

		stDtData3.stMyselfObjInfo.byAngryChangeNum = stDtData2.stMyselfObjInfo.byAngryChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astAngryChangeInfo, stDtData2.stMyselfObjInfo.astAngryChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astAngryChangeInfo));

		stDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtData2.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stDtData2.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));

		//攻击后
		DT_ATTACK_DATA3 &stAfterDtData3 = stData4.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;
		const DT_ATTACK_DATA2 &stAfterDtData = stData3.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;


		stAfterDtData3.stEnemyObjInfo.byBuffChangeNum = stAfterDtData.stEnemyObjInfo.byBuffChangeNum;;
		for( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo[byIdx], &stAfterDtData.stEnemyObjInfo.astBuffChangeInfo[byIdx], sizeof(stAfterDtData3.stEnemyObjInfo.astBuffChangeInfo[0]));
		}

		stAfterDtData3.stEnemyObjInfo.byHpChangeNum = stAfterDtData.stEnemyObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData.stEnemyObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stAfterDtData3.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData.stEnemyObjInfo.astHpChangeInfo[byIdx].nHP;
		}


		stAfterDtData3.stEnemyObjInfo.byAngryChangeNum = stAfterDtData.stEnemyObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stEnemyObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo[byIdx], &stAfterDtData.stEnemyObjInfo.astAngryChangeInfo[byIdx], sizeof(stAfterDtData3.stEnemyObjInfo.astAngryChangeInfo[0]));
		}


		stAfterDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stAfterDtData.stEnemyObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo, stAfterDtData.stEnemyObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo));


		//我方
		stAfterDtData3.stMyselfObjInfo.byBuffChangeNum = stAfterDtData.stMyselfObjInfo.byBuffChangeNum;
		for( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byBuffChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo[byIdx], &stAfterDtData.stMyselfObjInfo.astBuffChangeInfo[byIdx], sizeof(stAfterDtData3.stMyselfObjInfo.astBuffChangeInfo[0]));
		}


		stAfterDtData3.stMyselfObjInfo.byHpChangeNum = stAfterDtData.stMyselfObjInfo.byHpChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byHpChangeNum; byIdx++)
		{
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx = stAfterDtData.stMyselfObjInfo.astHpChangeInfo[byIdx].byFormationIdx;
			stAfterDtData3.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP = stAfterDtData.stMyselfObjInfo.astHpChangeInfo[byIdx].nHP;
		}

		stAfterDtData3.stMyselfObjInfo.byAngryChangeNum = stAfterDtData.stMyselfObjInfo.byAngryChangeNum;
		for ( byIdx = 0; byIdx < stAfterDtData3.stMyselfObjInfo.byAngryChangeNum; byIdx++)
		{
			memcpy(&stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo[byIdx], &stAfterDtData.stMyselfObjInfo.astAngryChangeInfo[byIdx], sizeof(stAfterDtData3.stMyselfObjInfo.astAngryChangeInfo[0]));
		}

		stAfterDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stAfterDtData.stMyselfObjInfo.bySkillEffectIDChangeNum;
		memcpy(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo, stAfterDtData.stMyselfObjInfo.astSkillEffectIDChangeInfo, sizeof(stAfterDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo));
	}

	//VerifyBattle(stData4);
}