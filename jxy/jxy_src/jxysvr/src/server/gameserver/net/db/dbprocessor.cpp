#include "dbprocessor.h"
#include <db/autosqlbuf.h>
#include <net/gt/gtpktbuilder.h>
#include <net/db/dbpktbuilder.h>
#include <sdstring.h>
#include <sdutil.h>
#include <commondef.h>
#include <errdef.h>
#include <logic/player/playermgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/server/utility.h>
#include "db/getbattledata2.h"
#include <logic/base/basedefine.h>
#include <logic/player/playermgr.h>
#include <logic/base/basedefine.h>
#include <logic/race/racepropmgr.h>
#include <logic/task/taskmgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/other/logicconfigmgr.h>
#include <logic/record/recordmgr.h>
#include <protocol/server/protogmgs.h>
#include "protocol/server/protogsdb.h"
#include <net/recharge/rcclient.h>
#include <logic/other/jsonopmgr.h>
#include <common/client/gmdef.h>
#include <logic/friendsys/privatemsgmgr.h>
#include <logic/plunder/plunder.h>
#include <logic/jingjie/jingjie.h>
#include <logic/item/itemmgr.h>
#include <logic/hero/hero.h>
#include <logic/vip/vippropmgr.h>
#include <net/cli/usermgr.h>
#include <logic/player/player.h>
#include <logic/instance/instance.h>
#include <logic/player/playersavemgr.h>
#include <db/settongbupayinfo.h>
#include <db/settongbupayprocessed.h>
#include "logic/awaken/awaken.h"
#include "logic/player/playermgr.h"
#include "logic/race/racemgr.h"
#include "net/ks/ksprocessor.h"
#include <framework/gsapi.h>
#include "net/pk/pkpktbuilder.h"
#include "logic/player/playersnapshotmgr.h"
CDBProcessor::CDBProcessor()
{
    Init() ;
}

CDBProcessor::~CDBProcessor()
{

}


CSDProtocol* CDBProcessor::GetProtocol()
{
    return CProtoGSDB::Instance();
}


BOOL CDBProcessor::Init()
{
    return TRUE ;
}



