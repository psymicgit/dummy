#include "awaken.h"
#include "awakenpropmgr.h"
#include "awakenpropmgr2.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <logic/hero/heromgr.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <logic/record/recordmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/base/basedefine.h>
#include <logic/task/taskmgr.h>
#include <logic/vip/vippropmgr.h>
#include <logic/player/player.h>
#include "logic/medal/playermedal.h"
#include "logic/other/fewparam.h"
using namespace SGDP;


IMPLEMENT_CLASS_POOL_MGR(CAwaken)
CAwaken::CAwaken()
{
    m_byAwakenVersion = 2; //tmd各种适配
    memset(&m_stDT_AWAKEN_DATA, 0, sizeof(m_stDT_AWAKEN_DATA));
}

CAwaken::~CAwaken()
{
    m_mapIdleStudyIdx.clear();
}

BOOL CAwaken::Init(DT_AWAKEN_DATA& stAwakenData, CPlayer* poOwner)
{
    if((NULL == poOwner))
    {
        return FALSE;
    }


    memcpy(&m_stDT_AWAKEN_DATA, &stAwakenData, sizeof(m_stDT_AWAKEN_DATA));
    m_poOwner = poOwner;

    for(UINT8 byIdx = 0; byIdx < MAX_BAG_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byIdx];
        if(0 == stDT_STUDY_DATA.byStudyColorKind) //0表示没有数据
        {
            m_mapIdleStudyIdx[byIdx] = byIdx;
        }
    }

    return TRUE;
}


//悟道
UINT16 CAwaken::Awaken(UINT8 byQuickFlag, UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM], DT_AWAKEN_TAB_DATA& stAwakenInfo)
{
    UINT16 wErrCode = 0;
    byAwakenNum = 0;
    if(byQuickFlag)
    {
        wErrCode = QuickAwaken(byAwakenNum, astAwakenGetInfo);
    }
    else
    {
        wErrCode = AwakenOnce(byGoldFlag, byAwakenNum, astAwakenGetInfo);
    }

    GetAwakenTabInfo(stAwakenInfo);
    return wErrCode;
}

// 悟道心得
VOID CAwaken::AwakenStudy(DT_AWAKEN_GET_DATA& stAwakenGetData)
{
    memset(&stAwakenGetData, 0, sizeof(stAwakenGetData));
    DT_AWAKEN_BASE_DATA& stBaseInfo = m_stDT_AWAKEN_DATA.stBaseInfo;
    //元宝悟道在入定/彻悟状态时使用不同的概率，则这两个状态的元宝悟道标记不能清除
    if((1 == stBaseInfo.byGoldFlag) && ((EALK_RUDING != stBaseInfo.byLevel) && (EALK_CHEWU != stBaseInfo.byLevel)))
    {
        stBaseInfo.byGoldFlag = 0;
    }

    C88MapItr itr = m_mapIdleStudyIdx.begin();
    if(itr == m_mapIdleStudyIdx.end())
    {
        //异常..
        return;
    }

    DT_STUDY_DATA* pstDT_STUDY_DATA = &m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first];
    memset(pstDT_STUDY_DATA, 0, sizeof(DT_STUDY_DATA));
    pstDT_STUDY_DATA->byStudyLevel = 1;//生成的心得为1级
    UINT32 dwGenCoinOnce = 0;
    stBaseInfo.byLevel = CAwakenPropMgr::Instance()->AwakenStudy(m_poOwner->GetLevel(), stBaseInfo.byLevel, stBaseInfo.byGoldFlag,
                         pstDT_STUDY_DATA->byStudyColorKind, pstDT_STUDY_DATA->byStudyAttrKind1, dwGenCoinOnce);
    if(0 != pstDT_STUDY_DATA->byStudyColorKind) //有生成心得
    {
        //记录高级别的心得
        if( EC_RED == pstDT_STUDY_DATA->byStudyColorKind || EC_ORANGE == pstDT_STUDY_DATA->byStudyColorKind )
        {
            CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDY, CRecordMgr::EASTUDYT_AWAKEN, pstDT_STUDY_DATA->byStudyColorKind, 0,
                                                m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, pstDT_STUDY_DATA->byStudyAttrKind1, pstDT_STUDY_DATA->byStudyAttrKind2, pstDT_STUDY_DATA->byStudyLevel);
        }

        stAwakenGetData.byStudyBagIdx = itr->first;
        m_mapIdleStudyIdx.erase(itr);
    }
    else
    {
        pstDT_STUDY_DATA = NULL;
        m_poOwner->AddCoin(dwGenCoinOnce, CRecordMgr::EACT_AWAKEN);
        stAwakenGetData.dwGetCoin = dwGenCoinOnce;
    }
    if(0 == stBaseInfo.byLevel)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Player[%d] Awaken Level is 0!"), MSG_MARK, m_poOwner->GetID());
        stBaseInfo.byLevel = EALK_KESHUI;
    }

    stAwakenGetData.byAwakenLevel = stBaseInfo.byLevel;

}


UINT16 CAwaken::QuickAwaken(UINT8& byAwakenNum, DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM])
{
    UINT16 wErrCode = ERR_AWAKEN::ID_SUCCESS;
    DT_AWAKEN_BASE_DATA& stBaseInfo = m_stDT_AWAKEN_DATA.stBaseInfo;

    //入定/彻悟状态可一键悟道
    //if(((1 == stBaseInfo.byGoldFlag) && ((EALK_RUDING == stBaseInfo.byLevel) || (EALK_CHEWU == stBaseInfo.byLevel)))
    //	|| (EALK_CHEWU == stBaseInfo.byLevel))
    //{
    //	return ERR_AWAKEN::ID_INVALID_STATE;
    //}

    // 最大循环MAX_AWAKEN_NUM次，避免死循环
    for(UINT16 wIdx = 0; ((wIdx < MAX_AWAKEN_NUM) && (m_mapIdleStudyIdx.size() > 0)); wIdx++)
    {
        wErrCode = AwakenOnce(0, byAwakenNum, astAwakenGetInfo);
        if((ERR_AWAKEN::ID_SUCCESS != wErrCode) || (EALK_CHEWU == stBaseInfo.byLevel)) //悟道错误或彻悟状态则退出悟道
        {
            break;
        }
    }

    //铜钱不足算成功
    if(ERR_COMMON::ID_COIN_NOT_ENOUGH == wErrCode)
    {
        wErrCode = ERR_AWAKEN::ID_SUCCESS;
    }

    return wErrCode;
}

UINT16 CAwaken::AwakenOnce(UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM])
{
    if(byGoldFlag)
    {
        //已经是入定状态
        if(EALK_RUDING == m_stDT_AWAKEN_DATA.stBaseInfo.byLevel)
        {
            return ERR_AWAKEN::ID_SUCCESS;
        }

        if(0 == GetCanCallNum())
        {
            return ERR_AWAKEN::ID_REACH_CALL_NUM;
        }

        //判断元宝
        UINT32 dwCostGold = CAwakenPropMgr::Instance()->GetAwakenGold();
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }

        //扣除元宝
        m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_AWAKENONCE);

        m_stDT_AWAKEN_DATA.stBaseInfo.byLevel = EALK_RUDING;
        m_stDT_AWAKEN_DATA.stBaseInfo.byGoldFlag = 1;

        m_stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum++;
        m_stDT_AWAKEN_DATA.stBaseInfo.qwlastCallTime = SDTimeSecs();

        return ERR_AWAKEN::ID_SUCCESS;
    }
    else
    {
        //判断心得碎片格子是否已满
        if(0 == m_mapIdleStudyIdx.size())
        {
            return ERR_AWAKEN::ID_STUDY_BAG_FULL;

        }

        //判断铜币
        UINT32 dwCostCoin = CAwakenPropMgr::Instance()->GetAwakenCoin(m_stDT_AWAKEN_DATA.stBaseInfo.byGoldFlag, m_stDT_AWAKEN_DATA.stBaseInfo.byLevel);
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostCoin)
        {
            return ERR_COMMON::ID_COIN_NOT_ENOUGH;
        }

        //扣除铜钱
        m_poOwner->DecCoin(dwCostCoin, CRecordMgr::EDCT_AWAKEN, m_stDT_AWAKEN_DATA.stBaseInfo.byLevel);

        AwakenStudy(astAwakenGetInfo[byAwakenNum++]);
    }

    return ERR_AWAKEN::ID_SUCCESS;
}

//转换心得经验
UINT16 CAwaken::ConvertExp(UINT8 byGridIdx, UINT8 byAll, UINT8 abySelectConvertColor[MAX_COLOR_NUM], UINT32& dwGenStudyExp, DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], UINT64& qwCurStudyExp)
{
    UINT16 wErrCode = ERR_STUDY_TO_EXP::ID_SUCCESS;
    dwGenStudyExp = 0;
    if(1 == byAll)
    {
        for(byGridIdx = 0; byGridIdx < MAX_BAG_STUDY_GRID_NUM; byGridIdx++)
        {
            if(m_mapIdleStudyIdx.find(byGridIdx) == m_mapIdleStudyIdx.end())
            {
                DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byGridIdx];
                if((0 == stDT_STUDY_DATA.byStudyColorKind) || (stDT_STUDY_DATA.byStudyColorKind > MAX_COLOR_NUM) || (0 == abySelectConvertColor[stDT_STUDY_DATA.byStudyColorKind - 1]))
                {
                    continue;
                }
                wErrCode = ConvertStudyExp(byGridIdx, dwGenStudyExp);
                if(wErrCode != ERR_STUDY_TO_EXP::ID_SUCCESS)
                {
                    break;
                }
            }
        }
    }
    else
    {
        wErrCode = ConvertStudyExp(byGridIdx, dwGenStudyExp);
    }

    GetBagStudyInfo(astBagStudyInfo);
    qwCurStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;

    return wErrCode;
}

