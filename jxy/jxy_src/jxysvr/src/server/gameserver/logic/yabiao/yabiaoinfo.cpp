#include "yabiaoinfo.h"
#include "yabiaomgr.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/event/timereventmgr.h>
#include "logic/other/singleprammgr.h"
#include "logic/record/recordmgr.h"
#include "logic/vip/vippropmgr.h"
#include "logic/medal/playermedal.h"
#include "logic/other/fewparam.h"
IMPLEMENT_CLASS_POOL_MGR(CYabiaoInfo);
CYabiaoInfo::CYabiaoInfo()
{
    m_poOwner = NULL;
    memset( &m_stDT_YABIAO_BASE_DATA, 0, sizeof( m_stDT_YABIAO_BASE_DATA ) );
    m_nCacheJiebiaoRemainJiebiaoIndex = -1; //默认为-1

    memset(&m_stBeginJiebiaoAck5, 0, sizeof(m_stBeginJiebiaoAck5));
    memset(&m_stBeginJiebiaoAck3, 0, sizeof(m_stBeginJiebiaoAck3));
	memset(&m_stBeginJiebiaoAck4, 0, sizeof(m_stBeginJiebiaoAck4));
}

CYabiaoInfo::~CYabiaoInfo()
{
    UnInit();
}


INT32 CYabiaoInfo::GetRemainYabiaoCnt()
{
    if( CYabiaoMgr::Instance()->GetTotalYabiaoCnt() < m_stDT_YABIAO_BASE_DATA.byAlreadyYabiaoCount )
    {
        return 0;
    }
    else
    {
        return CYabiaoMgr::Instance()->GetTotalYabiaoCnt() - m_stDT_YABIAO_BASE_DATA.byAlreadyYabiaoCount;
    }

}

INT32 CYabiaoInfo::GetRemainJiebiaoCnt()
{
    if( CYabiaoMgr::Instance()->GetTotalJiebiaoCnt() < m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount )
    {
        return 0;
    }
    else
    {
        return CYabiaoMgr::Instance()->GetTotalJiebiaoCnt() - m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount;
    }

}

INT32 CYabiaoInfo::GetRemainBeJiebiaoCnt()
{
    if( CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt() < m_stDT_YABIAO_BASE_DATA.byBeJiebiaoCount )
    {
        return 0;
    }
    else
    {
        return CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt() - m_stDT_YABIAO_BASE_DATA.byBeJiebiaoCount;
    }
// 	auto nRemainBeJiebiaoCnt=CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt()-m_stDT_YABIAO_BASE_DATA.byBeJiebiaoCount;
//
// 	if( nRemainBeJiebiaoCnt<0 )
// 	{
// 		USR_IMPORTANT( _SDT( "[%s %d] remain jiebiao cnt<0，remain jiebiao cnt:%d" ), MSG_MARK, nRemainBeJiebiaoCnt );
// 	}
//
// 	return nRemainBeJiebiaoCnt;
}

VOID CYabiaoInfo::Check4Reset()
{
    //DBG_INFO( "player:%u begin yabiao info reset check!", m_poOwner->GetID() );
    if( !IsToday( m_stDT_YABIAO_BASE_DATA.qwLastYabiaoTime ) )
    {
        m_stDT_YABIAO_BASE_DATA.byAlreadyYabiaoCount = 0;
        //DBG_INFO( "player:%u reset yabiao cnt", m_poOwner->GetID() );
    }
    else
    {
        //DBG_INFO( "player:%u last yabiao time:%llu", m_poOwner->GetID(), m_stDT_YABIAO_BASE_DATA.qwLastYabiaoTime );
    }

    //判断劫镖
    if( !IsToday( m_stDT_YABIAO_BASE_DATA.qwLastJiebiaoTime ) )
    {
        m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount = 0;
        //DBG_INFO( "player:%u reset jiebiao cnt", m_poOwner->GetID() );
    }
    else
    {
        //DBG_INFO( "player:%u last jiebiao time:%llu", m_poOwner->GetID(), m_stDT_YABIAO_BASE_DATA.qwLastJiebiaoTime );
    }

    if( !IsToday( m_stDT_YABIAO_BASE_DATA.qwLastFlushTime ) )
    {
        m_stDT_YABIAO_BASE_DATA.dwAlreadyFlushBiaoCnt = 0;
        m_stDT_YABIAO_BASE_DATA.wBiaoFlushCount = 0;
        //DBG_INFO( "player:%u reset FlushBiaoCnt", m_poOwner->GetID() );
    }
    else
    {
        //DBG_INFO( "player:%u last flush time:%llu", m_poOwner->GetID(), m_stDT_YABIAO_BASE_DATA.qwLastFlushTime );
    }
}

