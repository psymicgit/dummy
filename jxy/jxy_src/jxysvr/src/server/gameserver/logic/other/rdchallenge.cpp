#include "rdchallenge.h"
#include "rdchallengepropmgr.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/other/phystrengthmgr.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <logic/vip/vippropmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/plunder/plunderbasepropmgr.h>
#include <logic/race/racepropmgr.h>
#include <logic/base/basedefine.h>
#include <logic/hero/hero.h>
#include <sdloggerimpl.h>
#include <logic/other/challengepromgr.h>
#include <logic/begbluegas/begbluepropmgr.h>
#include "logic/other/fewparam.h"
IMPLEMENT_CLASS_POOL_MGR(CRdChallenge)
CRdChallenge::CRdChallenge()
{
    memset(&m_stDT_RANDOM_CHALLENGE_DATA, 0 , sizeof(m_stDT_RANDOM_CHALLENGE_DATA));
    memset(&m_stRecommendPlayerRcd, 0, sizeof(m_stRecommendPlayerRcd));
    m_bFirst = TRUE;
    m_mapChallengePlayerData.clear();
}

CRdChallenge::~CRdChallenge()
{

}

BOOL CRdChallenge::Init(DT_RANDOM_CHALLENGE_DATA& stDT_RANDOM_CHALLENGE_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    memset(&m_stDT_RANDOM_CHALLENGE_DATA, 0x00, sizeof(m_stDT_RANDOM_CHALLENGE_DATA));
    memcpy(&m_stDT_RANDOM_CHALLENGE_DATA, &stDT_RANDOM_CHALLENGE_DATA, sizeof(m_stDT_RANDOM_CHALLENGE_DATA));
    m_poOwner = poOwner;

    for(UINT8 byIdx = 0 ; byIdx < m_stDT_RANDOM_CHALLENGE_DATA.byPlayerNum; byIdx++)
    {
        m_mapChallengePlayerData[m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[byIdx].dwPlayerID] = &(m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[byIdx]);
    }

    return TRUE;
}



VOID CRdChallenge::GetRdChallengePointInfo(DT_RDCHALLENGE_POINT_DATA_CLI& stRdChallengePointInfo)
{
    //先回复体力再取值
    RdChallengePoinRecover();

    UINT16 wRefreshCD = CRdChallengePropMgr::Instance()->GetRdChallengeRefreshCD();

    stRdChallengePointInfo.wCurChallengePoint = m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint;
    stRdChallengePointInfo.wRefreshCD = wRefreshCD;
    stRdChallengePointInfo.wMaxChallengePoint = CRdChallengePropMgr::Instance()->GetMaxChallengePoint();
    if(0 == m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime)
    {
        stRdChallengePointInfo.wCountDown = 0;
    }
    else
    {
        //判断时间
        INT32 nInterval = (INT32)(SDTimeSecs() - m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime);
        if(nInterval < 0) //异常
        {
            stRdChallengePointInfo.wCountDown = 0;
            SYS_CRITICAL(_SDT("[%s: %d]: oNow is oldder than qwLastRCPoinRefreshTime !"), MSG_MARK);
        }
        else if(nInterval > wRefreshCD)
        {
            stRdChallengePointInfo.wCountDown = 0;
        }
        else
        {
            stRdChallengePointInfo.wCountDown = wRefreshCD - (UINT16)nInterval;
        }
    }

}


