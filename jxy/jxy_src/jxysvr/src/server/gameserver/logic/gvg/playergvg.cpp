#include "logic/gvg/playergvg.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/player/player.h"
#include "logic/faction/factionprop.h"
#include "logic/formation/formation.h"
#include "logic/gvg/gvgvideomgr.h"
#include "logic/jingjie/jingjiepropmgr.h"
#include "logic/jingjie/jingjie.h"
#include "logic/gvg/local/gvgfactionmgr.h"
#include "logic/gvg/local/gvggiftmgr.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/player/playerslogmgr.h"

#include "common/client/errdef.h"
#include <sdloggerimpl.h>

IMPLEMENT_CLASS_POOL_MGR(CPlayerGVG)

BOOL CPlayerGVG::Init(DT_PLAYER_GVG_DATA& stGVGData, CPlayer* poOwner)
{
	m_poOwner = poOwner;
	m_stGVGData = stGVGData;
	UINT8 byIdx = 0;
	
	for ( byIdx = 0; byIdx < MAX_GAMBLE_NUM; byIdx++)
	{
		m_mapGambleData[m_stGVGData.astGambleLstInfo[byIdx].qwGameID] = m_stGVGData.astGambleLstInfo[byIdx];
	}

	for ( byIdx = 0; byIdx < m_stGVGData.stGetGiftData.byGiftNum && byIdx < MAX_GVG_GIFT_NUM; byIdx++)
	{
		m_setGiftKey.insert( m_stGVGData.stGetGiftData.aqwGiftKey[byIdx]);
	}

	//获取押注奖励
	GetGambleReward();
	
	return TRUE;
}

DT_PLAYER_GVG_DATA& CPlayerGVG::GetPlayerGVGData()
{
	memset(&m_stGVGData.stGetGiftData, 0, sizeof(m_stGVGData.stGetGiftData));
	UINT8 byIdx = 0;
	for ( C64SetItr itr = m_setGiftKey.begin() ; itr != m_setGiftKey.end() && byIdx < MAX_GVG_GIFT_NUM ; itr++, byIdx++)
	{
		m_stGVGData.stGetGiftData.aqwGiftKey[byIdx] = *itr;
	}
	m_stGVGData.stGetGiftData.byGiftNum = byIdx;

	return m_stGVGData;
}

VOID CPlayerGVG::ResetGambleInfo()
{
	memset(m_stGVGData.astGambleLstInfo, 0, sizeof(m_stGVGData.astGambleLstInfo));
	m_mapGambleData.clear();
}

VOID CPlayerGVG::GetGambleReward()
{
    GVG_STAGE eCurStage = CGvGBuild::Instance()->GetCurStage();
    if(GVG_7_END <= eCurStage || m_stGVGData.wSessionID != CGvGBuild::Instance()->GetCurSession())
    {
        if (ESM_NO == m_stGVGData.byIsGetGamble)
        {
            for ( CMapGambleDataItr itr = m_mapGambleData.begin(); itr != m_mapGambleData.end(); itr++)
            {
                BOOL bWin = 0;
                SGvGFightResult stResult;
                BOOL bFound = CGvGBuild::Instance()->GetGvGFightResult(itr->first, stResult);
                if (!bFound)
                {
                    continue;
                }

                if( GR_LEFT_WIN == stResult.eResult && itr->second.dwSupportFactionID == stResult.dwFactionID1)
                {
                    bWin++;
                }
                else if (GR_RIGHT_WIN == stResult.eResult && itr->second.dwSupportFactionID == stResult.dwFactionID2)
                {
                    bWin++;
                }

                if(!bWin)
                {
                    continue;
                }

                DT_PLAYER_RES_DATA stResData = {0};
                CGvGPropMgr::Instance()->GetGambleReward(stResData, stResult.eBigRound, m_poOwner->GetLevel());

                m_poOwner->AddReward( stResData.byResKind, (UINT32)stResData.qwResValue, CRecordMgr::EACT_GVG_GAMBLE);

                vector<UINT64> vec64;
                vector<string> vecStr;
                vec64.push_back(stResData.qwResValue);
                CPlayerLogMgr::Instance()->AddBattleLog(m_poOwner->GetID(), 0, EBLT_GVG, EGLT_GVG_GAMBLE, vec64, vecStr);
            }

            m_stGVGData.byIsGetGamble = ESM_YES;

            PKT_CLIGS_RESOURCE_SYNC_NTF stNtf;
            m_poOwner->GetSyncResource(stNtf.stResourceSync);
            m_poOwner->SendMsg((TCHAR*)&stNtf, CLIGS_RESOURCE_SYNC_NTF);
        }
    }
    else
    {
        return;
    }
	
	if (m_stGVGData.wSessionID != CGvGBuild::Instance()->GetCurSession() )
	{
		//清空，每一届开始清空
		ResetGambleInfo();
        m_stGVGData.byIsGetGamble = ESM_NO;
	}

	m_stGVGData.wSessionID = CGvGBuild::Instance()->GetCurSession();
}

