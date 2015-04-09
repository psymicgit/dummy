#ifndef _equipmaster_h_
#define _equipmaster_h_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <common/client/commondef.h>
#include <protocol/server/protocligs.h>
#include "common/server/utility.h"

using namespace std ;

class CPlayer;
class CEquip;

/**
 * @brief 玩家的装备精通类
 */
class CEquipMaster : public CBaseObj
{
public:
    CEquipMaster();

public:
    // 初始化相关对象
    BOOL Init(DT_EQUIP_MASTER &stData, CPlayer* poOwner);
    VOID CopyTo(DT_EQUIP_MASTER &dst) { memcpy(&dst, &m_stData, sizeof(DT_EQUIP_MASTER)); }

public:
    // 是不是激活了武器精通，是的话装备属性得到提升
    void AddAttrByEquipMaster(CEquip&, DT_BATTLE_ATTRIBUTE&);

public:
    // 打开装备精通面板
    UINT16 OnOpenEquipMaster(PKT_CLIGS_OPEN_EQUIP_MASTER_ACK&);

    // 激活装备精通
    UINT16 OnActivateEquipMaster(PKT_CLIGS_ACTIVATE_MASTER_ACK&, EEquipPosType eEquipKind);

    // 突破
    UINT16 OnBreakMaster(PKT_CLIGS_BREAK_MASTER_ACK&, EEquipPosType byEquipKind);

    // 培养精通之书
    UINT16 OnCultivateMasterBook(PKT_CLIGS_GET_MASTER_BOOK_ACK&);

    // 刮开精通经验书面板
    UINT16 OnWipeMasterBook(PKT_CLIGS_WIPE_MASTER_BOOK_ACK&);

    // 精通经验书出千
    UINT16 OnCheatMasterBook(PKT_CLIGS_MASTER_CHEAT_ACK&);

    // 确认使用精通经验书
    UINT16 OnConfirmMasterBook(PKT_CLIGS_CONFIRM_MASTER_BOOK_ACK&);

public:
    // 装备精通等级、经验重置，用于gm指令
    void Reset();

    // 是否已激活
    BOOL IsActivated(DT_EQUIP_MASTER_DETAIL&);

    // 所有类别的装备精通是否已全部被激活
    BOOL IsAllActivated();
    
    // 是否达到突破条件
    BOOL IsBreakable(DT_EQUIP_MASTER_DETAIL&);

	// 是不是已经登峰造极了（所有精通全满）
	BOOL IsAllLevelMax();

    // 指定精通类别是不是已经登峰造极了
    BOOL IsLevelMax(DT_EQUIP_MASTER_DETAIL&);

	// 是否有某个精通类别处于可突破状态
	BOOL IsAnyBreakable();

    // 获取精通之书面板的状态
    EMasterBookPanelState GetMasterBookState();

    // 根据类别获取对应装备精通的详细信息
    DT_EQUIP_MASTER_DETAIL* GetMasterInfoByKind(UINT16 wEquipKind);

    // 所有精通类别的阶级是否已满
    BOOL IsAllRankTop();

    // 获取出千消耗的元宝
    UINT32 GetCheatGold();

private:
    // 激活装备精通
    void Activate(DT_EQUIP_MASTER_DETAIL&);

    // 突破精通阶级
    void BreakThrough(DT_EQUIP_MASTER_DETAIL&);

    // 设置精通经验书面板状态
    void SetExpBookPanelState(EMasterBookPanelState);

    // 生成精通经验书
    void GenerateMasterExpBook();

    // 根据获取到的经验书进行等级提升，并把升了多少经验存起来
    void LevelUpByExpBooks();

    // 增加指定装备类型的精通经验，并返回实际加了多少经验
    UINT32 AddExp(EEquipPosType eEquipKind, UINT32 dwExp);

    // 清除经验书面板并重置状态
    void ClearExpBooks();

    // 统计大经验书的数量
    BYTE CountBigExpBook();

    // 统计小经验书的数量
    BYTE CountSmallExpBook();

    // 获取出千次数
    BYTE GetCheatTimes();

    // 随机出一个类别来出千
    EEquipPosType RollCheatKind();

    // 在指定类别上出千
    void CheatAt(EEquipPosType byCheatKind);

