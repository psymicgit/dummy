#include "bossbattlemgr.h"
#include "bossbattle.h"
#include "bossbbattlelog.h"
#include <logic/player/playermgr.h>
#include <logic/monster/monster.h>
#include <logic/battle/battlemgr.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <logic/event/timereventmgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/lua/luamgr.h>
#include <net/db/dbpktbuilder.h>
#include <logic/record/recordmgr.h>
#include <algorithm>
#include <functional>
#include <net/cli/usermgr.h>
#include <logic/base/basedefine.h>
#include <logic/other/singleprammgr.h>
#include <logic/other/locale.h>
#include <logic/vip/vippropmgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/build/build.h>
#include <framework/gsconfig.h>
#include <logic/player/player.h>
#include <logic/other/errmsgmgr.h>
#include <logic/chat/chatmgr.h>

#include <errdef.h>

using namespace std;

IMPLEMENT_SINGLETON(CBossBattleMgr)

CBossBattleMgr::CBossBattleMgr()
{
    m_poBoss = NULL;
    m_pstCurBossBProp = NULL;
    m_bActivityOver = TRUE;
    m_wIncRateByStory = 0;
    m_wIncRateByGold = 0;
    m_qwStartTime = 0;
    m_byLastActivityID = 0;
    m_qwBossTotalHP = 0;
    m_bySyncActivityID = 0;
    m_qwSyncCurBossHP = 0;
    m_qwSyncStartTime = 0;
    m_bSyncFlag = FALSE;

    m_vecAllEnterPlayer.clear();
    m_mapAllEnterPlayer.clear();
    m_mapAllPlayer.clear();
    m_itrCur = m_mapAllEnterPlayer.end();
}

CBossBattleMgr::~CBossBattleMgr()
{

}

CBossBBattleLog* CBossBattleMgr::CreateBattleLog()
{
    return m_oBossBBLShmemObjMgr.Alloc();
}

VOID CBossBattleMgr::RemoveBattleLog(CBossBBattleLog* poBossBBattleLog)
{
    if(poBossBBattleLog)
    {
        m_oBossBBLShmemObjMgr.Free(poBossBBattleLog);
    }
}


BOOL CBossBattleMgr::Init()
{
    // 初始化CBossBattlePropMgr管理器
    if(FALSE == CBossBattlePropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CBossBattlePropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CBossBattlePropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CBossBattlePropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitModuleDataFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitDataFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitModuleLogDataFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitModuleLogDataFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitEvent failed!"), MSG_MARK);
        return FALSE;
    }

	if (! LoadCloseArea())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadCloseArea failed!"), MSG_MARK);
		return FALSE;
	}


    return TRUE;
}

BOOL CBossBattleMgr::LoadCloseArea()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CHAR szSql[1024] = {0};
	sprintf(szSql, "select * from closedarea ;");

	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
	if(SDDB_HAS_RECORDSET == nRet)
	{
		while(pRecordSet->GetRecord())
		{
			UINT16 wZoneID = SGDP::SDAtou( pRecordSet->GetFieldValueByName("ZoneID"));
			m_setCloseArea.insert(wZoneID);
		}
	}
	else if(SDDB_NO_RECORDSET == nRet)
	{
		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}

	return TRUE;
}



BOOL CBossBattleMgr::InitModuleDataFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CHAR szSql[1024] = {0};
    sprintf(szSql, "select * from module where KeyID = '%d';", EMDT_BossBattle);

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            UINT32 dwLen = pRecordSet->GetFieldLengthByName( "Value" );
            if (0 != dwLen)
            {
                DT_BOSS_BATTLE_DATA stDT_BOSS_BATTLE_DATA = {0};
                if( -1 == stDT_BOSS_BATTLE_DATA.DecodeSaveData( pRecordSet->GetFieldValueByName("Value"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] DT_BOSS_BATTLE_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    return FALSE;
                }
                m_mapBassBattleModuleData[stDT_BOSS_BATTLE_DATA.byActivityID] = stDT_BOSS_BATTLE_DATA;
            }
        }
    }
    else if(SDDB_NO_RECORDSET == nRet)
    {
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CBossBattleMgr::InitModuleLogDataFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CHAR szSql[1024] = {0};
    sprintf(szSql, "select * from modulelog where KeyID = '%d' order by OverTime desc limit %u;", EMDT_BossBattle, MAX_BOSSB_BATTLE_LOG_NUM * 2);

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            string strOverTime = pRecordSet->GetFieldValueByName( "OverTime" );
            UINT32 dwLen = pRecordSet->GetFieldLengthByName( "Value" );
            if (0 != dwLen)
            {
                DT_BOSS_BATTLE_DATA stDT_BOSS_BATTLE_DATA = {0};
                if( -1 == stDT_BOSS_BATTLE_DATA.DecodeSaveData( pRecordSet->GetFieldValueByName("Value"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] DT_BOSS_BATTLE_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    continue;
                }
                m_mapBossBBLData[stDT_BOSS_BATTLE_DATA.qwOverTime] = stDT_BOSS_BATTLE_DATA;
            }
        }

    }
    else if(SDDB_NO_RECORDSET == nRet)
    {
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        return TRUE;
    }
    else
    {
        SYS_CRITICAL( _SDT("[%s: %d] InitModuleLogDataFromDB exec sql failed[%s]!"), MSG_MARK, strError.c_str());
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CBossBattleMgr::InitEvent()
{
    CBossBPropMap& mapBossBProp = CBossBattlePropMgr::Instance()->GetBossBPropMap();
    for(CBossBPropMapItr itr = mapBossBProp.begin(); itr != mapBossBProp.end(); itr++)
    {
        SBossBProp& stProp = itr->second;
        CTimerEventMgr::Instance()->AddBossBattleEvent(stProp.byActivityId, TRUE);
        CTimerEventMgr::Instance()->AddBossBattleEvent(stProp.byActivityId, FALSE);

        //BOSS战斗推送
        //CTimerEventMgr::Instance()->AddNoticeEventEvent(ENT_BOSSBATTLE, stProp.dwStartTime, stProp.dwEndTime, stProp.dwStartEnterTime);
    }

    return TRUE;
}


VOID CBossBattleMgr::UnInit()
{
    CBossBattlePropMgr::Instance()->UnInit();
    CBossBattlePropMgr::DestroyInstance();
}


BOOL CBossBattleMgr::OnStartEvent(UINT8 byActivityID)
{
    SBossBProp* pstBossBProp = CBossBattlePropMgr::Instance()->GetBossBPropByID(byActivityID);
    if(NULL == pstBossBProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstBossBProp[%d] is NULL!"), MSG_MARK, byActivityID);
        return FALSE;
    }

    if(SDNow().DiffDateDay(CGSConfig::Instance()->GetServerOpentTime()) < pstBossBProp->wOpenActivityPassDay)
    {
        //开服前2天不开启
        return TRUE;
    }

    CMonster* m_poMonster = CMonsterMgr::Instance()->CreateMonster();
    if(NULL == m_poMonster)
    {
        return FALSE;
    }

	//赋值等级
	m_wBossLevel = pstBossBProp->wBossStartLevel;

    map<UINT8, DT_BOSS_BATTLE_DATA>::iterator itr = m_mapBassBattleModuleData.find(byActivityID);
    if(itr != m_mapBassBattleModuleData.end())
    {
        m_wBossLevel = 0 == itr->second.wLastLevel ? m_wBossLevel : itr->second.wLastLevel;
        if(itr->second.byNeedUpgrade)
        {
            m_wBossLevel++;
        }
    }
    m_wBossLevel = m_wBossLevel > pstBossBProp->wBossMaxLevel ? pstBossBProp->wBossMaxLevel : m_wBossLevel;
    if(!m_poMonster->Init(pstBossBProp->wBossMonsterID, 1, TRUE, m_wBossLevel))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: boss[%d] init failed!"), MSG_MARK, pstBossBProp->wBossMonsterID);
        return FALSE;
    }

    //先清除之前的boss
    if(m_poBoss)
    {
        CMonsterMgr::Instance()->RemoveMonster(m_poBoss);
        m_poBoss = NULL;
    }

    UINT64 qwNow = SDTimeSecs();

    m_poBoss = m_poMonster;
    m_pstCurBossBProp = pstBossBProp;
    m_bActivityOver = FALSE;
    m_qwStartTime = SDTimeSecs();
    m_byLastActivityID = m_pstCurBossBProp->byActivityId;

    m_wIncRateByStory = CLuamgr::Instance()->GetIncRateByStory();
    m_wIncRateByGold = CLuamgr::Instance()->GetIncRateByGold();

    m_qwBossTotalHP = m_poBoss->GetDT_BATTLE_ATTRIBUTE().qwHP;

    memset(m_astTopHurtInfo, 0, sizeof(m_astTopHurtInfo));

    // 在聊天频道显示活动开始通知
    AddChatOnActivityStart(qwNow - GetDayPassSecond(qwNow) + m_pstCurBossBProp->dwStartTime/*活动的预期开始时间*/, qwNow);

    //同步HP
    if((m_pstCurBossBProp->byActivityId == m_bySyncActivityID) && (m_qwSyncCurBossHP < m_qwBossTotalHP) && (0 != m_qwSyncCurBossHP) && (0 != m_bySyncActivityID)) //血量为0说明已经结束，不需同步
    {
        USR_INFO(_SDT("[%s: %d]: sync bossb data, sync activityid[%u], sync cur hp[%llu],  "), MSG_MARK, m_bySyncActivityID, m_qwSyncCurBossHP);
        m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP = m_qwSyncCurBossHP;
        m_bySyncActivityID = 0;
        m_qwSyncCurBossHP = 0;
        m_qwStartTime = m_qwSyncStartTime;
        m_bSyncFlag = TRUE;
    }
    else
    {
        //重启过，使用之前的血量
        map<UINT8, DT_BOSS_BATTLE_DATA>::iterator itr = m_mapBassBattleModuleData.find(m_pstCurBossBProp->byActivityId);
        if(itr != m_mapBassBattleModuleData.end())
        {
            DT_BOSS_BATTLE_DATA& stDT_BOSS_BATTLE_DATA = itr->second;
            if(IsToday(stDT_BOSS_BATTLE_DATA.qwSaveTime))
            {
                USR_INFO(_SDT("[%s: %d]: reset bossb data, activityid[%u], sync cur hp[%llu],  "), MSG_MARK, m_pstCurBossBProp->byActivityId, stDT_BOSS_BATTLE_DATA.qwCurHP);
                if(0 != stDT_BOSS_BATTLE_DATA.qwCurHP) //已结束，不需重新赋值
                {
                    m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP = stDT_BOSS_BATTLE_DATA.qwCurHP;
                    //m_qwStartTime = stDT_BOSS_BATTLE_DATA.qwStartTime;
                    memcpy(m_astTopHurtInfo, stDT_BOSS_BATTLE_DATA.astTopHurtInfo, sizeof(m_astTopHurtInfo));
                }
            }
        }
    }

    //赋值其他当前记录
    DT_BOSS_BATTLE_DATA& stDT_BOSS_BATTLE_DATA = m_mapBassBattleModuleData[m_pstCurBossBProp->byActivityId];
    memset(&stDT_BOSS_BATTLE_DATA.astBossBTopEncInfo, 0, sizeof(stDT_BOSS_BATTLE_DATA.astBossBTopEncInfo));
    memset(&stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo, 0, sizeof(stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo));
    stDT_BOSS_BATTLE_DATA.qwOverTime = 0;
    stDT_BOSS_BATTLE_DATA.qwStartTime = m_qwStartTime;
    stDT_BOSS_BATTLE_DATA.byActivityID = m_pstCurBossBProp->byActivityId;
    stDT_BOSS_BATTLE_DATA.wLastLevel = m_wBossLevel;
    stDT_BOSS_BATTLE_DATA.qwSaveTime = SDTimeSecs();

    return TRUE;
}



