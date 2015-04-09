
#include "rechargepktbuilder.h"
#include <protocol/server/protorechargegs.h>
#include <protocol/server/protocligs.h>
#include "json/json.h"
#include "net/cli/usermgr.h"
#include "logic/player/playermgr.h"
#include <db/dbmgr.h>
#include <db/autosqlbuf.h>
#include <stdexcept>
#include <logic/vip/vippropmgr.h>
#include <common/server/base64.h>
#include <logic/record/recordmgr.h>
#include <logic/player/playersavemgr.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <utility.h>

#include <errdef.h>
using namespace std;

CRechargePktBuilder::CRechargePktBuilder()
{
}

CRechargePktBuilder::~CRechargePktBuilder()
{
}


UINT16 CRechargePktBuilder::RechargeAck(tstring strRechargeAck)
{
    ZeroPacketBuffer();
    PKT_RECGS_RECHARGE_ACK* pstNtf = (PKT_RECGS_RECHARGE_ACK*)(GetPacketBuffer());

    //todo...

    return RECGS_RECHARGE_ACK;
}


UINT16 CRechargePktBuilder::SendHeart()
{
    ZeroPacketBuffer();
    PKT_RECGS_RECHARGE_ACK* pstNtf = (PKT_RECGS_RECHARGE_ACK*)(GetPacketBuffer());

    return RECGS_HEART_ACK;
}

UINT16 CRechargePktBuilder::QueryOrderID( UINT32 dwPlayerID, const string &strDispName, UINT16 wPlayerLevel, UINT16 wZoneID, UINT32 dwUserID )
{
    ZeroPacketBuffer();
    PKT_RECGS_GEN_GUID_ACK* pstQueryGenGUID = (PKT_RECGS_GEN_GUID_ACK*)(GetPacketBuffer());
    pstQueryGenGUID->szInfo;
    Json::Value root;
    root["PlayerID"] = dwPlayerID;
    root["DispName"] = strDispName;
    root["PlayerLevel"] = wPlayerLevel;
    root["ZoneID"] = wZoneID;
    root["UserID"] = dwUserID;

    string strInfo = root.toStyledString();
    memcpy( pstQueryGenGUID->szInfo, strInfo.c_str(), strInfo.size() );

    USR_INFO( _SDT( "player query order, gen msg:%s" ), strInfo.c_str() );
    return RECGS_GEN_GUID_REQ;
}

UINT16 CRechargePktBuilder::GenGUIDAck2GT( const string &strRet, CPlayer **ppPlayerSend )
{
    Json::Reader reader;
    Json::Value root;

    USR_INFO( _SDT( "GUID gen info:%s"  ), strRet.c_str() );

    if( !reader.parse( strRet, root ) )
    {
        SYS_CRITICAL( _SDT( "gen GUID return error" ) );
        return INVALID_MSGID;
    }
    CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer( root["PlayerID"].asUInt() );

    if( NULL == pPlayer )
    {
        SYS_CRITICAL( _SDT( "gen GUID return not include correct player ID" ) );
        return INVALID_MSGID;
    }

    *ppPlayerSend = pPlayer;
    ZeroPacketBuffer();
    PKT_CLIGS_GET_ORDER_ID_ACK *pstAck = (PKT_CLIGS_GET_ORDER_ID_ACK*)(GetPacketBuffer());

    string strOrderID = root["OrderID"].asString();
    BOOL bResult = root["Result"].asBool();
    if( root["Result"].asBool() && strOrderID.size() )
    {
        pstAck->wErrCode = ERR_GET_ORDER_ID::ID_SUCCESS;
        memcpy( pstAck->aszOrderID, strOrderID.c_str(), strOrderID.size() );
    }
    else
    {
        pstAck->wErrCode = ERR_GET_ORDER_ID::ID_ERR_PARSE_DATA_FROM_EXCHANGE_SERVER;
    }

    USR_INFO( _SDT( "get order id,ret:%d" ), pstAck->wErrCode );
    return CLIGS_GET_ORDER_ID_ACK;
}