UINT16 CAwaken::ConvertStudyExp(UINT8 byGridIdx, UINT32& dwGetExp)
{
    if(byGridIdx >= MAX_BAG_STUDY_GRID_NUM)
    {
        return ERR_STUDY_TO_EXP::ID_IDX_INVALID;
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byGridIdx];
    if(0 == stDT_STUDY_DATA.byStudyColorKind)
    {
        return ERR_STUDY_TO_EXP::ID_STUDY_NOT_EXIST;
    }
    UINT32 dwExp = CAwakenPropMgr::Instance()->GetStudyExp(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyLevel);
    dwGetExp += dwExp;
    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp += dwExp;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYEXP, CRecordMgr::EASET_CONVERTSTUDYEXP, dwExp, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp,
                                        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_STUDY_DATA.byStudyAttrKind1, 0, stDT_STUDY_DATA.byStudyLevel);

    //if( EC_RED==stDT_STUDY_DATA.byStudyColorKind || EC_ORANGE==stDT_STUDY_DATA.byStudyColorKind )
    //{
    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECSTUDY, CRecordMgr::EDSTUDYT_CONVERT, stDT_STUDY_DATA.byStudyColorKind, 0,
                                        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_STUDY_DATA.byStudyAttrKind1, 0, stDT_STUDY_DATA.byStudyLevel);
    //}
    m_mapIdleStudyIdx[byGridIdx] = byGridIdx;
    memset(&stDT_STUDY_DATA, 0, sizeof(stDT_STUDY_DATA));

    return ERR_STUDY_TO_EXP::ID_SUCCESS;
}


//升级心得
UINT16 CAwaken::UpgradeStudy(UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byToLevel, DT_HERO_STUDY_DATA_CLI& stHeroStudyInfo, UINT64& qwCurStudyExp)
{
    if(byGridIdx >= MAX_HERO_STUDY_GRID_NUM)
    {
        return ERR_UPGRADE_STUDY::ID_IDX_INVALID;
    }

    CHero* poHero = m_poOwner->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_UPGRADE_STUDY::ID_HERO_NOT_EXIST;
    }

    DT_STUDY_DATA* pstDT_STUDY_DATA = poHero->GetStudy(byGridIdx);
    if(NULL == pstDT_STUDY_DATA)
    {
        RETURN_OTHER_ERR;
    }
    if(0 == pstDT_STUDY_DATA->byStudyAttrKind1)
    {
        return ERR_UPGRADE_STUDY::ID_STUDY_NOT_EXIST;
    }
    if(pstDT_STUDY_DATA->byStudyLevel >= byToLevel)
    {
        GetHeroStudyDataCli(poHero, stHeroStudyInfo);
        qwCurStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;
        return ERR_UPGRADE_STUDY::ID_SUCCESS;
    }
    if(byToLevel != pstDT_STUDY_DATA->byStudyLevel + 1)
    {
        return ERR_UPGRADE_STUDY::ID_CANNOT_JMP_LEVEL;
    }

    UINT32 dwCostExp = CAwakenPropMgr::Instance()->GetStudyUpgradeNeedExp(pstDT_STUDY_DATA->byStudyColorKind, pstDT_STUDY_DATA->byStudyAttrKind1, pstDT_STUDY_DATA->byStudyLevel);
    if(m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp < dwCostExp)
    {
        return ERR_UPGRADE_STUDY::ID_NOT_ENOUGH_STUDYEXP;
    }

    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp -= dwCostExp;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECSTUDYEXP, CRecordMgr::EDSET_UPGRADESTUDY, dwCostExp, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);

    pstDT_STUDY_DATA->byStudyLevel++;

    GetHeroStudyDataCli(poHero, stHeroStudyInfo);
    qwCurStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;

    poHero->OnUpgradeStudy();

    //重置玩家属性
    m_poOwner->ResetBattleAttr();

    CTaskMgr::Instance()->OnStudy(m_poOwner);
    return ERR_UPGRADE_STUDY::ID_SUCCESS;
}

UINT16 CAwaken::DressStudy(UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx, DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], DT_HERO_STUDY_DATA_CLI& stHeroStudyInfo)
{
    if((byHeroGridIdx >= MAX_HERO_STUDY_GRID_NUM) || (byBagGridIdx >= MAX_BAG_STUDY_GRID_NUM))
    {
        return ERR_DRESS_STUDY::ID_IDX_INVALID;
    }

    CHero* poHero = m_poOwner->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_DRESS_STUDY::ID_HERO_NOT_EXIST;
    }

    DT_STUDY_DATA* pstHeroStudy = poHero->GetStudy(byHeroGridIdx);
    if(NULL == pstHeroStudy)
    {
        RETURN_OTHER_ERR;
    }

    DT_STUDY_DATA& stBagStudy = m_stDT_AWAKEN_DATA.astBagStudyInfo[byBagGridIdx];
    if(m_poOwner->CKGuide(EGF_AWAKEN)) //引导,给1级蓝色生命心得，悟道等级2
    {
        m_stDT_AWAKEN_DATA.stBaseInfo.byLevel = EALK_SHENGYOU;

        stBagStudy.byStudyColorKind = EC_BLUE;
        stBagStudy.byStudyAttrKind1 = EBAK_HP;
        stBagStudy.byStudyLevel = 1;
    }
    BOOL byOldBagStudyColorKind = stBagStudy.byStudyColorKind;
    BOOL byOldHeroStudyColorKind = pstHeroStudy->byStudyColorKind;
    BOOL byOldBagStudyAttrKind = stBagStudy.byStudyAttrKind1;
    BOOL byOldHeroStudyAttrKind = pstHeroStudy->byStudyAttrKind1;
    if((0 == byOldBagStudyColorKind) && (0 == byOldHeroStudyColorKind))
    {
        return ERR_DRESS_STUDY::ID_STUDY_NOT_EXIST;
    }

    //未解锁可卸载，不可穿戴
    if((0 == byOldHeroStudyColorKind) && (!CAwakenPropMgr::Instance()->CkUnlockHeroGrid(poHero->GetDT_HERO_BASE_DATA().wLevel, byHeroGridIdx)))
    {
        return ERR_DRESS_STUDY::ID_POS_NOT_UNLOCK;
    }

    //替换的属性不同的情况下，检查要穿戴的属性伙伴是否已拥有
    if((byOldBagStudyAttrKind != byOldHeroStudyAttrKind) && (0 != byOldBagStudyColorKind) && (poHero->CkHaveStudyAttrKind(stBagStudy.byStudyAttrKind1)))
    {
        return ERR_DRESS_STUDY::ID_EXIST_SAME_STUDY_ATTR_KIND;
    }

    //交换心得数据
    DT_STUDY_DATA stStudyTmp;
    memcpy(&stStudyTmp, pstHeroStudy, sizeof(DT_STUDY_DATA));
    memcpy(pstHeroStudy, &stBagStudy, sizeof(DT_STUDY_DATA));
    memcpy(&stBagStudy, &stStudyTmp, sizeof(DT_STUDY_DATA));
    if((0 == byOldHeroStudyColorKind) && (0 != byOldBagStudyColorKind)) //穿戴(原伙伴位置没心得，背包有心得，则交换后该背包位置无心得)
    {
        m_mapIdleStudyIdx[byBagGridIdx] = byBagGridIdx;
    }
    else if((0 == byOldBagStudyColorKind) && (0 != byOldHeroStudyColorKind)) //卸下(原伙伴位置有心得,背包没心得，则交换后该背包位置有心得)
    {
        m_mapIdleStudyIdx.erase(byBagGridIdx);
    }
    else {} //都有数据，交换

    GetBagStudyInfo(astBagStudyInfo);
    GetHeroStudyDataCli(poHero, stHeroStudyInfo);

    poHero->OnDressStudy();

    //重置玩家属性
    m_poOwner->ResetBattleAttr();

    m_poOwner->AddGuideRecord(EGF_AWAKEN);

    CTaskMgr::Instance()->OnStudy(m_poOwner);
    return ERR_DRESS_STUDY::ID_SUCCESS;
}


UINT16 CAwaken::OpenAwakenTab(DT_AWAKEN_TAB_DATA& stAwakenInfo)
{
    GetAwakenTabInfo(stAwakenInfo);

    return ERR_OPEN_AWAKEN::ID_SUCCESS;
}

VOID CAwaken::GetAwakenTabInfo(DT_AWAKEN_TAB_DATA& stAwakenInfo)
{
    memcpy(&stAwakenInfo.stBaseInfo, &m_stDT_AWAKEN_DATA.stBaseInfo, sizeof(stAwakenInfo.stBaseInfo));//注意sizeof及顺序
    GetBagStudyInfo(stAwakenInfo.astBagStudyInfo);
    stAwakenInfo.wMaxStudyLevel = CAwakenPropMgr::Instance()->GetMaxStudyLevel();
    CAwakenPropMgr::Instance()->GetAwakenCoin(stAwakenInfo.adwAwakenCoin);
    stAwakenInfo.dwAwakenGold = CAwakenPropMgr::Instance()->GetAwakenGold();
    stAwakenInfo.qwCurCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    stAwakenInfo.qwCurGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
    stAwakenInfo.wTatalCallNum = GetTotalCallNum();
    stAwakenInfo.wCanCallNum = GetCanCallNum();
}

