
#ifndef _SKILLMGR_H_
#define _SKILLMGR_H_

#include "skill.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;


typedef HashMap<UINT64, CSkill*> CID2SkillMap;
typedef CID2SkillMap::iterator	CID2SkillMapItr;


/**
* @brief 技能管理类
*/
class CSkillMgr
{
public:
    DECLARE_SINGLETON(CSkillMgr)

    /**
    * @brief
    * 初始化函数,读取配置文件等
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Init();

    /**
    * @brief
    * 卸载函数
    */
    VOID UnInit();

public:

	CSkill*		CreateSkill();
	VOID		RemoveSkill(CSkill* poSkill);    

	VOID AddBuff(CBattleAttrObj* poSkillOwner, CBattleAttrObj* poTargetObj, UINT16 wSkillID, const SSkillOnceBuffProp& stBuffProp);
	VOID AddSleepBuff(CBattleAttrObj* poSkillOwner, CBattleAttrObj* poTargetObj);
protected:
    CSkillMgr();
    ~CSkillMgr();
	/////////////////////

private:
    CID2SkillMap					m_mapID2Skill;
	CSDObjectPool<CSkill, CSDMutex> m_oSkillShmemObjMgr;
};




#endif //#ifndef _ROLEMGR_H_

