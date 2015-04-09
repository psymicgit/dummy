
#ifndef _OPENLIGHTPROPMGR_H_
#define _OPENLIGHTPROPMGR_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>

struct tagPKT_CLIGS_OPEN_SMITHY_ACK;
typedef tagPKT_CLIGS_OPEN_SMITHY_ACK PKT_CLIGS_OPEN_SMITHY_ACK;

struct tagDT_EQUIP_DATA_CLI;
typedef tagDT_EQUIP_DATA_CLI DT_EQUIP_DATA_CLI;

using namespace SGDP;

// 开光配置
struct SOpenLightProp
{
    UINT16  wKindID;            // 装备KindID
    UINT8   byIsCanOpenLight;   // 是否可开光
    UINT16  wNeedGoodsKindID;   // 开光需要的物品类型
    UINT16  wNeedGoodsCnt;      // 开光需要的物品数量
    UINT8   bySuccessRate;      // 开光成功率
    UINT32  dwFullRateGold;     // 补全概率需要的元宝
    UINT16  wNewKindID;         // 开光后的装备KindID
};


typedef map<UINT16, SOpenLightProp> COpenLightPropMap; // key为装备ID
typedef COpenLightPropMap::iterator COpenLightPropMapItr;

// 真装备
typedef set<UINT16, less<UINT16> > CRealEquipSet; 
typedef CRealEquipSet::iterator CRealEquipSetItr;


class COpenLightPropMgr : public CBasePopMgr
{
public:
	DECLARE_SINGLETON_PROPMGR(COpenLightPropMgr);

public:
	virtual EConfigModule GetConfigModuleType() { return ECM_OPENLIGHT; }

	BOOL Init();

	VOID UnInit();

public:	
	COpenLightPropMap& GetOpenLightPropMap() { return m_mapOpenLightPropMap; }

	void GetOpenLightFlagInfo(PKT_CLIGS_OPEN_SMITHY_ACK& stAck);    // 获取所有装备的可开光标识串
    void GetOpenLightFlagInfo_v410(PKT_CLIGS_OPEN_SMITHY_ACK_v410& stAck); // 获取所有装备的可开光标识串

    UINT8 GetOpenLightFlagOfEquip(DT_EQUIP_DATA_CLI &stEquipInfo); // 获取装备的可开光标识

    // 获取某个装备的开光配置信息     
	UINT16 GetOpenLightPropInfo(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, 
								DT_EQUIP_DATA_CLI& stAfterOpenLightInfo, UINT16& wOpenLightGoodsID,
								UINT16& wNeedOpenLightGoodsNum, UINT16& wOpenLightGoodsNum,
								UINT8& byOpenLightRate, UINT32& dwOpenLightGold,
								DT_BATTLE_ATTRIBUTE& stOldEquipInitBatlAttr, DT_BATTLE_ATTRIBUTE& stNewEquipInitBatlAttr,
								DT_BATTLE_ATTRIBUTE& stOldEquipCurrAttr);	

	//装备开光			
	UINT16 OpenLight(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byGoldFlag,
										UINT16& wNewEquipKindID, UINT16& wNewIdx, UINT8& byNewOpenLightFlag, 
										UINT16& wOpenLightGoodsID, UINT16& wOpenLightGoodsNum);		

public:
	COpenLightPropMgr();
	virtual ~COpenLightPropMgr();

protected:
	BOOL LoadOpenLightPropFromDB();
    
private:
	COpenLightPropMap m_mapOpenLightPropMap; // 开光配置
	CRealEquipSet m_setRealEquipSet; // 真装备SET集
	
};

#endif // 