BOOL CYabiaoInfo::EndYabiao()
{
    if( !IsYabiaoEnd() )
    {
        return FALSE;
    }

    UINT32 dwCoin = GetCurBiaoInfo().dwCoin;
    m_stDT_YABIAO_BASE_DATA.qwLastEncourageCoin = dwCoin < m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecCoin ? 0 : dwCoin - m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecCoin;

    UINT32 dwScience = GetCurBiaoInfo().dwScience;
    m_stDT_YABIAO_BASE_DATA.qwLastEncourageScience = dwScience < m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecScience ? 0 : dwScience - m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecScience;
    m_stDT_YABIAO_BASE_DATA.qwLastEncourageJingjie = GetCurBiaoInfo().dwJingjie;

    AddYabiaoBattleLog( ms_nYabiaoFlagSucceed, SDTimeSecs(), 0, 0, UINT32( m_stDT_YABIAO_BASE_DATA.qwLastEncourageCoin ),
                        UINT32( m_stDT_YABIAO_BASE_DATA.qwLastEncourageScience ), 0, m_stDT_YABIAO_BASE_DATA.byQuality, UINT32( m_stDT_YABIAO_BASE_DATA.qwLastEncourageJingjie ) );
    //ResetYabiaoInfo();

    m_stDT_YABIAO_BASE_DATA.byYabiaoFlag = ms_nYabiaoProcessFlagSuccess;
    CTimerEventMgr::Instance()->DelYabiaoEvent( m_poOwner->GetID() );
    return TRUE;
}

UINT8 CYabiaoInfo::GetUnreadBattleLogNum()
{
    UINT8 byRet = 0;
    for( UINT32 dwIndex = m_deqYabiaoBattleLog.size(); dwIndex > 0; --dwIndex )
    {
        if( !m_deqYabiaoBattleLog[dwIndex - 1]->GetDT_PAOSHANG_BATTLELOG_DATA().byNewFlag )
        {
            break;
        }

        //DBG_INFO( "yabiao log index:%u, is new log:%u", dwIndex, m_deqYabiaoBattleLog[dwIndex - 1]->GetDT_PAOSHANG_BATTLELOG_DATA().byNewFlag );

        ++byRet;
    }
    return byRet;
}

INT32 CYabiaoInfo::GetClearJiebiaoCDGold()
{
    return ( GetRemainJiebiaoCD() / 60 + 1 ) * CSinglePramMgr::Instance()->GetClearJiebiaoCDCostGoldPerMin();
}

VOID CYabiaoInfo::ClearJiebiaoCD()
{
    m_stDT_YABIAO_BASE_DATA.qwLastJiebiaoTime = 0;
}

// VOID CYabiaoInfo::OnCrocessDay()
// {
// 	DBG_INFO( "player %u crossday reset yabiao info!", m_poOwner->GetID() );
// 	m_stDT_YABIAO_BASE_DATA.byAlreadyYabiaoCount=0;
// 	m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount=0;
// 	m_stDT_YABIAO_BASE_DATA.dwAlreadyFlushBiaoCnt=0;
// }

BOOL CYabiaoInfo::IsYabiaoEnd()
{
    UINT64 qwPassTime = SDTimeSecs() < UINT32(m_stDT_YABIAO_BASE_DATA.qwLastYabiaoTime) ? 0 : SDTimeSecs() - m_stDT_YABIAO_BASE_DATA.qwLastYabiaoTime;
    INT32 nCostTime = CYabiaoMgr::Instance()->GetCostTime( m_stDT_YABIAO_BASE_DATA.byQuality );
    return qwPassTime >= nCostTime;
}