//挑战点恢复，由相关协议请求触发，不做定时器
VOID CRdChallenge::RdChallengePoinRecover()
{
    UINT16 wMax = CRdChallengePropMgr::Instance()->GetMaxChallengePoint();

    UINT64 qwCurSec = SDTimeSecs();
    if(0 == m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime)
    {
        if(m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint < wMax)
        {
            m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint = wMax;//
            //AddPhyStrength( MAXPHYSTRENGTH-GetDT_PLAYER_BASE_DATA().wPhyStrength, CRecordMgr::EAPST_RECOVER );
            //SYS_CRITICAL(_SDT("[%s: %d]: player[%u]recover phystrength[%u]!"), MSG_MARK, m_dwPlayerID, m_stDT_PLAYER_BASE_DATA.wPhyStrength);
        }
        m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime = qwCurSec;
    }
    else
    {
        //体力可以超过上限，超过上限则不回复
        if(m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint >= wMax)
        {
            m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime = qwCurSec;
            return;
        }

        //判断时间
        UINT16 wRefreshCD = CRdChallengePropMgr::Instance()->GetRdChallengeRefreshCD();
        UINT32	dwInterval = (UINT32)(qwCurSec - m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime);
        if(dwInterval > wRefreshCD)
        {
            UINT16 wRecoverValue = dwInterval / wRefreshCD;
            //AddPhyStrength(wAddValue, CRecordMgr::EAPST_RECOVER);
            m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime = qwCurSec - (dwInterval % wRefreshCD);//更新刷新时间
            m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint += wRecoverValue;
            //m_oPhystrength.SetSaveState(ESS_WANT_SAVE);
        }
        if(m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint >= wMax)
        {
            m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint = wMax;
        }
    }

}

UINT16 CRdChallenge::OpenRdchallengeTab(UINT8 byRefreshFlag, DT_RDC_TAB_DATA& stTabInfo)
{
    if(NULL == m_poOwner)
    {
        return ERR_OPEN_RDCHALLENGE_TAB::ID_UNLOCK;
    }

    UINT32 dwRefreshCoin = CRdChallengePropMgr::Instance()->GetRefreshCoin();
    stTabInfo.dwRefreshCoin = dwRefreshCoin;
    stTabInfo.dwClrCDGold = CRdChallengePropMgr::Instance()->GetClrRdChallengeCDGold();
    if(byRefreshFlag)
    {
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < dwRefreshCoin)
        {
            return ERR_COMMON::ID_COIN_NOT_ENOUGH;
        }
        m_poOwner->DecCoin(dwRefreshCoin, CRecordMgr::EDCT_RDCREFRESH);
        GetRdPlayerInfo(stTabInfo.byPlayerNum, stTabInfo.astPlayerInfo);
    }
    else
    {
        //int total = 0;
        //if (int i = 0; i < MAX_GET_RDC_NUM; i++)
        //{
        //	if(1 == m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[i].byHaveChallengeFlag)
        //	{
        //		total++;
        //	}
        //}

        ////全部已挑战，重新获取
        //if(total == MAX_GET_RDC_NUM)
        //{
        //	GetRdPlayerInfo(stTabInfo.byPlayerNum, stTabInfo.astPlayerInfo);
        //}
        //else
        //{
        //个数为0，则为第一次获取
        if(0 == m_stDT_RANDOM_CHALLENGE_DATA.byPlayerNum)
        {
            GetRdPlayerInfo(stTabInfo.byPlayerNum, stTabInfo.astPlayerInfo);
        }
        else
        {
            stTabInfo.byPlayerNum = m_stDT_RANDOM_CHALLENGE_DATA.byPlayerNum;
            for(UINT8 byIdx = 0; byIdx < stTabInfo.byPlayerNum; byIdx++)
            {
                memcpy(&(stTabInfo.astPlayerInfo[byIdx].stPlayerInfo), &(m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[byIdx]), sizeof(DT_RDC_PLAYER_DATA));
                GetRdEncInfo(stTabInfo.astPlayerInfo[byIdx]);

				//战力同步
                const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stTabInfo.astPlayerInfo[byIdx].stPlayerInfo.dwPlayerID);
                if (NULL != poPlayerBaseData)
                {
                    stTabInfo.astPlayerInfo[byIdx].stPlayerInfo.dwPower = poPlayerBaseData->dwPower;
                }
            }
        }
        //}
    }

    GetRdChallengePointInfo(stTabInfo.stChallengePointInfo);

    stTabInfo.wChallengeCountDown = GetChallengeCountDown();

    m_bFirst = FALSE;

    return ERR_OPEN_RDCHALLENGE_TAB::ID_SUCCESS;
}


UINT16 CRdChallenge::GetChallengeCountDown()
{
    if((1 == m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag) || (0 == m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet))
    {
        return 0;
    }
    UINT64 qwPassTime = SDTimeSecs() - m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCTime;
    UINT16 wCD = CRdChallengePropMgr::Instance()->GetRdChallengeCD();
    if(qwPassTime >= wCD)
    {
        return 0;
    }

    return wCD - (UINT16)qwPassTime;
}