BOOL CDBProcessor::OnPlayerDataAck(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    if(NULL == pstGetPlayerData)
    {
        return FALSE;
    }
    switch(pstGetPlayerData->stGetPlayerDataReq.byGetKind)
    {
    case GET_PLAYERDATATYPE_LOGIN:
        OnPlayerDataAck_Login(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_CHALLENGE:
        OnPlayerDataAck_Challenge(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_PLUNDER:
        OnPlayerDataAck_Plunder(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_SYNCDATA:
        OnPlayerDataAck_SyncData(pstGetPlayerData->stGetPlayerDataAck.wErrCode, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, dwTransID);
        break;
    case GET_PLAYERDATATYPE_DRIVEAWAY:
        OnPlayerDataAck_DriveAway(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_HEROINFO:
        OnPlayerDataAck_HeroInfo(pstGetPlayerData, dwTransID );
        break;
    case GET_PLAYERDATATYPE_GM:
        OnPlayerDataAck_GM(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_EXCHANGE:
        OnPlayerDataAck_Exchange( pstGetPlayerData, dwTransID );
        break;
    case GET_PLAYERDATATYPE_PK:
        OnPlayerDataAck_PK(pstGetPlayerData, dwTransID);
        break;
    case GET_RANDOM_CHALLENGE:
        OnRDChallenge(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_TONGBU:
        OnGetTongbuPayPlayer(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_GMPLAYERINFO:
        OnGmGetPlayerInfo(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_RECVFLOWERLOG:
        OnPlayerDataAck_RecvFlowerLog(pstGetPlayerData, dwTransID );
        break;
    case GET_PLAYERDATATYPE_SENDFLOWER:
        OnPlayerDataAck_SendFlower(pstGetPlayerData, dwTransID );
        break;
    case GET_PLAYERDATATYPE_SEND_PRIVATE_MSG:
        OnPlayerDataAck_SendPrivateMsg(pstGetPlayerData, dwTransID );
        break;
    case KICKED_PLAYER_OUT_FACTION:
        OnKickedPlayerOutFaction(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_RECALC_POWER:
        OnPlayerDataAck_ReCalculatePower(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_ADD_FRIENDS:
        OnPlayerDataAck_AddFriends(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_RETREATE:
        OnPlayerDataAck_Retreate(pstGetPlayerData, dwTransID);
        break;
    case GET_PLAYERDATATYPE_SENDFLOWEREX:
        OnPlayerDataAck_SendFlowerEx(pstGetPlayerData, dwTransID );
        break;
    case GET_PLAYERDATATYPE_ADD_TO_BLACK:
        OnPlayerDataAck_AddToBlack(pstGetPlayerData, dwTransID );
        break;
	case GET_PLAYERDATATYPE_GET_FRIENDS_NUM:
		OnPlayerDataAck_GetFriendsNum(pstGetPlayerData, dwTransID );
		break;
    case GET_PLAYERDATATYPE_ADD_HERO:
        OnPlayerDataAck_AddHero(pstGetPlayerData, dwTransID );
        break;
    case GET_PLAYERDATATYPE_REMOTE_PK:
        OnPlayerDataAck_RemotePK(pstGetPlayerData, dwTransID);
        break;
	case GET_PLAYER_INFO_AWAKEN:
		OnPlayerInfoAwaken(pstGetPlayerData, dwTransID );
		break;
	case GET_PLAYER_INFO_GODWEAPON:
		OnPlayerInfoGodWeapon(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_INFO_SKILL:
		OnPlayerInfoSkill(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_INFO_JEWELRY:
		OnPlayerInfoJewelry(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_INFO_PANDECT:
		OnPlayerInfoPandect(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_INFO_EQUIP:
		OnPlayerInfoEquip(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_FORMATION_TIP:
		OnPlayerFormationTip(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_SNAPSHOT:
		OnPlayerSnapshot(pstGetPlayerData);
		break;
	case GET_PLAYER_SOUL:
		OnPlayerSoul(pstGetPlayerData, dwTransID);
		break;
	case GET_PLAYER_PET:
		OnPlayerPet(pstGetPlayerData, dwTransID);
		break;
    default:
        CGetPlayerDataMgr::Instance()->OnGetPlayerData(pstGetPlayerData);//清除
        SYS_CRITICAL(_SDT("[%s:%d]: CDBProcessor::OnPlayerDataAck get player data failed, playerid = %u, getkind= %u"), MSG_MARK, 
            pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID, pstGetPlayerData->stGetPlayerDataReq.byGetKind);
        return FALSE;
    }

    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        gsapi::GetDBMgr()->SetStartData(pstGetPlayerData->stGetPlayerDataReq.dwPlayerID, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData);
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s:%d]: CDBProcessor::OnPlayerDataAck get player data failed, byGetKind = %u, playerid = %u, errcode= %u"), MSG_MARK, 
            pstGetPlayerData->stGetPlayerDataReq.byGetKind, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
    }

    CGetPlayerDataMgr::Instance()->OnGetPlayerData(pstGetPlayerData);//清除
    return TRUE;
}

BOOL CDBProcessor::OnPlayerDataAck2(SGetPlayerData2* pstGetPlayerData, UINT32 dwTransID)
{
    if(NULL == pstGetPlayerData)
    {
        return FALSE;
    }
    switch(pstGetPlayerData->byGetKind)
    {
    case GET_PLAYERDATATYPE_PLUNDER:
        OnPlayerDataAck_Plunder2(pstGetPlayerData, dwTransID);
        break;
    default:
        break;
    }
    if(SUCCESS == pstGetPlayerData->wErrCode)
    {
        gsapi::GetDBMgr()->SetStartData(pstGetPlayerData->dwPlayerID1, pstGetPlayerData->stDT_PLAYER_DATA1.stPlayerNoPvpData);
        gsapi::GetDBMgr()->SetStartData(pstGetPlayerData->dwPlayerID2, pstGetPlayerData->stDT_PLAYER_DATA2.stPlayerNoPvpData);
    }

    CGetPlayerDataMgr::Instance()->OnGetPlayerData2(pstGetPlayerData);//清除
    return TRUE;
}


BOOL CDBProcessor::OnCreatePlayerAck(DT_GSDB_CREATE_PALYER_DATA_ACK& stAck, UINT32 dwTransID)
{
    //GM
    if(1 == stAck.byGmFlag)
    {
        return OnCreatePlayerAck_GM(stAck, dwTransID);
    }

    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }


    if(SUCCESS == stAck.wErrCode)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_LOGIN, poUser->GetPlayerID());
        //客户端进入了获取用户信息的状态
        poUser->SetUserStat(CLIS_GET_USER_DATA_ING);

        return TRUE;
    }
    else
    {
        //下发错误应答信息
        UINT16 wErrCode = 0;
        SET_OTHER_ERR(wErrCode);
        UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(wErrCode, poUser->GetPlayer());
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

        //释放User
        CUserMgr::Instance()->ReleaseUser(poUser);
        poUser = NULL;
    }

    if(SUCCESS == stAck.wErrCode)
    {
        CPlayer* poPlayer = poUser->GetPlayer();
        if(poPlayer)
        {
            poPlayer->GetNewPlayerFlag() = FALSE;
        }
    }

    return TRUE;
}

BOOL CDBProcessor::OnCreatePlayerAck_GM(DT_GSDB_CREATE_PALYER_DATA_ACK& stAck, UINT32 dwTransID)
{
    if(SUCCESS == stAck.wErrCode)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_SYNCDATA, dwTransID);

        return TRUE;
    }

    return FALSE;
}



BOOL CDBProcessor::OnSavePlayerDataAck(CPlayerSaveData* poPlayerSaveData, UINT32 dwTransID)
{
    if(NULL == poPlayerSaveData)
    {
        return FALSE;
    }

    CPlayerSaveMgr::Instance()->OnSavePlayerDataAck(poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID, poPlayerSaveData);

    return TRUE;
}


BOOL CDBProcessor::OnSavePlayersPlunderDataAck(CPlayersPlunderSaveData* poPlayersPlunderSaveData, UINT32 dwTransID)
{
    if(NULL == poPlayersPlunderSaveData)
    {
        return FALSE;
    }

    CPlayerSaveMgr::Instance()->OnSavePlayerDataAck(poPlayersPlunderSaveData);

    return TRUE;
}

BOOL CDBProcessor::OnSavePlayersRaceDataAck(CPlayersRaceSaveData* poPlayersRaceSaveData, UINT32 dwTransID)
{
    if(NULL == poPlayersRaceSaveData)
    {
        return FALSE;
    }

    CPlayerSaveMgr::Instance()->OnSavePlayerDataAck(poPlayersRaceSaveData);

    return TRUE;
}

BOOL  CDBProcessor::OnBattleDataAck(UINT32 dwPlayerID, const DT_BATTLE_DATA5 &battleData, BOOL bHaveData)
{
    CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( dwPlayerID );
    if( NULL == pPlayer )
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->BattleDataAck5( pPlayer, battleData, bHaveData );
    if( wMsgID == INVALID_MSGID )
    {
        return FALSE;
    }
    return pPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID );
}

BOOL CDBProcessor::OnPlayerDataAck_Login(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    //CAutoCycle oAutoCycle("CDBProcessor::OnPlayerDataAck_Login");

    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }

    if (CLIS_GET_USER_DATA_ING != poUser->GetUserStat())
    {
        return FALSE;
    }

    //修改状态
    poUser->SetUserStat(CLIS_GET_USER_DATA_COMPLETE);

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 dwPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        //因为是在登录逻辑，即使重复登录也没有关系，所以不必在此消除数据
        BOOL bAlreadyLogin = FALSE;

        CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, poUser, bAlreadyLogin);
        if(poPlayer)
        {
            poPlayer->OnLogin();//上线
            poPlayer->ResetLoginTime(); //重置上线时间
            poPlayer->RecordPlayerOnOffLine( TRUE );
            poPlayer->SetOriZoneID(poUser->GetOriginalZoneID());

            CPlayerBaseDataCacheMgr::Instance()->ReSetLoginTime(dwTransID);//记录绑在最后登录时间
            const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
            //新增玩家，需加入基本信息缓存中
            if(NULL == pstPlayerBaseData)
            {
                CPlayerBaseDataCacheMgr::Instance()->AddPlayerBaseData(poPlayer);
            }


            //下发信息
            UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(SUCCESS, poPlayer);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            poUser->SetUserStat(CLIS_IN_GAME);

            //玩家已经正常处理成功，通知玩家
            if (1 == poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag)
            {
                PKT_CLIGS_EXCHANGE_RESULT stAck;
                stAck.wErrCode = ERR_EXCHANGE_RESULT_ID::ID_SUCCESS;
                stAck.qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
                stAck.dwExchangeGold = poPlayer->GetDT_EXCHANGE_RESULT_LOG().dwExchangeGold;
                stAck.byVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
                stAck.dwTotalExchangeGold = poPlayer->GetDT_PLAYER_BASE_DATA().dwTotalAmount;
                poPlayer->SendMsg( (const CHAR*)(&stAck), CLIGS_EXCHANGE_RESULT );
                poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag = 0;
            }
        }
        else
        {
            UINT16 wErrCode = 0;
            SET_OTHER_ERR(wErrCode);
            UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(wErrCode, poPlayer);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
        }
    }
    //没有玩家，则创建
    else if(GET_NODATA == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        if(!CPlayerMgr::Instance()->CreateNewPlayer(dwTransID, poUser))
        {
            UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(ERR_ENTER_GS::ID_INVALID_CAREER);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
        }
    }
    else if( REFUSE_LOGIN == pstGetPlayerData->stGetPlayerDataAck.wErrCode )
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(ERR_ENTER_GS::ID_SAVE_DATA_ERROR, NULL);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
    }
    //其他错误
    else
    {
        UINT16 wErrCode = 0;
        SET_OTHER_ERR(wErrCode);
        UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(wErrCode, NULL);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
    }

    return TRUE;
}


BOOL CDBProcessor::OnPlayerDataAck_Challenge(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        return FALSE;
    }

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 wPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin );
        if(poPlayer)
        {
            if( !bAlreadyLogin )
            {
                poPlayer->OnLogin(FALSE);//非用户登录上线状态
            }
            //挑战
            UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(0, poMyself, poPlayer);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
        }
        else
        {
            UINT16 wErrCode = 0;
            SET_OTHER_ERR(wErrCode);
            UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(wErrCode, poMyself, NULL);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
        }
    }
    else
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(ERR_CHALLENGE::ID_ENEMY_NOT_EXIST_ERR, poMyself, NULL);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
    }

    return TRUE;
}

BOOL CDBProcessor::OnPlayerDataAck_Plunder(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        return FALSE;
    }

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 wPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;
    //有玩家数据，创建玩家对象
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        CPlayer* poPlayer = InitPlayer( stDT_PLAYER_DATA, NULL, bAlreadyLogin );
        if(NULL == poPlayer)
        {
            UINT16 wErrCode = 0;
            SET_OTHER_ERR(wErrCode);
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(wErrCode, poMyself);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }

        if( !bAlreadyLogin )
        {
            poPlayer->OnLogin(FALSE);//非用户登录上线状态
        }

        CPlunder* poPlunder = poMyself->GetPlunder();
        if(NULL == poPlunder)
        {
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_UNLOCK, poMyself);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }

        ////又重新请求，丢弃之前的请求
        PKT_CLIGS_PLUNDER_REQ& stReq = poPlunder->GetReqParam(poMyself->GetID());

        CPlayer* poEnemy = CPlayerMgr::Instance()->FindPlayer(stReq.dwEnemyID);
        CPlayer* poCapture = CPlayerMgr::Instance()->FindPlayer(stReq.dwCaptureID);
        if((NULL == poEnemy) || (NULL == poCapture))
        {
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_ENEMY_NOT_EXIST, poMyself);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }

        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(0, poMyself, poEnemy, poCapture);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }
    else
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_ENEMY_NOT_EXIST, poMyself);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }

    return TRUE;
}


