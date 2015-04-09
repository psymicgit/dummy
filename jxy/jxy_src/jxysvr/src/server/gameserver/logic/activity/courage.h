#ifndef _COURAGE_H_
#define _COURAGE_H_



#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>
#include <list>


#include "activity.h"

using namespace SGDP;
using namespace std;
class CCourageGroup;
// 勇气试炼
typedef list<CCourageGroup*> CListLevelGroup; //key为level
typedef CListLevelGroup::iterator CListLevelGroupItr;

typedef vector<UINT32>CGroupIDVec;
typedef CGroupIDVec::iterator CGroupIDVecIter;


typedef map<UINT32,UINT16> CMapGroupIDNum;
typedef CMapGroupIDNum::iterator CMapGroupIDNumDItr;

class CPlayer;

class CCourage: public CBaseObj
{
public:
    CCourage();
    ~CCourage();

public:
    
	//初始化相关对象
    BOOL Init(DT_COURAGE_DATA &stCourageData, CPlayer* poOwner);

    DT_COURAGE_DATA& GetDT_COURAGE_DATA()
    {
        return m_stCourageData;
    };
    VOID GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA);
    BOOL GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt); 
    VOID	RatePassBoxEnc(UINT16 wLevel, DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox);//计算通关宝箱奖励
	VOID SetSelectType(UINT8 bySelectType);
	UINT8 GetSelectType();

	VOID GetCourageGroup(DT_COURAGE_GROUP_DATA_CLI_LST& stDataLst, BOOL bSelect = FALSE);
    CListLevelGroup	m_listLevelGroup;
    CMapGroupIDNum  m_mapGroupIDNum;
    CMapGroupIDNum  m_mapBackGroupID;
    CGroupIDVec		m_vecGroupID;
private:  
    DT_COURAGE_DATA			m_stCourageData;//
};

DECLARE_CLASS_POOL_MGR(CCourage);
#endif