VOID CAwaken::GetBagStudyInfo(DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM])
{
    for(UINT8 byIdx = 0; byIdx < MAX_BAG_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byIdx];
        GetDT_STUDY_DATA_CLI(stDT_STUDY_DATA, astBagStudyInfo[byIdx]);
    }
}

VOID CAwaken::GetDT_STUDY_DATA_CLI(DT_STUDY_DATA& stDT_STUDY_DATA, DT_STUDY_DATA_CLI& stDT_STUDY_DATA_CLI)
{
    memset(&stDT_STUDY_DATA_CLI, 0, sizeof(stDT_STUDY_DATA_CLI));
    if(0 == stDT_STUDY_DATA.byStudyColorKind)
    {
        return;
    }

    stDT_STUDY_DATA_CLI.byStudyColorKind = stDT_STUDY_DATA.byStudyColorKind;
    stDT_STUDY_DATA_CLI.byStudyAttrKind = stDT_STUDY_DATA.byStudyAttrKind1;
    stDT_STUDY_DATA_CLI.byStudyLevel = stDT_STUDY_DATA.byStudyLevel;
    stDT_STUDY_DATA_CLI.dwStudyAttrValue = CAwakenPropMgr::Instance()->GetStudyAttrValue(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyLevel);
    stDT_STUDY_DATA_CLI.dwStudyExp = CAwakenPropMgr::Instance()->GetStudyExp(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyLevel);;
    if(stDT_STUDY_DATA_CLI.byStudyLevel < CAwakenPropMgr::Instance()->GetMaxStudyLevel())
    {
        stDT_STUDY_DATA_CLI.dwNextLevelAttrValue = CAwakenPropMgr::Instance()->GetStudyAttrValue(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyLevel + 1);
        stDT_STUDY_DATA_CLI.dwUpgradeNeedExp = CAwakenPropMgr::Instance()->GetStudyExp(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyLevel + 1) - stDT_STUDY_DATA_CLI.dwStudyExp;
    }
}



UINT16 CAwaken::OpenDressStudyTab(DT_DRESS_STUDY_TAB_DATA& stDressStudyTabInfo)
{
    GetBagStudyInfo(stDressStudyTabInfo.astBagStudyInfo);
    CAwakenPropMgr::Instance()->GetwLevelStudyUnlockLevelInfo(stDressStudyTabInfo.awStudyUnlockLevelInfo);

    CKindID2HeroShmemMap& mapHero = m_poOwner->GetAllCarryHeroMap();
    stDressStudyTabInfo.byHeroNum = 0;
    CHero* poHero = mapHero.GetFistData();
    do
    {
        if(NULL == poHero)
        {
            break;
        }
        DT_HERO_STUDY_DATA_CLI& stDT_HERO_STUDY_DATA_CLI = stDressStudyTabInfo.astHeroStudyInfo[stDressStudyTabInfo.byHeroNum++];
        GetHeroStudyDataCli(poHero, stDT_HERO_STUDY_DATA_CLI);
    }
    while(poHero = mapHero.GetNextData());
    stDressStudyTabInfo.qwStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;
    stDressStudyTabInfo.qwCurCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    stDressStudyTabInfo.qwCurGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;

    return ERR_OPEN_DRESS_STUDY_TAB::ID_SUCCESS;
}


VOID CAwaken::GetHeroStudyDataCli(CHero* poHero, DT_HERO_STUDY_DATA_CLI& stDT_HERO_STUDY_DATA_CLI)
{
    if(NULL == poHero)
    {
        return;
    }

    stDT_HERO_STUDY_DATA_CLI.wHeroID = poHero->GetDT_HERO_BASE_DATA().wKindID;
    for(UINT8 byIdx = 0; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA* pstDT_STUDY_DATA = poHero->GetStudy(byIdx);
        GetDT_STUDY_DATA_CLI(*pstDT_STUDY_DATA, stDT_HERO_STUDY_DATA_CLI.astStudyInfo[byIdx]);
    }
}


VOID CAwaken::AddStudyForGm(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel, UINT8 byStudyAttrKind2, string strDesc)
{
    if(0 == byStudyColorKind || byStudyColorKind > EC_ORANGE)
    {
        return;
    }
    //太极无属性，现在双属性
    //if(EBAK_INVALID == byStudyAttrKind || byStudyAttrKind >= EBAK_MAX)
    //{
    //	return;
    //}
    if(0 == byStudyLevel || byStudyLevel > CAwakenPropMgr::Instance()->GetMaxStudyLevel())
    {
        return;
    }

    C88MapItr itr = m_mapIdleStudyIdx.begin();
    if(itr == m_mapIdleStudyIdx.end())
    {
        return;
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first];
    memset(&stDT_STUDY_DATA, 0, sizeof(DT_STUDY_DATA));

    stDT_STUDY_DATA.byStudyColorKind	= byStudyColorKind;
    stDT_STUDY_DATA.byStudyAttrKind1	= byStudyAttrKind;
    stDT_STUDY_DATA.byStudyAttrKind2	= byStudyAttrKind2;
    stDT_STUDY_DATA.byStudyLevel		= byStudyLevel;

    if( EC_RED == stDT_STUDY_DATA.byStudyColorKind || EC_ORANGE == stDT_STUDY_DATA.byStudyColorKind )
    {
        CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDY, CRecordMgr::EASTUDYT_GMCOMMAND, stDT_STUDY_DATA.byStudyColorKind, 0,
                                            m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel, 0, strDesc);
    }

    m_mapIdleStudyIdx.erase(itr);
}


VOID CAwaken::AddStudyExpForGm(UINT32 dwStudyExp)
{
    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp += dwStudyExp;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYEXP, CRecordMgr::EASET_GMCOMMAND, dwStudyExp, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
}

//获取元宝召唤次数
UINT16 CAwaken::GetCanCallNum()
{
    UINT16 wTatol = GetTotalCallNum();
    UINT16 wHaveCallNum = m_stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum;
    if(IsToday(m_stDT_AWAKEN_DATA.stBaseInfo.qwlastCallTime))
    {
        return wTatol > wHaveCallNum ? wTatol - wHaveCallNum : 0;
    }
    else
    {
        m_stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum = 0;
        return wTatol;
    }
}

//获取元宝召唤次数
UINT16 CAwaken::GetTotalCallNum()
{
    return CVipPropMgr::Instance()->GetIncNum(EVINF_AWAKENCALL, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
}



//悟道
UINT16 CAwaken::Awaken2(UINT8 byQuickFlag, UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM], DT_AWAKEN_TAB_DATA2& stAwakenInfo)
{
    UINT16 wErrCode = 0;
    byAwakenNum = 0;
    if(byQuickFlag)
    {
        wErrCode = QuickAwaken2(byAwakenNum, astAwakenGetInfo);
    }
    else
    {
        wErrCode = AwakenOnce2(byGoldFlag, byAwakenNum, astAwakenGetInfo);
    }

    GetAwakenTabInfo2(stAwakenInfo);
    return wErrCode;
}

// 悟道心得
VOID CAwaken::AwakenStudy2(DT_AWAKEN_GET_DATA2& stAwakenGetData)
{
    memset(&stAwakenGetData, 0, sizeof(stAwakenGetData));
    DT_AWAKEN_BASE_DATA& stBaseInfo = m_stDT_AWAKEN_DATA.stBaseInfo;
    //元宝悟道在入定/彻悟状态时使用不同的概率，则这两个状态的元宝悟道标记不能清除
    if((1 == stBaseInfo.byGoldFlag) && ((EALK_RUDING != stBaseInfo.byLevel) && (EALK_CHEWU != stBaseInfo.byLevel)))
    {
        stBaseInfo.byGoldFlag = 0;
    }

    C88MapItr itr = m_mapIdleStudyIdx.begin();
    if(itr == m_mapIdleStudyIdx.end())
    {
        //异常..
        return;
    }

    DT_STUDY_DATA* pstDT_STUDY_DATA = &m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first];
    memset(pstDT_STUDY_DATA, 0, sizeof(DT_STUDY_DATA));
    pstDT_STUDY_DATA->byStudyLevel = 1;//生成的心得为1级
    UINT32 dwGenCoinOnce = 0;
    UINT32 dwGenClipOnce = 0;
    BOOL bOpenExpStudy = NULL == m_poOwner->GetBuild(EBK_STUDYEXCHANGE) ? FALSE : TRUE;
	//ZTS
	if ( 5 == stBaseInfo.byLevel)
	{
		CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->AddZTSNum();
			poPlayerMedal->CheckMedalBySmallType(EMST_AWAKEN_ZTS);
		}
	}
    stBaseInfo.byLevel = CAwakenPropMgr2::Instance()->AwakenStudy(m_poOwner->GetLevel(), bOpenExpStudy, stBaseInfo.byLevel, stBaseInfo.byGoldFlag,
                         pstDT_STUDY_DATA->byStudyColorKind, pstDT_STUDY_DATA->byStudyAttrKind1, pstDT_STUDY_DATA->byStudyAttrKind2, dwGenCoinOnce, 
						 dwGenClipOnce, m_stDT_AWAKEN_DATA.byZTSUseTime);
    if(0 != pstDT_STUDY_DATA->byStudyColorKind) //有生成心得
    {
        //记录高级别的心得
        if( EC_RED == pstDT_STUDY_DATA->byStudyColorKind || EC_ORANGE == pstDT_STUDY_DATA->byStudyColorKind )
        {
			if ( EC_ORANGE == pstDT_STUDY_DATA->byStudyColorKind )
			{
				CFewParam& oFewParam = m_poOwner->GetFewParam();
				oFewParam.AddAwakenNum();
			}
            CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDY, CRecordMgr::EASTUDYT_AWAKEN, pstDT_STUDY_DATA->byStudyColorKind, 0,
                                                m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, pstDT_STUDY_DATA->byStudyAttrKind1, pstDT_STUDY_DATA->byStudyAttrKind2, pstDT_STUDY_DATA->byStudyLevel);
        }

        //记录获得红色卦象个数
        if (EC_RED == pstDT_STUDY_DATA->byStudyColorKind)
        {
            m_poOwner->CkToday();
            m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddRedStudyNum++;
        }
        stAwakenGetData.byStudyBagIdx = itr->first;
        m_mapIdleStudyIdx.erase(itr);
    }
    else
    {
        pstDT_STUDY_DATA = NULL;
        if(0 != dwGenCoinOnce)
        {
            m_poOwner->AddCoin(dwGenCoinOnce, CRecordMgr::EACT_AWAKEN);
            stAwakenGetData.dwGetCoin = dwGenCoinOnce;
        }
        if(0 != dwGenClipOnce)
        {
            stBaseInfo.qwStudyClip += dwGenClipOnce;
            stAwakenGetData.dwGetStudyClip = dwGenClipOnce;

            CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYCLIP, CRecordMgr::EASCTUDYTCLIP_AWAKEN, dwGenClipOnce, stBaseInfo.qwStudyClip,
                m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
        }
    }
    if(0 == stBaseInfo.byLevel)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Player[%d] Awaken Level is 0!"), MSG_MARK, m_poOwner->GetID());
        stBaseInfo.byLevel = EALK_KESHUI;
    }

    stAwakenGetData.byAwakenLevel = stBaseInfo.byLevel;

}


