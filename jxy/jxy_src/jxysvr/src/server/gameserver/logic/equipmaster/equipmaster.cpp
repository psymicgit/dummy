#include "equipmaster.h"

#include "logic/player/player.h"
#include "logic/lua/luamgr.h"
#include "logic/item/equip/equip.h"
#include "logic/other/singleprammgr.h"
#include <dll/sdframework/sdloggerimpl.h>
#include <logic/build/build.h>
#include <common/server/utility.h>

#include "equipmasterpropmgr.h"


IMPLEMENT_CLASS_POOL_MGR(CEquipMaster)

CEquipMaster::CEquipMaster()
{
    memset(&m_stData, 0 , sizeof(m_stData));
}

BOOL CEquipMaster::Init(DT_EQUIP_MASTER &stData, CPlayer* poOwner)
{
    memcpy(&m_stData, &stData, sizeof(stData));
    FixMaster();

    m_poOwner = poOwner;
    return TRUE;
}

// 修正装备精通数据，防数据异常
void CEquipMaster::FixMaster()
{
    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_EQUIP_MASTER_DETAIL &stInfo = m_stData.astMasterInfo[i];
        stInfo.byMasterKind = i + 1;
        stInfo.byMasterRank = MIN(stInfo.byMasterRank, EMG_GRADE_4);

        // 已激活但等级却为0级，则修正为1级
        if(stInfo.wMasterLevel == 0 && stInfo.byMasterRank != 0)
        {
            stInfo.wMasterLevel = 1;
        }
    }
}

// 装备精通等级、经验重置，仅用于gm指令
void CEquipMaster::Reset()
{
    memset(&m_stData, 0, sizeof(m_stData));

    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        m_stData.astMasterInfo[i].byMasterKind = i + 1;
    }

    m_stData.byHaveDataFlag = TRUE;

    FixMaster();
}

// 打开装备精通面板
UINT16 CEquipMaster::OnOpenEquipMaster(PKT_CLIGS_OPEN_EQUIP_MASTER_ACK &ack)
{
    CBuild *poBuild = m_poOwner->GetBuild(EBK_EQUIP_MASTER);
    if(NULL == poBuild)
    {
        return ERR_OPEN_EQUIP_MASTER::ID_UNOPEN;
    }

    poBuild->OnOpen();

    StoreOpenMasterAck(ack);
    return ERR_OPEN_EQUIP_MASTER::ID_SUCCESS;
}

// 激活装备精通
UINT16 CEquipMaster::OnActivateEquipMaster(PKT_CLIGS_ACTIVATE_MASTER_ACK &ack, EEquipPosType eEquipKind)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return ERR_ACTIVATE_MASTER::ID_UNOPEN;
    }

    DT_EQUIP_MASTER_DETAIL *pstData = GetMasterInfoByKind(eEquipKind);
    if(NULL == pstData)
    {
        return ERR_COMMON::ID_OTHER_ERR;
    }

    // 检测勋章数
    UINT32 dwMedalCost = CEquipMasterPropMgr::Instance()->GetBreakMedal(eEquipKind, EMG_GRADE_0); // 激活所需勋章相当于0级突破
    if(m_poOwner->GetMedal() < dwMedalCost)
    {
        return ERR_ACTIVATE_MASTER::ID_MEDAL_NOT_ENOUGH;
    }

    // 检测该类别是否已激活
    if(IsActivated(*pstData))
    {
        StoreActivateAck(ack, *pstData);
        return ERR_ACTIVATE_MASTER::ID_SUCCESS;
    }

    // 扣除勋章
    m_poOwner->DecMedal(dwMedalCost, CRecordMgr::EDMT_BREAK_THROUGH_MASTER, pstData->byMasterKind, pstData->byMasterRank, pstData->wMasterLevel);

    // 成功激活，并进行属性加成
    ack.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
    Activate(*pstData);
    m_poOwner->ResetBattleAttr(TRUE);
    ack.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

    StoreActivateAck(ack, *pstData);
    return ERR_ACTIVATE_MASTER::ID_SUCCESS;
}

