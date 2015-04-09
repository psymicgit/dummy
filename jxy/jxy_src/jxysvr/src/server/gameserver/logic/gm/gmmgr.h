
#ifndef _FORMATIONMGR_H_
#define _FORMATIONMGR_H_

#include <sdsingleton.h>

using namespace SGDP;


class CGmMgr
{
public:
    DECLARE_SINGLETON(CGmMgr)

    BOOL Init();

    VOID UnInit();

public:
	VOID AddCoin(UINT32 dwPlayerID, UINT32 dwCoin);
	VOID AddGold(UINT32 dwPlayerID, UINT32 dwGold);
	VOID AddStory(UINT32 dwPlayerID, UINT32 dwStory);
	VOID AddScience(UINT32 dwPlayerID, UINT32 dwScience);
	VOID AddPhystrength(UINT32 dwPlayerID, UINT16 wPhystrength);
	VOID SetTalent(UINT32 dwPlayerID, UINT16 wTalent);
	VOID SetLevel(UINT32 dwPlayerID, UINT16 wLevel);
	VOID AddRobot(UINT32 dwStartPlayerID, UINT8 byNum);
	VOID AddBuild(UINT32 dwPlayerID, UINT8 byBuildKindID);
	VOID AddSkill(UINT32 dwPlayerID, UINT16 wSkillID, UINT8 bySlotIdx);
	VOID AddItem(UINT32 dwPlayerID, UINT16 wItemID, UINT16 wNum);
	VOID PassAllCommonInstance(UINT32 dwPlayerID);
	VOID PassGuide(UINT32 dwPlayerID, UINT8 byGuideIdx);
	VOID PassAllGuide(UINT32 dwPlayerID);
	VOID AddHero(UINT32 dwPlayerID, UINT16 wHeroID);
	VOID ClrEliteChallenge( UINT32 dwPlayerID );
	VOID ClrClimbTowerChallenge( UINT32 dwPlayerID );
	VOID ResetClimbTowerChallenge( UINT32 dwPlayerID );
	
	

	VOID AddAllCoin(UINT32 dwCoin);
	VOID AddAllGold(UINT32 dwGold);
	VOID AddAllStory(UINT32 dwStory);
	VOID AddAllScience(UINT32 dwScience);
	VOID AddAllPhystrength(UINT16 wPhystrength);
	VOID Notice( UINT32 dwPlayerID, char *szData);
	VOID NoticeAll(char *szData);
	VOID ReloadCfg(UINT8 byModuleKind);
protected:

protected:
    CGmMgr();
    ~CGmMgr();


private:
 
};




#endif //#ifndef _ROLEMGR_H_