UINT16 CPlayerGVG::OpenMyGamble(PKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK& stAck)
{
    CGvGPropMgr::Instance()->GetGambleReward(stAck.stWinRes, GVG_16_TO_8, 0);
    stAck.stWinRes.qwResValue = 0;

	for ( UINT8 byIdx = 0; byIdx < MAX_GAMBLE_NUM; byIdx++)
	{
		if ( 0 != m_stGVGData.astGambleLstInfo[byIdx].qwGameID && 0 != m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID)
		{
            SGvGFightResult stResult;
			if (!CGvGBuild::Instance()->GetGvGFightResult(m_stGVGData.astGambleLstInfo[byIdx].qwGameID, stResult))
			{
				continue;
			}

            DT_GAMBLE_GIFT_DATA &stAckGambleInfo = stAck.astGambleInfo[byIdx];
            BOOL bWin = ((GR_LEFT_WIN == stResult.eResult && (stResult.dwFactionID1 == m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID)) ||
                         (GR_RIGHT_WIN == stResult.eResult) && (stResult.dwFactionID2 == m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID));
			
			if ( GR_NOT_START == stResult.eResult)
			{
                // 比赛未开始
				stAckGambleInfo.byGambleType = EGGR_NOT_DONE;
				CGvGPropMgr::Instance()->GetGambleReward(stAckGambleInfo.stGambleReward, stResult.eBigRound, m_poOwner->GetLevel());
				
				GetGambleFactionInfo(m_stGVGData.astGambleLstInfo[byIdx].qwGameID, EGGR_NOT_DONE, m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID, stAck.stGambleGameInfo.astGameInfo[stAck.stGambleGameInfo.byGambleNum]);
				stAck.stGambleGameInfo.byGambleNum++;
				continue;
			}
            else if (bWin)
            {
                // 下注赢了
                stAckGambleInfo.byGambleType = EGGR_WIN;
                CGvGPropMgr::Instance()->GetGambleReward(stAckGambleInfo.stGambleReward, stResult.eBigRound, m_poOwner->GetLevel());
                GetGambleFactionInfo(m_stGVGData.astGambleLstInfo[byIdx].qwGameID, EGGR_WIN, m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID, stAck.stGambleGameInfo.astGameInfo[stAck.stGambleGameInfo.byGambleNum]);
                stAck.stGambleGameInfo.byGambleNum++;

                stAck.stWinRes.qwResValue += stAckGambleInfo.stGambleReward.qwResValue;
                continue;
            }
            else
            {
                // 下注输了
                stAckGambleInfo.byGambleType = EGGR_LOST;
                CGvGPropMgr::Instance()->GetGambleReward(stAckGambleInfo.stGambleReward, stResult.eBigRound, m_poOwner->GetLevel());

                GetGambleFactionInfo(m_stGVGData.astGambleLstInfo[byIdx].qwGameID, EGGR_LOST, m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID, stAck.stGambleGameInfo.astGameInfo[stAck.stGambleGameInfo.byGambleNum]);
                stAck.stGambleGameInfo.byGambleNum++;
            }
		}
		else
		{
            // 这个地方实在是不方便获取押注轮次
            GvGRound eRound = GVG_SLEEPING;
            switch(byIdx)
            {
            case 0:
                eRound = GVG_16_TO_8;
                break;
            case 1:
                eRound = GVG_8_TO_4;
                break;
            default:
                eRound = GVG_FINAL_WAR;
                break;
            }

			stAck.astGambleInfo[byIdx].byGambleType = EGGR_NOT_GAMBLE;
			CGvGPropMgr::Instance()->GetGambleReward(stAck.astGambleInfo[byIdx].stGambleReward, eRound, m_poOwner->GetLevel());			
		}
	}

	return ERR_GVG_OPEN_MY_GAMBLE::ID_SUCCESS;
}

