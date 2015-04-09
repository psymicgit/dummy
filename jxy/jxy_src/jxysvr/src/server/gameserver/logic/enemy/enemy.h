
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>

struct tagDT_ENEMY_DATA_CLI;
typedef tagDT_ENEMY_DATA_CLI DT_ENEMY_DATA_CLI;

class CPlayer;
class CEnemy:public CBaseObj
{
public:
	CEnemy();
	~CEnemy();
public:
	BOOL Init(DT_ENEMY_DATA* pstDT_ENEMY_DATA, CPlayer* poOwner);
	DT_ENEMY_DATA& GetDT_ENEMY_DATA() { return m_stDT_ENEMY_DATA; }
	BOOL GetDT_ENEMY_DATA_CLI(DT_ENEMY_DATA_CLI& stDT_ENEMY_DATA_CLI); 
	CPlayer* GetOwner() { return m_poOwner; }
protected:

private:
	DT_ENEMY_DATA	m_stDT_ENEMY_DATA;
};


#endif