// 突破
UINT16 CEquipMaster::OnBreakMaster(PKT_CLIGS_BREAK_MASTER_ACK &ack, EEquipPosType eEquipKind)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return ERR_BREAK_MASTER::ID_UNOPEN;
    }

    DT_EQUIP_MASTER_DETAIL *pstData = GetMasterInfoByKind(eEquipKind);
    if(NULL == pstData)
    {
        return ERR_COMMON::ID_OTHER_ERR;
    }

    // 检测勋章数
    UINT32 dwMedalCost = CEquipMasterPropMgr::Instance()->GetBreakMedal(eEquipKind, (EEquipMasterRank)pstData->byMasterRank);
    if(m_poOwner->GetMedal() < dwMedalCost)
    {
        return ERR_BREAK_MASTER::ID_MEDAL_NOT_ENOUGH;
    }

    // 检测玩家等级
    UINT16 wRequiredPlayerLevel = CEquipMasterPropMgr::Instance()->GetBreakPlayerLevel(eEquipKind, (EEquipMasterRank)pstData->byMasterRank); // 突破所需的玩家等级
    if(m_poOwner->GetLevel() < wRequiredPlayerLevel)
    {
        return ERR_BREAK_MASTER::ID_PLAYER_LEVEL_NOT_ENOUGH;
    }

    // 检测是否可突破
    if(FALSE == IsBreakable(*pstData))
    {
        StoreBreakAck(ack, *pstData);
        return ERR_BREAK_MASTER::ID_SUCCESS;
    }

    // 扣除勋章
    m_poOwner->DecMedal(dwMedalCost, CRecordMgr::EDMT_BREAK_THROUGH_MASTER, pstData->byMasterKind, pstData->byMasterRank, pstData->wMasterLevel);

    // 成功突破，并进行属性值计算
    ack.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
    BreakThrough(*pstData);
    m_poOwner->ResetBattleAttr(TRUE);
    ack.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

    StoreBreakAck(ack, *pstData);
    return ERR_BREAK_MASTER::ID_SUCCESS;
}

// 培养精通之书
UINT16 CEquipMaster::OnCultivateMasterBook(PKT_CLIGS_GET_MASTER_BOOK_ACK &ack)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return ERR_GET_MASTER_BOOK::ID_UNOPEN;
    }

    // 检测所需勇气勋章
    UINT32 dwMedalCost = CEquipMasterPropMgr::Instance()->GetMasterCultivateMedalCost();
    if(m_poOwner->GetMedal() < dwMedalCost)
    {
        return ERR_GET_MASTER_BOOK::ID_MEDAL_NOT_ENOUGH;
    }

    // 检测是否已全部激活
    if(FALSE == IsAllActivated())
    {
        return ERR_GET_MASTER_BOOK::ID_NOT_ACTIVATE_ALL;
    }

    // 是否有某个精通类别可突破：是的话则提示玩家先突破
    if(IsAnyBreakable())
    {
        return ERR_GET_MASTER_BOOK::ID_UPGRADE_MASTER_PLEASE;
    }

    // 是否已经所有精通全满：全满后没必要再培养
    if(IsAllLevelMax())
    {
        return ERR_GET_MASTER_BOOK::ID_MASTER_MAX;
    }

    // 检测面板状态：培养精通之书面板应处于未打开状态
    if(GetMasterBookState() != MASTER_BOOK_PANEL_UN_OPEN)
    {
        StoreCultivateAck(ack);
        return ERR_GET_MASTER_BOOK::ID_SUCCESS;
    }    

    // 扣除勋章
    m_poOwner->DecMedal(dwMedalCost, CRecordMgr::EDMT_BREAK_THROUGH_MASTER);

    // 生成经验书刮刮乐
    GenerateMasterExpBook();
    StoreCultivateAck(ack);

    return ERR_GET_MASTER_BOOK::ID_SUCCESS;
}

// 刮开精通经验书面板
UINT16 CEquipMaster::OnWipeMasterBook(PKT_CLIGS_WIPE_MASTER_BOOK_ACK &ack)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return ERR_WIPE_MASTER_BOOK::ID_UNOPEN;
    }

    // 是否可刮：应处于打开但未刮开状态
    if(GetMasterBookState() != MASTER_BOOK_PANEL_GET_BUT_NOT_WIPED)
    {
        return ERR_WIPE_MASTER_BOOK::ID_SUCCESS;
    }

    SetExpBookPanelState(MASTER_BOOK_PANEL_WIPED_BUT_NOT_CONFIRMED);
    return ERR_WIPE_MASTER_BOOK::ID_SUCCESS;
}