BOOL CBossBattleMgr::OnEndEvent(UINT8 byActivityID)
{
    //AUTOCYCLE;

    //活动时间结束，未击杀，则结束活动
    if(!m_bActivityOver)
    {
        ActivityOver();
    }

    m_vecAllEnterPlayer.clear();
    m_mapAllEnterPlayer.clear();
    m_mapAllPlayer.clear();
    m_pstCurBossBProp = NULL;
    m_bActivityOver = TRUE;
    m_bSyncFlag = FALSE;

    return TRUE;
}


//玩家进入
UINT16 CBossBattleMgr::OnEnter(CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY, DT_BOSSB_MY_DATA_CLI& stMyBossBInfo, DT_BOSSB_PLAYER_DATA_CLI_LST& stPlayerInfo,
                               DT_BOSSB_DATA_CLI2& stBossBInfo, DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM], DT_HURT_ENC_DATA& stHurtEncInfo)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    CBuild* poBuild = poPlayer->GetBuild(EBK_BOSSBATTLE);
    if(NULL == poBuild)
    {
        return ERR_ENTER_BOSSB::ID_BOSSB_UNLOCK;
    }
    poBuild->OnOpen();

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return ERR_ENTER_BOSSB::ID_BOSSB_UNSTART;
    }
    UINT8 byEnterType = 0; //0 第一次进入, 1 进入又离开, 2 进入过当前只是重新同步数据
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        RETURN_OTHER_ERR;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
    UINT32 dwPassSec = GetDayPassSecond(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime);
    if(!IsToday(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime) || (dwPassSec < m_pstCurBossBProp->dwStartEnterTime) || (dwPassSec > m_pstCurBossBProp->dwEndTime)) //上次进入不是今天 || 上次进入不是当前活动，重新进入
    {
        byEnterType = 0;
    }
    else
    {
        if(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastLeaveActivityTime >= stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime)
        {
            byEnterType = 1;
        }
        else
        {
            byEnterType = 2;
        }
    }

    //添加管理
    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
        m_vecAllEnterPlayer.push_back(dwPlayerID);
        m_mapAllPlayer[dwPlayerID] = dwPlayerID;
    }

    //活动刚开启（可能重启了），则所有之前进入的都得重新加入管理
    if(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime < m_qwStartTime)
    {
        byEnterType = 0;
    }

    switch(byEnterType)
    {
        //第一次进入
    case 0:
    {
        memset(&stDT_BOSS_BATTLE_PLAYER_DATA, 0, sizeof(stDT_BOSS_BATTLE_PLAYER_DATA));
        poBossBattle->InitEnterData();

        ////位置赋值出生点
        //stDT_BOSS_BATTLE_PLAYER_DATA.wPosX = m_pstCurBossBProp->wBirthPosX;
        //stDT_BOSS_BATTLE_PLAYER_DATA.wPosY = m_pstCurBossBProp->wBirthPosY;
        //使用客户端的坐标
        stDT_BOSS_BATTLE_PLAYER_DATA.wPosX = wPosX;
        stDT_BOSS_BATTLE_PLAYER_DATA.wPosY = wPosY;

        //赋值时间
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime = SDTimeSecs();
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastLeaveActivityTime = 0;

        //赋值起始关注Idx
        stDT_BOSS_BATTLE_PLAYER_DATA.dwSyncStartIdx = RandomReseed(m_vecAllEnterPlayer.size());

        //添加关注
        Addattention(poPlayer);
    }
    break;
    //离开又进入
    case 1:
    {
        poBossBattle->InitEnterData();

        //使用客户端的坐标
        stDT_BOSS_BATTLE_PLAYER_DATA.wPosX = wPosX;
        stDT_BOSS_BATTLE_PLAYER_DATA.wPosY = wPosY;

        //赋值时间
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime = SDTimeSecs();
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastLeaveActivityTime = 0;

        //赋值起始关注Idx
        stDT_BOSS_BATTLE_PLAYER_DATA.dwSyncStartIdx = RandomReseed(m_vecAllEnterPlayer.size());

        //添加关注
        Addattention(poPlayer);
    }
    break;
    //进入过当前只是重新同步数据
    case 2:
        if(m_bSyncFlag)//gs同步的玩家，重新添加关注
        {
            poBossBattle->InitEnterData();

            //添加关注
            Addattention(poPlayer);
        }
        break;
    default:
        memset(&stDT_BOSS_BATTLE_PLAYER_DATA, 0, sizeof(stDT_BOSS_BATTLE_PLAYER_DATA));
        poBossBattle->InitEnterData();

        ////位置赋值出生点
        //stDT_BOSS_BATTLE_PLAYER_DATA.wPosX = m_pstCurBossBProp->wBirthPosX;
        //stDT_BOSS_BATTLE_PLAYER_DATA.wPosY = m_pstCurBossBProp->wBirthPosY;
        //使用客户端的坐标
        stDT_BOSS_BATTLE_PLAYER_DATA.wPosX = wPosX;
        stDT_BOSS_BATTLE_PLAYER_DATA.wPosY = wPosY;

        //赋值时间
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime = SDTimeSecs();
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastLeaveActivityTime = 0;

        //赋值起始关注Idx
        stDT_BOSS_BATTLE_PLAYER_DATA.dwSyncStartIdx = RandomReseed(m_vecAllEnterPlayer.size());

        //添加关注
        Addattention(poPlayer);

        break;
    }

    GetDT_BOSSB_MY_DATA_CLI(poPlayer, stDT_BOSS_BATTLE_PLAYER_DATA, stMyBossBInfo);

    GetDT_BOSSB_DATA_CLI(stBossBInfo);

    //发送显示玩家信息
    GetShowPlayerInfo(poPlayer, stDT_BOSS_BATTLE_PLAYER_DATA.dwSyncStartIdx, stPlayerInfo);

    GetTopHurtRate(astTopHurtInfo);

    GetHurtEncInfo(poPlayer->GetLevel(), stDT_BOSS_BATTLE_PLAYER_DATA, stHurtEncInfo);

    poPlayer->GetLocale().OnEnterBossB();

    return ERR_ENTER_BOSSB::ID_SUCCESS;
}

VOID CBossBattleMgr::GetTopHurtRate(DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM])
{
    //AUTOCYCLE;

    for(UINT8 byIdx = 0; byIdx < MAX_BOSS_ENC_TOP_NUM; byIdx++)
    {
        DT_BOSSB_HURT_RATE& stTopEnc1 = astTopHurtInfo[byIdx];
        stTopEnc1.dwPlayerID = m_astTopHurtInfo[byIdx].dwPlayerID;
        SDStrcpy(stTopEnc1.aszDispName, m_astTopHurtInfo[byIdx].aszDispName);
        stTopEnc1.dwHurtValue = m_astTopHurtInfo[byIdx].dwHurtValue;
        stTopEnc1.dwHurtRate = m_astTopHurtInfo[byIdx].dwHurtRate;
        stTopEnc1.dwHurtRank = byIdx + 1;
    }
}

VOID CBossBattleMgr::GetHurtEncInfo(UINT16 wPlayerLevel, DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, DT_HURT_ENC_DATA& stHurtEncInfo, UINT32 dwHurtValue)
{
    //AUTOCYCLE;

    UINT32 dwEncCoin = 0;
    UINT32 dwEncJingJie = 0;
    if(0 != dwHurtValue)
    {
        dwEncCoin =  CLuamgr::Instance()->GetBossBHurtEncCoin(wPlayerLevel, dwHurtValue);
        dwEncJingJie =  CLuamgr::Instance()->GetBossBHurtEncJingJie(wPlayerLevel, dwHurtValue);
    }
    stDT_BOSS_BATTLE_PLAYER_DATA.dwEncCoin += dwEncCoin;
    stDT_BOSS_BATTLE_PLAYER_DATA.dwEncJingJie += dwEncJingJie;
    stHurtEncInfo.dwEncCoin = stDT_BOSS_BATTLE_PLAYER_DATA.dwEncCoin;
    stHurtEncInfo.dwEncJingJie = stDT_BOSS_BATTLE_PLAYER_DATA.dwEncJingJie;
}


VOID CBossBattleMgr::Addattention(CPlayer* poNoticePlayer)
{
    //AUTOCYCLE;

    if(NULL == poNoticePlayer)
    {
        return;
    }
    UINT32 dwNoticePlayerID = poNoticePlayer->GetID();
    CBossBattle* poNoticeBossBattle = poNoticePlayer->GetBossBattle();
    UINT32 dwSize = m_vecAllEnterPlayer.size();
    //PKT_BOSSB_POS_NTF stNtf;
    //stNtf.dwPlayerNum = m_vecAllEnterPlayer.size();
    //stNtf.bySyncType = BOSSB_POS_TYPE_ENTER;
    //GetBOSSB_PLAYER_DATA_CLI(poNoticePlayer, stNtf.stPlayerInfo);
    if(dwSize <= (UINT32)(m_pstCurBossBProp->byShowNum + 1))
    {
        for(UINT8 byIdx = 0; (byIdx < dwSize); byIdx++)
        {
            UINT32 dwID = m_vecAllEnterPlayer[byIdx];
            if(dwNoticePlayerID == dwID)
            {
                continue;
            }
            CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
            if(NULL == poPlayer)
            {
                continue;
            }
            ////给关注的玩家发Enter通知
            //poPlayer->SendMsg((const CHAR*)(&stNtf), BOSSB_POS_NTF);
            //互加关注
            poNoticeBossBattle->OnAddBeattaction(dwID, TRUE);
            CBossBattle *poBossBattle = poPlayer->GetBossBattle();
            if(poBossBattle)
                poBossBattle->OnAddBeattaction(dwNoticePlayerID);
        }
    }
    else
    {
        BOOL bRereturn = FALSE;
        UINT32 dwSyncStartIdx = poNoticeBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().dwSyncStartIdx;
        UINT8 byNum = 0;
        for(UINT8 byIdx = dwSyncStartIdx; ((byIdx < dwSize) && (byNum < MAX_BOSSB_SHOW_PLAYER_NUM) && (byNum < m_pstCurBossBProp->byShowNum)); )
        {
            UINT32 dwPlayerID = m_vecAllEnterPlayer[byIdx];
            if(dwNoticePlayerID != dwPlayerID)
            {
                CPlayer* poPlayerTmp = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
                if(poPlayerTmp)
                {
                    CBossBattle *poBossBattle = poPlayerTmp->GetBossBattle();
                    if(poBossBattle)
                        poBossBattle->OnAddBeattaction(dwNoticePlayerID, TRUE);
                    byNum++;
                }
            }

            byIdx++;
            if((byIdx >= dwSize) &&  (!bRereturn )) //到末尾，从头
            {
                byIdx = 0;
                bRereturn = TRUE;
            }
            if(bRereturn && byIdx >= dwSyncStartIdx) //已经到起始点
            {
                break;
            }
        }
    }
}