UINT16 CAwaken::QuickAwaken2(UINT8& byAwakenNum, DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM])
{
    UINT16 wErrCode = ERR_AWAKEN::ID_SUCCESS;
    DT_AWAKEN_BASE_DATA& stBaseInfo = m_stDT_AWAKEN_DATA.stBaseInfo;

    //入定/彻悟状态可一键悟道
    //if(((1 == stBaseInfo.byGoldFlag) && ((EALK_RUDING == stBaseInfo.byLevel) || (EALK_CHEWU == stBaseInfo.byLevel)))
    //	|| (EALK_CHEWU == stBaseInfo.byLevel))
    //{
    //	return ERR_AWAKEN::ID_INVALID_STATE;
    //}

    // 最大循环MAX_AWAKEN_NUM次，避免死循环
    for(UINT16 wIdx = 0; ((wIdx < MAX_AWAKEN_NUM) && (m_mapIdleStudyIdx.size() > 0)); wIdx++)
    {
        wErrCode = AwakenOnce2(0, byAwakenNum, astAwakenGetInfo);
        if((ERR_AWAKEN::ID_SUCCESS != wErrCode) || (EALK_CHEWU == stBaseInfo.byLevel)) //悟道错误或彻悟状态则退出悟道
        {
            break;
        }
    }

    //铜钱不足算成功
    if(ERR_COMMON::ID_COIN_NOT_ENOUGH == wErrCode)
    {
        wErrCode = ERR_AWAKEN::ID_SUCCESS;
    }

    return wErrCode;
}


UINT16 CAwaken::AwakenOnce2(UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM])
{
    if(byGoldFlag)
    {
        DT_AWAKEN_BASE_DATA& stBaseInfo = m_stDT_AWAKEN_DATA.stBaseInfo;
        //已经是入定状态
        if(EALK_RUDING == stBaseInfo.byLevel)
        {
            return ERR_AWAKEN::ID_SUCCESS;
        }

        if(0 == GetCanCallNum2())
        {
            return ERR_AWAKEN::ID_REACH_CALL_NUM;
        }

        //判断元宝
        UINT32 dwCostGold = CAwakenPropMgr2::Instance()->GetAwakenGold();
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }

        //判断心得碎片格子是否已满
        C88MapItr itr = m_mapIdleStudyIdx.begin();
        if(itr == m_mapIdleStudyIdx.end())
        {
            return ERR_AWAKEN::ID_STUDY_BAG_FULL;
        }

        //扣除元宝
        m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_AWAKENONCE);

        stBaseInfo.byLevel = EALK_RUDING;
        stBaseInfo.byGoldFlag = 1;
        stBaseInfo.wHaveCallNum++;
        stBaseInfo.qwlastCallTime = SDTimeSecs();

        //赠送心得
        SetCallStudyInfo2(&(m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first]));
        DT_AWAKEN_GET_DATA2& stDT_AWAKEN_GET_DATA = astAwakenGetInfo[byAwakenNum++];
        stDT_AWAKEN_GET_DATA.byAwakenLevel = stBaseInfo.byLevel;
        stDT_AWAKEN_GET_DATA.byStudyBagIdx = itr->first;
        stDT_AWAKEN_GET_DATA.dwGetCoin = 0;
        stDT_AWAKEN_GET_DATA.dwGetStudyClip = 0;

        DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first];
        //记录获得太极的个数
        if (stDT_STUDY_DATA.byStudyColorKind == EC_ORANGE && stDT_STUDY_DATA.byStudyAttrKind1 == 0)
        {
            m_poOwner->CkToday();
            m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddTaijiStudyNum++;
        }
        m_mapIdleStudyIdx.erase(itr);

        return ERR_AWAKEN::ID_SUCCESS;
    }
    else
    {
        //判断心得碎片格子是否已满
        if(0 == m_mapIdleStudyIdx.size())
        {
            return ERR_AWAKEN::ID_STUDY_BAG_FULL;
        }

        //判断铜币
        UINT32 dwCostCoin = CAwakenPropMgr2::Instance()->GetAwakenCoin(m_poOwner->GetLevel(), m_stDT_AWAKEN_DATA.stBaseInfo.byGoldFlag, m_stDT_AWAKEN_DATA.stBaseInfo.byLevel);
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostCoin)
        {
            return ERR_COMMON::ID_COIN_NOT_ENOUGH;
        }

        //扣除铜钱
        m_poOwner->DecCoin(dwCostCoin, CRecordMgr::EDCT_AWAKEN, m_stDT_AWAKEN_DATA.stBaseInfo.byLevel);

        AwakenStudy2(astAwakenGetInfo[byAwakenNum++]);
    }

    return ERR_AWAKEN::ID_SUCCESS;
}

//转换心得经验
UINT16 CAwaken::ConvertExp2(UINT8 byGridIdx, UINT8 byAll, UINT8 abySelectConvertColor[MAX_COLOR_NUM], UINT32& dwGenStudyExp, DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], UINT64& qwCurStudyExp)
{
    UINT16 wErrCode = ERR_STUDY_TO_EXP::ID_SUCCESS;
    dwGenStudyExp = 0;
    if(1 == byAll)
    {
        for(byGridIdx = 0; byGridIdx < MAX_BAG_STUDY_GRID_NUM; byGridIdx++)
        {
            if(m_mapIdleStudyIdx.find(byGridIdx) == m_mapIdleStudyIdx.end())
            {
                DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byGridIdx];
                if(0 == stDT_STUDY_DATA.byStudyAttrKind1)
                {
                    // 不能穿戴的卦象直接转掉
                }
                else if((0 == stDT_STUDY_DATA.byStudyColorKind) || (stDT_STUDY_DATA.byStudyColorKind > MAX_COLOR_NUM) || (0 == abySelectConvertColor[stDT_STUDY_DATA.byStudyColorKind - 1]))
                {
                    continue;
                }
                wErrCode = ConvertStudyExp2(byGridIdx, dwGenStudyExp);
                if(wErrCode != ERR_STUDY_TO_EXP::ID_SUCCESS)
                {
                    break;
                }
            }
        }
    }
    else
    {
        wErrCode = ConvertStudyExp2(byGridIdx, dwGenStudyExp);
    }

    GetBagStudyInfo2(astBagStudyInfo);
    qwCurStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;

    return wErrCode;
}

UINT16 CAwaken::ConvertStudyExp2(UINT8 byGridIdx, UINT32& dwGetExp, string strDesc)
{
    if(byGridIdx >= MAX_BAG_STUDY_GRID_NUM)
    {
        return ERR_STUDY_TO_EXP::ID_IDX_INVALID;
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byGridIdx];
    if(0 == stDT_STUDY_DATA.byStudyColorKind)
    {
        return ERR_STUDY_TO_EXP::ID_STUDY_NOT_EXIST;
    }
    UINT32 dwExp = CAwakenPropMgr2::Instance()->GetStudyExp(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel);
    dwGetExp += dwExp;
    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp += dwExp;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYEXP, CRecordMgr::EASET_CONVERTSTUDYEXP, dwExp, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp,
                                        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel, 0, strDesc);

    //if( EC_RED==stDT_STUDY_DATA.byStudyColorKind || EC_ORANGE==stDT_STUDY_DATA.byStudyColorKind )
    //{
    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECSTUDY, CRecordMgr::EDSTUDYT_CONVERT, stDT_STUDY_DATA.byStudyColorKind, 0,
                                        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel, 0, strDesc);
    //}
    m_mapIdleStudyIdx[byGridIdx] = byGridIdx;
    memset(&stDT_STUDY_DATA, 0, sizeof(stDT_STUDY_DATA));

    return ERR_STUDY_TO_EXP::ID_SUCCESS;
}


