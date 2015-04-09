
#include "taskmgr.h"
#include "acceptconditon.h"
#include "finishconditon.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/errdef.h>
#include <logic/build/buildmgr.h>
#include <logic/player/playermgr.h>
#include <logic/base/logicfunc.h>
#include <common/server/utility.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/client/errdef.h>
#include <logic/player/player.h>

using namespace SGDP;


IMPLEMENT_SINGLETON(CTaskMgr)


CTaskMgr::CTaskMgr()
{

}

CTaskMgr::~CTaskMgr()
{
}

BOOL CTaskMgr::Init()
{
    // 初始化CTaskPropMgr管理器
    if(FALSE == CTaskPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CTaskPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CTaskPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CTaskPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CTaskMgr::UnInit()
{

}


CTask* CTaskMgr::CreateTask(UINT32 dwPlayerID, UINT8 byTaskLine)
{

    CTask*poTask = m_oTaskShmemObjMgr.Alloc();
    if(NULL == poTask)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: poTask is nULL!"), MSG_MARK);
        return NULL;
    }
    UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, byTaskLine);
    m_mapID2Task[qwKey] = poTask;

    return poTask;
}

VOID CTaskMgr::RemoveTask(UINT32 dwPlayerID, UINT8 byTaskLine)
{
    UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, byTaskLine);
    CID2TaskMapItr itr = m_mapID2Task.find(qwKey);
    if (m_mapID2Task.end() != itr)
    {
        CTask* poTask = itr->second;
        m_oTaskShmemObjMgr.Free(poTask);
        m_mapID2Task.erase(itr);
    }
}

CTask* CTaskMgr::FindTask(UINT32 dwPlayerID, UINT8 byTaskLine)
{
    UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, byTaskLine);
    CID2TaskMapItr itr = m_mapID2Task.find(qwKey);
    if (m_mapID2Task.end() != itr)
    {
        return itr->second;
    }

    return NULL;
}


//检查任务是否可接，若之前不可接现可接则记录为变化任务
VOID CTaskMgr::CkWantCheckAccept(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapGiveTask = poPlayer->GetGiveTask();
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CTask* poTask = mapGiveTask.GetFistData();
    while(poTask)
    {
        if((1 == poTask->GetCanAcceptFalg()) || (0 == poTask->CheckAccept()))
        {
            UINT8 byTaskLine = poTask->GetDT_TASK_DATA().byTaskLine;
            mapGiveTask.DelData(byTaskLine);
            mapAcceptTask.AddData(byTaskLine, poTask);
            poPlayer->RecordChangeTask(poTask);
        }

        poTask = mapGiveTask.GetNextData();
    }
}

