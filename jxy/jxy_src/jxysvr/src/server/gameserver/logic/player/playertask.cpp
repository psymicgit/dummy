#include "player.h"

#include <logic/task/taskmgr.h>
#include <logic/task/taskpropmgr.h>

BOOL CPlayer::InitTask(DT_TASK_DATA_LIST& stTaskData)
{
    UINT8 byTaskNum = stTaskData.byTaskNum;

    HashMap<UINT8, UINT8> mapHaveTaskLine;
    for(UINT8 byIdx = 0; byIdx < byTaskNum && byIdx < MAX_TASK_LINE_NUM; byIdx++)
    {
        DT_TASK_DATA* pstDT_TASK_DATA = &(stTaskData.astTaskInfoList[byIdx]);
        mapHaveTaskLine[pstDT_TASK_DATA->byTaskLine] = pstDT_TASK_DATA->byTaskLine;
        if(NULL == InitTask(pstDT_TASK_DATA))
        {
            continue;//跳过有问题数据
        }
    }

    //给新增任务线任务
    CTaskLine2TaskLinePropMap mapTaskLine = CTaskPropMgr::Instance()->GetTaskLine2TaskLinePropMap();
    for(CTaskLine2TaskLinePropMapItr itr = mapTaskLine.begin(); itr != mapTaskLine.end(); itr++)
    {
        CTaskLineProp* pstTaskLineProp = itr->second;
        HashMap<UINT8, UINT8>::iterator itrHave = mapHaveTaskLine.find(itr->first);
        //没有该线任务则添加
        if(itrHave == mapHaveTaskLine.end())
        {
            DT_TASK_DATA stDT_TASK_DATA;
            memset(&stDT_TASK_DATA, 0, sizeof(stDT_TASK_DATA));
            stDT_TASK_DATA.byTaskLine = itr->first;
            stDT_TASK_DATA.dwTaskIdx = pstTaskLineProp->dwFirstTaskIdx;
            stDT_TASK_DATA.byTaskState = ETS_GIVEN;
            if(!CTaskMgr::Instance()->SetTaskCond(this, stDT_TASK_DATA.byTaskLine, stDT_TASK_DATA.dwTaskIdx, stDT_TASK_DATA.byTaskType, stDT_TASK_DATA.stTaskConditionInfo))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: SetTaskCond failed!"), MSG_MARK);
                continue;
            }
            InitTask(&stDT_TASK_DATA);
        }
    }

    //玩家刚上线，检查任务条件
    CTaskMgr::Instance()->CkWantCheckAccept(this);
    CTaskMgr::Instance()->CheckFinish(this);

    return TRUE;

}

CTask* CPlayer::InitTask(DT_TASK_DATA* pstDT_TASK_DATA)
{
    CTask* poTask = CTaskMgr::Instance()->CreateTask(m_dwPlayerID, pstDT_TASK_DATA->byTaskLine);
    if(NULL == poTask)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateTask failed !"), MSG_MARK);
        return NULL;
    }
    if(FALSE == poTask->Init(pstDT_TASK_DATA, this))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: task init failed, line[%d], idx[%d]!"), MSG_MARK, pstDT_TASK_DATA->byTaskLine, pstDT_TASK_DATA->dwTaskIdx);
        CTaskMgr::Instance()->RemoveTask(m_dwPlayerID, pstDT_TASK_DATA->byTaskLine);
        return NULL;
    }

    DT_TASK_DATA& stDT_TASK_DATA = poTask->GetDT_TASK_DATA();
    if(ETS_GIVEN == stDT_TASK_DATA.byTaskState)
    {
        m_mapGiveTask.AddData(stDT_TASK_DATA.byTaskLine, poTask);
    }
    if(ETS_ACCEPTED == stDT_TASK_DATA.byTaskState)
    {
        m_mapAcceptTask.AddData(stDT_TASK_DATA.byTaskLine, poTask);
    }
    if(ETS_ENCOURAGE == stDT_TASK_DATA.byTaskState)
    {
        m_mapEncourageTask.AddData(stDT_TASK_DATA.byTaskLine, poTask);
    }
    if(ETS_FINISHED == stDT_TASK_DATA.byTaskState)
    {
        m_mapFinishTask.AddData(stDT_TASK_DATA.byTaskLine, poTask);
    }
    if(ETS_INVALID == stDT_TASK_DATA.byTaskState)
    {
        //已无效的任务会放m_mapAllTask中，少参与其他的逻辑，不需另行保存
    }
    m_mapAllTask.AddData(stDT_TASK_DATA.byTaskLine, poTask);

    //赋值上一个接受的主线任务配置
    if(MAIN_LINE_TASK == stDT_TASK_DATA.byTaskLine)
    {
        UINT32 dwTaskIdx = stDT_TASK_DATA.dwTaskIdx;
        if( ETS_GIVEN == stDT_TASK_DATA.byTaskState)
        {
            if(dwTaskIdx <= 1 )
            {
                m_dwLastAcceptMainLineTaskIdx = 0;
            }
            else
            {
                CTaskLineProp* poTaskLineProp = CTaskPropMgr::Instance()->GetTaskLineProp(MAIN_LINE_TASK);
                if(poTaskLineProp)
                {
                    STaskProp* pstSTaskProp = poTaskLineProp->GetLastTask(dwTaskIdx);
                    if(pstSTaskProp)
                    {
                        m_dwLastAcceptMainLineTaskIdx = pstSTaskProp->dwTaskIdx;
                    }

                }
            }
        }
        else
        {
            STaskProp* pstSTaskProp = CTaskPropMgr::Instance()->GetTaksProp(MAIN_LINE_TASK, dwTaskIdx);
            if(pstSTaskProp)
            {
                m_dwLastAcceptMainLineTaskIdx = pstSTaskProp->dwTaskIdx;
            }
        }
    }

    return poTask;
}


