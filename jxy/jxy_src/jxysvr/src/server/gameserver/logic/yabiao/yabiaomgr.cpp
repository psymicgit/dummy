#include "yabiaomgr.h"
#include <logic/base/dbconfig.h>
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <common/server/utility.h>
#include <logic/other/singleprammgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/faction/faction.h>
#include <logic/faction/factionmgr.h>
#include "dll/sdframework/sdapplication.h"
#include <algorithm>
#include <dll/sdframework/sdutil.h>
#include "logic/other/playerbattledata2recordmgr.h"
#include "logic/other/playerbattledatarecordmgr.h"
#include "logic/yabiao/yabiaoinfo.h"
#include <framework/gsapi.h>
#include <algorithm>
#include "common/server/ngstring.h"
#include <errdef.h>
#include <logic/activity/couragemgr.h>
#include "logic/other/fewparam.h"
#define SQL_READ_BIAO_INFO_STRING "select * from yabiaoinfo"
#define SQL_READ_BIAO_COST_STRING "select * from yabiaocost"
#define SQL_READ_SINGLE_PARAM_STRING "select * from singlepram"

IMPLEMENT_SINGLETON_PROPMGR(CYabiaoMgr);

CYabiaoMgr::CYabiaoMgr()
{

}

CYabiaoMgr::~CYabiaoMgr()
{

}

BOOL CYabiaoMgr::Init()
{
    if(!LoadFromDBEx())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}

VOID CYabiaoMgr::UnInit()
{
    m_vecYabiaoPlayer.clear();
    m_setYabiaoPlayer.clear();
    m_oCfgInfo.Reset();
    m_mapLevel2PlayerVecMap.clear();
}


UINT8 CYabiaoMgr::GetTotalYabiaoCnt()
{
    return CSinglePramMgr::Instance()->GetTotalYabiaoCnt();
}

UINT8 CYabiaoMgr::GetTotalJiebiaoCnt()
{
    return CSinglePramMgr::Instance()->GetTotalJiebiaoCnt();
}

UINT8 CYabiaoMgr::GetTotalBeJiebiaoCnt()
{
    return CSinglePramMgr::Instance()->GetTotalBeJiebiaoCnt();
}

BOOL CYabiaoMgr::LoadFromDBEx()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("yabiaoinfo").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord())// 当有行结果时
    {
        DOUBLE dbCoinFactor = SGDP::SDStrtod(pRes->GetFieldValueByName("CoinFactor"), NULL);
        INT32 nScience = SGDP::SDAtou(pRes->GetFieldValueByName("Science"));
        INT32 nCostTime = SGDP::SDAtou(pRes->GetFieldValueByName("CostTime"));
        INT32 nFlushUpdateRate = SGDP::SDAtou(pRes->GetFieldValueByName("FlushUpdateRate"));
        INT32 nJingjie = SGDP::SDAtou(pRes->GetFieldValueByName("Jingjie"));
        SBiaoBaseInfo info;
        info.oEncourage = SYabiaoEncourage( dbCoinFactor, nScience, nJingjie );
        info.nTime = nCostTime;
        info.nFlushUpdateRate = nFlushUpdateRate;
        m_oCfgInfo.vecBiaoInfo.push_back( info );
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("yabiaocost").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while( pRes->GetRecord() )
    {
        INT32 nGold = SGDP::SDAtou( pRes->GetFieldValueByName("Gold"));

        m_oCfgInfo.vecFlushCostGold.push_back( nGold );
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

//获得最高品质的等级
INT32 CYabiaoMgr::GetMaxQualityLevel()
{
    return m_oCfgInfo.vecBiaoInfo.size() - 1;
}

//获得对应等级押镖所需的时间(等级从1开始)
INT32 CYabiaoMgr::GetCostTime( INT32 nQuality )
{
    if( nQuality >= INT32( m_oCfgInfo.vecBiaoInfo.size() ) )
    {
        return -1;
    }


    return m_oCfgInfo.vecBiaoInfo[nQuality].nTime;
}

//直接最高等级消耗的元宝
INT32 CYabiaoMgr::GetDirectMaxLevelCostGold()
{
    return CSinglePramMgr::Instance()->GetMaxLevelCostGold();
}

//获得刷新消耗的元宝
INT32 CYabiaoMgr::GetFlushCostGoldByCount( INT32 nCount )
{
    if( nCount >= (INT32)m_oCfgInfo.vecFlushCostGold.size() )
    {
        return m_oCfgInfo.vecFlushCostGold[m_oCfgInfo.vecFlushCostGold.size() - 1];
    }
    return m_oCfgInfo.vecFlushCostGold[nCount];
}


SYabiaoEncourage CYabiaoMgr::GetEncourage( INT32 nQuality )
{
    if( nQuality < 0 || nQuality >= (INT32)m_oCfgInfo.vecBiaoInfo.size() )
    {
        //USR_IMPORTANT( "品质超出,nQuality=%d", nQuality );
        return SYabiaoEncourage( 0.0, 0, 0 );
    }
    return m_oCfgInfo.vecBiaoInfo[nQuality].oEncourage;
}

INT32 CYabiaoMgr::AddPlayer( UINT32 dwPlayer )
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer( dwPlayer );
    if( poPlayer == NULL )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: not Found PlayerID:%d "), MSG_MARK, dwPlayer);
        return -1;
    }

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if (NULL == poYabiaoInfo)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: not Found PlayerID:%d "), MSG_MARK, dwPlayer);
        return -1;
    }

    if( poYabiaoInfo->GetRemainYabiaoCnt() == 0 )
    {
        return ERR_BEGIN_YABIAO::ID_MAX_YABIAO_COUNT_ERROR;
    }


    INT32 nRet = poYabiaoInfo->BeginYabiao();
    if( nRet == 0 )
    {
        //正常开始，才添加数据
        AddPlayerToContainer( poPlayer );
    }

    return nRet;
}