/*
UINT16 CTaskMgr::AcceptTask(CPlayer* poPlayer, UINT8 byTaskLine, UINT8 byTaskIdx)
{
	if(NULL == poPlayer)
	{
		RETURN_OTHER_ERR;
	}

	if((byTaskLine <MAIN_LINE_TASK) || (byTaskLine > BRANCH_LINE_TASK))
	{
		return ERR_ACCEPT_TASK::ID_TASK_LINE_INVALID;
	}

	CLineIdx2TaskShmemMap& mapGiveTask = poPlayer->GetGiveTask();
	CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
	//主线，
	if(byTaskLine == MAIN_LINE_TASK)
	{
		CTask* poTask = poPlayer->GetTaskByLine(BRANCH_LINE_TASK);
		if(poTask)
		{
			//当前支线任务在进行中，不能再接受
			if((ETS_GIVEN != poTask->GetDT_TASK_DATA().byTaskState) && (ETS_FINISHED != poTask->GetDT_TASK_DATA().byTaskState) && (ETS_INVALID != poTask->GetDT_TASK_DATA().byTaskState))
			{
				return ERR_ACCEPT_TASK::ID_HAVEN_ACCEPT_TASK;
			}
		}
	}
	//支线，
	if(byTaskLine == BRANCH_LINE_TASK)
	{
		CTask* poTask = poPlayer->GetTaskByLine(MAIN_LINE_TASK);
		if(NULL == poTask)
		{
			return ERR_ACCEPT_TASK::ID_TASK_NOT_EXIST;
		}
		//当前主线任务在进行中，不能再接受
		if((ETS_GIVEN != poTask->GetDT_TASK_DATA().byTaskState) && (ETS_FINISHED != poTask->GetDT_TASK_DATA().byTaskState))
		{
			return ERR_ACCEPT_TASK::ID_HAVEN_ACCEPT_TASK;
		}
	}

	CTask* poTask = mapGiveTask.GetData(byTaskLine);
	if(NULL == poTask)
	{
		if(NULL == poPlayer->GetTask(byTaskLine))
		{
			return ERR_ACCEPT_TASK::ID_TASK_NOT_EXIST;
		}
		else
		{
			return ERR_ACCEPT_TASK::ID_STATE_NOT_MATCH;
		}
	}

	UINT16 wErrCode = poTask->CheckAccept();
	//接受任务，将接受任务管理转移到接受任务map
	if(ERR_ACCEPT_TASK::ID_SUCCESS == wErrCode)
	{
		mapGiveTask.DelData(byTaskLine);
		mapAcceptTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
		poPlayer->RecordChangeTask(poTask);

		//修改主线的接受的任务配置，用于判断解锁副本
		STaskProp* pstTaskProp = poTask->GetTaskProp();
		if((pstTaskProp) && (MAIN_LINE_TASK == pstTaskProp->byTaskLineIdx))
		{
			poPlayer->SetLastMainLineTaskProp(pstTaskProp);
		}

		CheckFinish(poPlayer);

		//若是主线任务接受成功，则已给予的支线任务状态无效
		if(byTaskLine == MAIN_LINE_TASK)
		{
			CTask* poBranchTask = mapGiveTask.GetData(BRANCH_LINE_TASK);
			if(poBranchTask)
			{
				poBranchTask->GetDT_TASK_DATA().byTaskState = ETS_INVALID;
				mapGiveTask.DelData(byTaskLine);
			}

		}
	}


	return wErrCode;
}

UINT16 CTaskMgr::GiveUpTask(CPlayer* poPlayer, UINT8 byTaskLine, UINT8 byTaskIdx)
{
	if(NULL == poPlayer)
	{
		RETURN_OTHER_ERR;
	}

	if((byTaskLine <MAIN_LINE_TASK) || (byTaskLine > BRANCH_LINE_TASK))
	{
		return ERR_GIVEUP_TASK::ID_TASK_LINE_INVALID;
	}

	if(MAIN_LINE_TASK == byTaskLine)
	{
		return ERR_GIVEUP_TASK::ID_CAN_NOT_GIVE_UP_MAIN_TASK;
	}

	CLineIdx2TaskShmemMap& mapGiveTask = poPlayer->GetGiveTask();
	CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
	CLineIdx2TaskShmemMap& mapChangeTask = poPlayer->GetChangeTaskMap();
	CTask* poTask = mapAcceptTask.GetData(byTaskLine);
	if(NULL == poTask)
	{
		if(NULL == poPlayer->GetTask(byTaskLine))
		{
			return ERR_GIVEUP_TASK::ID_TASK_NOT_EXIST;
		}
		else
		{
			return ERR_GIVEUP_TASK::ID_STATE_NOT_MATCH;
		}
	}

	//从接受mpa中清除，重新回到给予的map
	//若是支线，则判断主线是否可接，可接则支线不需再放给予列表
	if(BRANCH_LINE_TASK == byTaskLine)
	{
		mapAcceptTask.DelData(byTaskLine);
		//判断主线是否已可接，若可接则不会给下一个支线
		CTask* poMainTaks = poPlayer->GetTask(MAIN_LINE_TASK);
		if(ETS_GIVEN != poMainTaks->GetDT_TASK_DATA().byTaskState)
		{
			//异常，有支线时主线应该是给予的状态
			SYS_CRITICAL(_SDT("[%s: %d]: Main task is not ETS_GIVEN state!"), MSG_MARK);
		}
		//主线接受条件已到，则支线无效
		if(0 == poMainTaks->CheckAccept(poMainTaks->GetTaskProp()->mapAcceptCondition))
		{
			poTask->GetDT_TASK_DATA().byTaskState = ETS_INVALID;
			//poTask->SetSaveState(ESS_WANT_SAVE);
		}
		else
		{
			poTask->GetDT_TASK_DATA().byTaskState = ETS_GIVEN;
			mapGiveTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
			poPlayer->RecordChangeTask(poTask);
			//poTask->SetSaveState(ESS_WANT_SAVE);
		}
	}
	else
	{
		mapAcceptTask.DelData(byTaskLine);
		poTask->GetDT_TASK_DATA().byTaskState = ETS_GIVEN;
		mapGiveTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
		poPlayer->RecordChangeTask(poTask);
		//poTask->SetSaveState(ESS_WANT_SAVE);
	}


	return ERR_GIVEUP_TASK::ID_SUCCESS;
}
*/


