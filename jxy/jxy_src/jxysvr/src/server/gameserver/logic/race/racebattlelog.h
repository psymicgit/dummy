
#ifndef _RACEBATTLELOG_H_
#define _RACEBATTLELOG_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocommondata.h>

class CRaceBuild;
class CPlayer;

struct tagDT_RACE_BATTLELOG_DATA_CLI;
typedef tagDT_RACE_BATTLELOG_DATA_CLI DT_RACE_BATTLELOG_DATA_CLI;

class CRaceBattleLog:public CBaseObj
{
public:
	CRaceBattleLog();
	~CRaceBattleLog();
public:
	BOOL Init(DT_RACE_BATTLELOG_DATA* pstDT_RACE_BATTLELOG_DATA, CPlayer* poOwner);
	DT_RACE_BATTLELOG_DATA& GetDT_RACE_BATTLELOG_DATA() { return m_stDT_RACE_BATTLELOG_DATA; }
	VOID GetDT_RACE_BATTLELOG_DATA_CLI(DT_RACE_BATTLELOG_DATA_CLI& stDT_RACE_BATTLELOG_DATA_CLI);
protected:

private:
	DT_RACE_BATTLELOG_DATA	m_stDT_RACE_BATTLELOG_DATA;
};


#endif