// 精通经验书出千
UINT16 CEquipMaster::OnCheatMasterBook(PKT_CLIGS_MASTER_CHEAT_ACK &ack)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return ERR_MASTER_CHEAT::ID_UNOPEN;
    }

    // 状态检测：应该已有经验书才能出千
    if(FALSE == IsMasterBookGotten())
    {
        StoreCheatAck(ack);
        return ERR_MASTER_CHEAT::ID_SUCCESS;
    }

    SetExpBookPanelState(MASTER_BOOK_PANEL_WIPED_BUT_NOT_CONFIRMED);

    // 元宝是否足够
    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < GetCheatGold())
    {
        return ERR_MASTER_CHEAT::ID_GOLD_NOT_ENOUGH;
    }

    // 是否需要出千：没有小经验书则不需要出千
    if(CountSmallExpBook() == 0 || GetCheatTimes() >= MASTER_EXP_BOOK_COUNT)
    {
        return ERR_MASTER_CHEAT::ID_NOT_NEED_TO_CHEAT;
    }

    // 出千所需等级检测
    if(m_poOwner->GetLevel() < CEquipMasterPropMgr::Instance()->GetMasterCheatPlayerLevel())
    {
        return ERR_MASTER_CHEAT::ID_CHEAT_UNOPEN;
    }

    // 开始出千
    EEquipPosType byCheatKind = Cheat();
    if (0 == byCheatKind)
    {
        return ERR_MASTER_CHEAT::ID_NOT_NEED_TO_CHEAT;
    }

    // 扣除元宝
    UINT32 dwCheatGold = GetCheatGold();
    m_poOwner->DecGold(dwCheatGold, CRecordMgr::EDGT_EQUIP_MASTER_CHEAT, GetCheatTimes() - 1);

    StoreCheatAck(ack);
    return ERR_MASTER_CHEAT::ID_SUCCESS;
}

// 确认使用精通经验书
UINT16 CEquipMaster::OnConfirmMasterBook(PKT_CLIGS_CONFIRM_MASTER_BOOK_ACK &ack)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return ERR_CONFIRM_MASTER_BOOK::ID_UNOPEN;
    }

    UINT16 awCurLevel[EQUIP_KIND_COUNT];
    GetCurEquipLevels(awCurLevel);

    UINT16 awUpExps[EQUIP_KIND_COUNT] = {0}; // 四个类别各得到了多少经验

    // 是否可确认：应处于已打开经验书面板状态
    if(FALSE == IsMasterBookGotten())
    {
        StoreConfirmWipeAck(ack, awCurLevel, awUpExps);
        return ERR_CONFIRM_MASTER_BOOK::ID_SUCCESS;
    }

    GetExpSum(awUpExps);

    // 使用经验书，并重新计算属性加成
    ack.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
    LevelUpByExpBooks();
    m_poOwner->ResetBattleAttr(TRUE);
    ack.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

    SetExpBookPanelState(MASTER_BOOK_PANEL_UN_OPEN);
    m_stData.byCheatTimes = 0;

    StoreConfirmWipeAck(ack, awCurLevel, awUpExps);
    return ERR_CONFIRM_MASTER_BOOK::ID_SUCCESS;
}


// 是否已激活
BOOL CEquipMaster::IsActivated(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    return stDetail.byMasterRank;
}

// 所有类别的装备精通是否已全部被激活
BOOL CEquipMaster::IsAllActivated()
{
    for(BYTE i = 1; i <= EQUIP_KIND_COUNT; i++)
    {
        DT_EQUIP_MASTER_DETAIL &stData = m_stData.astMasterInfo[i - 1];
        if(FALSE == IsActivated(stData))
        {
            return FALSE;
        }
    }

    return TRUE;
}

// 激活
void CEquipMaster::Activate(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    if(IsActivated(stDetail))
    {
        return;
    }

    stDetail.byMasterRank = EMG_GRADE_1;
    stDetail.wMasterLevel = 1;
    stDetail.dwExp       = 0;
}

// 根据类别获取对应装备精通的详细信息
DT_EQUIP_MASTER_DETAIL* CEquipMaster::GetMasterInfoByKind(UINT16 wEquipKind)
{
    if(wEquipKind == 0 || wEquipKind > EQUIP_KIND_COUNT)
    {
        return NULL;
    }

    return &m_stData.astMasterInfo[wEquipKind - 1];
}

// 是否达到突破条件
BOOL CEquipMaster::IsBreakable(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    // 检测当前经验值是否已满、当前等级是否已满、阶级是否已到顶阶
    if(FALSE == IsExpBarFull(stDetail))
    {
        return FALSE;
    }

    if(FALSE == IsLevelFull(stDetail))
    {
        return FALSE;
    }

    if(IsRankTop(stDetail))
    {
        return FALSE;
    }

    return TRUE;
}

// 突破
void CEquipMaster::BreakThrough(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    if(FALSE == IsBreakable(stDetail))
    {
        return;
    }

    stDetail.byMasterRank++;
    stDetail.wMasterLevel = 1;
    stDetail.dwExp       = 0;
}