//升级心得
UINT16 CAwaken::UpgradeStudy2(UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byToLevel, DT_HERO_STUDY_DATA_CLI2& stHeroStudyInfo, UINT64& qwCurStudyExp)
{
    if(byGridIdx >= MAX_HERO_STUDY_GRID_NUM)
    {
        return ERR_UPGRADE_STUDY::ID_IDX_INVALID;
    }

    CHero* poHero = m_poOwner->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_UPGRADE_STUDY::ID_HERO_NOT_EXIST;
    }

    DT_STUDY_DATA* pstDT_STUDY_DATA = poHero->GetStudy(byGridIdx);
    if(NULL == pstDT_STUDY_DATA)
    {
        RETURN_OTHER_ERR;
    }
    if(0 == pstDT_STUDY_DATA->byStudyColorKind)
    {
        return ERR_UPGRADE_STUDY::ID_STUDY_NOT_EXIST;
    }
    if(pstDT_STUDY_DATA->byStudyLevel >= byToLevel)
    {
        GetHeroStudyDataCli2(poHero, stHeroStudyInfo);
        qwCurStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;
        return ERR_UPGRADE_STUDY::ID_SUCCESS;
    }
    if(byToLevel != pstDT_STUDY_DATA->byStudyLevel + 1)
    {
        return ERR_UPGRADE_STUDY::ID_CANNOT_JMP_LEVEL;
    }

    UINT32 dwCostExp = CAwakenPropMgr2::Instance()->GetStudyUpgradeNeedExp(pstDT_STUDY_DATA->byStudyColorKind, pstDT_STUDY_DATA->byStudyAttrKind1, pstDT_STUDY_DATA->byStudyAttrKind2, pstDT_STUDY_DATA->byStudyLevel);
    if(m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp < dwCostExp)
    {
        return ERR_UPGRADE_STUDY::ID_NOT_ENOUGH_STUDYEXP;
    }

    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp -= dwCostExp;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECSTUDYEXP, CRecordMgr::EDSET_UPGRADESTUDY, dwCostExp, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);

    pstDT_STUDY_DATA->byStudyLevel++;

    GetHeroStudyDataCli2(poHero, stHeroStudyInfo);
    qwCurStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;

    poHero->OnUpgradeStudy();

    //重置玩家属性
    m_poOwner->ResetBattleAttr();

    CTaskMgr::Instance()->OnStudy(m_poOwner);
    return ERR_UPGRADE_STUDY::ID_SUCCESS;
}

UINT16 CAwaken::DressStudy2(UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx, DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], DT_HERO_STUDY_DATA_CLI2& stHeroStudyInfo, UINT8& byHaveSecondsUndress, UINT8& byBagGridIdx2, UINT8& byHeroGridIdx2)
{
    byHaveSecondsUndress = 0;
    byBagGridIdx2 = 0;
    byHeroGridIdx2 = 0;

    if((byHeroGridIdx >= MAX_HERO_STUDY_GRID_NUM) || (byBagGridIdx >= MAX_BAG_STUDY_GRID_NUM))
    {
        return ERR_DRESS_STUDY::ID_IDX_INVALID;
    }

    CHero* poHero = m_poOwner->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_DRESS_STUDY::ID_HERO_NOT_EXIST;
    }

    DT_STUDY_DATA* pstHeroStudy = poHero->GetStudy(byHeroGridIdx);
    if(NULL == pstHeroStudy)
    {
        RETURN_OTHER_ERR;
    }

    DT_STUDY_DATA& stBagStudy = m_stDT_AWAKEN_DATA.astBagStudyInfo[byBagGridIdx];
    if(m_poOwner->CKGuide(EGF_AWAKEN)) //引导,给1级蓝色生命心得，悟道等级2
    {
        m_stDT_AWAKEN_DATA.stBaseInfo.byLevel = EALK_SHENGYOU;

        stBagStudy.byStudyColorKind = EC_BLUE;
        stBagStudy.byStudyAttrKind1 = EBAK_HP;
        stBagStudy.byStudyAttrKind2 = 0;
        stBagStudy.byStudyLevel = 1;
    }
    UINT8 byOldBagStudyColorKind = stBagStudy.byStudyColorKind;
    UINT8 byOldHeroStudyColorKind = pstHeroStudy->byStudyColorKind;
    UINT8 byOldBagStudyAttrKind1 = stBagStudy.byStudyAttrKind1;
    UINT8 byOldBagStudyAttrKind2 = stBagStudy.byStudyAttrKind2;
    UINT8 byOldHeroStudyAttrKind1 = pstHeroStudy->byStudyAttrKind1;
    UINT8 byOldHeroStudyAttrKind2 = pstHeroStudy->byStudyAttrKind2;
    if((0 == byOldBagStudyColorKind) && (0 == byOldHeroStudyColorKind))
    {
        return ERR_DRESS_STUDY::ID_STUDY_NOT_EXIST;
    }

    //不可穿戴卦象
    if((0 == byOldBagStudyAttrKind1) && (0 != byOldBagStudyColorKind))
    {
        return ERR_DRESS_STUDY::ID_CANNOT_DRESS_THIS_STUDY;
    }

    //未解锁可卸载，不可穿戴
    if((0 == byOldHeroStudyColorKind) && (!CAwakenPropMgr2::Instance()->CkUnlockHeroGrid(poHero->GetDT_HERO_BASE_DATA().wLevel, byHeroGridIdx)))
    {
        return ERR_DRESS_STUDY::ID_POS_NOT_UNLOCK;
    }

    DT_STUDY_DATA* pstDownHeroStudy2 = NULL; //需卸载下的伙伴的第二个卦象
    //属性完全一样，直接替换
    if((byOldBagStudyAttrKind1 == byOldHeroStudyAttrKind1) && (byOldBagStudyAttrKind2 == byOldHeroStudyAttrKind2))
    {
        pstDownHeroStudy2 = NULL;
    }
    else
    {
        //替换上去的只有1个属性
        if(((0 != byOldBagStudyAttrKind1) && (0 == byOldBagStudyAttrKind2))
                || ((0 == byOldBagStudyAttrKind1) && (0 != byOldBagStudyAttrKind2))
          )
        {
            //不同属性(完全相同属性前面已经判断)， 检查要穿戴的属性伙伴是否已拥有. 不存在则交换
            UINT8 byAttrGridIdx1 = 0;
            UINT8 byAttrGridIdx2 = 0;
            BOOL bHaveAttr1 = FALSE;
            BOOL bHaveAttr2 = FALSE;
            if(byOldBagStudyAttrKind1)
            {
                bHaveAttr1 = poHero->GetStudyAttrGrid(byOldBagStudyAttrKind1, byAttrGridIdx1);
            }
            if(byOldBagStudyAttrKind2)
            {
                bHaveAttr2 = poHero->GetStudyAttrGrid(byOldBagStudyAttrKind2, byAttrGridIdx2);
            }
            if((bHaveAttr1 && byAttrGridIdx1 != byHeroGridIdx) || (bHaveAttr2 && byAttrGridIdx2 != byHeroGridIdx))
            {
                return ERR_DRESS_STUDY::ID_EXIST_SAME_STUDY_ATTR_KIND;
            }
        }
        //替换上去的有2个属性
        else if((0 != byOldBagStudyAttrKind1) && (0 != byOldBagStudyAttrKind2)
               )
        {
            //完全相同之前已经判断
            //判断替换的位置是否存在
            UINT8 byGridIdx1 = 0;
            UINT8 byGridIdx2 = 0;
            BOOL bHaveAttr1 = poHero->GetStudyAttrGrid(byOldBagStudyAttrKind1, byGridIdx1);
            BOOL bHaveAttr2 = poHero->GetStudyAttrGrid(byOldBagStudyAttrKind2, byGridIdx2);
            //不能存在其他位置有两个相同属性的卦象，其中1个相同的必是byHeroGridIdx，因为客户端之间已经判断1个相同的
            if(bHaveAttr1 && bHaveAttr2)
            {
                if((byGridIdx1 != byHeroGridIdx) && (byGridIdx2 != byHeroGridIdx))
                {
                    return ERR_DRESS_STUDY::ID_EXIST_SAME_STUDY_ATTR_KIND;
                }
                else if(byGridIdx1 == byHeroGridIdx)
                {
                    pstDownHeroStudy2 = poHero->GetStudy(byGridIdx2);
                    byHeroGridIdx2 = byGridIdx2;
                }
                else
                {
                    pstDownHeroStudy2 = poHero->GetStudy(byGridIdx1);
                    byHeroGridIdx2 = byGridIdx1;
                }
            }
            //有1个相同，则判断是否是请求的位置，若不是则请求错误
            else if(bHaveAttr1 || bHaveAttr2)
            {
                if((bHaveAttr1) && (byGridIdx1 != byHeroGridIdx))
                {
                    return ERR_DRESS_STUDY::ID_EXIST_SAME_STUDY_ATTR_KIND;
                }
                else if((bHaveAttr2) && (byGridIdx2 != byHeroGridIdx))
                {
                    return ERR_DRESS_STUDY::ID_EXIST_SAME_STUDY_ATTR_KIND;
                }
            }
        }
        //卸载，没有属性，
        else
        {

        };
    }


    DT_STUDY_DATA stStudyTmp;
    //先替换第二个卦象(若存在)
    if(pstDownHeroStudy2)
    {
        if(0 == m_mapIdleStudyIdx.size())
        {
            return ERR_DRESS_STUDY::ID_STUDY_BAG_FULL;
        }
        byHaveSecondsUndress = 1;
        byBagGridIdx2 = m_mapIdleStudyIdx.begin()->first;
        DT_STUDY_DATA& stIdelBagStudy = m_stDT_AWAKEN_DATA.astBagStudyInfo[byBagGridIdx2];
        memcpy(&stStudyTmp, pstDownHeroStudy2, sizeof(DT_STUDY_DATA));
        memcpy(pstDownHeroStudy2, &stIdelBagStudy, sizeof(DT_STUDY_DATA));
        memcpy(&stIdelBagStudy, &stStudyTmp, sizeof(DT_STUDY_DATA));
        m_mapIdleStudyIdx.erase(byBagGridIdx2);
    }

    //交换心得数据
    memcpy(&stStudyTmp, pstHeroStudy, sizeof(DT_STUDY_DATA));
    memcpy(pstHeroStudy, &stBagStudy, sizeof(DT_STUDY_DATA));
    memcpy(&stBagStudy, &stStudyTmp, sizeof(DT_STUDY_DATA));
    if((0 == byOldHeroStudyColorKind) && (0 != byOldBagStudyColorKind)) //穿戴(原伙伴位置没心得，背包有心得，则交换后该背包位置无心得)
    {
        m_mapIdleStudyIdx[byBagGridIdx] = byBagGridIdx;
    }
    else if((0 == byOldBagStudyColorKind) && (0 != byOldHeroStudyColorKind)) //卸下(原伙伴位置有心得,背包没心得，则交换后该背包位置有心得)
    {
        m_mapIdleStudyIdx.erase(byBagGridIdx);
    }
    else
    {

    } //都有数据，交换



    GetBagStudyInfo2(astBagStudyInfo);
    GetHeroStudyDataCli2(poHero, stHeroStudyInfo);

    poHero->OnDressStudy();

    //重置玩家属性
    m_poOwner->ResetBattleAttr();

    m_poOwner->AddGuideRecord(EGF_AWAKEN);

    CTaskMgr::Instance()->OnStudy(m_poOwner);
    return ERR_DRESS_STUDY::ID_SUCCESS;
}


