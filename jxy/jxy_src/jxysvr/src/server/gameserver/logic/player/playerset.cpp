#include "player.h"
#include "logic/vip/vippropmgr.h"
#include "logic/record/recordmgr.h"
#include "playersavemgr.h"
#include <stdlib.h>
#include <net/db/dbpktbuilder.h>
#include <net/cli/user.h>
#include <framework/gsapi.h>
#include <logic/awaken/awaken.h>
#include "logic/medal/playermedal.h"

VOID CPlayer::SetPower(UINT32 dwPower)
{
    m_stDT_PLAYER_BASE_DATA.dwPower = dwPower;
}

VOID CPlayer::SetFirstAttack(UINT32 dwFirstAttack)
{
    m_stDT_PLAYER_BASE_DATA.dwFirstAttack = dwFirstAttack;
}

VOID CPlayer::SetTodayFirstLoginTime(UINT64 qwTodayFirstLoginTime)
{
    m_stDT_PLAYER_BASE_DATA.qwTodayFirstLoginTime = qwTodayFirstLoginTime;
}
VOID CPlayer::SetBagOpenNum(UINT8 byOpenNum)
{
    m_stDT_PLAYER_BASE_DATA.byBagOpenNum = byOpenNum;
}

VOID CPlayer::AddCoin(UINT32 dwCoin, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwCoin;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddCoin::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwCoin += dwCoin;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddCoin += dwCoin;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterCoin = m_stDT_PLAYER_BASE_DATA.qwCoin;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDCOIN, wOpType, dwCoin, m_stDT_PLAYER_BASE_DATA.qwCoin, 
        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

// dwRMB 金额单位为分
VOID CPlayer::AddGold(UINT32 dwGold, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment, UINT32 dwRMB)
{
    INT32 nTmp = dwGold;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddGold::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwGold += dwGold;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddGold += dwGold;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterGold = m_stDT_PLAYER_BASE_DATA.qwGold;

    UINT32 dwAddRMBGold     = 0;
    UINT32 dwAddRMBGoldGift = 0;
    UINT32 dwAddGmGold      = 0;
    UINT32 dwAddGameGold    = 0;

    // 检测是否是通过充值方式添加的元宝
    if( CRecordMgr::EGGT_ADDFLAGEXCHANGE == wOpType || CRecordMgr::EGGT_GM_FLAGEXCHANGE == wOpType)
    {
        //因为玩家充值不可能充入如此大额，所以，强转没有问题
        //分转元宝计算 一元宝等行十分。
        AddTotalAmount( dwGold );
        m_stDT_PLAYER_BASE_DATA.qwTotalRMB += dwRMB;

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTotalRMB += dwRMB;
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterTotalRMB = m_stDT_PLAYER_BASE_DATA.qwTotalRMB;

        // 充值元宝的来源途径 = 充值人民币（元）1:10兑换的元宝 + 人民币充值时赠送元宝 + 系统赠送元宝
        if(CRecordMgr::EGGT_ADDFLAGEXCHANGE == wOpType)
        {
            // 玩家充值时获得的元宝中，其中<人民币（元） * 10>的部分属于人民币兑换元宝，剩余的是充值赠送元宝
            // 注意，这里人民币的单位是：分
            dwAddRMBGold = dwRMB / 10;
            dwAddRMBGoldGift = (dwGold > dwAddRMBGold ? dwGold - dwAddRMBGold : 0);

            m_stDT_PLAYER_BASE_DATA.dwRMBGold     += dwAddRMBGold;
            m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift += dwAddRMBGoldGift;
        }
        else if(CRecordMgr::EGGT_GM_FLAGEXCHANGE == wOpType)
        {
            dwAddGmGold  = dwGold;
            m_stDT_PLAYER_BASE_DATA.dwGmGold      += dwAddGmGold;
        }

        //充值则马上保存
        CPlayerSaveMgr::Instance()->SavePlayerData(this);
    }
    else
    {
        // 普通游戏内元宝
        dwAddGameGold = dwGold;
    }

    // 计算出当前的游戏元宝
    UINT64 qwCashGold = m_stDT_PLAYER_BASE_DATA.dwRMBGold + m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift + m_stDT_PLAYER_BASE_DATA.dwGmGold;
    UINT64 qwGameGold = (m_stDT_PLAYER_BASE_DATA.qwGold > qwCashGold ? (m_stDT_PLAYER_BASE_DATA.qwGold - qwCashGold) : 0);

    // 记录元宝获得信息
    CRecordMgr::Instance()->RecordGold(this->GetOriginalZoneID(), this->GetID(), this->GetLevel(), this->GetDT_PLAYER_BASE_DATA().byVipLevel, ERM_ADDGOLD, wOpType, 
        dwAddRMBGold, dwAddRMBGoldGift, dwAddGmGold, dwAddGameGold,
        m_stDT_PLAYER_BASE_DATA.dwRMBGold, m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift, m_stDT_PLAYER_BASE_DATA.dwGmGold, qwGameGold,
        qwParam1, qwParam2, qwParam3, qwParam4, strComment);

    //元宝获得信息采集
    /*
    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDGOLD, wOpType, dwGold, m_stDT_PLAYER_BASE_DATA.qwGold, 
        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
    */
}
VOID CPlayer::AddStory(UINT32 dwStory, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwStory;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddStory::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwStory += dwStory;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStory += dwStory;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStory = m_stDT_PLAYER_BASE_DATA.qwStory;
    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDSTORY, wOpType, dwStory, m_stDT_PLAYER_BASE_DATA.qwStory, 
        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

VOID CPlayer::AddTotalAmount( UINT32 dwAmount )
{
    INT32 nTmp = dwAmount;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddTotalAmount::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        //return;	//vip需重算
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.dwTotalAmount += dwAmount;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAddAmount += dwAmount;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAfterAmount = m_stDT_PLAYER_BASE_DATA.dwTotalAmount;

    UINT8 byNewVipLevel = CVipPropMgr::Instance()->GetVipLevel( m_stDT_PLAYER_BASE_DATA.dwTotalAmount );

    if( byNewVipLevel > m_stDT_PLAYER_BASE_DATA.byVipLevel )
    {
        SetVipLevel( byNewVipLevel );
    }

}


VOID CPlayer::AddPhyStrength(UINT16 wPhyStrength, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT16 nTmp = wPhyStrength;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddPhyStrength::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.wPhyStrength += wPhyStrength;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddPhyStrength += wPhyStrength;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterPhyStrength = m_stDT_PLAYER_BASE_DATA.wPhyStrength;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDPHYSTRENGTH, wOpType, wPhyStrength, m_stDT_PLAYER_BASE_DATA.wPhyStrength, 
        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::AddScience(UINT32 dwScience, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwScience;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddScience::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwScience += dwScience;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddScience += dwScience;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterScience = m_stDT_PLAYER_BASE_DATA.qwScience;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDSCIENCE, wOpType, dwScience, m_stDT_PLAYER_BASE_DATA.qwScience, 
        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::AddBlueGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwGas;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddBlueGas::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwBlueGas += dwGas;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddBlueGas += dwGas;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterBlueGas = m_stDT_PLAYER_BASE_DATA.qwBlueGas;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDBLUEGAS, wOpType, dwGas, m_stDT_PLAYER_BASE_DATA.qwBlueGas,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);

}
VOID CPlayer::AddPurpleGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwGas;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddPurpleGas::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwPurpleGas += dwGas;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddPurpleGas += dwGas;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterPurpleGas = m_stDT_PLAYER_BASE_DATA.qwPurpleGas;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDPURPLEGAS, wOpType, dwGas, m_stDT_PLAYER_BASE_DATA.qwPurpleGas,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::AddJingJie(UINT32 dwJingJie, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwJingJie;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddJingJie::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwJingJie += dwJingJie;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddJingJie += dwJingJie;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDJINGJIE, wOpType, dwJingJie, m_stDT_PLAYER_BASE_DATA.qwJingJie,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

VOID CPlayer::AddMedal(UINT32 dwMedal, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwMedal;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: AddJingJie::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwMedal += dwMedal;

    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddMedal = dwMedal;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterMedal = m_stDT_PLAYER_BASE_DATA.qwMedal;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDMEDAL, wOpType, dwMedal, m_stDT_PLAYER_BASE_DATA.qwMedal,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

VOID CPlayer::SetJingJie(UINT32 dwJingJie, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwJingJie;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: SetJingJie::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwJingJie = dwJingJie;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_ADDJINGJIE, wOpType, dwJingJie, m_stDT_PLAYER_BASE_DATA.qwJingJie,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);

}

VOID CPlayer::DecCoin(UINT32 dwCoin, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwCoin;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecCoin::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwCoin = m_stDT_PLAYER_BASE_DATA.qwCoin < dwCoin ? 0 : (m_stDT_PLAYER_BASE_DATA.qwCoin - dwCoin);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecCoin += dwCoin;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterCoin = m_stDT_PLAYER_BASE_DATA.qwCoin;


    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECCOIN, wOpType, dwCoin, m_stDT_PLAYER_BASE_DATA.qwCoin,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::DecGold(UINT32 dwDecGold, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwDecGold;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecGold::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();


    UINT32 dwDecRMBGold     = 0;
    UINT32 dwDecRMBGoldGift = 0;
    UINT32 dwDecGmGold      = 0;
    UINT32 dwDecGameGold    = 0;

    m_stDT_PLAYER_BASE_DATA.qwGold = m_stDT_PLAYER_BASE_DATA.qwGold < dwDecGold ? 0 : (m_stDT_PLAYER_BASE_DATA.qwGold - dwDecGold);

    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecGold += dwDecGold;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterGold = m_stDT_PLAYER_BASE_DATA.qwGold;

    // 由于运营审计需要，玩家的每次元宝消费都需要注明其中多少来自于：充值元宝、充值赠送元宝、系统赠送元宝、游戏元宝，所以扣除元宝会麻烦一点
    // 其中：
    // 充值元宝 = 人民币元宝 = 玩家每次充值时的人民币（元） * 10
    // 充值赠送元宝 = 每次充值时实际获得的元宝 - 玩家当次充值的人民币（元） * 10，即每次充值时赠予的额外元宝
    // 系统赠送元宝 = 通过GM方式给玩家添加的充值元宝（GM可以给玩家添加充值元宝和普通元宝）
    // 游戏内元宝   = 游戏奖励给玩家的元宝
    // 
    // 扣除过程如下：
    // 1. 如果人民币元宝还足够的话，则优先扣除人民币元宝
    if(m_stDT_PLAYER_BASE_DATA.dwRMBGold >= dwDecGold)
    {
        dwDecRMBGold = dwDecGold; 
        m_stDT_PLAYER_BASE_DATA.dwRMBGold -= dwDecGold;
    }
    else
    {
        dwDecRMBGold = m_stDT_PLAYER_BASE_DATA.dwRMBGold;
        m_stDT_PLAYER_BASE_DATA.dwRMBGold = 0;

        UINT32 dwRemainDecGold = dwDecGold - dwDecRMBGold;

        // 2. 再从充值赠送元宝中扣除
        if(m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift >= dwRemainDecGold)
        {
            dwDecRMBGoldGift = dwRemainDecGold;
            m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift -= dwRemainDecGold;
        }
        else
        {
            dwDecRMBGoldGift = m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift;
            m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift = 0;

            dwRemainDecGold -= dwDecRMBGoldGift;

            // 3. 从系统赠送元宝中扣除
            if(m_stDT_PLAYER_BASE_DATA.dwGmGold >= dwRemainDecGold)
            {
                dwDecGmGold = dwRemainDecGold;
                m_stDT_PLAYER_BASE_DATA.dwGmGold -= dwRemainDecGold;
            }
            else
            {
                dwDecGmGold = m_stDT_PLAYER_BASE_DATA.dwGmGold;
                m_stDT_PLAYER_BASE_DATA.dwGmGold = 0;

                // 4. 最后再从游戏元宝中扣除
                dwDecGameGold = dwRemainDecGold - dwDecGmGold;
            }
        }
    }

    // 计算出当前的游戏元宝
    UINT64 qwCashGold = m_stDT_PLAYER_BASE_DATA.dwRMBGold + m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift + m_stDT_PLAYER_BASE_DATA.dwGmGold;
    UINT64 qwGameGold = (m_stDT_PLAYER_BASE_DATA.qwGold > qwCashGold ? (m_stDT_PLAYER_BASE_DATA.qwGold - qwCashGold) : 0);

    CRecordMgr::Instance()->RecordGold(this->GetOriginalZoneID(), this->GetID(), this->GetLevel(), this->GetDT_PLAYER_BASE_DATA().byVipLevel, ERM_DECGOLD, wOpType, 
        dwDecRMBGold, dwDecRMBGoldGift, dwDecGmGold, dwDecGameGold,
        m_stDT_PLAYER_BASE_DATA.dwRMBGold, m_stDT_PLAYER_BASE_DATA.dwRMBGoldGift, m_stDT_PLAYER_BASE_DATA.dwGmGold, qwGameGold,
        qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

VOID CPlayer::DecStory(UINT32 dwStory, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwStory;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecStory::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwStory = m_stDT_PLAYER_BASE_DATA.qwStory < dwStory ? 0 : (m_stDT_PLAYER_BASE_DATA.qwStory - dwStory);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStory += dwStory;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStory = m_stDT_PLAYER_BASE_DATA.qwStory;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECSTORY, wOpType, dwStory, m_stDT_PLAYER_BASE_DATA.qwStory,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::DecPhyStrength(UINT16 wPhyStrength, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT16 nTmp = wPhyStrength;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecPhyStrength::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.wPhyStrength = m_stDT_PLAYER_BASE_DATA.wPhyStrength < wPhyStrength ? 0 : (m_stDT_PLAYER_BASE_DATA.wPhyStrength - wPhyStrength);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.wDecPhyStrength += wPhyStrength;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterPhyStrength = m_stDT_PLAYER_BASE_DATA.wPhyStrength;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECPHYSTRENGTH, wOpType, wPhyStrength, m_stDT_PLAYER_BASE_DATA.wPhyStrength,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::DecScience(UINT32 dwScience, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwScience;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecScience::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwScience = m_stDT_PLAYER_BASE_DATA.qwScience < dwScience ? 0 : (m_stDT_PLAYER_BASE_DATA.qwScience - dwScience);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecScience += dwScience;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterScience = m_stDT_PLAYER_BASE_DATA.qwScience;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECSCIENCE, wOpType, dwScience, m_stDT_PLAYER_BASE_DATA.qwScience,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

VOID CPlayer::DecBlueGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwGas;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecBlueGas::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwBlueGas = m_stDT_PLAYER_BASE_DATA.qwBlueGas < dwGas ? 0 : (m_stDT_PLAYER_BASE_DATA.qwBlueGas - dwGas);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecBlueGas += dwGas;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterBlueGas = m_stDT_PLAYER_BASE_DATA.qwBlueGas;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECBLUEGAS, wOpType, dwGas, m_stDT_PLAYER_BASE_DATA.qwBlueGas,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);

}
VOID CPlayer::DecPurpleGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwGas;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecPurpleGas::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwPurpleGas = m_stDT_PLAYER_BASE_DATA.qwPurpleGas < dwGas ? 0 : (m_stDT_PLAYER_BASE_DATA.qwPurpleGas - dwGas);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecPurpleGas += dwGas;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterPurpleGas = m_stDT_PLAYER_BASE_DATA.qwPurpleGas;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECPURPLEGAS, wOpType, dwGas, m_stDT_PLAYER_BASE_DATA.qwPurpleGas,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}
VOID CPlayer::DecJingJie(UINT32 dwJingJie, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwJingJie;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecJingJie::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie < dwJingJie ? 0 : (m_stDT_PLAYER_BASE_DATA.qwJingJie - dwJingJie);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecJingJie += dwJingJie;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECJINGJIE, wOpType, dwJingJie, m_stDT_PLAYER_BASE_DATA.qwJingJie,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);

}

VOID CPlayer::DecMedal(UINT32 dwMedal, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    INT32 nTmp = dwMedal;
    if(nTmp <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: DecJingJie::Value is ivalid[%d]!"), MSG_MARK, nTmp);
        return;
    }
    CkToday();
    m_stDT_PLAYER_BASE_DATA.qwMedal = m_stDT_PLAYER_BASE_DATA.qwMedal < dwMedal ? 0 : (m_stDT_PLAYER_BASE_DATA.qwMedal - dwMedal);
    //记录当天的消费记录
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecMedal += dwMedal;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterMedal = m_stDT_PLAYER_BASE_DATA.qwMedal;

    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECMEDAL, wOpType, dwMedal, m_stDT_PLAYER_BASE_DATA.qwMedal,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);

}

VOID CPlayer::AddStudyCli(UINT32 dwOpValue, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    CAwaken* poAwaken = GetAwaken();
    if(NULL == poAwaken)
    {
        return;
    }

	poAwaken->GetDT_AWAKEN_DATA().stBaseInfo.qwStudyClip += dwOpValue;
    CRecordMgr::Instance()->RecordInfo( this->GetID(), ERM_DECSTUDYCLIP, wOpType, dwOpValue, poAwaken->GetDT_AWAKEN_DATA().stBaseInfo.qwStudyClip,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

VOID CPlayer::DecStudyCli(UINT32 dwOpValue, UINT16 wOpType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
    CAwaken* poAwaken = GetAwaken();
    if(NULL == poAwaken)
    {
        return;
    }
	DT_AWAKEN_BASE_DATA& stBase = poAwaken->GetDT_AWAKEN_DATA().stBaseInfo;
	stBase.qwStudyClip = stBase.qwStudyClip >= dwOpValue ? stBase.qwStudyClip - dwOpValue  : 0;

    CRecordMgr::Instance()->RecordInfo( GetID(), ERM_ADDSTUDYCLIP, wOpType, dwOpValue, poAwaken->GetDT_AWAKEN_DATA().stBaseInfo.qwStudyClip,
                                        GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, qwParam1, qwParam2, qwParam3, qwParam4, strComment);
}

UINT64 CPlayer::GetMedal() // 获取勇气勋章
{
    return m_stDT_PLAYER_BASE_DATA.qwMedal;
}

VOID CPlayer::SetVipLevel( UINT8 byVipLevel, UINT32 dwTotalAmount)
{
    //检测过虑最大值
    if (byVipLevel > CVipPropMgr::Instance()->GetMaxVipLevel())
    {
        byVipLevel = CVipPropMgr::Instance()->GetMaxVipLevel();
    }
    m_stDT_PLAYER_BASE_DATA.byVipLevel = byVipLevel;
	CPlayerMedal* poPlayerMedal = GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->CheckMedalBySmallType(EMST_PLAYER_VIP_LEVEL);
	}
    m_stDT_PLAYER_TODAY_CONSUME_DATA.byAddVipLevel += (byVipLevel - m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel);
    m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel = m_stDT_PLAYER_BASE_DATA.byVipLevel;

    if (0 != dwTotalAmount)
    {
        m_stDT_PLAYER_BASE_DATA.dwTotalAmount = dwTotalAmount;
    }
	CPlayerBaseDataCacheMgr::Instance()->SetVipLevel(m_dwPlayerID, byVipLevel);
}

VOID CPlayer::DecPayByGM(UINT32 dwDecRMB)
{
    USR_INFO(_SDT("[%s: %d]: DecPayByGM::PlayerID[%u] DecRMB[%u]!"), MSG_MARK, m_stDT_PLAYER_BASE_DATA.dwID, dwDecRMB);
    SBuyGoldProp buyInfo;
    UINT32 dwDecGold = 0;
    if(!CVipPropMgr::Instance()->GetBuyPropByRMB(GetAuthType(), dwDecRMB, buyInfo ))
    {
        dwDecGold = dwDecRMB * 10;
        USR_INFO(_SDT("[%s: %d]: DecPayByGM::PlayerID[%u] GetBuyPropByRMB[%u] not found! DecRMB = %u, DecGold is %u"), MSG_MARK, m_stDT_PLAYER_BASE_DATA.dwID, GetAuthType(), dwDecRMB, dwDecGold);

    }
    else
    {
        dwDecGold = buyInfo.dwGold;
        USR_INFO(_SDT("[%s: %d]: DecPayByGM::PlayerID[%u] DecRMB = %u, DecGold is %u"), MSG_MARK, m_stDT_PLAYER_BASE_DATA.dwID, dwDecRMB, dwDecGold);
    }

    USR_INFO(_SDT("[%s: %d]: DecPayByGM::PlayerID[%u] Before op TotalAmount is %u, VipLevel is %u"), MSG_MARK, m_stDT_PLAYER_BASE_DATA.dwID, m_stDT_PLAYER_BASE_DATA.dwTotalAmount, m_stDT_PLAYER_BASE_DATA.byVipLevel);
    m_stDT_PLAYER_BASE_DATA.dwTotalAmount = m_stDT_PLAYER_BASE_DATA.dwTotalAmount >  dwDecGold ? m_stDT_PLAYER_BASE_DATA.dwTotalAmount - dwDecGold : 0;
    UINT8 byNewVipLevel = CVipPropMgr::Instance()->GetVipLevel( m_stDT_PLAYER_BASE_DATA.dwTotalAmount );
    SetVipLevel( byNewVipLevel );
    USR_INFO(_SDT("[%s: %d]: DecPayByGM::PlayerID[%u] After op TotalAmount is %u, VipLevel is %u"), MSG_MARK, m_stDT_PLAYER_BASE_DATA.dwID, m_stDT_PLAYER_BASE_DATA.dwTotalAmount, m_stDT_PLAYER_BASE_DATA.byVipLevel);
}

void CPlayer::CkToday()
{
    //检测是否跨天数据
    if(!IsToday(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime))
    {
        CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_PLAYER_CONSUME);
        if(NULL == poCMD)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
            return ;
        }
        poCMD->SetUserData((VOID*)&GetDT_PLAYER_TODAY_CONSUME_DATA(), GetID());
        gsapi::GetDBMgr()->AddCommand(poCMD);

        CSDDateTime	oDateTime(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime);
        tstring		strDateTime = SDTimeToString(oDateTime);
        // DBG_INFO( _SDT( "[%s: %d]:player:%d LastConsumeTime:%llu, LastConsumeTime:%s" ), MSG_MARK, m_dwPlayerID, m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime, strDateTime.c_str());


        //清空当前的消费记录
        memset(&m_stDT_PLAYER_TODAY_CONSUME_DATA, 0x00, sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
        InitDT_PLAYER_TODAY_CONSUME_DATA();
    }
}

VOID CPlayer::AddFlowerNum( UINT32 dwFlowerNum )
{
    INT32 nTmp = dwFlowerNum;
    if(nTmp <= 0)
    {
        return;
    }
    m_stDT_PLAYER_BASE_DATA.dwFlowerSum += dwFlowerNum;
}