// 所有精通类别的阶级是否已经是最高
BOOL CEquipMaster::IsAllRankTop()
{
    for(BYTE i = 1; i <= EQUIP_KIND_COUNT; i++)
    {
        DT_EQUIP_MASTER_DETAIL &stData = m_stData.astMasterInfo[i - 1];
        if(stData.byMasterRank < MASTER_RANK_COUNT)
        {
            return FALSE;
        }
    }

    return TRUE;
}

// 获取出千消耗的元宝
UINT32 CEquipMaster::GetCheatGold()
{
    return CEquipMasterPropMgr::Instance()->GetCheatGoldCost(m_stData.byCheatTimes + 1);
}

// 获取指定装备的属性提升百分比
UINT16 CEquipMaster::GetCurPromote(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    UINT16 wPromote = CEquipMasterPropMgr::Instance()->GetAttrPromotion((EEquipPosType)stDetail.byMasterKind, (EEquipMasterRank)stDetail.byMasterRank, stDetail.wMasterLevel);
    return wPromote;
}

// 是否有某个精通类别处于可突破状态
BOOL CEquipMaster::IsAnyBreakable()
{
    for(BYTE i = 1; i <= EQUIP_KIND_COUNT; i++)
    {
        DT_EQUIP_MASTER_DETAIL &stData = m_stData.astMasterInfo[i - 1];
        if(IsBreakable(stData))
        {
            return TRUE;
        }
    }

    return FALSE;
}

// 是不是已经登峰造极了（所有精通全满）
BOOL CEquipMaster::IsAllLevelMax()
{
    for(BYTE i = 1; i <= EQUIP_KIND_COUNT; i++)
    {
        DT_EQUIP_MASTER_DETAIL &stData = m_stData.astMasterInfo[i - 1];
        if(FALSE == IsLevelMax(stData))
        {
            return FALSE;
        }
    }

    return TRUE;
}

// 指定精通类别是不是已经登峰造极了
BOOL CEquipMaster::IsLevelMax(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    return IsLevelFull(stDetail) && IsRankTop(stDetail);
}

// 指定类别的精通阶级是否已经是顶级
BOOL CEquipMaster::IsRankTop(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    return stDetail.byMasterRank >= EMG_GRADE_4;
}

// 指定类别的精通类别是否等级已满
BOOL CEquipMaster::IsLevelFull(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    UINT16 wBreakLevel = CEquipMasterPropMgr::Instance()->GetBreakLevel((EEquipPosType)stDetail.byMasterKind, (EEquipMasterRank)stDetail.byMasterRank);
    if(0 == wBreakLevel)
    {
        return FALSE;
    }

    return stDetail.wMasterLevel >= wBreakLevel;
}

// 生成精通经验书
void CEquipMaster::GenerateMasterExpBook()
{
    // 每个类别随机生成几本大经验书，剩下的就是小经验书
    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_MASTER_EXP_BOOK_DETAIL &stExpBook = m_stData.astExpBookPanel[i];
        stExpBook.byBigExpBookCnt = CEquipMasterPropMgr::Instance()->RollBigExpBookCnt((EEquipPosType)(i + 1));
    }

    // 设置当前经验书面板状态为：未刮开经验书
    m_stData.byCultivatePanelState = MASTER_BOOK_PANEL_GET_BUT_NOT_WIPED;
}

// 存入打开装备精通面板回应的消息结构体中
void CEquipMaster::StoreOpenMasterAck(PKT_CLIGS_OPEN_EQUIP_MASTER_ACK &ack)
{
    StoreIntoMasterDetail(ack.astEquipMasterDetail);
    if(GetMasterBookState() != MASTER_BOOK_PANEL_UN_OPEN)
    {
        StoreIntoBookPanel(ack.stExpBookPanelData);
    }

    ack.byExpBookPanelState = GetMasterBookState();
    ack.dwGetBookMedalCost  = CEquipMasterPropMgr::Instance()->GetMasterCultivateMedalCost();
    ack.qwCoin              = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    ack.dwGold              = (UINT32)m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
    ack.dwMedal             = (UINT32)m_poOwner->GetDT_PLAYER_BASE_DATA().qwMedal;
}

// 存入激活装备精通回应消息体
void CEquipMaster::StoreActivateAck(PKT_CLIGS_ACTIVATE_MASTER_ACK &ack, DT_EQUIP_MASTER_DETAIL &stDetail)
{
    StoreIntoMasterDetailByKind(ack.stEquipMasterInfo, stDetail);
    ack.dwMedal             = (UINT32)m_poOwner->GetMedal();
    ack.byCanGetMasterBook  = IsAllActivated();
}

// 存入突破回应消息体
void CEquipMaster::StoreBreakAck(PKT_CLIGS_BREAK_MASTER_ACK &ack, DT_EQUIP_MASTER_DETAIL &stDetail)
{
    StoreIntoMasterDetailByKind(ack.stEquipMasterInfo, stDetail);
    ack.dwMedal = (UINT32)m_poOwner->GetMedal();
}