BOOL CDBProcessor::OnPlayerDataAck_Plunder2(SGetPlayerData2* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        return FALSE;
    }

    DT_PLAYER_DATA& stDT_PLAYER_DATA1 = pstGetPlayerData->stDT_PLAYER_DATA1;
    DT_PLAYER_DATA& stDT_PLAYER_DATA2 = pstGetPlayerData->stDT_PLAYER_DATA2;
    //有玩家数据，创建玩家对象
    if(SUCCESS == pstGetPlayerData->wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        CPlayer* poEnemy = InitPlayer(stDT_PLAYER_DATA1, NULL, bAlreadyLogin);
        if(NULL == poEnemy)
        {
            UINT16 wErrCode = 0;
            SET_OTHER_ERR(wErrCode);
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(wErrCode, poMyself);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }

        if( !bAlreadyLogin )
        {
            poEnemy->OnLogin(FALSE);//非用户登录上线状态
        }

        bAlreadyLogin = FALSE;
        CPlayer* poCapture = InitPlayer(stDT_PLAYER_DATA2, NULL, bAlreadyLogin);
        if(NULL == poCapture)
        {
            UINT16 wErrCode = 0;
            SET_OTHER_ERR(wErrCode);
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(wErrCode, poMyself);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }

        if( !bAlreadyLogin )
        {
            poCapture->OnLogin(FALSE);//非用户登录上线状态
        }

        CPlunder* poPlunder = poMyself->GetPlunder();
        if(NULL == poPlunder)
        {
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_UNLOCK, poMyself);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }
        ////又重新请求，丢弃之前的请求
        PKT_CLIGS_PLUNDER_REQ& stReq = poPlunder->GetReqParam(poMyself->GetID());
        //if((pstGetPlayerData->dwPlayerID1 != stReq.dwEnemyID) || (pstGetPlayerData->dwPlayerID2 != stReq.dwCaptureID) ||
        //        (pstGetPlayerData->dwParam1 != stReq.byResistFlag))
        //{
        //    return TRUE;
        //}

        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(0, poMyself, poEnemy, poCapture);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }
    else
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_ENEMY_NOT_EXIST, poMyself);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }

    return TRUE;
}