CTask* CPlayer::GetTask(UINT8 byLineIdx)
{
    return m_mapAllTask.GetData(byLineIdx);
}

VOID CPlayer::GetDT_TASK_DATA_LIST_CLI(DT_TASK_DATA_LIST_CLI& stTaskInfoList, BOOL bLogin)
{
    UINT8 byTaskNum = 0;
    CTask* poTask = m_mapAllTask.GetFistData();
    while(poTask && ((byTaskNum < MAX_TASK_LINE_NUM)))
    {
        //已完成/无效/未接受(非主线)的不需下发
        if((ETS_FINISHED == poTask->GetDT_TASK_DATA().byTaskState) ||
                (ETS_INVALID == poTask->GetDT_TASK_DATA().byTaskState) ||
                ((ETS_GIVEN == poTask->GetDT_TASK_DATA().byTaskState) && (MAIN_LINE_TASK != poTask->GetDT_TASK_DATA().byTaskLine)))
        {
            poTask = m_mapAllTask.GetNextData();
            continue;
        }



        //292 检测客户端版本号，过虑掉
        if (GetCliVer() <= 292 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
                90003 == poTask->GetDT_TASK_DATA().dwTaskIdx)
        {
            poTask = m_mapAllTask.GetNextData();
            continue;
        }
        //@292
		//306 检测客户端版本号，过虑掉
		if (GetCliVer() <= 306 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
			99003 < poTask->GetDT_TASK_DATA().dwTaskIdx)
		{
			poTask = m_mapAllTask.GetNextData();
			continue;
		}
        DT_TASK_DATA_CLI& stDT_TASK_DATA_CLI = stTaskInfoList.astTaskInfoList[byTaskNum++];
        GetDT_TASK_DATA_CLI(poTask, stDT_TASK_DATA_CLI, bLogin);

        poTask = m_mapAllTask.GetNextData();
    }
    m_mapChangeTask.Clear();
    m_mapNBIEncourageTask.Clear();

    stTaskInfoList.byTaskNum = byTaskNum;
}