VOID CTaskMgr::OnPassInstance(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }
        STaskProp* poTaskProp = poTask->GetTaskProp();
        if(NULL == poTaskProp)
        {
            continue;
        }

        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case FIRSTPASSINSTANCE:
        {
            //不需记录
        }
        break;
        case SCOREPASSINSTANCE:
        {
            //不需记录
        }
        break;
        case TIMESPASSINSTANCE:
        {
            CFinishCondition_TimesPassInstance *pFinishCondition = (CFinishCondition_TimesPassInstance*)(poTaskProp->poFinishCondition);
            if((pFinishCondition->wSceneIdx == wSceneIdx) && (pFinishCondition->byTownIdx == byTownIdx) && (pFinishCondition->byInstanceIdx == byInstanceIdx))
            {
                DT_TIMESPASSINSTANCE_TASK_CONDITION& stTimesPassInstance = stTaskConditionInfo.stTimesPassInstance;
                stTimesPassInstance.byTimes++;
                //poTask->SetSaveState(ESS_WANT_SAVE);
            }
        }
        break;
        case CLIMBTOWER:
        {
        }
        break;
        default:
            continue;
        }


        //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
        if(poTask->CheckFinish())
        {
            mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
            mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
            poPlayer->RecordChangeTask(poTask);
            //poTask->SetSaveState(ESS_WANT_SAVE);
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnClimbTower(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if(NULL == poTaskProp)
        {
            continue;
        }

        switch(poTaskProp->eTaskType)
        {
        case CLIMBTOWER:
        {
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
                //poTask->SetSaveState(ESS_WANT_SAVE);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnCrown(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if(NULL == poTaskProp)
        {
            continue;
        }

        switch(poTaskProp->eTaskType)
        {
        case CROWN:
        {
            CFinishCondition_Crown *pFinishCondition = (CFinishCondition_Crown*)(poTaskProp->poFinishCondition);
            DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
            stTaskConditionInfo.stCrown.wCrownTimes++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}

VOID CTaskMgr::OnRecruitHero(CPlayer* poPlayer, UINT16 wHeroKindID)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if(NULL == poTaskProp)
        {
            continue;
        }

        switch(poTaskProp->eTaskType)
        {
        case RECRUITHERO:
        {
            //不需记录，回调只是触发检查
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
                //poTask->SetSaveState(ESS_WANT_SAVE);
            }
        }
        break;
        default:
            continue;
        }


    }
    while(poTask = mapAcceptTask.GetNextData());
}



VOID CTaskMgr::OnRecruitHeroNum(CPlayer* poPlayer, UINT16 wHeroKindID)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if(NULL == poTaskProp)
        {
            continue;
        }

        switch(poTaskProp->eTaskType)
        {
        case RECRUITHERONUM:
        {
            //不需记录，回调只是触发检查
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
                //poTask->SetSaveState(ESS_WANT_SAVE);
            }
        }
        break;
        default:
            continue;
        }


    }
    while(poTask = mapAcceptTask.GetNextData());
}