BOOL CDBProcessor::OnPlayerDataAck_SyncData(INT16 wErrCode, DT_PLAYER_DATA& stDT_PLAYER_DATA, UINT32 dwTransID)
{
    UINT32 dwPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    if(SUCCESS == wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
        if(poPlayer)
        {
            if( !bAlreadyLogin )
            {
                poPlayer->OnLogin(FALSE);//非用户登录上线状态
            }

            //User存在，则在线
            if(CUserMgr::Instance()->FindUser(dwPlayerID))
            {
                poPlayer->OnLogin(TRUE);
            }

            const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
            //新增玩家（GM），需加入基本信息缓存中
            if(NULL == pstPlayerBaseData)
            {
                CPlayerBaseDataCacheMgr::Instance()->AddPlayerBaseData(poPlayer);
            }

            //fortest convert tb
            //CDBPktBuilder::Instance()->CreatePlayerReq(dwTransID, dwPlayerID, "", "", "", 0, CPlayerSaveMgr::Instance()->GetPlayerData(poPlayer));
        }
    }
    return TRUE;
}


CPlayer* CDBProcessor::InitPlayer(DT_PLAYER_DATA& stPlayerData, CUser* poUser, BOOL &bAleadyLogin)
{
    bAleadyLogin = FALSE;
    UINT32 wPlayerID = stPlayerData.stPlayerNoPvpData.stBaseData.dwID;

    //判断内存中是否存在玩家信息，有可能在异步过程中被挑战获取信息并初始化了
    //若存在则以当前内存为主
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(wPlayerID);
    if(NULL == poPlayer)
    {
        poPlayer = CPlayerMgr::Instance()->CreatePlayer(wPlayerID);
        if(NULL == poPlayer)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreatePlayer failed!"), MSG_MARK);
            return NULL;
        }
        //设置玩家对象
        if(!poPlayer->Init(stPlayerData))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Player Init failed!PlayerID[%d]."), MSG_MARK, wPlayerID);
            CPlayerMgr::Instance()->RemovePlayer(wPlayerID);
            return NULL;
        }
        bAleadyLogin = FALSE;
    }
    else
    {
        bAleadyLogin = TRUE;
    }

    return poPlayer;
}


BOOL CDBProcessor::OnPlayerDataAck_DriveAway(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        return FALSE;
    }

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 wPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;
    //有玩家数据，创建玩家对象
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
        if(NULL == poPlayer)
        {
            UINT16 wErrCode = 0;
            SET_OTHER_ERR(wErrCode);
            UINT16 wMsgID = CGTPktBuilder::Instance()->OnDriveAwayAck(poMyself, wErrCode);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }

        if( !bAlreadyLogin )
        {
            poPlayer->OnLogin(FALSE);//非用户登录上线状态
        }

        UINT16 wMsgID = CGTPktBuilder::Instance()->OnDriveAwayAck(poMyself);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }
    else
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->OnDriveAwayAck(poMyself, ERR_DRIVE_AWAY_OWNER::ID_OWNER_NOT_EXIST);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }

    return TRUE;
}

BOOL CDBProcessor::OnPlayerDataAck_HeroInfo(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* pTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == pTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        pTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->GetHeroInfoAck( pTargetPlayer, UINT8( pstGetPlayerData->stGetPlayerDataReq.dwParam1 ), poMyself == pTargetPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: pack error！"), MSG_MARK );
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}


BOOL CDBProcessor::OnPlayerDataAck_GM(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    SGetPlayerDataReq	&	stGetPlayerDataReq	= pstGetPlayerData->stGetPlayerDataReq;
    DT_PLAYER_DATA&		stDT_PLAYER_DATA		= pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32				dwPlayerID			= stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    PKT_GMGS_PARM_INFO_ACK stAck;
    memset(&stAck, 0x00, sizeof(PKT_GMGS_PARM_INFO_ACK));
    stAck.wErrCode = ERR_GM_PARM_INFO::ID_SUCCESS;
    stAck.dwPlayerID = dwPlayerID;
    stAck.dwSerialNumber = stGetPlayerDataReq.dwSerialNumber;

    //有玩家数据，创建玩家对象
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
        if(poPlayer)
        {
            stAck.wErrCode = CGMProcessor::GmReq(poPlayer, stGetPlayerDataReq.dwSerialNumber, stGetPlayerDataReq.byAction, stGetPlayerDataReq.dwPlayerID, stGetPlayerDataReq.dwParam1, stGetPlayerDataReq.qwParam2, stGetPlayerDataReq.qwParam3, stGetPlayerDataReq.szParam, stGetPlayerDataReq.dwTransID, stGetPlayerDataReq.szDesc);
        }
        else
        {
            stAck.wErrCode = ERR_GM_PARM_INFO::ID_PLAYER_INIT_ERR;
        }
    }
    else if(GET_NODATA == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        stAck.wErrCode = ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR;//没有玩家
    }
    else
    {
        stAck.wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;////其他错误
    }

    if(0 != stGetPlayerDataReq.dwTransID)
    {
        CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(stGetPlayerDataReq.dwTransID);
        if(!poClinet)
        {
            return FALSE;
        }
        CHAR szRet[128] = {0};
        if(ERR_GM_PARM_INFO::ID_SUCCESS == stAck.wErrCode)
        {
            sprintf(szRet, "{\"status\":\"success\"}");
        }
        else
        {
            sprintf(szRet, "{\"status\":\"error_%d\"}", stAck.wErrCode);
        }
        poClinet->Rspone(szRet);
    }
    else
    {
        gsapi::GMMsgResponse(GMGS_PARM_INFO_ACK, stGetPlayerDataReq.dwPlayerID, (CHAR*)&stAck);
    }
    return TRUE;
}


