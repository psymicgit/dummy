
#ifndef _TASKMGR_H_
#define _TASKMGR_H_

#include "task.h"
#include "taskpropmgr.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdstring.h>

using namespace SGDP;



typedef HashMap<UINT64, CTask*> CID2TaskMap; //key为CTask的MemID
typedef CID2TaskMap::iterator CID2TaskMapItr;

typedef HashMap<UINT8, CTask*> CLineIdx2TaskMap; //key为任务线序号
typedef CLineIdx2TaskMap::iterator CLineIdx2TaskMapItr;

typedef vector<CTask*> CTaskVec;
typedef CTaskVec::iterator CTaskVecItr;

/**
* @brief 任务管理类
*/
class CTaskMgr
{
public:
	DECLARE_SINGLETON(CTaskMgr)

	BOOL Init();

	VOID UnInit();

public:

	CTask*	CreateTask(UINT32 dwPlayerID, UINT8 byTaskLine);

	VOID	RemoveTask(UINT32 dwPlayerID, UINT8 byTaskLine);
public:
	VOID	CkWantCheckAccept(CPlayer* poPlayer);//检查任务是否可接，若之前不可接现可接则记录为变化任务
	//接受任务
	//UINT16 AcceptTask(CPlayer* poPlayer, UINT8 byTaskLine, UINT8 byTaskIdx);

	//放弃任务
	//UINT16 GiveUpTask(CPlayer* poPlayer, UINT8 byTaskLine, UINT8 byTaskIdx);

	//任务完成条件判断
	VOID CheckFinish(CPlayer* poPlayer);//任务完成检查，有任务接受后(CkWantCheckAccept调用时)调用

	////////////////////////////////////////////////
	//任务完成条件回调
	VOID OnPassInstance(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);//接受/完成条件,首次通过副本任务,指定分数通过副本任务,指定次数通过副本任务
	VOID OnCrown(CPlayer* poPlayer);//收获任务,主城收获不算
	VOID OnRecruitHero(CPlayer* poPlayer, UINT16 wHeroKindID);//招募武将任务
	VOID OnClimbTower(CPlayer* poPlayer);//武将升级任务
	VOID OnKillMonsterInstance(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterKindID);//指定副本杀怪任务
	VOID OnStrengthenEquip(CPlayer* poPlayer, UINT16 wEquipID);//强化装备任务
	VOID OnActivateGodweapon(CPlayer* poPlayer);//激活神器 任务
	VOID OnUpgradeGodweapon(CPlayer* poPlayer);//升级神器 任务
	VOID OnChallenge(CPlayer* poPlayer); //竞技场挑战 任务
	VOID OnPlunder(CPlayer* poPlayer); //掠夺 任务
	VOID OnUpgradeSkill(CPlayer* poPlayer); //升级技能 任务
	VOID OnUpgradeJingJie(CPlayer* poPlayer, UINT16 wLevel, UINT16 wSubLevel); //升阶境界 任务
	VOID OnMosaicGem(CPlayer* poPlayer); //升级技能 任务
	VOID OnDressEquip(CPlayer* poPlayer, UINT16 wEquipID); //穿戴装备 任务
	VOID OnComposeGood(CPlayer* poPlayer, UINT16 wMaterialID); //合成物品 任务
	VOID OnEliteInstance(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx); //精英副本 任务
	VOID OnUpgradeScience(CPlayer* poPlayer); //升级科技点任务 任务
	VOID OnStrengthAttack(CPlayer* poPlayer); //强化攻击 任务
	VOID OnStrengthLife(CPlayer* poPlayer); //强化生命 任务
	VOID OnCrop(CPlayer* poPlayer); //合成物品 任务
	VOID OnUseExprienceDrug(CPlayer* poPlayer); //使用经验丹任务
	VOID OnMelt(CPlayer* poPlayer);	//溶练
	VOID OnStudy(CPlayer* poPlayer);
	VOID OnRecruitHeroNum(CPlayer* poPlayer, UINT16 wHeroKindID);//招募武将任务

	VOID OnOpenLight(CPlayer* poPlayer);//开光任务
	////////////////////////////////////////////////
	BOOL GiveNextTask(CPlayer* poPlayer, CTask* poTask);	
	BOOL SetTaskCond(CPlayer* poPlayer, UINT8 byCurTaskLine, UINT32 dwCurTaskIdx, UINT8& byTaskConditonType, DT_TASK_CONDITION& stTaskConditionInfo);//设置任务条件

	////////////////////////////////////////////////
	UINT16 RecvTaskEncourage(CPlayer* poPlayer, UINT8 byTaskLine, UINT32 dwTaskIdx);
	
protected:		
	
	CTask* FindTask(UINT32 dwPlayerID, UINT8 byTaskLine);
	//VOID GiveBranchTask(CPlayer* poPlayer, STaskProp* pstLastMainTaskProp);//给主线的支线任务
protected:
	CTaskMgr();
	~CTaskMgr();
protected:

private:
	CID2TaskMap					m_mapID2Task;
	CSDObjectPool<CTask, CSDMutex> m_oTaskShmemObjMgr;
};




#endif //#ifndef _ROLEMGR_H_