VOID CRechargePktBuilder::ProcessExchangeAck2GT( const string &strRet, CRechargeClient* poClient )
{
    Json::Reader reader;
    Json::Value root;

    DBG_INFO( _SDT( "recv order info:%s" ), strRet.c_str() );
    if( !reader.parse( strRet, root ) )
    {
        SYS_CRITICAL( _SDT( "recv order parse error" ) );
        return;
    }

    //批量订单传递过来,以订单为key
    Json::Value::Members mem = root.getMemberNames();
    for( std::size_t nIndex = 0; nIndex < mem.size(); ++nIndex )
    {
        try
        {
            CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( SDAtou( mem[nIndex].c_str() ) );

            if( NULL == pPlayer )
            {
                SYS_CRITICAL( _SDT( "player id error or player offline%u" ), root[ mem[nIndex] ][ "PlayerID" ].asUInt() );
                continue;
            }

            ZeroPacketBuffer();

            Json::Value result;
            PKT_RECGS_PROCESS_ORDER_ID_ACK *pstAck = (PKT_RECGS_PROCESS_ORDER_ID_ACK*)(GetPacketBuffer());
            if( root[ mem[nIndex] ]["OrderID"].asString() == "" )
            {
                //通知充值服务器无效订单号
                result["ResultCode"] = PROCESS_EXCHANGE_ORDER_ID_INVAID;
                memcpy( pstAck->szInfo, result.toStyledString().c_str(), result.toStyledString().size() );
                poClient->SendMsg( RECGS_PROCESS_ORDER_ID_ACK, GetPacketBuffer() );
                continue;
            }


            if( root[ mem[nIndex] ]["OrderID"].asString() == pPlayer->GetLastOrderID() )
            {
                //已经处理过的订单号
                result["ResultCode"] = PROCESS_EXCHANGE_ORDER_ID_REPEAT;
                result["OrderID"] = string( pPlayer->GetLastOrderID() );
                result["PlayerID"] = pPlayer->GetDT_PLAYER_BASE_DATA().dwID;
                memcpy( pstAck->szInfo, result.toStyledString().c_str(), result.toStyledString().size() );
                poClient->SendMsg( RECGS_PROCESS_ORDER_ID_ACK, GetPacketBuffer() );
                continue;
            }

            UINT32 dwAmount = root[ mem[nIndex] ]["Amount"].asUInt();
            UINT32 dwExchangeGold = Fen2Mao( dwAmount );

            //pPlayer->GetDT_PLAYER_BASE_DATA().qwGold+=dwExchangeGold;
            pPlayer->AddGold(dwExchangeGold, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, root[ mem[nIndex] ]["OrderID"].asString(), dwAmount);

            _SDTStrcpy( pPlayer->GetLastOrderID(), root[ mem[nIndex] ]["OrderID"].asString().c_str() );

            //玩家已经正常处理成功，通知玩家

            PKT_CLIGS_EXCHANGE_RESULT *pstNtfAck = (PKT_CLIGS_EXCHANGE_RESULT*)(GetPacketBuffer());

            pstNtfAck->wErrCode = ERR_EXCHANGE_RESULT_ID::ID_SUCCESS;
            pstNtfAck->qwGold = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
            pstNtfAck->dwExchangeGold = dwExchangeGold;

            pstNtfAck->byVipLevel = pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
            pstNtfAck->dwTotalExchangeGold = pPlayer->GetDT_PLAYER_BASE_DATA().dwTotalAmount;
            pPlayer->SendMsg( GetPacketBuffer(), CLIGS_EXCHANGE_RESULT );

            //通知充值服务器处理完毕
            result["ResultCode"] = 0;
            //描述
            result["ResultDes"] = "process success!";
            result["OrderID"] = pPlayer->GetLastOrderID();
            result["PlayerID"] = pPlayer->GetDT_PLAYER_BASE_DATA().dwID;
            memcpy( pstAck->szInfo, result.toStyledString().c_str(), result.toStyledString().size() );

            poClient->SendMsg( RECGS_PROCESS_ORDER_ID_ACK, GetPacketBuffer() );
            USR_INFO( _SDT( "order process succeed:%s gold:%d" ), result.toStyledString().c_str(), dwAmount );

            CPlayerSaveMgr::Instance()->SavePlayerData( pPlayer );
        }
        catch( std::runtime_error &error )
        {
            SYS_CRITICAL( _SDT("process order exception, maby data error, error:%s"), error.what() );
        }
    }
}

// UINT16 CRechargePktBuilder::QueryAppOrderIDCheck( CPlayer *poPlayer, const string &strReceiptData )
// {
// 	ZeroPacketBuffer();
// 	PKT_RECGS_CHECK_APP_ORDER_ID_REQ *pstReq=(PKT_RECGS_CHECK_APP_ORDER_ID_REQ*)(GetPacketBuffer());
//
// 	USR_INFO( _SDT("收到订单校验信息,玩家ID:%u 订单数据:%s"), poPlayer->GetID(), strReceiptData.c_str() );
//
// 	Json::Value root;
// 	root["PlayerID"]=poPlayer->GetID();
// 	root["ReceiptData"]=strReceiptData;
// 	root["PlayerLevel"]=poPlayer->GetLevel();
// 	root["ZoneID"]=poPlayer->GetUser()->GetZoneID();
// 	root["DispName"]=poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName;
//
// 	strcpy( pstReq->szInfo, root.toStyledString().c_str() );
//
// 	return RECGS_CHECK_APP_ORDER_ID_REQ;
// }