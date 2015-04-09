
#ifndef _LOGICFUNC_H_
#define _LOGICFUNC_H_

#include <sdtype.h>
#include "burstprop.h"
#include "basedefine.h"
#include <common/client/commondef.h>


struct tagDT_ATTR_INC_DATA;
typedef tagDT_ATTR_INC_DATA DT_ATTR_INC_DATA ;

//奖励最终属性值
struct SEncouragePropValue
{
	EBurstKind	eBurstKind; //爆出物品类型
	//
	UINT16 wSubKind;//eBurstKind为EBKIND_ITEM道具是道具ID，为EBKIND_ATTRIBUTE时是属性EBattleAttributeKind
	
	//数量（道具时）,值(体力/金钱/经验等/属性时为属性值)
	union 
	{
		UINT16 wNum;
		UINT32 dwValue;//金钱体力经验应该为正的
		INT32 nValue; 
	};
	SEncouragePropValue()
	{
		eBurstKind = EBKIND_INVALID;
		wSubKind = 0;
		wNum = 0;
	}
};



//奖励类型
enum EEncourageType
{
	EET_BATTLE = 0, //战斗
	EET_PASS = 1, //通关
	EET_TASK = 2, //任务
	EET_RACE = 3, //竞技
};

class CPlayer;

//公用的逻辑函数方法
class CLogicFunc
{
public:
	static BOOL Encourage(CPlayer* poOwner, SBurst* pstBurst, BOOL bFirstFlag = FALSE);//奖励
	static BOOL Encourage(CPlayer* poOwner, SEncourageProp& stEncourageProp, BOOL bFirstFlag = FALSE);//奖励

	static BOOL FirmPropValue(SEncouragePropValue& stEncouragePropValue, SBurst* pstBurst, BOOL bFirstFlag);//确定属性最终值
	static BOOL FixValue(SEncouragePropValue& stEncouragePropValue, EBurstKind eBurstKind, SProp* pstProp);//固定值赋值
	static BOOL RandomValue(SEncouragePropValue& stEncouragePropValue, EBurstKind eBurstKind, vector<SProp*>& vecProp);//随机值赋值，概率未命中，则FALSE

	//static BOOL EncourageItem(CPlayer* poOwner, UINT16 wItemKindID, UINT16 wItemNum);//奖励物品,背包满返回FALSE，同时放入临时背包中
	
	static UINT32 GetInstanceIdxKey(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
	static UINT64 GetInstanceIdxKey(UINT32 dwPlayerID,UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
	static VOID	  GetInctanceID(UINT32 dwID, DT_INSTANCE_ID& stInstanceID);

	static UINT64 GetObjKey(UINT32 dwPlayerID, UINT16 wKindID);
	static UINT64 GetObjKey(UINT32 dwPlayerID, UINT16 wID1, UINT16 wID2);
	static UINT64 GetItemKey(UINT32 dwPlayerID, UINT16 wKindID, UINT16 wIdx=1); //wIdx为同类型道具的序号
	//static UINT64 Get321616Key(UINT32 dwPlayerID,UINT16 wValue1,UINT16 wValue2); 
	static UINT16 Get88Key(UINT8 bywValue1,UINT8 byValue2);
	static UINT32 Get888Key(UINT8 bywValue1,UINT8 byValue2, UINT8 byValue3);
	static UINT32 Get8888Key(UINT8 bywValue1,UINT8 byValue2, UINT8 bywValue3, UINT8 byValue4);
	static UINT32 Get1688Key(UINT16 wValue1,UINT8 bywValue2,UINT8 byValue3);
	static UINT32 Get1616Key(UINT16 wValue1,UINT16 wValue2);
	static UINT64 Get161616Key(UINT16 wValue1,UINT16 wValue2,UINT16 wValue3);
	static UINT64 Get3232Key(UINT32 dwValue1,UINT32 dwValue2);
    static UINT64 Get3216Key(UINT32 dwGroupID, UINT16 wLevel);
	static VOID AddAttr(SAllAttrProp& stAttr, UINT8 byAttrKind, UINT32 dwAttrValue);
	static VOID AddAttr(const SAllAttrProp* pstSrc, DT_BATTLE_BASE_ATTRIBUTE& stDstBattleAttr, DT_HIDE_BATTLE_ATTRIBUTE& stDstHideBattleAttr, DT_EXT_BATTLE_ATTRIBUTE& stDstExtBattleAttr);
	static VOID ParseAttr(const SAllAttrProp& stAttr, const UINT8 byMaxDstNum, UINT8& byIncAttrNum, DT_ATTR_INC_DATA astIncAttrInfo[]);
};


#endif