VOID CBossBattleMgr::Delattention(CPlayer* poNoticePlayer)
{
    //AUTOCYCLE;

    if(NULL == poNoticePlayer)
    {
        return;
    }
    CBossBattle* poNoticeBossBattle = poNoticePlayer->GetBossBattle();
    if(NULL == poNoticeBossBattle)
    {
        return;
    }
    UINT32 dwNoticePlayerID = poNoticePlayer->GetID();
    UINT32 dwSize = m_vecAllEnterPlayer.size();
    //PKT_BOSSB_POS_NTF stNtf;
    //stNtf.dwPlayerNum = m_vecAllEnterPlayer.size();
    //stNtf.bySyncType = BOSSB_POS_TYPE_LEAVE;
    //GetBOSSB_PLAYER_DATA_CLI(poNoticePlayer, stNtf.stPlayerInfo);
    C3232Map& mapBeattentionID = poNoticeBossBattle->GetBeattentionIDMap();
    for(C3232MapItr itr = mapBeattentionID.begin(); itr != mapBeattentionID.end(); )
    {
        UINT32 dwID = itr->first;
        CPlayer* poFans = CPlayerMgr::Instance()->FindPlayer(dwID);
        if((NULL == poFans) || (poFans == poNoticePlayer) || (m_mapAllEnterPlayer.find(dwID) == m_mapAllEnterPlayer.end()))
        {
            mapBeattentionID.erase(itr++);
            continue;
        }
        ////给关注的玩家发Leave通知
        //poFans->SendMsg((const CHAR*)(&stNtf), BOSSB_POS_NTF);
        CBossBattle* poFansBossBattle = poFans->GetBossBattle();
        poFansBossBattle->OnDelBeattaction(dwNoticePlayerID);
        if(dwSize > m_pstCurBossBProp->byShowNum)//若玩家足够，给poFans补充关注
        {
            PKT_BOSSB_POS_NTF stEnterNtf;
            UINT32 dwSyncIdx = poFansBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().dwSyncStartIdx;
            UINT32 dwEnterID = m_vecAllEnterPlayer[(dwSyncIdx + m_pstCurBossBProp->byShowNum - 1) % dwSize]; //
            if(dwEnterID != dwID)
            {
                poFansBossBattle->OnAddBeattaction(dwEnterID);
                //CPlayer* poEnterPlayer =CPlayerMgr::Instance()->FindPlayer(dwEnterID);
                //if(poEnterPlayer)
                //{
                //	GetBOSSB_PLAYER_DATA_CLI(poEnterPlayer, stEnterNtf.stPlayerInfo);
                //	//发生Enter通知
                //	poFans->SendMsg((const CHAR*)(&stNtf), BOSSB_POS_NTF);
                //}
            }
        }
        itr++;
    }
    mapBeattentionID.clear();
}

//玩家离开
UINT16 CBossBattleMgr::OnLeave(CPlayer* poPlayer, BOOL bDisconnect)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //不在活动时间段内，直接返回成功
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        return ERR_LEAVE_BOSSB::ID_SUCCESS;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    if(!poPlayer->GetBuild(EBK_BOSSBATTLE))
    {
        return ERR_LEAVE_BOSSB::ID_BOSSB_UNLOCK;
    }

    //赋值离开时间
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_LEAVE_BOSSB::ID_BOSSB_UNLOCK;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
    stDT_BOSS_BATTLE_PLAYER_DATA.qwLastLeaveActivityTime = SDTimeSecs();

    //管理清除
    m_mapAllEnterPlayer.erase(dwPlayerID);
    vector<UINT32>::iterator itr = find(m_vecAllEnterPlayer.begin(), m_vecAllEnterPlayer.end(), dwPlayerID);
    if(itr != m_vecAllEnterPlayer.end())
    {
        m_vecAllEnterPlayer.erase(itr);
    }

    //去除关注
    Delattention(poPlayer);

    if(!bDisconnect)
    {
        poPlayer->GetLocale().OnLeaveBossB();
    }

    return ERR_LEAVE_BOSSB::ID_SUCCESS;
}

//玩家移动
UINT16 CBossBattleMgr::OnMove(CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY, UINT16& wOldPosX, UINT16& wOldPosY)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    if(!poPlayer->GetBuild(EBK_BOSSBATTLE))
    {
        return ERR_BOSSB_MOVE::ID_BOSSB_UNLOCK;
    }

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return ERR_BOSSB_MOVE::ID_BOSSB_UNSTART;
    }

    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_LEAVE_BOSSB::ID_BOSSB_UNLOCK;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
    wOldPosX = stDT_BOSS_BATTLE_PLAYER_DATA.wPosX;
    wOldPosY = stDT_BOSS_BATTLE_PLAYER_DATA.wPosY;

    //判断是否已进入boss战
    UINT32 dwPassSec = GetDayPassSecond(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime);
    if((dwPassSec < m_pstCurBossBProp->dwStartEnterTime) || (dwPassSec > m_pstCurBossBProp->dwEndTime))
    {
        return ERR_BOSSB_MOVE::ID_NOT_IN_ACTIVITY;
    }

    //在活动未开始，但可以进入，移动不能超过开始线
    if (dwPassSec < m_pstCurBossBProp->dwStartTime)
    {
        wPosX = 450;
    }

    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        return ERR_BOSSB_MOVE::ID_NOT_IN_ACTIVITY;
    }

    ////是否准备CD中
    //if((GetDayPassSecond() < m_pstCurBossBProp->dwStartTime))// < m_pstCurBossBProp->wReadyCD) && (wPosY > m_pstCurBossBProp->wMaxReadyPosY))
    //{
    //	return ERR_BOSSB_MOVE::ID_READY_CD;
    //}

    //判断位置是否没改变
    if((wPosX == stDT_BOSS_BATTLE_PLAYER_DATA.wPosX) && (wPosY == stDT_BOSS_BATTLE_PLAYER_DATA.wPosY))
    {
        return ERR_BOSSB_MOVE::ID_SUCCESS;
    }

    ////判断是否复活CD中
    //if(0 != GetReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA))
    //{
    //	return ERR_BOSSB_MOVE::ID_RELIVE_CD;
    //}

    //判断移动是否过于频繁
    SDGetTickCount();
    UINT64& qwLastMoveTime = poBossBattle->GetLastMoveTime();
    if(SDTimeMilliSec() - qwLastMoveTime < m_pstCurBossBProp->wMoveMinCD)
    {
        return ERR_BOSSB_MOVE::ID_MOVE_FREQUENTLY;
    }

    //客户端判断即可
    ////判断移动位置是否过大
    //if(abs(stDT_BOSS_BATTLE_PLAYER_DATA.wPosX - wPosX) >= m_pstCurBossBProp->wMoveMaxLen)
    //{
    //	return ERR_BOSSB_MOVE::ID_MOVE_LEN_INVALID;
    //}

    //赋值
    stDT_BOSS_BATTLE_PLAYER_DATA.wPosX = wPosX;
    stDT_BOSS_BATTLE_PLAYER_DATA.wPosY = wPosY;
    qwLastMoveTime = SDTimeMilliSec();

    ////Move通知
    //UINT32 dwSize = m_vecAllEnterPlayer.size();
    //PKT_BOSSB_POS_NTF stNtf;
    //stNtf.bySyncType = BOSSB_POS_TYPE_MOVE;
    //GetBOSSB_PLAYER_DATA_CLI(poPlayer, stNtf.stPlayerInfo);
    //stNtf.dwPlayerNum = m_vecAllEnterPlayer.size();
    C3232Map& mapBeattentionID = poBossBattle->GetBeattentionIDMap();
    for(C3232MapItr itr = mapBeattentionID.begin(); itr != mapBeattentionID.end();)
    {
        UINT32 dwID = itr->first;
        CPlayer* poFans = CPlayerMgr::Instance()->FindPlayer(dwID);
        if((NULL == poFans) || (poFans == poPlayer) || (m_mapAllEnterPlayer.find(dwID) == m_mapAllEnterPlayer.end()))
        {
            poBossBattle->LeaveID(dwID);
            mapBeattentionID.erase(itr++);
            continue;
        }
        ////发送玩家Move通知
        //poFans->SendMsg((const CHAR*)(&stNtf), BOSSB_POS_NTF);

        CBossBattle *poBossBattle = poFans->GetBossBattle();
        if(poBossBattle)
            poBossBattle->MoveID(dwPlayerID);

        itr++;
    }

    wOldPosX = stDT_BOSS_BATTLE_PLAYER_DATA.wPosX;
    wOldPosY = stDT_BOSS_BATTLE_PLAYER_DATA.wPosY;

    return 0;
}

