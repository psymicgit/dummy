
#ifndef _JINGJIE_H_
#define _JINGJIE_H_


#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>

using namespace std;

struct tagDT_JINGJIE_DATA_CLI;
typedef tagDT_JINGJIE_DATA_CLI DT_JINGJIE_DATA_CLI;

struct tagPKT_CLIGS_OPEN_JINGJIE_TAB_ACK;
typedef tagPKT_CLIGS_OPEN_JINGJIE_TAB_ACK PKT_CLIGS_OPEN_JINGJIE_TAB_ACK;

typedef map<UINT16, DT_SKILL_ACTION_DATA*> CSkillActionDataMap;
typedef CSkillActionDataMap::iterator CSkillActionDataMapItr;

class CPlayer;
class CSkill;
class CJingJie: public CBaseObj
{
public:
    CJingJie();
    ~CJingJie();
public:
    BOOL	Init(DT_JINGJIE_DATA& stJingJieData, CPlayer* poOwner);
    DT_JINGJIE_DATA& GetDT_JINGJIE_DATA()
    {
        return m_stDT_JINGJIE_DATA;
    }
    //////////////////////////////
    UINT16	Upgrade(UINT16 wLevel, DT_JINGJIE_DATA_CLI& stDT_JINGJIE_CLI);

    UINT16	OpenJingJieTab(PKT_CLIGS_OPEN_JINGJIE_TAB_ACK& stAck);

    //////////////////////////////
    const SAllAttrProp* GetAllIncAttr();
    BOOL	SetJingJieLevel(UINT16 wLevel, UINT16 wSubLevel);


    //////////////////////////////////////////////////////////////////////////
    UINT16	GetCoachDefaultSkillActionID();
    CSkill*	GetCoachSkill();
    UINT8	GetCoachCareerID();
    UINT16	GetJingjieQualityLevel();
    UINT16	GetJingJieLevel();
    VOID		OnJingjieQualityUpgrade();//境界进阶回调




protected:

    VOID	GetDT_JINGJIE_CLI(DT_JINGJIE_DATA_CLI& stDT_JINGJIE_CLI);
private:
    DT_JINGJIE_DATA		m_stDT_JINGJIE_DATA;
    CSkillActionDataMap m_mapSkillActionData;
};

DECLARE_CLASS_POOL_MGR(CJingJie)


#endif