BOOL CDBProcessor::OnPlayerDataAck_PK(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        return FALSE;
    }

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 wPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    CPlayer* poEnemyPlayer = NULL;
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        poEnemyPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnPKBattleReq(poMyself, poEnemyPlayer);
    if(INVALID_MSGID != wMsgID)
    {
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    }

    return TRUE;
}

BOOL CDBProcessor::OnRDChallenge(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed"), MSG_MARK);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        return FALSE;
    }

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 wPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    CPlayer* poEnemyPlayer = NULL;
    if(SUCCESS == pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        BOOL bAlreadyLogin = FALSE;
        poEnemyPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRdchallengeReq(poMyself, poEnemyPlayer);
    if(INVALID_MSGID != wMsgID)
    {
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    }

    return TRUE;
}



BOOL CDBProcessor::OnPlayerDataAck_Exchange( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID )
{
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: get data error, errcode:%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    UINT32 dwPlayerID = pstGetPlayerData->stGetPlayerDataReq.dwPlayerID;

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;

    BOOL bAlreadyLogin = FALSE;
    CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin );
    if(!poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: InitPlayer failed, PlayerID:%u"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if( !bAlreadyLogin )
    {
        poPlayer->OnLogin(FALSE);//非用户登录上线状态
    }

    //参数2表示元宝数
    poPlayer->AddGold( (UINT32)(pstGetPlayerData->stGetPlayerDataReq.qwParam2), CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, pstGetPlayerData->stGetPlayerDataReq.szOrderID, UINT32(pstGetPlayerData->stGetPlayerDataReq.qwParam3));
    _SDTStrcpy( poPlayer->GetLastOrderID(), pstGetPlayerData->stGetPlayerDataReq.szOrderID);

    //检测一下是苹果服务器的充值，还是网页订单的充值
    if (0 == pstGetPlayerData->stGetPlayerDataReq.dwTransID )
    {
        const DT_PLAYER_BASE_DATA &stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
        //通知已经产生结果了
        UINT32 dwMsgID = CGTPktBuilder::Instance()->OnAppPaySuccess(stDT_PLAYER_BASE_DATA.byVipLevel, stDT_PLAYER_BASE_DATA.dwTotalAmount, stDT_PLAYER_BASE_DATA.qwGold, (UINT32)pstGetPlayerData->stGetPlayerDataReq.qwParam2, pstGetPlayerData->stGetPlayerDataReq.szOrderID, dwPlayerID);
        if( dwMsgID == INVALID_MSGID )
        {
            return FALSE;
        }
        //通知玩家
        CPlayer *pNotifyPlayer = CPlayerMgr::Instance()->FindPlayer( pstGetPlayerData->stGetPlayerDataReq.dwParam1 );
        if( NULL == pNotifyPlayer )
        {
            SYS_CRITICAL(_SDT("[%s:%d]: get NotifyPlayer error, PlayerID:%u"), MSG_MARK, pstGetPlayerData->stGetPlayerDataReq.dwParam1);
            return FALSE;
        }
        pNotifyPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), dwMsgID );
    }
    else
    {
        //通知玩家
        CPlayer *pNotifyPlayer = CPlayerMgr::Instance()->FindPlayer( pstGetPlayerData->stGetPlayerDataReq.dwParam1 );

        //给网页发送推送信息
        CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(pstGetPlayerData->stGetPlayerDataReq.dwTransID);
        if(poClinet)
        {
            poClinet->SendPayRet(pstGetPlayerData->stGetPlayerDataReq.szOrderID, 0, pNotifyPlayer, (UINT32)(pstGetPlayerData->stGetPlayerDataReq.qwParam2));
            return TRUE;
        }
    }

    return TRUE;
}

BOOL CDBProcessor::OnPlayerDataAck_Retreate( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID )
{
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: get data error, errcode:%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    UINT32 dwPlayerID = pstGetPlayerData->stGetPlayerDataReq.dwPlayerID;

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;

    BOOL bAlreadyLogin = FALSE;
    CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin );
    if(!poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: InitPlayer failed, PlayerID:%u"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if( !bAlreadyLogin )
    {
        poPlayer->OnLogin(FALSE);//非用户登录上线状态
    }

    //参数2表示元宝数
    poPlayer->AddGold( (UINT32)(pstGetPlayerData->stGetPlayerDataReq.qwParam2), CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, pstGetPlayerData->stGetPlayerDataReq.szOrderID, UINT32(pstGetPlayerData->stGetPlayerDataReq.qwParam3));
    _SDTStrcpy( poPlayer->GetLastOrderID(), pstGetPlayerData->stGetPlayerDataReq.szOrderID);

    return TRUE;
}

BOOL CDBProcessor::OnGetTongbuPayPlayer( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID )
{
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnGetTongbuPayPlayer Get Player[%d] failed[%d]"), MSG_MARK,
                     pstGetPlayerData->stGetPlayerDataReq.dwPlayerID, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    UINT32 dwPlayerID = pstGetPlayerData->stGetPlayerDataReq.dwPlayerID;

    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;

    BOOL bAlreadyLogin = FALSE;
    CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin );
    if(!poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnGetTongbuPayPlayer InitPlayer failed playerid:%d"), MSG_MARK, dwPlayerID);
        CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(pstGetPlayerData->stGetPlayerDataReq.dwParam1);
        if(NULL == poClinet)
        {
            SYS_CRITICAL(_SDT("[%s:%d]: OnProcessedTongBuPayInfo find CRCClient[%d] OrderID[%s] failed!"), MSG_MARK, pstGetPlayerData->stGetPlayerDataReq.dwParam1, pstGetPlayerData->stGetPlayerDataReq.szOrderID);
            return FALSE;
        }
        poClinet->SendPayRet(pstGetPlayerData->stGetPlayerDataReq.szOrderID, 3, poPlayer, (UINT32)(pstGetPlayerData->stGetPlayerDataReq.qwParam2));
        return FALSE;
    }
    //已经处理的跳过
    if(0 == SDStrcmp(poPlayer->GetLastOrderID(), pstGetPlayerData->stGetPlayerDataReq.szOrderID))
    {
        return TRUE;
    }

    if( !bAlreadyLogin )
    {
        poPlayer->OnLogin(FALSE);//非用户登录上线状态
    }

    //参数2表示元宝数
    //参数3表示金额数
    poPlayer->AddGold( (UINT32)(pstGetPlayerData->stGetPlayerDataReq.qwParam2), CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, pstGetPlayerData->stGetPlayerDataReq.szOrderID, UINT32(pstGetPlayerData->stGetPlayerDataReq.qwParam3));
    _SDTStrcpy( poPlayer->GetLastOrderID(), pstGetPlayerData->stGetPlayerDataReq.szOrderID);


    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(pstGetPlayerData->stGetPlayerDataReq.dwParam1);
    if(NULL == poClinet)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnProcessedTongBuPayInfo find CRCClient[%d] OrderID[%s] failed!"), MSG_MARK, pstGetPlayerData->stGetPlayerDataReq.dwParam1, pstGetPlayerData->stGetPlayerDataReq.szOrderID);
        return FALSE;
    }
    poClinet->SendPayRet(pstGetPlayerData->stGetPlayerDataReq.szOrderID, 0, poPlayer, (UINT32)(pstGetPlayerData->stGetPlayerDataReq.qwParam2));

    return TRUE;
}