//玩家攻击
UINT16 CBossBattleMgr::OnHurt(CPlayer* poPlayer, UINT32& dwCurHurtValue, DT_BOSSB_HURT_RATE& stMyHurtInfo, DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM],
                              DT_HURT_ENC_DATA& stHurtEncInfo, UINT16& wReliveCountDown, UINT16& wGoldReliveCountDown, UINT64& qwTotalBossHP, UINT64& qwCurBossHP)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    if(!poPlayer->GetBuild(EBK_BOSSBATTLE))
    {
        return ERR_BOSSB_HURT::ID_BOSSB_UNLOCK;
    }

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return ERR_BOSSB_HURT::ID_BOSSB_UNSTART;
    }

    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_BOSSB_HURT::ID_BOSSB_UNLOCK;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();

    //判断是否已进入boss战
    UINT32 dwPassSec = GetDayPassSecond(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime);
    if((dwPassSec < m_pstCurBossBProp->dwStartEnterTime) || (dwPassSec > m_pstCurBossBProp->dwEndTime))
    {
        return ERR_BOSSB_HURT::ID_NOT_IN_ACTIVITY;
    }
    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        return ERR_BOSSB_HURT::ID_NOT_IN_ACTIVITY;
    }

    //是否准备CD中
    //if(GetDayPassSecond() - m_pstCurBossBProp->dwStartTime < m_pstCurBossBProp->wReadyCD)
    if(GetDayPassSecond() < m_pstCurBossBProp->dwStartTime)
    {
        return ERR_BOSSB_HURT::ID_READY_CD;
    }

    //判断是否复活CD中,计算走路的时间为1秒
    if(0 != GetReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA, 1))
    {
        return ERR_BOSSB_HURT::ID_RELIVE_CD;
    }

    if(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime > stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LastDeadTime:%llu, LastReliveTime:%llu, Now:%llu, between dead:%llu!, PlayerID=%lu"),
                     MSG_MARK, stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime, stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime, SDTimeSecs(), SDTimeSecs() - stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime, 
					 poPlayer->GetID());
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LastDeadTime:%llu, LastReliveTime:%llu, Now:%llu, between relive:%llu!, PlayerID=%lu"),
                     MSG_MARK, stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime, stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime, SDTimeSecs(), stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime - stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime,
					 poPlayer->GetID());
    }

    stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime = SDTimeSecs();

    //计算伤害及记录
    dwCurHurtValue = CountHurtValue(poPlayer, stMyHurtInfo, astTopHurtInfo);
	
    GetHurtEncInfo(poPlayer->GetLevel(), stDT_BOSS_BATTLE_PLAYER_DATA, stHurtEncInfo, dwCurHurtValue);
    wReliveCountDown = GetReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA);
    wGoldReliveCountDown = GetGoldReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA);

    qwTotalBossHP = m_poBoss->GetDT_BATTLE_ATTRIBUTE().qwHP;
    qwCurBossHP = m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP;

    return ERR_BOSSB_HURT::ID_SUCCESS;
}


VOID CBossBattleMgr::GetShowPlayerInfo(CPlayer* poPlayer, UINT32 dwSyncStartIdx, DT_BOSSB_PLAYER_DATA_CLI_LST& stDT_BOSSB_PLAYER_DATA_CLI_LST)
{
    //AUTOCYCLE;

    UINT32 dwSize = m_vecAllEnterPlayer.size();
    if(dwSyncStartIdx >= m_vecAllEnterPlayer.size())
    {
        dwSyncStartIdx = 0;
    }

    UINT32 dwMyID = poPlayer->GetID();
    UINT8& byNum = stDT_BOSSB_PLAYER_DATA_CLI_LST.byNum;
    BOOL bRereturn = FALSE;
    for(UINT8 byIdx = dwSyncStartIdx; ((byIdx < dwSize) && (byNum < MAX_BOSSB_SHOW_PLAYER_NUM) && (byNum < m_pstCurBossBProp->byShowNum)); )
    {
        UINT32 dwPlayerID = m_vecAllEnterPlayer[byIdx];
        if(dwMyID != dwPlayerID)
        {
            CPlayer* poPlayerTmp = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
            if(poPlayerTmp)
            {
                GetBOSSB_PLAYER_DATA_CLI(poPlayerTmp, stDT_BOSSB_PLAYER_DATA_CLI_LST.astInfo[stDT_BOSSB_PLAYER_DATA_CLI_LST.byNum++]);
            }
        }

        byIdx++;
        if((byIdx >= dwSize) &&  (!bRereturn )) //到末尾，从头
        {
            byIdx = 0;
            bRereturn = TRUE;
        }
        if(bRereturn && byIdx >= dwSyncStartIdx) //已经到起始点
        {
            break;
        }
    }

}

VOID CBossBattleMgr::GetBOSSB_PLAYER_DATA_CLI(CPlayer* poPlayer, DT_BOSSB_PLAYER_DATA_CLI& stDT_BOSSB_PLAYER_DATA_CLI)
{
    //AUTOCYCLE;

    stDT_BOSSB_PLAYER_DATA_CLI.dwPlayerID = poPlayer->GetID();
    SDStrcpy(stDT_BOSSB_PLAYER_DATA_CLI.aszDispName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);
    stDT_BOSSB_PLAYER_DATA_CLI.wHeroKindID = poPlayer->GetCoachHeroKindID();
    stDT_BOSSB_PLAYER_DATA_CLI.wLevel = poPlayer->GetLevel();
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(poBossBattle)
    {
        stDT_BOSSB_PLAYER_DATA_CLI.wPosX = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().wPosX;
        stDT_BOSSB_PLAYER_DATA_CLI.wPosY = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().wPosY;
    }
}

UINT32 CBossBattleMgr::CountHurtValue(CPlayer* poPlayer, DT_BOSSB_HURT_RATE& stMyHurtInfo, DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM])
{
    //AUTOCYCLE;

    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return 0;
    }
    UINT32 dwHurtValue = 0;
    //UINT32 dwIncHurtValue = 0;
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();

    UINT32 dwPower = (UINT32)(UINT64(poPlayer->GetDT_PLAYER_BASE_DATA().dwPower) * (100 + stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate) / 100);
    dwHurtValue = CLuamgr::Instance()->GetBossBHurtValueByPower(dwPower);

    //stDT_BOSS_BATTLE_PLAYER_DATA.dwTotalHurtValue += (dwHurtValue + dwIncHurtValue);
    m_poBoss->AddHPChange(-(INT32)dwHurtValue, ET_NORMAL, EHPCR_NORMAL_SKILL, 0);
	USR_INFO(_SDT("[%s:%u] CurHurtValue = %lu, PlayerID = %u, CurBossHP = %llu"), MSG_MARK, dwHurtValue, poPlayer->GetID(), m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP);
    stDT_BOSS_BATTLE_PLAYER_DATA.dwTotalHurtValue += (dwHurtValue);

    if(0 != m_qwBossTotalHP)
    {
        stMyHurtInfo.dwHurtValue = stDT_BOSS_BATTLE_PLAYER_DATA.dwTotalHurtValue;
        stMyHurtInfo.dwHurtRate = (UINT32)((UINT64)(stMyHurtInfo.dwHurtValue) * 100 * 100 / m_qwBossTotalHP); //万分比
    }
    else
    {
        stMyHurtInfo.dwHurtRate = 0;
    }
    UINT32 dwPlayerID = poPlayer->GetID();
    stMyHurtInfo.dwPlayerID = dwPlayerID;
    SDStrcpy(stMyHurtInfo.aszDispName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);

    SortTopHurt(stMyHurtInfo);

    memcpy(astTopHurtInfo, m_astTopHurtInfo, sizeof(m_astTopHurtInfo));

    C3232Map& mapBeattentionID = poBossBattle->GetBeattentionIDMap();
    for(C3232MapItr itr = mapBeattentionID.begin(); itr != mapBeattentionID.end(); )
    {
        UINT32 dwID = itr->first;
        CPlayer* poFans = CPlayerMgr::Instance()->FindPlayer(dwID);
        if((NULL == poFans) || (poFans == poPlayer) || (m_mapAllEnterPlayer.find(dwID) == m_mapAllEnterPlayer.end()))
        {
            poBossBattle->LeaveID(dwID);
            mapBeattentionID.erase(itr++);
            continue;
        }
        //poFans->GetBossBattle()->HurtID(dwPlayerID, dwCurTotalHurtValue);
        CBossBattle *poBossBattle = poFans->GetBossBattle();
        if(poBossBattle)
            poBossBattle->HurtID(dwPlayerID, dwHurtValue);

        itr++;
    }

    //死亡，活动结束
    if(m_poBoss->IsDead())
    {
        ActivityOver(poPlayer);
    }

    poPlayer->EndBattle();

    //return dwCurTotalHurtValue;
    return dwHurtValue;
}


VOID CBossBattleMgr::SortTopHurt(DT_BOSSB_HURT_RATE& stMyHurtInfo)
{
    BOOL bChange = FALSE;
    vector<DT_BOSSB_HURT_RATE> vecTmp;
    vecTmp.reserve(MAX_BOSS_ENC_TOP_NUM + 1);
    for(UINT8 byIdx = 0; byIdx < MAX_BOSS_ENC_TOP_NUM ; byIdx++)
    {
        DT_BOSSB_HURT_RATE& stDT_BOSSB_HURT_RATE = m_astTopHurtInfo[byIdx];
        if((0 == stDT_BOSSB_HURT_RATE.dwHurtValue) && (!bChange))
        {
            vecTmp.push_back(stMyHurtInfo);
            bChange = TRUE;
        }
        if(((stMyHurtInfo.dwHurtValue > stDT_BOSSB_HURT_RATE.dwHurtValue) || ((stMyHurtInfo.dwHurtValue == stDT_BOSSB_HURT_RATE.dwHurtValue) && (stMyHurtInfo.dwPlayerID == stDT_BOSSB_HURT_RATE.dwPlayerID)))
                && (!bChange))
        {
            vecTmp.push_back(stMyHurtInfo);
            bChange = TRUE;
        }
        if((bChange) && (stDT_BOSSB_HURT_RATE.dwPlayerID == stMyHurtInfo.dwPlayerID))
        {
            continue;
        }
        vecTmp.push_back(stDT_BOSSB_HURT_RATE);
    }

    if(bChange)
    {
        memset(m_astTopHurtInfo, 0, sizeof(m_astTopHurtInfo));
        UINT8 bySize = vecTmp.size();
        for(UINT8 byIdx = 0; ((byIdx < MAX_BOSS_ENC_TOP_NUM) && ( byIdx < bySize)); byIdx++)
        {
            memcpy(&(m_astTopHurtInfo[byIdx]), &(vecTmp[byIdx]), sizeof(DT_BOSSB_HURT_RATE));
        }
    }
}

// 活动开始时把开始通知发到活动聊天频道
void CBossBattleMgr::AddChatOnActivityStart(UINT64 qwActivityStartTime, UINT64 qwNow)
{
    // 如果已经超过活动正常开始时间，则不必在聊天频道发出活动开始的公告（因为这说明服务器重启过）
    if(qwActivityStartTime > 0 && qwNow > qwActivityStartTime)
    {
        return;
    }

    // 添加活动聊天：BOSS战已开始，请各位少侠火速前往麒麟洞镇压火麒麟
    string strChat = CMsgDefMgr::Instance()->GetErrMsg("BOSS_BATTLE_START");
    CChatMgr::Instance()->AddActivityChat(strChat, qwActivityStartTime);
}

