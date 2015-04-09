
#ifndef _FORMATION_H_
#define _FORMATION_H_

#include <protocol/server/protocommondata.h>
#include "protocol/server/protocligs.h"
#include <logic/base/baseobj.h>
#include <common/server/utility.h>
#include "logic/base/basedefine.h"

using namespace std ;

typedef map<UINT8, DT_NEW_FORMATION_INFO> CMapNewFormationInfo;
typedef CMapNewFormationInfo::iterator CMapNewFormationInfoItr;


class CPlayer;
class CFormation:public CBaseObj
{
public:
	CFormation();
	~CFormation();
public:
	//初始化
	BOOL Init(DT_NEW_FORMATION_DATA& stDT_FORMATION_DATA, CPlayer* poOwner);
	DT_NEW_FORMATION_DATA& GetDT_FORMATION_DATA();

	VOID OnDisband(UINT16 wHeroID);//?2é￠??μ÷
	//修改阵型信息
	VOID ResetFormation(DT_FORMATION_IDX_DATA& stInfo, UINT8 byFormationID);
	UINT16	OpenEmbattleTab(UINT16& wHeroNum, DT_HERO_EMBATLE_DATA astHeroInfoLst[MAX_HERO_NUM], DT_FORMATION_DATA& stFormationInfo);//打开布阵
	UINT16	OpenEmbattleTab2(PKT_CLIGS_OPEN_EMBATTLE_ACK2& stAck);//打开新布阵
	UINT16	Embattle(DT_FORMATION_DATA& stFormationInfo);
	UINT16  NewEmbattle(DT_FORMATION_DATA& stFormationInfo);
	UINT16  OldEmbattle(DT_FORMATION_DATA& stFormationInfo);
	BOOL		Embattle();//布阵
	
	BOOL GetDT_NEW_FORMATION_LST_CLIByAll(DT_NEW_FORMATION_LST_CLI& stCli);//显示
	BOOL GetDT_NEW_FORMATION_LST_CLIByUnlock(DT_NEW_FORMATION_LST_CLI& stCli);//显示但未解锁
	UINT16 OnOpenFormationScience(PKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK& stAck);
	UINT16 OnUpgradeFormation(PKT_CLIGS_UPGRADE_FORMATION_ACK& stAck, UINT8 byFormationID, DT_FORMATION_DATA& stFormation );
	UINT16 OnUpgradeFormationScience(PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK& stAck, UINT8 byFormationID);
	VOID GetFormationLevel(DT_FORMATION_DATA_LEVEL& stLevel, UINT8 byFormationID, UINT8 byFormationLevel);
	VOID CheckFormation();
	CMapNewFormationInfo& GetUnLockFormationInfoMap(){ return m_mapUnLockFormationInfo; }

	VOID GetSimpleFormation(DT_GVG_FORMATION_HERO_LST_INFO& stFront, DT_GVG_FORMATION_HERO_LST_INFO& stBack);
protected:
	BOOL UpdateFormation();
	VOID CreateFormation(DT_NEW_FORMATION_INFO& stInfo, UINT8 byFormationID);
	VOID AddUnLockFormation( DT_NEW_FORMATION_INFO& stInfo, UINT8 bySelectFormationID);
	VOID AddLockFormation(DT_NEW_FORMATION_INFO& stInfo);
	BOOL OldToNewFormation();//旧阵型转新阵型
	BOOL NewToOldFormation();//新阵型转旧阵型
	BOOL IsUnlock(UINT8 byFormationID, UINT8 byIdx);
	VOID UnLockPos(DT_NEW_FORMATION_INFO& stInfo);
	BOOL OldEmbattleOnFormation(DT_FORMATION_DATA& stFormationInfo);

	
private:
	DT_NEW_FORMATION_DATA	m_stDT_FORMATION_DATA;
	UINT8 m_bySelectFormationIdx;	//新布阵选择序号

	CMapNewFormationInfo m_mapLockFormationInfo;	//未解锁
	CMapNewFormationInfo m_mapUnLockFormationInfo;	//已解锁

	C88Map m_mapUnLockPos;	//key FormationID, value CMapNewFormationInfo::Key
	C88Map m_mapLockPos;
};
DECLARE_CLASS_POOL_MGR(CFormation)

#endif //#ifndef 