////////////////////////////////////////////////// 222222222222222222222222 //////////////////////////////////////////////////
////////////////////////////////////////////////// 222222222222222222222222 //////////////////////////////////////////////////


UINT16 CAwaken::OpenAwakenTab2(PKT_CLIGS_OPEN_AWAKEN_TAB_ACK2& stAwakenInfo)
{
    GetAwakenTabInfo2(stAwakenInfo.stAwakenInfo);

    if(m_poOwner->GetBuild(EBK_STUDYEXCHANGE))
    {
        GetIfResolve(stAwakenInfo.bySingleResolve,stAwakenInfo.byDoubleResolve);
    }
    return ERR_OPEN_AWAKEN::ID_SUCCESS;
}

VOID CAwaken::GetAwakenTabInfo2(DT_AWAKEN_TAB_DATA2& stAwakenInfo)
{
    memset(&stAwakenInfo, 0, sizeof(stAwakenInfo));
    memcpy(&stAwakenInfo.stBaseInfo, &m_stDT_AWAKEN_DATA.stBaseInfo, sizeof(stAwakenInfo.stBaseInfo));
    GetBagStudyInfo2(stAwakenInfo.astBagStudyInfo);
    stAwakenInfo.wMaxStudyLevel = CAwakenPropMgr2::Instance()->GetMaxStudyLevel();
    CAwakenPropMgr2::Instance()->GetAwakenCoin(stAwakenInfo.adwAwakenCoin);
    stAwakenInfo.dwAwakenGold = CAwakenPropMgr2::Instance()->GetAwakenGold();
    stAwakenInfo.qwCurCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    stAwakenInfo.qwCurGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
    stAwakenInfo.wTatalCallNum = GetTotalCallNum2();
    stAwakenInfo.wCanCallNum = GetCanCallNum2();
    GetCallStudyInfo2(stAwakenInfo.stCallStudy);
}

VOID CAwaken::GetBagStudyInfo2(DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM])
{
    for(UINT8 byIdx = 0; byIdx < MAX_BAG_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byIdx];
        GetDT_STUDY_DATA_CLI2(stDT_STUDY_DATA, astBagStudyInfo[byIdx]);
    }
}

VOID CAwaken::GetBagStudyInfo2NotDouble(DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM])
{
	for(UINT8 byIdx = 0; byIdx < MAX_BAG_STUDY_GRID_NUM; byIdx++)
	{
		DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byIdx];
		if ( 0 != stDT_STUDY_DATA.byStudyColorKind && 0 == stDT_STUDY_DATA.byStudyAttrKind2 && 0 != stDT_STUDY_DATA.byStudyAttrKind1)
		{
			GetDT_STUDY_DATA_CLI2(stDT_STUDY_DATA, astBagStudyInfo[byIdx]);
		}
	}
}

VOID CAwaken::GetCallStudyInfo2(DT_STUDY_DATA_CLI2& stCallStudy)
{
    DT_STUDY_DATA stDT_STUDY_DATA = {0};
    SetCallStudyInfo2(&stDT_STUDY_DATA);
    GetDT_STUDY_DATA_CLI2(stDT_STUDY_DATA, stCallStudy);
}

VOID CAwaken::SetCallStudyInfo2(DT_STUDY_DATA* pstDT_STUDY_DATA)
{
    if(NULL == pstDT_STUDY_DATA)
    {
        return;
    }
    SAwakenBaseProp2& stProp = CAwakenPropMgr2::Instance()->GetAwakenBaseProp();
    pstDT_STUDY_DATA->byStudyColorKind = stProp.byCallStudyColorKind;
    pstDT_STUDY_DATA->byStudyAttrKind1 = stProp.byCallStudyAttrKind1;
    pstDT_STUDY_DATA->byStudyAttrKind2 = stProp.byCallStudyAttrKind2;
    pstDT_STUDY_DATA->byStudyLevel = stProp.byCallStudyLevel;
}



VOID CAwaken::GetDT_STUDY_DATA_CLI2(DT_STUDY_DATA& stDT_STUDY_DATA, DT_STUDY_DATA_CLI2& stDT_STUDY_DATA_CLI)
{
    memset(&stDT_STUDY_DATA_CLI, 0, sizeof(stDT_STUDY_DATA_CLI));
    if(0 == stDT_STUDY_DATA.byStudyColorKind)
    {
        return;
    }

    stDT_STUDY_DATA_CLI.byStudyColorKind = stDT_STUDY_DATA.byStudyColorKind;
    stDT_STUDY_DATA_CLI.byStudyAttrKind1 = stDT_STUDY_DATA.byStudyAttrKind1;
    stDT_STUDY_DATA_CLI.byStudyAttrKind2 = stDT_STUDY_DATA.byStudyAttrKind2;
    stDT_STUDY_DATA_CLI.byStudyLevel = stDT_STUDY_DATA.byStudyLevel;
    CAwakenPropMgr2::Instance()->GetStudyAttrValue(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2,
            stDT_STUDY_DATA.byStudyLevel, stDT_STUDY_DATA_CLI.dwStudyAttrValue1, stDT_STUDY_DATA_CLI.dwStudyAttrValue2);
    stDT_STUDY_DATA_CLI.dwStudyExp = CAwakenPropMgr2::Instance()->GetStudyExp(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel);;
    if(stDT_STUDY_DATA_CLI.byStudyLevel < CAwakenPropMgr2::Instance()->GetMaxStudyLevel())
    {
        CAwakenPropMgr2::Instance()->GetStudyAttrValue(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel + 1,
                stDT_STUDY_DATA_CLI.dwNextLevelAttrValue1, stDT_STUDY_DATA_CLI.dwNextLevelAttrValue2);
        stDT_STUDY_DATA_CLI.dwUpgradeNeedExp = CAwakenPropMgr2::Instance()->GetStudyExp(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel + 1) - stDT_STUDY_DATA_CLI.dwStudyExp;
    }
}



UINT16 CAwaken::OpenDressStudyTab2(DT_DRESS_STUDY_TAB_DATA2& stDressStudyTabInfo)
{
    GetBagStudyInfo2(stDressStudyTabInfo.astBagStudyInfo);
    CAwakenPropMgr2::Instance()->GetwLevelStudyUnlockLevelInfo(stDressStudyTabInfo.awStudyUnlockLevelInfo);

    CKindID2HeroShmemMap& mapHero = m_poOwner->GetAllCarryHeroMap();
    stDressStudyTabInfo.byHeroNum = 0;
    CHero* poHero = mapHero.GetFistData();
    do
    {
        if(NULL == poHero)
        {
            break;
        }
        DT_HERO_STUDY_DATA_CLI2& stDT_HERO_STUDY_DATA_CLI = stDressStudyTabInfo.astHeroStudyInfo[stDressStudyTabInfo.byHeroNum++];
        GetHeroStudyDataCli2(poHero, stDT_HERO_STUDY_DATA_CLI);
    }
    while((poHero = mapHero.GetNextData()) && stDressStudyTabInfo.byHeroNum < MAX_FORMATION_IDX_NUM );
    stDressStudyTabInfo.qwStudyExp = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;
    stDressStudyTabInfo.qwCurCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    stDressStudyTabInfo.qwCurGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;

    return ERR_OPEN_DRESS_STUDY_TAB::ID_SUCCESS;
}