//活动结束时把boss战战报发到活动聊天频道
void CBossBattleMgr::AddChatOnActivityOver(DT_BOSS_BATTLE_DATA& stReport)
{
    std::vector<string> vecFillMsg;

    // 聊天消息前缀：本次击杀火麒麟活动中，
    string strReportPrefix = CMsgDefMgr::Instance()->GetErrMsg("BOSS_BATTLE_REPORT");

    CHAR szBuf[64] = {0};

    // 统计伤害最高的前几名玩家的奖励信息（2014年1月2日注：这里只能显示前3名玩家的奖励，从第4名开始，playeID和玩家昵称虽然都不为空，但铜钱和境界奖励均为0，被坑了）
    for(BYTE i = 0; i < 3; ++i)
    {
        DT_BOSS_BATTLE_ENC_DATA &stEnc = stReport.astBossBTopEncInfo[i];
        if(0 == stEnc.dwPlayerID)
        {
            break;
        }

        vecFillMsg.clear();

        sprintf(szBuf, "%u", i + 1);
        vecFillMsg.push_back(szBuf);
        vecFillMsg.push_back(stEnc.aszDispName);
        sprintf(szBuf, "%u", stEnc.dwCoin);
        vecFillMsg.push_back(szBuf);
        sprintf(szBuf, "%u", stEnc.dwJingJie);
        vecFillMsg.push_back(szBuf);

        // 加上：第{xx}名【{玩家名字}】,奖励铜钱{M},境界点{N}
        string strReport = strReportPrefix + CMsgDefMgr::Instance()->GetErrMsg("BOSS_BATTLE_RANK_ENCOURAGE", &vecFillMsg); // 直接用+=

        // 最终输出的信息是：本次击杀火麒麟活动中，第x名[{玩家名字}]，奖励铜钱{x}点，境界{y}点
        CChatMgr::Instance()->AddActivityChat(strReport);
    }
}

//活动Boss被击杀时，把boss击杀信息发到活动聊天频道
void CBossBattleMgr::AddChatOnBossKilled(DT_BOSS_BATTLE_DATA& stData)
{
    CHAR szBuf[32] = {0};
    std::vector<string> vecFillMsg;

    vecFillMsg.push_back(stData.stBossBKillEncInfo.aszDispName);
    sprintf(szBuf, "%u", stData.stBossBKillEncInfo.dwCoin);
    vecFillMsg.push_back(szBuf);

    // 提示：世界BOSS火麒麟已被玩家{玩家名字}击杀，获得奖励:铜钱{x}.
    string strActivityNoticeChat = CMsgDefMgr::Instance()->GetErrMsg("BOSS_BATTLE_BOSS_BE_KILLED", &vecFillMsg);
    CChatMgr::Instance()->AddActivityChat(strActivityNoticeChat);
}

VOID CBossBattleMgr::ActivityOver(CPlayer* poKiller)
{
    //AUTOCYCLE;

    //排序
    SortHurtRank();

    //活动结束，结算奖励，结束通知,记录模块数据
    UINT64 qwOverTime = SDTimeSecs();
    PKT_BOSSB_OVER_NTF2 stNtf;
    DT_BOSSB_OVER_INFO2& stBossOverInfo = stNtf.stBossOverInfo;
    //计算模块数据
    DT_BOSS_BATTLE_DATA& stDT_BOSS_BATTLE_DATA = m_mapBassBattleModuleData[m_pstCurBossBProp->byActivityId];
    memset(&stDT_BOSS_BATTLE_DATA , 0 , sizeof(stDT_BOSS_BATTLE_DATA));
    stDT_BOSS_BATTLE_DATA.qwOverTime = qwOverTime;

    //top奖励
    for(UINT8 byIdx = 0; byIdx < MAX_BOSS_ENC_TOP_NUM; byIdx++)
    {
        DT_BOSS_BATTLE_ENC_DATA& stTopEnc1 = stDT_BOSS_BATTLE_DATA.astBossBTopEncInfo[byIdx];
        if(0 == m_astTopHurtInfo[byIdx].dwPlayerID)
        {
            continue;
        }
        stTopEnc1.dwPlayerID = m_astTopHurtInfo[byIdx].dwPlayerID;
        stTopEnc1.dwCoin =  CLuamgr::Instance()->GetBossBTopEncCoin(CPlayerBaseDataCacheMgr::Instance()->GetLevel(m_astTopHurtInfo[byIdx].dwPlayerID), byIdx + 1);
        stTopEnc1.dwJingJie =  CLuamgr::Instance()->GetBossBTopEncJingJie(CPlayerBaseDataCacheMgr::Instance()->GetLevel(m_astTopHurtInfo[byIdx].dwPlayerID), byIdx + 1);
        SDStrcpy(stTopEnc1.aszDispName, m_astTopHurtInfo[byIdx].aszDispName);
        memcpy(&stBossOverInfo.astTopEncInfo[byIdx], &stTopEnc1, sizeof(stTopEnc1));
    }
    //击杀奖励
    if(poKiller)
    {
		CBossBattle* poBossBattle = poKiller->GetBossBattle();
		if ( NULL != poBossBattle)
		{
			poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().qwLastKillBossTime = SGDP::SDTimeSecs();
		}
        DT_BOSS_BATTLE_ENC_DATA& stKillEnc1 = stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo;
        stKillEnc1.dwPlayerID = poKiller->GetID();
        stKillEnc1.dwCoin =  CLuamgr::Instance()->GetBossBKillEncCoin(m_wBossLevel);
        stKillEnc1.dwJingJie =  CLuamgr::Instance()->GetBossBKillEncJingJie(m_wBossLevel);
        SDStrcpy(stKillEnc1.aszDispName, poKiller->GetDT_PLAYER_BASE_DATA().aszDispName);
        memcpy(&stBossOverInfo.stKillEncInfo, &stKillEnc1, sizeof(stKillEnc1));

        stBossOverInfo.byBossDeadFlag = 1;
        if(GetDayPassSecond() - m_pstCurBossBProp->dwStartTime < m_pstCurBossBProp->wUpgradeCostMaxKillSecs)
        {
            //如果BOSS达到最高等级，不再下发升级提示信息
            if (m_pstCurBossBProp->wBossMaxLevel > m_wBossLevel)
            {
                stDT_BOSS_BATTLE_DATA.byNeedUpgrade = TRUE;
                stBossOverInfo.byNeedUpgrade = 1;
            }
        }

        // 把boss被击杀信息发到活动聊天频道
        AddChatOnBossKilled(stDT_BOSS_BATTLE_DATA);
    }
    //top伤害
    memcpy(stBossOverInfo.astTopHurtInfo, m_astTopHurtInfo, sizeof(m_astTopHurtInfo));
    stBossOverInfo.qwTotalBossHP = m_poBoss->GetDT_BATTLE_ATTRIBUTE().qwHP;
    stBossOverInfo.qwCurBossHP = m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP;

    //给奖励，并结束通知
    UINT32 dwSize = m_mapAllPlayer.size();
    for(C3232MapItr itr = m_mapAllPlayer.begin(); itr != m_mapAllPlayer.end(); itr++)
    {
        UINT32 dwID = itr->second;
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
        if(NULL == poPlayer)//活动应该不会超过1个小时，若真不在内存中暂时不给奖励
        {
            continue;
        }
        CBossBattle* poBossBattle = poPlayer->GetBossBattle();
        if(NULL == poBossBattle)
        {
            continue;
        }
        DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
        //自身伤害
        DT_BOSSB_HURT_RATE& stMyHurtInfo = stBossOverInfo.stMyHurtInfo;
        //if(0 != dwBossTotalHurt)
        if(0 != m_qwBossTotalHP)
        {
            stMyHurtInfo.dwHurtValue = stDT_BOSS_BATTLE_PLAYER_DATA.dwTotalHurtValue;
            stMyHurtInfo.dwHurtRate = (UINT32)((UINT64)(stMyHurtInfo.dwHurtValue) * 100 * 100 / m_qwBossTotalHP); //万分比
            stMyHurtInfo.dwHurtRate = stMyHurtInfo.dwHurtRate > 10000 ? 10000 : stMyHurtInfo.dwHurtRate;
        }
        else
        {
            stMyHurtInfo.dwHurtRate = 0;
        }
        stMyHurtInfo.dwHurtRank = stDT_BOSS_BATTLE_PLAYER_DATA.dwHurtRank;
        stMyHurtInfo.dwPlayerID = poPlayer->GetID();
        SDStrcpy(stMyHurtInfo.aszDispName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);
        memcpy(&stDT_BOSS_BATTLE_PLAYER_DATA.stMyHurtInfo, &stMyHurtInfo, sizeof(stMyHurtInfo));

        //伤害奖励
        DT_BOSS_BATTLE_ENC_DATA& stHurtEncInfo = stBossOverInfo.stHurtEncInfo;
        stHurtEncInfo.dwCoin = stDT_BOSS_BATTLE_PLAYER_DATA.dwEncCoin;
        stHurtEncInfo.dwJingJie = stDT_BOSS_BATTLE_PLAYER_DATA.dwEncJingJie;
        stHurtEncInfo.dwPlayerID = poPlayer->GetID();
        SDStrcpy(stMyHurtInfo.aszDispName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);
        memcpy(&stDT_BOSS_BATTLE_PLAYER_DATA.stHurtEncInfo, &stHurtEncInfo, sizeof(stHurtEncInfo));

        //英雄奖励, >=1%
        DT_BOSS_BATTLE_ENC_DATA& stHeroEncInfo = stBossOverInfo.stHeroEncInfo;
        memset(&stHeroEncInfo, 0, sizeof(DT_BOSS_BATTLE_ENC_DATA));
        memset(&stDT_BOSS_BATTLE_PLAYER_DATA.stHeroEncInfo, 0, sizeof(DT_BOSS_BATTLE_ENC_DATA));
        if(stMyHurtInfo.dwHurtRate >= 100)
        {
			CBossBattle* poBossBattle = poPlayer->GetBossBattle();
			if ( NULL != poBossBattle)
			{
				poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().qwBossHurtTime = SGDP::SDTimeSecs();
			}
            stHeroEncInfo.dwCoin = CLuamgr::Instance()->GetBossBHeroHurtEncCoin();
            stHeroEncInfo.dwJingJie = CLuamgr::Instance()->GetBossBHeroHurtEncJingJie();
            stHeroEncInfo.dwPlayerID = poPlayer->GetID();
            SDStrcpy(stHeroEncInfo.aszDispName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);
            memcpy(&stDT_BOSS_BATTLE_PLAYER_DATA.stHeroEncInfo, &stHeroEncInfo, sizeof(DT_BOSS_BATTLE_ENC_DATA));
        }

        //给奖励
        GiveEnc(poPlayer, stNtf.stBossOverInfo);

        const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
        stNtf.qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
        stNtf.qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
        stNtf.qwCurStory = stDT_PLAYER_BASE_DATA.qwStory;
        stNtf.qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;

        //记录战报
        DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA = {0};
        stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.byActivityID = m_pstCurBossBProp->byActivityId;
        stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.qwOverTime = qwOverTime;
        memcpy(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stMyHurtInfo, &stBossOverInfo.stMyHurtInfo, sizeof(DT_BOSSB_HURT_RATE));
        memcpy(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stHurtEncInfo, &stBossOverInfo.stHurtEncInfo, sizeof(DT_BOSS_BATTLE_ENC_DATA));
        memcpy(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stHeroEncInfo, &stBossOverInfo.stHeroEncInfo, sizeof(DT_BOSS_BATTLE_ENC_DATA));
        poBossBattle->AddBattleLog(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA);
        stNtf.byUnreadBattleLogNum = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().byUnreadBattleLogNum;

        //通知
		if ( VER_PET <= poPlayer->GetCliVer())
		{
			poPlayer->SendMsg((const CHAR*)(&stNtf), BOSSB_OVER_NTF2);
		}
		else
		{
			static PKT_BOSSB_OVER_NTF stNtfOld;
			stNtfOld.byUnreadBattleLogNum = stNtf.byUnreadBattleLogNum;
			stNtfOld.qwCurCoin = stNtf.qwCurCoin;
			stNtfOld.qwCurGold = stNtf.qwCurGold;
			stNtfOld.qwCurJingJie = stNtf.qwCurJingJie;
			stNtfOld.qwCurStory =stNtf.qwCurStory;
			ConvertInfo2ToInfo(stNtf.stBossOverInfo, stNtfOld.stBossOverInfo);
			poPlayer->SendMsg((const CHAR*)(&stNtfOld), BOSSB_OVER_NTF);
		}
        

        //退出位置

        poPlayer->GetLocale().OnLeaveBossB();
    }

    //把boss战战报发到活动聊天频道
    AddChatOnActivityOver(stDT_BOSS_BATTLE_DATA);

    //给其他未进入boss战的且已开发boss战的所有在线玩家发生通知
    const CSessionID2UserMap& mapUser = CUserMgr::Instance()->GetAllUserMap();
    //清除个人奖励
    memset(&stBossOverInfo.stMyHurtInfo, 0, sizeof(stBossOverInfo.stMyHurtInfo));
    memset(&stBossOverInfo.stHurtEncInfo, 0, sizeof(stBossOverInfo.stHurtEncInfo));
    memset(&stBossOverInfo.stHeroEncInfo, 0, sizeof(stBossOverInfo.stHeroEncInfo));
    for(CSessionID2UserMapConstItr itr = mapUser.begin(); itr != mapUser.end(); itr++)
    {
        UINT32 dwID = itr->first;
        //跳过进入boss战
        if(m_mapAllEnterPlayer.find(dwID) != m_mapAllEnterPlayer.end())
        {
            continue;
        }
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
        if((NULL == poPlayer) || (!poPlayer->CkLogin())) //要求在线玩家
        {
            continue;
        }
        CBossBattle* poBossBattle = poPlayer->GetBossBattle();
        if(NULL == poBossBattle)
        {
            stNtf.byUnreadBattleLogNum = 0;
        }
        else
        {
            stNtf.byUnreadBattleLogNum = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().byUnreadBattleLogNum;
        }

        const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
        stNtf.qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
        stNtf.qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
        stNtf.qwCurStory = stDT_PLAYER_BASE_DATA.qwStory;
        stNtf.qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
		if ( VER_PET <= poPlayer->GetCliVer())
		{
			//通知
			poPlayer->SendMsg((const CHAR*)(&stNtf), BOSSB_OVER_NTF2);
		}
		else
		{
			static PKT_BOSSB_OVER_NTF stNtfOld;
			stNtfOld.byUnreadBattleLogNum = stNtf.byUnreadBattleLogNum;
			stNtfOld.qwCurCoin = stNtf.qwCurCoin;
			stNtfOld.qwCurGold = stNtf.qwCurGold;
			stNtfOld.qwCurJingJie = stNtf.qwCurJingJie;
			stNtfOld.qwCurStory =stNtf.qwCurStory;
			ConvertInfo2ToInfo(stNtf.stBossOverInfo, stNtfOld.stBossOverInfo);
			poPlayer->SendMsg((const CHAR*)(&stNtfOld), BOSSB_OVER_NTF);
		}
    }

    //记录模块
    OnSave();
    m_mapBossBBLData[stDT_BOSS_BATTLE_DATA.qwOverTime] = stDT_BOSS_BATTLE_DATA;

    //结束
    m_bActivityOver = TRUE;
    m_pstCurBossBProp = NULL;
}