// 存入培养精通之书回应消息体
void CEquipMaster::StoreCultivateAck(PKT_CLIGS_GET_MASTER_BOOK_ACK &ack)
{
    StoreIntoBookPanel(ack.stExpBookPanelData);

    ack.dwNextMedalCost = CEquipMasterPropMgr::Instance()->GetMasterCultivateMedalCost();
    ack.dwMedal         = (UINT32)m_poOwner->GetMedal();
}

// 存入确认刮开结果回应
void CEquipMaster::StoreConfirmWipeAck(PKT_CLIGS_CONFIRM_MASTER_BOOK_ACK &ack, UINT16 awPreEquipLevel[EQUIP_KIND_COUNT], UINT16 awUpExps[EQUIP_KIND_COUNT])
{
    ack.wCurPlayerLevel = m_poOwner->GetLevel();

    StoreIntoLevelUpInfo(ack.astLevelUpInfo, awPreEquipLevel, awUpExps);
    StoreIntoMasterDetail(ack.astEquipMasterDetail);
}

// 存入出千回应
void CEquipMaster::StoreCheatAck(PKT_CLIGS_MASTER_CHEAT_ACK &ack)
{
    ack.dwGold = (UINT32)m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;

    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        ack.astExpBooks[i].byBigExpBookCnt = m_stData.astExpBookPanel[i].byBigExpBookCnt;
    }

    StoreIntoCheatInfo(ack.stCheatInfo);
}

// 存入某个类别的装备精通详细信息
void CEquipMaster::StoreIntoMasterDetailByKind(DT_EQUIP_MASTER_DETAIL_CLI &stCliData, DT_EQUIP_MASTER_DETAIL &stInfo)
{
    CEquipMasterPropMgr &oPropMgr  = *CEquipMasterPropMgr::Instance();
    EEquipPosType eEquipKind       = (EEquipPosType)stInfo.byMasterKind;
    EEquipMasterRank eEquipRank    = (EEquipMasterRank)stInfo.byMasterRank;

    stCliData.byEquipKind          = eEquipKind;
    stCliData.byMasterRank         = eEquipRank;
    stCliData.wMasterLevel         = stInfo.wMasterLevel;
    stCliData.dwCurMasterExp       = stInfo.dwExp;
    stCliData.dwMaxMasterExp       = oPropMgr.GetMaxExp(eEquipKind, eEquipRank, stInfo.wMasterLevel);
    stCliData.wCurPromote          = oPropMgr.GetAttrPromotion(eEquipKind, eEquipRank, stInfo.wMasterLevel);
    stCliData.wNextPromote         = oPropMgr.GetNextPromotion(eEquipKind, eEquipRank, stInfo.wMasterLevel);
    stCliData.byActivateOrBreak    = GetActivateOrBreakState(stInfo);
    stCliData.dwMedalCost          = oPropMgr.GetBreakMedal(eEquipKind, eEquipRank);
    stCliData.wRequiredPlayerLevel = oPropMgr.GetBreakPlayerLevel(eEquipKind, eEquipRank);

    // 无法再提升时，最大经验就不显示了，变为0
    if(IsLevelMax(stInfo))
    {
        stCliData.dwMaxMasterExp   = 0;
    }
}

// 存入装备精通的详细信息列表
void CEquipMaster::StoreIntoMasterDetail(DT_EQUIP_MASTER_DETAIL_CLI astData[EQUIP_KIND_COUNT])
{
    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_EQUIP_MASTER_DETAIL_CLI &stCliData = astData[i];
        DT_EQUIP_MASTER_DETAIL &stData = m_stData.astMasterInfo[i];

        StoreIntoMasterDetailByKind(stCliData, stData);
    }
}

// 存入使用经验书后的升级过程信息
void CEquipMaster::StoreIntoLevelUpInfo(DT_MASTER_LEVEL_UP_INFO astInfo[EQUIP_KIND_COUNT], UINT16 awPreLevel[EQUIP_KIND_COUNT], UINT16 awUpExps[EQUIP_KIND_COUNT])
{
    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_MASTER_LEVEL_UP_INFO &stInfo = astInfo[i];
        stInfo.wUpExp = awUpExps[i];

        GenLeveUpInfo(stInfo, (EEquipPosType)(i + 1), awPreLevel[i]);
    }
}

