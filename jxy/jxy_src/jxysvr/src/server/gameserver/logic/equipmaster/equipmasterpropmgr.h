#ifndef _equipmasterpropmgr_h_
#define _equipmasterpropmgr_h_

/**
 * @brief 装备精通配置数据
 */

#include <vector>

#include <utility.h>
#include <sdtype.h>
#include <sddebug.h>
#include <common/client/commondef.h>
#include <sdhashmap.h>

#include "equipmaster.h"
#include "logic/base/basepropmgr.h"

namespace SGDP
{
class ISDDBSession;
}

// 装备精通的突破数据
struct SEquipMasterBreakProp
{
    EEquipPosType    eEquipKind;        // 装备类别（头盔、战袍、武器、战靴）
    EEquipMasterRank eEquipRank;        // 装备精通阶级（未激活0、初1、中2、高3、极4）
    UINT32           wBreakMedal;       // 突破到下一级所需勇气勋章
    UINT16           wBreakLevel;       // 突破所需精通等级
    UINT16           wBreakPlayerLevel; // 突破所需玩家等级
};

// 装备精通的经验及加成数据
struct SEquipMasterExpProp
{
    EEquipPosType    eEquipKind;  // 装备类别（头盔、战袍、武器、战靴）
    EEquipMasterRank eEquipRank;  // 装备精通阶级（未激活0、初1、中2、高3、极4）
    UINT16           wEquipLevel; // 精通级别
    UINT32           qwExp;       // 本级经验条长度
    UINT32           qwPromote;   // 属性加成，比如20则表示属性加成+20%
};

// 培养精通之书经验书生成的概率数据
struct SEquipMasterRateProp
{
    EEquipPosType eEquipKind;      // 装备类别（头盔、战袍、武器、战靴）
    BYTE          byBigExpBookCnt; // 大经验书数量（0、1、2、3）
    BYTE          byRate;          // 对应概率，如数值10表示10%
    UINT16        qwExp;           // 加多少经验
};

// 培养精通之书大经验书额外经验数据
struct SEquipMasterExtraExpProp
{
    BYTE byBigExpBookCnt; // 大经验书数量(1-12)
    UINT16 wExtraExp;     // 对应的额外经验值
};

// 出千耗费元宝数据
struct SEquipMasterCheatCostProp
{
    BYTE byCheatTimes;    // 出千次数，范围1-12
    UINT16 wGoldCost;     // 元宝耗费数量
};

// 激活及突破数据表
typedef std::map<UINT64, SEquipMasterBreakProp> CMasterBreakPropMap; // key = 精通类别 + 精通阶级

// 经验及加成数据表
typedef std::map<UINT64, SEquipMasterExpProp> CMasterExpPropMap; // key = 类别 + 阶级 + 级别

// 培养精通之书时生成精通经验书的概率表
typedef std::map<UINT64, SEquipMasterRateProp> CMasterRatePropMap; // key = 类别 + 大经验书数量

// 额外经验数据表
typedef std::vector<SEquipMasterExtraExpProp> CMasterExtraExpPropVec;

// 出千耗费元宝数据
typedef std::vector<SEquipMasterCheatCostProp> CMasterCheatCostPropVec;


// 精通经验书类别概率表
typedef std::vector<BYTE> ExpBookRateVec;

// 生成大精通经验书的概率表
// 表中内容如下：
// [
//     头盔{0本大经验书概率， 1本概率， 2本概率， 3本概率}
//     战袍{0本大经验书概率， 1本概率， 2本概率， 3本概率}
//     武器{0本大经验书概率， 1本概率， 2本概率， 3本概率}
//     战靴{0本大经验书概率， 1本概率， 2本概率， 3本概率}
// ]
typedef std::vector<CRandomVec> CRollBigExpBookVec;

/**
 * @brief 装备精通配置数据
 */
class CEquipMasterPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CEquipMasterPropMgr);

    CEquipMasterPropMgr();

public:
    // @impl
    virtual EConfigModule GetConfigModuleType()
    {
        return ECM_EQUIP_MASTER;
    }

    BOOL Init();

    VOID UnInit();