VOID CBossBattleMgr::GiveEnc(CPlayer* poPlayer, DT_BOSSB_OVER_INFO2& stNtf)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        return;
    }

    //伤害奖励信息
    poPlayer->AddCoin(stNtf.stHurtEncInfo.dwCoin, CRecordMgr::EACT_BOSSB_HURTENC);
    poPlayer->AddJingJie(stNtf.stHurtEncInfo.dwJingJie, CRecordMgr::EAJT_BOSSB_HURTENC);

    //英雄奖励信息
    poPlayer->AddCoin(stNtf.stHeroEncInfo.dwCoin, CRecordMgr::EACT_BOSSB_HEROENC);
    poPlayer->AddJingJie(stNtf.stHeroEncInfo.dwJingJie, CRecordMgr::EAJT_BOSSB_HEROENC);

    //top奖励信息
    UINT32 dwPlayerID = poPlayer->GetID();
    for(UINT8 byIdx = 0; byIdx < MAX_BOSS_ENC_TOP_NUM; byIdx++)
    {
        DT_BOSS_BATTLE_ENC_DATA& stTopEnc = stNtf.astTopEncInfo[byIdx];
        if(dwPlayerID == stTopEnc.dwPlayerID)
        {
            poPlayer->AddCoin(stTopEnc.dwCoin, CRecordMgr::EACT_BOSSB_TOPENC, byIdx + 1);
            poPlayer->AddJingJie(stTopEnc.dwJingJie, CRecordMgr::EAJT_BOSSB_TOPENC, byIdx + 1);
            break;
        }
    }

    //最后1击奖励信息
    DT_BOSS_BATTLE_ENC_DATA& stKillEnc = stNtf.stKillEncInfo;
    if(dwPlayerID == stKillEnc.dwPlayerID)
    {
        poPlayer->AddCoin(stKillEnc.dwCoin, CRecordMgr::EACT_BOSSB_KILLENC);
        poPlayer->AddJingJie(stKillEnc.dwJingJie, CRecordMgr::EAJT_BOSSB_KILLENC);
    }
}


UINT16 CBossBattleMgr::OnOpenInspireTab(CPlayer* poPlayer, DT_INSPIRE_TAB_DATA& stInspireTabInfo)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    if(!poPlayer->GetBuild(EBK_BOSSBATTLE))
    {
        return ERR_OPEN_INSPIRE_TAB::ID_BOSSB_UNLOCK;
    }

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return ERR_OPEN_INSPIRE_TAB::ID_BOSSB_UNSTART;
    }

    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_LEAVE_BOSSB::ID_BOSSB_UNLOCK;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
    //判断是否已进入boss战
    UINT32 dwPassSec = GetDayPassSecond(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime);
    if((dwPassSec < m_pstCurBossBProp->dwStartEnterTime) || (dwPassSec > m_pstCurBossBProp->dwEndTime))
    {
        return ERR_OPEN_INSPIRE_TAB::ID_NOT_IN_ACTIVITY;
    }

    GetDT_INSPIRE_TAB_DATA(stDT_BOSS_BATTLE_PLAYER_DATA, stInspireTabInfo);

    return ERR_OPEN_INSPIRE_TAB::ID_SUCCESS;
}

UINT16 CBossBattleMgr::OnInspirePower(CPlayer* poPlayer, UINT8 byGoldInspireFlag, DT_INSPIRE_TAB_DATA& stInspireTabInfo)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    if(!poPlayer->GetBuild(EBK_BOSSBATTLE))
    {
        return ERR_INSPIRE_POWER::ID_BOSSB_UNLOCK;
    }

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return ERR_INSPIRE_POWER::ID_BOSSB_UNSTART;
    }

    //
    if((byGoldInspireFlag) && (!CVipPropMgr::Instance()->CkOpenFunc(EVF_BOSSBGOLDINSPIREPOWER, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel)))
    {
        return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
    }

    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_LEAVE_BOSSB::ID_BOSSB_UNLOCK;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
    //判断是否已进入boss战
    UINT32 dwPassSec = GetDayPassSecond(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime);
    if((dwPassSec < m_pstCurBossBProp->dwStartEnterTime) || (dwPassSec > m_pstCurBossBProp->dwEndTime))
    {
        return ERR_INSPIRE_POWER::ID_NOT_IN_ACTIVITY;
    }

    if(stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate >= m_pstCurBossBProp->wCanIncPowerRate)
    {
        GetDT_INSPIRE_TAB_DATA(stDT_BOSS_BATTLE_PLAYER_DATA, stInspireTabInfo);
        return ERR_INSPIRE_POWER::ID_REACH_MAX;
    }

    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    if(0 == byGoldInspireFlag)
    {
        if(stDT_PLAYER_BASE_DATA.qwStory < m_pstCurBossBProp->wIncCostStory)
        {
            return ERR_INSPIRE_POWER::ID_STORY_NOT_ENOUGH;
        }
        poPlayer->DecStory(m_pstCurBossBProp->wIncCostStory, CRecordMgr::EDST_BOSSB_INSPIREPOWER);
    }
    else
    {
        if(stDT_PLAYER_BASE_DATA.qwGold < m_pstCurBossBProp->wIncCostGold)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
        poPlayer->DecGold(m_pstCurBossBProp->wIncCostGold, CRecordMgr::EDGT_BOSSBINSPIRE);
    }
    stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate += CLuamgr::Instance()->GetInspireRate(byGoldInspireFlag, stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate);
    if(stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate > m_pstCurBossBProp->wCanIncPowerRate)
    {
        stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate = m_pstCurBossBProp->wCanIncPowerRate;
    }

    GetDT_INSPIRE_TAB_DATA(stDT_BOSS_BATTLE_PLAYER_DATA, stInspireTabInfo);

    return ERR_INSPIRE_POWER::ID_SUCCESS;
}

