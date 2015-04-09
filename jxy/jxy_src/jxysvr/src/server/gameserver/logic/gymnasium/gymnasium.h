#ifndef _GYMNASIUM_H_
#define _GYMNASIUM_H_
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <map>
class CFaction;
using std::map;

typedef map<UINT8, DT_GYMNASIUM_SKILL_INFO> CMapMySkill;
typedef CMapMySkill::iterator CMapMySkillItr;
typedef CMapMySkill::const_iterator CMapMySkillConstItr;

class CPlayer ;
class CGymnasium:public CBaseObj
{
public:
	CGymnasium()
	{ 
		memset(&m_stGymnasiumSkillData, 0, sizeof(DT_GYMNASIUM_DATA));
	}
	~CGymnasium(){}

	UINT16 OpenGymnasium( DT_GYMNASIUM_CLI_INFO& stGymnasiumInfo, UINT8& bySkillNum,DT_SKILL_CLI_INFO* astSkillInfo, UINT32 adwSkillSumDoorsTribute[MAX_GYMNASIUM_SKILL_NUM] );
	UINT16 LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK& stAck);
	UINT16 LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2& stAck, UINT8 byCurLevel);
	UINT16 OpenGymnasiumUpgrade( DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo );
	UINT16 GymnasiumUpgrade( PKT_CLIGS_GYMNASIUM_UPGRADE_ACK& stAck );
	UINT32 GetGymnasiumAttrLevel(UINT8 byKindID);
	VOID GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt);
	UINT16 OpenFactionSkill(PKT_CLIGS_OPEN_FACTION_SKILL_ACK& stAck);
protected:
	VOID GetGymnasiumInfo(DT_GYMNASIUM_CLI_INFO& stGymnasiumInfo);
	VOID GetSkillInfo(UINT8& bySkillNum,DT_SKILL_CLI_INFO* astSkillInfo, UINT32 adwSkillSumDoorsTribute[MAX_GYMNASIUM_SKILL_NUM]);
	VOID GetSingleSkillInfo(DT_SKILL_CLI_INFO& stSkillInfo, UINT8 bySkillID);
	VOID GetSingleSkillInfoByScience(DT_SKILL_CLI_INFO& stSkillInfo, UINT8 bySkillID);
	VOID GetInfoBySkillProp(DT_SKILL_CLI_INFO& stSkillInfo);
	UINT16 GetAfterLearnSkillInfo(DT_SKILL_CLI_INFO& stSkillInfo, UINT32& dwLeftDoorsTribute);
	UINT16 GetGymnasiumUpgradeInfo(DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo, UINT32 dwGymnasiumLevel);
	CFaction* GetFaction();
	UINT32 GetFactionID();
	BOOL IsOpen();
public:
	// 初始化相关对象
	BOOL Init(DT_GYMNASIUM_DATA &stDT_GYMNASIUM_SKILL_DATA, CPlayer* poOwner);
	DT_GYMNASIUM_DATA& GetDT_GYMNASIUM_SKILL_DATA();
	CMapMySkill& GetMySkillMap(){ return m_mapMySkill; }
private:
	DT_GYMNASIUM_DATA	m_stGymnasiumSkillData;
	CMapMySkill   m_mapMySkill;
};
#endif