VOID CRdChallenge::GetRdPlayerInfo(UINT8& byPlayerNum, DT_RDC_PLAYER_DATA_CLI astPlayerInfo[MAX_GET_RDC_NUM])
{
    m_mapChallengePlayerData.clear();
    byPlayerNum = 0;
    CPlayerBaseDataVec vecPlayer;

    //获取比自己高1级的玩家1个
    C3232Map mapHave;
    UINT8 byNum = 1;
    UINT32 dwPlayerID = m_poOwner->GetID();
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    CPlayerBaseDataCacheMgr::Instance()->GetPlayerByLevel(dwPlayerID, wPlayerLevel + 1, byNum, vecPlayer, m_stRecommendPlayerRcd.dwPlayerH1);
    GetPlayerData(vecPlayer, byPlayerNum, astPlayerInfo, mapHave);
    UINT8 bySize = vecPlayer.size();
    if(bySize > 0)
    {
        m_stRecommendPlayerRcd.dwPlayerH1 = vecPlayer[bySize - 1]->dwID; //记录，用于下次刷新不同玩家
    }
    vecPlayer.clear();
    //获取等级一样的玩家3个
    byNum = 3 + (byNum - bySize);////获取等级一样的玩家3个,若高1级没有，则同等级的补上
    CPlayerBaseDataCacheMgr::Instance()->GetPlayerByLevel(dwPlayerID, wPlayerLevel, byNum, vecPlayer, m_stRecommendPlayerRcd.dwPlayerE);
    GetPlayerData(vecPlayer, byPlayerNum, astPlayerInfo, mapHave);
    bySize = vecPlayer.size();
    if(bySize > 0)
    {
        m_stRecommendPlayerRcd.dwPlayerE = vecPlayer[bySize - 1]->dwID; //记录，用于下次刷新不同玩家
    }
    vecPlayer.clear();
    //获取比自己低1级的玩家1个
    byNum = 1 + (byNum - bySize);////获取低1等级玩家1个,若同等级级没有，则低1级的补上
    CPlayerBaseDataCacheMgr::Instance()->GetPlayerByLevel(dwPlayerID, wPlayerLevel - 1, byNum, vecPlayer, m_stRecommendPlayerRcd.dwPlayerL1);
    GetPlayerData(vecPlayer, byPlayerNum, astPlayerInfo, mapHave);
    bySize = vecPlayer.size();
    if(bySize > 0)
    {
        m_stRecommendPlayerRcd.dwPlayerL1 = vecPlayer[bySize - 1]->dwID; //记录，用于下次刷新不同玩家
    }
    //人数不够，则取等级接近的玩家
    if(byPlayerNum < MAX_GET_RDC_NUM)
    {
        UINT8 byLoopNum = (UINT8)(CSinglePramMgr::Instance()->GetMaxPlayerLevel());
        UINT16 wNextLevel = wPlayerLevel - 2;
        UINT8 byGap = 2;
        for(; (((byPlayerNum < MAX_GET_RDC_NUM)) && (byLoopNum > 0) && (wNextLevel > 0)); byLoopNum--)
        {
            UINT8 byNum = MAX_GET_RDC_NUM - byPlayerNum;
            if(0 == byLoopNum % 2)
            {
                wNextLevel = wPlayerLevel - byGap;
            }
            else
            {
                wNextLevel = wPlayerLevel + byGap;
                byGap++;
            }
            CPlayerBaseDataCacheMgr::Instance()->GetPlayerByLevel(dwPlayerID, wNextLevel, byNum, vecPlayer, 0);
            GetPlayerData(vecPlayer, byPlayerNum, astPlayerInfo, mapHave);
        }
    }

    //人数不够，则取机器人
    if(byPlayerNum < MAX_GET_RDC_NUM)
    {
        UINT8 byNum = byPlayerNum;
        UINT32 dwPlunderRobotStartID = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwPlunderRobotStartID;
        for(UINT8 byIdx = 0; byIdx < MAX_ROBOT_NUM; byIdx++)
        {
            UINT32 dwID = dwPlunderRobotStartID + byIdx;
            const SPlayerBaseData* pstPlayerData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwID);
            if(pstPlayerData)
            {
                vecPlayer.push_back((SPlayerBaseData*)pstPlayerData);
                byNum++;
                if(byNum >= MAX_GET_RDC_NUM)
                {
                    break;
                }
            }
        }
        GetPlayerData(vecPlayer, byPlayerNum, astPlayerInfo, mapHave);
    }

    //引导，机器人
    if(m_poOwner->CKGuide(EGF_RDC))
    {
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(CRacePropMgr::Instance()->GetRaceProp().dwRobotID);
        if(pstPlayerBaseData)
        {
            DT_RDC_PLAYER_DATA& stPlayerData = astPlayerInfo[byPlayerNum / 2].stPlayerInfo;
            stPlayerData.dwPlayerID = pstPlayerBaseData->dwID;
            SDStrcpy(stPlayerData.aszDispName, pstPlayerBaseData->aszDispName);
            stPlayerData.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
            stPlayerData.byRobotFunc = pstPlayerBaseData->byRobotFunc;
            stPlayerData.wLevel = pstPlayerBaseData->wLevel;
            stPlayerData.dwPower = pstPlayerBaseData->dwPower;
            stPlayerData.byHaveChallengeFlag = 0;
            GetRdEncInfo(astPlayerInfo[byPlayerNum / 2]);
        }
    }

    m_stDT_RANDOM_CHALLENGE_DATA.byPlayerNum = byPlayerNum;
    for(UINT8 byIdx = 0 ; byIdx < byPlayerNum; byIdx++)
    {
        memcpy(&(m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[byIdx]), &(astPlayerInfo[byIdx].stPlayerInfo), sizeof(DT_RDC_PLAYER_DATA));
        m_mapChallengePlayerData[m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[byIdx].dwPlayerID] = &(m_stDT_RANDOM_CHALLENGE_DATA.astPlayerInfo[byIdx]);
    }

}