VOID CPlayer::GetDT_TASK_DATA_LIST_CLI_Change(DT_TASK_DATA_LIST_CLI& stTaskInfoList)
{
    UINT8 byTaskNum = 0;
    CTask* poTask = m_mapChangeTask.GetFistData();
    while(poTask)
    {
        //已完成/无效/未接受(非主线)的不需下发
        if((ETS_FINISHED == poTask->GetDT_TASK_DATA().byTaskState) ||
                (ETS_INVALID == poTask->GetDT_TASK_DATA().byTaskState) ||
                ((ETS_GIVEN == poTask->GetDT_TASK_DATA().byTaskState) && (MAIN_LINE_TASK != poTask->GetDT_TASK_DATA().byTaskLine)))
        {
            poTask = m_mapAllTask.GetNextData();
            continue;
        }

        //292 检测客户端版本号，过虑掉
        if (GetCliVer() <= 292 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
                90003 == poTask->GetDT_TASK_DATA().dwTaskIdx)
        {
            poTask = m_mapAllTask.GetNextData();
            continue;
        }
		//306 检测客户端版本号，过虑掉
		if (GetCliVer() <= 306 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
			99003 < poTask->GetDT_TASK_DATA().dwTaskIdx)
		{
			poTask = m_mapAllTask.GetNextData();
			continue;
		}

        DT_TASK_DATA_CLI& stDT_TASK_DATA_CLI = stTaskInfoList.astTaskInfoList[byTaskNum++];
        GetDT_TASK_DATA_CLI(poTask, stDT_TASK_DATA_CLI);

        poTask = m_mapChangeTask.GetNextData();
    }
    //下发已完成未领取奖励的
    poTask = m_mapEncourageTask.GetFistData();
    while(poTask)
    {
        if(!m_mapChangeTask.GetData(poTask->GetDT_TASK_DATA().byTaskLine))
        {
            //292 检测客户端版本号，过虑掉
            if (GetCliVer() <= 292 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
                    90003 == poTask->GetDT_TASK_DATA().dwTaskIdx)
            {
                poTask = m_mapEncourageTask.GetNextData();
                continue;
            }
            //@292
			//306 检测客户端版本号，过虑掉
			if (GetCliVer() <= 306 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
				99003 < poTask->GetDT_TASK_DATA().dwTaskIdx)
			{
				poTask = m_mapEncourageTask.GetNextData();
				continue;
			}
            DT_TASK_DATA_CLI& stDT_TASK_DATA_CLI = stTaskInfoList.astTaskInfoList[byTaskNum++];
            GetDT_TASK_DATA_CLI(poTask, stDT_TASK_DATA_CLI);
        }

        poTask = m_mapEncourageTask.GetNextData();
    }
    stTaskInfoList.byTaskNum = byTaskNum;

    m_mapChangeTask.Clear();
    m_mapNBIEncourageTask.Clear();
}


VOID CPlayer::GetDT_TASK_DATA_CLI(CTask* poTask, DT_TASK_DATA_CLI& stDT_TASK_DATA_CLI, BOOL bLogin)
{
    if (NULL == poTask)
    {
        return;
    }


    //292 检测客户端版本号，过虑掉
    if (GetCliVer() <= 292 &&
            1 == poTask->GetDT_TASK_DATA().byTaskLine &&
            90003 == poTask->GetDT_TASK_DATA().dwTaskIdx)
    {
        return;
    }
    //@292
	//306 检测客户端版本号，过虑掉
	if (GetCliVer() <= 306 &&
		1 == poTask->GetDT_TASK_DATA().byTaskLine &&
		99003 < poTask->GetDT_TASK_DATA().dwTaskIdx)
	{
		return;
	}

    DT_TASK_DATA& stDT_TASK_DATA = poTask->GetDT_TASK_DATA();
    switch(poTask->GetDT_TASK_DATA().byTaskState)
    {
    case ETS_GIVEN:
    {
        stDT_TASK_DATA_CLI.byTaskState = ETS_GIVEN;
        DT_GIVEN_TASK_DATA& stGivenTask = stDT_TASK_DATA_CLI.stTaskInfo.stGivenTask;
        poTask->GetDT_TASK_BASE_DATA_CLI(stGivenTask.stTaskBaseData);
        DT_TASK_DATA& stDT_TASK_DATA = poTask->GetDT_TASK_DATA();
        stGivenTask.byTaskLineIdx = stDT_TASK_DATA.byTaskLine;
        stGivenTask.dwTaskIdx = stDT_TASK_DATA.dwTaskIdx;
        stGivenTask.qwStateUpdateTime = stDT_TASK_DATA.qwStateUpdateTime;
        STaskProp *poTaskProp = poTask->GetTaskProp();
        if (NULL == poTaskProp)
        {
            return;
        }
        stGivenTask.wPlayerLevel = poTaskProp->wAcceptPlayerLevel;
        memcpy(&(stGivenTask.stInstanceID), &(poTaskProp->stAcceptInstanceID), sizeof(DT_INSTANCE_ID));
        stGivenTask.byCanAccept = poTask->GetCanAcceptFalg();
    }
    break;
    case ETS_ACCEPTED:
    {
        stDT_TASK_DATA_CLI.byTaskState = ETS_ACCEPTED;
        DT_ACCEPT_TASK_DATA& stAcceptTask = stDT_TASK_DATA_CLI.stTaskInfo.stAcceptTask;
        poTask->GetDT_TASK_BASE_DATA_CLI(stAcceptTask.stTaskBaseData);
        DT_TASK_DATA& stDT_TASK_DATA = poTask->GetDT_TASK_DATA();
        stAcceptTask.byTaskLineIdx = stDT_TASK_DATA.byTaskLine;
        stAcceptTask.dwTaskIdx = stDT_TASK_DATA.dwTaskIdx;
        stAcceptTask.qwStateUpdateTime = stDT_TASK_DATA.qwStateUpdateTime;
    }
    break;
    case ETS_ENCOURAGE:
    {
        stDT_TASK_DATA_CLI.byTaskState = ETS_ENCOURAGE;
        DT_ENCOURAGE_TASK_DATA& stEncourageTask = stDT_TASK_DATA_CLI.stTaskInfo.stEncourageTask;
        if((CkNeedTaskBaseInfo(poTask)) || (bLogin))
        {
            stEncourageTask.byHaveBaseData = 1;
            poTask->GetDT_TASK_BASE_DATA_CLI(stEncourageTask.astTaskBaseData[0]);
        }
        else
        {
            stDT_TASK_DATA_CLI.byTaskState = stDT_TASK_DATA.byTaskState;
            stEncourageTask.byHaveBaseData = 0;
        }
        stEncourageTask.byTaskLineIdx = stDT_TASK_DATA.byTaskLine;
        stEncourageTask.dwTaskIdx = stDT_TASK_DATA.dwTaskIdx;
        stEncourageTask.qwStateUpdateTime = stDT_TASK_DATA.qwStateUpdateTime;
    }
    break;
    case ETS_INVALID:
    {
        stDT_TASK_DATA_CLI.byTaskState = ETS_INVALID;
        DT_INVALID_TASK_DATA& stInvalidTask = stDT_TASK_DATA_CLI.stTaskInfo.stInvalidTask;
        stInvalidTask.byTaskLineIdx = stDT_TASK_DATA.byTaskLine;
        stInvalidTask.dwTaskIdx = stDT_TASK_DATA.dwTaskIdx;
    }
    break;
    }

}