BOOL CYabiaoMgr::DeletePlayer( UINT32 dwPlayer )
{
    //因为缓存了玩家的数据是一个小时，而押镖时间是在一个小时以内，所以不可能会查找不到玩家
    vector< UINT32 >::iterator itrFind = std::find( m_vecYabiaoPlayer.begin(), m_vecYabiaoPlayer.end(), dwPlayer );
    if( itrFind != m_vecYabiaoPlayer.end() )
    {
        //删除玩家
        //DBG_INFO( _SDT( "玩家:%d 删除押镖状态" ), dwPlayer );
        CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer( *itrFind );
        m_vecYabiaoPlayer.erase(itrFind);
        if( poPlayer )
        {
            set< UINT32 >::iterator itrSetFind = m_setYabiaoPlayer.find( dwPlayer );
            if( itrSetFind != m_setYabiaoPlayer.end() )
            {
                m_setYabiaoPlayer.erase( itrSetFind );
            }
            CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
            if (poYabiaoInfo)
            {
                poYabiaoInfo->EndYabiao();
            }


            C32Vec& vecPlayer = m_mapLevel2PlayerVecMap[poPlayer->GetLevel()];
            UINT32 dwSize = vecPlayer.size();
            for(C32VecItr itr = vecPlayer.begin(); itr != vecPlayer.end(); itr++)
            {
                if(*itr == dwPlayer)
                {
                    vecPlayer.erase(itr);
                    break;
                }
            }

            return TRUE;
        }

        //DBG_INFO( _SDT( "玩家:%d 在set中找不到" ), dwPlayer );
    }
    else
    {
        //DBG_INFO( _SDT( "玩家:%d 在vector中找不到" ), dwPlayer );
    }

    return FALSE;
}

UINT32 CYabiaoMgr::GetPlayerIDByIndex( INT32 nIndex )
{
    if( INT32( m_vecYabiaoPlayer.size() ) <= nIndex )
    {
        return 0;
    }

    return m_vecYabiaoPlayer[nIndex];
}

INT32 CYabiaoMgr::GetPlayerCount()
{
    return m_vecYabiaoPlayer.size();
}

vector<UINT32> CYabiaoMgr::GetEnemy(UINT16 wPlayerLevel, const set<UINT32> &setEnemy, UINT8 byNum, UINT16 wTimeToEnd )
{
    vector<UINT32> vecFind;
    vecFind.resize( m_vecYabiaoPlayer.size() );
    set_intersection( m_setYabiaoPlayer.begin(), m_setYabiaoPlayer.end(), setEnemy.begin(), setEnemy.end(), vecFind.begin() );

    return GetLimitYabiaoPlayer( wPlayerLevel, TRUE, byNum ? byNum : CSinglePramMgr::Instance()->GetJiebiaoViewPlayerCnt(), vecFind, wTimeToEnd );
}