//获取玩家信息
VOID CRdChallenge::GetPlayerData(CPlayerBaseDataVec& vecPlayer, UINT8& byPlayerNum, DT_RDC_PLAYER_DATA_CLI astPlayerInfo[MAX_GET_RDC_NUM], C3232Map& mapHave)
{
    UINT8 bySize = vecPlayer.size();
    for(UINT8 byIdx = 0; ((byIdx < bySize) && (byPlayerNum < MAX_GET_RDC_NUM)); byIdx++)
    {
        SPlayerBaseData* pstPlayerBaseData = vecPlayer[byIdx];
        if(mapHave.end() != mapHave.find(pstPlayerBaseData->dwID))
        {
            continue;
        }
        DT_RDC_PLAYER_DATA& stPlayerData = astPlayerInfo[byPlayerNum].stPlayerInfo;
        stPlayerData.dwPlayerID = pstPlayerBaseData->dwID;
        SDStrcpy(stPlayerData.aszDispName, pstPlayerBaseData->aszDispName);
        stPlayerData.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
        stPlayerData.byRobotFunc = pstPlayerBaseData->byRobotFunc;
        stPlayerData.wLevel = pstPlayerBaseData->wLevel;
        stPlayerData.dwPower = pstPlayerBaseData->dwPower;
        stPlayerData.byHaveChallengeFlag = 0;
        GetRdEncInfo(astPlayerInfo[byPlayerNum]);

        mapHave[pstPlayerBaseData->dwID] = pstPlayerBaseData->dwID;
        byPlayerNum++;
    }
}


VOID CRdChallenge::GetRdEncInfo(DT_RDC_PLAYER_DATA_CLI& stPlayerData)
{
    DT_RDC_ENC_DATA& stSuccessEnc = stPlayerData.stSuccessEnc;
    DT_RDC_ENC_DATA& stFailEnc = stPlayerData.stFailEnc;
    UINT16 wMyLevel = m_poOwner->GetLevel();
    UINT16 wEnemyLevel = stPlayerData.stPlayerInfo.wLevel;
    stSuccessEnc.dwJingJie = CLuamgr::Instance()->GetRdchallengeEncJingJie(0, wMyLevel, wEnemyLevel);
    stSuccessEnc.dwPurpleGas = CLuamgr::Instance()->GetRdchallengeEncPurpleGas(0, wMyLevel, wEnemyLevel);
    stFailEnc.dwJingJie = CLuamgr::Instance()->GetRdchallengeEncJingJie(1, wMyLevel, wEnemyLevel);
    stFailEnc.dwPurpleGas = CLuamgr::Instance()->GetRdchallengeEncPurpleGas(1, wMyLevel, wEnemyLevel);
}


