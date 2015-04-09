#include "nsprocessor.h"
#include <net/db/dbpktbuilder.h>
#include <net/gt/gtpktbuilder.h>
#include <net/cli/usermgr.h>
#include <framework/gsconfig.h>
#include <db/autosqlbuf.h>
#include <commondef.h>
#include <sdutil.h>
#include <errdef.h>
#include <logic/player/playermgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/playergetdatamgr.h>
#include <common/server/utility.h>
#include <logic/other/phystrengthmgr.h>
#include <logic/race/racepropmgr.h>
#include "net/recharge/rechargeclient.h"
#include "net/recharge/rechargepktbuilder.h"
#include "net/recharge/rechargeclient.h"
#include "protocol/server/protogsns.h"



CNSProcessor::CNSProcessor()
{
    Init() ;
}

CNSProcessor::~CNSProcessor()
{

}

CSDProtocol* CNSProcessor::GetProtocol()
{
    return CProtoGSNS::Instance();
}


BOOL CNSProcessor::Init()
{
    //使用CGTPipeChannel处理的消息,需要在CGTPipeChannel::OnRecvz中特别处理
    RegisterCommand(GTGS_ENTERGS_REQ, CNSProcessor::OnEnterGSReq);
    return TRUE ;
}


BOOL CNSProcessor::OnEnterGSReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{



    return TRUE;
}