vector<UINT32> CYabiaoMgr::GetYabiaoPlayer(UINT16 wPlayerLevel, UINT16 wTimeToEnd, UINT8 byNum )
{
    //个数限制
    return GetLimitYabiaoPlayer( wPlayerLevel, FALSE, byNum ? byNum : CSinglePramMgr::Instance()->GetJiebiaoViewPlayerCnt(), m_vecYabiaoPlayer, wTimeToEnd );
}

BOOL CYabiaoMgr::FlushToUpdateQuality( INT32 nQuality )
{
    if( nQuality >= (INT32)m_oCfgInfo.vecBiaoInfo.size() )
    {
        SYS_CRITICAL( _SDT( "no cfg Quality:%d" ), nQuality );
        return FALSE;
    }

    INT32 nRandomResult = RandomReseed(100);

    //随机数范围0~99,从0开始，所以计算的时候都从0开始计算
    if( nRandomResult < m_oCfgInfo.vecBiaoInfo[nQuality].nFlushUpdateRate )
    {
        //命中
        return TRUE;
    }

    return FALSE;
}

bool SortByTime( const DT_YABIAO_PLAYER_INFO &info1, const DT_YABIAO_PLAYER_INFO &info2)
{
    return info1.wAlreadyYabiaoTime > info2.wAlreadyYabiaoTime;
}

BOOL CYabiaoMgr::FillPlayerYabiaoInfo( PKT_CLIGS_OPEN_JIEBIAO_ACK &ack, const vector<UINT32> &vecPlayer, UINT32 dwViewerID, UINT16 wJiebiaoLevel,CPlayer *pOwner )
{
    UINT32 nIndex = 0;

    for( ; nIndex < vecPlayer.size() && nIndex < MAX_YABIAO_PLAYER_NUM; ++nIndex )
    {
        if( 0 == vecPlayer[nIndex] )
        {
            continue;
        }

        CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer( vecPlayer[nIndex] );
        if( NULL == pPlayer )
        {
            DBG_INFO( _SDT( "[%s %d]player not found，ID:%d" ), MSG_MARK, vecPlayer[nIndex] );
            continue;
        }
        CYabiaoInfo *poYabiaoInfo = pPlayer->GetYabiaoInfo();
        if (NULL == poYabiaoInfo)
        {
            DBG_INFO( _SDT( "[%s %d]player not found yabiao，ID:%d" ), MSG_MARK, vecPlayer[nIndex] );
            continue;
        }

        strcpy( ack.astYabiaoPlayerInfo[nIndex].aszDispName, pPlayer->GetDT_PLAYER_BASE_DATA().aszDispName );
        ack.astYabiaoPlayerInfo[nIndex].stBeJiebiaoInfo.byRemianOperateCnt = (UINT8)poYabiaoInfo->GetRemainBeJiebiaoCnt();
        ack.astYabiaoPlayerInfo[nIndex].stBeJiebiaoInfo.byTotalOperateCnt = GetTotalBeJiebiaoCnt();
        ack.astYabiaoPlayerInfo[nIndex].dwID = vecPlayer[nIndex];
        ack.astYabiaoPlayerInfo[nIndex].dwPower = pPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
        ack.astYabiaoPlayerInfo[nIndex].qwCoinEncourage = poYabiaoInfo->GetJiebiaoCoinEncourage( pPlayer->GetLevel(), wJiebiaoLevel );
        ack.astYabiaoPlayerInfo[nIndex].dwScienceEncourage = poYabiaoInfo->GetJiebiaoScienceEncourage( pPlayer->GetLevel(), wJiebiaoLevel );
        ack.astYabiaoPlayerInfo[nIndex].wAlreadyYabiaoTime = poYabiaoInfo->GetYabiaoPassedTime();

        DT_BIAO_INFO biaoInfo = poYabiaoInfo->GetCurBiaoInfo();
        ack.astYabiaoPlayerInfo[nIndex].wTotalYabiaoTime = biaoInfo.wCostTime;
        ack.astYabiaoPlayerInfo[nIndex].byQuality = biaoInfo.byQualityLevel;
        ack.astYabiaoPlayerInfo[nIndex].wLevel = pPlayer->GetLevel();
        ack.astYabiaoPlayerInfo[nIndex].wKindID = pPlayer->GetCoachHeroKindID();

        vector<UINT32> vecPlayerList = poYabiaoInfo->GetJiebiaoPlayerID();
        vector<UINT32>::iterator itrFind = std::find( vecPlayerList.begin(), vecPlayerList.end(), dwViewerID );

        if( itrFind == vecPlayerList.end() )
        {
            ack.astYabiaoPlayerInfo[nIndex].byCanJiebiao = TRUE;
        }
        else
        {
            ack.astYabiaoPlayerInfo[nIndex].byCanJiebiao = FALSE;
        }
    }

    std::sort( ack.astYabiaoPlayerInfo, ack.astYabiaoPlayerInfo + MAX_YABIAO_PLAYER_NUM, SortByTime );
    ack.byYabiaoPlayerNum = nIndex;

	nIndex = 0;

	if (NULL == pOwner)
	{
		DBG_INFO( _SDT( "[%s %d]pOwner not found yabiao" ), MSG_MARK );
		return FALSE;
	}

	CYabiaoInfo *poMyYabiaoInfo = pOwner->GetYabiaoInfo();

	if (NULL == poMyYabiaoInfo)
	{
		DBG_INFO( _SDT( "[%s %d]poMyYabiaoInfo not found yabiao" ), MSG_MARK );
		return FALSE;
	}

	UINT32 dwMyFactionID = 0;
	CFaction::CkFactionPlayerData(dwMyFactionID,pOwner->GetDT_PLAYER_BASE_DATA().dwID);

	for( ; nIndex < MAX_YABIAO_PLAYER_NUM; ++nIndex )
	{
		if( 0 == ack.astYabiaoPlayerInfo[nIndex].dwID )
		{
			continue;
		}

		CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer( ack.astYabiaoPlayerInfo[nIndex].dwID );

		if( NULL == pPlayer )
		{
			continue;
		}

		CYabiaoInfo *poYabiaoInfo = pPlayer->GetYabiaoInfo();
		if (NULL == poYabiaoInfo)
		{
			continue;
		}

		UINT32 dwFactionID = 0;

		BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, pPlayer->GetDT_PLAYER_BASE_DATA().dwID);
		if ( bRet )
		{
			CFaction *pCFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);

			if(pCFaction != NULL)
			{
				strcpy( ack.astYaBiaoFactionName[nIndex].aszFactionName,  pCFaction->GetDT_FACTION_BASE_DATA().aszFactionName );

				if (dwFactionID == dwMyFactionID)
				{
					ack.astYaBiaoFactionName[nIndex].bySameFaction = TRUE;
				}
				ack.abyFactionIconID[nIndex] = pCFaction->GetFactionIconID();
			}
		}
	}

    return TRUE;
}

