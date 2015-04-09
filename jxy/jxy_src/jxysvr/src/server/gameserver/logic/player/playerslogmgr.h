///<------------------------------------------------------------------------------
//< @file:   logic\player\playerslogmgr.h
//< @author: Kang_zhiquan
//< @date:   2014年5月21日 16:55:31
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playerslogmgr_h_
#define _playerslogmgr_h_

#include "sdsingleton.h"
#include "protocol/server/protocligs.h"
#include <map>
#include "sdtype.h"
#include "common/client/commondef.h"

using namespace std;

typedef multimap<UINT64, DT_BATTLE_LOG_MSG_DATA> CMultiMapBattleLog;
typedef CMultiMapBattleLog::iterator CMultiMapBattleLogItr;

class CPlayerLogMgr
{
	DECLARE_SINGLETON(CPlayerLogMgr)
public:
	CPlayerLogMgr();
	~CPlayerLogMgr();
	BOOL Init();
public:
	//添加战报
	VOID AddBattleLog(UINT32 dwPlayerID, UINT32 dwEnemyPlayerID, EBattleLogType eLogType, UINT8 bySmallLogType, const vector<UINT64>& vec64, const vector<string>& vecStr);
	//跨服战报
	//BOOL AddGvGBattleLog(UINT8 bySmallLogType, const vector<UINT64>& vec64, const vector<string>& vecStr, string& strMsg);
public:
	//获取未读信息个数
	UINT32 GetUnReadLogNum(UINT32 dwPlayerID, UINT8 byLogType);

	VOID GetUnReadLogNum(UINT32 dwPlayerID, PKT_CLIGS_UNREAD_BATTLE_NTF& stNtf);
	//获取未读信息内容
	BOOL GetUnReadLogMsg(UINT32 dwPlayerID, UINT8 byLogType, DT_BATTLE_LOG_MSG_LST_CLI& stData);
	//保存进数据库
	VOID Run();
	//要求客户端版本
	UINT32 GetCliVersion(){ return 312; }
private:
	BOOL LoadFromDB();
private:
	CMultiMapBattleLog m_mapDBLog;//数据库中的未读战报
	CMultiMapBattleLog m_mapWillSaveLog;//要保存的战报
	UINT64 m_qwSaveTime;
};

#endif //_playerslogmgr_h_