VOID CTaskMgr::OnOpenLight(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if(NULL == poTaskProp)
        {
            continue;
        }
		DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
		stTaskConditionInfo.stOpenLight.byExt++;

        switch(poTaskProp->eTaskType)
        {
        case OPENLIGHT:
        {
            //不需记录，回调只是触发检查
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
                //poTask->SetSaveState(ESS_WANT_SAVE);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}




VOID CTaskMgr::OnKillMonsterInstance(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterKindID)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }

        switch(poTaskProp->eTaskType)
        {
        case KILLMONSTERINSTANCE:
        {
            CFinishCondition_KillMonstersInstance *pFinishCondition = (CFinishCondition_KillMonstersInstance*)(poTaskProp->poFinishCondition);

            if((pFinishCondition->wSceneIdx == wSceneIdx) &&
                    (pFinishCondition->byTownIdx == byTownIdx) &&
                    (pFinishCondition->byInstanceIdx == byInstanceIdx) &&
                    (pFinishCondition->wMonsterKindID == wMonsterKindID))
            {
                DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
                stTaskConditionInfo.stKillMonstersInstance.byKillNum++;
                //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
                if(poTask->CheckFinish())
                {
                    mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                    mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                    poPlayer->RecordChangeTask(poTask);
                    //poTask->SetSaveState(ESS_WANT_SAVE);
                }
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}

VOID CTaskMgr::OnStrengthenEquip(CPlayer* poPlayer, UINT16 wEquipID)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case STRENGTHENEQUIPLEVEL:
        {
            CFinishCondition_StrengthenEquipLevel *pFinishCondition = (CFinishCondition_StrengthenEquipLevel*)(poTaskProp->poFinishCondition);

            //不需记录，回调只是触发检查
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        case STRENGTHENEQUIPNUM:
        {
            CFinishCondition_StrengthenEquipNum *pFinishCondition = (CFinishCondition_StrengthenEquipNum*)(poTaskProp->poFinishCondition);
            if(pFinishCondition->wEquipKindID == wEquipID || 0 == pFinishCondition->wEquipKindID)
            {
                DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
                stTaskConditionInfo.stStrengthenEquipNum.wNum++;
                //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
                if(poTask->CheckFinish())
                {

                    mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                    mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                    poPlayer->RecordChangeTask(poTask);
                }
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}


//激活神器 任务
VOID CTaskMgr::OnActivateGodweapon(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case ACTIVATEGODWEAPON:
        {
            CFinishCondition_ActivateGodweapon *pFinishCondition = (CFinishCondition_ActivateGodweapon*)(poTaskProp->poFinishCondition);
            //不需记录，回调只是触发检查
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}

//升级神器 任务
VOID CTaskMgr::OnUpgradeGodweapon(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case UPGRADEGODWEAPON:
        {
            CFinishCondition_UpgradeGodweapon *pFinishCondition = (CFinishCondition_UpgradeGodweapon*)(poTaskProp->poFinishCondition);
            //不需记录，回调只是触发检查
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}

//使用丹药 任务
VOID CTaskMgr::OnUseExprienceDrug(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case USEEXPRIENCEDRUG:
        {
            CFinishCondition_UseExprienceDrug *pFinishCondition = (CFinishCondition_UseExprienceDrug*)(poTaskProp->poFinishCondition);

            stTaskConditionInfo.stUseExprienceDrug.wNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}



VOID CTaskMgr::OnComposeGood(CPlayer* poPlayer, UINT16 wMaterialID)//合成物品 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case COMPOSEGOOD:
        {
            CFinishCondition_ComposeGood *pFinishCondition = (CFinishCondition_ComposeGood*)(poTaskProp->poFinishCondition);
            if (pFinishCondition->wMaterialID == wMaterialID)
            {
                DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
                stTaskConditionInfo.stComposeGood.wNum++;
                //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
                if(poTask->CheckFinish())
                {
                    mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                    mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                    poPlayer->RecordChangeTask(poTask);
                }
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnEliteInstance(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)//精英副本 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case ELITEINSTANCE:
        {
            CFinishCondition_EliteInstance *pFinishCondition = (CFinishCondition_EliteInstance*)(poTaskProp->poFinishCondition);

            //if (pFinishCondition->wSceneIdx == wSceneIdx &&
            //        pFinishCondition->byTownIdx == byTownIdx &&
            //        pFinishCondition->byInstanceIdx == byInstanceIdx )
            //{
            DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
            stTaskConditionInfo.stEliteInstance.byUpgradeFlag = 1;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
            //}
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}



VOID CTaskMgr::OnUpgradeScience(CPlayer* poPlayer) //升级科技点任务 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case UPGRADSCIENCE:
        {
            CFinishCondition_UpgradeScience *pFinishCondition = (CFinishCondition_UpgradeScience*)(poTaskProp->poFinishCondition);

            DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
            stTaskConditionInfo.stUpgradeScience.wNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnStrengthAttack(CPlayer* poPlayer) //强化攻击 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case STRENGTHATTACK:
        {
            CFinishCondition_StrengthAttack *pFinishCondition = (CFinishCondition_StrengthAttack*)(poTaskProp->poFinishCondition);

            DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
            stTaskConditionInfo.stStrengthAttack.wNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnStrengthLife(CPlayer* poPlayer) //强化生命 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case STRENGTHLIFE:
        {
            CFinishCondition_StrengthLife *pFinishCondition = (CFinishCondition_StrengthLife*)(poTaskProp->poFinishCondition);
            DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
            stTaskConditionInfo.stStrengthLife.wNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnCrop(CPlayer* poPlayer) //合成物品 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }
        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        switch(poTaskProp->eTaskType)
        {
        case CROP:
        {
            CFinishCondition_Crop *pFinishCondition = (CFinishCondition_Crop*)(poTaskProp->poFinishCondition);
            DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
            stTaskConditionInfo.stCrop.wNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}



//竞技场挑战 任务
VOID CTaskMgr::OnChallenge(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case CHALLENGE:
        {
            stTaskConditionInfo.stChallenge.wChallengeNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}

//掠夺 任务
VOID CTaskMgr::OnPlunder(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case PLUNDER:
        {
            stTaskConditionInfo.stPlunder.wPlunderNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}


VOID CTaskMgr::OnDressEquip(CPlayer* poPlayer, UINT16 wEquipKindID) //穿戴装备 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case DRESSEQUIP:
        {
            if((stTaskConditionInfo.stDressEquip.wEquipID == wEquipKindID) || (0 == stTaskConditionInfo.stDressEquip.wEquipID))
            {
                stTaskConditionInfo.stDressEquip.byDressFlag = 1;
                //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
                if(poTask->CheckFinish())
                {
                    mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                    mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                    poPlayer->RecordChangeTask(poTask);
                }
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}

VOID CTaskMgr::OnMelt(CPlayer* poPlayer)//溶练器魂
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case MELTEQUIP:
        {
            CFinishCondition_MultEquip *pFinishCondition = (CFinishCondition_MultEquip*)(poTaskProp->poFinishCondition);

            stTaskConditionInfo.stMeltEquip.wNum++;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}

VOID CTaskMgr::OnStudy(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case STUDY:
        {
            CFinishCondition_Study *pFinishCondition = (CFinishCondition_Study*)(poTaskProp->poFinishCondition);

            //stTaskConditionInfo.stStudy.byLevel = 0;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}


//升级技能 任务
VOID CTaskMgr::OnUpgradeSkill(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case UPGRADESKILL:
        {
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }
    }
    while(poTask = mapAcceptTask.GetNextData());
}

VOID CTaskMgr::OnUpgradeJingJie(CPlayer* poPlayer, UINT16 wLevel, UINT16 wSubLevel)//升阶装备 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case UPGRADEJINGJIE:
        {
            stTaskConditionInfo.stUpgradeJingJie.wLevel = wLevel;
            stTaskConditionInfo.stUpgradeJingJie.wSubLevel = wSubLevel;
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}



VOID CTaskMgr::OnMosaicGem(CPlayer* poPlayer) //升级技能 任务
{
    if(NULL == poPlayer)
    {
        return;
    }

    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    do
    {
        if(NULL == poTask)
        {
            break;
        }

        STaskProp* poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            break;
        }
        DT_TASK_CONDITION& stTaskConditionInfo = poTask->GetConditionRecord();
        switch(poTaskProp->eTaskType)
        {
        case MOSAICGEM:
        {
            //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
            if(poTask->CheckFinish())
            {
                mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
                mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
                poPlayer->RecordChangeTask(poTask);
            }
        }
        break;
        default:
            continue;
        }

    }
    while(poTask = mapAcceptTask.GetNextData());
}


BOOL CTaskMgr::GiveNextTask(CPlayer* poPlayer, CTask* poTask)
{
    if((NULL == poPlayer) || (NULL == poTask))
    {
        return FALSE;
    }

    /*
    //支线任务，判断当前主线任务上是否还有支线
    if(BRANCH_LINE_TASK == poTask->GetDT_TASK_DATA().byTaskLine)
    {
    	//当前主线上没有了支线任务
    	if(poTask->GetDT_TASK_DATA().dwTaskIdx >= poTask->GetTaskProp()->pstMainTaskProp->dwEndBranchTaskIdx)
    	{
    		return;
    	}
    	//判断主线是否已可接，若可接则不会给下一个支线
    	CTask* poMainTaks = poPlayer->GetTask(MAIN_LINE_TASK);
    	if(ETS_GIVEN != poMainTaks->GetDT_TASK_DATA().byTaskState)
    	{
    		//异常，有支线时主线应该是给予的状态
    		SYS_CRITICAL(_SDT("[%s: %d]: Main task is not ETS_GIVEN state!"), MSG_MARK);
    		return;
    	}
    	//主线接受条件已到，则不给下一个支线
    	if(ERR_ACCEPT_TASK::ID_SUCCESS == poMainTaks->CheckAccept(poMainTaks->GetTaskProp()->mapAcceptCondition))
    	{
    		return;
    	}
    	//判断主线是否存在下一个支线任务,没有则返回
    	if((poTask->GetDT_TASK_DATA().dwTaskIdx+1) > poMainTaks->GetTaskProp()->dwEndBranchTaskIdx)
    	{
    		return;
    	}

    }
    */

    STaskProp * poTaskProp = poTask->GetTaskProp();
    if (NULL == poTaskProp)
    {
        return FALSE;
    }

    if(0 == poTaskProp->dwNextTaskIdx)
    {
        poTask->GetDT_TASK_DATA().byTaskState = ETS_INVALID;
        return FALSE;
    }
    UINT8 byTaskLine = poTask->GetDT_TASK_DATA().byTaskLine;
    STaskProp* pstLastMainTaskProp = NULL;
    if(MAIN_LINE_TASK == poTask->GetDT_TASK_DATA().byTaskLine)
    {
        pstLastMainTaskProp = poTask->GetTaskProp();
    }

    //更改该线的任务为下一个，并且重新初始化下一个任务信息
    DT_TASK_DATA stDT_TASK_DATA;
    memset(&stDT_TASK_DATA, 0, sizeof(DT_TASK_DATA));
    stDT_TASK_DATA.byTaskLine = byTaskLine;
    UINT32 dwTaskIdx = poTaskProp->dwTaskIdx;
    stDT_TASK_DATA.dwTaskIdx = poTaskProp->dwNextTaskIdx;
    stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
    stDT_TASK_DATA.byTaskState = ETS_GIVEN;

    if(!SetTaskCond(poPlayer, stDT_TASK_DATA.byTaskLine, stDT_TASK_DATA.dwTaskIdx, stDT_TASK_DATA.byTaskType, stDT_TASK_DATA.stTaskConditionInfo))
    {
        poTask->GetDT_TASK_DATA().byTaskState = ETS_INVALID;
        SYS_CRITICAL(_SDT("[%s: %d]: SetTaskCond failed!"), MSG_MARK);
        return FALSE;
    }

    //重新初始化下一个任务信息
    if(FALSE == poTask->Init(&stDT_TASK_DATA, poPlayer))
    {
        poTask->GetDT_TASK_DATA().byTaskState = ETS_INVALID;
        SYS_CRITICAL(_SDT("[%s: %d]: task init failed!"), MSG_MARK);
        return FALSE;
    }

    poPlayer->GetFinishTask().DelData(byTaskLine);
    poPlayer->GetChangeTask().AddData(byTaskLine, poTask);

    //修改信息
    UINT8 byTaskState = poTask->GetDT_TASK_DATA().byTaskState;
    if(ETS_GIVEN == byTaskState)
    {
        poPlayer->GetGiveTask().AddData(byTaskLine, poTask);
    }
    else if(ETS_ACCEPTED == byTaskState)
    {
        poPlayer->GetAcceptTask().AddData(byTaskLine, poTask);
    }
    else if(ETS_ENCOURAGE == byTaskState)
    {
        poPlayer->GetEncourageTask().AddData(byTaskLine, poTask);
    }
    else if(ETS_FINISHED == byTaskState)
    {
        poPlayer->GetFinishTask().AddData(byTaskLine, poTask);
    }
    poPlayer->RecordChangeTask(poTask);

    /*
    //是主线，则判断是否需给支线
    if(MAIN_LINE_TASK == poTask->GetDT_TASK_DATA().byTaskLine)
    {
    	//主线不可接，则给上个主线任务的支线任务
    	if(0 == poTask->GetCanAcceptFalg())
    	{
    		GiveBranchTask(poPlayer, pstLastMainTaskProp);
    	}
    }
    */

    return TRUE;
}

/*
//给支线任务
VOID CTaskMgr::GiveBranchTask(CPlayer* poPlayer, STaskProp* pstLastMainTaskProp)
{
	if((NULL == poPlayer) || (NULL == pstLastMainTaskProp))
	{
		return;
	}

	//有支线任务
	if(0 != pstLastMainTaskProp->dwStartBranchTaskIdx)
	{
		DT_TASK_DATA stDT_TASK_DATA;
		memset(&stDT_TASK_DATA, 0, sizeof(DT_TASK_DATA));
		stDT_TASK_DATA.byTaskLine = BRANCH_LINE_TASK;
		stDT_TASK_DATA.dwTaskIdx = pstLastMainTaskProp->dwStartBranchTaskIdx;
		stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
		stDT_TASK_DATA.byTaskState = ETS_GIVEN;

		if(!SetTaskCond(stDT_TASK_DATA.byTaskLine, stDT_TASK_DATA.dwTaskIdx, stDT_TASK_DATA.byTaskType, stDT_TASK_DATA.stTaskConditionInfo))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: SetTaskCond failed!"), MSG_MARK);
			return;
		}

		CTask* poranchTask = poPlayer->GetTaskByLine(BRANCH_LINE_TASK);
		if(NULL == poranchTask)
		{
			poranchTask = CTaskMgr::Instance()->CreateTask(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, BRANCH_LINE_TASK);
			if(NULL == poranchTask)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: CreateTask failed !"), MSG_MARK);
				return;
			}
			//poranchTask->SetObjDataSaveType(EODST_INSERT);
		}

		//初始化
		if(FALSE == poranchTask->Init(&stDT_TASK_DATA, poPlayer))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: task init failed!"), MSG_MARK);
			return;
		}

		if(ETS_GIVEN == poranchTask->GetDT_TASK_DATA().byTaskState)
		{
			poPlayer->GetGiveTask().AddData(BRANCH_LINE_TASK, poranchTask);
			poPlayer->GetAllTask().AddData(BRANCH_LINE_TASK, poranchTask);	//支线可能是新增的,由map跳过
			poPlayer->RecordChangeTask(poranchTask);
		}

		//poranchTask->SetSaveState(ESS_WANT_SAVE);
	}
}
*/

BOOL CTaskMgr::SetTaskCond(CPlayer* poPlayer, UINT8 byTaskLine, UINT32 dwTaskIdx, UINT8& byTaskConditonType, DT_TASK_CONDITION& stTaskConditionInfo)
{
    STaskProp* pTaskProp = CTaskPropMgr::Instance()->GetTaksProp(byTaskLine, dwTaskIdx);
    if(NULL == pTaskProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pTaskProp is nULL!"), MSG_MARK);
        return FALSE;
    }
    byTaskConditonType = pTaskProp->eTaskType;
    switch(pTaskProp->eTaskType)
    {
    case CROWN:
    {
        stTaskConditionInfo.stCrown.wCrownTimes = 0;
    }
    break;
    //case BUILDUPGRADE:
    //{
    //    //不需记录
    //}
    //break;
    case RECRUITHERO:
    {
        //不需记录
    }
    break;
    //case HEROCOLOR:
    //{
    //    //不需记录
    //}
    //break;
    //case HEROUPGRADE:
    //{
    //    //不需记录
    //}
    //break;
    case FIRSTPASSINSTANCE:
    {
        //不需记录
    }
    break;
    case SCOREPASSINSTANCE:
    {
        //不需记录
    }
    break;
    case TIMESPASSINSTANCE:
    {
        CFinishCondition_TimesPassInstance* poFinCond = dynamic_cast<CFinishCondition_TimesPassInstance*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stTimesPassInstance.wSceneIdx = poFinCond->wSceneIdx;
        stTaskConditionInfo.stTimesPassInstance.byTownIdx = poFinCond->byTownIdx;
        stTaskConditionInfo.stTimesPassInstance.byInstanceIdx = poFinCond->byInstanceIdx;
        stTaskConditionInfo.stTimesPassInstance.byTimes = 0;
    }
    break;
    case KILLMONSTERINSTANCE:
    {
        CFinishCondition_KillMonstersInstance* poFinCond = dynamic_cast<CFinishCondition_KillMonstersInstance*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stKillMonstersInstance.wSceneIdx = poFinCond->wSceneIdx;
        stTaskConditionInfo.stKillMonstersInstance.byTownIdx = poFinCond->byTownIdx;
        stTaskConditionInfo.stKillMonstersInstance.byInstanceIdx = poFinCond->byInstanceIdx;
        stTaskConditionInfo.stKillMonstersInstance.wMonsterKindID = poFinCond->wMonsterKindID;
        stTaskConditionInfo.stKillMonstersInstance.byKillNum = 0;
    }
    break;
    case STRENGTHENEQUIPLEVEL:
    {
        CFinishCondition_StrengthenEquipLevel* poFinCond = dynamic_cast<CFinishCondition_StrengthenEquipLevel*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }

        stTaskConditionInfo.stStrengthenEquipLevel.wEquipKindID = poFinCond->wEquipKindID;
        stTaskConditionInfo.stStrengthenEquipLevel.wLevel = poFinCond->wLevel;
    }
    break;
    case STRENGTHENEQUIPNUM:
    {
        CFinishCondition_StrengthenEquipNum* poFinCond = dynamic_cast<CFinishCondition_StrengthenEquipNum*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stStrengthenEquipNum.wEquipKindID = poFinCond->wEquipKindID;
        stTaskConditionInfo.stStrengthenEquipNum.wNum = 0;
    }
    break;
    case ACTIVATEGODWEAPON:
    {
        CFinishCondition_ActivateGodweapon* poFinCond = dynamic_cast<CFinishCondition_ActivateGodweapon*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stActivateGodweapon.wHeroKindID = poFinCond->wHeroKindID;

    }
    break;
    case UPGRADEGODWEAPON:
    {
        CFinishCondition_UpgradeGodweapon* poFinCond = dynamic_cast<CFinishCondition_UpgradeGodweapon*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stUpgradeGodweapon.wHeroKindID = poFinCond->wHeroKindID;
        stTaskConditionInfo.stUpgradeGodweapon.wQuality = poFinCond->wQuality;
        stTaskConditionInfo.stUpgradeGodweapon.wLevel = poFinCond->wLevel;

    }
    break;
    case USEEXPRIENCEDRUG:
    {
        CFinishCondition_UseExprienceDrug* poFinCond = dynamic_cast<CFinishCondition_UseExprienceDrug*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stUseExprienceDrug.wNum = 0;
    }
    break;
    case MELTEQUIP:
    {
        CFinishCondition_MultEquip* poFinCond = dynamic_cast<CFinishCondition_MultEquip*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stMeltEquip.wNum = 0;
    }
    break;
    case STUDY:
    {
        CFinishCondition_Study* poFinCond = dynamic_cast<CFinishCondition_Study*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stStudy.byLevel = 0;
    }
    break;
    case COMPOSEGOOD:
    {
        CFinishCondition_ComposeGood* poFinCond = dynamic_cast<CFinishCondition_ComposeGood*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stComposeGood.wMaterialID = poFinCond->wMaterialID;
        stTaskConditionInfo.stComposeGood.wMaterialNum = poFinCond->wMaterialNum;
        stTaskConditionInfo.stComposeGood.wKindID = poFinCond->wKindID;
        stTaskConditionInfo.stComposeGood.wNum = 0;
    }
    break;
    case ELITEINSTANCE:
    {
        CFinishCondition_EliteInstance* poFinCond = dynamic_cast<CFinishCondition_EliteInstance*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stEliteInstance.wSceneIdx = poFinCond->wSceneIdx;
        stTaskConditionInfo.stEliteInstance.byTownIdx = poFinCond->byTownIdx;
        stTaskConditionInfo.stEliteInstance.byInstanceIdx = poFinCond->byInstanceIdx;
        stTaskConditionInfo.stEliteInstance.byUpgradeFlag = 0;
    }
    break;
    case UPGRADSCIENCE:
    {
        CFinishCondition_UpgradeScience* poFinCond = dynamic_cast<CFinishCondition_UpgradeScience*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stUpgradeScience.wNum = 0;
    }
    break;
    case STRENGTHATTACK:
    {
        CFinishCondition_StrengthAttack* poFinCond = dynamic_cast<CFinishCondition_StrengthAttack*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stStrengthAttack.wNum = 0;
    }
    break;
    case STRENGTHLIFE:
    {
        CFinishCondition_StrengthLife* poFinCond = dynamic_cast<CFinishCondition_StrengthLife*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stStrengthLife.wNum = 0;
    }
    break;
    case CROP:
    {
        CFinishCondition_Crop* poFinCond = dynamic_cast<CFinishCondition_Crop*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stCrop.wNum = 0;
    }
    break;
    case CHALLENGE:
    {
        CFinishCondition_Challenge* poFinCond = dynamic_cast<CFinishCondition_Challenge*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stChallenge.wChallengeNum = 0;

    }
    break;
    case PLUNDER:
    {
        CFinishCondition_Plunder* poFinCond = dynamic_cast<CFinishCondition_Plunder*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stPlunder.wPlunderNum = 0;

    }
    break;
    //case DRESSSKILL:
    //{
    //    CFinishCondition_DressSkill* poFinCond = dynamic_cast<CFinishCondition_DressSkill*>(pTaskProp->poFinishCondition);
    //    if(NULL == poFinCond)
    //    {
    //        return FALSE;
    //    }
    //    stTaskConditionInfo.stDressSkill.wSkillID = poFinCond->wSkillID;
    //    stTaskConditionInfo.stDressSkill.byDressFlag = 0;
    //}
    //break;
    case UPGRADESKILL:
    {
        //if (NULL == poPlayer)
        //{
        //    return TRUE;
        //}

        //CFinishCondition_UpgradeSkill* poFinCond = dynamic_cast<CFinishCondition_UpgradeSkill*>(pTaskProp->poFinishCondition);
        //if(NULL == poFinCond)
        //{
        //    return FALSE;
        //}
        //stTaskConditionInfo.stUpgradeSkill.wSkillActionID = 0;
        //stTaskConditionInfo.stUpgradeSkill.wLevel = 0;


        //CJingJie *poJingJie = poPlayer->GetJingJie();
        //if ((0 != poFinCond->wUnlockIdx) && (NULL != poJingJie))
        //{
        //    stTaskConditionInfo.stUpgradeSkill.wSkillActionID = poJingJie->GetSkillActionIDByUnlockIdx(poFinCond->wUnlockIdx);
        //    stTaskConditionInfo.stUpgradeSkill.wLevel = 0;
        //}
    }
    break;
    case UPGRADEJINGJIE:
    {
        CFinishCondition_UpgradeJingJie* poFinCond = dynamic_cast<CFinishCondition_UpgradeJingJie*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stUpgradeJingJie.wLevel = 0;
        stTaskConditionInfo.stUpgradeJingJie.wSubLevel = 0;
    }
    break;
    case MOSAICGEM:
    {
        CFinishCondition_MosaicGem* poFinCond = dynamic_cast<CFinishCondition_MosaicGem*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stMosaicGem.wNum = 0;
    }
    break;
    case CLIMBTOWER:
    {
        CFinishCondition_ClimbTower* poFinCond = dynamic_cast<CFinishCondition_ClimbTower*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stClimbTower.wNum = 0;
    }
    break;
    case DRESSEQUIP:
    {
        CFinishCondition_DressEquip* poFinCond = dynamic_cast<CFinishCondition_DressEquip*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stDressEquip.wEquipID = poFinCond->wEquipID;
        stTaskConditionInfo.stDressEquip.byDressFlag = 0;
    }
    break;
    case RECRUITHERONUM:
    {
        CFinishCondition_RecruitHeroNum* poFinCond = dynamic_cast<CFinishCondition_RecruitHeroNum*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stRecruitHeroNum.wNum = poFinCond->wNum;
    }
    break;
    case OPENLIGHT:
    {
        CFinishCondition_OpenLight* poFinCond = dynamic_cast<CFinishCondition_OpenLight*>(pTaskProp->poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        stTaskConditionInfo.stOpenLight.byExt = 0;
    }
    break;
    default:
        return FALSE;
    }

    return TRUE;
}


VOID CTaskMgr::CheckFinish(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    CLineIdx2TaskShmemMap& mapAcceptTask = poPlayer->GetAcceptTask();
    CLineIdx2TaskShmemMap& mapEncourageTask = poPlayer->GetEncourageTask();
    CTask* poTask = mapAcceptTask.GetFistData();
    while(poTask)
    {
        //完成任务，将接受任务管理从接受任务map清除，并移到领取奖励任务map
        if(poTask->CheckFinish())
        {
            mapAcceptTask.DelData(poTask->GetDT_TASK_DATA().byTaskLine);
            mapEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
            poPlayer->RecordChangeTask(poTask);
            //poTask->SetSaveState(ESS_WANT_SAVE);
        }
        poTask = mapAcceptTask.GetNextData();
    }
}


UINT16 CTaskMgr::RecvTaskEncourage(CPlayer* poPlayer, UINT8 byTaskLine, UINT32 dwTaskIdx)
{
     //= FindTask(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, byTaskLine);
	CTask* poTask = poPlayer->GetTask(byTaskLine);
    if (NULL == poTask)
    {
        return ERR_RECV_TASK_ENCOURAGE::ID_TASK_NOT_EXIT_ERR;
    }

    //已经领取
    if(dwTaskIdx < poTask->GetDT_TASK_DATA().dwTaskIdx)
    {
        return ERR_RECV_TASK_ENCOURAGE::ID_HAVE_RECV_ERR;
    }
    //状态已经转移
    if(dwTaskIdx > poTask->GetDT_TASK_DATA().dwTaskIdx)
    {
        return ERR_RECV_TASK_ENCOURAGE::ID_STATE_ERR;
    }

    UINT16 wErrCode = poTask->Encourage();
    if(ERR_RECV_TASK_ENCOURAGE::ID_SUCCESS != wErrCode)
    {
        return wErrCode;
    }

    //从奖励任务map中删除
    poPlayer->GetEncourageTask().DelData(byTaskLine);
    poPlayer->GetFinishTask().AddData(byTaskLine, poTask);

    //赋值下一个任务
    GiveNextTask(poPlayer, poTask);

    //回调处理 是否可以接受任务
    CkWantCheckAccept(poPlayer);

    //检测任务线是否完成
    CheckFinish(poPlayer);

    return ERR_RECV_TASK_ENCOURAGE::ID_SUCCESS;
}