UINT16 CRdChallenge::Rdchallenge(CPlayer * poEnemyPlayer, DT_BATTLE_DATA5& stBattleInfo, DT_RDC_TAB_DATA& stTabInfo, DT_RDC_ENC_DATA& stEncInfo, UINT16 &wKindID)
{
    if((NULL == m_poOwner) || (NULL == poEnemyPlayer))
    {
        return ERR_RDCHALLENGE::ID_UNLOCK;
    }
	if( 0 == m_poOwner->GetBag().GetIdleSize())
	{
		return ERR_RDCHALLENGE::ID_NOT_BAG;
	}
    RdChallengePoinRecover();
    if(0 == m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint)
    {
        return ERR_RDCHALLENGE::ID_NUM_OVER;
    }
    CRDCPlayerMapItr itr = m_mapChallengePlayerData.find(poEnemyPlayer->GetID());
    if(itr == m_mapChallengePlayerData.end())
    {
        return ERR_RDCHALLENGE::ID_NOT_REFRESH_PLAYER;
    }
    DT_RDC_PLAYER_DATA* pstDT_RDC_PLAYER_DATA = itr->second;
    if(1 == pstDT_RDC_PLAYER_DATA->byHaveChallengeFlag)
    {
        return ERR_RDCHALLENGE::ID_HAVE_CHALLENGE;
    }

    if((0 != m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet) && (0 != GetChallengeCountDown()) && (1 != m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag))
    {
        OpenRdchallengeTab(0, stTabInfo);
        return ERR_RDCHALLENGE::ID_IN_CD;
    }

    BOOL bRobot = poEnemyPlayer->GetID() == CRacePropMgr::Instance()->GetRaceProp().dwRobotID ? TRUE : FALSE;
    if(bRobot)
    {
        DT_BATTLE_BASE_ATTRIBUTE& stMyselfAttr = poEnemyPlayer->GetCoachHero()->GetDT_BATTLE_ATTRIBUTE();
        DT_BATTLE_BASE_ATTRIBUTE& stEnemyAttr = poEnemyPlayer->GetCoachHero()->GetDT_BATTLE_ATTRIBUTE();
        stEnemyAttr.qwHP = stMyselfAttr.qwHP;
        stEnemyAttr.dwAttack = stMyselfAttr.dwAttack >> 2;
    }
    if(!CBattleMgr::Instance()->Battle4(m_poOwner, poEnemyPlayer, stBattleInfo, TRUE))
    {
        RETURN_OTHER_ERR;
    }
    if(bRobot)
    {
        stBattleInfo.byBattleResult = EBT_SUCCESS;
    }

    UINT16 wMyLevel = m_poOwner->GetLevel();
    UINT16 wEnemyLevel = poEnemyPlayer->GetLevel();


    if (m_poOwner->GetCliVer() > CLIENT_VER)
    {
        //随机获取一个物品ID 暂时不用根据等级来 直接写死
        wKindID = CChallengePropMgr::Instance()->RateItemID(100, 100 , m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet);
        if (wKindID != 0)
        {
            ECreateItemRet	emCreateItemRet  = ECIR_SUCCESS;
            CItem*	poItem = NULL;
            poItem =  CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, wKindID, 1, emCreateItemRet, 0, CRecordMgr::EAIT_RDCHALLENGE);
            //背包满
            if(NULL == poItem && ECIR_FAILED_BAG_FULL == emCreateItemRet)
            {
                return ERR_RDCHALLENGE::ID_NOT_BAG;
            }
        }
    }

    m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint--;
    m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCTime = SDTimeSecs();
    m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet = (0 == stBattleInfo.byBattleResult) ? 0 : 1;
	BOOL bSuccess = stBattleInfo.byBattleResult == EBT_SUCCESS ? TRUE : FALSE;
	CFewParam& oFewParam = m_poOwner->GetFewParam();
	if ( bSuccess)
	{
		oFewParam.AddTrialSuccess();
	}
	else
	{
		oFewParam.AddTrialFailed();
	}
    m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag = 0;
    if (0 == m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet)
    {
        pstDT_RDC_PLAYER_DATA->byHaveChallengeFlag = 1;
    }
	stEncInfo.dwJingJie = CLuamgr::Instance()->GetRdchallengeEncJingJie(m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet, wMyLevel, wEnemyLevel);
	stEncInfo.dwPurpleGas = CLuamgr::Instance()->GetRdchallengeEncPurpleGas(m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet, wMyLevel, wEnemyLevel);
    m_poOwner->AddJingJie(stEncInfo.dwJingJie, CRecordMgr::EAJT_RDCHALLENGE);
    m_poOwner->AddPurpleGas(stEncInfo.dwPurpleGas, CRecordMgr::EAPGT_RDCHALLENGE);
    m_poOwner->AddGuideRecord(EGF_RDC);

    OpenRdchallengeTab(0, stTabInfo);


    return ERR_RDCHALLENGE::ID_SUCCESS;
}