BOOL CYabiaoInfo::Init(DT_YABIAO_DATA &stDT_YABIAO_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
    memcpy(&m_stDT_YABIAO_BASE_DATA, &stDT_YABIAO_DATA.stYabiaoBaseInfo, sizeof(DT_YABIAO_BASE_DATA));

    //从数据库读取进来初始化时，也许已经在时间上完成押镖了，需要进行判断

    if( m_stDT_YABIAO_BASE_DATA.byYabiaoFlag == ms_nYabiaoProcessFlagRunning )
    {
        if( GetYabiaoPassedTime() >= GetCurBiaoInfo().wCostTime )
        {
            m_stDT_YABIAO_BASE_DATA.byYabiaoFlag = ms_nYabiaoProcessFlagStand;
            EndYabiao();
        }
        else
        {
            BeginYabiao( 0 );
            CYabiaoMgr::Instance()->AddPlayerToContainer( m_poOwner);
        }
    }

    for( INT32 nIndex = 0; nIndex < stDT_YABIAO_DATA.stYabiaoBattleLog.byBattleLogNum && nIndex < MAX_YABIAO_BATTLELOG_NUM; ++nIndex )
    {
        //过虑一些没用的数据，敌人都没有，完了。
        //if(0 == pstDT_YABIAO_BATTLELOG_DATA_LIST->astPaoshangBattleLogInfo[nIndex].dwEnemyPlayerID)
        //{
        //	continue;
        //}

        //过虑一些没用的数据,如果时间都比当前时间早，数据肯定有问题。
        if(stDT_YABIAO_DATA.stYabiaoBattleLog.astPaoshangBattleLogInfo[nIndex].qwBattleTime > SDTimeSecs())
        {
            continue;
        }

        CYabiaoBattleLog* pLog = CYabiaoBattleLogPoolMgr::Instance()->Create();
        if (NULL == pLog)
        {
            continue;
        }

        pLog->Init(stDT_YABIAO_DATA.stYabiaoBattleLog.astPaoshangBattleLogInfo[nIndex], m_poOwner );
        m_deqYabiaoBattleLog.push_back( pLog );
        //DBG_INFO( "init log index:%d, is new log:%u", nIndex, pLog->GetDT_PAOSHANG_BATTLELOG_DATA().byNewFlag );
    }

    //Check4Reset();
    return TRUE;
}

VOID CYabiaoInfo::UnInit()
{
    for(CYabiaoBattleLogDeq::iterator itr = m_deqYabiaoBattleLog.begin(); itr != m_deqYabiaoBattleLog.end(); itr++)
    {
        CYabiaoBattleLog* pYabiaoBattleLog = *itr;
        if(pYabiaoBattleLog)
        {
            CYabiaoBattleLogPoolMgr::Instance()->Remove(pYabiaoBattleLog);
        }
    }
    m_deqYabiaoBattleLog.clear();
}

UINT8 CYabiaoInfo::GetYabiaoFlag()
{
    return m_stDT_YABIAO_BASE_DATA.byYabiaoFlag;
}

DT_BIAO_INFO CYabiaoInfo::GetCurBiaoInfo()
{
    return CYabiaoMgr::Instance()->GetBiaoInfo( m_stDT_YABIAO_BASE_DATA.byQuality, m_poOwner->GetLevel() );
}


UINT16 CYabiaoInfo::GetFlushCostGold()
{
    if( m_stDT_YABIAO_BASE_DATA.dwAlreadyFlushBiaoCnt < CSinglePramMgr::Instance()->GetFreeFlushBiaoCnt() )
    {
        return 0;
    }
    return CYabiaoMgr::Instance()->GetFlushCostGoldByCount( m_stDT_YABIAO_BASE_DATA.wBiaoFlushCount );
}

UINT16 CYabiaoInfo::GetFlushCount()
{
    return m_stDT_YABIAO_BASE_DATA.wBiaoFlushCount;
}