BOOL CDBProcessor::OnGmGetPlayerInfo( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID )
{
    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(dwTransID);
    if(NULL == poClinet)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnGmGetPlayerInfo find CRCClient[%d] failed!"), MSG_MARK, dwTransID);
        return FALSE;
    }

    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnGmGetPlayerInfo Get Player[%d] failed[%d]"), MSG_MARK,
                     pstGetPlayerData->stGetPlayerDataReq.dwPlayerID, pstGetPlayerData->stGetPlayerDataAck.wErrCode);

        poClinet->Rspone("{\"status\":\"error_12\"}");
        return FALSE;
    }

    CJsonOpMgr::Instance()->Instance()->AddJsonParsePlayerData(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, dwTransID);

    return TRUE;
}


VOID CDBProcessor::PassAllCommonInstance(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    CTask* poMainTask = poPlayer->GetTaskByLine(MAIN_LINE_TASK);
    if(NULL == poMainTask)
    {
        return;
    }
    CTaskLineProp* pstMainLineTaskProp  = CTaskPropMgr::Instance()->GetTaskLineProp(MAIN_LINE_TASK);
    if((NULL == pstMainLineTaskProp) || (0 == pstMainLineTaskProp->mapTaskProp.size()))
    {
        return;
    }
    STaskProp* pstMainTaskProp = pstMainLineTaskProp->mapTaskProp.rbegin()->second;
    UINT32 dwMaxIdx = pstMainTaskProp->dwTaskIdx;
    switch(poMainTask->GetDT_TASK_DATA().byTaskState)
    {
    case ETS_GIVEN:
    {
        poPlayer->GetGiveTask().DelData(MAIN_LINE_TASK);
    }
    break;
    case ETS_ACCEPTED:
    {
        poPlayer->GetAcceptTask().DelData(MAIN_LINE_TASK);
    }
    break;
    case ETS_ENCOURAGE:
    {
        poPlayer->GetEncourageTask().DelData(MAIN_LINE_TASK);
    }
    break;
    case ETS_FINISHED:
    {
        poPlayer->GetFinishTask().DelData(MAIN_LINE_TASK);
    }
    break;
    default:
        break;
    }
    DT_TASK_DATA& stDT_TASK_DATA = poMainTask->GetDT_TASK_DATA();
    stDT_TASK_DATA.dwTaskIdx = dwMaxIdx;
    stDT_TASK_DATA.byTaskState = ETS_ACCEPTED;
    stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
    poPlayer->SetLastMainLineTaskProp(pstMainTaskProp->dwTaskIdx);

    if(!CTaskMgr::Instance()->SetTaskCond(poPlayer, stDT_TASK_DATA.byTaskLine, stDT_TASK_DATA.dwTaskIdx, stDT_TASK_DATA.byTaskType, stDT_TASK_DATA.stTaskConditionInfo))
    {
        USR_INFO(_SDT("[%s: %d]: SetTaskCond failed!"), MSG_MARK);
        return;
    }
    //重新初始化任务信息
    if(FALSE == poMainTask->Init(&stDT_TASK_DATA, poPlayer))
    {
        USR_INFO(_SDT("[%s: %d]: task init failed!"), MSG_MARK);
        return;
    }
    poPlayer->RecordChangeTask(poMainTask);
    DT_INSTANCE_DATA_LIST stInstanceLst;
    memset(&stInstanceLst, 0, sizeof(stInstanceLst));
    stInstanceLst.wSceneIdx = EIT_COMMON;
    DT_INSTANCE_ID stMaxInstanceID = pstMainTaskProp->stInstanceID;
    for(UINT8 byTownIdx = 1; ((byTownIdx <= stMaxInstanceID.byTownIdx) && (stInstanceLst.wInstanceNum < MAX_INSTANCE_PER_SCENE_NUM)); byTownIdx++)
    {
        for(UINT8 byInstanceIdx = 1; byInstanceIdx <= 3; byInstanceIdx++)
        {
            DT_INSTANCE_DATA& stDT_INSTANCE_DATA = stInstanceLst.astInstanceInfo[stInstanceLst.wInstanceNum++];
            //stDT_INSTANCE_DATA.byScore = 5;
            stDT_INSTANCE_DATA.qwLastPassTime = SDTimeSecs();
            //stDT_INSTANCE_DATA.wPassTimes = 1;
            DT_INSTANCE_ID& stInstanceID = stDT_INSTANCE_DATA.stInstanceID;
            stInstanceID.wSceneIdx = EIT_COMMON;
            stInstanceID.byTownIdx = byTownIdx;
            stInstanceID.byInstanceIdx = byInstanceIdx;
        }
    }

    poPlayer->GetInstance().AddSceneInstanceRecord(EIT_COMMON, stInstanceLst);
    poPlayer->CkOpenFunc();
}

