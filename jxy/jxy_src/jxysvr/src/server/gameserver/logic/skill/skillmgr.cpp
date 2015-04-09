
#include "skillmgr.h"
#include "skillaffecthandler.h"
#include "buffaffecthandler.h"
#include "skillpropmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/base/logicfunc.h>
#include <logic/player/player.h>
#include "logic/pet/pet.h"

IMPLEMENT_SINGLETON(CSkillMgr)

CSkillMgr::CSkillMgr()
{

}

CSkillMgr::~CSkillMgr()
{
	CSkillPropMgr::DestroyInstance();
}

BOOL CSkillMgr::Init()
{
	//读取配置文件

	// 初始化CSkillPropMgr管理器
	if(FALSE == CSkillPropMgr::CreateInstance())
	{
		USR_INFO(_SDT("[%s: %d]: CSkillPropMgr::CreateInstance failed!"), MSG_MARK);        
		return FALSE;
	}
	if(FALSE == CSkillPropMgr::Instance()->Init())
	{
		USR_INFO(_SDT("[%s: %d]: CSkillPropMgr Init failed!"), MSG_MARK);        
		return FALSE;
	}


	return TRUE;

}


VOID CSkillMgr::UnInit()
{
	CSkillPropMgr::Instance()->UnInit();

}


CSkill* CSkillMgr::CreateSkill()
{
	return m_oSkillShmemObjMgr.Alloc();
}

VOID CSkillMgr::RemoveSkill(CSkill* poSkill)
{
	m_oSkillShmemObjMgr.Free(poSkill);  
}

VOID CSkillMgr::AddBuff(CBattleAttrObj* poSkillOwner, CBattleAttrObj* poTargetObj, UINT16 wSkillID, const SSkillOnceBuffProp& stBuffProp)
{
	//判断添加BUFF概率
	static UINT32 dwSeed = 0;
	dwSeed = (GetCycleCount() + (dwSeed++)) << 7;
	SDSetSeed(dwSeed);
	if(SDRandom(100) > stBuffProp.byBuffRate)
	{
		return;
	}

	CBuff* poBuff = CBuffMgr::Instance()->CreateBuff();
	if(NULL == poBuff)
	{
		return ;
	}
	if(!poBuff->Init(wSkillID, stBuffProp.byBuffKindID, poSkillOwner, poTargetObj, stBuffProp.wBuffParam, stBuffProp.byBuffRound))
	{
		CBuffMgr::Instance()->RemoveBuff(poBuff->GetMemID());
		return ;
	}
	if(!poTargetObj->AddBuff(poBuff))
	{
		CBuffMgr::Instance()->RemoveBuff(poBuff->GetMemID());
		return ;
	}
	CPet* poPet = dynamic_cast<CPet*>(poSkillOwner);
	if ( NULL != poPet)
	{
		poBuff->SetExtValue(poPet->GetLastHurt());
	}
}

VOID CSkillMgr::AddSleepBuff(CBattleAttrObj* poSkillOwner, CBattleAttrObj* poTargetObj)
{

	CBuff* poBuff = CBuffMgr::Instance()->CreateBuff();
	if(NULL == poBuff)
	{
		return ;
	}
	if(!poBuff->Init(0, EBK_SLEEP, poSkillOwner, poTargetObj, 0, 1))
	{
		CBuffMgr::Instance()->RemoveBuff(poBuff->GetMemID());
		return ;
	}
	if(!poTargetObj->AddBuff(poBuff))
	{
		CBuffMgr::Instance()->RemoveBuff(poBuff->GetMemID());
		return ;
	}
	poBuff->Affect(EBAP_ROUND_BEGIN, TRUE, poSkillOwner, poTargetObj);
}