// 获取指定装备精通处于什么状态：可升级0、可激活1、可突破2、无法提升3、可突破但玩家等级不足4
EEquipMasterDetailState CEquipMaster::GetActivateOrBreakState(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    if(FALSE == IsActivated(stDetail))
    {
        return EQUIP_MASTER_ACTIVATABLE; // 可激活
    }

    // 是否可突破
    if(IsBreakable(stDetail))
    {
        // 玩家等级是否不足
        if(m_poOwner->GetLevel() < CEquipMasterPropMgr::Instance()->GetBreakPlayerLevel((EEquipPosType)stDetail.byMasterKind, (EEquipMasterRank)stDetail.byMasterRank))
        {
            return EQUIP_MASTER_PLAYER_LVL_NOT_ENOUGH;
        }

        return EQUIP_MASTER_BREAKABLE; // 可突破
    }

    // 是否已经巅峰
    if(IsLevelFull(stDetail) && IsRankTop(stDetail))
    {
        return EQUIP_MASTER_TOP;
    }

    return EQUIP_MASTER_UPGRADABLE;
}

// 存入培养精通之书面板的出千信息
void CEquipMaster::StoreIntoCheatInfo(DT_MASTER_CHEAT_INFO_CLI &stCheatInfo)
{
    stCheatInfo.wCheatGold          = GetCheatGold();

    CHAR freeChatTime = CEquipMasterPropMgr::Instance()->GetMaxFreeCheatCnt() - GetCheatTimes();
    stCheatInfo.byFreeCheatCnt =  (freeChatTime > 0 ? freeChatTime : 0);
}

// 存入精通经验书面板的详细信息列表
void CEquipMaster::StoreIntoBookPanel(DT_MASTER_EXP_BOOK_PANEL_CLI &stBookPanel)
{
    for(int i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        stBookPanel.astExpBooks[i].byBigExpBookCnt = m_stData.astExpBookPanel[i].byBigExpBookCnt;
    }

    stBookPanel.wBigBookExp     = CEquipMasterPropMgr::Instance()->GetBigBookExp();
    stBookPanel.wSmallBookExp   = CEquipMasterPropMgr::Instance()->GetSmallBookExp();

    stBookPanel.byIsOpenUpCheat = (m_poOwner->GetLevel() >= CEquipMasterPropMgr::Instance()->GetMasterCheatPlayerLevel());
    if(stBookPanel.byIsOpenUpCheat)
    {
        StoreIntoCheatInfo(stBookPanel.stCheatInfo);
    }
}

// 获取精通之书面板的状态
EMasterBookPanelState CEquipMaster::GetMasterBookState()
{
    return (EMasterBookPanelState)m_stData.byCultivatePanelState;
}

// 是否已获取到精通之书
BOOL CEquipMaster::IsMasterBookGotten()
{
    return (MASTER_BOOK_PANEL_WIPED_BUT_NOT_CONFIRMED == GetMasterBookState() || MASTER_BOOK_PANEL_GET_BUT_NOT_WIPED == GetMasterBookState());
}

// 获取当前的精通等级
UINT16 CEquipMaster::GetCurEquipLevel(EEquipPosType eEquipKind)
{
    DT_EQUIP_MASTER_DETAIL *pstDetail = GetMasterInfoByKind(eEquipKind);
    if(NULL == pstDetail)
    {
        return 0;
    }

    return pstDetail->wMasterLevel;
}

// 获取各个精通类别当前的等级
void CEquipMaster::GetCurEquipLevels(UINT16 wCurLevel[EQUIP_KIND_COUNT])
{
    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        wCurLevel[i] = GetCurEquipLevel((EEquipPosType)(i + 1));
    }
}


// 设置精通经验书面板状态
void CEquipMaster::SetExpBookPanelState(EMasterBookPanelState ePanelState)
{
    m_stData.byCultivatePanelState = ePanelState;
}

// 计算经验书面板上的经验总和，并在此基础上进行大经验书经验翻倍计算
void CEquipMaster::GetExpUp(UINT16 awExpUp[EQUIP_KIND_COUNT])
{
    GetExpSum(awExpUp);

    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_MASTER_EXP_BOOK_DETAIL &stExpBook = m_stData.astExpBookPanel[i];

        // 如果大经验书满了，则最终经验值 = 原本经验值 X 奖励倍数
        if(MASTER_EXP_BOOK_COUNT_PER_KIND == stExpBook.byBigExpBookCnt)
        {
            awExpUp[i] *= CEquipMasterPropMgr::Instance()->GetExpXnum();
        }
    }
}