UINT16 CYabiaoMgr::GetJiebiaoCD()
{
    return CSinglePramMgr::Instance()->GetJiebiaoCD();
}

INT32 CYabiaoMgr::BeginJiebiao( CPlayer &playerSelf, UINT32 dwBeJiebiaoPlayerID, UINT64 &qwEncourageCoin, UINT32 &dwEncourageScience,
                                DT_BATTLE_DATA5 &stDT_BATTLE_DATA, UINT64 &qwBattleIndexOut )
{
    qwEncourageCoin = 0;
    dwEncourageScience = 0;

    CYabiaoInfo *poSelfYabiaoInfo = playerSelf.GetYabiaoInfo();
    if (NULL == poSelfYabiaoInfo)
    {
        return ERR_JIEBIAO::ID_PLAYER_MAX_BE_JIEBIAO_COUNT_ERROR;
    }

    INT32 nRemainJiebiaoCnt = poSelfYabiaoInfo->GetRemainJiebiaoCnt();

    if( nRemainJiebiaoCnt <= 0 )
    {
        //劫镖次数用尽
        return ERR_JIEBIAO::ID_MAX_JIEBIAO_COUNT_ERROR;
    }

    CPlayer* pPlayerBeJiebiao = CPlayerMgr::Instance()->FindPlayer( dwBeJiebiaoPlayerID );
    if( NULL == pPlayerBeJiebiao )
    {
        return ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
    }

    CYabiaoInfo *poBeYabiaoInfo = pPlayerBeJiebiao->GetYabiaoInfo();
    if (NULL == poBeYabiaoInfo)
    {
        return ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
    }

    vector<UINT32> vecBejiebiaoPlayerID = poBeYabiaoInfo->GetJiebiaoPlayerID();
    if( std::find( vecBejiebiaoPlayerID.begin(), vecBejiebiaoPlayerID.end(), playerSelf.GetID() ) != vecBejiebiaoPlayerID.end() )
    {
        return ERR_JIEBIAO::ID_CANNOT_REPEAT_JIEBIAO_PLAYER;
    }

    UINT8 byRemainBeJiebiaoCnt = poBeYabiaoInfo->GetRemainBeJiebiaoCnt();
    if( byRemainBeJiebiaoCnt <= 0 )
    {
        return ERR_JIEBIAO::ID_PLAYER_MAX_BE_JIEBIAO_COUNT_ERROR;
    }

    UINT64 curTime = SDTimeSecs();
    if(!CBattleMgr::Instance()->Battle4(&playerSelf, pPlayerBeJiebiao, stDT_BATTLE_DATA, TRUE))
    {
        return ERR_JIEBIAO::ID_BATTLE_ERROR;
    }
	BOOL bSuccess = stDT_BATTLE_DATA.byBattleResult == EBT_SUCCESS ? TRUE : FALSE;
	CFewParam& oFewParam = playerSelf.GetFewParam();
	if ( bSuccess)
	{
		oFewParam.AddInterceptSuccess();
	}
	else
	{
		oFewParam.AddInterceptFailed();
	}

    UINT64 qwBattleIndex = 0;
    //CPlayerBattleDataRecordMgr::Instance()->RecordBattle( stDT_BATTLE_DATA, qwBattleIndex, playerSelf.GetID(), pPlayerBeJiebiao->GetID() );
    //统一用新表
    CPlayerBattleData2RecordMgr::Instance()->RecordBattle( stDT_BATTLE_DATA, qwBattleIndex, playerSelf.GetID(), pPlayerBeJiebiao->GetID() );
    qwBattleIndexOut = qwBattleIndex;

    //劫镖是否成功，1拦截成功
    BOOL bJiebiaoSuccess = ( stDT_BATTLE_DATA.byBattleResult == 0 );

    if( bJiebiaoSuccess )
    {
        //打赢了，公式计算获得奖励
        qwEncourageCoin = poBeYabiaoInfo->GetJiebiaoCoinEncourage( pPlayerBeJiebiao->GetLevel(), playerSelf.GetLevel() );

        dwEncourageScience = poBeYabiaoInfo->GetJiebiaoScienceEncourage( pPlayerBeJiebiao->GetLevel(), playerSelf.GetLevel() );
    }

    if( !poBeYabiaoInfo->BeJiebiao( bJiebiaoSuccess, curTime, playerSelf.GetID(),
                                    UINT32(qwEncourageCoin), dwEncourageScience , qwBattleIndex, (UINT8)poBeYabiaoInfo->GetCurBiaoQuality(), 0 ) )
    {
        return ERR_JIEBIAO::ID_PLAYER_MAX_BE_JIEBIAO_COUNT_ERROR;
    }

    INT32 nJiebiaoRet = poSelfYabiaoInfo->Jiebiao( bJiebiaoSuccess, curTime, dwBeJiebiaoPlayerID,
                        UINT32(qwEncourageCoin), dwEncourageScience, qwBattleIndex, UINT8( poBeYabiaoInfo->GetCurBiaoQuality() ), qwEncourageCoin, 0 );
    if( nJiebiaoRet != ERR_JIEBIAO::ID_SUCCESS )
    {
        return nJiebiaoRet;
    }

    //劫镖，相互加仇敌
    pPlayerBeJiebiao->AddEnemy(playerSelf.GetID());
    playerSelf.AddEnemy(pPlayerBeJiebiao->GetID());

    return ERR_JIEBIAO::ID_SUCCESS;
}

