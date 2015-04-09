#ifndef _TASK_H_
#define _TASK_H_

#include "taskprop.h"
#include <logic/base/baseobj.h>


//每条任务线信息,每个玩家都会有所有先的任务，区别在于是否达到接受条件
//新建玩家就给所有线的第一个任务
//所有任务在自己任务线内都是线性的


//ETS_GIVEN = 0,//给予
//ETS_ACCEPTED = 1,//接受,该状态下任务完成条件需保存
//ETS_ENCOURAGE = 2,//任务完成，需领取奖励
//ETS_FINISHED = 3,//完成且已经领取奖励,可能会存在于该线没有下一个任务时


class CPlayer;
class CTask: public CBaseObj
{
public:
    CTask();
    ~CTask();
public:
    BOOL		Init(DT_TASK_DATA* pstDT_TASK_DATA, CPlayer* poOwner);
    UINT16	CheckAccept();//检查接受条件是否成功，按任务类型选择性记录
    BOOL		CheckFinish();//检查完成条件是否成功，任务可完成返回TRUE，调用者清除该完成任务
    DT_TASK_DATA& GetDT_TASK_DATA()
    {
        return m_stDT_TASK_DATA;
    }
    DT_TASK_CONDITION& GetConditionRecord()
    {
        return m_stDT_TASK_DATA.stTaskConditionInfo;
    }
    STaskProp* GetTaskProp();
    VOID	GetDT_TASK_BASE_DATA_CLI(DT_TASK_BASE_DATA& stDT_TASK_BASE_DATA);
    VOID	GetDT_TASK_ENCOURAGE_DATA(DT_TASK_ENCOURAGE_DATA& stTaskEncourage);

    UINT16	Encourage();//奖励

    UINT8	GetCanAcceptFalg()
    {
        return m_byCanAcceptFlag;
    }
protected:
    BOOL		CheckFinish(CFinishCondition* poFinishCondition);
    UINT16	CheckAccept(CAcceptType2CheckMap& mapAcceptCondition);
private:
    DT_TASK_DATA		m_stDT_TASK_DATA;//任务记录
    UINT8				m_byCanAcceptFlag;//是否可接(用于给予状态的任务)
};

#endif