UINT16 CPlayerGVG::GambleFactionAck(PKT_CLIGS_GVG_GAMBLE_REQ& stReq)
{
	SGvGFightResult stResult ;

	if( !CGvGBuild::Instance()->GetGvGFightResult(stReq.qwGambleID, stResult) )
	{
		return ERR_GVG_GAMBLE::ID_NOT_HAVE_BATTLE;
	}

	if ( EGLR_NOT_START != stResult.eResult)
	{
		return ERR_GVG_GAMBLE::ID_ALREAD_BATTLE;
	}
	
    DT_GVG_ROUND eGamble;
    eGamble.byBigRound   = stResult.eBigRound;
    eGamble.bySmallRound = stResult.bySmallRound;
    // CGvGBuild::Instance()->GetGambleRound();

	DT_GAMBLE_DATA stGambleData = {0};
	stGambleData.dwSupportFactionID = stReq.dwWinFactionID;
	stGambleData.qwGameID = stReq.qwGambleID;
	if( !AddGambleInfo(eGamble, stGambleData) )
	{
		return ERR_GVG_GAMBLE::ID_ALREAD_GAMBLE;
	}

    CGvGBuild::Instance()->AddFactionSupport(stReq.dwWinFactionID);

    DBG_INFO(_SDT("CPlayerGVG::GambleFactionAck: player[%u:%s] gamble at faction<%u> in gamble<%llu> of round<%u>, <current stage = %u>, session = %u."), 
        m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_poOwner->GetDT_PLAYER_BASE_DATA().aszDispName, stReq.dwWinFactionID, stReq.qwGambleID, 
        stResult.eBigRound, CGvGBuild::Instance()->GetCurRound(), CGvGBuild::Instance()->GetCurStage(), CGvGBuild::Instance()->GetCurSession());

	return ERR_GVG_GAMBLE::ID_SUCCESS;
}

UINT16 CPlayerGVG::GetGvGFormationPlayerTip(PKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK& stAck)
{
	CFaction* poFaction = m_poOwner->GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_READ_FORMATION_PLAYER_TIP::ID_NOT_JOIN_FACTION;
	}
	SDStrncpy(stAck.stTipInfo.aszDispPlayerName, m_poOwner->GetDT_PLAYER_BASE_DATA().aszDispName, USERNAME_LEN - 1);
	stAck.stTipInfo.byPlayerPosition = poFaction->GetJob(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID);
	stAck.stTipInfo.dwCoachID = m_poOwner->GetCoachHeroKindID();
	DT_DOORSTRIBUTE_LEVEL_DATA stLevelData = {0};
	UINT64 qwDoorsTribute = poFaction->GetSumDoorsTribute(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID);
	CFactorPropMgr::Instance()->GetDoorsTributeInfo( qwDoorsTribute,  stLevelData);
	stAck.stTipInfo.dwDoorsTributeLevel = stLevelData.wDoorsTributeLevel;
	stAck.stTipInfo.dwPlayerID = m_poOwner->GetDT_PLAYER_BASE_DATA().dwID;
	stAck.stTipInfo.dwPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	stAck.stTipInfo.wLevel = m_poOwner->GetLevel();
	m_poOwner->GetFormation().GetSimpleFormation(stAck.stTipInfo.stFrontRow, stAck.stTipInfo.stBackRow);
	CJingJie* poJingJie = m_poOwner->GetJingJie();
	if ( NULL != poJingJie)
	{
		stAck.stTipInfo.byColor = (UINT8)CJingJiePropMgr::Instance()->GetJingJieColor(poJingJie->GetDT_JINGJIE_DATA().stBaseData.wLevel);
	}
	return ERR_READ_FORMATION_PLAYER_TIP::ID_SUCCESS;
}
BOOL CPlayerGVG::AddGambleInfo(DT_GVG_ROUND& eGvGGamble, DT_GAMBLE_DATA& stData)
{
	CMapGambleDataItr itr = m_mapGambleData.find(stData.qwGameID);
	if ( itr != m_mapGambleData.end())
	{
		return FALSE;
	}
	UINT8 byIdx = 0;
	BOOL bRet = GetIdxByGambleRound(eGvGGamble, byIdx);
	if ( !bRet)
	{
		return FALSE;
	}

	if ( 0 != m_stGVGData.astGambleLstInfo[byIdx].qwGameID || 0 != m_stGVGData.astGambleLstInfo[byIdx].dwSupportFactionID)
	{
		return FALSE;
	}
	m_stGVGData.astGambleLstInfo[byIdx] = stData;
	m_mapGambleData[stData.qwGameID] = stData;
	return TRUE;
}


