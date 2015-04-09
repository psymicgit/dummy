
#ifndef _BEGBLUEPROPMGR_H_
#define _BEGBLUEPROPMGR_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;

const UINT32 CLIENT_VER = 304;
// 求仙露配置
struct SBegBlueProp
{
    UINT8   byCurrSmileCnt; // 当前笑脸数量
    
    UINT8   by0SmileOdds;   // 0个笑脸的概率
    UINT8   by1SmileOdds;   // 1个笑脸的概率
    UINT8   by2SmileOdds;   // 2个笑脸的概率
    UINT8   by3SmileOdds;   // 3个笑脸的概率
    UINT8   by4SmileOdds;   // 4个笑脸的概率
    UINT8   by5SmileOdds;   // 5个笑脸的概率
    UINT8   by6SmileOdds;   // 6个笑脸的概率
    
    UINT8   byX1Odds;       // 倍率为1的概率
    UINT8   byX2Odds;       // 倍率为2的概率
    UINT8   byX3Odds;       // 倍率为3的概率
    UINT8   byX4Odds;       // 倍率为4的概率
    UINT8   byX5Odds;       // 倍率为5的概率
    UINT8   byX6Odds;       // 倍率为6的概率

	CRandomVec vecSmileRandom; // 不同个数笑脸的概率，按笑脸个数顺序插入
	CRandomVec vecXnMultipleRandom; // 不同倍率的概率，按倍率大小顺序插入    

    UINT32  dwBlueUnit;     // 单倍仙露值
    UINT32  dwStoryUnit;    // 单倍阅历值
    TCHAR   aszLuckPrompt[BEGBLUE_LUCK_PROMPT];
};

struct SBegBlueCourageProp
{
    UINT32 dwSmileNum;     //笑脸个数
    UINT32 dwBlue;         //可领取的仙露
};

typedef map<UINT16, SBegBlueProp> CBegBluePropMap; // key为笑脸的个数
typedef CBegBluePropMap::iterator CBegBluePropMapItr;

typedef map<UINT32, SBegBlueCourageProp> CBegBlueCouragePropMap; // key为累计的仙露
typedef CBegBlueCouragePropMap::iterator CBegBlueCouragePropMapItr;
typedef CBegBlueCouragePropMap::reverse_iterator CBegBlueReverseCouragePropItr;

class CBegBluePropMgr : public CBasePopMgr
{
public:
	DECLARE_SINGLETON_PROPMGR(CBegBluePropMgr);

public:
	virtual EConfigModule GetConfigModuleType() { return ECM_BEGBLUE; }

	BOOL Init();

	VOID UnInit();

public:	
	//CBegBluePropMap& GetBegBluePropMap() { return m_mapBegBluePropMap; }
	BOOL GetBegBlueRes(UINT16 wLastSmileCnt, UINT16& wNewSmileCnt, UINT16& wXnMultiple, 
		               UINT32& dwBlueUnit, UINT32& dwStoryUnit, TCHAR aszLuckPrompt[BEGBLUE_LUCK_PROMPT], UINT32 dwCliver);
    SBegBlueCourageProp* GetBegBlueCourageProp(UINT32 dwSmileNum);

    CBegBlueCouragePropMap GetEncourageProp() { return m_mapBegBlueCourageProp; }
    //获取奖励提示
    VOID GetEncouragePrompt(UINT32 dwSmileNum ,TCHAR aszEncouragePrompt[BEGBLUE_ENCOURAGE_PROMPT]);
public:
	CBegBluePropMgr();
	virtual ~CBegBluePropMgr();

protected:
	BOOL LoadBegBluePropFromDB();
    BOOL LoadBegBlueCouragePropFromDB();
    
private:
	CBegBluePropMap m_mapBegBluePropMap; // 求仙露配置
    CBegBlueCouragePropMap m_mapBegBlueCourageProp;
};

#endif // 