VOID CPlayer::GetDT_TASK_COND_RECORD_LIST(DT_TASK_COND_RECORD_LIST& stTaskRecordList)
{
    UINT8 byTaskNum = 0;
    CTask* poTask = m_mapAcceptTask.GetFistData();
    while(poTask)
    {
        //292 检测客户端版本号，过虑掉
        if (GetCliVer() <= 292 && 1 == poTask->GetDT_TASK_DATA().byTaskLine &&
                90003 == poTask->GetDT_TASK_DATA().dwTaskIdx)
        {
            poTask = m_mapAcceptTask.GetNextData();
            continue;
        }
		//306 检测客户端版本号，过虑掉
		if (GetCliVer() <= 306 &&
			1 == poTask->GetDT_TASK_DATA().byTaskLine &&
			99003 < poTask->GetDT_TASK_DATA().dwTaskIdx)
		{
			poTask = m_mapAcceptTask.GetNextData();
			return;
		}

        DT_ACCEPT_TASK_COND_RECORD& stDT_ACCEPT_TASK_COND_RECORD = stTaskRecordList.astTaskConRecordList[byTaskNum++];
        stDT_ACCEPT_TASK_COND_RECORD.byTaskLineIdx = poTask->GetDT_TASK_DATA().byTaskLine;
        stDT_ACCEPT_TASK_COND_RECORD.dwTaskIdx = poTask->GetDT_TASK_DATA().dwTaskIdx;
        stDT_ACCEPT_TASK_COND_RECORD.byTaskType = poTask->GetDT_TASK_DATA().byTaskType;
        memcpy(&stDT_ACCEPT_TASK_COND_RECORD.stTaskCondRecord, &(poTask->GetDT_TASK_DATA().stTaskConditionInfo), sizeof(DT_TASK_CONDITION));

        poTask = m_mapAcceptTask.GetNextData();
    }
    stTaskRecordList.byTaskNum = byTaskNum;
}


//记录变化的任务
VOID CPlayer::RecordChangeTask(CTask* poTask)
{
    if(NULL == poTask)
    {
        return;
    }
    //刚接受立即满足完成条件的领取奖励状态任务（未下发的接受任务，当前状态已经修改成领取奖励状态），则需下发任务基本信息
    if((m_mapChangeTask.GetData(poTask->GetDT_TASK_DATA().byTaskLine)) && (ETS_ENCOURAGE == poTask->GetDT_TASK_DATA().byTaskState))
    {
        m_mapNBIEncourageTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
    }

    m_mapChangeTask.AddData(poTask->GetDT_TASK_DATA().byTaskLine, poTask);
}