BOOL CDBProcessor::OnSaveBossBDataAck(BOOL bSuccess, UINT32 dwTransID)
{

    return TRUE;
}

BOOL CDBProcessor::OnSaveTongBuPayInfo(STongBuPayInfo& stPayInfo)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(stPayInfo.dwPlayerID);
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnSaveTongBuPayInfo find player[%d] failed!"), MSG_MARK, stPayInfo.dwPlayerID);
    }
    UINT16 wErrCode = ERR_GET_ORDER_ID::ID_SUCCESS;
    if(!stPayInfo.bSuccess)
    {
        wErrCode = ERR_GET_ORDER_ID::ID_ERR_PARSE_DATA_FROM_EXCHANGE_SERVER;
    }
    UINT16 wMsgID = CGTPktBuilder::Instance()->GenGUIDAck2GT(stPayInfo.strOrderID, wErrCode);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID );

    return TRUE;
}

BOOL CDBProcessor::OnProcessedTongBuPayInfo(STongBuPayProcessed& stPayInfo)
{
    //处理成功
    CPlayer* poPlayer = NULL;
    if(0 == stPayInfo.wErrCode)
    {
        poPlayer = CPlayerMgr::Instance()->FindPlayer( stPayInfo.dwPlayerID );
        if( NULL == poPlayer )
        {
            //如果玩家不在线,则调出玩家,以设置数据
            CGetPlayerDataMgr::Instance()->GetPlayerData( 0 , GET_PLAYERDATATYPE_TONGBU, stPayInfo.dwPlayerID,
                    stPayInfo.dwSessionID, stPayInfo.dwGold, stPayInfo.dwRMB, (char *)stPayInfo.strOrderID.c_str() );
            return TRUE;
        }
        //已经处理的跳过
        if(0 == SDStrcmp(poPlayer->GetLastOrderID(), stPayInfo.strOrderID.c_str()))
        {
            return TRUE;
        }
        //玩家在线，则增加元宝
        poPlayer->AddGold( stPayInfo.dwGold, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, stPayInfo.strOrderID, stPayInfo.dwGold);
        _SDTStrcpy( poPlayer->GetLastOrderID(), stPayInfo.strOrderID.c_str() );
    }

    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(stPayInfo.dwSessionID);
    if(NULL == poClinet)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnProcessedTongBuPayInfo find CRCClient[%d] OrderID[%s] failed!"), MSG_MARK, stPayInfo.dwSessionID, stPayInfo.strOrderID.c_str());
        return FALSE;
    }
    poClinet->SendPayRet(stPayInfo.strOrderID, stPayInfo.wErrCode, poPlayer, stPayInfo.dwGold);
    return TRUE;
}

BOOL  CDBProcessor::OnPlayerDataAck_RecvFlowerLog(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poMyself = poUser->GetPlayer();
    if(NULL == poMyself)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetFlowerByPlayerIDAck(poTargetPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: pack error！"), MSG_MARK );
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

BOOL  CDBProcessor::OnPlayerDataAck_SendFlower(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSendFlowerAck(poPlayer, poTargetPlayer, pstGetPlayerData->stGetPlayerDataReq.dwParam1);
    if(INVALID_MSGID == wMsgID)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: pack error！"), MSG_MARK );
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}


BOOL  CDBProcessor::OnPlayerDataAck_SendFlowerEx(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSendFlowerExAck(poPlayer, poTargetPlayer, pstGetPlayerData->stGetPlayerDataReq.dwParam1);
    if(INVALID_MSGID == wMsgID)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: pack error！"), MSG_MARK );
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

BOOL  CDBProcessor::OnPlayerDataAck_SendPrivateMsg(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    //检测玩家的私聊信息是否在内存中
    if(!CPrivateMsgMgr::Instance()->CkPrivateMsg(poPlayer->GetID(), poTargetPlayer->GetID()))
    {
        CDBPktBuilder::Instance()->GetPrivateMsg(poPlayer->GetID(), poTargetPlayer->GetID(), em_Msg, pstGetPlayerData->stGetPlayerDataReq.szMsg);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSendPrivateMsgAck(poPlayer, poTargetPlayer, (CHAR*)pstGetPlayerData->stGetPlayerDataReq.szMsg);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

BOOL CDBProcessor::OnKickedPlayerOutFaction(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnKickedFactionReq(poPlayer, poTargetPlayer->GetID());
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return  TRUE;
}


// 创建帮派
BOOL CDBProcessor::OnCreateFactionReq( UINT32 dwPlayerID, UINT16 byCreateFactionType, TCHAR aszFactionName[MAX_FACTION_NUM], TCHAR aszFactionDescName[MAX_FACTION_DESC_NUM], UINT32 dwFactionID, UINT16 wErrCode)
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if (NULL == poPlayer)
    {
        //对于玩家这么快的就在内存中消失，可能存在问题，直接废弃掉这种没用的数据。
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCreateFactionReq(poPlayer, byCreateFactionType, aszFactionName, aszFactionDescName, dwFactionID, wErrCode);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), CLIGS_CREATE_FACTION_ACK);

    return TRUE;
}

BOOL CDBProcessor::OnGetPrivateMsg(GET_PRIVATEMSGLOG stInfo, SPrivateMsgList listPrivateMsg, UINT16 wErrCode)
{
    if(SUCCESS != wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, wErrCode);
        return FALSE;
    }

    CPrivateMsgMgr::Instance()->SetPrivateMsgVec(stInfo.dwSenderID, stInfo.dwRecverID, listPrivateMsg);
    CPlayer* poPlayer  = CPlayerMgr::Instance()->FindPlayer(stInfo.dwSenderID);
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnGetPrivateMsg find player[%d] failed!"), MSG_MARK, stInfo.dwSenderID);
        return FALSE;
    }
    
    UINT16 wMsgID = INVALID_MSGID;
    switch(stInfo.byType)
    {
    case em_Msg:
    {
        CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(stInfo.dwRecverID);
        if(NULL == poTargetPlayer)
        {
            SYS_CRITICAL(_SDT("[%s:%d]: OnGetPrivateMsg find player[%d] failed!"), MSG_MARK, stInfo.dwRecverID);
            return FALSE;
        }
        wMsgID = CGTPktBuilder::Instance()->OnSendPrivateMsgAck(poPlayer, poTargetPlayer, (CHAR*)stInfo.strMsgContent.c_str());
    }
    break;
    case em_Log:
        wMsgID = CGTPktBuilder::Instance()->OnGetPrivateMsgLogListAck(poPlayer, stInfo.dwRecverID, stInfo.dwStartIdx, stInfo.byGetMsgNum);
        break;
    }

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    return poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

VOID CDBProcessor::OnPlayerDataAck_ReCalculatePower(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 dwPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        return;
    }

    BOOL bAlreadyLogin = FALSE;
    CPlayer* poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poPlayer || TRUE == bAlreadyLogin)
    {
        return;
    }

    if(poPlayer->GetUser())
    {
        return;
    }

    poPlayer->OnLogin(FALSE);//非用户登录上线状态
    CRaceMgr::Instance()->OnLoadOfflineRacerAtStartUp(poPlayer);
}