INT32 CYabiaoInfo::BeginYabiao( time_t tmBegin/*=0*/ )
{
    if( m_stDT_YABIAO_BASE_DATA.byYabiaoFlag != ms_nYabiaoProcessFlagStand && tmBegin != 0 )
    {
        return ERR_BEGIN_YABIAO::ID_YABIAOING;
    }

    if( tmBegin != 0 )
    {
        m_stDT_YABIAO_BASE_DATA.byAlreadyYabiaoCount += 1;
    }


    UINT8 byQuality = m_stDT_YABIAO_BASE_DATA.byQuality;
    ResetYabiaoInfo();
    m_stDT_YABIAO_BASE_DATA.byQuality = byQuality;
    m_stDT_YABIAO_BASE_DATA.byYabiaoFlag = ms_nYabiaoProcessFlagRunning;

    if( 0 == tmBegin )
    {
        //沿用旧的时间
    }
    else
    {
        m_stDT_YABIAO_BASE_DATA.qwLastYabiaoTime = tmBegin;
    }
	if ( CYabiaoMgr::Instance()->GetMaxQualityLevel() == byQuality)
	{
		CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->AddEscortNum();
			poPlayerMedal->CheckMedalBySmallType(EMST_ESCORT);
		}
		CFewParam& oFew = m_poOwner->GetFewParam();
		oFew.AddXiJingMaster();
	}

	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		if ( CYabiaoMgr::Instance()->GetMaxQualityLevel() == byQuality)
		{
			poPlayerMedal->AddEscortNum();
			poPlayerMedal->CheckMedalBySmallType(EMST_ESCORT);
		}
	}

    //设置定时器
    CTimerEventMgr::Instance()->AddYabiaoEvent( m_poOwner->GetDT_PLAYER_BASE_DATA().dwID,
            GetCurBiaoInfo().wCostTime < GetYabiaoPassedTime() ? 0 : GetCurBiaoInfo().wCostTime - GetYabiaoPassedTime() ) ;
    return ERR_BEGIN_YABIAO::ID_SUCCESS;
}

VOID CYabiaoInfo::ResetYabiaoInfo()
{
    m_stDT_YABIAO_BASE_DATA.byYabiaoFlag = ms_nYabiaoProcessFlagStand;
    m_stDT_YABIAO_BASE_DATA.byQuality = 0;
    m_stDT_YABIAO_BASE_DATA.byBeJiebiaoCount = 0;
    m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecCoin = 0;
    m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecScience = 0;
    m_vecJiebiaoPlayerID.clear();
}

UINT32 CYabiaoInfo::GetYabiaoPassedTime()
{
    if( !GetYabiaoFlag() )
    {
        return 0;
    }

    UINT64 qwPassTime = SDTimeSecs() - m_stDT_YABIAO_BASE_DATA.qwLastYabiaoTime;
    UINT32 dwTotalTime = (UINT32)(CYabiaoMgr::Instance()->GetCostTime(m_stDT_YABIAO_BASE_DATA.byQuality));
    return qwPassTime > dwTotalTime ? 0 : (UINT32)qwPassTime;
}

INT32 CYabiaoInfo::GetRemainJiebiaoCD()
{
    INT32 nRet = INT32( CYabiaoMgr::Instance()->GetJiebiaoCD() - ( SDTimeSecs() - m_stDT_YABIAO_BASE_DATA.qwLastJiebiaoTime ) );

    if( nRet < 0 )
    {
        return nRet = 0;
    }

    //DBG_INFO( _SDT("player:%u return remain jiebiao CD time:%d"), m_poOwner->GetID(), nRet );

    return nRet;
}