UINT16 CRdChallenge::Rdchallenge2(CPlayer * poEnemyPlayer, DT_BATTLE_DATA3& stBattleInfo, DT_RDC_TAB_DATA& stTabInfo, DT_RDC_ENC_DATA& stEncInfo, UINT16 &wKindID)
{
    if((NULL == m_poOwner) || (NULL == poEnemyPlayer))
    {
        return ERR_RDCHALLENGE::ID_UNLOCK;
    }

    RdChallengePoinRecover();
    if(0 == m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint)
    {
        return ERR_RDCHALLENGE::ID_NUM_OVER;
    }
    CRDCPlayerMapItr itr = m_mapChallengePlayerData.find(poEnemyPlayer->GetID());
    if(itr == m_mapChallengePlayerData.end())
    {
        return ERR_RDCHALLENGE::ID_NOT_REFRESH_PLAYER;
    }
    DT_RDC_PLAYER_DATA* pstDT_RDC_PLAYER_DATA = itr->second;
    if(1 == pstDT_RDC_PLAYER_DATA->byHaveChallengeFlag)
    {
        return ERR_RDCHALLENGE::ID_HAVE_CHALLENGE;
    }

    if((0 != m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet) && (0 != GetChallengeCountDown()) && (1 != m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag))
    {
        OpenRdchallengeTab(0, stTabInfo);
        return ERR_RDCHALLENGE::ID_IN_CD;
    }

    BOOL bRobot = poEnemyPlayer->GetID() == CRacePropMgr::Instance()->GetRaceProp().dwRobotID ? TRUE : FALSE;
    if(bRobot)
    {
        DT_BATTLE_BASE_ATTRIBUTE& stMyselfAttr = poEnemyPlayer->GetCoachHero()->GetDT_BATTLE_ATTRIBUTE();
        DT_BATTLE_BASE_ATTRIBUTE& stEnemyAttr = poEnemyPlayer->GetCoachHero()->GetDT_BATTLE_ATTRIBUTE();
        stEnemyAttr.qwHP = stMyselfAttr.qwHP;
        stEnemyAttr.dwAttack = stMyselfAttr.dwAttack >> 2;
    }
    if(!CBattleMgr::Instance()->Battle2(m_poOwner, poEnemyPlayer, stBattleInfo, TRUE))
    {
        RETURN_OTHER_ERR;
    }
    if(bRobot)
    {
        stBattleInfo.byBattleResult = EBT_SUCCESS;
    }

    UINT16 wMyLevel = m_poOwner->GetLevel();
    UINT16 wEnemyLevel = poEnemyPlayer->GetLevel();

    m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet = (0 == stBattleInfo.byBattleResult) ? 0 : 1;
	BOOL bSuccess = stBattleInfo.byBattleResult == EBT_SUCCESS ? TRUE : FALSE;
	CFewParam& oFewParam = m_poOwner->GetFewParam();
	if ( bSuccess)
	{
		oFewParam.AddTrialSuccess();
	}
	else
	{
		oFewParam.AddTrialFailed();
	}
    if (m_poOwner->GetCliVer() > CLIENT_VER)
    {
        //随机获取一个物品ID 暂时不用根据等级来 直接写死
        wKindID = CChallengePropMgr::Instance()->RateItemID(100, 100 , m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet);
        if (wKindID != 0)
        {
            ECreateItemRet	emCreateItemRet  = ECIR_SUCCESS;
            CItem*	poItem = NULL;
            poItem =  CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, wKindID, 1, emCreateItemRet, 0, CRecordMgr::EAIT_RDCHALLENGE);
            //背包满
            if(NULL == poItem && ECIR_FAILED_BAG_FULL == emCreateItemRet)
            {
                return ERR_RDCHALLENGE::ID_NOT_BAG;
            }
        }
    }

    m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint--;
    m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCTime = SDTimeSecs();
    m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag = 0;
    if (0 == m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet)
    {
        pstDT_RDC_PLAYER_DATA->byHaveChallengeFlag = 1;
    }
    stEncInfo.dwJingJie = CLuamgr::Instance()->GetRdchallengeEncJingJie(m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet, wMyLevel, wEnemyLevel);
    stEncInfo.dwPurpleGas = CLuamgr::Instance()->GetRdchallengeEncPurpleGas(m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet, wMyLevel, wEnemyLevel);
    m_poOwner->AddJingJie(stEncInfo.dwJingJie, CRecordMgr::EAJT_RDCHALLENGE);
    m_poOwner->AddPurpleGas(stEncInfo.dwPurpleGas, CRecordMgr::EAPGT_RDCHALLENGE);
    m_poOwner->AddGuideRecord(EGF_RDC);

    OpenRdchallengeTab(0, stTabInfo);


    return ERR_RDCHALLENGE::ID_SUCCESS;
}