// 单纯计算经验书面板上的经验总和
void CEquipMaster::GetExpSum(UINT16 awExpSum[EQUIP_KIND_COUNT])
{
    memset(awExpSum, 0, sizeof(awExpSum[0]) * EQUIP_KIND_COUNT);

    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_MASTER_EXP_BOOK_DETAIL &stExpBook = m_stData.astExpBookPanel[i];

        UINT32 dwBigExp = CEquipMasterPropMgr::Instance()->GetBigBookExp() * stExpBook.byBigExpBookCnt;
        UINT32 dwSmallExp = CEquipMasterPropMgr::Instance()->GetSmallBookExp() * (MASTER_EXP_BOOK_COUNT_PER_KIND - stExpBook.byBigExpBookCnt);

        // 经验书经验 = 小书经验 + 大书经验
        awExpSum[i] =  dwBigExp + dwSmallExp;
    }
}

// 根据获取到的经验书进行等级提升，并将
void CEquipMaster::LevelUpByExpBooks()
{
    UINT16 awExpUp[EQUIP_KIND_COUNT];
    GetExpUp(awExpUp);

    for(BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        AddExp((EEquipPosType)(i + 1), awExpUp[i]);
    }

    ClearExpBooks();
}

// 当前等级经验条是否已满
BOOL CEquipMaster::IsExpBarFull(DT_EQUIP_MASTER_DETAIL &stDetail)
{
    UINT32 dwCurLevelMaxExp = CEquipMasterPropMgr::Instance()->GetMaxExp((EEquipPosType)stDetail.byMasterKind, (EEquipMasterRank)stDetail.byMasterRank, stDetail.wMasterLevel);
    if(stDetail.dwExp >= dwCurLevelMaxExp)
    {
        return TRUE;
    }

    return FALSE;
}

// 增加指定装备类型的精通经验，并返回实际加了多少经验
UINT32 CEquipMaster::AddExp(EEquipPosType eEquipKind, UINT32 dwExpAdd)
{
    DT_EQUIP_MASTER_DETAIL *pstData = GetMasterInfoByKind(eEquipKind);
    if(NULL == pstData)
    {
        return 0;
    }

    UINT32 dwLeftExp = dwExpAdd;

    // 突破等级
    UINT16 wBreakLevel = CEquipMasterPropMgr::Instance()->GetBreakLevel(eEquipKind, (EEquipMasterRank)pstData->byMasterRank);
    if(0 == wBreakLevel)
    {
        return 0;
    }

    // 根据所得的经验一直升级
    while(dwLeftExp > 0)
    {
        UINT32 dwMaxExp = CEquipMasterPropMgr::Instance()->GetMaxExp(eEquipKind, (EEquipMasterRank)pstData->byMasterRank, pstData->wMasterLevel);
        if(0 == dwMaxExp)
        {
            break;;
        }

        // 所得经验是否足够升到下一级
        if(pstData->dwExp + dwLeftExp >= dwMaxExp)
        {
            // 是不是快要突破了
            if(pstData->wMasterLevel >= wBreakLevel)
            {
                // 是否已经升到极阶，到极的话无法再突破，经验条变为0
                if(pstData->byMasterRank >= EMG_GRADE_4)
                {
                    pstData->dwExp = 0;
                }
                else
                {
                    dwLeftExp -= dwMaxExp - pstData->dwExp;
                    pstData->dwExp = dwMaxExp;
                }

                break;
            }
            else
            {
                dwLeftExp -= dwMaxExp - pstData->dwExp;
                pstData->wMasterLevel++;
                pstData->dwExp = 0;
            }
        }
        else
        {
            // 不够升到下一级
            pstData->dwExp += dwLeftExp;
            dwLeftExp = 0;

            break;
        }
    }

    return dwExpAdd - dwLeftExp;
}

// 生成使用经验书后的升级过程信息
void CEquipMaster::GenLeveUpInfo(DT_MASTER_LEVEL_UP_INFO &stInfo, EEquipPosType eEquipKind, UINT16 wPreLevel)
{
    DT_EQUIP_MASTER_DETAIL *pstDetail = GetMasterInfoByKind(eEquipKind);
    if(NULL == pstDetail)
    {
        return;
    }

    EEquipMasterRank eEquipRank = (EEquipMasterRank)pstDetail->byMasterRank;

    BYTE byLevelDiff = pstDetail->wMasterLevel - wPreLevel - 1;
    BYTE byMinLevelDiff = MIN(byLevelDiff, MASTER_MAX_LEVEL_UP);
    
    for (BYTE i = 0; i < byMinLevelDiff; i++)
    {
        DT_MASTER_LEVEL_UP &stLevelUp = stInfo.astLevelUp[i];

        stLevelUp.dwMaxMasterExp = CEquipMasterPropMgr::Instance()->GetMaxExp(eEquipKind, eEquipRank, wPreLevel + i + 1);
        stLevelUp.wPromote       = CEquipMasterPropMgr::Instance()->GetAttrPromotion(eEquipKind, eEquipRank, wPreLevel + i + 1);
        stLevelUp.wNextPromote   = CEquipMasterPropMgr::Instance()->GetAttrPromotion(eEquipKind, eEquipRank, wPreLevel + i + 2);
    }

    stInfo.byLevelDiff = byMinLevelDiff;
    stInfo.wPreLevel = wPreLevel;
}