UINT16 CBossBattleMgr::OnBossBRelive(CPlayer* poPlayer)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //判断玩家是否开放boss战
    UINT32 dwPlayerID = poPlayer->GetID();
    if(!poPlayer->GetBuild(EBK_BOSSBATTLE))
    {
        return ERR_BOSSB_RELIVE::ID_BOSSB_UNLOCK;
    }

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return ERR_BOSSB_RELIVE::ID_BOSSB_UNSTART;
    }

    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_LEAVE_BOSSB::ID_BOSSB_UNLOCK;
    }
    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();

    //判断是否已进入boss战
    UINT32 dwPassSec = GetDayPassSecond(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastEnterActivityTime);
    if((dwPassSec < m_pstCurBossBProp->dwStartEnterTime) || (dwPassSec > m_pstCurBossBProp->dwEndTime))
    {
        return ERR_BOSSB_RELIVE::ID_NOT_IN_ACTIVITY;
    }

    //判断是否复活CD中
    if(0 != GetReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA))
    {
        if(0 != GetGoldReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA))
        {
            return ERR_BOSSB_RELIVE::ID_GOLD_RELIVE_CD;
        }
        if(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < m_pstCurBossBProp->wReliveGold)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
        stDT_BOSS_BATTLE_PLAYER_DATA.byLastReliveType = 1;
        stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime = SDTimeSecs();
        poPlayer->DecGold(m_pstCurBossBProp->wReliveGold, CRecordMgr::EDGT_BOSSBRELIVE);
    }
    else //不在CD中，直接返回成功
    {
        return  ERR_BOSSB_RELIVE::ID_SUCCESS;
    }

    return  ERR_BOSSB_RELIVE::ID_SUCCESS;
}
BOOL CBossBattleMgr::IsCloseArea(UINT16 wZoneID)
{
	if ( m_setCloseArea.find(wZoneID) == m_setCloseArea.end())
	{
		return FALSE;
	}
	return TRUE;
}

UINT16 CBossBattleMgr::OnGetLastBossBInfo(CPlayer* poPlayer, DT_BOSSB_OVER_INFO2& stBossOverInfo)
{
    //AUTOCYCLE;

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    UINT32 dwPlayerID = poPlayer->GetID();
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return ERR_GET_LAST_BOSSB_INFO::ID_BOSSB_UNLOCK;
    }
    //活动未结束
    if((m_pstCurBossBProp) || (!m_bActivityOver))
    {
        return ERR_GET_LAST_BOSSB_INFO::ID_BOSSB_UNOVER;
    }

    DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
    memcpy(&stBossOverInfo.stMyHurtInfo, &stDT_BOSS_BATTLE_PLAYER_DATA.stMyHurtInfo, sizeof(DT_BOSSB_HURT_RATE));

    DT_BOSS_BATTLE_DATA& stDT_BOSS_BATTLE_DATA = m_mapBassBattleModuleData[m_byLastActivityID];
    memcpy(&stBossOverInfo.astTopEncInfo, &stDT_BOSS_BATTLE_DATA.astBossBTopEncInfo, sizeof(stBossOverInfo.astTopEncInfo));
    memcpy(&stBossOverInfo.astTopHurtInfo, &stDT_BOSS_BATTLE_DATA.astTopHurtInfo, sizeof(stBossOverInfo.astTopHurtInfo));
    DT_BOSS_BATTLE_ENC_DATA& stKillEnc = stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo;
    //if(dwPlayerID == stKillEnc.dwPlayerID)
    {
        memcpy(&stBossOverInfo.stKillEncInfo, &stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo, sizeof(stBossOverInfo.stKillEncInfo));
    }
    memcpy(&stBossOverInfo.stHurtEncInfo, &stDT_BOSS_BATTLE_PLAYER_DATA.stHeroEncInfo, sizeof(DT_BOSS_BATTLE_ENC_DATA));
    stBossOverInfo.stHurtEncInfo.dwCoin = stDT_BOSS_BATTLE_PLAYER_DATA.dwEncCoin;
    stBossOverInfo.stHurtEncInfo.dwJingJie = stDT_BOSS_BATTLE_PLAYER_DATA.dwEncJingJie;
    memcpy(&stBossOverInfo.stHeroEncInfo, &stDT_BOSS_BATTLE_PLAYER_DATA.stHeroEncInfo, sizeof(DT_BOSS_BATTLE_ENC_DATA));

    stBossOverInfo.qwTotalBossHP = stDT_BOSS_BATTLE_DATA.qwTotalHP;
    stBossOverInfo.qwCurBossHP = stDT_BOSS_BATTLE_DATA.qwCurHP;
    stBossOverInfo.byBossDeadFlag = stDT_BOSS_BATTLE_DATA.qwCurHP == 0 ? 1 : 0;
    stBossOverInfo.byNeedUpgrade = stDT_BOSS_BATTLE_DATA.byNeedUpgrade;

    //数据异常
    if(0 == stBossOverInfo.stMyHurtInfo.dwPlayerID || 0 == stBossOverInfo.stMyHurtInfo.dwHurtRank )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: OnGetLastBossBInfo data 0, playerid:%d!"), MSG_MARK, dwPlayerID);
    }
    return ERR_GET_LAST_BOSSB_INFO::ID_SUCCESS;
}

VOID CBossBattleMgr::GetDT_INSPIRE_TAB_DATA(DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, DT_INSPIRE_TAB_DATA& stInspireTabInfo)
{
    //AUTOCYCLE;

    stInspireTabInfo.wHaveIncPowerRate = stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate;
    stInspireTabInfo.wIncCostStory = m_pstCurBossBProp->wIncCostStory;
    stInspireTabInfo.wIncCostGold = m_pstCurBossBProp->wIncCostGold;
    stInspireTabInfo.wCanIncPowerRate = m_pstCurBossBProp->wCanIncPowerRate;
    stInspireTabInfo.wStoryIncRate = m_wIncRateByStory;
    stInspireTabInfo.wGoldIncRate = m_wIncRateByGold;
}

VOID CBossBattleMgr::GetDT_BOSSB_MY_DATA_CLI(CPlayer* poPlayer, DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, DT_BOSSB_MY_DATA_CLI& stMyBossBInfo)
{
    //AUTOCYCLE;

    if(NULL == m_pstCurBossBProp)
    {
        return ;
    }
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    stMyBossBInfo.qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    stMyBossBInfo.qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    stMyBossBInfo.qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    stMyBossBInfo.qwCurStory = stDT_PLAYER_BASE_DATA.qwStory;
    stMyBossBInfo.wPosX = stDT_BOSS_BATTLE_PLAYER_DATA.wPosX;
    stMyBossBInfo.wPosY = stDT_BOSS_BATTLE_PLAYER_DATA.wPosY;
    stMyBossBInfo.wIncPowerRate = stDT_BOSS_BATTLE_PLAYER_DATA.wIncPowerRate;

    stMyBossBInfo.wReliveCountDown = GetReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA);
    stMyBossBInfo.wGoldReliveCountDown = GetGoldReliveCountDown(stDT_BOSS_BATTLE_PLAYER_DATA);
    stMyBossBInfo.dwHurtValue = stDT_BOSS_BATTLE_PLAYER_DATA.dwTotalHurtValue;

    if(0 != m_qwBossTotalHP)
    {
        stMyBossBInfo.dwHurtRate = (UINT32)((UINT64)(stDT_BOSS_BATTLE_PLAYER_DATA.dwTotalHurtValue) * 100  * 100 / m_qwBossTotalHP);
    }
    else
    {
        stMyBossBInfo.dwHurtRate = 0;
    }
}

