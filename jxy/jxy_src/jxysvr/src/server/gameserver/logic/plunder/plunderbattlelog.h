
#ifndef _PLUNDERBATTLELOG_H_
#define _PLUNDERBATTLELOG_H_

#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <logic/base/baseobj.h>

class CPlayer;
class CPlunder;

struct tagDT_PLUNDER_BATTLELOG_DATA_CLI;
typedef tagDT_PLUNDER_BATTLELOG_DATA_CLI DT_PLUNDER_BATTLELOG_DATA_CLI;

class CPlunderBattleLog:public CBaseObj
{
public:
	CPlunderBattleLog();
	~CPlunderBattleLog();
public:
	BOOL Init(const DT_PLUNDER_BATTLELOG_DATA* pstDT_PLUNDER_BATTLELOG_DATA, CPlayer* poOwner, CPlunder* poPlunder);
	DT_PLUNDER_BATTLELOG_DATA& GetDT_PLUNDER_BATTLELOG_DATA() { return m_stDT_PLUNDER_BATTLELOG_DATA; }
	VOID GetDT_PLUNDER_BATTLELOG_DATA_CLI(DT_PLUNDER_BATTLELOG_DATA_CLI& stDT_PLUNDER_BATTLELOG_DATA_CLI);
protected:

private:
	DT_PLUNDER_BATTLELOG_DATA	m_stDT_PLUNDER_BATTLELOG_DATA;
	CPlunder*					m_poPlunder;
};


#endif