// 清除经验书面板并重置状态
void CEquipMaster::ClearExpBooks()
{
    memset(m_stData.astExpBookPanel, 0, sizeof(m_stData.astExpBookPanel));
}

// 是不是激活了武器精通，是的话装备属性得到提升
void CEquipMaster::AddAttrByEquipMaster(CEquip &oEquip, DT_BATTLE_ATTRIBUTE& stAttr)
{
    if(NULL == m_poOwner->GetBuild(EBK_EQUIP_MASTER))
    {
        return;
    }

    SEquipProp* pstEquipProp =  CEquipPropMgr::Instance()->GetProp(oEquip.GetItemKindID());
    if(NULL == pstEquipProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstEquipProp is NULL,KindID[%d]!"), MSG_MARK, oEquip.GetItemKindID());
        return;
    }

    EEquipPosType eEquipKind = (EEquipPosType)pstEquipProp->byPosKindID; // 获取装备的位置类型（头盔、战袍、武器、战靴）
    DT_EQUIP_MASTER_DETAIL *pstData = GetMasterInfoByKind(eEquipKind);
    if(NULL == pstData)
    {
        return;
    }

    if(FALSE == IsActivated(*pstData))
    {
        return;
    }

    // 获取装备属性提升百分比
    UINT16 wPromote = GetCurPromote(*pstData);
    stAttr.dwAttack += (UINT32)(stAttr.dwAttack * FLOAT(wPromote) / 100);
}

// 统计大经验书的数量
BYTE CEquipMaster::CountBigExpBook()
{
    BYTE byBigExpBookCnt = 0;

    // 四个类别大经验书数量累加
    for (BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        byBigExpBookCnt += m_stData.astExpBookPanel[i].byBigExpBookCnt;
    }

    return byBigExpBookCnt;
}

// 统计小经验书的数量
BYTE CEquipMaster::CountSmallExpBook()
{
    return MASTER_EXP_BOOK_COUNT - CountBigExpBook();
}

// 获取出千次数
BYTE CEquipMaster::GetCheatTimes()
{
    return m_stData.byCheatTimes;
}

// 随机出一个类别来出千
EEquipPosType CEquipMaster::RollCheatKind()
{
    // 先统计出多少类别有小经验书可以出千
    BYTE byCanCheatKindCnt = 0;
    for (BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_MASTER_EXP_BOOK_DETAIL &stExpBook   = m_stData.astExpBookPanel[i];
        if(stExpBook.byBigExpBookCnt < MASTER_EXP_BOOK_COUNT_PER_KIND)
        {
            byCanCheatKindCnt++;
        }
    }

    if(0 == byCanCheatKindCnt)
    {
        return (EEquipPosType)(0);
    }

    // 然后随机选个可以出千的类别
    BYTE byRand = Random(1, byCanCheatKindCnt);

    BYTE byCheatKind = 0;
    for (BYTE i = 0; i < EQUIP_KIND_COUNT; i++)
    {
        DT_MASTER_EXP_BOOK_DETAIL &stExpBook   = m_stData.astExpBookPanel[i];
        if(stExpBook.byBigExpBookCnt < MASTER_EXP_BOOK_COUNT_PER_KIND)
        {
            byRand--;

            if(0 == byRand)
            {
                byCheatKind = i + 1;
                break;;
            }
        }
    }

    return (EEquipPosType)byCheatKind;
}

// 在指定类别上出千
void CEquipMaster::CheatAt(EEquipPosType byCheatKind)
{
    if(byCheatKind == 0 || byCheatKind > EQUIP_KIND_COUNT)
    {
        return;
    }

    DT_MASTER_EXP_BOOK_DETAIL &stExpBook   = m_stData.astExpBookPanel[byCheatKind - 1];
    if(stExpBook.byBigExpBookCnt >= MASTER_EXP_BOOK_COUNT_PER_KIND)
    {
        return;
    }

    stExpBook.byBigExpBookCnt++;
    m_stData.byCheatTimes++;
}

// 出千，失败返回0，成功返回出千类别
EEquipPosType CEquipMaster::Cheat()
{
    EEquipPosType byCheatKind = RollCheatKind();
    CheatAt(byCheatKind);

    return byCheatKind;
}
