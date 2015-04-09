
#ifndef _WORSHIP_H_
#define _WORSHIP_H_

#include <logic/base/baseobj.h>
#include <logic/base/basedefine.h>
#include <common/server/utility.h>
#include "worshippropmgr.h"

using namespace std ;

typedef map<UINT16, DT_WORSHIP_DATA*> CDT_WORSHIP_DATAMap;
typedef CDT_WORSHIP_DATAMap::iterator CDT_WORSHIP_DATAMapItr;

struct tagDT_WORSHIP_DATA_CLI_LST;
typedef tagDT_WORSHIP_DATA_CLI_LST DT_WORSHIP_DATA_CLI_LST;

struct tagDT_WORSHIP_DATA_CLI;
typedef tagDT_WORSHIP_DATA_CLI DT_WORSHIP_DATA_CLI;

struct tagDT_FUNC_COMMON_PROMPT;
typedef tagDT_FUNC_COMMON_PROMPT DT_FUNC_COMMON_PROMPT;

class CPlayer;
class CWorship: public CBaseObj
{
public:
    CWorship();
    ~CWorship();
public:
    //初始化相关对象
    BOOL Init(DT_WORSHIP_DATA_LST& stDT_WORSHIP_DATA_LST, CPlayer* poOwner);
    DT_WORSHIP_DATA_LST& GetDT_WORSHIP_DATA_LST() { return m_stDT_WORSHIP_DATA_LST; }
	UINT16 OpenWorshipTab(UINT8 byWorshipType, DT_WORSHIP_DATA_CLI_LST& stWorshipInfo);
	UINT16 Worship(UINT8 byWorshipType, UINT8 byWorshipTypeLevel, DT_WORSHIP_DATA_CLI_LST& stWorshipInfo);
	BOOL GetPrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt);
	BOOL CKCanWorship(UINT8 byWorshipType, BOOL bPrompt = FALSE, UINT8 byWorshipTypeLevel = 0);//检测是否可膜拜，true为可膜拜
protected:	
	VOID GetDT_WORSHIP_DATA_CLI(DT_WORSHIP_DATA& stDT_WORSHIP_DATA, DT_WORSHIP_DATA_CLI& stDT_WORSHIP_DATA_CLI);
	VOID AddNewProp(CLevel2WorshipPropMap& mapProp);	
private:
    DT_WORSHIP_DATA_LST	m_stDT_WORSHIP_DATA_LST;
	CDT_WORSHIP_DATAMap	m_mapDT_WORSHIP_DATA;
};

DECLARE_CLASS_POOL_MGR(CWorship)
#endif //#ifndef 