VOID CAwaken::GetHeroStudyDataCli2(CHero* poHero, DT_HERO_STUDY_DATA_CLI2& stDT_HERO_STUDY_DATA_CLI)
{
    if(NULL == poHero)
    {
        return;
    }

    stDT_HERO_STUDY_DATA_CLI.wHeroID = poHero->GetDT_HERO_BASE_DATA().wKindID;
    for(UINT8 byIdx = 0; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA* pstDT_STUDY_DATA = poHero->GetStudy(byIdx);
        GetDT_STUDY_DATA_CLI2(*pstDT_STUDY_DATA, stDT_HERO_STUDY_DATA_CLI.astStudyInfo[byIdx]);
    }
}


VOID CAwaken::AddStudyForGm2(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel)
{
    if(0 == byStudyColorKind || byStudyColorKind > EC_ORANGE)
    {
        return;
    }
    if(EBAK_INVALID == byStudyAttrKind || byStudyAttrKind >= EBAK_MAX)
    {
        return;
    }
    if(0 == byStudyLevel || byStudyLevel > CAwakenPropMgr2::Instance()->GetMaxStudyLevel())
    {
        return;
    }

    C88MapItr itr = m_mapIdleStudyIdx.begin();
    if(itr == m_mapIdleStudyIdx.end())
    {
        return;
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first];
    memset(&stDT_STUDY_DATA, 0, sizeof(DT_STUDY_DATA));

    stDT_STUDY_DATA.byStudyColorKind = byStudyColorKind;
    stDT_STUDY_DATA.byStudyAttrKind1 = byStudyAttrKind;
    stDT_STUDY_DATA.byStudyAttrKind2 = 0;
    stDT_STUDY_DATA.byStudyLevel = byStudyLevel;

    if( EC_RED == stDT_STUDY_DATA.byStudyColorKind || EC_ORANGE == stDT_STUDY_DATA.byStudyColorKind )
    {
        CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDY, CRecordMgr::EASTUDYT_GMCOMMAND, stDT_STUDY_DATA.byStudyColorKind, 0,
                                            m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyAttrKind2, stDT_STUDY_DATA.byStudyLevel);
    }

    m_mapIdleStudyIdx.erase(itr);
}


VOID CAwaken::AddStudyExpForGm2(UINT32 dwStudyExp)
{
    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp += dwStudyExp;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYEXP, CRecordMgr::EASET_GMCOMMAND, dwStudyExp, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
}

VOID  CAwaken::AddStudyClip(UINT32 dwClip, UINT16 wOpType)
{
    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip += dwClip;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYCLIP, wOpType, dwClip, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
}

VOID	 CAwaken::DecStudyClipForGm(UINT32 dwClip)
{
    m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip > dwClip ? m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip - dwClip : 0;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECSTUDYCLIP, CRecordMgr::EDSCTUDYTCLIP_CONVERT, dwClip, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
}

UINT16 CAwaken::DelStudyForGM(UINT8 byGridIdx, string strDesc)
{
    UINT32 dwGetExp = 0;
    return ConvertStudyExp2(byGridIdx, dwGetExp, strDesc);
}

//获取元宝召唤次数
UINT16 CAwaken::GetCanCallNum2()
{
    UINT16 wTatol = GetTotalCallNum2();
    UINT16 wHaveCallNum = m_stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum;
    if(IsToday(m_stDT_AWAKEN_DATA.stBaseInfo.qwlastCallTime))
    {
        return wTatol > wHaveCallNum ? wTatol - wHaveCallNum : 0;
    }
    else
    {
        m_stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum = 0;
        return wTatol;
    }
}