VOID CYabiaoInfo::AddYabiaoBattleLog( UINT8 byFlag, UINT64 qwBattleTime, UINT32 dwEnemyPlayerID,
                                      UINT8 byResult, UINT32 dwCoinChange, UINT32 dwScienceChange, UINT64 qwBattleIndex, UINT8 byQuality, UINT32 dwJingjie, UINT8 byNewFlag )
{
    CYabiaoBattleLog* pYabiaoBattleLog = CYabiaoBattleLogPoolMgr::Instance()->Create();
    DT_YABIAO_BATTLELOG_DATA stDT_YABIAO_BATTLELOG_DATA = {0};
    stDT_YABIAO_BATTLELOG_DATA.byFlag = byFlag;
    stDT_YABIAO_BATTLELOG_DATA.byNewFlag = byNewFlag;
    stDT_YABIAO_BATTLELOG_DATA.byResult = byResult;
    stDT_YABIAO_BATTLELOG_DATA.dwCoinChange = dwCoinChange;
    stDT_YABIAO_BATTLELOG_DATA.dwScienceChange = dwScienceChange;
    stDT_YABIAO_BATTLELOG_DATA.dwEnemyPlayerID = dwEnemyPlayerID;
    stDT_YABIAO_BATTLELOG_DATA.qwBattleTime = qwBattleTime;
    stDT_YABIAO_BATTLELOG_DATA.qwBattleDataIndex = qwBattleIndex;
    stDT_YABIAO_BATTLELOG_DATA.byQuality = byQuality;
    stDT_YABIAO_BATTLELOG_DATA.dwJingjieChange = dwJingjie;
    pYabiaoBattleLog->Init(stDT_YABIAO_BATTLELOG_DATA, m_poOwner );
    m_deqYabiaoBattleLog.push_back( pYabiaoBattleLog );

    while( m_deqYabiaoBattleLog.size() > MAX_YABIAO_BATTLELOG_NUM )
    {
        CYabiaoBattleLog* pYabiaoBattleLog = m_deqYabiaoBattleLog.front();
        CYabiaoBattleLogPoolMgr::Instance()->Remove(pYabiaoBattleLog);
        m_deqYabiaoBattleLog.pop_front();
    }

    //DBG_INFO( _SDT( "logging yabiao,player ID:%u flag:%d result:%d coin:%u Science:%u enemy player ID:%u battle time:%llu battle log index:%llu newflag:%d jingjie:%u" ),
    //          m_poOwner->GetID(), data.byFlag, data.byResult, data.dwCoinChange, data.dwScienceChange, data.dwEnemyPlayerID, data.qwBattleTime,
    //          data.qwBattleDataIndex, data.byNewFlag, data.dwJingjieChange );
}