BOOL CPlayerGVG::GetGambleFactionInfo(UINT64 qwGambleID, GvGGambleResult eResult, UINT32 dwSuportFactionID, DT_GAMBLE_GAME_INFO& stInfo)
{
    SGvGFightResult stResult;
    if (!CGvGBuild::Instance()->GetGvGFightResult(qwGambleID, stResult))
    {
        return FALSE;
    }

	stInfo.stGameKind.byBigRound = stResult.eBigRound;
	stInfo.stGameKind.bySmallRound = stResult.bySmallRound;
	
	stInfo.byResult = eResult;
	stInfo.qwVideoID = stResult.videoID;

	//门派1信息
	DT_GVG_FACTION stFactionInfo = {0};
	CGvGFactionMgr::Instance()->GetFactionInfo( stResult.dwFactionID1, stFactionInfo);
	stInfo.stFactionInfo1.stFactionInfo = stFactionInfo.stBaseData;
	stInfo.stFactionInfo1.wZoneID = stFactionInfo.wZoneID;
	if ( dwSuportFactionID == stResult.dwFactionID1)
	{
		stInfo.stFactionInfo1.byIsGamble = ESM_YES;
	}
	
	//门派2信息
	memset( &stFactionInfo, 0, sizeof(DT_GVG_FACTION));
	CGvGFactionMgr::Instance()->GetFactionInfo( stResult.dwFactionID2, stFactionInfo);
	stInfo.stFactionInfo2.stFactionInfo = stFactionInfo.stBaseData;
	stInfo.stFactionInfo2.wZoneID = stFactionInfo.wZoneID;
	if ( dwSuportFactionID == stResult.dwFactionID2)
	{
		stInfo.stFactionInfo2.byIsGamble = ESM_YES;
	}

	//确认胜利门派
	if ( GR_LEFT_WIN == stResult.eResult )
	{
		stInfo.stFactionInfo1.byIsWin = ESM_YES;
	}
	else if ( GR_RIGHT_WIN == stResult.eResult)
	{
		stInfo.stFactionInfo2.byIsWin = ESM_YES;
	}
	return TRUE;
}

UINT16 CPlayerGVG::OpenMyGvGift(PKT_CLIGS_OPEN_GVG_GIFT_ACK& stAck)
{
	CGvGGiftMgr::Instance()->GetGiftInfo(m_poOwner->GetID(), m_setGiftKey, stAck.stGiftLstInfo);

	return ERR_OPEN_GVG_GIFT::ID_SUCCESS;
}

UINT16 CPlayerGVG::GetMyGift( PKT_CLIGS_GET_GVG_GIFT_REQ& sReq, PKT_CLIGS_GET_GVG_GIFT_ACK& stAck)
{
	UINT16 wRet = 0;
	if ( 0 == sReq.qwGiftID)
	{
		wRet = GetAllGift();
	}
	else
	{
		wRet = GetSingleGift(sReq.qwGiftID);
	}

	if ( ERR_GET_GVG_GIFT::ID_SUCCESS == wRet)
	{
		stAck.qwGiftID = sReq.qwGiftID;
		m_poOwner->GetSyncResource(stAck.stSyncInfo);
	}
	stAck.byLeftGiftNum = GetUnGetGiftNum();
	return wRet;
}

UINT8 CPlayerGVG::GetUnGetGiftNum()
{
	C64Set setUnGetKey;
	CGvGGiftMgr::Instance()->GetUnGetGiftKey(m_poOwner->GetID(), m_setGiftKey, setUnGetKey);
	return setUnGetKey.size();
}

UINT16 CPlayerGVG::GetSingleGift(UINT64 qwKey)
{
	UINT32 dwKey = qwKey >> 32;
	if ( m_poOwner->GetID() != dwKey)
	{
		return ERR_GET_GVG_GIFT::ID_NOT_EXIST;
	}
	if( m_setGiftKey.end() != m_setGiftKey.find(qwKey))
	{
		return ERR_GET_GVG_GIFT::ID_ALREAD_GET;
	}
	DT_GVG_GIFT_REWARD_DATA stData = {0};
	if( !CGvGGiftMgr::Instance()->GetGiftInfo(qwKey, stData))
	{
		return ERR_GET_GVG_GIFT::ID_NOT_EXIST;
	}

	EM_RESULT eRes = CGvGGiftMgr::Instance()->AddGiftGoods(m_poOwner, qwKey);
	if ( GIFT_BAG_FULL == eRes)
	{
		return ERR_GET_GVG_GIFT::ID_BAG_FULL;
	}
	else if ( GIFT_ID_NOT_EXIST == eRes)
	{
		return ERR_GET_GVG_GIFT::ID_NOT_EXIST;
	}
	//插入已领取的KEY
	m_setGiftKey.insert(qwKey);
	return ERR_GET_GVG_GIFT::ID_SUCCESS;
}