UINT32 CYabiaoMgr::CalcCoin( UINT8 byQuality, UINT16 wLevel )
{
    DOUBLE dbCoinFactor = GetEncourage( byQuality ).dbCoinFactor;

    //根据公式计算
    UINT32 dwResult = UINT32( dbCoinFactor * wLevel * 2000 + 5000 );

    //DBG_INFO( "quality:%d CoinFactor:%f Level:%d Result:%u", byQuality, dbCoinFactor, wLevel, dwResult );
    return dwResult;
}

vector<UINT32> CYabiaoMgr::GetLimitYabiaoPlayer( UINT16 wPlayerLevel, BOOL bGetEnemy, UINT32 dwGetNum, const vector<UINT32> &vecSrc, UINT16 wTimeToEnd )
{
    UINT32 nSrcBegin = 0;

    if( wTimeToEnd )
    {
        BOOL bFind = FALSE;
        //先获得大于指定时间到结束的玩家索引

        //CNGString ss;
        //ss += "TimeToEnd:";
        //ss += wTimeToEnd;
        for( UINT32 dwIndex = 0; dwIndex < vecSrc.size(); ++dwIndex )
        {
            if( 0 == vecSrc[dwIndex] )
            {
                continue;
            }

            CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( vecSrc[dwIndex] );
            if( NULL == pPlayer )
            {
                continue;
            }

            CYabiaoInfo *poYabiaoInfo = pPlayer->GetYabiaoInfo();
            if (NULL == poYabiaoInfo)
            {
                continue;
            }

            //距离镖结束还有多久
            UINT32 nRemainTime = poYabiaoInfo->GetCurBiaoInfo().wCostTime - poYabiaoInfo->GetYabiaoPassedTime();

            //ss += " Index:";
            //ss += dwIndex;
            //ss += "RemainTime:";
            //ss += nRemainTime;
            if( wTimeToEnd < nRemainTime )
            {
                nSrcBegin = dwIndex;
                bFind = TRUE;
                break;
            }
        }

        //DBG_INFO( "Yabiao Between Timer:%s", ss.str().c_str() );
        if( FALSE == bFind )
        {
            return vector<UINT32>();
        }
    }

    vector<UINT32> vecRet;
	C3232Map mapTmpPlayer;
	if( INT32(vecSrc.size() - nSrcBegin) <= dwGetNum )
	{
		for( UINT32 nIndex = nSrcBegin; nIndex < vecSrc.size(); ++nIndex )
		{
			UINT32 dwID = vecSrc[nIndex];
			if(mapTmpPlayer.find(dwID) != mapTmpPlayer.end())
			{
				continue;
			}
			mapTmpPlayer[dwID] = dwID;
			vecRet.push_back(dwID);
		}

		return vecRet;
	}

    UINT32 dwCount = vecRet.size();
    UINT32 dwIndex = nSrcBegin;
    if(0 == wTimeToEnd)
    {
        dwIndex = 0;
    }
    UINT32 dwSize = vecSrc.size();
    //人数不够多/获取仇敌，按时间
    if((dwSize < UINT32(CSinglePramMgr::Instance()->GetJiebiaoViewPlayerCnt()) * 5) || (dwGetNum < 4) || (bGetEnemy))
    {
        //均匀的获得时间
        UINT32 dwInterval = 0;

        if (vecRet.size() > dwIndex)
        {
            dwInterval = (vecRet.size() - dwIndex) / dwGetNum;
        }
        else
        {
            dwInterval = 1;
        }

        BOOL bRevert = FALSE;
        while( dwCount < (UINT32)dwGetNum )
        {
            if( dwIndex >= (INT32)dwSize)
            {
                if(!bRevert)
                {
                    dwIndex = 0;
                    bRevert = TRUE;
                }
                else
                {
                    break;
                }
            }

            try
            {
                UINT32 dwID = vecSrc.at(dwIndex);
                if(mapTmpPlayer.find(dwID) != mapTmpPlayer.end())
                {
                    continue;
                }
                vecRet.push_back( dwID );
            }
            catch (...)
            {
                break;
            }

            ++dwIndex;
            dwIndex += dwInterval;
            ++dwCount;
        }
    }
    //人数多,根据等级,3/4同等级，其他随机
    else
    {
        //3/4同等级
        C32Vec&	vecLevelSrcTmp = m_mapLevel2PlayerVecMap[wPlayerLevel];
        //C3232Map mapRandomPlayer;
        UINT32 dwNumTmp = dwGetNum * 3 / 4;
        UINT32 dwSrcSize = vecLevelSrcTmp.size();
        UINT8 byInterval = 1;
        UINT32 dwIdx = 0;
        if(dwSrcSize <= dwNumTmp)
        {
            byInterval = 1;
        }
        else
        {
            byInterval = dwSrcSize / dwNumTmp;
        }
        UINT32 dwNum = 0;
        for(dwIdx = RandomReseed(byInterval + 1); ((dwIdx < dwSrcSize) && (dwNum < dwNumTmp)); dwIdx += byInterval, dwNum++)
        {
            UINT32 dwID = vecLevelSrcTmp[dwIdx];
            if(mapTmpPlayer.find(dwID) != mapTmpPlayer.end())
            {
                continue;
            }
            vecRet.push_back( dwID);
            mapTmpPlayer[dwID] = dwID;
        }

        //其余再随机取，按时间间隔取
        dwSrcSize = m_vecYabiaoPlayer.size();
        dwNumTmp = dwGetNum - vecRet.size();
        UINT32 dwTmp = dwSrcSize - vecLevelSrcTmp.size();
        if(dwTmp <= dwNumTmp)
        {
            byInterval = 1;
        }
        else
        {
            byInterval = dwTmp / dwNumTmp;
        }
        BOOL bRevert = FALSE;
        dwCount = 0;
        dwIndex = RandomReseed(dwSrcSize);//随机一个位置上开始取
        UINT8 byNum = 0;
        while( (byNum < (UINT32)dwNumTmp) && (dwCount++ < dwSrcSize) )
        {
            if( dwIndex >= (INT32)dwSrcSize)
            {
                if(!bRevert)
                {
                    dwIndex = dwIndex - dwSrcSize;
                    if(dwIndex >= dwSrcSize)//防止异常
                    {
                        break;
                    }
                    bRevert = TRUE;
                }
                else
                {
                    break;
                }
            }

            UINT32 dwID = m_vecYabiaoPlayer[dwIndex];
            dwIndex += byInterval;
            if(mapTmpPlayer.find(dwID) != mapTmpPlayer.end())
            {
                continue;
            }
            vecRet.push_back(dwID);
            mapTmpPlayer[dwID] = dwID;
            byNum++;

        }
    }

    return vecRet;
}