UINT16 CRdChallenge::Rdchallenge3(CPlayer * poEnemyPlayer, DT_BATTLE_DATA4& stBattleInfo, DT_RDC_TAB_DATA& stTabInfo, DT_RDC_ENC_DATA& stEncInfo ,UINT16 &wKindID)
{
	if((NULL == m_poOwner) || (NULL == poEnemyPlayer))
	{
		return ERR_RDCHALLENGE::ID_UNLOCK;
	}

	RdChallengePoinRecover();
	if(0 == m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint)
	{
		return ERR_RDCHALLENGE::ID_NUM_OVER;
	}
	CRDCPlayerMapItr itr = m_mapChallengePlayerData.find(poEnemyPlayer->GetID());
	if(itr == m_mapChallengePlayerData.end())
	{
		return ERR_RDCHALLENGE::ID_NOT_REFRESH_PLAYER;
	}
	DT_RDC_PLAYER_DATA* pstDT_RDC_PLAYER_DATA = itr->second;
	if(1 == pstDT_RDC_PLAYER_DATA->byHaveChallengeFlag)
	{
		return ERR_RDCHALLENGE::ID_HAVE_CHALLENGE;
	}

	if((0 != m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet) && (0 != GetChallengeCountDown()) && (1 != m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag))
	{
		OpenRdchallengeTab(0, stTabInfo);
		return ERR_RDCHALLENGE::ID_IN_CD;
	}

	BOOL bRobot = poEnemyPlayer->GetID() == CRacePropMgr::Instance()->GetRaceProp().dwRobotID ? TRUE : FALSE;
	if(bRobot)
	{
		DT_BATTLE_BASE_ATTRIBUTE& stMyselfAttr = poEnemyPlayer->GetCoachHero()->GetDT_BATTLE_ATTRIBUTE();
		DT_BATTLE_BASE_ATTRIBUTE& stEnemyAttr = poEnemyPlayer->GetCoachHero()->GetDT_BATTLE_ATTRIBUTE();
		stEnemyAttr.qwHP = stMyselfAttr.qwHP;
		stEnemyAttr.dwAttack = stMyselfAttr.dwAttack >> 2;
	}
	if(!CBattleMgr::Instance()->Battle3(m_poOwner, poEnemyPlayer, stBattleInfo, TRUE))
	{
		RETURN_OTHER_ERR;
	}
	if(bRobot)
	{
		stBattleInfo.byBattleResult = EBT_SUCCESS;
	}

	UINT16 wMyLevel = m_poOwner->GetLevel();
	UINT16 wEnemyLevel = poEnemyPlayer->GetLevel();

	m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet = (0 == stBattleInfo.byBattleResult) ? 0 : 1;
	BOOL bSuccess = stBattleInfo.byBattleResult == EBT_SUCCESS ? TRUE : FALSE;
	CFewParam& oFewParam = m_poOwner->GetFewParam();
	if ( bSuccess)
	{
		oFewParam.AddTrialSuccess();
	}
	else
	{
		oFewParam.AddTrialFailed();
	}
	if (m_poOwner->GetCliVer() > CLIENT_VER)
	{
		//随机获取一个物品ID 暂时不用根据等级来 直接写死
		wKindID = CChallengePropMgr::Instance()->RateItemID(100, 100 , m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet);
		if (wKindID != 0)
		{
			ECreateItemRet	emCreateItemRet  = ECIR_SUCCESS;
			CItem*	poItem = NULL;
			poItem =  CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, wKindID, 1, emCreateItemRet, 0, CRecordMgr::EAIT_RDCHALLENGE);
			//背包满
			if(NULL == poItem && ECIR_FAILED_BAG_FULL == emCreateItemRet)
			{
				return ERR_RDCHALLENGE::ID_NOT_BAG;
			}
		}
	}

	m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint--;
	m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCTime = SDTimeSecs();
	m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag = 0;
	if (0 == m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet)
	{
		pstDT_RDC_PLAYER_DATA->byHaveChallengeFlag = 1;
	}
	stEncInfo.dwJingJie = CLuamgr::Instance()->GetRdchallengeEncJingJie(m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet, wMyLevel, wEnemyLevel);
	stEncInfo.dwPurpleGas = CLuamgr::Instance()->GetRdchallengeEncPurpleGas(m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet, wMyLevel, wEnemyLevel);
	m_poOwner->AddJingJie(stEncInfo.dwJingJie, CRecordMgr::EAJT_RDCHALLENGE);
	m_poOwner->AddPurpleGas(stEncInfo.dwPurpleGas, CRecordMgr::EAPGT_RDCHALLENGE);
	m_poOwner->AddGuideRecord(EGF_RDC);

	OpenRdchallengeTab(0, stTabInfo);


	return ERR_RDCHALLENGE::ID_SUCCESS;
}
UINT16 CRdChallenge::ClrRDChallengeCD()
{
    //输且CD中，则清除CD
    UINT16 wCountDown = GetChallengeCountDown();
    if((0 != m_stDT_RANDOM_CHALLENGE_DATA.byLastRCRet) && (0 != wCountDown) && (1 != m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag))
    {
        UINT64 qwMin = DivCeil(wCountDown, 60);
        UINT16 wCostGold = (UINT16)(CRdChallengePropMgr::Instance()->GetClrRdChallengeCDGold() * qwMin);
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < wCostGold)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
        m_poOwner->DecGold(wCostGold, CRecordMgr::EDGT_CLRRDCCD, qwMin);

        m_stDT_RANDOM_CHALLENGE_DATA.byClrCDFlag = 1;
    }

    return ERR_CLR_RDCHALLENGE_CD::ID_SUCCESS;
}

UINT16	CRdChallenge::GetChallengeTimes(UINT32 &dwLeftTimes, UINT32 &dwTimes)
{
    RdChallengePoinRecover();

    dwLeftTimes = m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint;
    dwTimes = CRdChallengePropMgr::Instance()->GetMaxChallengePoint();
    return 0;
}




BOOL CRdChallenge::GetPrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt)
{
    memset(&stPvpPrompt, 0, sizeof(&stPvpPrompt));
    stPvpPrompt.byBuildKindID = EBK_PVP;

    RdChallengePoinRecover();
    if(0 == m_stDT_RANDOM_CHALLENGE_DATA.wCurChallengePoint)
    {
        //UINT16 wRefreshCD = CRdChallengePropMgr::Instance()->GetRdChallengeRefreshCD();
        //UINT32	dwInterval = (UINT32)(SDTimeSecs() - m_stDT_RANDOM_CHALLENGE_DATA.qwLastRCPoinRefreshTime);
        //stPvpPrompt.dwFuncCountDown = wRefreshCD - dwInterval;
        return FALSE;
    }

    stPvpPrompt.byFuncFlag = EPP_RDC;//可挑战

    return TRUE;
}
