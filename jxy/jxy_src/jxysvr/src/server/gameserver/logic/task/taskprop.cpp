#include "taskprop.h"
#include "acceptconditon.h"
#include "finishconditon.h"


STaskEncourageProp::STaskEncourageProp()
{
	dwCoin	= 0;
	dwGold	= 0;
	wPhyStrength = 0;
	dwScience	= 0;
	dwExperience = 0;
	dwFame	= 0;
	dwStory = 0;
	dwBlueGas = 0; //蓝气
	dwPurpleGas = 0; //紫气
	dwJingJie = 0;
	mapItemNum.clear();
}
STaskEncourageProp::~STaskEncourageProp()
{
	mapItemNum.clear();
}


STaskProp::STaskProp()
{
	byTaskLineIdx = 0;
	dwTaskIdx = 0;
	eTaskType = INVALID;
	mapAcceptCondition.clear();
	poFinishCondition = NULL;
	memset(&stInstanceID, 0, sizeof(stInstanceID));
	wAcceptPlayerLevel = 0;
	memset(&stAcceptInstanceID, 0, sizeof(stAcceptInstanceID));
	dwNextTaskIdx = 0;
}

STaskProp::~STaskProp()
{
	for(CAcceptType2CheckMapItr itr = mapAcceptCondition.begin(); itr != mapAcceptCondition.end(); itr++)
	{
		SDDelete itr->second;
		itr->second = NULL;
	}
	mapAcceptCondition.clear();

	if(poFinishCondition)
	{
		SDDelete poFinishCondition;
		poFinishCondition = NULL;
	}

	/*
	for(CTaskIdx2TaskPropMapItr itr = mapBranchTask.begin(); itr != mapBranchTask.end(); itr++)
	{
		SDDelete itr->second;
	}
	mapBranchTask.clear();
	*/

}


CTaskLineProp::CTaskLineProp()
{
	byTaskLineIdx = 0;
	dwFirstTaskIdx = 0;
	mapTaskProp.clear();

}


CTaskLineProp::~CTaskLineProp()
{
	for(CTaskIdx2TaskPropMapItr itr = mapTaskProp.begin(); itr != mapTaskProp.end(); itr++)
	{
		SDDelete itr->second;
		itr->second = NULL;
	}
	mapTaskProp.clear();
}



//获取dwTaskIdx的下一个任务，dwTaskIdx不一定有效，没有返回NULL
STaskProp* CTaskLineProp::GetNextTask(UINT32 dwTaskIdx)
{
	STaskProp stProp;
	stProp.byTaskLineIdx = byTaskLineIdx;
	stProp.dwTaskIdx = dwTaskIdx;
	CTaskIdx2TaskPropMapItr itr = mapTaskProp.upper_bound(dwTaskIdx);
	if(itr != mapTaskProp.end())
	{
		return itr->second;
	}

	return NULL;
}

STaskProp* CTaskLineProp::GetLastTask(UINT32 dwTaskIdx)
{
	STaskProp stProp;
	stProp.byTaskLineIdx = byTaskLineIdx;
	stProp.dwTaskIdx = dwTaskIdx;
	CTaskIdx2TaskPropMapItr itr = mapTaskProp.find(dwTaskIdx);
	if(itr != mapTaskProp.end())
	{
		itr--;
		if(itr != mapTaskProp.end())
		{
			return itr->second;
		}		
	}

	return NULL;
}