UINT16 CBossBattleMgr::GetReliveCountDown(DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, UINT8 byCalculateWalk)
{
    //AUTOCYCLE;
    if(NULL == m_pstCurBossBProp)
    {
        return 0;
    }

    //立即复活的玩家死亡时间会早于复活时间
    if(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime >= stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime)
    {
        UINT64 qwPassTime =  SDTimeSecs() - stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime ;
        if( qwPassTime < (m_pstCurBossBProp->wReliveCD + byCalculateWalk))
        {
            return m_pstCurBossBProp->wReliveCD - (UINT16)qwPassTime + byCalculateWalk;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

UINT16 CBossBattleMgr::GetGoldReliveCountDown(DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA)
{
    //AUTOCYCLE;

    if(NULL == m_pstCurBossBProp)
    {
        return 0;
    }
    if(stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime > stDT_BOSS_BATTLE_PLAYER_DATA.qwLastReliveTime)
    {
        UINT64 qwPassTime =  SDTimeSecs() - stDT_BOSS_BATTLE_PLAYER_DATA.qwLastDeadTime ;
        if( qwPassTime < m_pstCurBossBProp->wGoldReliveCD)
        {
            return m_pstCurBossBProp->wGoldReliveCD - (UINT16)qwPassTime;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    return 0;
}

VOID CBossBattleMgr::GetDT_BOSSB_DATA_CLI(DT_BOSSB_DATA_CLI2& stBossBInfo)
{
    //AUTOCYCLE;

    if(NULL == m_pstCurBossBProp)
    {
        return;
    }

    stBossBInfo.wBossID = m_pstCurBossBProp->wBossMonsterID;
    stBossBInfo.wBossLevel = m_wBossLevel;
    stBossBInfo.qwTotalBossHP = m_poBoss->GetDT_BATTLE_ATTRIBUTE().qwHP;
    stBossBInfo.qwCurBossHP = m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP;
    stBossBInfo.qwHPPerGrid = m_pstCurBossBProp->wHPPerGrid;
    stBossBInfo.dwPlayerNum = m_vecAllEnterPlayer.size();
    UINT32 dwDayPassTime = GetDayPassSecond();
    if(dwDayPassTime > m_pstCurBossBProp->dwStartTime)
    {
        stBossBInfo.wStopCountDown = m_pstCurBossBProp->dwEndTime > dwDayPassTime ? m_pstCurBossBProp->dwEndTime - dwDayPassTime : 0;
    }
    else
    {
        stBossBInfo.wStopCountDown = m_pstCurBossBProp->wKeepTime;
    }
    //stBossBInfo.wReadyCountDown  = m_pstCurBossBProp->dwReadyOverTime > dwDayPassTime ? m_pstCurBossBProp->dwReadyOverTime - dwDayPassTime : 0;
    stBossBInfo.wReadyCountDown  = m_pstCurBossBProp->dwStartTime > dwDayPassTime ? m_pstCurBossBProp->dwStartTime - dwDayPassTime : 0;
    stBossBInfo.wReliveGold = m_pstCurBossBProp->wReliveGold;
}


BOOL MyCmp (CBossBattle* poA, CBossBattle* poB)
{
    return poA->GetDT_BOSS_BATTLE_PLAYER_DATA().dwTotalHurtValue < poB->GetDT_BOSS_BATTLE_PLAYER_DATA().dwTotalHurtValue;
};


VOID CBossBattleMgr::SortHurtRank()
{
    //AUTOCYCLE;

    vector<CBossBattle*> vecAllEnterPlayerBossB;
    for(C3232MapItr itr = m_mapAllPlayer.begin(); itr != m_mapAllPlayer.end(); itr++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second);
        if(NULL == poPlayer)
        {
            continue;
        }
        CBossBattle* poBossBattle = poPlayer->GetBossBattle();
        if((NULL == poBossBattle) || (0 == poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().dwTotalHurtValue)) //伤害0不参加排名
        {
            continue;
        }
        vecAllEnterPlayerBossB.push_back(poBossBattle);
    }

    sort(vecAllEnterPlayerBossB.begin(), vecAllEnterPlayerBossB.end(), MyCmp);
    UINT32 dwSize = vecAllEnterPlayerBossB.size();
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vecAllEnterPlayerBossB[dwIdx]->GetDT_BOSS_BATTLE_PLAYER_DATA().dwHurtRank = dwSize - dwIdx;
    }

}

VOID CBossBattleMgr::GetBattleLog(UINT64 qwOverTime, UINT16& wBossID, DT_BOSS_BATTLE_ENC_DATA astBossBTopEncInfo[MAX_BOSS_ENC_TOP_NUM], DT_BOSS_BATTLE_ENC_DATA& stBossBKillEncInfo)
{
    //AUTOCYCLE;

    memset(astBossBTopEncInfo, 0, sizeof(DT_BOSS_BATTLE_ENC_DATA) * MAX_BOSS_ENC_TOP_NUM);
    map<UINT64, DT_BOSS_BATTLE_DATA>::iterator itr = m_mapBossBBLData.find(qwOverTime);
    if(itr == m_mapBossBBLData.end())
    {
        return;
    }

    DT_BOSS_BATTLE_DATA& stDT_BOSS_BATTLE_DATA = itr->second;
    wBossID = stDT_BOSS_BATTLE_DATA.wBossID;
    memcpy(astBossBTopEncInfo, stDT_BOSS_BATTLE_DATA.astBossBTopEncInfo, sizeof(stDT_BOSS_BATTLE_DATA.astBossBTopEncInfo));
    memcpy(&stBossBKillEncInfo, &stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo, sizeof(stDT_BOSS_BATTLE_DATA.stBossBKillEncInfo));
}

BOOL CBossBattleMgr::OnSave()
{
    //AUTOCYCLE;

    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return FALSE;
    }
    DT_BOSS_BATTLE_DATA& stDT_BOSS_BATTLE_DATA = m_mapBassBattleModuleData[m_pstCurBossBProp->byActivityId];
    stDT_BOSS_BATTLE_DATA.byActivityID = m_pstCurBossBProp->byActivityId;
    stDT_BOSS_BATTLE_DATA.wLastLevel = m_wBossLevel;
    stDT_BOSS_BATTLE_DATA.qwTotalHP = m_qwBossTotalHP;
    stDT_BOSS_BATTLE_DATA.qwCurHP = m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP;
    stDT_BOSS_BATTLE_DATA.qwSaveTime = SDTimeSecs();
    memcpy(stDT_BOSS_BATTLE_DATA.astTopHurtInfo, m_astTopHurtInfo, sizeof(m_astTopHurtInfo));
    CDBPktBuilder::Instance()->SaveBossBDataReq(&stDT_BOSS_BATTLE_DATA);

    return TRUE;
}

BOOL CBossBattleMgr::Run()
{
    //活动是否开启
    if((NULL == m_pstCurBossBProp) || (m_bActivityOver))
    {
        //活动未开始
        return TRUE;
    }

    if(m_itrCur == m_mapAllEnterPlayer.end())
    {
        m_itrCur = m_mapAllEnterPlayer.begin();
    }
    for(UINT16 wNum = 0 ; ((m_itrCur != m_mapAllEnterPlayer.end()) && (wNum < m_pstCurBossBProp->wDealNum)); m_itrCur++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(m_itrCur->first);
        if(NULL == poPlayer)
        {
            continue;
        }
        CBossBattle *poBossBattle = poPlayer->GetBossBattle();
        if(NULL == poBossBattle)
        {
            continue;
        }

        if(SDTimeMilliSec() - poBossBattle->GetLastDealTime() < m_pstCurBossBProp->wDealInterval)
        {
            continue;
        }
        SyncInfo(poPlayer);

        poBossBattle->GetLastDealTime() = SDTimeMilliSec();
    }

    return TRUE;
}
VOID CBossBattleMgr::ConvertInfo2ToInfo(const DT_BOSSB_OVER_INFO2& stBossInfo2, DT_BOSSB_OVER_INFO& stBossOverInfo)
{
	stBossOverInfo.stMyHurtInfo = stBossInfo2.stMyHurtInfo;
	memcpy( stBossOverInfo.astTopEncInfo, stBossInfo2.astTopEncInfo, sizeof(stBossOverInfo.astTopEncInfo));
	memcpy( stBossOverInfo.astTopHurtInfo, stBossInfo2.astTopHurtInfo, sizeof(stBossOverInfo.astTopHurtInfo));
	stBossOverInfo.byBossDeadFlag = stBossInfo2.byBossDeadFlag;
	stBossOverInfo.byNeedUpgrade = stBossInfo2.byNeedUpgrade;
	stBossOverInfo.dwCurBossHP = (UINT32)stBossInfo2.qwCurBossHP;
	stBossOverInfo.dwTotalBossHP = (UINT32)stBossInfo2.qwTotalBossHP;
	stBossOverInfo.stHeroEncInfo = stBossInfo2.stHeroEncInfo;
	stBossOverInfo.stHurtEncInfo = stBossInfo2.stHurtEncInfo;
	stBossOverInfo.stKillEncInfo = stBossInfo2.stKillEncInfo;
	stBossOverInfo.stMyHurtInfo = stBossInfo2.stMyHurtInfo;
}

VOID CBossBattleMgr::SyncInfo(CPlayer* poPlayer)
{
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();

    PKT_BOSSB_INFO_SYNC_NTF2 stNtf;
    stNtf.dwPlayerNum = m_vecAllEnterPlayer.size();
    stNtf.qwTotalBossHP = m_poBoss->GetDT_BATTLE_ATTRIBUTE().qwHP;
    stNtf.qwCurBossHP = m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP;
    memcpy(stNtf.astTopHurtInfo, m_astTopHurtInfo, sizeof(m_astTopHurtInfo));

    C3232Map& mapLeave = poBossBattle->GetLeaveID();
    for(C3232MapItr itr = mapLeave.begin(); ((itr != mapLeave.end()) && (stNtf.byLeaveNum < MAX_BOSSB_SHOW_PLAYER_NUM)); itr++)
    {
        stNtf.adwLeaveInfo[stNtf.byLeaveNum++] = itr->first;
    }
    C3232Map& mapEnter = poBossBattle->GetEnterID();
    for(C3232MapItr itr = mapEnter.begin(); ((itr != mapEnter.end()) && (stNtf.byEnterNum < MAX_BOSSB_SHOW_PLAYER_NUM)); itr++)
    {
        CPlayer* poEnter = CPlayerMgr::Instance()->FindPlayer(itr->first);
        if(NULL == poEnter)
        {
            continue;
        }
        GetBOSSB_PLAYER_DATA_CLI(poEnter, stNtf.astEnterInfo[stNtf.byEnterNum++]);
    }
    C3232Map& mapMove = poBossBattle->GetMoveID();
    for(C3232MapItr itr = mapMove.begin(); ((itr != mapMove.end()) && (stNtf.byMoveNum < MAX_BOSSB_SHOW_PLAYER_NUM)); itr++)
    {
        CPlayer* poMove = CPlayerMgr::Instance()->FindPlayer(itr->first);
        if(NULL == poMove)
        {
            continue;
        }

        CBossBattle *poBossBattle = poMove->GetBossBattle();
        if(NULL == poBossBattle)
        {
            continue;
        }

        DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA = poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA();
        DT_BOSSB_POS& stDT_BOSSB_POS = stNtf.astMoveInfo[stNtf.byMoveNum++];
        stDT_BOSSB_POS.dwPlayerID = itr->first;
        stDT_BOSSB_POS.wPosX = stDT_BOSS_BATTLE_PLAYER_DATA.wPosX;
        stDT_BOSSB_POS.wPosY = stDT_BOSS_BATTLE_PLAYER_DATA.wPosY;
    }
    C3232Map& mapHurt = poBossBattle->GetHurtID();
    for(C3232MapItr itr = mapHurt.begin(); ((itr != mapHurt.end()) && (stNtf.byHurtNum < MAX_BOSSB_SHOW_PLAYER_NUM)); itr++)
    {
        stNtf.astHurtInfo[stNtf.byHurtNum].dwPlayerID = itr->first;
        stNtf.astHurtInfo[stNtf.byHurtNum].dwHurtValue = itr->second;
        stNtf.byHurtNum++;
    }
	if ( VER_PET <= poPlayer->GetCliVer())
	{
		poPlayer->SendMsg((const CHAR*)(&stNtf), BOSSB_INFO_SYNC_NTF2);
	}
	else
	{
		static PKT_BOSSB_INFO_SYNC_NTF stOldNtf;
		memcpy(stOldNtf.adwLeaveInfo, stNtf.adwLeaveInfo, sizeof(stOldNtf.adwLeaveInfo));
		memcpy(stOldNtf.astEnterInfo, stNtf.astEnterInfo, sizeof(stOldNtf.astEnterInfo));
		memcpy(stOldNtf.astHurtInfo, stNtf.astHurtInfo, sizeof(stOldNtf.astHurtInfo));
		memcpy(stOldNtf.astMoveInfo, stNtf.astMoveInfo, sizeof(stOldNtf.astMoveInfo));
		memcpy(stOldNtf.astTopHurtInfo, stNtf.astTopHurtInfo, sizeof(stOldNtf.astTopHurtInfo));
		stOldNtf.byEnterNum = stNtf.byEnterNum;
		stOldNtf.byHurtNum = stNtf.byHurtNum;
		stOldNtf.byLeaveNum = stNtf.byLeaveNum;
		stOldNtf.byMoveNum = stNtf.byMoveNum;
		stOldNtf.dwCurBossHP = (UINT32)stNtf.qwCurBossHP;
		stOldNtf.dwPlayerNum = stNtf.dwPlayerNum;
		stOldNtf.dwTotalBossHP = (UINT32)stNtf.qwTotalBossHP;
		poPlayer->SendMsg((const CHAR*)(&stOldNtf), BOSSB_INFO_SYNC_NTF);
	}
    

    poBossBattle->EndSync();
}