//领取全部礼包
UINT16 CPlayerGVG::GetAllGift()
{
	C64Set setUnGetKey;
	CGvGGiftMgr::Instance()->GetUnGetGiftKey(m_poOwner->GetID(), m_setGiftKey, setUnGetKey);
	for ( C64SetItr itr = setUnGetKey.begin(); itr != setUnGetKey.end(); itr++)
	{
		EM_RESULT eRes = CGvGGiftMgr::Instance()->AddGiftGoods(m_poOwner, *itr);
		if ( GIFT_BAG_FULL == eRes)
		{
			return ERR_GET_GVG_GIFT::ID_BAG_FULL;
		}
		else if ( GIFT_ID_NOT_EXIST == eRes)
		{
			return ERR_GET_GVG_GIFT::ID_NOT_EXIST;
		}
		m_setGiftKey.insert(*itr);
	}
	return ERR_GET_GVG_GIFT::ID_SUCCESS;
}

UINT16 CPlayerGVG::GetUnReadLogCnt()
{
    CFaction *poMyFaction = m_poOwner->GetFaction();
    if(NULL == poMyFaction)
    {
        return 0;
    }

    UINT16 wUnReadLogCnt = 0;
    DT_FACTION_GVG_LOG_LIST &stGvGLogList = poMyFaction->GetFactionGvGData().stGvGLogList;

    // 本派当前可读的战报条数
    UINT16 wReadableLogCnt = poMyFaction->GetReadbleGvGLogCnt();

    UINT16 wCurSessionID = CGvGBuild::Instance()->GetCurSession(); // 当前处于第几届
    if (m_stGVGData.wSessionID != wCurSessionID)
    {
        wUnReadLogCnt = wReadableLogCnt;
    }
    else if(m_stGVGData.wReadLogCnt < stGvGLogList.wLogCnt)
    {
        wUnReadLogCnt = (wReadableLogCnt > m_stGVGData.wReadLogCnt ? wReadableLogCnt - m_stGVGData.wReadLogCnt : 0);
    }

    return wUnReadLogCnt;
}

void CPlayerGVG::ClearUnReadLogCnt()
{
    CFaction *poMyFaction = m_poOwner->GetFaction();
    if(NULL == poMyFaction)
    {
        return;
    }

    m_stGVGData.wSessionID  = CGvGBuild::Instance()->GetCurSession();
    m_stGVGData.wReadLogCnt = poMyFaction->GetReadbleGvGLogCnt();
}

UINT32 CPlayerGVG::GetGambleFactionID(DT_GVG_ROUND &stRound)
{
    UINT8 byIdx = 0;
    BOOL bRet = GetIdxByGambleRound(stRound, byIdx);
    if ( !bRet)
    {
        return 0;
    }

    DT_GAMBLE_DATA &stGamble = m_stGVGData.astGambleLstInfo[byIdx];
    if(stGamble.qwGameID && stGamble.dwSupportFactionID)
    {
        return stGamble.dwSupportFactionID;
    }

    return 0;
}

BOOL CPlayerGVG::IsGamble(DT_GVG_ROUND &stRound)
{
    UINT32 dwGambleFactionID = GetGambleFactionID(stRound);
	return (dwGambleFactionID > 0);
}

BOOL CPlayerGVG::GetIdxByGambleRound(DT_GVG_ROUND& eRound, UINT8& byIdx )
{
	if ( GVG_16_TO_8 == eRound.byBigRound)
	{
		byIdx = 0;
		return TRUE;
	}
	else if ( GVG_8_TO_4 == eRound.byBigRound )
	{
		byIdx = 1;
		return TRUE;
	}
	else if ( GVG_FINAL_WAR == eRound.byBigRound)
	{
		if ( 0 < eRound.bySmallRound && eRound.bySmallRound < 8)
		{
			byIdx = eRound.bySmallRound + 1;
            return TRUE;
		}
	}
	return FALSE;
}