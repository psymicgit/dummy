
#ifndef _ITEMCOMPOSEPROPMGR_H
#define _ITEMCOMPOSEPROPMGR_H

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <logic/base/basepropmgr.h>
#include <logic/item/equip/equippropmgr.h>

using namespace std;
using namespace SGDP;

typedef struct tagItemComposeProp
{
    UINT16			wKindID;				///< 道具类型
    CItemRateMap		mapComposeItem;		///< 组合的道具(宝箱的为打开宝箱的道具)
} SItemComposeProp;

struct SComposeEquipProp
{
    UINT16	wEquipLevel;		//装备等级
    UINT16	wClipItemID;		//碎片物品ID
    UINT16	wNeedClipNum;		//合成所需碎片数
    UINT32	dwCommonComposeGold;//普通合成所需元宝
    UINT32	dwPurpleComposeGold;//紫色合成所需元宝
    UINT32	dwRedComposeGold;	//红色合成所需元宝
    UINT32	dwOrangeComposeGold;//橙色合成所需元宝
    UINT16	wBlueRate;			//蓝色概率
    UINT16	wPurpleRate;		//紫色概率
    UINT16	wRedRate;			//红色概率
    UINT16	wOrageRate;			//橙色概率
    UINT16	wEquipComposeCD;	//装备合成CD
};

typedef map<UINT16, SComposeEquipProp> CComposeEquipPropMap; //key为wEquipLevel
typedef CComposeEquipPropMap::iterator CComposeEquipPropMapItr;

typedef vector<SEquipProp> CEquipPropVec;
typedef CEquipPropVec::iterator CEquipPropVecItr;
typedef map<UINT8, CEquipPropVec> CColor2EquipPropVecMap;//key为装备颜色
typedef CColor2EquipPropVecMap::iterator CColor2EquipPropVecMapItr;
typedef map<UINT16, CColor2EquipPropVecMap> CLevel2Color2EquipPropVecMap; //key为装备等级
typedef CLevel2Color2EquipPropVecMap::iterator CLevel2Color2EquipPropVecMapItr;


class CPlayer;
class CItemComposePropMgr : public CBasePopMgr
{
public:
    typedef HashMap<UINT16, SItemComposeProp*> CItemComposePropMap;		//key为wKindID
    typedef CItemComposePropMap::iterator CItemComposePropMapItr;

    DECLARE_SINGLETON_PROPMGR(CItemComposePropMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_ITEM_COMPOSE ;
    }

    /**
     * @brief 初始化属性管理器
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL Init();

    /**
     * @brief 结束属性管理器
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    VOID UnInit();

    /**
    * @brief 查找属性
    * @return 属性
    */
    SItemComposeProp* GetProp(UINT16 wKindID);
    BOOL GetDT_NEED_ITEM_DATA(CPlayer* poPlayer, DT_NEED_ITEM_DATA &stDT_NEED_ITEM_DATA, UINT16 wItemKindID, UINT16 wSubKindID);
    UINT16 GetGemMakeOpenNeedTowerNum(UINT16 wGemLevel);
    VOID GetInitDT_GEM_MAKER_DATA(DT_GEM_MAKER_DATA	astDT_GEM_MAKER_DATA[], UINT8 byMaxGemLevel)
    {
        for (UINT8 byNum = 0 ; byNum < m_byMaxGemLevel && byNum < byMaxGemLevel; byNum++)
        {
            memcpy(&astDT_GEM_MAKER_DATA[byNum], &m_astDT_GEM_MAKER_DATA[byNum], sizeof(DT_GEM_MAKER_DATA));
        }
    }
    CComposeEquipPropMap& GetComposeEquipProp()
    {
        return m_mapComposeEquipProp;
    }
    CColor2EquipPropVecMap* GetEquipLevelProp(UINT16 wEquipLevel)
    {
        return &m_mapLevel2Color2EquipPropVec[wEquipLevel];
    }
    UINT32	GetColorComposeNeedGold(UINT16 wEquipLevel, UINT8 byCurLowColor);
    SComposeEquipProp* GetComposeEquipProp(UINT16 wEquipLevel);

	UINT8	GetMaxGemLevel(){return m_byMaxGemLevel;}
public:
    CItemComposePropMgr();
    virtual ~CItemComposePropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    //BOOL LoadFromDB();

    BOOL LoadFromDBEx();

    BOOL LoadGemMakerOpenPropFromDB();

    BOOL LoadComposeEquipPropFromDB();

    //初始化宝石合成信息
    BOOL InitGemMakerInfo();

    //初始化礼包属性的组合道具
    VOID InitBoxComposeItem();

    //初始化装备合成道具
    VOID InitEquipCompose();

private:
    CItemComposePropMap			m_mapItemComposeProp;
    C1616Map						m_mapGemMakerOpenProp;

    UINT8						m_byMaxGemLevel;
    DT_GEM_MAKER_DATA				m_astDT_GEM_MAKER_DATA[MAX_GEM_LEVEL_EX];
    CComposeEquipPropMap			m_mapComposeEquipProp;
    CLevel2Color2EquipPropVecMap	m_mapLevel2Color2EquipPropVec;
};

#endif // #ifndef _EQUIPPROPMGR_H_