public:
    // 获取升级所需经验
    UINT32 GetMaxExp(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank, UINT16 wEquipLevel);

    // 获取阶级突破所需勋章（当阶级为0时相当于突破）
    UINT32 GetBreakMedal(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank);

    // 获取阶级突破所需等级
    UINT16 GetBreakLevel(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank);

    // 获取阶级突破所需的玩家等级
    UINT16 GetBreakPlayerLevel(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank);

    // 获取属性加成百分比
    UINT16 GetAttrPromotion(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank, UINT16 wEquipLevel);

    // 获取下一级别的属性加成百分比
    UINT16 GetNextPromotion(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank, UINT16 wEquipLevel);

    // 根据当前等级获取下一级
    BOOL GetNextLevel(/* IN */EEquipPosType eEquipKind, /* IN OUT */EEquipMasterRank &eEquipRank, /* IN OUT */UINT16 &wMasterLevel);

    // 随机出大精通经验书的数量
    BYTE RollBigExpBookCnt(EEquipPosType eEquipKind);

    // 根据大经验书数量获取经验，经验值 = 大经验书经验 + 剩余小经验书经验
    UINT32 GetBookExp(EEquipPosType eEquipKind, BYTE byBigExpBookCnt);

    // 根据出千次数获取所需元宝
    UINT16 GetCheatGoldCost(BYTE byCheatTimes);

    // 每次培养精通之书可以免费出千几次
    BYTE   GetMaxFreeCheatCnt(){return m_byMaxFreeCheatCnt;}
    
    // 每本小经验书加多少经验
    UINT32 GetSmallBookExp(){return m_dwSmallBookExp;}

    // 每本大经验书加多少经验
    UINT32 GetBigBookExp(){return m_dwBigBookExp;}

    // 获取培养精通之书的勇气勋章消耗
    UINT32 GetMasterCultivateMedalCost(){return m_dwMasterCultivateMedalCost;}

    // 装备精通出千功能多少级开放
    UINT32 GetMasterCheatPlayerLevel(){return m_dwMasterCheatPlayerLevel;}

    // 某精通类别的大经验书满后所获经验乘以多少倍
    BYTE   GetExpXnum(){return m_byExpXnum;}

private:
    // 载入装备精通所有配置数据
    BOOL LoadFromDB();

    // 检测配置数据是否出现了异常：正常TRUE/异常FALSE
    BOOL CheckProp();

    // 载入突破数据
    BOOL LoadBreakPropFromDB(SGDP::ISDDBSession&);

    // 载入经验数据
    BOOL LoadExpPropFromDB(SGDP::ISDDBSession&);

    // 载入概率数据
    BOOL LoadRatePropFromDB(SGDP::ISDDBSession&);

    // 载入培养经验书出千数据
    BOOL LoadCheatPropFromDB(SGDP::ISDDBSession&);

    // 载入定义数据
    BOOL LoadDefPropFromDB(SGDP::ISDDBSession&);

    // 将从数据库取出的数据解析成方便读取的配置格式
    void CastProps();

private:
    // 根据精通类别、阶级、等级获取指定的装备精通经验信息
    const SEquipMasterExpProp* GetExpProp(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank, UINT16 wEquipLevel);

    // 根据精通类别、阶级获取装备精通突破信息
    const SEquipMasterBreakProp* GetBreakProp(EEquipPosType eEquipKind, EEquipMasterRank eEquipRank);

private:
    // 培养精通之书时生成精通经验书的概率列表
    CMasterRatePropMap m_mapRate;

    // 激活及突破数据表
    CMasterBreakPropMap m_mapBreak;

    // 经验及加成数据表
    CMasterExpPropMap m_mapExp;

    // 出千耗费元宝数据
    CMasterCheatCostPropVec m_vecCheatCost;

    UINT32 m_dwMasterCultivateMedalCost; // 培养精通之书耗费的勇气勋章
    UINT32 m_dwMasterCheatPlayerLevel;   // 装备精通出千功能多少级开放
    UINT32 m_dwBigBookExp;               // 每本大经验书加多少经验
    UINT32 m_dwSmallBookExp;             // 每本小经验书加多少经验
    BYTE   m_byMaxFreeCheatCnt;          // 每次培养精通之书可以免费出千几次
    BYTE   m_byExpXnum;                  // 某精通类别的大经验书满后所获经验乘以几倍

private:
    // 生成大精通经验书的概率表
    CRollBigExpBookVec m_vecBigExpBookRate;

    // 达到多少本大经验书才能获得额外经验
    BYTE m_byExtraExpBigBookCnt;
};

#endif // _equipmasterpropmgr_h_