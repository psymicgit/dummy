
#ifndef _INSTANCEMGR_H_
#define _INSTANCEMGR_H_

#include "instance.h"
#include "instancerecord.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;


typedef HashMap<UINT32, CInstance*> CID2InstanceMap;
typedef CID2InstanceMap::iterator	CID2InstanceMapItr;

typedef HashMap<UINT64, CInstanceRecord*> CID2InstanceRecordMap;
typedef CID2InstanceRecordMap::iterator	CID2InstanceRecordMapItr;

/**
* @brief 副本管理类
*/

class CInstanceMgr
{	
public:
    DECLARE_SINGLETON(CInstanceMgr)

    BOOL Init();


    VOID UnInit();

	CInstanceRecord* CreatInstanceRecord(UINT32 dwPlayerID, UINT16 wSceneID, UINT8 byTownIdx, UINT8 byInstanceIdx);
	VOID			RemoveInsanceRecord(UINT32 dwPlayerID, UINT16 wSceneID, UINT8 byTownIdx, UINT8 byInstanceIdx);
	UINT16			InstanceBattle(CPlayer* poMyself, CMonsterGroup* poEnemy,  UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, 
		UINT8 byBattleIdx, DT_BATTLE_DATA5& stDT_BATTLE_DATA, UINT8& byPassFlag, DT_UNLOCK_DATA& stUnlockInfo,
		DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,
		DT_PASS_ENCOURAGE_DATA& stPassEncourage,DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage, 
		DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bGuide = FALSE);//副本战斗

    UINT16			InstanceBattle2(CPlayer* poMyself, CMonsterGroup* poEnemy,  UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, 
        UINT8 byBattleIdx, DT_BATTLE_DATA3& stDT_BATTLE_DATA, UINT8& byPassFlag, DT_UNLOCK_DATA& stUnlockInfo,
        DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,
        DT_PASS_ENCOURAGE_DATA& stPassEncourage,DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage, 
        DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bGuide = FALSE);//副本战斗
	UINT16			InstanceBattle3(CPlayer* poMyself, CMonsterGroup* poEnemy,  UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, 
		UINT8 byBattleIdx, DT_BATTLE_DATA4& stDT_BATTLE_DATA, UINT8& byPassFlag, DT_UNLOCK_DATA& stUnlockInfo,
		DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,
		DT_PASS_ENCOURAGE_DATA& stPassEncourage,DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage, 
		DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bGuide = FALSE);//副本战斗
public:
	UINT16 OpenNeiDanInstance(CPlayer* poPlayer, PKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK& stAck);
	UINT16 NeiDanBattle(CPlayer* poPlayer, PKT_CLIGS_NEIDAN_BATTLE_REQ& stReq, PKT_CLIGS_NEIDAN_BATTLE_ACK& stAck);
	UINT16 SkipNeiDanBattle(CPlayer* poPlayer, PKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ& stReq, PKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK& stAck);
	UINT16 RecvNeiDanAward(CPlayer* poPlayer, PKT_CLIGS_RECV_NEIDAN_AWARD_ACK& stAck);
protected:
    CInstanceMgr();
    ~CInstanceMgr();

protected:
	//副本记录
	VOID PassRecord(CPlayer* poMyself, CMonsterGroup* poEnemy, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIdx, UINT8 byScore, BOOL bInstancePass, 
		DT_UNLOCK_DATA& stUnlockInfo, DT_BATTLE_ENCOURAGE_DATA& stBattleEncourage, DT_DATA_AFTER_BATTLE_ENCOURAGE& stAfterBattleEncourage,DT_PASS_ENCOURAGE_DATA& stPassEncourage, 
		DT_DATA_AFTER_PASS_ENCOURAGE& stAfterPassEncourage, DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox);

	VOID NeiDanBattle(CPlayer* poPlayer, DT_INSTANCE_ID& stInstanceID, DT_PASS_ENCOURAGE_BOX_DATA& stEncourageBoxData, DT_BATTLE_DATA5* pstBattleData = NULL, BOOL bIsSkill = FALSE);
	VOID ChangeMonsterAttr(CMonsterGroup* poEnemy);
	VOID RecoverMonsterAttr(CMonsterGroup* poEnemy);

private:
	CID2InstanceRecordMap					m_mapID2InstanceRecord;		
	CSDObjectPool<CInstanceRecord, CSDMutex> m_oInstanceRecordShmemObjMgr;

	map<UINT8, DT_BATTLE_BASE_ATTRIBUTE>			m_mapOldMonsterAttr;
};




#endif //#ifndef _ROLEMGR_H_