//获取元宝召唤次数
UINT16 CAwaken::GetTotalCallNum2()
{
    return CVipPropMgr::Instance()->GetIncNum(EVINF_AWAKENCALL, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
}

UINT16 CAwaken::OpenAwakenExchangeTab(UINT8& byExchangeNum, DT_AWAKEN_STUDY_EXCHANGE_DATA astStudyExchangeInfo[MAX_EXCHANGE_STUDY_NUM], UINT64& qwCurCoin, UINT64& qwCurStudyClip)
{
    if(!m_poOwner->GetBuild(EBK_STUDYEXCHANGE))
    {
        return ERR_OPEN_AWAKEN_EXCHANGE_TAB::ID_STUDY_EXCHANGE_UNLOCK;
    }
    CAwakenClipToStudyPropVec2& vecProp = CAwakenPropMgr2::Instance()->GetAwakenClipToStudyPropVec();
    UINT8 bySize = vecProp.size();
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    BOOL bGetNextOpen = FALSE;
    UINT16 wLastNeedPlayerLevel = 0;
    for(UINT8 byIdx = 0; ((byIdx < bySize) && (byExchangeNum < MAX_EXCHANGE_STUDY_NUM)); byIdx++)
    {
        SAwakenClipToStudyProp2& stProp = vecProp[byIdx];
        if(wPlayerLevel < stProp.wNeedPlayerLevel)
        {
            if(0 == wLastNeedPlayerLevel)
            {
                wLastNeedPlayerLevel = stProp.wNeedPlayerLevel;
            }
            if(stProp.wNeedPlayerLevel != wLastNeedPlayerLevel)
            {
                bGetNextOpen = TRUE;
            }
            if(bGetNextOpen)
            {
                break;
            }
        }
        DT_AWAKEN_STUDY_EXCHANGE_DATA& stDT_AWAKEN_STUDY_EXCHANGE_DATA = astStudyExchangeInfo[byExchangeNum++];
        DT_STUDY_DATA stDT_STUDY_DATA = {0};
        stDT_STUDY_DATA.byStudyColorKind = stProp.byStudyColorKind;
        stDT_STUDY_DATA.byStudyAttrKind1 = stProp.byStudyAttrKind1;
        stDT_STUDY_DATA.byStudyAttrKind2 = stProp.byStudyAttrKind2;
        stDT_STUDY_DATA.byStudyLevel = stProp.byStudyLevel;
        GetDT_STUDY_DATA_CLI2(stDT_STUDY_DATA, stDT_AWAKEN_STUDY_EXCHANGE_DATA.stStudyInfo);
        stDT_AWAKEN_STUDY_EXCHANGE_DATA.dwNeedCoin = stProp.dwNeedCoin;
        stDT_AWAKEN_STUDY_EXCHANGE_DATA.dwNeedClip = stProp.dwNeedClip;
        stDT_AWAKEN_STUDY_EXCHANGE_DATA.wNeePlayerLevel = stProp.wNeedPlayerLevel;
    }
    qwCurCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    qwCurStudyClip = m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip;

    return ERR_OPEN_AWAKEN_EXCHANGE_TAB::ID_SUCCESS;
}

UINT16 CAwaken::ExchangeStudy(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel, UINT8& byStudyBagIdx, DT_AWAKEN_TAB_DATA2& stAwakenTabInfo)
{
    if(!m_poOwner->GetBuild(EBK_STUDYEXCHANGE))
    {
        return ERR_EXCHANGE_STUDY::ID_STUDY_EXCHANGE_UNLOCK;
    }
    GetAwakenTabInfo2(stAwakenTabInfo);
    SAwakenClipToStudyProp2* pstProp = CAwakenPropMgr2::Instance()->GetClipToStudyProp(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyLevel);
    if(NULL == pstProp)
    {
        return ERR_EXCHANGE_STUDY::ID_STUDY_NOT_EXIST;
    }
    if(m_poOwner->GetLevel() < pstProp->byStudyLevel)
    {
        return ERR_EXCHANGE_STUDY::ID_PLAYER_LEVEL_NOT_REACH;
    }

    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < pstProp->dwNeedCoin)
    {
        return ERR_COMMON::ID_COIN_NOT_ENOUGH;
    }

    if(m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip < pstProp->dwNeedClip)
    {
        return ERR_EXCHANGE_STUDY::ID_NOT_ENOUGH_CLIP;
    }

    C88MapItr itr = m_mapIdleStudyIdx.begin();
    if(itr == m_mapIdleStudyIdx.end())
    {
        return ERR_EXCHANGE_STUDY::ID_STUDY_BAG_FULL;
    }

    if(0 != pstProp->dwNeedCoin)
    {
        m_poOwner->DecCoin(pstProp->dwNeedCoin, CRecordMgr::EDCT_EXCHANGESTUDY, byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyLevel);
    }
    if(0 != pstProp->dwNeedClip)
    {
        m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip -= pstProp->dwNeedClip;

        CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECSTUDYCLIP, CRecordMgr::EDSCTUDYTCLIP_EXCHANGE, pstProp->dwNeedClip, m_stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip,
            m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[itr->first];
    stDT_STUDY_DATA.byStudyColorKind = pstProp->byStudyColorKind;
    stDT_STUDY_DATA.byStudyAttrKind1 = pstProp->byStudyAttrKind1;
    stDT_STUDY_DATA.byStudyAttrKind2 = pstProp->byStudyAttrKind2;
    stDT_STUDY_DATA.byStudyLevel = pstProp->byStudyLevel;

    byStudyBagIdx = itr->first;

    //记录获得太极的个数
    if (stDT_STUDY_DATA.byStudyColorKind == EC_ORANGE && stDT_STUDY_DATA.byStudyAttrKind1 == 0)
    {
        m_poOwner->CkToday();
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddTaijiStudyNum++;
    }

    GetAwakenTabInfo2(stAwakenTabInfo);

    m_mapIdleStudyIdx.erase(itr);

    return ERR_EXCHANGE_STUDY::ID_SUCCESS;
}

UINT16 CAwaken::OpenPlayerInfoAwaken(PKT_CLIGS_PLAYER_INFO_AWAKEN_ACK& stAck)
{
	CAwakenPropMgr2::Instance()->GetwLevelStudyUnlockLevelInfo(stAck.awStudyUnlockLevelInfo);

	set<CHero*> setHero;
	stAck.byHeroNum = 0;
	//插入主角
	CHero* poHero = m_poOwner->GetCoachHero();
	if ( NULL == poHero)
	{
		return ERR_PLAYER_INFO_AWAKEN::ID_COACH_HERO_LOST;
	}
	DT_HERO_STUDY_DATA_CLI2& stDT_HERO_STUDY_DATA_CLI = stAck.astHeroStudyInfo[stAck.byHeroNum];
	GetHeroStudyDataCli2(poHero, stDT_HERO_STUDY_DATA_CLI);
	stAck.awHeroLevel[stAck.byHeroNum] = poHero->GetDT_HERO_BASE_DATA().wLevel;
	setHero.insert(poHero);
	stAck.byHeroNum++;
	CFormationHeroShmemMap& mapFormation = m_poOwner->GetFormationHeroMap();
	poHero = mapFormation.GetFistData();

	while ( poHero)
	{
		if ( setHero.find(poHero) != setHero.end() )
		{
			poHero = mapFormation.GetNextData();
			continue;
		}
		DT_HERO_STUDY_DATA_CLI2& stDT_HERO_STUDY_DATA_CLI = stAck.astHeroStudyInfo[stAck.byHeroNum];
		GetHeroStudyDataCli2(poHero, stDT_HERO_STUDY_DATA_CLI);
		stAck.awHeroLevel[stAck.byHeroNum] = poHero->GetDT_HERO_BASE_DATA().wLevel;
		stAck.byHeroNum++;
		setHero.insert(poHero);
		poHero = mapFormation.GetNextData();
	}
	CKindID2HeroShmemMap& mapHero = m_poOwner->GetAllCarryHeroMap();
	
	poHero = mapHero.GetFistData();
	while ( poHero )
	{
		if ( setHero.find(poHero) != setHero.end() )
		{
			poHero = mapHero.GetNextData();
			continue;
		}
		DT_HERO_STUDY_DATA_CLI2& stDT_HERO_STUDY_DATA_CLI = stAck.astHeroStudyInfo[stAck.byHeroNum];
		GetHeroStudyDataCli2(poHero, stDT_HERO_STUDY_DATA_CLI);
		stAck.awHeroLevel[stAck.byHeroNum] = poHero->GetDT_HERO_BASE_DATA().wLevel;
		stAck.byHeroNum++;
		setHero.insert(poHero);
		poHero = mapHero.GetNextData();
	}
		
	return ERR_PLAYER_INFO_AWAKEN::ID_SUCCESS;
}

UINT16 CAwaken::OnOpenResolveTrigram(UINT8 byPos, PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK &psAck)
{

    if(!m_poOwner->GetBuild(EBK_STUDYEXCHANGE))
    {
        return ERR_OPEN_RESOLVE_TRIGRAM::ID_UNOPEN;
    }

    if(byPos >= MAX_BAG_STUDY_GRID_NUM)
    {
        return ERR_OPEN_RESOLVE_TRIGRAM::ID_IS_NOT_ORANGE_STUDY;
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byPos];
    if(0 == stDT_STUDY_DATA.byStudyColorKind)
    {
        return ERR_OPEN_RESOLVE_TRIGRAM::ID_ID_INVALID;
    }

    if (4 != stDT_STUDY_DATA.byStudyColorKind || stDT_STUDY_DATA.byStudyAttrKind1 == 0)
    {
        return ERR_OPEN_RESOLVE_TRIGRAM::ID_IS_NOT_ORANGE_STUDY;
    }

    UINT8 byStudyColorKind = stDT_STUDY_DATA.byStudyColorKind;

    //双属性卦象
    if (stDT_STUDY_DATA.byStudyColorKind == 4 && stDT_STUDY_DATA.byStudyAttrKind1 !=0 && stDT_STUDY_DATA.byStudyAttrKind2 != 0)
    {
        byStudyColorKind = EC_DOUBLE_COLOR;
    }

    UINT8 byIfResolve = CAwakenPropMgr2::Instance()->GetAwakenIfResolveByID(byStudyColorKind);
    if (byIfResolve == 0)
    {
        return ERR_OPEN_RESOLVE_TRIGRAM::ID_UNOPEN;
    }
    SAwakenResolveProp *pSAwakenpro = CAwakenPropMgr2::Instance()->GetAwakenResolvePropByID(byStudyColorKind);
    if (pSAwakenpro == NULL)
    {
        return ERR_OPEN_RESOLVE_TRIGRAM::ID_UNOPEN;
    }
    psAck.wCommonNum = pSAwakenpro->wCommonNum;
    psAck.wSpecialNum = pSAwakenpro->wSpecialNum;
    psAck.dwGoldNum = pSAwakenpro->dwSpecialGold;
    psAck.dwCoinNum = pSAwakenpro->dwCommonCoin;
    psAck.byBagGridIdx = byPos;

    return ERR_OPEN_RESOLVE_TRIGRAM::ID_SUCCESS;
}

UINT16 CAwaken::OnResolveTrigramReq(UINT8 byPos, UINT8 byUseGold, PKT_CLIGS_RESOLVE_TRIGRAM_ACK &psAck)
{
  
    if(!m_poOwner->GetBuild(EBK_STUDYEXCHANGE))
    {
        return ERR_RESOLVE_TRIGRAM::ID_UNOPEN;
    }

    if(byPos >= MAX_BAG_STUDY_GRID_NUM)
    {
        return ERR_RESOLVE_TRIGRAM::ID_IS_NOT_ORANGE_STUDY;
    }

    DT_STUDY_DATA& stDT_STUDY_DATA = m_stDT_AWAKEN_DATA.astBagStudyInfo[byPos];

    if (0 == stDT_STUDY_DATA.byStudyColorKind)
    {
        return ERR_RESOLVE_TRIGRAM::ID_ID_INVALID;
    }

    if (4 != stDT_STUDY_DATA.byStudyColorKind || stDT_STUDY_DATA.byStudyAttrKind1 == 0)
    {
        return ERR_RESOLVE_TRIGRAM::ID_IS_NOT_ORANGE_STUDY;
    }

    UINT8 byStudyColorKind = stDT_STUDY_DATA.byStudyColorKind;

    //双属性卦象
    if (stDT_STUDY_DATA.byStudyColorKind == 4 && stDT_STUDY_DATA.byStudyAttrKind1 !=0 && stDT_STUDY_DATA.byStudyAttrKind2 != 0)
    {
        byStudyColorKind = EC_DOUBLE_COLOR;
    }

    UINT8 byIfResolve = CAwakenPropMgr2::Instance()->GetAwakenIfResolveByID(byStudyColorKind);
    if (byIfResolve == 0)
    {
        return ERR_RESOLVE_TRIGRAM::ID_IS_NOT_ORANGE_STUDY;
    }

    DT_AWAKEN_BASE_DATA& stBaseInfo = m_stDT_AWAKEN_DATA.stBaseInfo;
    SAwakenResolveProp *pSAwakenpro = CAwakenPropMgr2::Instance()->GetAwakenResolvePropByID(byStudyColorKind);
    if (pSAwakenpro == NULL)
    {
        return ERR_RESOLVE_TRIGRAM::ID_UNOPEN;
    }

    UINT32 dwGenClipOnce = 0;
    if (byUseGold)
    {
        //判断元宝
        UINT32 dwCostGold = pSAwakenpro->dwSpecialGold;
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
        {
            return ERR_RESOLVE_TRIGRAM::ID_NOT_ENOUGH_GOLD;
        }

        //扣除元宝
        m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_AWAKENONCE);
        //获得心得数量
        dwGenClipOnce = pSAwakenpro->wSpecialNum;
    }
    else
    {
        //判断铜钱
        UINT32 dwCostCoin = pSAwakenpro->dwCommonCoin;
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostCoin)
        {
            return ERR_RESOLVE_TRIGRAM::ID_NOT_ENOUGH_COIN;
        }

        //扣除元宝
        m_poOwner->DecCoin(dwCostCoin, CRecordMgr::EDGT_AWAKENONCE);
        //获得心得数量
        dwGenClipOnce = pSAwakenpro->wCommonNum;
    }
 
    psAck.dwGetStudyClip = dwGenClipOnce;
    stBaseInfo.qwStudyClip += dwGenClipOnce;
    psAck.byBagGridIdx = byPos; 
    psAck.byUseGold = byUseGold;
    m_mapIdleStudyIdx[byPos] = byPos;
    memset(&stDT_STUDY_DATA, 0, sizeof(stDT_STUDY_DATA));

    psAck.qwStudyClip = stBaseInfo.qwStudyClip;

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDSTUDYCLIP, CRecordMgr::EASCTUDYTCLIP_AWAKEN, dwGenClipOnce, stBaseInfo.qwStudyClip,
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);

    GetBagStudyInfo2(psAck.astBagStudyInfo);

    return ERR_RESOLVE_TRIGRAM::ID_SUCCESS;
}

VOID CAwaken::GetIfResolve( UINT8 &byCommon, UINT8 &bySpecial )
{
    byCommon = CAwakenPropMgr2::Instance()->GetAwakenIfResolveByID(EC_ORANGE);
    bySpecial = CAwakenPropMgr2::Instance()->GetAwakenIfResolveByID(EC_DOUBLE_COLOR);
}