UINT16 CYabiaoInfo::FlushBiaoInfo()
{
    Check4Reset();
    if( GetYabiaoFlag() != ms_nYabiaoProcessFlagStand )
    {
        //USR_IMPORTANT( _SDT( "player:%d, flush quality failed,cannot flush in yabiao status！" ), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        return ERR_FLUSH_BIAO_QUALITY::ID_YABIAOING;
    }
    if( IsBiaoQualityMaxLevel() )
    {
        //DBG_INFO( _SDT( "玩家:%d, 镖已经是最好的品质，不能刷新！" ), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        return ERR_FLUSH_BIAO_QUALITY::ID_MAX_QUALITY_LEVEL;
    }

    UINT16 nCostGold = GetFlushCostGold();
    if( m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < nCostGold )
    {
        //DBG_INFO( _SDT( "玩家:%d 元宝不够刷新品质" ), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    if( nCostGold )
    {
        m_poOwner->DecGold( nCostGold, CRecordMgr::EDGT_FLUSHBIAOINFO,  m_stDT_YABIAO_BASE_DATA.dwAlreadyFlushBiaoCnt);
        m_stDT_YABIAO_BASE_DATA.wBiaoFlushCount++;
    }
    m_stDT_YABIAO_BASE_DATA.dwAlreadyFlushBiaoCnt++;
    m_stDT_YABIAO_BASE_DATA.qwLastFlushTime = SDTimeSecs();
    //成功则升级，失败则降级
    if( CYabiaoMgr::Instance()->FlushToUpdateQuality( m_stDT_YABIAO_BASE_DATA.byQuality ) )
    {
        m_stDT_YABIAO_BASE_DATA.byQuality += 1;
// 		DBG_INFO( _SDT( "玩家:%d,升级品质成功!当前品质:%d" ),
// 			m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_YABIAO_BASE_DATA.byQuality );
    }
    else
    {
        if( m_stDT_YABIAO_BASE_DATA.byQuality > 0 )
        {
            m_stDT_YABIAO_BASE_DATA.byQuality -= 1;
// 			DBG_INFO( _SDT( "玩家:%d,升级品质失败!当前品质:%d" ),
// 				m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_YABIAO_BASE_DATA.byQuality );
        }
        else
        {
            SYS_CRITICAL( _SDT( "player:%d,update quality failed!But quality level not zero，please check config" ),
                          m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        }

    }
    return ERR_FLUSH_BIAO_QUALITY::ID_SUCCESS;
}

UINT16 CYabiaoInfo::DirectMaxLevel()
{
    if(!CVipPropMgr::Instance()->CkOpenFunc(EVF_QUJINGCALLTANGSENG , m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel))
    {
        return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
    }
    if( GetYabiaoFlag() != ms_nYabiaoProcessFlagStand )
    {
        //USR_IMPORTANT( _SDT( "玩家:%d, 刷新品质失败,押镖中不能刷新！" ), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        return ERR_FLUSH_BIAO_QUALITY::ID_YABIAOING;
    }
    if( IsBiaoQualityMaxLevel() )
    {
        //DBG_INFO( _SDT( "玩家:%d, 镖已经是最好的品质，不能刷新！" ), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        return ERR_FLUSH_BIAO_QUALITY::ID_MAX_QUALITY_LEVEL;
    }

    UINT16 nCostGold = CYabiaoMgr::Instance()->GetDirectMaxLevelCostGold();
    if( m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < nCostGold )
    {
        //DBG_INFO( _SDT( "玩家:%d 元宝不够刷新品质" ), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    m_poOwner->DecGold( nCostGold, CRecordMgr::EDGT_FLUSHBIAOINFOMAXLEVEL );

    m_stDT_YABIAO_BASE_DATA.byQuality = CYabiaoMgr::Instance()->GetMaxQualityLevel();

    return ERR_FLUSH_BIAO_QUALITY::ID_SUCCESS;
}

BOOL CYabiaoInfo::IsBiaoQualityMaxLevel()
{
    return m_stDT_YABIAO_BASE_DATA.byQuality >= CYabiaoMgr::Instance()->GetMaxQualityLevel();
}

UINT16 CYabiaoInfo::GetCurBiaoQuality()
{
    return m_stDT_YABIAO_BASE_DATA.byQuality;
}

PKT_CLIGS_BEGIN_JIEBIAO_ACK5 *CYabiaoInfo::GetJiebiaoCache5( INT32 nIndex )
{
    if( m_nCacheJiebiaoRemainJiebiaoIndex != nIndex )
    {
        return NULL;
    }

    return &m_stBeginJiebiaoAck5;
}

PKT_CLIGS_BEGIN_JIEBIAO_ACK3 *CYabiaoInfo::GetJiebiaoCache3( INT32 nIndex )
{
    if( m_nCacheJiebiaoRemainJiebiaoIndex != nIndex )
    {
        return NULL;
    }

    return &m_stBeginJiebiaoAck3;
}

vector<UINT32> CYabiaoInfo::GetJiebiaoPlayerID()
{
    return m_vecJiebiaoPlayerID;
}

VOID CYabiaoInfo::SetJiebiaoCache5( INT32 nIndex, const PKT_CLIGS_BEGIN_JIEBIAO_ACK5 &stBeginJiebiaoAck )
{
    if( m_nCacheJiebiaoRemainJiebiaoIndex == nIndex )
    {
        return;
    }

    m_nCacheJiebiaoRemainJiebiaoIndex = nIndex;
    m_stBeginJiebiaoAck5 = stBeginJiebiaoAck;
}

VOID CYabiaoInfo::SetJiebiaoCache3( INT32 nIndex, const PKT_CLIGS_BEGIN_JIEBIAO_ACK3 &stBeginJiebiaoAck )
{
    if( m_nCacheJiebiaoRemainJiebiaoIndex == nIndex )
    {
        return;
    }

    m_nCacheJiebiaoRemainJiebiaoIndex = nIndex;
    m_stBeginJiebiaoAck3 = stBeginJiebiaoAck;
}

DT_YABIAO_BASE_DATA &CYabiaoInfo::GetDT_YABIAO_BASE_DATA()
{
    return m_stDT_YABIAO_BASE_DATA;
}

VOID CYabiaoInfo::SetJiebiaoCache4( INT32 nIndex, const PKT_CLIGS_BEGIN_JIEBIAO_ACK4 &stBeginJiebiaoAck )
{
	if( m_nCacheJiebiaoRemainJiebiaoIndex == nIndex )
	{
		return;
	}

	m_nCacheJiebiaoRemainJiebiaoIndex = nIndex;
	m_stBeginJiebiaoAck4 = stBeginJiebiaoAck;
}
PKT_CLIGS_BEGIN_JIEBIAO_ACK4 *CYabiaoInfo::GetJiebiaoCache4( INT32 nIndex )
{
	if( m_nCacheJiebiaoRemainJiebiaoIndex != nIndex )
	{
		return NULL;
	}

	return &m_stBeginJiebiaoAck4;
}

INT32 CYabiaoInfo::Jiebiao(BOOL bSuccess, time_t curTime, UINT32 dwEnemyPlayerID, UINT32 dwCoin,
                           UINT32 dwScience, UINT64 qwBattleIndex, UINT8 byQuality, UINT64 qwEncourageCoin, UINT32 dwJingjie)
{


    AddYabiaoBattleLog( ms_nYabiaoFlagJiebiao, curTime, dwEnemyPlayerID, !bSuccess, dwCoin, dwScience, qwBattleIndex, byQuality, dwJingjie, FALSE );
    if( m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount >= CYabiaoMgr::Instance()->GetTotalJiebiaoCnt() )
    {
        return ERR_JIEBIAO::ID_MAX_JIEBIAO_COUNT_ERROR;
    }

	if( !IsToday( m_stDT_YABIAO_BASE_DATA.qwLastJiebiaoTime ) )
	{
		m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount = 0;
	}

    m_poOwner->AddCoin( (UINT32)(qwEncourageCoin), CRecordMgr::EACT_JIEBIAO );

    //劫镖成功才添加劫镖次数
    if( bSuccess )
    {
        ++m_stDT_YABIAO_BASE_DATA.byAlreadyJiebiaoCount;
    }



    m_stDT_YABIAO_BASE_DATA.qwLastJiebiaoTime = SDTimeSecs();
    return ERR_JIEBIAO::ID_SUCCESS;
}

UINT64 CYabiaoInfo::GetJiebiaoCoinEncourage( UINT16 wYabiaoLevel, UINT16 wJiebiaoLevel )
{
    return GetJiebiaoEncourage( wYabiaoLevel, wJiebiaoLevel, GetCurBiaoInfo().dwCoin );
}

UINT32 CYabiaoInfo::GetJiebiaoScienceEncourage( UINT16 wYabiaoLevel, UINT16 wJiebiaoLevel )
{
    return 0;
    //return UINT32( GetJiebiaoEncourage( wYabiaoLevel, wJiebiaoLevel, GetCurBiaoInfo().dwScience ) );
}

UINT64 CYabiaoInfo::GetJiebiaoEncourage( UINT16 wYabiaoLevel, UINT16 wJiebiaoLevel, UINT64 dwYabiaoEncourage )
{
    if( wJiebiaoLevel == 0 || wYabiaoLevel == 0 )
    {
        return 0;
    }

    if( wYabiaoLevel - wJiebiaoLevel > 10 )
    {
        if( wYabiaoLevel == 10 )
        {
            return 0;
        }

        return UINT64( ( wYabiaoLevel / ( wYabiaoLevel - 10 ) ) * 0.1 * dwYabiaoEncourage );
    }
    else if( wJiebiaoLevel < wYabiaoLevel )
    {
        return UINT64( float(wYabiaoLevel) / wJiebiaoLevel * 0.1 * dwYabiaoEncourage );
    }
    else if( wJiebiaoLevel < wYabiaoLevel + 11 )
    {
        return UINT64( ( float(wYabiaoLevel) / wJiebiaoLevel ) * 0.1 * dwYabiaoEncourage );
    }

    return 0;
}

BOOL CYabiaoInfo::BeJiebiao( BOOL bSuccess, time_t curTime, UINT32 dwEnemyPlayerID, UINT32 dwCoin,
                             UINT32 dwScience, UINT64 qwBattleIndex, UINT8 byQuality, UINT32 dwJingjie )
{
    AddYabiaoBattleLog( ms_nYabiaoFlagBeJiebiao, curTime, dwEnemyPlayerID, bSuccess, dwCoin, dwScience, qwBattleIndex, byQuality, dwJingjie );
    if( !bSuccess )
    {
        return TRUE;
    }
    if( GetRemainBeJiebiaoCnt() <= 0 )
    {
// 		USR_IMPORTANT( _SDT( "玩家:%u 被劫镖次数已达上限，当前被劫镖数:%d,最大被劫镖数:%d" ),
// 			m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_YABIAO_BASE_DATA.byBeJiebiaoCount,
// 			CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt() );
        return FALSE;
    }

    if( bSuccess )
    {
        ++m_stDT_YABIAO_BASE_DATA.byBeJiebiaoCount;
        m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecCoin += dwCoin;
        m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecScience += dwScience;
        m_vecJiebiaoPlayerID.push_back( dwEnemyPlayerID );
    }

    return TRUE;
}

CYabiaoInfo::CYabiaoBattleLogDeq & CYabiaoInfo::GetYabiaoBattleLog()
{
    return m_deqYabiaoBattleLog;
}

VOID CYabiaoInfo::OnYabiaoBattleLogRead()
{
    for( UINT32 dwIndex = 0; dwIndex < m_deqYabiaoBattleLog.size(); ++dwIndex )
    {
        if( m_deqYabiaoBattleLog[dwIndex]->GetDT_PAOSHANG_BATTLELOG_DATA().byNewFlag )
        {
            m_deqYabiaoBattleLog[dwIndex]->GetDT_PAOSHANG_BATTLELOG_DATA().byNewFlag = FALSE;
        }
    }
}

UINT64 CYabiaoInfo::GetLastEncourageCoin()
{
    return m_stDT_YABIAO_BASE_DATA.qwLastEncourageCoin;
}

UINT64 CYabiaoInfo::GetLastEncourageScience()
{
    return m_stDT_YABIAO_BASE_DATA.qwLastEncourageScience;
}

UINT64 CYabiaoInfo::GetLastEncourageJingjie()
{
    return m_stDT_YABIAO_BASE_DATA.qwLastEncourageJingjie;
}

INT32 CYabiaoInfo::OnObtainEncourage()
{
    if( m_stDT_YABIAO_BASE_DATA.byYabiaoFlag != ms_nYabiaoProcessFlagSuccess )
    {
        return ERR_GET_YABIAO_ENCOURAGE::ID_TIME_NOT_DOWN;
    }

    m_stDT_YABIAO_BASE_DATA.byYabiaoFlag = ms_nYabiaoProcessFlagStand;

    m_poOwner->AddCoin( (UINT32)(m_stDT_YABIAO_BASE_DATA.qwLastEncourageCoin), CRecordMgr::EACT_YABIAO, m_stDT_YABIAO_BASE_DATA.byQuality, m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecCoin);
    m_poOwner->AddScience( (UINT32)(m_stDT_YABIAO_BASE_DATA.qwLastEncourageScience), CRecordMgr::EAST_YABIAO, m_stDT_YABIAO_BASE_DATA.byQuality, m_stDT_YABIAO_BASE_DATA.dwJiebiaoDecScience);
    m_poOwner->AddJingJie( (UINT32)(m_stDT_YABIAO_BASE_DATA.qwLastEncourageJingjie), CRecordMgr::EAJT_YABIAO, m_stDT_YABIAO_BASE_DATA.byQuality);
    ResetYabiaoInfo();

    return ERR_GET_YABIAO_ENCOURAGE::ID_SUCCESS;
}


//INT32 CYabiaoInfo::GetRemainJiebiaoCnt()
//{
//    return GetRemainJiebiaoCnt();
//}

//CYabiaoInfo *CPlayer::GetYabiaoInfo()
//{
//    return m_poYabiaoInfo;
//}

INT32 CYabiaoInfo::BeginYabiao()
{
    INT32 nRet = BeginYabiao( SDTimeSecs() );
    if( nRet == 0 )
    {
        m_poOwner->AddGuideRecord( EGF_YABIAO);
    }
    return nRet;
}

BOOL CYabiaoInfo::IsYabiao()
{
    return GetYabiaoFlag();
}