    // 出千，失败返回0，成功返回出千类别
    EEquipPosType Cheat();
    
    // 是否已获取到精通之书
    BOOL IsMasterBookGotten();

    // 获取当前的精通等级
    UINT16 GetCurEquipLevel(EEquipPosType eEquipKind);

    // 获取各个精通类别当前的等级
    void GetCurEquipLevels(UINT16 wCurLevel[EQUIP_KIND_COUNT]);

	// 计算经验书面板上的经验总和，并在此基础上进行大经验书经验翻倍计算
	void GetExpUp(UINT16 awExpUp[EQUIP_KIND_COUNT]);

    // 单纯计算经验书面板上的经验总和
    void GetExpSum(UINT16 awExpSum[EQUIP_KIND_COUNT]);

public:
    // 存入打开装备精通面板回应消息体
    void StoreOpenMasterAck(PKT_CLIGS_OPEN_EQUIP_MASTER_ACK &ack);

    // 存入激活回应消息体
    void StoreActivateAck(PKT_CLIGS_ACTIVATE_MASTER_ACK &ack, DT_EQUIP_MASTER_DETAIL&);

    // 存入突破回应消息体
    void StoreBreakAck(PKT_CLIGS_BREAK_MASTER_ACK &ack, DT_EQUIP_MASTER_DETAIL&);

    // 存入培养精通之书回应消息体
    void StoreCultivateAck(PKT_CLIGS_GET_MASTER_BOOK_ACK &ack);

    // 存入确认刮开结果回应
    void StoreConfirmWipeAck(PKT_CLIGS_CONFIRM_MASTER_BOOK_ACK &ack, UINT16 awPreEquipLevel[EQUIP_KIND_COUNT], UINT16 awUpExps[EQUIP_KIND_COUNT]);

    // 存入出千回应
    void StoreCheatAck(PKT_CLIGS_MASTER_CHEAT_ACK &ack);

private:
    // 存入装备精通的详细信息列表
    void StoreIntoMasterDetail(DT_EQUIP_MASTER_DETAIL_CLI astData[EQUIP_KIND_COUNT]);

    // 存入使用经验书后的升级过程信息
    void StoreIntoLevelUpInfo(DT_MASTER_LEVEL_UP_INFO astInfo[EQUIP_KIND_COUNT], UINT16 awPreLevel[EQUIP_KIND_COUNT], UINT16 awUpExps[EQUIP_KIND_COUNT]);

    // 存入某个类别的装备精通详细信息
    void StoreIntoMasterDetailByKind(DT_EQUIP_MASTER_DETAIL_CLI&, DT_EQUIP_MASTER_DETAIL&);

    // 存入精通经验书面板的详细信息列表
    void StoreIntoBookPanel(DT_MASTER_EXP_BOOK_PANEL_CLI &stBookPanel);

    // 存入培养精通之书面板的出千信息
    void StoreIntoCheatInfo(DT_MASTER_CHEAT_INFO_CLI &stCheatInfo);

private:

    // 获取指定装备精通处于什么状态：可升级0、可激活1、可突破2、无法提升3、可突破但玩家等级不足4
    EEquipMasterDetailState GetActivateOrBreakState(DT_EQUIP_MASTER_DETAIL&);

    // 指定类别的当前等级经验条是否已满
    BOOL IsExpBarFull(DT_EQUIP_MASTER_DETAIL&);

    // 指定类别的精通阶级是否已经是顶级
    BOOL IsRankTop(DT_EQUIP_MASTER_DETAIL&);

    // 指定类别的精通类别是否经验已满
    BOOL IsLevelFull(DT_EQUIP_MASTER_DETAIL&);

    // 获取指定装备的属性提升百分比
    UINT16 GetCurPromote(DT_EQUIP_MASTER_DETAIL&);

    // 生成使用经验书后的升级过程信息
    void GenLeveUpInfo(DT_MASTER_LEVEL_UP_INFO&, EEquipPosType eEquipKind, UINT16 wPreLevel);

    // 修正装备精通数据，防数据异常
    void FixMaster();

private:
    DT_EQUIP_MASTER	m_stData;
};

DECLARE_CLASS_POOL_MGR(CEquipMaster)

#endif // _equipmaster_h_
