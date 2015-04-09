
#ifndef _LUAACTIVITY_H_
#define _LUAACTIVITY_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/client/commondef.h>
#include <common/server/utility.h>

#include <map>

using namespace std;

typedef map<UINT32, DT_LUAACTIVITY_RECORD_DATA> CDT_LUAACTIVITY_RECORD_DATAMap;
typedef CDT_LUAACTIVITY_RECORD_DATAMap::iterator CDT_LUAACTIVITY_RECORD_DATAMapItr;

class CPlayer;
class CLuaActivity:public CBaseObj
{
public:
	CLuaActivity();
	~CLuaActivity();
public:
	BOOL Init(const DT_LUAACTIVITY_RECORD_LIST_DATA& stDT_LUAACTIVITY_RECORD_LIST_DATA, CPlayer* poOwner);
	VOID GetDT_ACTIVITY_RECORD_DATA(DT_LUAACTIVITY_RECORD_LIST_DATA& stDT_LUAACTIVITY_RECORD_LIST_DATA);

	UINT16 CkActivityHaveDoNum(UINT32 dwId);
	VOID RecordDo(UINT32 dwId);
	BOOL ActivityEncourage(UINT32 dwId, EBurstKind eEncKind, UINT32 dwValue1, UINT32 dwValue2);


private:
	
	CDT_LUAACTIVITY_RECORD_DATAMap m_mapActivityEndTimeRecord;
};
DECLARE_CLASS_POOL_MGR(CLuaActivity)

#endif

