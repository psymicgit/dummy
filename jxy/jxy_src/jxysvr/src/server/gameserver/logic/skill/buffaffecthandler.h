

#ifndef _BUFFAFFECTHANDLER_H_
#define _BUFFAFFECTHANDLER_H_

#include <sdtype.h>
#include <sdhashmap.h>

class CBuffAffect;
using namespace std ;

typedef HashMap<UINT8,CBuffAffect*> CBuffAffectOpratorMap;
typedef CBuffAffectOpratorMap::iterator CBuffAffectOpratorMapItr;

/**
* @brief buff效果处理类
*/
class CBuffAffectHandler
{
public:
	static CBuffAffectHandler* Instance() { static CBuffAffectHandler oBuffAffectHandler; return &oBuffAffectHandler;}
protected:
	CBuffAffectHandler(){ Init(); };        
	~CBuffAffectHandler(){};
public:
	CBuffAffect* GetBuffAffect(UINT8 byBuffKind);
protected:
	VOID Init();
private:
	CBuffAffectOpratorMap m_mapAffectOprator;
};

#endif //#ifndef _BUFFAFFECTHANDLER_H_