//检查是否需下发任务基本信息，用于领取奖励状态任务
BOOL CPlayer::CkNeedTaskBaseInfo(CTask* poTask)
{
    if(NULL == poTask)
    {
        return FALSE;
    }

    if(m_mapNBIEncourageTask.GetData(poTask->GetDT_TASK_DATA().byTaskLine))
    {
        return TRUE;
    }

    return FALSE;
}

//返回值为当前已领取奖励任务是否新解锁副本
UINT8 CPlayer::GetMaxUnlockCommonInstanceID(DT_INSTANCE_ID& stDT_INSTANCE_ID, BOOL bAcceptTask)
{
    //
    if(0 == m_dwLastAcceptMainLineTaskIdx)
    {
        DT_INSTANCE_ID& stInstanceID = CTaskPropMgr::Instance()->GetInitUnlockInstanceID();
        stDT_INSTANCE_ID.wSceneIdx = stInstanceID.wSceneIdx;
        stDT_INSTANCE_ID.byTownIdx = stInstanceID.byTownIdx;
        stDT_INSTANCE_ID.byInstanceIdx = stInstanceID.byInstanceIdx;
    }
    else
    {
        STaskProp* pstSTaskProp = CTaskPropMgr::Instance()->GetTaksProp(MAIN_LINE_TASK, m_dwLastAcceptMainLineTaskIdx);
        if(NULL == pstSTaskProp)
        {
            CTaskLineProp* poTaskLineProp = CTaskPropMgr::Instance()->GetTaskLineProp(MAIN_LINE_TASK);
            if(poTaskLineProp)
            {
                pstSTaskProp = poTaskLineProp->GetNextTask(m_dwLastAcceptMainLineTaskIdx);
            }
        }
        if(NULL == pstSTaskProp)
        {
            DT_INSTANCE_ID& stInstanceID = CTaskPropMgr::Instance()->GetInitUnlockInstanceID();
            stDT_INSTANCE_ID.wSceneIdx = stInstanceID.wSceneIdx;
            stDT_INSTANCE_ID.byTownIdx = stInstanceID.byTownIdx;
            stDT_INSTANCE_ID.byInstanceIdx = stInstanceID.byInstanceIdx;
        }

        DT_INSTANCE_ID& stInstanceID = pstSTaskProp->stInstanceID;
        stDT_INSTANCE_ID.wSceneIdx = stInstanceID.wSceneIdx;
        stDT_INSTANCE_ID.byTownIdx = stInstanceID.byTownIdx;
        stDT_INSTANCE_ID.byInstanceIdx = stInstanceID.byInstanceIdx;
        if(bAcceptTask)
        {
            return pstSTaskProp->bNewUnlockInstanceID == TRUE ? 1 : 0;
        }
    }

    return 0;
}

//检查任务是否已经完成，已完成返回TRUE
BOOL CPlayer::CkTaskFinish(UINT8 byTaskLine, UINT32 dwTaskIdx)
{
    CTask* poTask = m_mapAllTask.GetData(byTaskLine);
    if(NULL == poTask)
    {
        return FALSE;
    }

    DT_TASK_DATA& stDT_TASK_DATA = poTask->GetDT_TASK_DATA();
    if(((ETS_FINISHED == stDT_TASK_DATA.byTaskState) && (stDT_TASK_DATA.dwTaskIdx == dwTaskIdx))
            || (stDT_TASK_DATA.dwTaskIdx > dwTaskIdx))
    {
        return TRUE;
    }

    return FALSE;
}


CTask* CPlayer::GetTaskByLine(UINT8 byTaskLine)
{
    return m_mapAllTask.GetData(byTaskLine);
}

VOID CPlayer::GetUnlockNewInstance(UINT8& byUnlockNewInstanceFlag, UINT8& byHaveNewInstanceFlag, DT_INSTANCE_ID& stUnlockCommonInstanceID, DT_INSTANCE_DATA_CLI& stNewCommonInstanceID)
{
    byUnlockNewInstanceFlag = 0;
    byHaveNewInstanceFlag = 0;
    if(m_mapChangeTask.GetData(MAIN_LINE_TASK)) //只有主线任务才解锁新副本
    {
        byUnlockNewInstanceFlag = GetMaxUnlockCommonInstanceID(stUnlockCommonInstanceID, TRUE);
        if (m_poInstance)
            byHaveNewInstanceFlag = m_poInstance->GetNewCommonInstanceID(byUnlockNewInstanceFlag, stNewCommonInstanceID);
    }
}
