

#ifndef _SKILLAFFECTHANDLER_H_
#define _SKILLAFFECTHANDLER_H_

#include <sdhashmap.h>
#include "skillaffect.h"

using namespace std ;

typedef HashMap<UINT8,CSkillOnceAffect*> CSkillAffectOpratorMap;
typedef CSkillAffectOpratorMap::iterator CSkillAffectOpratorMapItr;

/**
* @brief buff效果处理类
*/
class CSkillOnceAffectHandler
{
public:
	static CSkillOnceAffectHandler* Instance() { static CSkillOnceAffectHandler oSkillAffectHandler; return &oSkillAffectHandler;}
protected:
	CSkillOnceAffectHandler(){ Init(); };        
	~CSkillOnceAffectHandler(){};
public:
	CSkillOnceAffect* GetSkillAffect(UINT8 bySkillEffectKindID);
protected:
	VOID Init();
private:
	CSkillAffectOpratorMap m_mapAffectOprator;
};

#endif //#ifndef _BUFFAFFECTHANDLER_H_

