#ifndef _MARK_H_
#define _MARK_H_
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
class CPlayer;
class CPlayerMark:public CBaseObj
{
public:
	CPlayerMark(){ memset(&m_stPlayerMarkData, 0, sizeof(DT_PLAYER_MARK_DATA));}
	~CPlayerMark(){}
public:
	// 初始化相关对象
	DT_PLAYER_MARK_DATA& GetDT_PLAYER_MARK_DATA() { return m_stPlayerMarkData; }
	BOOL Init(DT_PLAYER_MARK_DATA& stPlayerMarkData, CPlayer* poPlayer);
private:
	DT_PLAYER_MARK_DATA m_stPlayerMarkData;
};
#endif