DT_BIAO_INFO CYabiaoMgr::GetBiaoInfo( UINT8 byLevel, UINT16 wPlayerLevel )
{
    DT_BIAO_INFO info;
    info.byQualityLevel = byLevel;
    SYabiaoEncourage encourage;
    encourage = GetEncourage( info.byQualityLevel );
    info.dwCoin = CalcCoin( byLevel, wPlayerLevel );
    info.dwScience = 0; //encourage.nScience;
    info.dwJingjie = encourage.nJingjie;

    info.wCostTime = GetCostTime( info.byQualityLevel );
    return info;
}

// VOID CYabiaoMgr::ClearJiebiaoCache()
// {
// 	m_mapJiebiaoAckCache.clear();
// }

UINT64 CYabiaoMgr::GetJiebiaoCacheKey( UINT32 dwPlayerID, UINT32 dwCount )
{
    return UINT64(dwCount) << 32 | dwPlayerID;
}

// PKT_CLIGS_BEGIN_JIEBIAO_ACK *CYabiaoMgr::GetJiebiaoAck( UINT32 dwPlayerID, UINT32 dwCount )
// {
// 	auto *poPlayer=CPlayerMgr::Instance()->FindPlayer( dwPlayerID );
// 	if( NULL==poPlayer )
// 	{
// 		return NULL;
// 	}
//
// 	auto itrFind=m_mapJiebiaoAckCache.find( GetJiebiaoCacheKey( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, dwCount ) );
// 	if( itrFind!=m_mapJiebiaoAckCache.end() )
// 	{
// 		return &itrFind->second;
// 	}
//
// 	return NULL;
// }

// VOID CYabiaoMgr::AddJiebiaoAck( UINT32 dwPlayerID, UINT32 dwCount, const PKT_CLIGS_BEGIN_JIEBIAO_ACK &ack )
// {
// 	auto *poPlayer=CPlayerMgr::Instance()->FindPlayer( dwPlayerID );
// 	if( NULL==poPlayer )
// 	{
// 		return;
// 	}
// 	m_mapJiebiaoAckCache[ GetJiebiaoCacheKey( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, dwCount ) ]=ack;
// }

vector<SBiaoBaseInfo> CYabiaoMgr::GetBiaoInfoList()
{
    return m_oCfgInfo.vecBiaoInfo;
}

BOOL CYabiaoMgr::AddPlayerToContainer( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }
    UINT32 dwPlayerID = poPlayer->GetID();
    if( m_setYabiaoPlayer.find( dwPlayerID ) != m_setYabiaoPlayer.end() )
    {
        return FALSE;
    }
    else
    {
        m_vecYabiaoPlayer.push_back( dwPlayerID );
        m_setYabiaoPlayer.insert( dwPlayerID );
        m_mapLevel2PlayerVecMap[poPlayer->GetLevel()].push_back(dwPlayerID);
        return TRUE;
    }
}

