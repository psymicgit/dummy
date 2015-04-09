
#include "nspktbuilder.h"
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <protocol/server/protogtgs.h>
#include <logic/bag/bag.h>
#include <common/client/commondef.h>
#include <logic/battle/battlemgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/build/buildpropmgr.h>
#include <logic/build/build.h>
#include <logic/shop/shopmgr.h>
#include <logic/formation/formationmgr.h>
#include <logic/item/itemmakemgr.h>
#include <logic/onhook/onhookmgr.h>
#include <logic/plunder/plundermgr.h>
#include <logic/godweapon/godweaponmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/chat/chatmgr.h>
#include <logic/instance/instancepropmgr.h>
#include <logic/yabiao/yabiaomgr.h>
#include <logic/player/playermgr.h>


CNSPktBuilder::CNSPktBuilder()
{

}

CNSPktBuilder::~CNSPktBuilder()
{

}

UINT16 CNSPktBuilder::EnterGSAck(UINT16 wErrCode, CPlayer* poPlayer)
{
    return CLIGS_ENTERGS_ACK;
}