BOOL  CDBProcessor::OnPlayerDataAck_AddFriends(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (NULL == poUser)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
        return FALSE;
    }
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAddFriendAck(poPlayer, poTargetPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: pack error！"), MSG_MARK );
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

BOOL CDBProcessor::OnPlayerDataAck_RemotePK(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    DT_PLAYER_DATA& stDT_PLAYER_DATA = pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA;
    UINT32 dwPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;

    //有玩家数据，创建玩家对象
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        return FALSE;
    }

    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(poPlayer)
    {
        CPKPktBuilder::Instance()->SyncPlayer(dwTransID, poPlayer);
        return TRUE;
    }
    else
    {
        BOOL bAlreadyLogin = FALSE;
        poPlayer = InitPlayer(stDT_PLAYER_DATA, NULL, bAlreadyLogin);
        if(NULL == poPlayer)
        {
            return FALSE;
        }

        if(!bAlreadyLogin)
        {
            poPlayer->OnLogin(FALSE);//非用户登录上线状态
        }

        CPKPktBuilder::Instance()->SyncPlayer(dwTransID, poPlayer);
        // CPKPktBuilder::Instance()->SyncPlayerData(dwTransID, stDT_PLAYER_DATA);
        // CPlayerMgr::Instance()->RemovePlayer(poPlayer->GetID());
    }
    
    return TRUE;
}

BOOL  CDBProcessor::OnPlayerDataAck_AddToBlack(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnAddToBlackListAck(poPlayer, poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: pack error！"), MSG_MARK );
		return FALSE;
	}
	return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}


BOOL  CDBProcessor::OnPlayerDataAck_GetFriendsNum(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strErrorMsg.c_str());
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strErrorMsg.c_str());
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    CFriends *poFriends = poTargetPlayer->GetFriends();
    if (NULL == poFriends)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strErrorMsg.c_str());
        return FALSE;
    }

    map<string , UINT32> mapData;
    mapData["friendsnum"] = poFriends->GetFriendsNum();
    string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapData);
    pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strMsg);
    return TRUE;
	return 0 ;
}

BOOL CDBProcessor::OnPlayerDataAck_AddHero( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID )
{
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strErrorMsg.c_str());
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    DBG_INFO(_SDT("[%s: %d]: InitPlayer "), MSG_MARK);
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strErrorMsg.c_str());
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

    UINT16 wErrCode = poTargetPlayer->GMRecuit(pstGetPlayerData->stGetPlayerDataReq.dwParam1, pstGetPlayerData->stGetPlayerDataReq.qwParam2);

    string strMsg = GetRsponeResult(wErrCode);
    pstGetPlayerData->stGetPlayerDataReq.poClinet->Rspone(strMsg);

    return TRUE;
}

BOOL CDBProcessor::OnPlayerInfoAwaken(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoAwakenAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerInfoGodWeapon(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoGodWeaponAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerInfoSkill(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoSkillAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerInfoPandect(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnViewPlayerAck(poPlayer, poTargetPlayer->GetID());
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerInfoJewelry(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoSkillAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerInfoEquip(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoEquipAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerFormationTip(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID)
{
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnReadFormationPlayerTipAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerSoul(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID) {
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser) {
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer) {
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode) {
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoSoulAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID) {
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerPet(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID) {
	CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
	if (NULL == poUser) {
		SYS_CRITICAL(_SDT("[%s:%d]: Find client failed.PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	CPlayer* poPlayer = poUser->GetPlayer();
	if(NULL == poPlayer) {
		SYS_CRITICAL(_SDT("[%s:%d]: player is NULL，PlayerID:%d"), MSG_MARK, dwTransID);
		return FALSE;
	}
	if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode) {
		SYS_CRITICAL(_SDT("[%s:%d]: data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
		return FALSE;
	}

	BOOL bAlreadyLogin = FALSE;
	CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
	if(NULL == poTargetPlayer)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
		return FALSE;
	}
	if( !bAlreadyLogin )
	{
		poTargetPlayer->OnLogin(FALSE);
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoPetAck(poTargetPlayer);
	if(INVALID_MSGID == wMsgID) {
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CDBProcessor::OnPlayerSnapshot(SGetPlayerData* pstGetPlayerData)
{
    if(SUCCESS != pstGetPlayerData->stGetPlayerDataAck.wErrCode)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: CDBProcessor::OnPlayerSnapshot data error :errcode%d"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.wErrCode);
        return FALSE;
    }

    BOOL bAlreadyLogin = FALSE;
    CPlayer* poTargetPlayer = InitPlayer(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA, NULL, bAlreadyLogin);
    if(NULL == poTargetPlayer)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: init player[%d] faild"), MSG_MARK, pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
        return FALSE;
    }
    if( !bAlreadyLogin )
    {
        poTargetPlayer->OnLogin(FALSE);
    }

	CPlayerSnapShotMgr::Instance()->AddPlayerData(pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData);
	return TRUE;
}
