
#include "gtpktbuilder.h"
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
#include <logic/item/equip/openlightpropmgr.h>
#include <logic/onhook/onhookmgr.h>
#include <logic/plunder/plundermgr.h>
#include <logic/godweapon/godweaponmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/chat/chatmgr.h>
#include <logic/instance/instancepropmgr.h>
#include <logic/yabiao/yabiaomgr.h>
#include <logic/player/playermgr.h>
#include <logic/vip/vippropmgr.h>
#include <logic/other/xiangqianmgr.h>
#include <logic/record/recordmgr.h>
#include <logic/lua/luamgr.h>
#include <db/basedbccmd.h>
#include <db/setapppayinfo.h>
#include <framework/gsconfig.h>
#include <logic/activity/bossbattlemgr.h>
#include <logic/other/commoninfomgr.h>
#include <logic/faction/factionmgr.h>
#include <logic/other/gsnoticemgr.h>
#include <logic/itemexchange/itemexchangemgr.h>
#include <logic/activityencourage/oprateingactivitymgr.h>
#include <logic/faction/faction.h>
#include <logic/friendsys/sendflowerpropmgr.h>
#include <logic/friendsys/privatemsgmgr.h>
#include "logic/activity/couragemgr.h"
#include "logic/encourage/onlineencouragerecord.h"
#include "logic/other/locale.h"
#include "logic/other/state.h"
#include "logic/race/race.h"
#include "logic/item/itemmgr.h"
#include "logic/task/taskmgr.h"
#include "logic/race/racemgr.h"
#include "logic/encourage/loginencouragerecord.h"
#include "logic/moneytree/moneytree.h"
#include "logic/hero/hero.h"
#include "logic/other/phystrength.h"
#include "logic/yabiao/yabiaoinfo.h"
#include "logic/vip/vipextdata.h"
#include "logic/jingjie/jingjie.h"
#include "logic/skill/collectgas.h"
#include "logic/other/worship.h"
#include "logic/other/rdchallenge.h"
#include "logic/activityencourage/activityencourage.h"
#include <logic/player/player.h>
#include <logic/plunder/plunderbasepropmgr.h>
#include <logic/jingjie/jingjiepropmgr.h>
#include <logic/awaken/awaken.h>
#include "logic/crop/crop.h"
#include "logic/climbtower/climbtower.h"
#include "logic/other/gatherscience.h"
#include "logic/other/equipcompose.h"
#include <logic/faction/factionplayer.h>
#include <framework/gsapi.h>
#include <net/cli/user.h>
#include "logic/science/sciencetree.h"
#include "logic/begbluegas/begblue.h"
#include "logic/activity/eatbread.h"
#include "logic/http/pushplayerinfohttpcmd.h"
#include "logic/chat/wordfilter.h"
#include "logic/gvg/playergvg.h"
#include "logic/activity/couragemgr.h"
//#include "logic/vip/vipgiftpropmgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/other/errmsgmgr.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/player/playerslogmgr.h"
#include "logic/soul/playersoul.h"
#include "logic/soul/soulmanager.h"
#include "logic/soul/soul.h"
#include "logic/soul/soulpropmgr.h"
#include "logic/medal/playermedal.h"
#include "logic/gvg/local/gvgfactionmgr.h"
#include "logic/pet/petmng.h"
#include "logic/activityencourage/activityencouragemgr.h"
#include "logic/item/equip/equipmakemgr.h"
#include "logic/fly/flypropmgr.h"
#include "logic/item/equip/equipmakemgr.h"
#include "logic/other/exchange.h"
#include "logic/hero/heropropmgr.h"

CGTPktBuilder::CGTPktBuilder()
{
    memset(&m_szEncodeBuffer, 0, sizeof(m_szEncodeBuffer));
    m_nEncodedLen   = -1;
}

CGTPktBuilder::~CGTPktBuilder()
{

}

UINT16 CGTPktBuilder::EnterGSAck(UINT16 wErrCode, CPlayer* poPlayer)
{
    ZERO_PACKET(PKT_CLIGS_ENTERGS_ACK);

    pstAck->wErrCode = wErrCode;
    if(ERR_ENTER_GS::ID_SUCCESS != pstAck->wErrCode)
    {
        return CLIGS_ENTERGS_ACK;
    }

    if(NULL == poPlayer)
    {
        return CLIGS_ENTERGS_ACK;
    }

    CLuamgr::Instance()->CkFinishActivity(poPlayer, 1);

    //基本信息
    if(NULL == poPlayer->GetDT_PLAYER_BASE_DATA_CLI(pstAck->stPlayerInfo))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
    }

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    //武将列表
    pstAck->wHeroNum = 0;
    poPlayer->GetCarryHeroInfo( pstAck->wHeroNum, pstAck->astHeroInfoLst );

    poPlayer->GetBuildList(pstAck->wBuildNum, pstAck->astBuildInfo);
    //在线奖励信息

    UINT16 encourageRet = poPlayer->GetOnlineEncourageRecord().GetCurEncourage(pstAck->awRecvCountDown[0], pstAck->astOnlineEncourage[0]);
    if( ERR_RECV_ONLINE_ENCOURAGE::ID_SUCCESS == encourageRet )
    {
        pstAck->byOnlineEncourageFlag = 1;
    }

    //未读战报
    poPlayer->GetUnreadBattleLogNum(pstAck->byBattleLogTypeNum, pstAck->astUnreadBattleLogInfo);
    pstAck->wJuniorLevel = CSinglePramMgr::Instance()->GetJuniorLevel();
    CVipPropMgr::Instance()->OpenVipTab(poPlayer->GetAuthType(), pstAck->stVipTabInfo.byVipLevelNum, pstAck->stVipTabInfo.astVipLevelInfo);
    pstAck->stVipTabInfo.byCurVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    pstAck->stVipTabInfo.dwHaveRechargeGold = poPlayer->GetDT_PLAYER_BASE_DATA().dwTotalAmount;

    poPlayer->GetCliVerInfo(pstAck->dwCliVer, pstAck->dwResVer, pstAck->dwUIVer, pstAck->aszResVer);
    pstAck->qwServerTime = SDTimeSecs();
    pstAck->wCanUseBoxGoodsID = poPlayer->GetBoxGoodsPrompt();
    poPlayer->GetDT_EXT_FUNC_PARAM_LST(pstAck->stExtFuncParamLst);
    poPlayer->GetActivityCenterPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    CGsNoticeMgr::Instance()->GetGsNoticeData(pstAck->stNoticeNewInfo);
	//旧版本兼容（强制更新后可以删除）
	pstAck->stNoticeInfo.dwNoticeID = pstAck->stNoticeNewInfo.dwNoticeID;
	pstAck->stNoticeInfo.byUrlFlag = pstAck->stNoticeNewInfo.byUrlFlag;
	pstAck->stNoticeInfo.byForcePopFlag = pstAck->stNoticeNewInfo.byForcePopFlag;
	SDStrncpy(pstAck->stNoticeInfo.aszNoticeContent, pstAck->stNoticeNewInfo.aszNoticeContent, MAX_CONTENT_SIZE - 1);

    COprateingActivityMgr::Instance()->GetOprateingActivityData(poPlayer, pstAck->stOperatingActivityInfo);
    // 获取未读离线私聊消息总数
    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL != poFriends)
    {
        //获取未读消息条数
        poFriends->GetOfflinePrivateMsgNum(pstAck->wUnreadOfflineMsgNum);

        //发送送花通知
        poFriends->CkRecvFlowerNtf();
    }

    CFaction* poFaction = poPlayer->GetFaction();
    if (NULL == poFaction)
    {
        pstAck->byHaveFaction = ESM_NO;
    }
    else
    {
        pstAck->byHaveFaction = ESM_YES;
    }

    BYTE byReEditName = get_bit(poPlayer->GetExtData(), em_EDIT_NAME);
	if(byReEditName > 0)
	{
		pstAck->byEditDspName = 1;
        string strMsg = CMsgDefMgr::Instance()->GetErrMsg("YOU_NEED_REEDIT_YOUR_NAME");

        memset(pstAck->aszEditDspName, 0, MAX_EDIT_LEN);
        SGDP::SDStrncpy(pstAck->aszEditDspName, strMsg.c_str(), MAX_EDIT_LEN - 1);
	}
	else
	{
		pstAck->byEditDspName = 0;
	}
    //给好友发送在线信息
    vector<UINT32> vecPlayerID;
    CFriends::GetFriends(poPlayer->GetID(), vecPlayerID);
    CChatMgr::Instance()->AddFriendsOnline(poPlayer->GetID(), vecPlayerID);

    pstAck->byIsFly = poPlayer->IsFly();
    return CLIGS_ENTERGS_ACK;
}

UINT16 CGTPktBuilder::GetHeroInfoErrorAck( UINT32 dwError )
{
    ZERO_PACKET(PKT_CLIGS_GET_HERO_INFO_ACK);
    pstAck->wErrCode = dwError;

    return CLIGS_GET_HERO_INFO_ACK;
}

UINT16 CGTPktBuilder::GetHeroInfoAck(CPlayer* poPlayer, UINT8 byEquipBagFlag, BOOL bSelfQuery)
{
    ZERO_PACKET(PKT_CLIGS_GET_HERO_INFO_ACK);

    if(NULL == poPlayer)
    {
        pstAck->wErrCode = ERR_GET_HERO_INFO::ID_PLAY_NOT_EXIT;
        return CLIGS_GET_HERO_INFO_ACK;
    }

    UINT16 wHeroNum = 0;
    poPlayer->GetCarryHeroInfo( wHeroNum, pstAck->stHeroInfo.astHeroBaseData,
                                pstAck->stHeroInfo.astEquipDataList, pstAck->stHeroInfo.astEnableAttrUpdate, pstAck->stHeroInfo.astSkillInfo );

    pstAck->stHeroInfo.wHeroNum = wHeroNum;

    pstAck->dwPlayerID = poPlayer->GetID();
    pstAck->wCoachExpRate = CSinglePramMgr::Instance()->GetCoachExpRate();
    if( bSelfQuery )
    {
        pstAck->byEquipBagFlag = byEquipBagFlag;
    }
    else
    {
        pstAck->byEquipBagFlag = NONNEED;
    }

    //战斗信息赋值
    pstAck->stPlayerBattleInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    SDStrcpy( pstAck->stPlayerBattleInfo.aszDisplayName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName );

    if( NULL != poPlayer->GetRace() )
    {
        //新玩家没有竞技场信息
        pstAck->stPlayerBattleInfo.dwRank = poPlayer->GetRace()->GetDT_RACE_BASE_DATA().dwRank;
    }

    //赋值装备背包信息
    if( NEED == pstAck->byEquipBagFlag )
    {

        poPlayer->GetBag().GetDT_BAG_DATA_CLI(pstAck->astEquipBagDataInfo[0].stBagInfo);

        //玩家背包中空闲装备
        poPlayer->GetIdleEquip(pstAck->astEquipBagDataInfo[0].stBagEquipDataList);

        pstAck->byGoodsCellNum = poPlayer->GetBag().GetGoodsCellNum();
    }

    CXiangqianMgr::Instance()->GetXiangqianEquipDes( pstAck->stEquipXiangqianDes.aszEquipPos1Des, pstAck->stEquipXiangqianDes.aszEquipPos2Des,
            pstAck->stEquipXiangqianDes.aszEquipPos3Des, pstAck->stEquipXiangqianDes.aszEquipPos4Des );

    pstAck->byVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;

    return CLIGS_GET_HERO_INFO_ACK;
}

UINT16 CGTPktBuilder::OnEnterHomeAck(CPlayer* poPlayer, UINT8 byKind)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_ENTER_HOME_ACK);

    pstAck->wErrCode = 0;
    pstAck->byKind = byKind;

    UINT32 dwFactionID = poPlayer->GetFactionID();
    if (dwFactionID == 0)
    {
        pstAck->byHaveFactionState = 0;	//无门派
    }
    else
    {
        pstAck->byHaveFactionState = 1;	//有门派
    }

    //建筑信息
    poPlayer->GetBuildList(pstAck->wBuildNum, pstAck->astBuildInfo);

    //登录奖励状态

    //不返回登录奖励
    pstAck->byLoginEncourageState = 0;//oLoginEncRecd.GetLoginEncourageState();

    //未读战报
    poPlayer->GetUnreadBattleLogNum(pstAck->byBattleLogTypeNum, pstAck->astUnreadBattleLogInfo);


    poPlayer->GetState().GetUnlockInfo(pstAck->awUnlockHeroID, pstAck->wUnlockEquipLevel,
                                       pstAck->dwUnlockElite, pstAck->stUnlockScienceList );

    //引导
    memcpy(pstAck->abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);

    //提示
    poPlayer->GetPrompt(pstAck->stPromptInfo);
    pstAck->stOtherPromptInfo.byHaveBetterHeroFlag = poPlayer->HaveBetterHero() == TRUE ? 1 : 0;
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);
    poPlayer->GetSideActiviPrompt(pstAck->stActiveSidePrompt);
    //回家园，则退出副本(若在副本中)
    poPlayer->ExitInstance();

    //位置回调
    poPlayer->GetLocale().OnEnterHome();
	poPlayer->GetState().OnEnterHome();

    // 将最新的玩家信息<等级、昵称、职业>推送到用户中心
    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_PUSH_PLAYER_INFO);
    if (NULL == poHttpCmd)
    {
        SYS_CRITICAL( _SDT("[%s:%d] error, alloc CPushPlayerInfoHttpCMD failed when player:%u enter home: memory not enough"), MSG_MARK, poPlayer->GetID());
    }
    else
    {
        SPlayerBaseInfo stPlayerBaseInfo;
        stPlayerBaseInfo.dwPlayerID  = poPlayer->GetID();
        stPlayerBaseInfo.wLevel      = poPlayer->GetLevel();
        stPlayerBaseInfo.strNickName = poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName;
		stPlayerBaseInfo.byVipLv	 = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
		stPlayerBaseInfo.byAuthType = poPlayer->GetAuthType();
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poPlayer->GetID());
        if(poPlayerBaseData )
        {
            stPlayerBaseInfo.wCoachHeroID    = poPlayerBaseData->wMainHeroKindID;
        }
        else
        {
            stPlayerBaseInfo.wCoachHeroID    = poPlayer->GetCoachHeroKindID();
            SYS_CRITICAL( _SDT("[%s:%d] push playerinfo of player:%u failed: can't find SPlayerBaseData in CPlayerBaseDataCacheMgr"), MSG_MARK, poPlayer->GetID());
        }
            
        poHttpCmd->SetUserData(&stPlayerBaseInfo, sizeof(SPlayerBaseInfo), 0);
        poHttpCmd->Init();

        if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
        {
            CHttpMgr::Instance()->FreeCmd(poHttpCmd);
            SYS_CRITICAL( _SDT("[%s:%d] push playerinfo of player:%u failed: can't AddCommand"), MSG_MARK, poPlayer->GetID());
        }
    }

    return CLIGS_ENTER_HOME_ACK;
}


UINT16 CGTPktBuilder::EnterInstanceAck(UINT16 wErrCode, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_ENTER_INSTANCE_ACK);
    if(0 != wErrCode)
    {
        pstAck->wErrCode = wErrCode;
        return CLIGS_ENTER_INSTANCE_ACK;
    }

    //返回悬挂物
    //poPlayer->GetDT_HANG_DATA_LIST(pstAck->stHangInfo);

    //返回怪物信息
    DT_MONSTER_DATA_LIST* pstDT_MONSTER_DATA_LIST = CMonsterMgr::Instance()->GetInstanceMonster(wSceneIdx, byTownIdx, byInstanceIdx);
    if(NULL == pstDT_MONSTER_DATA_LIST)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
    }
    else
    {
        memcpy(&(pstAck->stMonsterInfo), pstDT_MONSTER_DATA_LIST, sizeof(DT_MONSTER_DATA_LIST));
    }

    //返回玩家HP
    pstAck->dwMaxHP = static_cast<UINT32>(poPlayer->GetMaxDT_BATTLE_ATTRIBUTE().qwHP);
    pstAck->dwCurHP = static_cast<UINT32>(poPlayer->GetCurDT_BATTLE_ATTRIBUTE().qwHP);

    CInstanceRecord* poInstanceRecord = poPlayer->GetInstance().GetSingleInstaceRecord(wSceneIdx, byTownIdx, byInstanceIdx);
    if (NULL != poInstanceRecord)
    {
        pstAck->byScore = poInstanceRecord->GetDT_INSTANCE_DATA().byScore;
    }

    //返回体力信息
    //获取体力信息
    if(!poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stPhyStrengthInfo))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_ENTER_INSTANCE_ACK;
    }

    //宝箱信息
    poPlayer->GetInstance().GenEncInfo(pstAck->byHaveBoxFlag, pstAck->astBoxInfo[0], pstAck->astHeroUpgradeInfo[0]);
	if ( EIT_OUT_BOUND == wSceneIdx && ESM_YES == pstAck->byHaveBoxFlag)
	{
		 memset(&(pstAck->stMonsterInfo), 0, sizeof(DT_MONSTER_DATA_LIST));
	}
    //
    pstAck->wDoubleExpNum = poPlayer->GetState().GetDT_STATE_DATA().wDoubleExpNum;

    poPlayer->GetCliVerInfo(pstAck->dwCliVer, pstAck->dwResVer, pstAck->dwUIVer, pstAck->aszResVer);



    return CLIGS_ENTER_INSTANCE_ACK;
}




UINT16 CGTPktBuilder::ExitInstanceAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EXIT_INSTANCE_ACK);

    poPlayer->ExitInstance();

    if(!poPlayer->GetDT_PLAYER_BASE_DATA_CLI(pstAck->stPlayerInfo))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_EXIT_INSTANCE_ACK;
    }

    pstAck->wErrCode = ERR_EXIT_INSTANCE::ID_SUCCESS;

    return CLIGS_EXIT_INSTANCE_ACK;
}



UINT16 CGTPktBuilder::GetInstanceRecordAck(PKT_CLIGS_GET_SCENE_INSTANCE_ACK* pstPKT_CLIGS_GET_SCENE_INSTANCE_ACK)
{
    ZERO_PACKET(PKT_CLIGS_GET_SCENE_INSTANCE_ACK);

    memcpy(pstAck, pstPKT_CLIGS_GET_SCENE_INSTANCE_ACK, sizeof(PKT_CLIGS_GET_SCENE_INSTANCE_ACK));
    return CLIGS_GET_SCENE_INSTANCE_ACK;
}

//2012-11-28:打开铁匠铺
UINT16 CGTPktBuilder::OpenSmithyAck(CPlayer* poPlayer, UINT8 byBuildKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    if (poPlayer->GetCliVer() >= VERSION_FLY)
    {
        ZERO_PACKET(PKT_CLIGS_OPEN_SMITHY_ACK_v410);
        pstAck->wErrCode = poPlayer->GetOpenSmithyAck_v410(*pstAck);

        return CLIGS_OPEN_SMITHY_ACK_v410;
    }
    else
    {
        ZERO_PACKET(PKT_CLIGS_OPEN_SMITHY_ACK);
        pstAck->wErrCode = poPlayer->GetOpenSimthyAck(*pstAck);

        return CLIGS_OPEN_SMITHY_ACK;
    }

    return INVALID_MSGID;
}

UINT16 CGTPktBuilder::SmithyBuyEquipAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byEquipPos)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_SMITHY_BUY_EQUIP_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poPlayer->SmithyBuyEquip(wHeroKindID, byEquipPos, pstAck->stEquipStrengthenInfo, pstAck->stEquipInfo, pstAck->dwStrengthen10Coin);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_SMITHY_BUY_EQUIP_ACK;
}

UINT16 CGTPktBuilder::StrengthenAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byStrengthenType, UINT16 wLevel)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EQUIP_STRENGTHEN_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CItemMgr::Instance()->Strengthen(poPlayer, wEquipKindID, wIdx, byStrengthenType, wLevel,
                       pstAck->stEquipStrengthenInfo, pstAck->stEquipEnchantInfo, pstAck->stEquipInfo, pstAck->dwStrengthen10Coin);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    pstAck->dwPlayerMaxHP = static_cast<UINT32>(poPlayer->GetMaxDT_BATTLE_ATTRIBUTE().qwHP);
    pstAck->dwPlayerCurHP = static_cast<UINT32>(poPlayer->GetCurDT_BATTLE_ATTRIBUTE().qwHP);
    pstAck->wStrengthenCountDown = poPlayer->GetStrenthenCountDown();

    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_EQUIP_STRENGTHEN_ACK;
}

UINT16 CGTPktBuilder::EquipEnchantAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT16 wLevel)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EQUIP_ENCHANT_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CItemMgr::Instance()->Enchant(poPlayer, wEquipKindID, wIdx, wLevel, pstAck->stEquipStrengthenInfo, pstAck->stEnchantInfo, pstAck->stEquipInfo);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    pstAck->dwEnchantStoneNum = poPlayer->GetBag().GetGoodsPileNum(ENCHANT_STONE_ITEM_ID);

    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_EQUIP_ENCHANT_ACK;
}

UINT16 CGTPktBuilder::GetOpenLightInfoAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_GET_OPENLIGHT_INFO_ACK);

    //
    pstAck->wEquipKindID = wEquipKindID;
    pstAck->wEquipIdx = wIdx;


    pstAck->wErrCode = COpenLightPropMgr::Instance()->GetOpenLightPropInfo(poPlayer, wEquipKindID, wIdx,
                       pstAck->stAfterOpenLightInfo, pstAck->wOpenLightGoodsID,
                       pstAck->wNeedOpenLightGoodsNum, pstAck->wOpenLightGoodsNum,
                       pstAck->byOpenLightRate, pstAck->dwOpenLightGold,
                       pstAck->stOldEquipInitBatlAttr, pstAck->stNewEquipInitBatlAttr,
                       pstAck->stOldEquipCurrBatlAttr);

    return CLIGS_GET_OPENLIGHT_INFO_ACK;
}

UINT16 CGTPktBuilder::OpenLightAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byGoldFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPENLIGHT_INFO_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = COpenLightPropMgr::Instance()->OpenLight(poPlayer, wEquipKindID, wIdx, byGoldFlag,
                       pstAck->wEquipKindID, pstAck->wEquipIdx, pstAck->byOpenLightFlagInfo,
                       pstAck->wOpenLightGoodsID, pstAck->wOpenLightGoodsNum);

    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    return CLIGS_OPENLIGHT_INFO_ACK;
}


UINT16 CGTPktBuilder::OpenRecruitAck(CPlayer* poPlayer, UINT8 byBuildKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    if (poPlayer->GetCliVer() >= VERSION_FLY)
    {
        ZERO_PACKET(PKT_CLIGS_OPEN_RECRUIT_ACK_v410);
        pstAck->wErrCode = poPlayer->GetOpenCruitAck_v410(*pstAck);
        poPlayer->GetRecruitPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

        return CLIGS_OPEN_RECRUIT_ACK_v410;
    }
    else
    {
        ZERO_PACKET(PKT_CLIGS_OPEN_RECRUIT_ACK);
        pstAck->wErrCode = poPlayer->GetOpenCruitAck(*pstAck);
        poPlayer->GetRecruitPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

        return CLIGS_OPEN_RECRUIT_ACK;
    }

    return INVALID_MSGID;
}


UINT16 CGTPktBuilder::RecruitHeroAck(CPlayer* poPlayer, UINT16 wHeroKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_RECRUIT_HERO_ACK);
    pstAck->wErrCode = poPlayer->Recuit(wHeroKindID, pstAck->stHeroBaseInfo);
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    pstAck->qwPlayerCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    poPlayer->GetRecruitPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    memcpy(pstAck->abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);

    return CLIGS_RECRUIT_HERO_ACK;
}


UINT16 CGTPktBuilder::DisbandHeroAck(CPlayer* poPlayer, UINT16 wHeroKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_DISBAND_HERO_ACK);
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poPlayer->Disbang(wHeroKindID);
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->dwPlayerHP = static_cast<UINT32>(poPlayer->GetMaxDT_BATTLE_ATTRIBUTE().qwHP);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_DISBAND_HERO_ACK;
}


UINT16 CGTPktBuilder::OpenBagAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	CBag& oBag = poPlayer->GetBag();
	if ( VERSION_PET <= poPlayer->GetCliVer())
	{
		ZERO_PACKET(PKT_CLIGS_OPEN_BAG_ACK2);
		pstAck->wErrCode = ERR_OPEN_BAG::ID_SUCCESS;
		oBag.GetDT_BAG_DATA_CLI(pstAck->stBagInfo);
		oBag.GetBagItem(pstAck->stBagItemInfo);
		CXiangqianMgr::Instance()->GetXiangqianEquipDes( pstAck->stEquipXiangqianDes.aszEquipPos1Des, pstAck->stEquipXiangqianDes.aszEquipPos2Des,
			pstAck->stEquipXiangqianDes.aszEquipPos3Des, pstAck->stEquipXiangqianDes.aszEquipPos4Des );
		return CLIGS_OPEN_BAG_ACK2;
	}
	else
	{
		ZERO_PACKET(PKT_CLIGS_OPEN_BAG_ACK);
		pstAck->wErrCode = ERR_OPEN_BAG::ID_SUCCESS;
		oBag.GetDT_BAG_DATA_CLI(pstAck->stBagInfo);
		oBag.GetBagItem(pstAck->stBagItemInfo);
		oBag.GetBagItem(pstAck->stBagItemInfo2);
		CXiangqianMgr::Instance()->GetXiangqianEquipDes( pstAck->stEquipXiangqianDes.aszEquipPos1Des, pstAck->stEquipXiangqianDes.aszEquipPos2Des,
			pstAck->stEquipXiangqianDes.aszEquipPos3Des, pstAck->stEquipXiangqianDes.aszEquipPos4Des );
		return CLIGS_OPEN_BAG_ACK;
	}

    return CLIGS_OPEN_BAG_ACK2;
}




UINT16 CGTPktBuilder::RecvTaskEncourageAck(CPlayer* poPlayer, UINT8 byTaskLineIdx, UINT32 dwTaskIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK);
    pstAck->byTaskLineIdx = byTaskLineIdx;
    pstAck->dwTaskIdx = dwTaskIdx;
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CTaskMgr::Instance()->RecvTaskEncourage(poPlayer, byTaskLineIdx, dwTaskIdx);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetUnlockNewInstance(pstAck->byUnlockNewInstanceFlag, pstAck->byHaveNewInstanceFlag, pstAck->astUnlockCommonInstanceID[0], pstAck->astNewCommonInstanceID[0]);//



    //若状态改变或已经领取，则下发整个任务线给客户端，并设置为领取任务成功
    if((ERR_RECV_TASK_ENCOURAGE::ID_STATE_ERR == pstAck->wErrCode) || (ERR_RECV_TASK_ENCOURAGE::ID_HAVE_RECV_ERR == pstAck->wErrCode))
    {
        poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList);
        pstAck->wErrCode = ERR_RECV_TASK_ENCOURAGE::ID_SUCCESS;
    }
    else
    {
        poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    }
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    poPlayer->GetHeroUpgradeInfo(pstAck->byHeroNum, pstAck->astHeroUpgradeInfo);


    //查找主角升级信息（临时方案）
    for (UINT8  byHeroNum = 0; byHeroNum < pstAck->byHeroNum; byHeroNum++)
    {
        if (pstAck->astHeroUpgradeInfo[byHeroNum].wHeroKindID == poPlayer->GetCoachHeroKindID())
        {
            memcpy(&pstAck->stMainHeroUpgradeInfo, &pstAck->astHeroUpgradeInfo[byHeroNum], sizeof(DT_HERO_UPGRADE_DATA));
            break;
        }
        //poPlayer->GetMainHeroUpgradeInfo(pstAck->stMainHeroUpgradeInfo);
    }


    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->qwStory = stDT_PLAYER_BASE_DATA.qwStory;
    pstAck->qwBlueGas = stDT_PLAYER_BASE_DATA.qwBlueGas;
    pstAck->qwPurpleGas = stDT_PLAYER_BASE_DATA.qwPurpleGas;
    pstAck->qwJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    //pstAck->dwCurFame = stDT_PLAYER_BASE_DATA.dwFame;
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stPhyStrengthInfo);
    pstAck->wUnlockEquipLevel = poPlayer->GetState().GetDT_STATE_DATA().wUnlockEquipLevel;
    poPlayer->GetState().GetDT_STATE_DATA().wUnlockEquipLevel = 0;
    pstAck->stUnlockScienceList = poPlayer->GetState().GetDT_STATE_DATA().stUnlockScienceList;
    memset( &pstAck->stUnlockScienceList, 0, sizeof( pstAck->stUnlockScienceList ) );
    memcpy(pstAck->abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);
    poPlayer->GetPrompt(pstAck->stPromptInfo);
    return CLIGS_RECV_TASK_ENCOURAGE_ACK;
}


UINT16 CGTPktBuilder::OpenRaceBuildAck(CPlayer* poPlayer, UINT8 byBuildKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_RACE_BUILD_ACK);
    pstAck->wErrCode = poPlayer->GetOpenRaceBuildAck(*pstAck);
    poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_OPEN_RACE_BUILD_ACK;
}

UINT16 CGTPktBuilder::ChallengeAck(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    CUser* poUser = poPlayer->GetUser();
	if ((poUser) && ( poUser->GetCliVer() >= VERSION_PET) )
	{
		return ChallengeAck5(wErrCode, poPlayer, poEnemy);
	}
	else if ((poUser) && ( poUser->GetCliVer() >= VERSION_SOUL))
	{
		return ChallengeAck4(wErrCode, poPlayer, poEnemy);
	}
	else if ((poUser) && (poUser->GetCliVer() > 310))
	{
		return ChallengeAck3(wErrCode, poPlayer, poEnemy);
	}
   
    
    return ChallengeAck5(wErrCode, poPlayer, poEnemy);

}

UINT16 CGTPktBuilder::ChallengeAck5(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_CHALLENGE_ACK5);

    if(0 != wErrCode)
    {
        pstAck->wErrCode = wErrCode;
        return CLIGS_CHALLENGE_ACK5;
    }

    //检测一下是否开放
    CRace* poRace = poPlayer->GetRace();
    if(NULL == poRace)
    {
        pstAck->wErrCode = ERR_CHALLENGE::ID_UNLOCK_ERR;
        return CLIGS_CHALLENGE_ACK5;
    }
    //检测条件是否满足
    wErrCode = poRace->CkCondition();
    if (ERR_CHALLENGE::ID_SUCCESS != wErrCode)
    {
        pstAck->wErrCode = wErrCode;
        return CLIGS_CHALLENGE_ACK5;
    }

    if(NULL == poEnemy)
    {
        pstAck->wErrCode = ERR_CHALLENGE::ID_ENEMY_NOT_EXIST_ERR;
        return CLIGS_CHALLENGE_ACK5;
    }

    if(poPlayer == poEnemy)
    {
        pstAck->wErrCode = ERR_CHALLENGE::ID_ENEMY_CANNOT_MYSELF_ERR;
        return CLIGS_CHALLENGE_ACK5;
    }

    if(FALSE == poEnemy->GetDT_ENEMY_HERO_DATA(pstAck->byEnemyHeroNum, pstAck->astEnemyInfoList))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_CHALLENGE_ACK5;
    }


    if(FALSE == CRaceMgr::Instance()->Challenge(poPlayer, poEnemy, pstAck->stBattleInfo, pstAck->stChallengeEncourage))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_CHALLENGE_ACK5;
    }


    //poPlayer->GetMainHeroUpgradeInfo(pstAck->stMainHeroUpgradeInfo);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    //pstAck->dwCurFame = stDT_PLAYER_BASE_DATA.dwFame;

    CRaceMgr::Instance()->GetChallengePlayerData(poEnemy->GetDT_PLAYER_BASE_DATA().dwID, pstAck->stEnemyPlayerData);
    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetSciencePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_CHALLENGE_ACK5;
}

UINT16 CGTPktBuilder::RecvRankEncourageAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK);

    CRace* poRace = poPlayer->GetRace();
    if(NULL == poRace)
    {
        pstAck->wErrCode = ERR_RECV_RANK_ENCOURAGE::ID_RACE_NOT_EXIST_ERR;
        return CLIGS_RECV_RANK_ENCOURAGE_ACK;
    }
    pstAck->wErrCode = poRace->RankEncourage(pstAck->stRankEncourage);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);


    return CLIGS_RECV_RANK_ENCOURAGE_ACK;
}


UINT16 CGTPktBuilder::OpenShopAck(CPlayer* poPlayer, UINT8 byTabID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_SHOP_ACK);
    CBuild* poBuild = poPlayer->GetBuild(EBK_SHOP);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SHOP::ID_SHOP_NOT_EXIST_ERR;
        return CLIGS_OPEN_SHOP_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CShopMgr::Instance()->OpenShopByTab(poPlayer->GetLevel(), byTabID, pstAck->byItemNum, pstAck->astItemDataList);
    pstAck->byTabID = byTabID;

    return CLIGS_OPEN_SHOP_ACK;
}

UINT16 CGTPktBuilder::BuyItemAck(CPlayer* poPlayer, UINT8 byTabID, UINT16 wItemKindID, UINT16 wPileCount)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_BUY_ITEM_ACK);
    pstAck->wErrCode = CShopMgr::Instance()->Buy(poPlayer, byTabID, wItemKindID, wPileCount);
    pstAck->byTabID = byTabID;
    pstAck->wKindID = wItemKindID;
    pstAck->wPileCount = wPileCount;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);

    return CLIGS_BUY_ITEM_ACK;

}

UINT16 CGTPktBuilder::SellItemAck(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wIdx, UINT16 wNum)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_SELL_ITEM_ACK);
    pstAck->wErrCode = CItemMgr::Instance()->Sell(poPlayer, wItemKindID, wIdx, wNum, pstAck->stItemNumList);
    pstAck->wKindID = wItemKindID;
    pstAck->wIdx = wIdx;
    pstAck->wNum = wNum;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    return CLIGS_SELL_ITEM_ACK;
}

UINT16 CGTPktBuilder::ClrPlayerCache(UINT8 byExt)
{
    ZERO_PACKET(PKT_GSGT_CLR_CACHE_RPT);
    pstAck->byExt = byExt;

    return GSGT_CLR_CACHE_RPT;
}

UINT16 CGTPktBuilder::RegiUserNameNtf()
{
    ZERO_PACKET(PKT_CLIGS_REGISTER_USERNAME_NTF);
    pstAck->byExt = 0;

    return CLIGS_REGISTER_USERNAME_NTF;
}

UINT16 CGTPktBuilder::EquipAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byDressType, UINT16 wDressItemKindID, UINT16 wDressItemIdx, UINT16 wUndressItemKindID, UINT16 wUndressItemIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EQUIP_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CItemMgr::Instance()->Equip(poPlayer, wHeroKindID, byDressType, wDressItemKindID, wDressItemIdx, wUndressItemKindID, wUndressItemIdx, pstAck->stHeroPowerInfo);
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byDressType = byDressType;
    pstAck->wDressItemKindID = wDressItemKindID;
    pstAck->wDressItemIdx = wDressItemIdx;
    pstAck->wUndressItemKindID = wUndressItemKindID;
    pstAck->wUndressItemIdx = wUndressItemIdx;
    pstAck->dwPlayerHP = static_cast<UINT32>(poPlayer->GetMaxDT_BATTLE_ATTRIBUTE().qwHP);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poPlayer->GetHeroPanelLstCli(pstAck->stHeroPanelInfo);
    return CLIGS_EQUIP_ACK;
}

UINT16 CGTPktBuilder::QuickEquipAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byEquipNum, DT_EQUIP_ID astEquipIDLst[EQUIP_POS_NUM])
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_QUICK_EQUIP_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CItemMgr::Instance()->QuickEquip(poPlayer, wHeroKindID, byEquipNum, astEquipIDLst, pstAck->stHeroPowerInfo);
    memcpy(pstAck->astEquipIDLst, astEquipIDLst, sizeof(pstAck->astEquipIDLst));
    pstAck->dwPlayerHP = static_cast<UINT32>(poPlayer->GetMaxDT_BATTLE_ATTRIBUTE().qwHP);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poPlayer->GetHeroPanelLstCli(pstAck->stHeroPanelInfo);
    return CLIGS_QUICK_EQUIP_ACK;
}


UINT16 CGTPktBuilder::EmbattleAck(CPlayer* poPlayer, DT_FORMATION_DATA& stFormationInfo)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EMBATTLE_ACK);
    CFormation &oFormation = poPlayer->GetFormation();
    pstAck->wErrCode = oFormation.Embattle(stFormationInfo);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_EMBATTLE_ACK;
}

UINT16 CGTPktBuilder::OpenEmbattleAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_EMBATTLE_ACK);
    CBuild* poBuild = poPlayer->GetBuild(EBK_FROMATION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_EMBATTLE::ID_FORMATION_NOT_OPEN;
        return CLIGS_OPEN_EMBATTLE_ACK;
    }
    poBuild->OnOpen();

    CFormation &oFormation = poPlayer->GetFormation();

    pstAck->wErrCode = oFormation.OpenEmbattleTab(pstAck->wHeroNum, pstAck->astHeroInfoLst, pstAck->stFormationInfo);

    return CLIGS_OPEN_EMBATTLE_ACK;
}

UINT16 CGTPktBuilder::MakeItemAck(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wEquipKindID, UINT16 wEquipIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_MAKE_ITEM_ACK);

    pstAck->wErrCode = CItemMakeMgr::Instance()->MakeItem(poPlayer, wItemKindID, wEquipKindID, wEquipIdx, pstAck->stItemInfo, pstAck->aszErrInfo);

    if (poPlayer->GetCliVer() < VERSION_FLY){
        poPlayer->GetBag().GetBagItem(pstAck->stGoodsBagItemInfo);
        poPlayer->GetBag().GetBagItem(pstAck->stGoodsBagItemInfo2);
    }else{
        poPlayer->GetBag().GetBagItemNum(pstAck->stBagItemNumInfo);
    }

    pstAck->qwBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;


    return CLIGS_MAKE_ITEM_ACK;
}

UINT16 CGTPktBuilder::OnOpenInstanceEncourageBoxAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK);

    pstAck->wErrCode = poPlayer->GetInstance().OpenEncourageBox(pstAck->stPassEncourage, &(pstAck->stAfterPassEncourage));

    memcpy(pstAck->abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);


    return CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK;
}

UINT16 CGTPktBuilder::OnIncreaseBagSizeAck(CPlayer* poPlayer, UINT16 wInc2Size, UINT8 byGoldFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_INCREASE_BAG_SIZE_ACK);

//    pstAck->wErrCode = poPlayer->GetBag().IncreaseSize(wInc2Size, byGoldFlag);
	pstAck->wErrCode = ERR_INCREASE_SIZE::ID_SUCCESS;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_INCREASE_BAG_SIZE_ACK;
}


UINT16 CGTPktBuilder::OnhookAck(CPlayer* poPlayer, UINT8 byRunTimes, DT_INSTANCE_ID stDT_INSTANCE_ID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_ONHOOK_ACK);
    CBuild* poBuild = poPlayer->GetBuild(EBK_ONHOOK);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_ONHOOK::ID_ONHOOK_UNLOCK;
        return CLIGS_ONHOOK_ACK;
    }
    poBuild->OnOpen();

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = COnhookMgr::Instance()->Onhook(poPlayer, byRunTimes, stDT_INSTANCE_ID, pstAck->stOnhookResult, pstAck->stAfterOnhookData);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetUnlockNewInstance(pstAck->byUnlockNewInstanceFlag, pstAck->byHaveNewInstanceFlag, pstAck->astUnlockCommonInstanceID[0], pstAck->astNewCommonInstanceID[0]);//
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stAfterOnhookData.stCurPhyStrengthInfo);
    //提示
    poPlayer->GetPrompt(pstAck->stPromptInfo);
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);
    pstAck->dwFlyExp = poPlayer->GetCoachHero()->GetFlyExp();

    return CLIGS_ONHOOK_ACK;
}

/*
UINT16 CGTPktBuilder::OnInterruptOnhookAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_INTERRUPT_ONHOOK_ACK);
    //ZERO_PACKET(PKT_CLIGS_INTERRUPT_ONHOOK_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_INTERRUPT_ONHOOK_ACK*)(GetPacketBuffer());
    if(NULL == poPlayer->GetBuild(EBK_ONHOOK))
    {
        pstAck->wErrCode = ERR_ONHOOK::ID_ONHOOK_UNLOCK;
        return CLIGS_INTERRUPT_ONHOOK_ACK;
    }
    pstAck->wErrCode = COnhookMgr::Instance()->Interrupt(poPlayer);

    return CLIGS_INTERRUPT_ONHOOK_ACK;
}

UINT16 CGTPktBuilder::OnNoCDOnhookAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_NOCD_ONHOOK_ACK);
    //ZERO_PACKET(PKT_CLIGS_NOCD_ONHOOK_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_NOCD_ONHOOK_ACK*)(GetPacketBuffer());
    if(NULL == poPlayer->GetBuild(EBK_ONHOOK))
    {
        pstAck->wErrCode = ERR_ONHOOK::ID_ONHOOK_UNLOCK;
        return CLIGS_NOCD_ONHOOK_ACK;
    }
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = COnhookMgr::Instance()->OnhookNoCD(poPlayer, pstAck->stOnhookResult, pstAck->stAfterOnhookData);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    return CLIGS_NOCD_ONHOOK_ACK;
}


UINT16 CGTPktBuilder::OnOnhookResultAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
	ZERO_PACKET(PKT_CLIGS_ONHOOK_RESULT_ACK);
    if(NULL == poPlayer->GetBuild(EBK_ONHOOK))
    {
        pstAck->wErrCode = ERR_ONHOOK::ID_ONHOOK_UNLOCK;
        return CLIGS_ONHOOK_RESULT_ACK;
    }
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = COnhookMgr::Instance()->GetOnhookResult(poPlayer, pstAck->stInstanceID, pstAck->dwNoCDGold, pstAck->wAllFinishCountDown,
                       pstAck->wNextFinishCountDown, pstAck->stOnhookResult, pstAck->stAfterOnhookData);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);


    return CLIGS_ONHOOK_RESULT_ACK;
}
*/

UINT16 CGTPktBuilder::OnClrChallengeCDAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLR_CHALLENGE_CD_ACK);
    CRace* poRace = poPlayer->GetRace();
    if(NULL == poRace)
    {
        pstAck->wErrCode = ERR_CLR_CHALLENGE_CD::ID_RACE_NOT_EXIST_ERR;
        return CLIGS_CLR_CHALLENGE_CD_ACK;
    }
    pstAck->wErrCode = poRace->ClrChallengeCD();
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_CLR_CHALLENGE_CD_ACK;
}


UINT16 CGTPktBuilder::OnRecvOnlineEncourageAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK);
    //登录奖励状态
    pstAck->wErrCode = poPlayer->GetOnlineEncourageRecord().RecvEncourage(pstAck->wRecvCountDown, pstAck->byNextOnlineEncourageFlag,
                       pstAck->astNextOnlineEncourage[0], pstAck->stCurPlayerInfo);
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stCurPhyStrengthInfo);


    return CLIGS_RECV_ONLINE_ENCOURAGE_ACK;
}

UINT16 CGTPktBuilder::OnSyncTimeAck(UINT64 qwCliTime)
{

    ZERO_PACKET(PKT_CLIGS_SYNC_TIME_ACK);
    pstAck->qwCliTime = qwCliTime;
    pstAck->qwServTime = SDTimeSecs();

    return CLIGS_SYNC_TIME_ACK;
}

UINT16 CGTPktBuilder::OnRegisterDspNameAck(CPlayer* poPlayer, CHAR* pszDspName)
{

    ZERO_PACKET(PKT_CLILS_REGISTER_DSPNAME_ACK);
    pstAck->wErrCode = poPlayer->RegisterDspName(pszDspName);
    SDStrncpy(pstAck->aszDspName, pszDspName, USERNAME_LEN - 1);

    return CLILS_REGISTER_DSPNAME_ACK;
}

UINT16 CGTPktBuilder::UseGoodsAck(CPlayer* poPlayer, UINT16 wGoodsKindID, UINT8 byGetBagInfoFlag, UINT16 wUseNum)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLILS_USE_GOODS_ACK);
    wUseNum = 0 == wUseNum ? 1 : wUseNum;
    pstAck->wUseNum = wUseNum;
    pstAck->wGoodsKindID = wGoodsKindID;

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CItemMgr::Instance()->UseGoods(poPlayer, wGoodsKindID, wUseNum, pstAck->byUseBoxFlag, pstAck->astBoxItemInfo[0]);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->wPhyStrength = stDT_PLAYER_BASE_DATA.wPhyStrength;
    pstAck->qwScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->qwBlueGas = stDT_PLAYER_BASE_DATA.qwBlueGas;
    pstAck->qwPurpleGas = stDT_PLAYER_BASE_DATA.qwPurpleGas;
    pstAck->qwJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    poPlayer->GetMainHeroUpgradeInfo(pstAck->astMainHeroUpgradeInfo[0]);
    pstAck->byMainHeroUpgradeFlag = pstAck->astMainHeroUpgradeInfo[0].byUpgradeFlag;
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    pstAck->byGetBagInfoFlag = byGetBagInfoFlag;
    if(1 == byGetBagInfoFlag)
    {
        if (poPlayer->GetCliVer() < VERSION_FLY){
            poPlayer->GetBag().GetBagItem(pstAck->astBagInfo[0]);
            poPlayer->GetBag().GetBagItem(pstAck->astBagInfo2[0]);
        }else{
            poPlayer->GetBag().GetBagItem(pstAck->astBagInfo2[0]);
            poPlayer->GetBag().GetBagItemNum(pstAck->stBagItemNumInfo);
        }
    }
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stCurPhyStrengthInfo);
    return CLILS_USE_GOODS_ACK;
}

UINT16 CGTPktBuilder::OnOpenLoginEncourageAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK);

    pstAck->wErrCode = poPlayer->GetLoginEncourageRecord().GetEncourageInfo(pstAck->stLoginEncourageInfo);

    return CLILS_OPEN_LOGIN_ENCOURAGE_ACK;
}

UINT16 CGTPktBuilder::OnRateLoginEncourageAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK);
    poPlayer->GetLoginEncourageRecord().OnRate();

    pstAck->byExt = 0;

    return CLILS_RATE_LOGIN_ENCOURAGE_ACK;
}

UINT16 CGTPktBuilder::OnPutLoginEncourageIntoBagAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK);

    pstAck->wErrCode = poPlayer->GetLoginEncourageRecord().PutIntoBag(pstAck->byHaveEncourageFlag, pstAck->astLoginEncourageInfo[0]);


    return CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK;
}

UINT16 CGTPktBuilder::OnGetRankAck()
{

    ZERO_PACKET(PKT_CLIGS_GET_RANK_ACK);
    pstAck->wErrCode = CRaceMgr::Instance()->GetSHowRank(pstAck->byShowNum, pstAck->astRankShowInfo);

    return CLIGS_GET_RANK_ACK;
}


UINT16 CGTPktBuilder::OnAcceptTaskAck(CPlayer* poPlayer, UINT8 byTaskLineIdx, UINT32 dwTaskIdx)
{

    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    /*
    ZERO_PACKET(PKT_CLIGS_ACCEPT_TASK_ACK);
    //ZERO_PACKET(PKT_CLIGS_ACCEPT_TASK_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_ACCEPT_TASK_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = CTaskMgr::Instance()->AcceptTask(poPlayer, byTaskLineIdx, byTaskLineIdx);
    //若状态改变，则下发整个任务线给客户端
    if(ERR_ACCEPT_TASK::ID_STATE_NOT_MATCH == pstAck->wErrCode)
    {
        poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList);
    }
    else
    {
        poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    }
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    //当前解锁副本信息
    pstAck->byUnlockNewInstanceFlag = 0;
    if(((0 == pstAck->wErrCode) || (ERR_ACCEPT_TASK::ID_STATE_NOT_MATCH == pstAck->wErrCode)) && (MAIN_LINE_TASK == byTaskLineIdx)) //只有主线任务才解锁新副本
    {
        pstAck->byUnlockNewInstanceFlag = poPlayer->GetMaxUnlockCommonInstanceID(pstAck->astUnlockCommonInstanceID[0], dwTaskIdx);
    	pstAck->byHaveNewInstanceFlag = poPlayer->GetNewCommonInstanceID(pstAck->byUnlockNewInstanceFlag, pstAck->astNewCommonInstanceID[0]);
    }

    */
    return CLIGS_ACCEPT_TASK_ACK;
}


UINT16 CGTPktBuilder::OnGiveUpTaskAck(CPlayer* poPlayer, UINT8 byTaskLineIdx, UINT32 dwTaskIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    /*
    ZERO_PACKET(PKT_CLIGS_GIVEUP_TASK_ACK);
    ZERO_PACKET(PKT_CLIGS_GIVEUP_TASK_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_GIVEUP_TASK_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = CTaskMgr::Instance()->GiveUpTask(poPlayer, byTaskLineIdx, byTaskLineIdx);
    //若状态改变，则下发整个任务线给客户端
    if(ERR_ACCEPT_TASK::ID_STATE_NOT_MATCH == pstAck->wErrCode)
    {
        poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList);
    }
    else
    {
        poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    }
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    */
    return CLIGS_GIVEUP_TASK_ACK;

}


UINT16 CGTPktBuilder::OnGetRaceBattleLogAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GET_RACE_BATTLELOG_ACK);
    CRace* poRace = poPlayer->GetRace();
    if(NULL == poRace)
    {
        pstAck->wErrCode = ERR_GET_RACE_BATTLE_LOG::ID_UNLOCK_RACE_ERR;
        return CLIGS_GET_RACE_BATTLELOG_ACK;
    }
    poRace->GetShowRaceBattleLog(pstAck->byShowNum, pstAck->astRankBattleLogInfo);
    pstAck->wErrCode = ERR_GET_RACE_BATTLE_LOG::ID_SUCCESS;

    return CLIGS_GET_RACE_BATTLELOG_ACK;
}

UINT16 CGTPktBuilder::OnOpenCaptureTabAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_CAPTURE_TAB_ACK);
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_OPEN_CAPTURE_TAB::ID_PLUNDER_UNLOCK;
        return CLIGS_OPEN_CAPTURE_TAB_ACK;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_PLUNDER);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_CAPTURE_TAB::ID_PLUNDER_UNLOCK;
        return CLIGS_OPEN_CAPTURE_TAB_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = poPlunder->OpenCaptureTab(pstAck->stOwner, pstAck->wPlunderNumPerDay, pstAck->wCanPlunderNum, pstAck->wVipExtNum, pstAck->wFreeCanPlunderNum,
                       pstAck->byCaptureNum, pstAck->dwPlunderGold, pstAck->astCaptureInfo, pstAck->awUnlockGridLevel,
                       pstAck->byUnreadPlunderBattleLogNum, pstAck->stLastBattleLog, pstAck->astCaptureGridInfo);
    pstAck->byBleedPerGoldClrMin = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().byBleedPerGoldClrMin;
    pstAck->dwDriveAwayGold = poPlunder->GetDriveAwayGold();
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->dwCurPower = stDT_PLAYER_BASE_DATA.dwPower;

    return CLIGS_OPEN_CAPTURE_TAB_ACK;
}


UINT16 CGTPktBuilder::PlunderAck(UINT16 wErrCode, CPlayer* poMyself, CPlayer* poEnemy, CPlayer* poCapture)
{
    if(NULL == poMyself)
    {
        return INVALID_MSGID;
    }
	CPlayerMedal* poPlayerMedal = poMyself->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->AddArrestNum();
		poPlayerMedal->CheckMedalBySmallType(EMST_ARREST_BLACK);
	}
    CUser* poUser = poMyself->GetUser();
	if ( (poUser) && (poUser->GetCliVer() >= VERSION_PET) )
	{
		return PlunderAck5(wErrCode, poMyself, poEnemy, poCapture);
	}
	else if ( (poUser) && (poUser->GetCliVer() >= VERSION_SOUL))
	{
		return PlunderAck4(wErrCode, poMyself, poEnemy, poCapture);
	}
	else if ((poUser) && (poUser->GetCliVer() > 310))
	{
		return PlunderAck3(wErrCode, poMyself, poEnemy, poCapture);
	}
    
    return PlunderAck5(wErrCode, poMyself, poEnemy, poCapture);
}

UINT16 CGTPktBuilder::PlunderAck5(UINT16 wErrCode, CPlayer* poMyself, CPlayer* poEnemy, CPlayer* poCapture)
{
    if(NULL == poMyself)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_PLUNDER_ACK5);
    CPlunder* poPlunder = poMyself->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_PLUNDER::ID_UNLOCK;
        return CLIGS_PLUNDER_ACK5;
    }
    PKT_CLIGS_PLUNDER_REQ& stReqRaram = poPlunder->GetReqParam(poMyself->GetID());
    pstAck->byResistFlag = stReqRaram.byResistFlag;
    pstAck->byUseItemFlag = stReqRaram.byUseItemFlag;
    pstAck->byCaptureGridIdx = stReqRaram.byCaptureGridIdx;
    if((NULL == poEnemy) || (NULL == poCapture) || (0 != wErrCode))
    {
        if(wErrCode == 0)
        {
            SET_OTHER_ERR(pstAck->wErrCode);
        }
        else
        {
            pstAck->wErrCode = wErrCode;
        }
        return CLIGS_PLUNDER_ACK5;
    }
    //以实际玩家ID为准
    stReqRaram.dwEnemyID = poEnemy->GetID();
    stReqRaram.dwCaptureID = poCapture->GetID();
    pstAck->dwEnemyID = stReqRaram.dwEnemyID;
    pstAck->dwCaptureID = stReqRaram.dwCaptureID;

    //战斗掠夺对象是自己的检查之前已经判断,不应该走到这里，异常
    if((poMyself == poEnemy) || ((poMyself == poCapture) && (0 == stReqRaram.byResistFlag)))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: poActive == poPassive!"), MSG_MARK);
        pstAck->wErrCode = ERR_PLUNDER::ID_ENEMY_CANNOT_MYSELF;
        return CLIGS_PLUNDER_ACK5;
    }
    CPlunder* poEnemyPlunder = poEnemy->GetPlunder();
    CPlunder* poCapturePlunder = poCapture->GetPlunder();
    if((NULL == poEnemyPlunder) || (NULL == poCapturePlunder))
    {
        pstAck->wErrCode = ERR_PLUNDER::ID_ENEMY_UNLOCK;
        return CLIGS_PLUNDER_ACK5;
    }

    //反抗,判断是否被俘虏
    if((1 == stReqRaram.byResistFlag) && (poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID != stReqRaram.dwEnemyID))
    {
        pstAck->wErrCode = ERR_PLUNDER::ID_NOT_BECAPTURE;
        return CLIGS_PLUNDER_ACK5;
    }
    //争夺,判断俘虏是否是战斗者的俘虏
    else if((poEnemy != poCapture) && (0 == stReqRaram.byResistFlag))
    {
        poCapturePlunder->GetBeCaptureInfo(pstAck->stBeCaptureInfo); //先获取被俘信息，同时触发检查释放
        UINT32 dwCaptureOwnerPlayerID = poCapturePlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID;
        if(0 == dwCaptureOwnerPlayerID)
        {
            pstAck->wErrCode = ERR_PLUNDER::ID_NO_OWNER;
            return CLIGS_PLUNDER_ACK5;
        }
        else if((dwCaptureOwnerPlayerID != stReqRaram.dwEnemyID))
        {
            pstAck->wErrCode = ERR_PLUNDER::ID_NOT_THIS_OWNER;
            return CLIGS_PLUNDER_ACK5;
        }
    }
    //掠夺
    else
    {
        //nothing
    }

    //检查掠夺条件
    pstAck->wErrCode = poPlunder->CkContiction(poEnemy, poCapture, stReqRaram);
    if(ERR_PLUNDER::ID_SUCCESS != pstAck->wErrCode)
    {
        return CLIGS_PLUNDER_ACK5;
    }

    if(FALSE == CPlunderMgr::Instance()->Plunder(poMyself, poEnemy, poCapture, stReqRaram, pstAck->stBattleInfo, pstAck->stPlunderEncourage))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_PLUNDER_ACK5;
    }

    CRaceMgr::Instance()->GetChallengePlayerData(poEnemy->GetDT_PLAYER_BASE_DATA().dwID, pstAck->stEnemyPlayerData);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poMyself->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;

    UINT32 dwCaptureOwnerPlayerID = 0;
    poPlunder->GetPlunderData(pstAck->wCanPlunderNum, pstAck->wFreePlunderNum, pstAck->dwPlunderGold, pstAck->wPlunderItemNum, dwCaptureOwnerPlayerID);

    //抓到俘虏，返回俘虏信息
    if((0 == pstAck->stBattleInfo.byBattleResult) && (0 == stReqRaram.byResistFlag))
    {
        pstAck->byCaptureFlag = 1;
        poPlunder->GetCapture(poCapture->GetDT_PLAYER_BASE_DATA().dwID, pstAck->astCaptureInfo[0]);
    }
    //任务信息
    poMyself->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poMyself->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    poMyself->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poMyself->GetPlunderPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_PLUNDER_ACK5;
}

UINT16 CGTPktBuilder::PlunderAck3(UINT16 wErrCode, CPlayer* poMyself, CPlayer* poEnemy, CPlayer* poCapture)
{
    if(NULL == poMyself)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_PLUNDER_ACK3);
    CPlunder* poPlunder = poMyself->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_PLUNDER::ID_UNLOCK;
        return CLIGS_PLUNDER_ACK3;
    }
    PKT_CLIGS_PLUNDER_REQ& stReqRaram = poPlunder->GetReqParam(poMyself->GetID());
    pstAck->byResistFlag = stReqRaram.byResistFlag;
    pstAck->byUseItemFlag = stReqRaram.byUseItemFlag;
    pstAck->byCaptureGridIdx = stReqRaram.byCaptureGridIdx;
    if((NULL == poEnemy) || (NULL == poCapture) || (0 != wErrCode))
    {
        if(wErrCode == 0)
        {
            SET_OTHER_ERR(pstAck->wErrCode);
        }
        else
        {
            pstAck->wErrCode = wErrCode;
        }
        return CLIGS_PLUNDER_ACK3;
    }
    //以实际玩家ID为准
    stReqRaram.dwEnemyID = poEnemy->GetID();
    stReqRaram.dwCaptureID = poCapture->GetID();
    pstAck->dwEnemyID = stReqRaram.dwEnemyID;
    pstAck->dwCaptureID = stReqRaram.dwCaptureID;

    //战斗掠夺对象是自己的检查之前已经判断,不应该走到这里，异常
    if((poMyself == poEnemy) || ((poMyself == poCapture) && (0 == stReqRaram.byResistFlag)))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: poActive == poPassive!"), MSG_MARK);
        pstAck->wErrCode = ERR_PLUNDER::ID_ENEMY_CANNOT_MYSELF;
        return CLIGS_PLUNDER_ACK3;
    }
    CPlunder* poEnemyPlunder = poEnemy->GetPlunder();
    CPlunder* poCapturePlunder = poCapture->GetPlunder();
    if((NULL == poEnemyPlunder) || (NULL == poCapturePlunder))
    {
        pstAck->wErrCode = ERR_PLUNDER::ID_ENEMY_UNLOCK;
        return CLIGS_PLUNDER_ACK3;
    }

    //反抗,判断是否被俘虏
    if((1 == stReqRaram.byResistFlag) && (poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID != stReqRaram.dwEnemyID))
    {
        pstAck->wErrCode = ERR_PLUNDER::ID_NOT_BECAPTURE;
        return CLIGS_PLUNDER_ACK3;
    }
    //争夺,判断俘虏是否是战斗者的俘虏
    else if((poEnemy != poCapture) && (0 == stReqRaram.byResistFlag))
    {
        poCapturePlunder->GetBeCaptureInfo(pstAck->stBeCaptureInfo); //先获取被俘信息，同时触发检查释放
        UINT32 dwCaptureOwnerPlayerID = poCapturePlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID;
        if(0 == dwCaptureOwnerPlayerID)
        {
            pstAck->wErrCode = ERR_PLUNDER::ID_NO_OWNER;
            return CLIGS_PLUNDER_ACK3;
        }
        else if((dwCaptureOwnerPlayerID != stReqRaram.dwEnemyID))
        {
            pstAck->wErrCode = ERR_PLUNDER::ID_NOT_THIS_OWNER;
            return CLIGS_PLUNDER_ACK3;
        }
    }
    //掠夺
    else
    {
        //nothing
    }

    //检查掠夺条件
    pstAck->wErrCode = poPlunder->CkContiction(poEnemy, poCapture, stReqRaram);
    if(ERR_PLUNDER::ID_SUCCESS != pstAck->wErrCode)
    {
        return CLIGS_PLUNDER_ACK3;
    }

    if(FALSE == CPlunderMgr::Instance()->Plunder2(poMyself, poEnemy, poCapture, stReqRaram, pstAck->stBattleInfo, pstAck->stPlunderEncourage))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_PLUNDER_ACK3;
    }

    CRaceMgr::Instance()->GetChallengePlayerData(poEnemy->GetDT_PLAYER_BASE_DATA().dwID, pstAck->stEnemyPlayerData);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poMyself->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;

    UINT32 dwCaptureOwnerPlayerID = 0;
    poPlunder->GetPlunderData(pstAck->wCanPlunderNum, pstAck->wFreePlunderNum, pstAck->dwPlunderGold, pstAck->wPlunderItemNum, dwCaptureOwnerPlayerID);

    //抓到俘虏，返回俘虏信息
    if((0 == pstAck->stBattleInfo.byBattleResult) && (0 == stReqRaram.byResistFlag))
    {
        pstAck->byCaptureFlag = 1;
        poPlunder->GetCapture(poCapture->GetDT_PLAYER_BASE_DATA().dwID, pstAck->astCaptureInfo[0]);
    }
    //任务信息
    poMyself->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poMyself->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    poMyself->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poMyself->GetPlunderPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_PLUNDER_ACK3;
}

UINT16  CGTPktBuilder::PlunderAck4(UINT16 wErrCode, CPlayer* poMyself, CPlayer* poEnemy , CPlayer* poCapture )
{
	if(NULL == poMyself)
	{
		return INVALID_MSGID;
	}


	ZERO_PACKET(PKT_CLIGS_PLUNDER_ACK4);
	CPlunder* poPlunder = poMyself->GetPlunder();
	if(NULL == poPlunder)
	{
		pstAck->wErrCode = ERR_PLUNDER::ID_UNLOCK;
		return CLIGS_PLUNDER_ACK4;
	}
	PKT_CLIGS_PLUNDER_REQ& stReqRaram = poPlunder->GetReqParam(poMyself->GetID());
	pstAck->byResistFlag = stReqRaram.byResistFlag;
	pstAck->byUseItemFlag = stReqRaram.byUseItemFlag;
	pstAck->byCaptureGridIdx = stReqRaram.byCaptureGridIdx;
	if((NULL == poEnemy) || (NULL == poCapture) || (0 != wErrCode))
	{
		if(wErrCode == 0)
		{
			SET_OTHER_ERR(pstAck->wErrCode);
		}
		else
		{
			pstAck->wErrCode = wErrCode;
		}
		return CLIGS_PLUNDER_ACK4;
	}
	//以实际玩家ID为准
	stReqRaram.dwEnemyID = poEnemy->GetID();
	stReqRaram.dwCaptureID = poCapture->GetID();
	pstAck->dwEnemyID = stReqRaram.dwEnemyID;
	pstAck->dwCaptureID = stReqRaram.dwCaptureID;

	//战斗掠夺对象是自己的检查之前已经判断,不应该走到这里，异常
	if((poMyself == poEnemy) || ((poMyself == poCapture) && (0 == stReqRaram.byResistFlag)))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: poActive == poPassive!"), MSG_MARK);
		pstAck->wErrCode = ERR_PLUNDER::ID_ENEMY_CANNOT_MYSELF;
		return CLIGS_PLUNDER_ACK4;
	}
	CPlunder* poEnemyPlunder = poEnemy->GetPlunder();
	CPlunder* poCapturePlunder = poCapture->GetPlunder();
	if((NULL == poEnemyPlunder) || (NULL == poCapturePlunder))
	{
		pstAck->wErrCode = ERR_PLUNDER::ID_ENEMY_UNLOCK;
		return CLIGS_PLUNDER_ACK4;
	}

	//反抗,判断是否被俘虏
	if((1 == stReqRaram.byResistFlag) && (poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID != stReqRaram.dwEnemyID))
	{
		pstAck->wErrCode = ERR_PLUNDER::ID_NOT_BECAPTURE;
		return CLIGS_PLUNDER_ACK4;
	}
	//争夺,判断俘虏是否是战斗者的俘虏
	else if((poEnemy != poCapture) && (0 == stReqRaram.byResistFlag))
	{
		poCapturePlunder->GetBeCaptureInfo(pstAck->stBeCaptureInfo); //先获取被俘信息，同时触发检查释放
		UINT32 dwCaptureOwnerPlayerID = poCapturePlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID;
		if(0 == dwCaptureOwnerPlayerID)
		{
			pstAck->wErrCode = ERR_PLUNDER::ID_NO_OWNER;
			return CLIGS_PLUNDER_ACK4;
		}
		else if((dwCaptureOwnerPlayerID != stReqRaram.dwEnemyID))
		{
			pstAck->wErrCode = ERR_PLUNDER::ID_NOT_THIS_OWNER;
			return CLIGS_PLUNDER_ACK4;
		}
	}
	//掠夺
	else
	{
		//nothing
	}

	//检查掠夺条件
	pstAck->wErrCode = poPlunder->CkContiction(poEnemy, poCapture, stReqRaram);
	if(ERR_PLUNDER::ID_SUCCESS != pstAck->wErrCode)
	{
		return CLIGS_PLUNDER_ACK4;
	}

	if(FALSE == CPlunderMgr::Instance()->Plunder3(poMyself, poEnemy, poCapture, stReqRaram, pstAck->stBattleInfo, pstAck->stPlunderEncourage))
	{
		SET_OTHER_ERR(pstAck->wErrCode);
		return CLIGS_PLUNDER_ACK4;
	}

	CRaceMgr::Instance()->GetChallengePlayerData(poEnemy->GetDT_PLAYER_BASE_DATA().dwID, pstAck->stEnemyPlayerData);
	const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poMyself->GetDT_PLAYER_BASE_DATA();
	pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
	pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
	pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;

	UINT32 dwCaptureOwnerPlayerID = 0;
	poPlunder->GetPlunderData(pstAck->wCanPlunderNum, pstAck->wFreePlunderNum, pstAck->dwPlunderGold, pstAck->wPlunderItemNum, dwCaptureOwnerPlayerID);

	//抓到俘虏，返回俘虏信息
	if((0 == pstAck->stBattleInfo.byBattleResult) && (0 == stReqRaram.byResistFlag))
	{
		pstAck->byCaptureFlag = 1;
		poPlunder->GetCapture(poCapture->GetDT_PLAYER_BASE_DATA().dwID, pstAck->astCaptureInfo[0]);
	}
	//任务信息
	poMyself->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
	poMyself->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

	poMyself->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poMyself->GetPlunderPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

	return CLIGS_PLUNDER_ACK4;
}

UINT16 CGTPktBuilder::OnPlayerUpgrqage(UINT32 dwPlayerID, UINT16 wLevel)
{
    ZERO_PACKET(PKT_GSLS_PLAYER_UPGRADE_NTF);
    pstAck->dwPlayerID = dwPlayerID;
    pstAck->wLevel = wLevel;

    return GSLS_PLAYER_UPGRADE_NTF;
}

UINT16 CGTPktBuilder::OnOpenEnemyTabAck(CPlayer* poPlayer, UINT16 wStartIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_ENEMY_TAB_ACK);
    pstAck->wErrCode = poPlayer->GetEnemyInfo(wStartIdx, pstAck->wTotalNum, pstAck->wCurNum, pstAck->astEnemyInfo);
    pstAck->wStartIdx = wStartIdx;

    return CLIGS_OPEN_ENEMY_TAB_ACK;
}


UINT16 CGTPktBuilder::OnDelEnemyAck(CPlayer* poPlayer, UINT32 dwEnemyID, UINT16 wStartIdx)
{

    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_DEL_ENEMY_ACK);
    pstAck->wErrCode = poPlayer->DelEnemy(dwEnemyID, wStartIdx, pstAck->wTotalNum, pstAck->wCurNum, pstAck->astEnemyInfo);
    pstAck->dwEnemyID = dwEnemyID;

    return CLIGS_DEL_ENEMY_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlunderRecommendPlayerAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK);

    pstAck->wErrCode = CPlunderMgr::Instance()->OpenPlunderScence(poPlayer, pstAck->wCanPlunderNum, pstAck->wFreePlunderNum, pstAck->dwPlunderGold,
                       pstAck->wPlunderItemNum, pstAck->byPlunderPlayerNum, pstAck->astPlunderPlayerInfo);
    CPlunderMgr::Instance()->GetPlunderLevelGapItem(pstAck->byUseItemGapLevel, pstAck->dwPlunderItemPrice, pstAck->wPlunderItemID);


    return CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK;
}


UINT16 CGTPktBuilder::OnRefreshPlunderRecommendAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK);

    pstAck->wErrCode = CPlunderMgr::Instance()->RefreshPlunderRecommend(poPlayer, pstAck->byPlunderPlayerNum, pstAck->astPlunderPlayerInfo);

    return CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK;
}


UINT16 CGTPktBuilder::OnCollectIncomeAck(CPlayer* poPlayer, UINT32  dwCapturePlayerID, UINT32 dwIncomeValue)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_COLLECT_INCOME_ACK);
    pstAck->dwCapturePlayerID = dwCapturePlayerID;
    pstAck->dwIncomeValue = dwIncomeValue;
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_COLLECT_INCOME::ID_PLUNDER_UNLOCK;
        return CLIGS_COLLECT_INCOME_ACK;
    }

    pstAck->wErrCode = poPlunder->CollectIncome(dwCapturePlayerID, dwIncomeValue, pstAck->stWorkInfo, pstAck->byCaptureReleaseFlag, pstAck->byHalved);
    pstAck->dwIncomeValue = dwIncomeValue;//返回最终值
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetCropPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_COLLECT_INCOME_ACK;
}


UINT16 CGTPktBuilder::OnBleedCaptureAck(CPlayer* poPlayer, UINT32 dwCapturePlayerID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_BLEED_CAPTURE_ACK);
    pstAck->dwCapturePlayerID = dwCapturePlayerID;
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_BLEED_CAPTURE::ID_PLUNDER_UNLOCK;
        return CLIGS_BLEED_CAPTURE_ACK;
    }

    pstAck->wErrCode = poPlunder->BleedCapture(dwCapturePlayerID, pstAck->byIncomeType, pstAck->dwIncomeValue);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetPlunderPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_BLEED_CAPTURE_ACK;
}


UINT16 CGTPktBuilder::OnSetWorkTypeAck(CPlayer* poPlayer, UINT32 dwCapturePlayerID, UINT8 byIncomeType)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_SET_WORK_TYPE_ACK);
    pstAck->dwCapturePlayerID = dwCapturePlayerID;
    pstAck->byIncomeType = byIncomeType;
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_SET_WORK_TYPE::ID_PLUNDER_UNLOCK;
        return CLIGS_SET_WORK_TYPE_ACK;
    }

    pstAck->wErrCode = poPlunder->SetWorkType(dwCapturePlayerID, byIncomeType, pstAck->stWorkInfo);

    return CLIGS_SET_WORK_TYPE_ACK;
}

UINT16 CGTPktBuilder::OnDriveAwayAck(CPlayer* poPlayer, UINT16 wErrCode)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_DRIVE_AWAY_OWNER_ACK);
    if(ERR_DRIVE_AWAY_OWNER::ID_SUCCESS != wErrCode)
    {
        pstAck->wErrCode  = wErrCode;
        return CLIGS_DRIVE_AWAY_OWNER_ACK;
    }

    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_DRIVE_AWAY_OWNER::ID_PLUNDER_UNLOCK;
        return CLIGS_DRIVE_AWAY_OWNER_ACK;
    }
	CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->AddDriveNum();
		poPlayerMedal->CheckMedalBySmallType(EMST_DRIVE_BLACK);
	}
    pstAck->wErrCode = poPlunder->DriveAway(pstAck->dwDriveAwayGold);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_DRIVE_AWAY_OWNER_ACK;
}

UINT16 CGTPktBuilder::OnRecvLeftIncomeAck(CPlayer* poPlayer, UINT8 byGridIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_LEFT_INCOME_ACK);
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_RECV_LEFT_INCOME::ID_PLUNDER_UNLOCK;
        return CLIGS_RECV_LEFT_INCOME_ACK;
    }
    pstAck->wErrCode = poPlunder->RecvLeftIncome(byGridIdx, pstAck->byIncomeType, pstAck->dwIncomeValue);
    pstAck->byGridIdx = byGridIdx;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;



    return CLIGS_RECV_LEFT_INCOME_ACK;
}


UINT16 CGTPktBuilder::OnFreeCaptureAck(CPlayer* poPlayer, UINT32 dwCapturePlayerID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_FREE_CAPTURE_ACK);

    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_FREE_CAPTURE::ID_PLUNDER_UNLOCK;
        return CLIGS_FREE_CAPTURE_ACK;
    }

    //DBG_INFO( _SDT( "[%s,%d] 收到释放俘虏请求, 玩家ID:%d" ), MSG_MARK, poPlayer->GetID() );
    pstAck->wErrCode = poPlunder->FreeCapture(dwCapturePlayerID, pstAck->dwCoinIncome, pstAck->dwScienceIncome);
    pstAck->dwCapturePlayerID = dwCapturePlayerID;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    poPlayer->GetPlunderPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_FREE_CAPTURE_ACK;
}


UINT16 CGTPktBuilder::OnOpenMoneyTreeAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_MONEY_TREE_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_MONEYTREE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_MONEY_TREE::ID_MONEY_TREE_UNLOCK;
        return CLIGS_OPEN_MONEY_TREE_ACK;
    }
    poBuild->OnOpen();

    CMoneyTree* poMoneyTree = poPlayer->GetMoneyTree();
    if(NULL == poMoneyTree)
    {
        pstAck->wErrCode = ERR_OPEN_MONEY_TREE::ID_MONEY_TREE_UNLOCK;
        return CLIGS_OPEN_MONEY_TREE_ACK;
    }

    poMoneyTree->GetDT_MONEY_TREE_DATA_CLI(pstAck->stMoneyTreeInfo, pstAck->wGetCoinRate2);
    pstAck->wErrCode = ERR_OPEN_MONEY_TREE::ID_SUCCESS;

    poMoneyTree->GetDT_MONEY_TREE_DATA().byHaveDataFlag = 1;

    return CLIGS_OPEN_MONEY_TREE_ACK;
}


UINT16 CGTPktBuilder::OnRockMoneyTreeAck(CPlayer* poPlayer, UINT8 byGuideFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_ROCK_MONEY_TREE_ACK);

    CMoneyTree* poMoneyTree = poPlayer->GetMoneyTree();
    if(NULL == poMoneyTree)
    {
        pstAck->wErrCode = ERR_ROCK_MONEY_TREE::ID_MONEY_TREE_UNLOCK;
        return CLIGS_ROCK_MONEY_TREE_ACK;
    }
	CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->AddRankMoneyNum();
		poPlayerMedal->CheckMedalBySmallType(EMST_ROCK_MONEY);
	}

    pstAck->wErrCode = poMoneyTree->RockMoney(pstAck->dwGetCoin, byGuideFlag, pstAck->stMoneyTreeInfo, pstAck->wGetCoinRate2);
    pstAck->byGuideFlag = byGuideFlag;
    pstAck->qwPlayerCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwPlayerGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetMoneyTreePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_ROCK_MONEY_TREE_ACK;
}


UINT16 CGTPktBuilder::OnGetPlunderBattleLogAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK);
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        pstAck->wErrCode = ERR_GET_PLUNDER_BATTLE_LOG::ID_PLUNDER_UNLOCK;
        return CLIGS_GET_PLUNDER_BATTLELOG_ACK;
    }
    poPlunder->GetShowPlunderBattleLog(pstAck->byShowNum, pstAck->astBattleLogInfo);
    pstAck->dwCaptureOwnerID = poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID;
    pstAck->wErrCode = ERR_GET_PLUNDER_BATTLE_LOG::ID_SUCCESS;

    return CLIGS_GET_PLUNDER_BATTLELOG_ACK;
}


UINT16 CGTPktBuilder::OnOpenGodweaponAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_GODWEAPON_ACK);
    CBuild* poBuild = poPlayer->GetBuild(EBK_GODWEAPON);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_GODWEAPON::ID_GODWEAPON_UNLOCK;
        return CLIGS_OPEN_GODWEAPON_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CGodweaponMgr::Instance()->OpenGodweapon(poPlayer, pstAck->wGodweaponNum, pstAck->astGodweaponLst,
                       pstAck->wUpgradeQualityItemNum, pstAck->dwUpgradeQualityItemPrice, pstAck->wUpgradeQualityItemID, pstAck->dwActiveCoin, pstAck->aszExtContext);
    pstAck->qwPlayerScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

    pstAck->byPerfectFreeGodweapon = CVipPropMgr::Instance()->CkOpenFunc( EVF_GODWEAPONPERFECTRELEASE, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel );
    pstAck->byOpenNeedVipLevel = CVipPropMgr::Instance()->GetOpenLevel( EVF_GODWEAPONPERFECTRELEASE );
    pstAck->wPerfectFreeGodweaponCost = CSinglePramMgr::Instance()->GetPerfectFreeGodweaponGold();
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    return CLIGS_OPEN_GODWEAPON_ACK;
}

//
UINT16 CGTPktBuilder::OnUpgradeGodweaponAck(CPlayer* poPlayer, UINT16 wHeroKindID,	UINT16 wQuality, UINT16 wLevel)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_UPGRADE_GODWEAPON_ACK);
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CGodweaponMgr::Instance()->UpgradeGodweapon(poPlayer, wHeroKindID, wQuality, wLevel, pstAck->stGodweaponInfo, pstAck->wUpgradeQualityItemNum);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->qwPlayerScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_UPGRADE_GODWEAPON_ACK;
}


UINT16 CGTPktBuilder::OnFreeGodweaponAck(CPlayer* poPlayer, UINT16 wHeroKindID, BOOL bUseGold)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    DBG_INFO(_SDT("[%s: %d]: PlayerID:%u Free Godweapon! HeroKindID:%u, UseGold:%u"), MSG_MARK, poPlayer->GetID(), wHeroKindID, bUseGold);



    ZERO_PACKET(PKT_CLIGS_FREE_GODWEAPON_ACK);
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CGodweaponMgr::Instance()->FreeGodweapon(poPlayer, wHeroKindID, pstAck->stGodweaponInfo, bUseGold);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->qwPlayerScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_FREE_GODWEAPON_ACK;
}

/*
UINT16 CGTPktBuilder::OnOpenAwakenAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_OPEN_AWAKEN_ACK);
    //ZERO_PACKET(PKT_CLIGS_OPEN_AWAKEN_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_OPEN_AWAKEN_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_AWAKEN_ACK;
    }

    pstAck->wErrCode = poAwaken->GetAwakenCli(pstAck->stAwakenInfo, pstAck->dwAwakenCoin, pstAck->abyGridStudyClipColor, pstAck->awPickupStudyClipNum);

    return CLIGS_OPEN_AWAKEN_ACK;
}

UINT16 CGTPktBuilder::OnAwakenAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_AWAKEN_ACK);
    //ZERO_PACKET(PKT_CLIGS_AWAKEN_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_AWAKEN_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_AWAKEN_ACK;
    }

    pstAck->wErrCode = poAwaken->Awaken(pstAck->byAwakenLevel, pstAck->dwAwakenCoin, pstAck->byStudyColor, pstAck->dwStudyExp);
    pstAck->qwPlayerCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    return CLIGS_AWAKEN_ACK;
}

UINT16 CGTPktBuilder::OnPickupStudyClipAck(CPlayer* poPlayer)
{

    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_PICKUP_STUDY_CLIP_ACK);
    //ZERO_PACKET(PKT_CLIGS_PICKUP_STUDY_CLIP_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_PICKUP_STUDY_CLIP_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_PICKUP_STUDY_CLIP::ID_AWAKEN_UNLOCK;
        return CLIGS_PICKUP_STUDY_CLIP_ACK;
    }

    pstAck->wErrCode = poAwaken->PickupStudyClip(pstAck->qwStudyExp, pstAck->awPickupStudyClipNum);

    return CLIGS_PICKUP_STUDY_CLIP_ACK;
}


UINT16 CGTPktBuilder::OnOpenStudyAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_OPEN_STUDY_ACK);
    //ZERO_PACKET(PKT_CLIGS_OPEN_STUDY_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_OPEN_STUDY_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_STUDY_ACK;
    }

    pstAck->wErrCode = poAwaken->OpenStudy(pstAck->qwStudyExp, pstAck->awPickupStudyClipNum, pstAck->stStoreStudyInfo, pstAck->stHeroStudyInfo);

    return CLIGS_OPEN_STUDY_ACK;
}


UINT16 CGTPktBuilder::OnDressStudyAck(CPlayer* poPlayer, UINT8 byOpPos, UINT16 wHeroKindID, UINT8 byDressType, UINT8 bySrcPos,	UINT8 byDstPos)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_DRESS_STUDY_ACK);
    //ZERO_PACKET(PKT_CLIGS_DRESS_STUDY_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_DRESS_STUDY_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_DRESS_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_DRESS_STUDY_ACK;
    }

    pstAck->wErrCode = poAwaken->DressStudy(byOpPos, wHeroKindID, byDressType, bySrcPos, byDstPos);
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byDressType = byDressType;
    pstAck->bySrcPos = bySrcPos;
    pstAck->byDstPos = byDstPos;

    return CLIGS_DRESS_STUDY_ACK;
}

UINT16 CGTPktBuilder::OnStudyClip2ExpAck(CPlayer* poPlayer, UINT8 byColor, UINT16 wNum)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK);
    //ZERO_PACKET(PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_CLIP_TO_EXP::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_CLIP_TO_EXP_ACK;
    }

    pstAck->wErrCode = poAwaken->StudyClip2Exp(byColor, wNum, pstAck->qwStudyExp, pstAck->awPickupStudyClipNum);

    return CLIGS_STUDY_CLIP_TO_EXP_ACK;
}

UINT16 CGTPktBuilder::OnStudy2ExpAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byPosIdx)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_STUDY_TO_EXP_ACK);
    //ZERO_PACKET(PKT_CLIGS_STUDY_TO_EXP_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_STUDY_TO_EXP_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_TO_EXP::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_TO_EXP_ACK;
    }
    pstAck->wErrCode = poAwaken->Study2Exp(wHeroKindID, byPosIdx, pstAck->qwStudyExp, pstAck->awPickupStudyClipNum, pstAck->byHeroStudyFlag, pstAck->astHeroStudyInfo[0]);
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byPosIdx = byPosIdx;

    return CLIGS_STUDY_TO_EXP_ACK;
}

UINT16 CGTPktBuilder::OnOpenStudyConvertAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_OPEN_STUDY_CONVERT_ACK);
    //ZERO_PACKET(PKT_CLIGS_OPEN_STUDY_CONVERT_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_OPEN_STUDY_CONVERT_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_STUDY_CONVERT::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_STUDY_CONVERT_ACK;
    }
    pstAck->wErrCode = poAwaken->OpenStudyConvert(pstAck->astStudyCfgInfo);

    return CLIGS_OPEN_STUDY_CONVERT_ACK;
}

UINT16 CGTPktBuilder::OnStudyConvertAck(CPlayer* poPlayer, UINT8 byStudyKind, UINT8 byStudyColor)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_STUDY_CONVERT_ACK);
    //ZERO_PACKET(PKT_CLIGS_STUDY_CONVERT_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_STUDY_CONVERT_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_CONVERT::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_CONVERT_ACK;
    }
    pstAck->wErrCode = poAwaken->StudyConvert(byStudyKind, byStudyColor);

    return CLIGS_STUDY_CONVERT_ACK;
}


UINT16 CGTPktBuilder::OnStudyClipAuto2ExpAck(CPlayer* poPlayer, UINT8 bySellectFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK);
    //ZERO_PACKET(PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_CLIP_AUTO2EXP::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_CLIP_AUTO2EXP_ACK;
    }
    pstAck->wErrCode = poAwaken->StudyClipAuto2Exp(bySellectFlag);
    pstAck->bySellectFlag = bySellectFlag;

    return CLIGS_STUDY_CLIP_AUTO2EXP_ACK;
}

UINT16 CGTPktBuilder::OnStudyUpgradeAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byPos, UINT8 byLevel)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	ZERO_PACKET(PKT_CLIGS_STUDY_UPGRADE_ACK);
    //ZERO_PACKET(PKT_CLIGS_STUDY_UPGRADE_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_STUDY_UPGRADE_ACK*)(GetPacketBuffer());
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_UPGRADE::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_UPGRADE_ACK;
    }
    pstAck->wErrCode = poAwaken->StudyUpgrade(wHeroKindID, byPos, byLevel, pstAck->stHeroStudyInfo);
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byPos = byPos;

    return CLIGS_STUDY_UPGRADE_ACK;
}

*/

UINT16 CGTPktBuilder::OnVisitHeroAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 bySizeFlag, UINT8 byGoldFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_VISIT_HERO_ACK);
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->wErrCode = poPlayer->Visit(wHeroKindID, bySizeFlag, byGoldFlag, pstAck->byWinFlag, pstAck->byNeedVisitNum, pstAck->byCurVisitNum, pstAck->wUpgradeVisitNum, pstAck->dwReturnCoin);
    pstAck->qwPlayerCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwPlayerGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_VISIT_HERO_ACK;
}

//2012-12-07 体力购买
UINT16 CGTPktBuilder::OnBuyPhystrengthAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_BUY_PHYSTRENGTH_ACK);

    pstAck->wErrCode = poPlayer->GetPhystrength().BuyPhyStrength(pstAck->byCanBuyNum, pstAck->dwNextBuyGold);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->wCurPhyStrength = poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength;
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stCurPhyStrengthInfo);

    return CLIGS_BUY_PHYSTRENGTH_ACK;
}




UINT16 CGTPktBuilder::OnOpenMaintownAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_MAINTOWN_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_MAINTOWN);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_MAINTOWN::ID_BUILD_NOT_EXIST;
        return CLIGS_OPEN_MAINTOWN_ACK;
    }
    poBuild->OnOpen();
    CBuild* poBuildLevy = poPlayer->GetBuild(EBK_LEVY);
    if(poBuildLevy)
    {
        poBuildLevy->OnOpen();
    }

    pstAck->wErrCode = poPlayer->OpenMainTown(pstAck->wCurLevel, pstAck->byCanCrowFlag, pstAck->dwCrownCoin, pstAck->dwCrownGold,
                       pstAck->dwCrownJingJie, pstAck->dwLevyCountDown, pstAck->dwLevyCoin, pstAck->dwLevyJingJie, pstAck->byLevyNum, pstAck->byTotalLevyNum);
    poPlayer->GetMaintownPrompt1(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetMaintownPrompt2(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    const DT_PLAYER_BASE_DATA& stBaseData = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stBaseData.qwCoin;
    pstAck->qwCurGold = stBaseData.qwGold;
    pstAck->qwCurJingJie = stBaseData.qwJingJie;

    return CLIGS_OPEN_MAINTOWN_ACK;
}

UINT16 CGTPktBuilder::OnRecvIncomeAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_MAINTOWN_INCOME_ACK);
    pstAck->wErrCode = poPlayer->Crown(pstAck->byCanCrowFlag);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    poPlayer->GetMaintownPrompt1(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_RECV_MAINTOWN_INCOME_ACK;
}

UINT16 CGTPktBuilder::OnLevyIncomeAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK);


    pstAck->wErrCode = poPlayer->Levy(pstAck->dwLevyCountDown, pstAck->byLevyNum);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;

    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    poPlayer->GetMaintownPrompt2(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_LEVY_MAINTOWN_INCOME_ACK;
}


//2012-11-28装备合成请求
UINT16 CGTPktBuilder::OnComposeEquipAck(CPlayer* poPlayer, UINT16 wEquipItemID, UINT16 wEquipIdx, UINT8 byGoldComposeFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_COMPOSE_EQUIP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wSrcEquipItemID = wEquipItemID;
    pstAck->wSrcEquipIdx = wEquipIdx;
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CItemMakeMgr::Instance()->ComposeEquip(poPlayer, wEquipItemID, wEquipIdx, byGoldComposeFlag, pstAck);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    return CLIGS_COMPOSE_EQUIP_ACK;
}

//2012-12-07 打开体力购买面板
UINT16 CGTPktBuilder::OnOpenPhyStrengthAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_PHYSTRENGTH_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = 0;
    pstAck->wErrCode = poPlayer->GetPhystrength().OpenPhyStrength(pstAck->byStrength, pstAck->byTotalBuyTimes, pstAck->byBuyTimes, pstAck->wVipExtNum, pstAck->dwBuyGold);
    CEatBread *poEatBread  = poPlayer->GetEatBread();
    if (poEatBread)
    {
        pstAck->wErrCode = poEatBread->GetPhystrengthActivityData(pstAck->byActivityNum, pstAck->astActivityInfo);
    }

    return CLIGS_OPEN_PHYSTRENGTH_ACK;
}

UINT16 CGTPktBuilder::OnClrStrengthenCDAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLR_STRENGTHEN_CD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = poPlayer->ClrStrengthenCD();
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_CLR_STRENGTHEN_CD_ACK;
}

UINT16	CGTPktBuilder::OnOpenCropReq(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_CROP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_CROP);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_CROP::ID_CROP_IS_NOT_OPEN;
        return CLIGS_OPEN_CROP_ACK;
    }
    poBuild->OnOpen();

    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL == poCrop)
    {
        pstAck->wErrCode = ERR_OPEN_CROP::ID_CROP_IS_NOT_OPEN;
        return CLIGS_OPEN_CROP_ACK;
    }

    pstAck->wErrCode = ERR_OPEN_CROP::ID_SUCCESS;
    poCrop->GetDT_CROP_SEED_INFO_DATA(pstAck->astCropSeedInfoList, pstAck->byCropSeedNum);
    poCrop->GetDT_CROP_FIELD_INFO_DATA(pstAck->astCropFieldInfoList);
    poCrop->GetDT_CROP_MORE_INFO_DATA(pstAck->astCropMoreInfoList);
    pstAck->wNoCDGoldPerMin = CSinglePramMgr::Instance()->GetGainCropCDGoldPerMin();
    pstAck->byBagFullFlag = poPlayer->GetBagFullFlag();
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
    pstAck->qwJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    poPlayer->GetCropPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_OPEN_CROP_ACK;
}

UINT16	CGTPktBuilder::OnOpenCropFieldReq(CPlayer* poPlayer, UINT8 byFieldID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_CROP_FIELD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL == poCrop)
    {
        pstAck->wErrCode = ERR_CROP_FIELD::ID_CROP_IS_NOT_OPEN;
        return CLIGS_OPEN_CROP_FIELD_ACK;
    }

    pstAck->wErrCode = poCrop->OpenCropField(byFieldID);
    pstAck->byFieldID = byFieldID;
    pstAck->qwCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin; //铜钱
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold; //元宝
    poCrop->GetDT_CROP_FIELD_INFO_DATA(byFieldID, pstAck->stCropFieldInfo);
    poPlayer->GetCropPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_OPEN_CROP_FIELD_ACK;
}


UINT16	CGTPktBuilder::OnCropReq(CPlayer* poPlayer, UINT8 byFieldID, UINT16 wCropKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CROP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_CROP);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_CROP::ID_CROP_IS_NOT_OPEN;
        return CLIGS_CROP_ACK;
    }
    poBuild->OnOpen();

    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL == poCrop)
    {
        pstAck->wErrCode = ERR_CROP::ID_CROP_IS_NOT_OPEN;
        return CLIGS_CROP_ACK;
    }

    pstAck->byFieldID = byFieldID;
    pstAck->wErrCode = poCrop->Crop(byFieldID, wCropKindID);
    poCrop->GetDT_CROP_FIELD_INFO_DATA(byFieldID, pstAck->stCropFieldInfo);
    poCrop->GetDT_CROP_MORE_INFO_DATA(byFieldID, pstAck->stCropMoreInfo);
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetCropPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_CROP_ACK;
}

UINT16	CGTPktBuilder::OnGainCropReq(CPlayer* poPlayer, UINT8 byFieldID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GAIN_CROP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL == poCrop)
    {
        pstAck->wErrCode = ERR_GAIN_CROP::ID_CROP_IS_NOT_OPEN;
        return CLIGS_CROP_ACK;
    }

    pstAck->wErrCode = poCrop->GainCrop(byFieldID, pstAck->byCropType, pstAck->dwCropValue, pstAck->wCropKindID);
    pstAck->qwCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin; //铜钱
    pstAck->qwScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
    pstAck->qwJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poCrop->GetDT_CROP_FIELD_INFO_DATA(byFieldID, pstAck->stCropFieldInfo);
    poCrop->GetDT_CROP_MORE_INFO_DATA(byFieldID, pstAck->stCropMoreInfo);
    poPlayer->GetCropPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetSciencePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_GAIN_CROP_ACK;
}

UINT16	CGTPktBuilder::OnClrCropReq(CPlayer* poPlayer, UINT8 byFieldID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLR_CROP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL == poCrop)
    {
        pstAck->wErrCode = ERR_CLR_CROP::ID_CROP_IS_NOT_OPEN;
        return CLIGS_CLR_CROP_ACK;
    }
    pstAck->wErrCode = poCrop->ClrCrop(byFieldID);
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold; //元宝
    poPlayer->GetCropPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_CLR_CROP_ACK;
}

UINT16   CGTPktBuilder::OnOpenExperienceDrugReq(CPlayer* poPlayer, UINT16 wHeroID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_EXPERIENCE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_EXPRIENCE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_ECPERIENCE_DRUG::ID_ECPERIENCE_UNLOCK;
        return CLIGS_OPEN_EXPERIENCE_ACK;
    }
    poBuild->OnOpen();

    CHero* poHero = poPlayer->GetHero(wHeroID);
    if(NULL == poHero)
    {
        pstAck->wErrCode = ERR_OPEN_ECPERIENCE_DRUG::ID_HERO_NOT_EXIT;
        return CLIGS_OPEN_EXPERIENCE_ACK;
    }

    //检测武将类型是否为主武将
    if (poPlayer->GetCoachHero() == poHero || 1 == poHero->GetDT_HERO_BASE_DATA().byCoach)
    {
        pstAck->wErrCode = ERR_OPEN_ECPERIENCE_DRUG::ID_HERO_IS_COACH;
        return CLIGS_OPEN_EXPERIENCE_ACK;
    }

    poPlayer->GetVipExtData().Check4ResetExperience();

    pstAck->wLevel = poHero->GetDT_HERO_BASE_DATA().wLevel;
    poHero->GetExperience(pstAck->dwExperience, pstAck->dwUpgradeExperience);

    //获取经验丹的相关信息
    pstAck->wErrCode = CDrugMgr::Instance()->GetDT_EXPERIENCE_DRUG_DATA(poPlayer, pstAck->astExperienceDrugDataList);

    if( pstAck->wErrCode != ERR_OPEN_ECPERIENCE_DRUG::ID_SUCCESS )
    {
        return CLIGS_OPEN_EXPERIENCE_ACK;
    }

    pstAck->wErrCode = CDrugMgr::Instance()->GetDT_GOLD_EXPERIENCE_DRUG_DATA( poPlayer, pstAck->stGoldExperienceDrugData );


    return CLIGS_OPEN_EXPERIENCE_ACK;
}


UINT16   CGTPktBuilder::OnUserExperienceDrugReq(CPlayer* poPlayer, UINT16 wHeroID, UINT16 wExperienceDrugID, UINT16 wGoldExperienceRemainCount)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_USER_EXPERIENCE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CHero* poHero = poPlayer->GetHero(wHeroID);
    if(NULL == poHero)
    {
        pstAck->wErrCode =  ERR_USER_ECPERIENCE_DRUG::ID_HERO_NOT_EXIT;
        return CLIGS_USER_EXPERIENCE_ACK;
    }

    pstAck->stPowerChangeData.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CDrugMgr::Instance()->UseExperienceDrug(poPlayer, wHeroID, wExperienceDrugID, wGoldExperienceRemainCount);//使用经验丹
    pstAck->stPowerChangeData.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    CDrugMgr::Instance()->GetDT_GOLD_EXPERIENCE_DRUG_DATA(poPlayer, pstAck->stGoldExperienceDrugData);

    CDrugMgr::Instance()->GetDT_EXPERIENCE_DRUG_DATA(poPlayer, wExperienceDrugID, pstAck->stExperienceDrugData);
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(pstAck->stHeroBaseDataCli, FALSE);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_USER_EXPERIENCE_ACK;
}

UINT16	CGTPktBuilder::OnOpenScienceBuildReq(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_OPEN_SCIENCE_BUILD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_SCIENCE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SCIENCE::ID_SCIENCE_UNLOCK;
        return CLIGS_OPEN_SCIENCE_BUILD_ACK;
    }
    poBuild->OnOpen();


    CScienceTree* poScienceTree = poPlayer->GetScienceTree();
    if(NULL == poScienceTree)
    {
        pstAck->wErrCode = ERR_OPEN_SCIENCE::ID_SCIENCE_UNLOCK;
        return CLIGS_OPEN_SCIENCE_BUILD_ACK;
    }

    pstAck->wErrCode = poScienceTree->GetScienceTreeBuildInfo( pstAck->qwCurStory, pstAck->byAlreadyOpenScienceNum,
                       pstAck->astAlreadyOpenScienceList, pstAck->byWillOpenScienceNum, pstAck->astWillOpenScienceList );
    //DBG_INFO( _SDT( "[%s: %d]:返回的当前阅历值为:%d" ), MSG_MARK, pstAck->qwCurStory );
    return CLIGS_OPEN_SCIENCE_BUILD_ACK;
}

UINT16	CGTPktBuilder::OnUpdateScienceReq(CPlayer* poPlayer, UINT8 byKind, UINT16 wCurLevel)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_UPDATE_SCIENCE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    //如果科技建筑本身未解锁，则不能升级
    if( NULL == poPlayer->GetBuild( EBK_SCIENCE))
    {
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_ATTR_NOT_UNLOCK_ERR;
    }

    CScienceTree* poScienceTree = poPlayer->GetScienceTree();
    if(NULL == poScienceTree)
    {
        pstAck->wErrorCode = ERR_UPGRADE_SCIENCE_ATTREXT::ID_ATTR_NOT_UNLOCK_ERR;
        return CLIGS_UPDATE_SCIENCE_ACK;
    }

    pstAck->wErrorCode = poScienceTree->UpdateScience( byKind, wCurLevel, pstAck->stUpdatedData,
                         pstAck->stPower, pstAck->qwCurStory );
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetSciencePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    //DBG_INFO( _SDT( "[%s: %d]:返回的当前阅历值为:%d" ), MSG_MARK, pstAck->qwCurStory );
    return CLIGS_UPDATE_SCIENCE_ACK;
}

//熔合器魂
UINT16  CGTPktBuilder::OnOpenMeltEquipReq(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_OPEN_MELT_EQUIP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_MELTING);
    if(NULL == poBuild)
    {
        pstAck->wErrorCode = ERR_OPEN_MELT_EQUIP::ID_BUILD_NOT_OPEN;
        return CLIGS_OPEN_MELT_EQUIP_ACK;
    }
    poBuild->OnOpen();

    //玩家背包中空闲装备
    poPlayer->GetBag().GetDT_EQUIP_MELT_DATA_LST_CLI(pstAck->stEquipInfo);
   // poPlayer->GetBag().GetDT_GOOD_MELT_DATA_LST_CLI(pstAck->stGoodInfo);
    pstAck->wErrorCode = ERR_OPEN_MELT_EQUIP::ID_SUCCESS;
    return CLIGS_OPEN_MELT_EQUIP_ACK;
}

//熔炼器魂
UINT16  CGTPktBuilder::OnMeltReq(CPlayer* poPlayer, UINT8 byGoldFlag, UINT16 wEquipNum, DT_MELT_EQUIP_DATA stDT_MELT_EQUIP_DATA[MAX_BAG_OPEN_NUM], UINT16 wGoodNum, DT_MELT_GOOD_DATA astMeltGoodDataList[MAX_BAG_OPEN_NUM])
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    for (int i = 0; i < wEquipNum; i++)
    {
        DBG_INFO(_SDT("[%s: %d]: Melt Equip Req PlayerID: %u, KindID:%u Idx:%u!"), MSG_MARK, poPlayer->GetID(), stDT_MELT_EQUIP_DATA[i].wEquipID, stDT_MELT_EQUIP_DATA[i].wIdx);
    }

    for (int i = 0; i < wGoodNum; i++)
    {
        DBG_INFO(_SDT("[%s: %d]: Melt Good Req PlayerID: %u, KindID:%u Num:%u!"), MSG_MARK, poPlayer->GetID(), astMeltGoodDataList[i].wKindID, astMeltGoodDataList[i].wNum);
    }

    ZERO_PACKET(PKT_CLIGS_MELT_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    UINT64	qwOldScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    CBuild*	poBuild = poPlayer->GetBuild(EBK_MELTING);
    if(NULL == poBuild)
    {
        pstAck->wErrorCode = ERR_MELT_EQUIP::ID_BUILD_NOT_OPEN;
        return CLIGS_MELT_ACK;
    }

    if((byGoldFlag) && (!CVipPropMgr::Instance()->CkOpenFunc(EVF_GOLDMELTEQUIP, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel)))
    {
        pstAck->wErrorCode = ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
        return CLIGS_MELT_ACK;
    }

    //首先检测装备是否存在， 以及元宝的花费。
    UINT32 dwCostGold = 0;
    for (int i = 0; i < wEquipNum; i++)
    {
        UINT32 dwTmp = 0;
        pstAck->wErrorCode = CItemMgr::Instance()->CheckMeltEquip(poPlayer, stDT_MELT_EQUIP_DATA[i].wEquipID, stDT_MELT_EQUIP_DATA[i].wIdx, dwTmp);
        if(ERR_MELT_EQUIP::ID_SUCCESS != pstAck->wErrorCode)
        {
            return CLIGS_MELT_ACK;
            break;
        }
        dwCostGold += dwTmp;
    }

    for (int i = 0; i < wGoodNum; i++)
    {
        UINT32 dwTmp = 0;
        pstAck->wErrorCode = CItemMgr::Instance()->CheckMeltGood(poPlayer, astMeltGoodDataList[i].wKindID, astMeltGoodDataList[i].wNum, dwTmp);
        if(ERR_MELT_EQUIP::ID_SUCCESS != pstAck->wErrorCode)
        {
            return CLIGS_MELT_ACK;
            break;
        }
        dwCostGold += dwTmp;
    }

    if((byGoldFlag) && (poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold))
    {
        pstAck->wErrorCode = ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        return CLIGS_MELT_ACK;
    }

    for (int i = 0; i < wEquipNum; i++)
    {
        pstAck->wErrorCode = CItemMgr::Instance()->MeltEquip(poPlayer, byGoldFlag, stDT_MELT_EQUIP_DATA[i].wEquipID, stDT_MELT_EQUIP_DATA[i].wIdx);
        if(ERR_MELT_EQUIP::ID_SUCCESS != pstAck->wErrorCode)
        {
            break;
        }
    }

    for (int i = 0; i < wGoodNum; i++)
    {
        pstAck->wErrorCode = CItemMgr::Instance()->MeltGood(poPlayer, byGoldFlag, astMeltGoodDataList[i].wKindID, astMeltGoodDataList[i].wNum);
        if(ERR_MELT_EQUIP::ID_SUCCESS != pstAck->wErrorCode)
        {
            break;
        }
    }

    if(byGoldFlag)
    {
        poPlayer->DecGold(dwCostGold, CRecordMgr::EDGT_MELT);
    }

    //器魂数
    if (poPlayer->GetDT_PLAYER_BASE_DATA().qwScience > qwOldScience)
    {
        pstAck->dwAddScience = (UINT32)(poPlayer->GetDT_PLAYER_BASE_DATA().qwScience - qwOldScience);
    }
    else
    {
        pstAck->dwAddScience = 0;
    }

    pstAck->qwScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    //玩家背包中空闲装备
    poPlayer->GetBag().GetDT_EQUIP_MELT_DATA_LST_CLI(pstAck->stEquipInfo);
	//不支持装备碎片
   // poPlayer->GetBag().GetDT_GOOD_MELT_DATA_LST_CLI(pstAck->stGoodInfo);
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_MELT_ACK;

}

UINT16	CGTPktBuilder::OnOpenUpdateAttrPannelReq( CPlayer* poPlayer, UINT16 wKind, UINT8 byAttrKind )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_UPDATEATTR);
    if(NULL == poBuild)
    {
        pstAck->wErrorCode = ERR_OPEN_UPDATE_ATTR_PANNEL::ID_ATTR_UPDATE_ATTR_UNLOCK;
        return CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK;
    }
    poBuild->OnOpen();

    DT_UPDATE_ATTR_DATA oData;
    pstAck->wErrorCode = poPlayer->GetUpdateAttrInfo( wKind, byAttrKind, oData );

    pstAck->stUpdateAttr = oData;
    if(poPlayer->CKGuide( EGF_UPDATEATTR))
    {
        //引导消耗0
        pstAck->stUpdateAttr.dwCostCoin = 0;
    }

    return CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK;
}

UINT16	CGTPktBuilder::OnUpdateAttrReq( CPlayer* poPlayer, UINT16 wHeroID, UINT8 byAttrKind, UINT8 byUpdateKind, UINT16 wCurCount )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_UPDATE_ATTR_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    DT_UPDATE_ATTR_DATA oData;
    DT_UPDATED_ATTR_BATTLE_DATA oBattleData;
    //DBG_INFO( _SDT("请求提升属性:%d"), byAttrKind );
    pstAck->wErrorCode = poPlayer->UpdateAttr( wHeroID, byAttrKind, byUpdateKind, wCurCount, oData, oBattleData );

    pstAck->wKindID = wHeroID;
    pstAck->stAttrRet = oData;
    pstAck->stBattleInfo = oBattleData;
    //DBG_INFO( _SDT("返回属性:%d"), pstAck->stAttrRet.byAttrKind );
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    return CLIGS_UPDATE_ATTR_ACK;
}


UINT16	CGTPktBuilder::OnComposeReq( CPlayer* poPlayer, UINT16 wItemKindID, UINT8 byFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_COMPOSE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CItemMakeMgr::Instance()->ComposeGoods(poPlayer, wItemKindID, pstAck->stItemInfo);
    if ( poPlayer->GetCliVer() < VER_PET ) // 旧版本才需要重新下发背包信息，新版本就不用了
    {
        if( byFlag == CItemMakeMgr::ms_nXiangqianTabCompose )//镶嵌面板 
        {
            poPlayer->GetBag().GetXiangqianGemList( pstAck->stGoodsBagItemInfo );
            poPlayer->GetBag().GetXiangqianGemList( pstAck->stGoodsBagItemInfo2 );
        }
        else if ( byFlag == 2)//人物面板，装备背包
        {
            poPlayer->GetBag().GetEquipByPlayerPanel(pstAck->stGoodsBagItemInfo2);
        }
        else if ( byFlag == 3)
        {
            poPlayer->GetBag().GetAboutJewelryLstCli(pstAck->stGoodsBagItemInfo2);
        }
        else 
        {
            poPlayer->GetBag().GetBagItem(pstAck->stGoodsBagItemInfo);
            poPlayer->GetBag().GetBagItem(pstAck->stGoodsBagItemInfo2);
        }
    }else{ // 新版本则下发各个道具的当前数量
        poPlayer->GetBag().GetBagItemNum(pstAck->stBagItemNumInfo);
    }

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);

    return CLIGS_COMPOSE_ACK;
}


UINT16	CGTPktBuilder::OnGetItemReq( CPlayer* poPlayer, UINT16 wItemKindID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GET_ITEM_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wItemKindID = wItemKindID;
    CGoods* poGoods = poPlayer->GetBag().GetGoodsItem(wItemKindID);
    if(NULL == poGoods)
    {
        pstAck->wItemNum = 0;
    }
    else
    {
        pstAck->wItemNum = poGoods->GetCurPileNum();
    }
    pstAck->byshop = CShopMgr::Instance()->GetTabID(wItemKindID, pstAck->byTab);
    DT_INSTANCE_ID* pstInstanceID = CInstancePropMgr::Instance()->GetBurstItemInstancdID(wItemKindID);
    //副本不一定有爆出该物品
    if(NULL != pstInstanceID)
    {
        pstAck->stBurstInstance.wSceneIdx = pstInstanceID->wSceneIdx;
        pstAck->stBurstInstance.byTownIdx = pstInstanceID->byTownIdx;
        pstAck->stBurstInstance.byInstanceIdx = pstInstanceID->byInstanceIdx;
    }
    else
    {
        pstAck->stBurstInstance.wSceneIdx = 0;
        pstAck->stBurstInstance.byTownIdx = 0;
        pstAck->stBurstInstance.byInstanceIdx = 0;
    }
    pstAck->wErrCode = ERR_GET_ITEM::ID_SUCCESS;
    return CLIGS_GET_ITEM_ACK;
}


UINT16	CGTPktBuilder::OnGenOrderIDReqError( UINT16 wErrorCode )
{


    ZERO_PACKET(PKT_CLIGS_GET_ORDER_ID_ACK);
    pstAck->wErrCode = wErrorCode;

    return CLIGS_GET_ORDER_ID_ACK;
}

//
UINT16	CGTPktBuilder::OnComposeSeedReq( CPlayer* poPlayer, UINT16 wItemKindID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_SEED_COMPOSE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = CItemMakeMgr::Instance()->ComposeSeed(poPlayer, wItemKindID, pstAck->stItemInfo);
    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL != poCrop)
    {
        poCrop->GetDT_CROP_SEED_INFO_DATA(pstAck->astCropSeedInfoList, pstAck->byCropSeedNum);
    }

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    return CLIGS_SEED_COMPOSE_ACK;
}


UINT16	CGTPktBuilder::OnChatReq( CPlayer* poPlayer, UINT32 dwPlayerID, UINT8 byChatType, UINT8 abyChatContent[MAX_CHAT_CONTENT_NUM], UINT32 dwChatLen)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CHAT_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    //检测用户ID是否正确
    if (poPlayer->GetDT_PLAYER_BASE_DATA().dwID != dwPlayerID)
    {
        pstAck->wErrCode = ERR_CHAT::ID_CHAT_PLAYERID_ERROR;
        return CLIGS_CHAT_ACK;
    }

    //检测用户聊天类型
    if (ECT_USER != byChatType && ECT_FACTION != byChatType)
    {
        pstAck->wErrCode = ERR_CHAT::ID_CHAT_TYPE_ERROR;
        return CLIGS_CHAT_ACK;
    }

    //检测用户聊天数据
    if (dwChatLen == 0)
    {
        pstAck->wErrCode = ERR_CHAT::ID_CHAT_EMPTY;
        return CLIGS_CHAT_ACK;
    }

    if(dwChatLen > MAX_CHAT_CONTENT_NUM)
    {
        pstAck->wErrCode = ERR_CHAT::ID_CHAT_DATA_ERROR;
        return CLIGS_CHAT_ACK;
    }

    CHAT_DATA stCHAT_DATA;
    memset(&stCHAT_DATA, 0x00, sizeof(stCHAT_DATA));

    stCHAT_DATA.dwSerialNumber = CChatMgr::Instance()->AllocChatSerialNum();
    stCHAT_DATA.dwPlayerID     = dwPlayerID;
    stCHAT_DATA.byChatType     = byChatType;
    stCHAT_DATA.qwChatTime     = SDTimeSecs();
    stCHAT_DATA.dwChatLen      = dwChatLen;
    memcpy(stCHAT_DATA.aszUserName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, USERNAME_LEN);
    memcpy(stCHAT_DATA.abyChatContent, abyChatContent, dwChatLen);

    //如果是帮派的聊天信息，要投递到帮派的信息队列中去
    pstAck->wErrCode   = CChatMgr::Instance()->AddChatData(poPlayer, stCHAT_DATA, pstAck->aszErrInfo);

    memcpy(&pstAck->stChatData, &stCHAT_DATA, sizeof(CHAT_DATA));
    pstAck->byVipLevel      = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    pstAck->byFactionIconID = poPlayer->GetFactionIconID();

    return CLIGS_CHAT_ACK;
}


UINT16	CGTPktBuilder::OnOpenChatReq( CPlayer* poPlayer, UINT32 dwSerialNumber)
{

    ZERO_PACKET(PKT_CLIGS_OPEN_CHAT_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CChatMgr::Instance()->GetChatData(*pstAck, poPlayer, dwSerialNumber);
    //pstAck->byChatNum = 20;

    return CLIGS_OPEN_CHAT_ACK;
}


UINT16 CGTPktBuilder::OnOpenActivityCenterAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPNE_ACTIVITY_CENTER::ID_UNLOCK;
        return CLIGS_OPNE_ACTIVITY_CENTER_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = poPlayer->GetActivityCenterData(pstAck->byActivityNum, pstAck->astActivityInfo, pstAck->abyActivityStartCountDown);

    return CLIGS_OPNE_ACTIVITY_CENTER_ACK;

}

UINT16 CGTPktBuilder::OnOpneEatBreadAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPNE_EAT_BREAD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_EATBREAD);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPNE_EAT_BREAD::ID_UNLOCK;
        return CLIGS_OPNE_EAT_BREAD_ACK;
    }
    poBuild->OnOpen();

    CEatBread *poEatBread  = poPlayer->GetEatBread();
    if (NULL == poEatBread)
    {
        pstAck->wErrCode = ERR_OPNE_EAT_BREAD::ID_UNLOCK;
        return CLIGS_OPNE_EAT_BREAD_ACK;
    }

    pstAck->wErrCode = poEatBread->OpenEatBread(*pstAck);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->wCurPhyStrength = poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength;
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stCurPhyStrengthInfo);
    return CLIGS_OPNE_EAT_BREAD_ACK;
}


UINT16 CGTPktBuilder::OnObtainPhystrengthenAck(CPlayer* poPlayer, UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CEatBread *poEatBread  = poPlayer->GetEatBread();
    if (NULL == poEatBread)
    {
        pstAck->wErrCode = ERR_LEAVE_EAT_BREAD::ID_UNLOCK;
        return CLIGS_OBTAIN_PHYSTRENGTHEN_ACK;
    }

    pstAck->wErrCode = poEatBread->ObtainPhystrengthen(byHaveObtainPhyStrengthenCli, dwHaveObtainCoinCli, pstAck->byHaveObtainPhyStrengthen, pstAck->dwHaveObtainCoin, pstAck->wHaveUseTime);
    pstAck->byHaveObtainPhyStrengthenCli = byHaveObtainPhyStrengthenCli;
    pstAck->dwHaveObtainCoinCli = dwHaveObtainCoinCli;

    return CLIGS_OBTAIN_PHYSTRENGTHEN_ACK;
}


UINT16 CGTPktBuilder::OnOLeaveEatBreadAck(CPlayer* poPlayer, UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_LEAVE_EAT_BREAD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CEatBread *poEatBread  = poPlayer->GetEatBread();
    if (NULL == poEatBread)
    {
        pstAck->wErrCode = ERR_OBTAIN_PHYSTRENGTHEN::ID_UNLOCK;
        return CLIGS_OBTAIN_PHYSTRENGTHEN_ACK;
    }

    pstAck->wErrCode = poEatBread->LeaveEatBread(byHaveObtainPhyStrengthenCli, dwHaveObtainCoinCli, pstAck->byHaveObtainPhyStrengthen, pstAck->dwHaveObtainCoin);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->wCurPhyStrength = poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength;
    poPlayer->GetActivityCenterPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stCurPhyStrengthInfo);
    return CLIGS_LEAVE_EAT_BREAD_ACK;
}


//重置爬塔
UINT16 CGTPktBuilder::OnClimbTowerResetReq(CPlayer* poPlayer,  UINT16 wSceneIdx, UINT8 byTownIdx)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLIMB_TOWER_RESET_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CClimbTower *poClimbTower = poPlayer->GetClimbTower();
    if (NULL == poClimbTower)
    {
        pstAck->wErrCode = ERR_CLIMB_TOWER_RESET::ID_UNDER_NO_RESET;
        return CLIGS_CLIMB_TOWER_RESET_ACK;
    }

    //UINT8 byMaxInstanceNum = 0;
    pstAck->wErrCode = poClimbTower->ResetClimbTowerChallengeTime(byTownIdx);
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetInstance().GetClimbTownInstanceInfo(pstAck->stClimbTowerInfo.byMaxInstanceNum, pstAck->stClimbTowerInfo.byMaxUnlockInstanceNum, pstAck->stClimbTowerInfo.stCurInstanceInfo);
    poClimbTower->GetClimbTowerBaseInfo(pstAck->stClimbTowerInfo.wCanResetClimbTownNum, pstAck->stClimbTowerInfo.wResetClimbTownNumPerDay);
    pstAck->stClimbTowerInfo.wBuyResetClimbTownGold = poClimbTower->GetResetClimbTowerGold((UINT8)poPlayer->GetInstance().GetCurrentClimbTower());
    poPlayer->GetEmprisePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_CLIMB_TOWER_RESET_ACK;
}

//爬塔挂机
UINT16 CGTPktBuilder::OnClimbTowerOnHookReq(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CClimbTower *poClimbTower = poPlayer->GetClimbTower();
    if (NULL == poClimbTower)
    {
        pstAck->wErrCode =  ERR_CLIMB_TOWER_ONHOOK::ID_ONHOOK_UNLOCK;
        return CLIGS_CLIMB_TOWER_ONHOOK_ACK;
    }

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;


    pstAck->wErrCode = COnhookMgr::Instance()->OnhookTower(poPlayer, wSceneIdx, byTownIdx, pstAck->byOnhookResultNum, pstAck->astOnhookResult, pstAck->stAfterOnhookData);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetUnlockNewInstance(pstAck->byUnlockNewInstanceFlag, pstAck->byHaveNewInstanceFlag, pstAck->astUnlockCommonInstanceID[0], pstAck->astNewCommonInstanceID[0]);//
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetInstance().GetClimbTownInstanceInfo(pstAck->stClimbTowerInfo.byMaxInstanceNum, pstAck->stClimbTowerInfo.byMaxUnlockInstanceNum, pstAck->stClimbTowerInfo.stCurInstanceInfo);
    poClimbTower->GetClimbTowerBaseInfo(pstAck->stClimbTowerInfo.wCanResetClimbTownNum, pstAck->stClimbTowerInfo.wResetClimbTownNumPerDay);
    pstAck->stClimbTowerInfo.wBuyResetClimbTownGold = poClimbTower->GetResetClimbTowerGold((UINT8)poPlayer->GetInstance().GetCurrentClimbTower());
    poPlayer->GetEmprisePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_CLIMB_TOWER_ONHOOK_ACK;
}


UINT16	CGTPktBuilder::OnOpenPaoshangBuildReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_UPDATEATTR);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_YABIAO_BUILD::ID_BUILD_UNLOCK;
        return CLIGS_OPEN_PAOSHANG_BUILD_ACK;
    }
    poBuild->OnOpen();

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        pstAck->wErrCode = ERR_OPEN_YABIAO_BUILD::ID_BUILD_UNLOCK;
        return CLIGS_OPEN_PAOSHANG_BUILD_ACK;
    }

    pstAck->wAllPlayerYabiaoCnt = CYabiaoMgr::Instance()->GetPlayerCount();

    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL != poPlunder)
    {
        pstAck->byEnemyYabiaoCnt = CYabiaoMgr::Instance()->GetEnemy( poPlayer->GetLevel(), poPlunder->GetEnemySet() ).size();
    }
    else
    {
        pstAck->byEnemyYabiaoCnt = 0;
    }


    pstAck->byRemainJiebiaoCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
    pstAck->byRemainYabiaoCnt = poYabiaoInfo->GetRemainYabiaoCnt();
    pstAck->wErrCode = ERR_OPEN_YABIAO_BUILD::ID_SUCCESS;

    pstAck->wAlreadyYabiaoTime = poYabiaoInfo->GetYabiaoPassedTime();
    pstAck->stBiaoInfo = poYabiaoInfo->GetCurBiaoInfo();

    pstAck->byYabiaoFlag = poYabiaoInfo->GetYabiaoFlag();

    pstAck->qwCoinEncourage = poYabiaoInfo->GetLastEncourageCoin();
    pstAck->dwScienceEncourage = UINT32( poYabiaoInfo->GetLastEncourageScience() );
    pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();
    pstAck->wClearCDCostGoldPerMin = CSinglePramMgr::Instance()->GetClearJiebiaoCDCostGoldPerMin();
    //DBG_INFO( "玩家:%u 返回押镖铜钱奖励:%llu, 科技点奖励%llu",
    //          poPlayer->GetID(), pstAck->qwCoinEncourage, pstAck->dwScienceEncourage );
    return CLIGS_OPEN_PAOSHANG_BUILD_ACK;
}

UINT16	CGTPktBuilder::OnOpenYabiaoReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_YABIAO_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_YABIAO);
    CBuild* poBuildAdventure = poPlayer->GetBuild(EBK_ADVENTURE);
    if((NULL == poBuild) || !poBuildAdventure)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_OPEN_YABIAO_ACK;
    }
    poBuild->OnOpen();
    poBuildAdventure->OnOpen();

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_OPEN_YABIAO_ACK;
    }

    poYabiaoInfo->Check4Reset();
    pstAck->byYabiaoing = (UINT8)poYabiaoInfo->GetYabiaoFlag() == CYabiaoInfo::ms_nYabiaoProcessFlagRunning;
    pstAck->wCurBiaoIndex = poYabiaoInfo->GetCurBiaoQuality();
    pstAck->wDirectMaxLevelCostGold = CYabiaoMgr::Instance()->GetDirectMaxLevelCostGold();
    pstAck->wFlushCostGold = poYabiaoInfo->GetFlushCostGold();
    pstAck->wFlushCount = poYabiaoInfo->GetFlushCount();
    pstAck->wErrCode = ERR_OPEN_YABIAO::ID_SUCCESS;
    pstAck->byFreeFlushCnt = UINT8( CSinglePramMgr::Instance()->GetFreeFlushBiaoCnt() );

    //剩余刷新次数计算
    UINT32 dwAlreadyFlushCnt = poYabiaoInfo->GetDT_YABIAO_BASE_DATA().dwAlreadyFlushBiaoCnt;
    pstAck->byFreeFlushRemainCnt = pstAck->byFreeFlushCnt > dwAlreadyFlushCnt ? pstAck->byFreeFlushCnt - dwAlreadyFlushCnt : 0;

    vector<SBiaoBaseInfo> vecBiaoInfo = CYabiaoMgr::Instance()->GetBiaoInfoList();
    for( INT32 nIndex = 0; nIndex < MAX_BIAO_QUALITY; ++nIndex )
    {
        pstAck->astBiaoInfo[nIndex] = CYabiaoMgr::Instance()->GetBiaoInfo( nIndex, poPlayer->GetLevel() );
        pstAck->wBiaoQualityNum = nIndex + 1;
    }


    return CLIGS_OPEN_YABIAO_ACK;
}

UINT16	CGTPktBuilder::OnBeginYabiaoReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_BEGIN_YABIAO_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_BEGIN_YABIAO_ACK;
    }

    poYabiaoInfo->Check4Reset();

    INT32 nRet = CYabiaoMgr::Instance()->AddPlayer( poPlayer->GetDT_PLAYER_BASE_DATA().dwID );
    if( nRet == -1 )
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = nRet;
    pstAck->wAlreadyYabiaoTime = poYabiaoInfo->GetYabiaoPassedTime();
    pstAck->wTotalYabiaoTime = poYabiaoInfo->GetCurBiaoInfo().wCostTime;

    pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainBeJiebiaoCnt();
    pstAck->stBeJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt();

    //被劫镖的铜钱奖励
    pstAck->qwBeJiebiaoCoinEncourage = poYabiaoInfo->GetJiebiaoCoinEncourage( poPlayer->GetLevel(), poPlayer->GetLevel() );
    pstAck->dwBeJiebiaoScienceEncourage = poYabiaoInfo->GetJiebiaoScienceEncourage( poPlayer->GetLevel(), poPlayer->GetLevel() );
    //DBG_INFO( _SDT( "ID:%d already yabiao time:%d" ), poPlayer->GetDT_PLAYER_BASE_DATA().dwID, pstAck->wAlreadyYabiaoTime );

    UINT32 dwFactionID = 0;

    BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, poPlayer->GetDT_PLAYER_BASE_DATA().dwID);
    if ( bRet )
    {
        CFaction *pCFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);

        if(pCFaction != NULL)
        {
            strcpy( pstAck->aszFactionName,  pCFaction->GetDT_FACTION_BASE_DATA().aszFactionName );
			pstAck->byFactionIconID = pCFaction->GetFactionIconID();
        }
    }

    return CLIGS_BEGIN_YABIAO_ACK;
}


UINT16	CGTPktBuilder::OnFlushBiaoQualityReq( CPlayer* poPlayer, UINT16 wCurQuality, BOOL bDirectMaxLevel )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_FLUSH_BIAO_QUALITY_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_FLUSH_BIAO_QUALITY_ACK;
    }

    pstAck->byDirectMaxLevel = bDirectMaxLevel;
    DT_BIAO_INFO stBiaoInfo = poYabiaoInfo->GetCurBiaoInfo();
    if( wCurQuality != stBiaoInfo.byQualityLevel && bDirectMaxLevel )
    {
        if( poYabiaoInfo->IsBiaoQualityMaxLevel() )
        {
            FillBiaoAckInfo( *pstAck, *poPlayer );
            return CLIGS_FLUSH_BIAO_QUALITY_ACK;
        }
    }

    if( wCurQuality != stBiaoInfo.byQualityLevel )
    {
        FillBiaoAckInfo( *pstAck, *poPlayer );
        return CLIGS_FLUSH_BIAO_QUALITY_ACK;
    }

    if( bDirectMaxLevel )
    {
        pstAck->wErrCode = poYabiaoInfo->DirectMaxLevel();
    }
    else
    {
        pstAck->wErrCode = poYabiaoInfo->FlushBiaoInfo();
    }

    FillBiaoAckInfo( *pstAck, *poPlayer );
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->byFreeFlushCnt = UINT8( CSinglePramMgr::Instance()->GetFreeFlushBiaoCnt() );

    //剩余刷新次数计算
    UINT32 dwAlreadyFlushCnt = poYabiaoInfo->GetDT_YABIAO_BASE_DATA().dwAlreadyFlushBiaoCnt;
    pstAck->byFreeFlushRemainCnt = pstAck->byFreeFlushCnt > dwAlreadyFlushCnt ? pstAck->byFreeFlushCnt - dwAlreadyFlushCnt : 0;
    return CLIGS_FLUSH_BIAO_QUALITY_ACK;
}

VOID CGTPktBuilder::FillBiaoAckInfo( PKT_CLIGS_FLUSH_BIAO_QUALITY_ACK &ack, CPlayer &oPlayer )
{
    CYabiaoInfo *poYabiaoInfo = oPlayer.GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        return;
    }

    ack.stBiaoInfo = poYabiaoInfo->GetCurBiaoInfo();
    ack.qwCurGold = oPlayer.GetDT_PLAYER_BASE_DATA().qwGold;
    ack.wFlushCostGold = poYabiaoInfo->GetFlushCostGold();
    ack.wFlushCount = poYabiaoInfo->GetFlushCount();
}


UINT16	CGTPktBuilder::OnOpenJiebiaoReq( CPlayer* poPlayer, BOOL bEnemy, UINT16 wTimeToEnd, UINT8 byNum )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_JIEBIAO_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_OPEN_JIEBIAO_ACK;
    }

    poYabiaoInfo->Check4Reset();

    if( byNum < 1 )
    {
        pstAck->wErrCode = ERR_OPEN_JIEBIAO::ID_QUERY_PLAYER_NUM_ERROR;
        return CLIGS_OPEN_JIEBIAO_ACK;
    }


    vector<UINT32> vecYabiaoPlayer;
    if( bEnemy )
    {
        //是仇敌，返回仇敌的押镖信息
        CPlunder* poPlunder = poPlayer->GetPlunder();

        if(NULL != poPlunder)
        {
            vecYabiaoPlayer = CYabiaoMgr::Instance()->GetEnemy( poPlayer->GetLevel(), poPlunder->GetEnemySet(), byNum + 1, wTimeToEnd );
        }
    }
    else
    {
        //普通，返回全部的押镖信息
        vecYabiaoPlayer = CYabiaoMgr::Instance()->GetYabiaoPlayer( poPlayer->GetLevel(), wTimeToEnd, byNum + 1 );
    }

    vector<UINT32>::iterator itrFind = std::find( vecYabiaoPlayer.begin(), vecYabiaoPlayer.end(), poPlayer->GetID() );

    //判断是否有必要添加自己进去

    if( itrFind == vecYabiaoPlayer.end() )
    {
        //添加的同时，删除一个已经获取到的
        if( vecYabiaoPlayer.size() == byNum + 1 )
        {
            vecYabiaoPlayer.pop_back();
        }

        if( wTimeToEnd )
        {
            //差异
        }
        else
        {
            //非差异
            if( vecYabiaoPlayer.size() == byNum )
            {
                vecYabiaoPlayer.pop_back();
            }

            if( poYabiaoInfo->GetYabiaoFlag() == CYabiaoInfo::ms_nYabiaoProcessFlagRunning )
            {
                vecYabiaoPlayer.push_back( poPlayer->GetID() );
            }

        }


    }
    else
    {
        //如果是请求差异，那么不需要主角
        if( wTimeToEnd )
        {
            //差异
            vecYabiaoPlayer.erase( itrFind );
        }
        //非差异
        else
        {
            //什么都不做

        }
    }

    CYabiaoMgr::Instance()->FillPlayerYabiaoInfo( *pstAck, vecYabiaoPlayer, poPlayer->GetID(), poPlayer->GetLevel(), poPlayer);
    pstAck->wLongestYabiaoEndTime = wTimeToEnd;

    pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();
    pstAck->byRemainJiebiaoCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
    pstAck->wClearCDCostGoldPerMin = CSinglePramMgr::Instance()->GetClearJiebiaoCDCostGoldPerMin();

    pstAck->wAlreadyYabiaoTime = poYabiaoInfo->GetYabiaoPassedTime();
    pstAck->stBiaoInfo = poYabiaoInfo->GetCurBiaoInfo();
    pstAck->byRemainYabiaoCnt = poYabiaoInfo->GetRemainYabiaoCnt();;
    pstAck->qwCoinEncourage = poYabiaoInfo->GetLastEncourageCoin();
    pstAck->qwScienceEncourage = poYabiaoInfo->GetLastEncourageScience();
    pstAck->qwJingjieEncourage = poYabiaoInfo->GetLastEncourageJingjie();
    pstAck->byYabiaoFlag = poYabiaoInfo->GetYabiaoFlag();
    pstAck->qwCurJingjie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;

    //劫镖减少的钱
    pstAck->dwJiebiaoReduceCoin = poYabiaoInfo->GetDT_YABIAO_BASE_DATA().dwJiebiaoDecCoin; //UINT32( poYabiaoInfo->GetDT_YABIAO_BASE_DATA().byBeJiebiaoCount*poYabiaoInfo->GetJiebiaoEncourage( , ,  ) );
    pstAck->dwJiebiaoReduceScience = poYabiaoInfo->GetDT_YABIAO_BASE_DATA().dwJiebiaoDecScience;
	pstAck->byDisPlayJieBiao = CSinglePramMgr::Instance()->GetInterceptFlag();
    return CLIGS_OPEN_JIEBIAO_ACK;
}

UINT16	CGTPktBuilder::OnBeginJiebiaoReq( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
	CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
	if ( NULL != poPlayerMedal )
	{
		poPlayerMedal->AddInterceptNum();
		poPlayerMedal->CheckMedalBySmallType(EMST_INTERCEPT);
	}
    CUser* poUser = poPlayer->GetUser();
	if ( (poUser) && (poUser->GetCliVer() >= VERSION_PET))
	{
		return OnBeginJiebiaoReq5(poPlayer, dwEnemyID, byCount, qwBattleIndex, bRepeat);
	}
	else if ( (poUser) && (poUser->GetCliVer() >= VERSION_SOUL))
	{
		return OnBeginJiebiaoReq4(poPlayer, dwEnemyID, byCount, qwBattleIndex, bRepeat);
	}
    else if ((poUser) && (poUser->GetCliVer() > 310))
    {
        return OnBeginJiebiaoReq3(poPlayer, dwEnemyID, byCount, qwBattleIndex, bRepeat);
    }
    return OnBeginJiebiaoReq5(poPlayer, dwEnemyID, byCount, qwBattleIndex, bRepeat);
}

UINT16	CGTPktBuilder::OnBeginJiebiaoReq5( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_BEGIN_JIEBIAO_ACK5);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_BEGIN_JIEBIAO_ACK5;
    }

    //查看是否是重复的请求
    PKT_CLIGS_BEGIN_JIEBIAO_ACK5 *pAckFind = poYabiaoInfo->GetJiebiaoCache5( byCount );
    // 	auto *pAckFind=CYabiaoMgr::Instance()->GetJiebiaoAck( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, byCount );
    if( NULL != pAckFind )
    {
        DBG_INFO( _SDT("repeat yabiao request,player:%u Index:%u"), poPlayer->GetID(), byCount );
        (*pstAck) = *pAckFind;
        bRepeat = TRUE;
        return CLIGS_BEGIN_JIEBIAO_ACK5;
    }


    //劫镖成功不成功固定的数据赋值
    pstAck->dwBeJiebiaoPlayerID = dwEnemyID;
    pstAck->dwJiebiaoPlayerID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;
    pstAck->stJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalJiebiaoCnt();
    pstAck->stBeJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt();

    pstAck->stJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    // 	if( poYabiaoInfo->GetRemainJiebiaoCD()!=0 )
    // 	{
    // 		pstAck->wErrCode=ERR_JIEBIAO::ID_JIEBIAO_IN_CD;
    // 		return CLIGS_BEGIN_JIEBIAO_ACK;
    // 	}

    CPlayer* pEnemyPlayer = CPlayerMgr::Instance()->FindPlayer( dwEnemyID );
    if( NULL == pEnemyPlayer )
    {
        pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
        return CLIGS_BEGIN_JIEBIAO_ACK5;
    }

    CYabiaoInfo *poEnemyYabiaoInfo = pEnemyPlayer->GetYabiaoInfo();
    if(NULL == poEnemyYabiaoInfo)
    {
        pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
        return CLIGS_BEGIN_JIEBIAO_ACK5;
    }

    pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poEnemyYabiaoInfo->GetRemainBeJiebiaoCnt();

    if( poPlayer->GetDT_PLAYER_BASE_DATA().dwID == dwEnemyID )
    {
        pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_JIEBIAO_SELF;
        return CLIGS_BEGIN_JIEBIAO_ACK5;
    }
    pstAck->wErrCode = CYabiaoMgr::Instance()->BeginJiebiao( *poPlayer, dwEnemyID, pstAck->qwCoinEncourage, pstAck->dwScienceEncourage, pstAck->stBattleData, qwBattleIndex );
    //劫镖成功影响的数据赋值
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();
    pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poEnemyYabiaoInfo->GetRemainBeJiebiaoCnt();
    pstAck->stJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

    if( pstAck->wErrCode == ERR_JIEBIAO::ID_SUCCESS )
    {
        //如果挑战成功，则缓存
        poYabiaoInfo->SetJiebiaoCache5( pstAck->stJiebiaoInfo.byRemianOperateCnt + 1, *pstAck );
    }

    poPlayer->GetPrompt( pstAck->stPromptInfo );

   
    return CLIGS_BEGIN_JIEBIAO_ACK5;
}

UINT16	CGTPktBuilder::OnBeginJiebiaoReq3( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_BEGIN_JIEBIAO_ACK3);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_BEGIN_JIEBIAO_ACK3;
    }

    //查看是否是重复的请求
    PKT_CLIGS_BEGIN_JIEBIAO_ACK3 *pAckFind = poYabiaoInfo->GetJiebiaoCache3( byCount );
    // 	auto *pAckFind=CYabiaoMgr::Instance()->GetJiebiaoAck( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, byCount );
    if( NULL != pAckFind )
    {
        DBG_INFO( _SDT("repeat yabiao request,player:%u Index:%u"), poPlayer->GetID(), byCount );
        (*pstAck) = *pAckFind;
        bRepeat = TRUE;
        return CLIGS_BEGIN_JIEBIAO_ACK3;
    }


    //劫镖成功不成功固定的数据赋值
    pstAck->dwBeJiebiaoPlayerID = dwEnemyID;
    pstAck->dwJiebiaoPlayerID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;
    pstAck->stJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalJiebiaoCnt();
    pstAck->stBeJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt();

    pstAck->stJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    // 	if( poYabiaoInfo->GetRemainJiebiaoCD()!=0 )
    // 	{
    // 		pstAck->wErrCode=ERR_JIEBIAO::ID_JIEBIAO_IN_CD;
    // 		return CLIGS_BEGIN_JIEBIAO_ACK;
    // 	}

    CPlayer* pEnemyPlayer = CPlayerMgr::Instance()->FindPlayer( dwEnemyID );
    if( NULL == pEnemyPlayer )
    {
        pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
        return CLIGS_BEGIN_JIEBIAO_ACK3;
    }

    CYabiaoInfo *poEnemyYabiaoInfo = pEnemyPlayer->GetYabiaoInfo();
    if(NULL == poEnemyYabiaoInfo)
    {
        pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
        return CLIGS_BEGIN_JIEBIAO_ACK3;
    }

    pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poEnemyYabiaoInfo->GetRemainBeJiebiaoCnt();

    if( poPlayer->GetDT_PLAYER_BASE_DATA().dwID == dwEnemyID )
    {
        pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_JIEBIAO_SELF;
        return CLIGS_BEGIN_JIEBIAO_ACK3;
    }

	DT_BATTLE_DATA5 stBattleData5 = {0};
    pstAck->wErrCode = CYabiaoMgr::Instance()->BeginJiebiao( *poPlayer, dwEnemyID, pstAck->qwCoinEncourage, pstAck->dwScienceEncourage, stBattleData5, qwBattleIndex );
	CBattleMgr::Instance()->Convert5To3(stBattleData5, pstAck->stBattleData);
    //劫镖成功影响的数据赋值
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();
    pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poEnemyYabiaoInfo->GetRemainBeJiebiaoCnt();
    pstAck->stJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

    if( pstAck->wErrCode == ERR_JIEBIAO::ID_SUCCESS )
    {
        //如果挑战成功，则缓存
        poYabiaoInfo->SetJiebiaoCache3( pstAck->stJiebiaoInfo.byRemianOperateCnt + 1, *pstAck );
        //CYabiaoMgr::Instance()->AddJiebiaoAck( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, byCount, *pstAck );

        //记录战斗信息
        //		CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_DRIVEAWAY, dwCaptureOwnerPlayerID);
    }

    poPlayer->GetPrompt( pstAck->stPromptInfo );

    //DBG_INFO( _SDT( "[%s,%d]player:%u return remain jiebiao count:%d, player:%u remain bejiebiao cnt:%d, battle index:%llu, coin:%llu" ), MSG_MARK,
    //	poPlayer->GetID(), pstAck->stBeJiebiaoInfo.byRemianOperateCnt, dwEnemyID, pstAck->stBeJiebiaoInfo.byRemianOperateCnt, qwBattleIndex, pstAck->qwCoinEncourage );
    return CLIGS_BEGIN_JIEBIAO_ACK3;
}

UINT16	CGTPktBuilder::OnBeginJiebiaoReq4( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat )
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}


	ZERO_PACKET(PKT_CLIGS_BEGIN_JIEBIAO_ACK4);
	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
	if(NULL == poYabiaoInfo)
	{
		SET_OTHER_ERR(pstAck->wErrCode);
		return CLIGS_BEGIN_JIEBIAO_ACK4;
	}

	//查看是否是重复的请求
	PKT_CLIGS_BEGIN_JIEBIAO_ACK4 *pAckFind = poYabiaoInfo->GetJiebiaoCache4( byCount );
	// 	auto *pAckFind=CYabiaoMgr::Instance()->GetJiebiaoAck( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, byCount );
	if( NULL != pAckFind )
	{
		DBG_INFO( _SDT("repeat yabiao request,player:%u Index:%u"), poPlayer->GetID(), byCount );
		(*pstAck) = *pAckFind;
		bRepeat = TRUE;
		return CLIGS_BEGIN_JIEBIAO_ACK4;
	}


	//劫镖成功不成功固定的数据赋值
	pstAck->dwBeJiebiaoPlayerID = dwEnemyID;
	pstAck->dwJiebiaoPlayerID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;
	pstAck->stJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalJiebiaoCnt();
	pstAck->stBeJiebiaoInfo.byTotalOperateCnt = CYabiaoMgr::Instance()->GetTotalBeJiebiaoCnt();

	pstAck->stJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
	pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

	// 	if( poYabiaoInfo->GetRemainJiebiaoCD()!=0 )
	// 	{
	// 		pstAck->wErrCode=ERR_JIEBIAO::ID_JIEBIAO_IN_CD;
	// 		return CLIGS_BEGIN_JIEBIAO_ACK;
	// 	}

	CPlayer* pEnemyPlayer = CPlayerMgr::Instance()->FindPlayer( dwEnemyID );
	if( NULL == pEnemyPlayer )
	{
		pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
		return CLIGS_BEGIN_JIEBIAO_ACK4;
	}

	CYabiaoInfo *poEnemyYabiaoInfo = pEnemyPlayer->GetYabiaoInfo();
	if(NULL == poEnemyYabiaoInfo)
	{
		pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_FIND_PLAYER;
		return CLIGS_BEGIN_JIEBIAO_ACK4;
	}

	pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poEnemyYabiaoInfo->GetRemainBeJiebiaoCnt();

	if( poPlayer->GetDT_PLAYER_BASE_DATA().dwID == dwEnemyID )
	{
		pstAck->wErrCode = ERR_JIEBIAO::ID_CANNOT_JIEBIAO_SELF;
		return CLIGS_BEGIN_JIEBIAO_ACK4;
	}

	DT_BATTLE_DATA5 stBattleData = {0};
	pstAck->wErrCode = CYabiaoMgr::Instance()->BeginJiebiao( *poPlayer, dwEnemyID, pstAck->qwCoinEncourage, pstAck->dwScienceEncourage, stBattleData, qwBattleIndex );
	CBattleMgr::Instance()->Convert5To4(stBattleData, pstAck->stBattleData);

	//劫镖成功影响的数据赋值
	pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

	pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();
	pstAck->stBeJiebiaoInfo.byRemianOperateCnt = poEnemyYabiaoInfo->GetRemainBeJiebiaoCnt();
	pstAck->stJiebiaoInfo.byRemianOperateCnt = poYabiaoInfo->GetRemainJiebiaoCnt();
	pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

	if( pstAck->wErrCode == ERR_JIEBIAO::ID_SUCCESS )
	{
		//如果挑战成功，则缓存
		poYabiaoInfo->SetJiebiaoCache4( pstAck->stJiebiaoInfo.byRemianOperateCnt + 1, *pstAck );
		//CYabiaoMgr::Instance()->AddJiebiaoAck( poPlayer->GetDT_PLAYER_BASE_DATA().dwID, byCount, *pstAck );

		//记录战斗信息
		//		CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_DRIVEAWAY, dwCaptureOwnerPlayerID);
	}

	poPlayer->GetPrompt( pstAck->stPromptInfo );

	//DBG_INFO( _SDT( "[%s,%d]player:%u return remain jiebiao count:%d, player:%u remain bejiebiao cnt:%d, battle index:%llu, coin:%llu" ), MSG_MARK,
	//	poPlayer->GetID(), pstAck->stBeJiebiaoInfo.byRemianOperateCnt, dwEnemyID, pstAck->stBeJiebiaoInfo.byRemianOperateCnt, qwBattleIndex, pstAck->qwCoinEncourage );
	return CLIGS_BEGIN_JIEBIAO_ACK4;
}

UINT16  CGTPktBuilder::BeJiebiaoEventNtf( BOOL bRevoltSuccess, UINT64 qwIndex, UINT64 qwCoinDec, UINT32 dwScienceDec,
        const string &strEnemyDispName, UINT16 wEnemyLevel, UINT32 dwEnemyPower, BYTE byUnreadBattleLogNum )
{


    ZERO_PACKET(PKT_CLIGS_BE_JIEBIAO_NTF);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    strcpy( pstAck->aszEnemyDispName, strEnemyDispName.c_str() );
    pstAck->byResult = bRevoltSuccess;
    pstAck->dwEnemyPower = dwEnemyPower;
    pstAck->qwCoinDec = qwCoinDec;
    pstAck->qwIndex = qwIndex;
    pstAck->wEnemyLevel = wEnemyLevel;
    pstAck->byUnreadBattleLogNum = byUnreadBattleLogNum;
    pstAck->dwScienceDec = dwScienceDec;

    return CLIGS_BE_JIEBIAO_NTF;
}

UINT16  CGTPktBuilder::BattleDataAck2( CPlayer*poPlayer, const DT_BATTLE_DATA3 &data, BOOL bHaveData )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    return BattleDataAck3(poPlayer, *(DT_BATTLE_DATA3*)&data, bHaveData);
}

UINT16  CGTPktBuilder::BattleDataAck4( CPlayer*poPlayer, const DT_BATTLE_DATA4 &data, BOOL bHaveData )
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	return BattleDataAck4(poPlayer, *(DT_BATTLE_DATA4*)&data, bHaveData);
}

UINT16  CGTPktBuilder::BattleDataAck5( CPlayer*poPlayer, const DT_BATTLE_DATA5 &data, BOOL bHaveData )
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	return BattleDataAck5(poPlayer, *(DT_BATTLE_DATA5*)&data, bHaveData);
}

UINT16  CGTPktBuilder::BattleDataAck5( CPlayer*poPlayer, DT_BATTLE_DATA5 &data, BOOL bHaveData )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_QUERY_BATTLE_RECORD_ACK5);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if( bHaveData == FALSE )
    {
        pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_BATTLE_NOT_EXIST;
        return CLIGS_QUERY_BATTLE_RECORD_ACK5;
    }

    
    pstAck->stBattleData = data;
    pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_SUCCESS;
    return CLIGS_QUERY_BATTLE_RECORD_ACK5;
}


UINT16  CGTPktBuilder::BattleDataAck3( CPlayer*poPlayer, DT_BATTLE_DATA3 &data, BOOL bHaveData )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
	
	if( poPlayer->GetCliVer() > 310)
    {
        ZERO_PACKET(PKT_CLIGS_QUERY_BATTLE_RECORD_ACK3);
        if (NULL == pstAck)
        {
            return INVALID_MSGID;
        }

        if( bHaveData == FALSE )
        {
            pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_BATTLE_NOT_EXIST;
            return CLIGS_QUERY_BATTLE_RECORD_ACK3;
        }

        pstAck->stBattleData = data;
        pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_SUCCESS;
        return CLIGS_QUERY_BATTLE_RECORD_ACK3;
    }

    ZERO_PACKET(PKT_CLIGS_QUERY_BATTLE_RECORD_ACK3);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if( bHaveData == FALSE )
    {
        pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_BATTLE_NOT_EXIST;
        return CLIGS_QUERY_BATTLE_RECORD_ACK3;
    }

    pstAck->stBattleData = data;
    pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_SUCCESS;
    return CLIGS_QUERY_BATTLE_RECORD_ACK3;
}

UINT16  CGTPktBuilder::BattleDataAck4( CPlayer*poPlayer, DT_BATTLE_DATA4 &data, BOOL bHaveData )
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}
	if ( poPlayer->GetCliVer() >= VERSION_SOUL)
	{
		ZERO_PACKET(PKT_CLIGS_QUERY_BATTLE_RECORD_ACK4);
		if (NULL == pstAck)
		{
			return INVALID_MSGID;
		}

		if( bHaveData == FALSE )
		{
			pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_BATTLE_NOT_EXIST;
			return CLIGS_QUERY_BATTLE_RECORD_ACK4;
		}

		pstAck->stBattleData = data;
		pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_SUCCESS;
		return CLIGS_QUERY_BATTLE_RECORD_ACK4;
	}
	else if( poPlayer->GetCliVer() > 310)
	{
		ZERO_PACKET(PKT_CLIGS_QUERY_BATTLE_RECORD_ACK3);
		if (NULL == pstAck)
		{
			return INVALID_MSGID;
		}

		if( bHaveData == FALSE )
		{
			pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_BATTLE_NOT_EXIST;
			return CLIGS_QUERY_BATTLE_RECORD_ACK3;
		}

		CBattleMgr::Instance()->Convert4To3(data, pstAck->stBattleData);
		pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_SUCCESS;
		return CLIGS_QUERY_BATTLE_RECORD_ACK3;
	}

	ZERO_PACKET(PKT_CLIGS_QUERY_BATTLE_RECORD_ACK4);
	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	if( bHaveData == FALSE )
	{
		pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_BATTLE_NOT_EXIST;
		return CLIGS_QUERY_BATTLE_RECORD_ACK3;
	}

	pstAck->stBattleData = data;
	pstAck->wErrCode = ERR_GET_BATTLE_DATA::ID_SUCCESS;
	return CLIGS_QUERY_BATTLE_RECORD_ACK3;
}

UINT16  CGTPktBuilder::OnQueryYabiaoBattleLogReq( CPlayer *poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK);


    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_QUERY_YABIAO_BATTLELOG_ACK;
    }

    CYabiaoInfo::CYabiaoBattleLogDeq &deqYabiaoBattleLog = poYabiaoInfo->GetYabiaoBattleLog();

    for( INT32 nIndex = (INT32)deqYabiaoBattleLog.size() - 1; nIndex >= 0; --nIndex )
    {
        if (nIndex > MAX_YABIAO_BATTLELOG_NUM )
        {
            continue;
        }
        DT_YABIAO_BATTLELOG_DATA_CLI &v_DT_YABIAO_BATTLELOG_DATA_CLI = pstAck->astYabiaoBattleLog[pstAck->byLogNum++];
        v_DT_YABIAO_BATTLELOG_DATA_CLI.stBattleLogData = deqYabiaoBattleLog[nIndex]->GetDT_PAOSHANG_BATTLELOG_DATA();

        //ssDbgOut << "Index:" << nIndex - 1 << " Time:" << v_DT_YABIAO_BATTLELOG_DATA_CLI.stBattleLogData.qwBattleTime
        //         << " Coin:" << v_DT_YABIAO_BATTLELOG_DATA_CLI.stBattleLogData.dwCoinChange
        //         << " Type:" << v_DT_YABIAO_BATTLELOG_DATA_CLI.stBattleLogData.byFlag << ",";

        const SPlayerBaseData* pBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData( v_DT_YABIAO_BATTLELOG_DATA_CLI.stBattleLogData.dwEnemyPlayerID );
        if( NULL == pBaseData )
        {
            continue;
        }
        strcpy( v_DT_YABIAO_BATTLELOG_DATA_CLI.aszEnemyDispName, pBaseData->aszDispName );
    }

    poYabiaoInfo->OnYabiaoBattleLogRead();
    //DBG_INFO( "[%s %d]: %s", MSG_MARK, ssDbgOut.str().c_str() );

    pstAck->wErrCode = ERR_GET_YABIAO_BATTLE_LOG::ID_SUCCESS;
    return CLIGS_QUERY_YABIAO_BATTLELOG_ACK;
}

UINT16	CGTPktBuilder::OnQueryYabiaoEncourageReq( CPlayer *poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK);

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_QUERY_YABIAO_ENCOURAGE_ACK;
    }

    pstAck->wErrCode = ERR_GET_YABIAO_ENCOURAGE::ID_SUCCESS;

    pstAck->qwCoinEncourage = poYabiaoInfo->GetLastEncourageCoin();
    pstAck->qwScienceEncourage = poYabiaoInfo->GetLastEncourageScience();
    pstAck->qwJingjieEncourage = poYabiaoInfo->GetLastEncourageJingjie();

    pstAck->wErrCode = poYabiaoInfo->OnObtainEncourage();

    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwCurJingjie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;

    poPlayer->GetPrompt( pstAck->stPromptInfo );

    return CLIGS_QUERY_YABIAO_ENCOURAGE_ACK;
}

UINT16	CGTPktBuilder::OnClearJiebiaoCDReq( CPlayer *poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLEAR_JIEBIAO_CD_ACK);

    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_CLEAR_JIEBIAO_CD_ACK;
    }

    pstAck->wErrCode = ERR_CLEAR_JIEBIAO_CD::ID_SUCCESS;

    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();
    INT32 nCostGold = poYabiaoInfo->GetClearJiebiaoCDGold();
    if( pstAck->qwCurGold < nCostGold )
    {
        pstAck->wErrCode = ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        return CLIGS_CLEAR_JIEBIAO_CD_ACK;
    }

    poPlayer->DecGold( nCostGold, CRecordMgr::EDGT_CLRJIEBIAOCD );

    poYabiaoInfo->ClearJiebiaoCD();
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->wJiebiaoCD = poYabiaoInfo->GetRemainJiebiaoCD();

    return CLIGS_CLEAR_JIEBIAO_CD_ACK;
}

UINT16 CGTPktBuilder::OnUpgradeJingjieReq(CPlayer *poPlayer, UINT16 wLevel)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_UPGRADE_JINGJIE_ACK);

    CJingJie* poJingJie = poPlayer->GetJingJie();
    if(NULL == poJingJie)
    {
        pstAck->wErrCode = ERR_UPGRADE_JINGJIE::ID_JINGJIE_UNLOCK;
        return CLIGS_UPGRADE_JINGJIE_ACK;
    }
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poJingJie->Upgrade(wLevel, pstAck->stJingJieInfo);
    pstAck->qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    pstAck->byColor = (UINT8)CJingJiePropMgr::Instance()->GetJingJieColor(poJingJie->GetDT_JINGJIE_DATA().stBaseData.wLevel);

    return CLIGS_UPGRADE_JINGJIE_ACK;
}


UINT16 CGTPktBuilder::OnOpenGenGasTabReq(CPlayer *poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_GEN_GAS_TAB_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_COLLECTGAS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_GEN_GAS_TAB::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_OPEN_GEN_GAS_TAB_ACK;
    }
    poBuild->OnOpen();

    CCollectGas *poCollectGas = poPlayer->GetCollectGas();
    if (NULL == poCollectGas)
    {
        pstAck->wErrCode = ERR_OPEN_GEN_GAS_TAB::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_OPEN_GEN_GAS_TAB_ACK;
    }

    pstAck->wErrCode = poCollectGas->OpenGenGasTab(*pstAck);

    return CLIGS_OPEN_GEN_GAS_TAB_ACK;
}


UINT16 CGTPktBuilder::OnGenGasBallReq(CPlayer *poPlayer, UINT8 byFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GEN_GAS_BALL_ACK);

    if(!poPlayer->GetBuild(EBK_COLLECTGAS))
    {
        pstAck->wErrCode = ERR_GEN_GAS_BALL::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_GEN_GAS_BALL_ACK;
    }

    CCollectGas *poCollectGas = poPlayer->GetCollectGas();
    if (NULL == poCollectGas)
    {
        pstAck->wErrCode = ERR_GEN_GAS_BALL::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_GEN_GAS_BALL_ACK;
    }
	CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		if(0 == byFlag)
		{
			poPlayerMedal->AddNormalGatherNum();
			poPlayerMedal->CheckMedalBySmallType(EMST_GATHER_SCIENCE_GOLD);
		}
		else
		{
			poPlayerMedal->AddGoldGatherNum();
			poPlayerMedal->CheckMedalBySmallType(EMST_GATHER_SCIENCE_GOLD);
		}
	}


    pstAck->wErrCode = poCollectGas->GenGas(byFlag, pstAck->stGenGasInfo);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_GEN_GAS_BALL_ACK;
}
UINT16 CGTPktBuilder::OnUpGasBallLevelReq(CPlayer *poPlayer, UINT8 byFullLevelFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_UP_GAS_BALL_LEVEL_ACK);

    if(!poPlayer->GetBuild(EBK_COLLECTGAS))
    {
        pstAck->wErrCode = ERR_UP_GAS_BALL_LEVEL::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_UP_GAS_BALL_LEVEL_ACK;
    }

    CCollectGas *poCollectGas = poPlayer->GetCollectGas();
    if (NULL == poCollectGas)
    {
        pstAck->wErrCode = ERR_UP_GAS_BALL_LEVEL::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_UP_GAS_BALL_LEVEL_ACK;
    }

    pstAck->wErrCode = poCollectGas->UpGasLevel(byFullLevelFlag, pstAck->stGenGasInfo);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_UP_GAS_BALL_LEVEL_ACK;
}
UINT16 CGTPktBuilder::OnCollectGasReq(CPlayer *poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_COLLECT_GAS_ACK);

    if(!poPlayer->GetBuild(EBK_COLLECTGAS))
    {
        pstAck->wErrCode = ERR_COLLECT_GAS::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_COLLECT_GAS_ACK;
    }

    CCollectGas *poCollectGas = poPlayer->GetCollectGas();
    if (NULL == poCollectGas)
    {
        pstAck->wErrCode = ERR_COLLECT_GAS::ID_COLLECT_GAS_UNLOCK;
        return CLIGS_COLLECT_GAS_ACK;
    }

    pstAck->wErrCode = poCollectGas->CollectGas(pstAck->stGenGasInfo);
    pstAck->qwScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

    return CLIGS_COLLECT_GAS_ACK;
}
UINT16 CGTPktBuilder::OnUpgradeSkillActionLevelReq(CPlayer *poPlayer, UINT16 wActionID, UINT16 wActionLevelCli)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK);

    if(!poPlayer->GetBuild(EBK_SKILL))
    {
        pstAck->wErrCode = ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_SKILL_UNLOCK;
        return CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK;
    }
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poPlayer->UpgradeSkillAction(wActionID, wActionLevelCli, pstAck->stSkillTabInfo);
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    return CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK;
}
UINT16 CGTPktBuilder::OnSelectSkillActionEffectReq(CPlayer *poPlayer, UINT16 wActionID, UINT16 wEffectID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK);

    if(!poPlayer->GetBuild(EBK_SKILL))
    {
        pstAck->wErrCode = ERR_SELECT_SKILL_ACTION_EFFECT::ID_SKILL_UNLOCK;
        return CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK;
    }
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poPlayer->SetActionEffectRangProp(wActionID, wEffectID, byAttackRangKind, byTraceBuffKind, pstAck->stSkillTabInfo);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    return CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK;
}
UINT16 CGTPktBuilder::OnOpenJingjieTabReq(CPlayer *poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_JINGJIE_TAB_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_JINGJIE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_JINGJIE_TAB::ID_JINGJIE_UNLOCK;
        return CLIGS_OPEN_JINGJIE_TAB_ACK;
    }
    CJingJie* poJingJie = poPlayer->GetJingJie();
    if(NULL == poJingJie)
    {
        pstAck->wErrCode = ERR_OPEN_JINGJIE_TAB::ID_JINGJIE_UNLOCK;
        return CLIGS_OPEN_JINGJIE_TAB_ACK;
    }
    poBuild->OnOpen();
    pstAck->wErrCode = poJingJie->OpenJingJieTab(*pstAck);

    return CLIGS_OPEN_JINGJIE_TAB_ACK;
}

UINT16 CGTPktBuilder::OnOpenSkillTabReq(CPlayer *poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_SKILL_TAB_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_SKILL);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SKILL_TAB::ID_SKILL_UNLOCK;
        return CLIGS_OPEN_SKILL_TAB_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = poPlayer->OpenSkillTab(pstAck->stSkillTabInfo);

    return CLIGS_OPEN_SKILL_TAB_ACK;
}


//打开VIP面板
UINT16 CGTPktBuilder::OnOpenVipTabReq(CPlayer *poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_VIP_TAB_ACK);
    pstAck->wErrCode = ERR_OPEN_VIP_TAB::ID_SUCCESS;
    CVipPropMgr::Instance()->OpenVipTab(poPlayer, poPlayer->GetAuthType(), pstAck->stVipTabInfo.byVipLevelNum, pstAck->stVipTabInfo.astVipLevelInfo,
                                        pstAck->astBuyGoldTxtLst, pstAck->aszProContextHead, pstAck->aszVipProContext, pstAck->aszBuyUrl);
    pstAck->stVipTabInfo.byCurVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    pstAck->stVipTabInfo.dwHaveRechargeGold = poPlayer->GetDT_PLAYER_BASE_DATA().dwTotalAmount;

    //当天第一次登录才会弹VIP面板信息
    if(0 == get_bit(poPlayer->GetExtData(), em_VIP))
    {
        pstAck->aszProContextHead[0] = '\0';
        pstAck->aszVipProContext[0] = '\0';
    }
    clr_bit(poPlayer->GetExtData(), em_VIP);
    //
	SAuthBuyGoldProp* poBuyGoldProp = CVipPropMgr::Instance()->GetBuyGoldInfoByAuthType(poPlayer->GetAuthType());
    if (NULL == poBuyGoldProp)
    {
        return CLIGS_OPEN_VIP_TAB_ACK;
    }

    for( UINT32 dwIndex = 0; dwIndex < poBuyGoldProp->m_vecBuyGoldProp.size(); ++dwIndex )
    {
        pstAck->astBuyGoldLst[ pstAck->byBuyGoldInfoNum ].dwGoldNum	= poBuyGoldProp->m_vecBuyGoldProp[dwIndex].dwGold;
        pstAck->astBuyGoldLst[ pstAck->byBuyGoldInfoNum ].dwRMB		= poBuyGoldProp->m_vecBuyGoldProp[dwIndex].wRMB;
        pstAck->astBuyGoldLst[ pstAck->byBuyGoldInfoNum ].dwProductID	= poBuyGoldProp->m_vecBuyGoldProp[dwIndex].dwProductID;

        pstAck->astBuyGoldLstEx[ pstAck->byBuyGoldInfoNum ].dwGoldNum	= poBuyGoldProp->m_vecBuyGoldProp[dwIndex].dwGold;
        pstAck->astBuyGoldLstEx[ pstAck->byBuyGoldInfoNum ].dwRMB		= poBuyGoldProp->m_vecBuyGoldProp[dwIndex].wRMB;
        SDStrncpy(pstAck->astBuyGoldLstEx[ pstAck->byBuyGoldInfoNum ].aszProductID, poBuyGoldProp->m_vecBuyGoldProp[dwIndex].aszProductID, MAX_PRODUCTID_LEN - 1);

        pstAck->byBuyGoldInfoNum++;
    }

    return CLIGS_OPEN_VIP_TAB_ACK;
}


UINT16 CGTPktBuilder::OnOpenAwakenTabReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_AWAKEN_TAB_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_AWAKEN);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_AWAKEN_TAB_ACK;
    }
    poBuild->OnOpen();

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_AWAKEN_TAB_ACK;
    }

    poAwaken->GetAwakenVersion() = 1;
    pstAck->wErrCode = poAwaken->OpenAwakenTab(pstAck->stAwakenInfo);

    return CLIGS_OPEN_AWAKEN_TAB_ACK;
}

UINT16 CGTPktBuilder::OnAwakenReq(CPlayer* poPlayer, UINT8 byQuickFlag, UINT8 byGoldFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_AWAKEN_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_AWAKEN_ACK;
    }
    poAwaken->GetAwakenVersion() = 1;
    pstAck->wErrCode = poAwaken->Awaken(byQuickFlag, byGoldFlag, pstAck->byAwakenNum, pstAck->astAwakenGetInfo, pstAck->stAwakenTabInfo);
    pstAck->byQuickFlag = byQuickFlag;
    pstAck->byGoldFlag = byGoldFlag;

    return CLIGS_AWAKEN_ACK;
}

UINT16 CGTPktBuilder::OnOpenDressStudyTabReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_DRESS_STUDY_TAB::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_DRESS_STUDY_TAB_ACK;
    }
    poAwaken->GetAwakenVersion() = 1;
    pstAck->wErrCode = poAwaken->OpenDressStudyTab(pstAck->stDressStudyTabInfo);

    return CLIGS_OPEN_DRESS_STUDY_TAB_ACK;

}

UINT16 CGTPktBuilder::OnDressStudyReq(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_DRESS_STUDY_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_DRESS_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_DRESS_STUDY_ACK;
    }
    poAwaken->GetAwakenVersion() = 1;

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poAwaken->DressStudy(wHeroKindID, byBagGridIdx, byHeroGridIdx, pstAck->astBagStudyInfo, pstAck->stHeroStudyInfo);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byBagGridIdx = byBagGridIdx;
    pstAck->byHeroGridIdx = byHeroGridIdx;

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);


    return CLIGS_DRESS_STUDY_ACK;
}

UINT16 CGTPktBuilder::OnStudyToExpReq(CPlayer* poPlayer, UINT8 byBagGridIdx, UINT8 byAllFlag, UINT8 abySelectConvertColor[MAX_COLOR_NUM])
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_STUDY_TO_EXP_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_TO_EXP::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_TO_EXP_ACK;
    }
    poAwaken->GetAwakenVersion() = 1;
    pstAck->wErrCode = poAwaken->ConvertExp(byBagGridIdx, byAllFlag, abySelectConvertColor, pstAck->dwGenStudyExp, pstAck->astBagStudyInfo, pstAck->qwCurStudyExp);
    memcpy(pstAck->abySelectConvertColor, abySelectConvertColor, sizeof(pstAck->abySelectConvertColor));
    pstAck->byBagGridIdx = byBagGridIdx;
    pstAck->byAllFlag = byAllFlag;

    return CLIGS_STUDY_TO_EXP_ACK;
}

UINT16 CGTPktBuilder::OnStudyUpgradeReq(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byLevel)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_STUDY_UPGRADE_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_UPGRADE_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_UPGRADE_ACK;
    }
    poAwaken->GetAwakenVersion() = 1;
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poAwaken->UpgradeStudy(wHeroKindID, byGridIdx, byLevel, pstAck->stHeroStudyInfo, pstAck->qwCurStudyExp);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byGridIdx = byGridIdx;
    pstAck->byLevel = byLevel;

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    return CLIGS_STUDY_UPGRADE_ACK;
}

//////////////////////////////////////////////////////////////////
UINT16 CGTPktBuilder::OnOpenAwakenTabReq2(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_AWAKEN_TAB_ACK2);

    CBuild* poBuild = poPlayer->GetBuild(EBK_AWAKEN);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_AWAKEN_TAB_ACK2;
    }
    poBuild->OnOpen();
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_AWAKEN_TAB_ACK2;
    }

    poAwaken->GetAwakenVersion() = 2;
    pstAck->wErrCode = poAwaken->OpenAwakenTab2(*pstAck);

    return CLIGS_OPEN_AWAKEN_TAB_ACK2;
}

UINT16 CGTPktBuilder::OnAwakenReq2(CPlayer* poPlayer, UINT8 byQuickFlag, UINT8 byGoldFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_AWAKEN_ACK2);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_AWAKEN::ID_AWAKEN_UNLOCK;
        return CLIGS_AWAKEN_ACK2;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->wErrCode = poAwaken->Awaken2(byQuickFlag, byGoldFlag, pstAck->byAwakenNum, pstAck->astAwakenGetInfo, pstAck->stAwakenTabInfo);
    pstAck->byQuickFlag = byQuickFlag;
    pstAck->byGoldFlag = byGoldFlag;

    return CLIGS_AWAKEN_ACK2;
}

UINT16 CGTPktBuilder::OnOpenDressStudyTabReq2(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_DRESS_STUDY_TAB::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_DRESS_STUDY_TAB_ACK2;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->wErrCode = poAwaken->OpenDressStudyTab2(pstAck->stDressStudyTabInfo);

    return CLIGS_OPEN_DRESS_STUDY_TAB_ACK2;

}

UINT16 CGTPktBuilder::OnDressStudyReq2(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_DRESS_STUDY_ACK2);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_DRESS_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_DRESS_STUDY_ACK2;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poAwaken->DressStudy2(wHeroKindID, byBagGridIdx, byHeroGridIdx, pstAck->astBagStudyInfo, pstAck->stHeroStudyInfo,
                       pstAck->byHaveSecondsUndress, pstAck->byBagGridIdx2, pstAck->byHeroGridIdx2);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byBagGridIdx = byBagGridIdx;
    pstAck->byHeroGridIdx = byHeroGridIdx;

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);


    return CLIGS_DRESS_STUDY_ACK2;
}

UINT16 CGTPktBuilder::OnStudyToExpReq2(CPlayer* poPlayer, UINT8 byBagGridIdx, UINT8 byAllFlag, UINT8 abySelectConvertColor[MAX_COLOR_NUM])
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_STUDY_TO_EXP_ACK2);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_STUDY_TO_EXP::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_TO_EXP_ACK2;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->wErrCode = poAwaken->ConvertExp2(byBagGridIdx, byAllFlag, abySelectConvertColor, pstAck->dwGenStudyExp, pstAck->astBagStudyInfo, pstAck->qwCurStudyExp);
    memcpy(pstAck->abySelectConvertColor, abySelectConvertColor, sizeof(pstAck->abySelectConvertColor));
    pstAck->byBagGridIdx = byBagGridIdx;
    pstAck->byAllFlag = byAllFlag;

    return CLIGS_STUDY_TO_EXP_ACK2;
}

UINT16 CGTPktBuilder::OnStudyUpgradeReq2(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byLevel)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_STUDY_UPGRADE_ACK2);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_UPGRADE_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_STUDY_UPGRADE_ACK2;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poAwaken->UpgradeStudy2(wHeroKindID, byGridIdx, byLevel, pstAck->stHeroStudyInfo, pstAck->qwCurStudyExp);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wHeroKindID = wHeroKindID;
    pstAck->byGridIdx = byGridIdx;
    pstAck->byLevel = byLevel;

    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    return CLIGS_STUDY_UPGRADE_ACK2;
}
//////////////////////////////////////////////////////////////////


UINT16 CGTPktBuilder::OnOpenClimbTowerReq(CPlayer *poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_CLIMB_TOWER_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_CLIMBTOWER);
    CBuild* poBuildAdventure = poPlayer->GetBuild(EBK_ADVENTURE);
    if((NULL == poBuild) || !poBuildAdventure)
    {
        pstAck->wErrCode = ERR_OPEN_CLIMB_TOWER::ID_CLIMB_TOWER_NOT_OPEN_ERR;
        return CLIGS_OPEN_CLIMB_TOWER_ACK;
    }
    poBuild->OnOpen();
    poBuildAdventure->OnOpen();

    CClimbTower *poClimbTower = poPlayer->GetClimbTower();
    if (NULL == poClimbTower)
    {
        pstAck->wErrCode = ERR_OPEN_CLIMB_TOWER::ID_CLIMB_TOWER_NOT_OPEN_ERR;
        return CLIGS_OPEN_CLIMB_TOWER_ACK;
    }

    poPlayer->GetInstance().GetClimbTowerInstanceAck(*pstAck);
    poClimbTower->GetClimbTowerBaseInfo(pstAck->stClimbTowerInfo.wCanResetClimbTownNum , pstAck->stClimbTowerInfo.wResetClimbTownNumPerDay);
    pstAck->stClimbTowerInfo.wBuyResetClimbTownGold = poClimbTower->GetResetClimbTowerGold((UINT8)poPlayer->GetInstance().GetCurrentClimbTower());
    poPlayer->GetEmprisePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    pstAck->qwBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;

    return CLIGS_OPEN_CLIMB_TOWER_ACK;
}


//购买重置次数请求
//UINT16 CGTPktBuilder::OnBuyClimbTowerResetReq(CPlayer *poPlayer, UINT8 byBuyNum)
//{
//	if( NULL == poPlayer )
//	{
//		return INVALID_MSGID;
//	}
//
//
//	ZERO_PACKET(PKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK* pstAck = (ZERO_PACKET(PKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK*)(GetPacketBuffer());
//	pstAck->wErrCode = poPlayer->BuyClimbTowerResetReq(byBuyNum);
//	poPlayer->GetClimbTowerBaseInfo(pstAck->wCanResetClimbTownNum , pstAck->wCanBuyResetClimbTownNum, pstAck->wBuyResetClimbTownGold);
//	pstAck->stClimbTowerInfo.wBuyResetClimbTownGold = poPlayer->GetResetClimbTowerGold();
//	pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
//	return CLIGS_BUY_CLIMB_TOWER_RESET_ACK;
//}

UINT16 CGTPktBuilder::OnOpenXiangqianTabReq( CPlayer *poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK);

    //判断是否解锁
    CBuild *pBuild = poPlayer->GetBuild( EBK_XIANGQIAN );
    if( NULL == pBuild )
    {
        pstAck->wErrCode = ERR_OPEN_XIANGQIAN_TAB::ID_XIANGQIAN_UNLOCK;
        return CLIGS_OPEN_XIANGQIAN_TAB_ACK;
    }
    pBuild->OnOpen();

    pstAck->wErrCode = ERR_OPEN_XIANGQIAN_TAB::ID_SUCCESS;
    poPlayer->GetBag().GetXiangqianGemList( pstAck->stBagItemInfo );
    CXiangqianMgr::Instance()->GetXiangqianEquipDes( pstAck->stEquipXiangqianDes.aszEquipPos1Des, pstAck->stEquipXiangqianDes.aszEquipPos2Des,
            pstAck->stEquipXiangqianDes.aszEquipPos3Des, pstAck->stEquipXiangqianDes.aszEquipPos4Des );
    return CLIGS_OPEN_XIANGQIAN_TAB_ACK;
}

UINT16 CGTPktBuilder::OnXiangqianReq( CPlayer *poPlayer, UINT16 wEquipID, UINT16 wEquipIdx, UINT16 wItemID, UINT8 byXiangqianPos, UINT16 wHeroID )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_XIANGQIAN_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = CXiangqianMgr::Instance()->OnXiangqian( pstAck->stXiangqianData.stEquip, poPlayer, wEquipID, wEquipIdx, wItemID, byXiangqianPos, wHeroID );
    CTaskMgr::Instance()->OnMosaicGem(poPlayer);

    poPlayer->GetBag().GetXiangqianGemList( pstAck->stXiangqianData.stBagItemInfo );
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    return CLIGS_XIANGQIAN_ACK;
}

UINT16 CGTPktBuilder::OnRemoveXiangqianReq( CPlayer *poPlayer, UINT16 wHeroID, UINT16 wEquipID, UINT16 wIdx, UINT8 byPos, BOOL bRemoveAll )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_REMOVE_XIANGQIAN_ACK);

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    //判断是否解锁
    CBuild *pBuild = poPlayer->GetBuild( EBK_XIANGQIAN );
    if( NULL == pBuild )
    {
        pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_BUILD_UNLOCK;
        return CLIGS_REMOVE_XIANGQIAN_ACK;
    }

    CEquip *pEquip = poPlayer->GetEquip( wEquipID, wIdx );
    if( NULL == pEquip )
    {
        pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_EQUIP_NOT_EXIST;
        return CLIGS_REMOVE_XIANGQIAN_ACK;
    }

    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    if( !bRemoveAll )
    {
        if( byPos >= MAX_XIANGQIAN_NUM )
        {
            pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_EQUIP_NOT_XIANGQIAN;
            return CLIGS_REMOVE_XIANGQIAN_ACK;
        }

        UINT16 wItemID = pEquip->GetDT_EQUIP_DATA().astXiangqianList[byPos].wItemID;
        if( 0 == wItemID )
        {
            //返回判断不存在错误
            pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_EQUIP_NOT_XIANGQIAN;
            return CLIGS_REMOVE_XIANGQIAN_ACK;
        }

        ECreateItemRet ret;
        CItemMgr::Instance()->CreateItemIntoBag( poPlayer, wItemID, 1, ret, 0, CRecordMgr::EAIT_REMOVEXIANGQIAN );

        //判断格子是否足够
        if( ret == ECIR_SUCCESS )
        {
            //成功
            pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_SUCCESS;
            pEquip->GetDT_EQUIP_DATA().astXiangqianList[byPos].wItemID = 0;

            CHero *pHero = pEquip->GetHero();
            if( pHero )
            {
                pHero->InitBattleAttribute();
                poPlayer->ResetBattleAttr();
            }

        }
        else
        {
            SYS_CRITICAL( _SDT( "[%s %d] CGTPktBuilder::OnRemoveXiangqianReq bag is full, player:%u errorcode:%d" ), MSG_MARK, poPlayer->GetID(), ret );
            pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_CHECK_BAG_AREA;
        }

        poPlayer->GetBag().GetXiangqianGemList( pstAck->stXiangqianData.stBagItemInfo );
        pEquip->GetDT_EQUIP_DATA_CLI( pstAck->stXiangqianData.stEquip );

        pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
        return CLIGS_REMOVE_XIANGQIAN_ACK;
    }

    else
    {
        INT32 nSuccessNum = 0;
        pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_SUCCESS;
        ECreateItemRet ret = ECIR_SUCCESS;
        for( UINT32 dwIndex = 0; dwIndex < MAX_XIANGQIAN_NUM; ++dwIndex )
        {

            UINT16 wItemID = pEquip->GetDT_EQUIP_DATA().astXiangqianList[dwIndex].wItemID;
            if( 0 == wItemID )
            {
                continue;
            }

            ECreateItemRet curRet;
            CItemMgr::Instance()->CreateItemIntoBag( poPlayer, wItemID, 1, curRet, 0, CRecordMgr::EAIT_REMOVEXIANGQIAN );

            //判断格子是否足够
            if( curRet == ECIR_SUCCESS )
            {
                nSuccessNum++;
                //成功

                pEquip->GetDT_EQUIP_DATA().astXiangqianList[dwIndex].wItemID = 0;
            }
            else
            {
                ret = curRet;
                SYS_CRITICAL( _SDT( "[%s %d] CGTPktBuilder::OnRemoveXiangqianReq CreateItemIntoBag failed, player ID:%u opret:%d" ), MSG_MARK, poPlayer->GetID(), ret );
            }
        }

        if( nSuccessNum != 0 )
        {
            CHero *pHero = pEquip->GetHero();
            if( pHero )
            {
                pHero->InitBattleAttribute();
                poPlayer->ResetBattleAttr();
            }
        }

        if( ret != ECIR_SUCCESS )
        {
            pstAck->wErrCode = ERR_REMOVE_XIANGQIAN::ID_CHECK_BAG_AREA;
        }
        pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
        poPlayer->GetBag().GetXiangqianGemList( pstAck->stXiangqianData.stBagItemInfo );
        pEquip->GetDT_EQUIP_DATA_CLI( pstAck->stXiangqianData.stEquip );
        return CLIGS_REMOVE_XIANGQIAN_ACK;
    }
}

UINT16	CGTPktBuilder::OnAppIDCheckError( UINT16 wErrorCode, UINT32 dwPlayerID, string strTransactionID)
{


    ZERO_PACKET(PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK);
    pstAck->wErrCode = wErrorCode;
    pstAck->dwPlayerID = dwPlayerID;
    memcpy( pstAck->szTransactionID, strTransactionID.c_str(), strTransactionID.size() );

    return CLIGS_CHECK_APPSTORE_ORDER_ID_ACK;
}


UINT16 CGTPktBuilder::OnResetEliteInstance( CPlayer *poPlayer, UINT8 byIndex, UINT16 wRemainRetNum )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_RESET_ELITE_INSTANCE_ACK);

    pstAck->wErrCode = ERR_RESET_ELITE_INSTANCE::ID_SUCCESS;
    if( poPlayer->GetVipExtData().GetRemainResetEliteInstanceNum( byIndex ) != wRemainRetNum )
    {
        poPlayer->GetInstance().GetEliteInstanceData( *pstAck, byIndex );
        return CLIGS_RESET_ELITE_INSTANCE_ACK;
    }

    pstAck->wErrCode = poPlayer->GetInstance().ResetEliteTime( byIndex );

    if( pstAck->wErrCode == ERR_RESET_ELITE_INSTANCE::ID_SUCCESS )
    {
        poPlayer->GetInstance().GetEliteInstanceData( *pstAck, byIndex );
    }

    return CLIGS_RESET_ELITE_INSTANCE_ACK;
}

UINT16 CGTPktBuilder::OnEliteInstanceOnHook( CPlayer *poPlayer, UINT8 byIndex )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK);

    pstAck->wErrCode = COnhookMgr::Instance()->OnhookElitInstanceByTownIndex( poPlayer, byIndex, pstAck );
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);

    return CLIGS_ELITE_INSTANCE_ON_HOOK_ACK;
}

UINT16 CGTPktBuilder::OnGetEliteInstance( CPlayer *poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_GET_ELITE_INSTANCE_ACK);

    CBuild* poBuild = poPlayer->GetBuild(EBK_ELITEINSTANCE);
    CBuild* poBuildAdventure = poPlayer->GetBuild(EBK_ADVENTURE);
    if((NULL == poBuild) || !poBuildAdventure)
    {
        pstAck->wErrCode = ERR_GET_SCENEINSTANCE::ID_SCENE_NOT_EXIST_ERR;
        return CLIGS_GET_ELITE_INSTANCE_ACK;
    }
    poBuild->OnOpen();
    poBuildAdventure->OnOpen();

    //查找是否存在记录
    poPlayer->GetInstance().GetEliteInstanceRecordAck(*pstAck);

    return CLIGS_GET_ELITE_INSTANCE_ACK;
}


UINT16 CGTPktBuilder::OnGetActivityPromptReq( CPlayer *poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }



    ZERO_PACKET(PKT_CLIGS_GET_ACTIVITY_PROMPT_ACK);

    pstAck->wErrCode = ERR_GET_ACTIVITY_PROMPT::ID_SUCCESS;
    poPlayer->GetActivityCenterPrompt(pstAck->stPromptInfo);

    return CLIGS_GET_ACTIVITY_PROMPT_ACK;
}

UINT16 CGTPktBuilder::OnBlueToPurpleReq( CPlayer *poPlayer, UINT32 dwBlueGas)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }



    //ZERO_PACKET(PKT_CLIGS_BLUE_TO_PURPLE_ACK *pstAck=(ZERO_PACKET(PKT_CLIGS_BLUE_TO_PURPLE_ACK *)(GetPacketBuffer());

    //pstAck->wErrCode = ERR_GET_ACTIVITY_PROMPT::ID_SUCCESS;
    //CJingJie* poJingJie = poPlayer->GetJingJie();
    //if(NULL == poJingJie)
    //{
    //	pstAck->wErrCode = ERR_BLUE_TO_PURPLE::ID_JINGJIE_UNLOCK;
    //	return CLIGS_UPGRADE_JINGJIE_ACK;
    //}

    //pstAck->wErrCode = poJingJie->BlueToPurp(dwBlueGas, pstAck->qwCurBlueGas, pstAck->qwCurPurpleGas);

    return CLIGS_BLUE_TO_PURPLE_ACK;
}

UINT16 CGTPktBuilder::OnPKBattleReq(CPlayer *poMyself, CPlayer *poEnemy)
{
    if(NULL == poMyself)
    {
        return INVALID_MSGID;
    }

    CUser* poUser = poMyself->GetUser();
	if ( (poUser) && (poUser->GetCliVer() >= VERSION_PET) )
	{
		return OnPKBattleReq5(poMyself, poEnemy);
	}
	else if ( (poUser) && (poUser->GetCliVer() >= VERSION_SOUL))
	{
		return OnPKBattleReq4(poMyself, poEnemy);
	}
	else if ((poUser) && (poUser->GetCliVer() > 310))
	{
		return OnPKBattleReq3(poMyself, poEnemy);
	}

    
    return OnPKBattleReq5(poMyself, poEnemy);
}

UINT16 CGTPktBuilder::OnPKBattleReq5(CPlayer *poMyself, CPlayer *poEnemy)
{
    if(NULL == poMyself)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_PKBATTLE_ACK5);
    pstAck->wErrCode = ERR_PKBATTLE::ID_SUCCESS;

    if(NULL == poEnemy)
    {
        pstAck->wErrCode = ERR_PKBATTLE::ID_ENEMY_NOT_EXIST;
        return CLIGS_PKBATTLE_ACK5;
    }

    if(poEnemy == poMyself)
    {
        pstAck->wErrCode = ERR_PKBATTLE::ID_ENEMY_MYSELF;
        return CLIGS_PKBATTLE_ACK5;
    }

    if(!CBattleMgr::Instance()->Battle4(poMyself, poEnemy, pstAck->stBattleInfo, TRUE))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_PKBATTLE_ACK5;
    }


    return CLIGS_PKBATTLE_ACK5;
}

UINT16 CGTPktBuilder::OnPKBattleReq3(CPlayer *poMyself, CPlayer *poEnemy)
{
    if(NULL == poMyself)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_PKBATTLE_ACK3);
    pstAck->wErrCode = ERR_PKBATTLE::ID_SUCCESS;

    if(NULL == poEnemy)
    {
        pstAck->wErrCode = ERR_PKBATTLE::ID_ENEMY_NOT_EXIST;
        return CLIGS_PKBATTLE_ACK3;
    }

    if(poEnemy == poMyself)
    {
        pstAck->wErrCode = ERR_PKBATTLE::ID_ENEMY_MYSELF;
        return CLIGS_PKBATTLE_ACK3;
    }

    if(!CBattleMgr::Instance()->Battle2(poMyself, poEnemy, pstAck->stBattleInfo, TRUE))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_PKBATTLE_ACK3;
    }

    return CLIGS_PKBATTLE_ACK3;
}

UINT16	CGTPktBuilder::OnPKBattleReq4(CPlayer *poMyself, CPlayer *poEnemy)
{
	if(NULL == poMyself)
	{
		return INVALID_MSGID;
	}


	ZERO_PACKET(PKT_CLIGS_PKBATTLE_ACK4);
	pstAck->wErrCode = ERR_PKBATTLE::ID_SUCCESS;

	if(NULL == poEnemy)
	{
		pstAck->wErrCode = ERR_PKBATTLE::ID_ENEMY_NOT_EXIST;
		return CLIGS_PKBATTLE_ACK4;
	}

	if(poEnemy == poMyself)
	{
		pstAck->wErrCode = ERR_PKBATTLE::ID_ENEMY_MYSELF;
		return CLIGS_PKBATTLE_ACK4;
	}

	if(!CBattleMgr::Instance()->Battle3(poMyself, poEnemy, pstAck->stBattleInfo, TRUE))
	{
		SET_OTHER_ERR(pstAck->wErrCode);
		return CLIGS_PKBATTLE_ACK4;
	}

	return CLIGS_PKBATTLE_ACK4;
}
UINT16 CGTPktBuilder::OnResourcesPromptReq( CPlayer *poPlayer, UINT32 dwRsID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_RESOURCES_PROMPT_ACK);
    switch(dwRsID)
    {
    case ERS_COIN:
        poPlayer->GetCoinResourcePrompt(pstAck->stRsPromptData);
        break;
    case ERS_STORY:
        poPlayer->GetStoryResourcePrompt(pstAck->stRsPromptData);
        break;
    case ERS_SCIENCE:
        poPlayer->GetScienceResourcePrompt(pstAck->stRsPromptData);
        break;
    case ERS_JINGJIE:
        poPlayer->GetJingJieResourcePrompt(pstAck->stRsPromptData);
        break;
    default:
        break;
    }
    pstAck->wErrCode = 0;


    return CLIGS_RESOURCES_PROMPT_ACK;
}

UINT16 CGTPktBuilder::OnEnterBossbReq(CPlayer *poPlayer, UINT16 wPosX, UINT16 wPosY)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	if ( VER_PET <= poPlayer->GetCliVer())
	{
		ZERO_PACKET(PKT_ENTER_BOSSB_ACK2);
		pstAck->wErrCode = CBossBattleMgr::Instance()->OnEnter(poPlayer, wPosX, wPosY, pstAck->stMyBossBInfo, pstAck->stPlayerInfo, pstAck->stBossBInfo, pstAck->astTopHurtInfo, pstAck->stHurtEncInfo);
		poPlayer->GetCliVerInfo(pstAck->dwCliVer, pstAck->dwResVer, pstAck->dwUIVer, pstAck->aszResVer);
		return ENTER_BOSSB_ACK2;
	}
	else
	{
		ZERO_PACKET(PKT_ENTER_BOSSB_ACK);
		DT_BOSSB_DATA_CLI2 stBossInfo;
		pstAck->wErrCode = CBossBattleMgr::Instance()->OnEnter(poPlayer, wPosX, wPosY, pstAck->stMyBossBInfo, pstAck->stPlayerInfo, stBossInfo, pstAck->astTopHurtInfo, pstAck->stHurtEncInfo);
		pstAck->stBossBInfo.dwCurBossHP = (UINT32)stBossInfo.qwCurBossHP;
		pstAck->stBossBInfo.dwHPPerGrid = (UINT32)stBossInfo.qwHPPerGrid;
		pstAck->stBossBInfo.dwPlayerNum = stBossInfo.dwPlayerNum;
		pstAck->stBossBInfo.dwTotalBossHP = (UINT32)stBossInfo.qwTotalBossHP;
		pstAck->stBossBInfo.wBossID = stBossInfo.wBossID;
		pstAck->stBossBInfo.wBossLevel = stBossInfo.wBossLevel;
		pstAck->stBossBInfo.wReadyCountDown = stBossInfo.wReadyCountDown;
		pstAck->stBossBInfo.wReliveGold = stBossInfo.wReliveGold;
		pstAck->stBossBInfo.wStopCountDown = stBossInfo.wStopCountDown;
		poPlayer->GetCliVerInfo(pstAck->dwCliVer, pstAck->dwResVer, pstAck->dwUIVer, pstAck->aszResVer);
		return ENTER_BOSSB_ACK;
	}

}

UINT16 CGTPktBuilder::OnLeaveBossbReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_LEAVE_BOSSB_ACK);
    pstAck->wErrCode = CBossBattleMgr::Instance()->OnLeave(poPlayer);

    return LEAVE_BOSSB_ACK;
}


UINT32 CGTPktBuilder::OnAppPayCheckReq( CPlayer *poPlayer, const string &strOrderID, const string &strReceiptData, UINT32 dwTransID, UINT32 dwPayPlayerID )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK);
    pstAck->wErrCode = ERR_CHECK_APPSTORE_ORDER::ID_ORDER_ID_ERROR;
    USR_INFO( "Query PlayerID:%u OrderID:%s strReceiptData:%s PayPlayerID:%u", poPlayer->GetDT_PLAYER_BASE_DATA().dwID, strOrderID.c_str(), strReceiptData.c_str(), dwPayPlayerID );

    //CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SETPAYINFO);
    //if(NULL == poCMD)
    //{
    //    SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
    //    return FALSE;
    //}
    //SPayInfo payInfo;
    //if(0 == dwPayPlayerID)
    //{
    //    payInfo.dwPlayerID = poPlayer->GetID();
    //}
    //else
    //{
    //    payInfo.dwPlayerID = dwPayPlayerID;
    //}
    //payInfo.dwPlayerLevel = poPlayer->GetLevel();
    //payInfo.dwQueryPlayerID = poPlayer->GetID();
    //CUser *pUser = poPlayer->GetUser();
    //if( !pUser )
    //{
    //    return INVALID_MSGID;
    //}


    //payInfo.dwUserID		= pUser->GetUserID();
    //payInfo.dwZoneID		= pUser->GetZoneID();
    //payInfo.strOrderID	= strOrderID;
    //payInfo.strReceipt	= strReceiptData;
    //payInfo.strSource		= "AppStore";
    //payInfo.strTableName	= CGSConfig::Instance()->GetAppPayTableName();
    //payInfo.strDeviceID	= pUser->GetDeviceID();

    //poCMD->SetUserData( &payInfo, dwTransID );

    //gsapi::GetDBMgr()->AddPayCommand( poCMD );
    return CLIGS_CHECK_APPSTORE_ORDER_ID_ACK;
}

UINT32 CGTPktBuilder::OnAppPaySuccess( UINT8 byVipLevel, UINT32 dwTotalAmount, UINT64 qwAllGold, UINT32 dwExchangeGold, const string &strTransactionID, UINT32 dwPlayerID)
{

    ZERO_PACKET(PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK);

    pstAck->byVipLevel = byVipLevel;
    pstAck->dwExchangeGold = dwExchangeGold;
    pstAck->dwTotalExchangeGold = dwTotalAmount;
    pstAck->qwGold = qwAllGold;
    pstAck->dwPlayerID = dwPlayerID;
    pstAck->wErrCode = ERR_CHECK_APPSTORE_ORDER::ID_SUCCESS;
    memcpy( pstAck->szTransactionID, strTransactionID.c_str(), strTransactionID.size() );

    return CLIGS_CHECK_APPSTORE_ORDER_ID_ACK;
}

UINT16 CGTPktBuilder::OnBossbMoveReq(CPlayer *poPlayer, UINT16 wPosX, UINT16 wPosY)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_BOSSB_MOVE_ACK);
    pstAck->wErrCode = CBossBattleMgr::Instance()->OnMove(poPlayer, wPosX, wPosY, pstAck->wOldPosX, pstAck->wOldPosY);
    pstAck->wOldPosX = wPosX;
    pstAck->wOldPosY = wPosY;

    return BOSSB_MOVE_ACK;
}

UINT16 CGTPktBuilder::OnBossbHurtReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	if ( VER_PET <= poPlayer->GetCliVer())
	{
		ZERO_PACKET(PKT_BOSSB_HURT_ACK2);
		pstAck->wErrCode = CBossBattleMgr::Instance()->OnHurt(poPlayer, pstAck->dwCurHurtValue, pstAck->stMyHurtInfo, pstAck->astTopHurtInfo,
			pstAck->stHurtEncInfo, pstAck->wReliveCountDown,  pstAck->wGoldReliveCountDown, pstAck->qwTotalBossHP, pstAck->qwCurBossHP);

		return BOSSB_HURT_ACK2;
	}
	else
	{
		ZERO_PACKET(PKT_BOSSB_HURT_ACK);
		UINT64 qwTotalBossHP;
		UINT64 qwCurBossHP;
		pstAck->wErrCode = CBossBattleMgr::Instance()->OnHurt(poPlayer, pstAck->dwCurHurtValue, pstAck->stMyHurtInfo, pstAck->astTopHurtInfo,
			pstAck->stHurtEncInfo, pstAck->wReliveCountDown,  pstAck->wGoldReliveCountDown, qwTotalBossHP, qwCurBossHP);
		pstAck->dwTotalBossHP = (UINT32)qwTotalBossHP;
		pstAck->dwCurBossHP = (UINT32)qwCurBossHP;

		return BOSSB_HURT_ACK;
	}
}

UINT16 CGTPktBuilder::OnOpenInspireTabReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_OPEN_INSPIRE_TAB_ACK);
    pstAck->wErrCode = CBossBattleMgr::Instance()->OnOpenInspireTab(poPlayer, pstAck->stInspireTabInfo);

    return OPEN_INSPIRE_TAB_ACK;
}

UINT16 CGTPktBuilder::OnInspirePowerReq(CPlayer *poPlayer, UINT8 byGoldInspireFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_INSPIRE_POWER_ACK);
    pstAck->wErrCode = CBossBattleMgr::Instance()->OnInspirePower(poPlayer, byGoldInspireFlag, pstAck->stInspireTabInfo);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCurStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;

    return INSPIRE_POWER_ACK;
}

UINT16 CGTPktBuilder::OnBossBReliveReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_BOSSB_RELIVE_ACK);
    pstAck->wErrCode = CBossBattleMgr::Instance()->OnBossBRelive(poPlayer);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return BOSSB_RELIVE_ACK;
}

UINT16 CGTPktBuilder::OnGetLastBossBInfoReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

	if ( VER_PET <= poPlayer->GetCliVer())
	{
		ZERO_PACKET(PKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2);
		pstAck->wErrCode = CBossBattleMgr::Instance()->OnGetLastBossBInfo(poPlayer, pstAck->stBossOverInfo);
		const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
		pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
		pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
		pstAck->qwCurStory = stDT_PLAYER_BASE_DATA.qwStory;
		pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;

		return BOSSB_GET_LAST_BOSSB_INFO_ACK2;
	}
	else
	{
		ZERO_PACKET(PKT_BOSSB_GET_LAST_BOSSB_INFO_ACK);
		DT_BOSSB_OVER_INFO2 stInfo2;
		pstAck->wErrCode = CBossBattleMgr::Instance()->OnGetLastBossBInfo(poPlayer, stInfo2);
		CBossBattleMgr::Instance()->ConvertInfo2ToInfo(stInfo2, pstAck->stBossOverInfo);
		const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
		pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
		pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
		pstAck->qwCurStory = stDT_PLAYER_BASE_DATA.qwStory;
		pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;

		return BOSSB_GET_LAST_BOSSB_INFO_ACK;
	}
    
}

UINT16 CGTPktBuilder::OnOpenBossBBLReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK);
    CBossBattle* poBossB = poPlayer->GetBossBattle();
    if(NULL == poBossB)
    {
        pstAck->wErrCode = ERR_OPEN_BOSSB_BATTLE_LOG::ID_BOSSB_UNLOCK;
        return CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK;
    }
    pstAck->wErrCode = poBossB->GetShowBossBBattleLog(pstAck->byNum, pstAck->astBossBattleBL);

    return CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK;
}

UINT16 CGTPktBuilder::OnOpenRechargeEncourageReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK);
    CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
    pstAck->wErrCode = oActivityEncourage.OpenRechargeEncourage(*pstAck);
    return CLILS_OPEN_RECHARGE_ENCOURAGE_ACK;
}

UINT16 CGTPktBuilder::OnRecvRechargeEncourageReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK);
    CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
    pstAck->wErrCode = oActivityEncourage.RecvRechargeEncourage(*pstAck);

    return CLIGS_RECV_RECHARGE_ENCOURAGE_ACK;
}

UINT16	CGTPktBuilder::OnOpenScoreEncourageReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK);
    CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
    pstAck->wErrCode = oActivityEncourage.OpenScoreEncourage(*pstAck);
    return CLILS_OPEN_SCORE_ENCOURAGE_ACK;
}

UINT16	CGTPktBuilder::OnRecvScoreEncourageReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK);
    CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
    pstAck->wErrCode = oActivityEncourage.RecvScoreEncourage(*pstAck);

    return CLIGS_RECV_SCORE_ENCOURAGE_ACK;
}

UINT16 CGTPktBuilder::OnGetRandomDspNameReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GET_RANDOM_DSPNAME_ACK);
    pstAck->wErrCode = ERR_GET_RANDOM_DSPNAME::ID_SUCCESS;
    SDStrncpy(pstAck->aszDspName, CCommonInfoMgr::Instance()->GetRdDspName(poPlayer).c_str(), USERNAME_LEN - 1);

    return CLIGS_GET_RANDOM_DSPNAME_ACK;
}

UINT16 CGTPktBuilder::OnOpenExperienceTransferReq( CPlayer *poPlayer, UINT16 wHeroID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK);
    pstAck->wErrCode =  poPlayer->OpenExperienceTransfer(wHeroID, *pstAck);
    return CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK;
}

UINT16 CGTPktBuilder::OnExperienceTransferReq( CPlayer *poPlayer, UINT16 wHeroID, UINT16 wTransHeroID, UINT8 byGold)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_EXPERIENCE_TRANSFER_ACK);
    pstAck->stPowerChangeData.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode =  poPlayer->ExperienceTransfer(wHeroID, wTransHeroID, byGold, pstAck->stHeroInfo, pstAck->stTransHeroInfo);
    pstAck->stPowerChangeData.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(pstAck->stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    return CLIGS_EXPERIENCE_TRANSFER_ACK;
}

UINT16 CGTPktBuilder::OnOpenRdchallengeTabReq( CPlayer *poPlayer, UINT8 byRefreshFlag)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK);
    CBuild* poBuild = poPlayer->GetBuild(EBK_RDCHALLENGE);
    if(!poBuild)
    {
        return ERR_OPEN_RDCHALLENGE_TAB::ID_UNLOCK;
    }
    poBuild->OnOpen();


    CRdChallenge *poRdChallenge = poPlayer->GetRdChallenge();
    if (NULL == poRdChallenge)
    {
        return ERR_OPEN_RDCHALLENGE_TAB::ID_UNLOCK;
    }


    pstAck->wErrCode = poRdChallenge->OpenRdchallengeTab(byRefreshFlag, pstAck->stTabInfo);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    pstAck->qwCurPurpleGas = stDT_PLAYER_BASE_DATA.qwPurpleGas;
    CWorship *poWorship = poPlayer->GetWorship();
    if (poWorship)
        pstAck->byHaveWorship = poWorship->CKCanWorship(EWT_RDC, FALSE) ? 0 : 1;

    return CLIGS_OPEN_RDCHALLENGE_TAB_ACK;
}

UINT16 CGTPktBuilder::OnRdchallengeReq(CPlayer *poPlayer, CPlayer * poEnemyPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    CUser* poUser = poPlayer->GetUser();
	if ( (poUser) && (poUser->GetCliVer() >= VERSION_PET) )
	{
		return OnRdchallengeReq5(poPlayer, poEnemyPlayer);
	}
	else if ( (poUser) && (poUser->GetCliVer() >= VERSION_SOUL))
	{
		return OnRdchallengeReq4(poPlayer, poEnemyPlayer);
	}
	else if ((poUser) && (poUser->GetCliVer() > 310))
	{
		return OnRdchallengeReq3(poPlayer, poEnemyPlayer);
	}

  
    return OnRdchallengeReq5(poPlayer, poEnemyPlayer);
}

UINT16 CGTPktBuilder::OnRdchallengeReq5(CPlayer *poPlayer, CPlayer * poEnemyPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RDCHALLENGE_ACK5);
    if(!poPlayer->GetBuild(EBK_RDCHALLENGE))
    {
        return ERR_RDCHALLENGE::ID_UNLOCK;
    }

    CRdChallenge *poRdChallenge = poPlayer->GetRdChallenge();
    if (NULL == poRdChallenge)
    {
        return ERR_RDCHALLENGE::ID_UNLOCK;
    }

    pstAck->wErrCode = poRdChallenge->Rdchallenge(poEnemyPlayer, pstAck->stBattleInfo, pstAck->stTabInfo, pstAck->stEncInfo, pstAck->wKindID);
    pstAck->qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    pstAck->qwCurPurpleGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_RDCHALLENGE_ACK5;
}

UINT16 CGTPktBuilder::OnRdchallengeReq3(CPlayer *poPlayer, CPlayer * poEnemyPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_RDCHALLENGE_ACK3);
    if(!poPlayer->GetBuild(EBK_RDCHALLENGE))
    {
        pstAck->wErrCode = ERR_RDCHALLENGE::ID_UNLOCK;
        return CLIGS_RDCHALLENGE_ACK3;
    }
    CRdChallenge *poRdChallenge = poPlayer->GetRdChallenge();
    if (NULL == poRdChallenge)
    {
        pstAck->wErrCode = ERR_RDCHALLENGE::ID_UNLOCK;
        return CLIGS_RDCHALLENGE_ACK3;
    }
    pstAck->wErrCode = poRdChallenge->Rdchallenge2(poEnemyPlayer, pstAck->stBattleInfo, pstAck->stTabInfo, pstAck->stEncInfo,pstAck->wKindID);
    pstAck->qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    pstAck->qwCurPurpleGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_RDCHALLENGE_ACK3;
}

UINT16	CGTPktBuilder::OnRdchallengeReq4(CPlayer *poPlayer, CPlayer * poEnemyPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_RDCHALLENGE_ACK4);
	if(!poPlayer->GetBuild(EBK_RDCHALLENGE))
	{
		pstAck->wErrCode = ERR_RDCHALLENGE::ID_UNLOCK;
		return CLIGS_RDCHALLENGE_ACK4;
	}
	CRdChallenge *poRdChallenge = poPlayer->GetRdChallenge();
	if (NULL == poRdChallenge)
	{
		pstAck->wErrCode = ERR_RDCHALLENGE::ID_UNLOCK;
		return CLIGS_RDCHALLENGE_ACK4;
	}
	pstAck->wErrCode = poRdChallenge->Rdchallenge3(poEnemyPlayer, pstAck->stBattleInfo, pstAck->stTabInfo, pstAck->stEncInfo,pstAck->wKindID);
	pstAck->qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
	pstAck->qwCurPurpleGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
	poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

	return CLIGS_RDCHALLENGE_ACK4;
}
UINT16 CGTPktBuilder::OnClrRDChallengeCDReq(CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_CLR_RDCHALLENGE_CD_ACK);
    if(!poPlayer->GetBuild(EBK_RDCHALLENGE))
    {
        pstAck->wErrCode = ERR_CLR_RDCHALLENGE_CD::ID_UNLOCK;
        return CLIGS_CLR_RDCHALLENGE_CD_ACK;
    }

    CRdChallenge *poRdChallenge = poPlayer->GetRdChallenge();
    if (NULL == poRdChallenge)
    {
        pstAck->wErrCode = ERR_CLR_RDCHALLENGE_CD::ID_UNLOCK;
        return CLIGS_CLR_RDCHALLENGE_CD_ACK;
    }

    pstAck->wErrCode = poRdChallenge->ClrRDChallengeCD();
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_CLR_RDCHALLENGE_CD_ACK;
}


UINT16 CGTPktBuilder::OnOpenWorshipTabReq(CPlayer *poPlayer, UINT8 byWorshipType)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_WORSHIP_TAB_ACK);
    CWorship *poWorship = poPlayer->GetWorship();
    if(NULL == poWorship)
    {
        pstAck->wErrCode = ERR_OPEN_WORSHIP_TAB::ID_UNLOCK;
        return CLIGS_OPEN_WORSHIP_TAB_ACK;
    }

    pstAck->wErrCode = poWorship->OpenWorshipTab(byWorshipType, pstAck->stWorshipInfo);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    pstAck->qwCurPurpleGas = stDT_PLAYER_BASE_DATA.qwPurpleGas;

    return CLIGS_OPEN_WORSHIP_TAB_ACK;
}

UINT16 CGTPktBuilder::OnWorshipReq(CPlayer *poPlayer, UINT8 byWorshipType, UINT8 byWorshipTypeLevel)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_WORSHIP_ACK);
    CWorship *poWorship = poPlayer->GetWorship();
    if(NULL == poWorship)
    {
        pstAck->wErrCode = ERR_WORSHIP::ID_UNLOCK;
        return CLIGS_OPEN_WORSHIP_TAB_ACK;
    }


    pstAck->wErrCode = poWorship->Worship(byWorshipType, byWorshipTypeLevel, pstAck->stWorshipInfo);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    pstAck->qwCurPurpleGas = stDT_PLAYER_BASE_DATA.qwPurpleGas;
    poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_WORSHIP_ACK;
}


UINT16 CGTPktBuilder::GenGUIDAck2GT(string strOrderID, UINT16 wErrCode)
{

    ZERO_PACKET(PKT_CLIGS_GET_ORDER_ID_ACK);
    SDStrncpy( pstAck->aszOrderID, strOrderID.c_str(), MAX_ORDER_ID_LEN - 1);
    pstAck->wErrCode = wErrCode;

    USR_INFO( _SDT( "get order id,ret:%d" ), pstAck->wErrCode );
    return CLIGS_GET_ORDER_ID_ACK;
}

UINT16	CGTPktBuilder::OnActivityNotifyReq(CPlayer *poPlayer, UINT8 byActivityType, UINT8 byNotify)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_ACTIVITY_NOTIFY_ACK);
    pstAck->wErrCode = poPlayer->ActivityNotifyReq(byActivityType, byNotify);
    return CLIGS_ACTIVITY_NOTIFY_ACK;
}

UINT16	CGTPktBuilder::OnOpenDogFecesReq(CPlayer *poPlayer, UINT8 byType)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_DOGFECES_ACK);
    pstAck->wErrCode = poPlayer->OpenDogFeces(byType, pstAck->byNum, pstAck->astDogFecesBuildTip, pstAck->stPromptInfo);

    return CLIGS_OPEN_DOGFECES_ACK;
}

UINT16 CGTPktBuilder::OnGetActivityCenterReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_GET_ACTIVITY_CENTER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPNE_ACTIVITY_CENTER::ID_UNLOCK;
        return CLIGS_GET_ACTIVITY_CENTER_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = poPlayer->GetActivityCenterData(pstAck->byActivityNum, pstAck->astActivityInfo, pstAck->abyActivityStartCountDown);

    return CLIGS_GET_ACTIVITY_CENTER_ACK;

}

UINT16	CGTPktBuilder::OnGetActivityEncourageInfoReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY_ENCOURAGE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_ACTIVITY_ENCOURRAGE_INFO::ID_SUCCESS;
        return CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK;
    }
    poBuild->OnOpen();
    CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
    pstAck->wErrCode = oActivityEncourage.GetActivityEncourageInfo(*pstAck);
    return CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK;
}

UINT16	CGTPktBuilder::OnGetActivityEncourageInfoAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY_ENCOURAGE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_ACTIVITY_ENCOURRAGE_INFO::ID_SUCCESS;
        return CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2;
    }
    poBuild->OnOpen();
    CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
    pstAck->wErrCode = oActivityEncourage.GetActivityEncourageInfo(*pstAck);

    return CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2;
}

UINT16	CGTPktBuilder::OnRecvActivityEncourageAck(CPlayer* poPlayer, UINT16 wActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM], UINT16 wErrCode)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}


	ZERO_PACKET(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK);
	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}
	if ( wErrCode != ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS)
	{
		pstAck->wErrCode = wErrCode;
		return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY_ENCOURAGE);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS;
		return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
	}
	poBuild->OnOpen();

		//奖励信息是否存在。
		SActivityEncouragePro  * poActivityEncouragePro = CActivityEncourageMgr::Instance()->GetActivityEncouragePro(wActivityID);
		if (NULL == poActivityEncouragePro)
		{
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_ACTIVITY_NOT_EXIT;
			return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
		}


		//检测是否已领取奖励信息
		if (!poPlayer->GetActivityEncourage().CkActivityEncourage(wActivityID))
		{
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_ENCOURRAGE_RECVD;
			return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
		}

		vector<UINT16> vecItemID;
		UINT32 unSoulNum = 0;
		for(CJsonResItemVectorItr itr = poActivityEncouragePro->vecActivityEncourageItemInfo.vecJsonResItem.begin(); itr != poActivityEncouragePro->vecActivityEncourageItemInfo.vecJsonResItem.end(); itr++)
		{
			CJsonResItem &stActivityEncourageItem =  *itr;
			if (ACTE_GOOD == stActivityEncourageItem.byKindType)
			{
				vecItemID.push_back(stActivityEncourageItem.dwKindValue);
			}

			if (ACTE_SOUL == stActivityEncourageItem.byKindType) {
				unSoulNum += stActivityEncourageItem.byNum;
			}
		}

		//检测背包是否满了
		if(CItemMgr::Instance()->CkBagFull(poPlayer, vecItemID))
		{
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_BAGFUL;
			return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
		}

		// 检查是否可塞入战魂背包
		if (unSoulNum > 0) {
			CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
			if (NULL == pPlayerSoul || pPlayerSoul->GetSoulBagLeftSize() < unSoulNum) {
				pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_BAGFUL;
				return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
			}
		}

		if(CActivityEncourageMgr::Instance()->IsNeedCdKey(wActivityID))
		{
			CActivityEncourageMgr::Instance()->CkActivityCdKeyValid(poPlayer->GetID(), wActivityID, aszSeriesNo);
		}

		CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
		pstAck->wErrCode = oActivityEncourage.RecvActivityEncourage(*pstAck, wActivityID, aszSeriesNo, FALSE);


	pstAck->wActivityIDOld = wActivityID;
	pstAck->stRsyncRecvEncourageData.qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	pstAck->stRsyncRecvEncourageData.qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stRsyncRecvEncourageData.stCurPhyStrengthInfo);
	pstAck->stRsyncRecvEncourageData.qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
	pstAck->stRsyncRecvEncourageData.qwCurStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
	pstAck->stRsyncRecvEncourageData.qwCurBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
	pstAck->stRsyncRecvEncourageData.qwCurPurpleGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
	pstAck->stRsyncRecvEncourageData.qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
	poPlayer->GetPrompt(pstAck->stRsyncRecvEncourageData.stPromptInfo);
	poPlayer->GetNewEquipPrompt(pstAck->stRsyncRecvEncourageData.stNewEquipPromptInfo);


	//检测是否可取奖励信息
	if (!poPlayer->GetActivityEncourage().CkActivityEncourage(wActivityID))
	{
		pstAck->byHide = 1;
	}
	else
	{
		pstAck->byHide = 0;
	}
	return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
}

UINT16	CGTPktBuilder::OnRecvActivityEncourageAck(CPlayer* poPlayer, UINT64 qwActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM], UINT16 wErrCode)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

	if ( wErrCode != ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS)
	{
		pstAck->wErrCode = wErrCode;
		return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
	}

    CBuild* poBuild = poPlayer->GetBuild(EBK_ACTIVITY_ENCOURAGE);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS;
        return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
    }
    poBuild->OnOpen();

	//背包补偿
	if ( BAG_GIFT_ID == qwActivityID)
	{
		if ( 0 != poPlayer->GetDT_PLAYER_BASE_DATA().byBagOpenNum)
		{
			UINT32 dwGold = CBagPropMgr::Instance()->GetEnourageGold(poPlayer->GetDT_PLAYER_BASE_DATA().byBagOpenNum);
			poPlayer->AddGold( dwGold, CRecordMgr::EGGT_BAG_GIFT, poPlayer->GetDT_PLAYER_BASE_DATA().byBagOpenNum);
			poPlayer->SetBagOpenNum( 0 );
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS;
		}
	}
	else
	{
		//奖励信息是否存在。
		SActivityEncouragePro  * poActivityEncouragePro = CActivityEncourageMgr::Instance()->GetActivityEncouragePro(qwActivityID);
		if (NULL == poActivityEncouragePro)
		{
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_ACTIVITY_NOT_EXIT;
			return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
		}


		//检测是否已领取奖励信息
		if (!poPlayer->GetActivityEncourage().CkActivityEncourage(qwActivityID))
		{
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_ENCOURRAGE_RECVD;
			return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
		}

		vector<UINT16> vecItemID;
		UINT32 unSoulNum = 0;
		for(CJsonResItemVectorItr itr = poActivityEncouragePro->vecActivityEncourageItemInfo.vecJsonResItem.begin(); itr != poActivityEncouragePro->vecActivityEncourageItemInfo.vecJsonResItem.end(); itr++)
		{
			CJsonResItem &stActivityEncourageItem =  *itr;
			if (ACTE_GOOD == stActivityEncourageItem.byKindType)
			{
				vecItemID.push_back(stActivityEncourageItem.dwKindValue);
			}

			if (ACTE_SOUL == stActivityEncourageItem.byKindType) {
				unSoulNum += stActivityEncourageItem.byNum;
			}
		}

		//检测背包是否满了
		if(CItemMgr::Instance()->CkBagFull(poPlayer, vecItemID))
		{
			pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_BAGFUL;
			return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
		}

		// 检查是否可塞入战魂背包
		if (unSoulNum > 0) {
			CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
			if (NULL == pPlayerSoul || pPlayerSoul->GetSoulBagLeftSize() < unSoulNum) {
				pstAck->wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_BAGFUL;
				return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
			}
		}

		if(CActivityEncourageMgr::Instance()->IsNeedCdKey(qwActivityID))
		{
			CActivityEncourageMgr::Instance()->CkActivityCdKeyValid(poPlayer->GetID(), qwActivityID, aszSeriesNo);
		}

		CActivityEncourage &oActivityEncourage = poPlayer->GetActivityEncourage();
		pstAck->wErrCode = oActivityEncourage.RecvActivityEncourage(*pstAck, qwActivityID, aszSeriesNo, FALSE);
	}

	
    pstAck->qwActivityID = qwActivityID;
    pstAck->stRsyncRecvEncourageData.qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->stRsyncRecvEncourageData.qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stRsyncRecvEncourageData.stCurPhyStrengthInfo);
    pstAck->stRsyncRecvEncourageData.qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->stRsyncRecvEncourageData.qwCurStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
    pstAck->stRsyncRecvEncourageData.qwCurBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->stRsyncRecvEncourageData.qwCurPurpleGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
    pstAck->stRsyncRecvEncourageData.qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    poPlayer->GetPrompt(pstAck->stRsyncRecvEncourageData.stPromptInfo);
    poPlayer->GetNewEquipPrompt(pstAck->stRsyncRecvEncourageData.stNewEquipPromptInfo);


    //检测是否可取奖励信息
    if (!poPlayer->GetActivityEncourage().CkActivityEncourage(qwActivityID))
    {
        pstAck->byHide = 1;
    }
    else
    {
        pstAck->byHide = 0;
    }
    return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;
}


UINT16	CGTPktBuilder::OnRecvActivityEncourageAck(UINT16 wErrCode)
{


    ZERO_PACKET(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = wErrCode;
    return CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;
}

UINT16 CGTPktBuilder::OnOpenGemMakerTabReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CItemMakeMgr::Instance()->OpenGemMaker(poPlayer, pstAck->byMakerNum, pstAck->astGemMakerInfo);
    pstAck->qwCurBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->byCurTowerNum = poPlayer->GetInstance().GetMaxClimbTownInstanceID().byInstanceIdx;
    //SDStrcpy(pstAck->aszOtherPrompt, "fortest"); // fortest

    return CLIGS_OPEN_GEM_MAKER_TAB_ACK;
}

UINT16 CGTPktBuilder::OnOpenGemMakerTabExReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CItemMakeMgr::Instance()->OpenGemMakerEx(poPlayer, pstAck->byMakerNum, pstAck->astGemMakerInfo);
    pstAck->qwCurBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->byCurTowerNum = poPlayer->GetInstance().GetMaxClimbTownInstanceID().byInstanceIdx;
    //SDStrcpy(pstAck->aszOtherPrompt, "fortest"); // fortest

    return CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK;
}




UINT16	CGTPktBuilder::OnRecvRatingEncourageReq(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byType, UINT16 wChapterID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_RATING_ENCOURAGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = poPlayer->GetInstance().RecvRatingEncourageReq(wSceneIdx, byType, wChapterID, pstAck->stAccumulatedEncourage);
    pstAck->byType = byType;
    pstAck->wChapterID = wChapterID;
    pstAck->stRsyncRecvEncourageData.qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->stRsyncRecvEncourageData.qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stRsyncRecvEncourageData.stCurPhyStrengthInfo);
    pstAck->stRsyncRecvEncourageData.qwCurScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->stRsyncRecvEncourageData.qwCurStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
    pstAck->stRsyncRecvEncourageData.qwCurBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->stRsyncRecvEncourageData.qwCurPurpleGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
    pstAck->stRsyncRecvEncourageData.qwCurJingJie = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    poPlayer->GetPrompt(pstAck->stRsyncRecvEncourageData.stPromptInfo);
    poPlayer->GetNewEquipPrompt(pstAck->stRsyncRecvEncourageData.stNewEquipPromptInfo);
    return CLIGS_RECV_RATING_ENCOURAGE_ACK;
}


UINT16 CGTPktBuilder::OnOpenAwakenExchangeTabReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_AWAKEN_EXCHANGE_TAB::ID_AWAKEN_UNLOCK;
        return CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->wErrCode = poAwaken->OpenAwakenExchangeTab(pstAck->byExchangeNum, pstAck->astStudyExchangeInfo, pstAck->qwCurCoin, pstAck->qwCurStudyClip);

    return CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK;
}

UINT16 CGTPktBuilder::OnExchangeStudyReq(CPlayer* poPlayer, UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EXCHANGE_STUDY_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_EXCHANGE_STUDY::ID_AWAKEN_UNLOCK;
        return CLIGS_EXCHANGE_STUDY_ACK;
    }
    poAwaken->GetAwakenVersion() = 2;
    pstAck->wErrCode = poAwaken->ExchangeStudy(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyLevel, pstAck->byStudyBagIdx, pstAck->stAwakenTabInfo);

    return CLIGS_EXCHANGE_STUDY_ACK;
}

UINT16 CGTPktBuilder::OnOpenEquipComposeTabReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = poPlayer->GetEquipCompse().OpenEquipComposeTab(pstAck->byEquipLevelNum, pstAck->astEquipLevelInfo, pstAck->wComposeCountDown);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurBlueGas = stDT_PLAYER_BASE_DATA.qwBlueGas;

    return CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK;
}


UINT16 CGTPktBuilder::OnEquipComposeReq(CPlayer* poPlayer, UINT16 wEquipLevel, UINT8 byCurColorFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_EQUIP_COMPOSE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = poPlayer->GetEquipCompse().EquipCompose(wEquipLevel, byCurColorFlag, pstAck->stEquipLevelInfo, pstAck->wComposeCountDown);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurBlueGas = stDT_PLAYER_BASE_DATA.qwBlueGas;

    return CLIGS_EQUIP_COMPOSE_ACK;
}

UINT16 CGTPktBuilder::OnRecvComposeEquipReq(CPlayer* poPlayer, UINT16 wEquipLevel)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }


    ZERO_PACKET(PKT_CLIGS_RECV_COMPOSE_EQUIP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = poPlayer->GetEquipCompse().RecvComposeEquip(wEquipLevel, pstAck->stEquipLevelInfo, pstAck->stRecvEquipInfo, pstAck->stRecvEquipStrengthenInfo,  pstAck->dwStrengthen10Coin);
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);

    return CLIGS_RECV_COMPOSE_EQUIP_ACK;
}


// 打开帮派面板
UINT16	CGTPktBuilder::OnOpenFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_FACTION_ACK;
    }
    poBuild->OnOpen();
	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_OPEN_FACTION::ID_UNOPEN;
		return CLIGS_OPEN_FACTION_ACK;
	}
    pstAck->wErrCode = poFactionPlayer->OnOpenFactionAck(*pstAck);

    return CLIGS_OPEN_FACTION_ACK;
}

// 申请加入帮派
UINT16	CGTPktBuilder::OnApplyFactionReq( CPlayer* poPlayer, UINT32 dwFactionID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_APPLY_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_APPLY_FACTION::ID_UNOPEN;
        return CLIGS_APPLY_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->dwFactionID = dwFactionID;
    pstAck->wErrCode = CFactionMgr::Instance()->ApplyFactionDataReq(dwFactionID, poPlayer->GetID(), pstAck->dwJoinFactionCDTime);
    return CLIGS_APPLY_FACTION_ACK;
}

// 加入申请帮派
UINT16	CGTPktBuilder::OnCancelApplyFactionReq( CPlayer* poPlayer, UINT32 dwFactionID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_CANCEL_APPLY_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_CANCEL_APPLY_FACTION::ID_UNOPEN;
        return CLIGS_CANCEL_APPLY_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->dwFactionID = dwFactionID;
    pstAck->wErrCode = CFactionMgr::Instance()->CancelApplyFactionDataReq(dwFactionID, poPlayer->GetID());
    return CLIGS_CANCEL_APPLY_FACTION_ACK;
}
// 创建帮派
UINT16	CGTPktBuilder::OnCreateFactionReq( CPlayer* poPlayer, UINT16 byCreateFactionType, TCHAR aszFactionName[MAX_FACTION_NUM], TCHAR aszFactionDescName[MAX_FACTION_DESC_NUM], UINT32 dwFactionID, UINT16 wErrCode)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_CREATE_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_CREATE_FACTION::ID_UNOPEN;
        return CLIGS_CREATE_FACTION_ACK;
    }
    poBuild->OnOpen();

    if (wErrCode == ERR_CREATE_FACTION::ID_SUCCESS)
    {
        pstAck->wErrCode = CFactionMgr::Instance()->CreateFactionReq(poPlayer, byCreateFactionType, aszFactionName, aszFactionDescName, dwFactionID, pstAck->stFactionOwnerData);
    }
    else
    {
        pstAck->wErrCode = wErrCode;
    }

    return CLIGS_CREATE_FACTION_ACK;
}

// 查找帮派
UINT16	CGTPktBuilder::OnSearchFactionReq( CPlayer* poPlayer, TCHAR aszFactionName[MAX_FACTION_NUM])
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SEARCH_FACTION_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEARCH_FACTION::ID_UNOPEN;
        return CLIGS_SEARCH_FACTION_ACK;
    }
    poBuild->OnOpen();

    CFactionMgr::Instance()->GetSearchFactionCliLst(pstAck->stSearchFactionBaseDataCliLst, aszFactionName, poPlayer->GetID());
    return CLIGS_SEARCH_FACTION_ACK;
}

// 打开属于自己帮派
UINT16	CGTPktBuilder::OnOpenOwnerFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_OWNER_FACTION_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_OWNER_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_OWNER_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->OnOpenOwnerFactionReq(*pstAck, poPlayer->GetID());
    return CLIGS_OPEN_OWNER_FACTION_ACK;
}

// 打开帮派门供
UINT16	CGTPktBuilder::OnOpenDoorsTributeFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->OpenDoorsTributeFactionReq(*pstAck, poPlayer->GetID());
    return CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK;
}

// 门供帮派
UINT16	CGTPktBuilder::OnDoorsTributeFactionReq( CPlayer* poPlayer, UINT8 byDoorsTributeFlag)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_DOORS_TRIBUTE_FACTION::ID_UNOPEN;
        return CLIGS_DOORS_TRIBUTE_FACTION_ACK;
    }
    poBuild->OnOpen();


    pstAck->wErrCode	=	CFactionMgr::Instance()->DoorsTributeFactionReq(*pstAck, byDoorsTributeFlag, poPlayer);
    pstAck->qwCoin	=	poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwGold	=	poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->byDoorsTributeFlag = byDoorsTributeFlag;
    return CLIGS_DOORS_TRIBUTE_FACTION_ACK;
}
// 获取门供帮派奖励
UINT16	CGTPktBuilder::OnRecvDoorsTributeFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_UNOPEN;
        return CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->RecvDoorsTributeFactionReq(poPlayer , *pstAck);
    poPlayer->GetPrompt(pstAck->stPromptInfo);
    poPlayer->GetNewEquipPrompt(pstAck->stNewEquipPromptInfo);
    return CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK;
}

// 帮派踢出
UINT16	CGTPktBuilder::OnKickedFactionReq( CPlayer* poPlayer, UINT32 dwKickedPlayerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_KICKED_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_KICKED_FACTION::ID_UNOPEN;
        return CLIGS_KICKED_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->KickedFactionReq(poPlayer->GetID(), dwKickedPlayerID, *pstAck);

    return CLIGS_KICKED_FACTION_ACK;
}

// 设置帮派职位
UINT16	CGTPktBuilder::OnJobFactionReq( CPlayer* poPlayer, UINT32 dwJoinPlayerID, UINT8 byJobFlag )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_JOB_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_JOB_FACTION::ID_UNOPEN;
        return CLIGS_JOB_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->JobFactionReq(poPlayer->GetID(), dwJoinPlayerID, byJobFlag, *pstAck);
    return CLIGS_JOB_FACTION_ACK;
}

// 设置帮公告
UINT16	CGTPktBuilder::OnSetNoticeFactionReq( CPlayer* poPlayer, UINT8 byFlag, TCHAR aszFactionNotice[MAX_FACTION_DESC_NUM])
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SET_NOTICE_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SET_NOTICE_FACTION::ID_UNOPEN;
        return CLIGS_SET_NOTICE_FACTION_ACK;
    }
    poBuild->OnOpen();
    CkEmoji(aszFactionNotice);
    pstAck->wErrCode = CFactionMgr::Instance()->SetNoticeFactionReq(poPlayer->GetID(), byFlag, aszFactionNotice);
    if (ERR_SET_NOTICE_FACTION::ID_SUCCESS == pstAck->wErrCode)
    {
        pstAck->byFlag = byFlag;
        SDStrncpy(pstAck->aszFactionNotice, aszFactionNotice, sizeof(pstAck->aszFactionNotice) - 1);
    }
    return CLIGS_SET_NOTICE_FACTION_ACK;
}

// 帮派解散
UINT16	CGTPktBuilder::OnDissolveFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_DISSOLVE_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_DISSOLVE_FACTION::ID_UNOPEN;
        return CLIGS_DISSOLVE_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->DissolveFactionReq(poPlayer->GetID());
    return CLIGS_DISSOLVE_FACTION_ACK;
}

// 帮派审核
UINT16	CGTPktBuilder::OnAuditFactionReq( CPlayer* poPlayer, UINT32 dwFactionNum, DT_FACTION_AUDIT_PLAYER_DATA astAuditPlayerData[DT_FACTION_PLAYER_NUM] )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_AUDIT_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_AUDIT_FACTION::ID_UNOPEN;
        return CLIGS_AUDIT_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->AuditFactionReq(poPlayer->GetID(), dwFactionNum, astAuditPlayerData, *pstAck);
    return CLIGS_AUDIT_FACTION_ACK;
}

UINT16	CGTPktBuilder::OnAuditFactionReq2( CPlayer* poPlayer, UINT32 dwPlayerID, UINT8 byAuditWay )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_AUDIT_FACTION_ACK2);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_AUDIT_FACTION::ID_UNOPEN;
        return CLIGS_AUDIT_FACTION_ACK2;
    }
    poBuild->OnOpen();
    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_AUDIT_FACTION::ID_UNOPEN;
        return CLIGS_AUDIT_FACTION_ACK2;
    }

    pstAck->wErrCode = poFactionPlayer->AuditFactionReq( dwPlayerID, byAuditWay, *pstAck);
    pstAck->byAuditWay = byAuditWay;
    pstAck->dwAuditPlayerID = dwPlayerID;
    return CLIGS_AUDIT_FACTION_ACK2;
}


// 打开帮派申请列表
UINT16 CGTPktBuilder::OnOpenApplyFactionReq( CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_APPLY_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_APPLY_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_APPLY_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->OpenApplyFactionReq(pstAck->stApplyFactionBaseDataCliLst, poPlayer->GetID());
    return CLIGS_OPEN_APPLY_FACTION_ACK;
}

// 打开帮派排名列表
UINT16	CGTPktBuilder::OnOpenRankFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_RANK_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_RANK_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_RANK_FACTION_ACK;
    }
    poBuild->OnOpen();


    pstAck->wErrCode = CFactionMgr::Instance()->GetRankFactionCliLst(pstAck->stRankFactionBaseDataCliLst, poPlayer->GetID());
    pstAck->qwNeedGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_FACTION_CREATE_BY_GOLD, 0);
    pstAck->qwNeedCoin = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_FACTION_CREATE_BY_COIN, 0);
    return CLIGS_OPEN_RANK_FACTION_ACK;
}

// 打开帮派审核列表
UINT16	CGTPktBuilder::OnOpenAuditFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_AUDIT_FACTION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_AUDIT_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_AUDIT_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->OpenAuditFaction(*pstAck, poPlayer->GetID());
    return CLIGS_OPEN_AUDIT_FACTION_ACK;
}

// 打开帮派审核列表
UINT16	CGTPktBuilder::OnOpenAuditFactionAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_AUDIT_FACTION_ACK2);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_AUDIT_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_AUDIT_FACTION_ACK2;
    }
    poBuild->OnOpen();
    CFactionPlayer* poFactonPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactonPlayer )
    {
        pstAck->wErrCode = ERR_OPEN_AUDIT_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_AUDIT_FACTION_ACK2;
    }
    pstAck->wErrCode = poFactonPlayer->OpenAuditFaction(*pstAck);
    return CLIGS_OPEN_AUDIT_FACTION_ACK2;
}

// 打开帮派日志列表
UINT16	CGTPktBuilder::OnOpenLogFactionReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_LOG_FACTION_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_LOG_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_LOG_FACTION_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = CFactionMgr::Instance()->GetDT_FACTION_LOG_DATA_CLI_LST(pstAck->stFactionLogDataCliLst, poPlayer->GetID());
    return CLIGS_OPEN_LOG_FACTION_ACK;
}

// 打开聚魂面板
UINT16	CGTPktBuilder::OnOpenGetherScienceReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_GATHER_SCIENCE_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CGatherScience *poGatherScience = poPlayer->GetGatherScience();
    if (NULL == poGatherScience)
    {
        pstAck->wErrCode = ERR_OPEN_GATHER_SCIENCE::ID_SUCCESS;
        return CLIGS_OPEN_GATHER_SCIENCE_ACK;
    }

    pstAck->wErrCode = poGatherScience->OpenGetherScience(pstAck->stGatherScience);
    pstAck->qwScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

    return CLIGS_OPEN_GATHER_SCIENCE_ACK;
}


// 聚魂
UINT16	CGTPktBuilder::OnGetherScienceReq( CPlayer* poPlayer, UINT8 byGatherType)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GATHER_SCIENCE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CGatherScience *poGatherScience = poPlayer->GetGatherScience();
    if (NULL == poGatherScience)
    {
        pstAck->wErrCode =  ERR_OPEN_GATHER_SCIENCE::ID_SUCCESS;
        return CLIGS_GATHER_SCIENCE_ACK;
    }

    pstAck->wErrCode = poGatherScience->GetherScience(pstAck->stGatherScience, byGatherType);
    pstAck->qwScience = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    pstAck->qwCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    return CLIGS_GATHER_SCIENCE_ACK;
}
// 打开仙露阁
UINT16 CGTPktBuilder::OnOpenBluePavilionReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_BLUE_PAVILION_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_BEGBLUE);
    if ( NULL == pBuild)
    {
        pstAck->wErrCode = ERR_OPEN_BLUE_PAVILION::ID_BUILD_NOT_EXIST;
        return CLIGS_OPEN_BLUE_PAVILION_ACK;
    }

    CBegBlue *poBegBlue = poPlayer->GetBegBlue();
    if ( NULL == poBegBlue)
    {
        pstAck->wErrCode = ERR_OPEN_BLUE_PAVILION::ID_BUILD_NOT_EXIST;
        return CLIGS_OPEN_BLUE_PAVILION_ACK;
    }

    pstAck->wErrCode = poBegBlue->OpenBluePavilion(pstAck->stBluePavilion, pstAck->byIsIgnoreXnMultiple, pstAck->stEncourage);
    pstAck->stPlayerResInfo.qwGold  = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->stPlayerResInfo.qwBlue  =  poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->stPlayerResInfo.qwStory =  poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;

    return CLIGS_OPEN_BLUE_PAVILION_ACK;
}

// 求仙露
UINT16 CGTPktBuilder::OnBegBlueReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_BEGBLUE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_BEGBLUE);
    if ( NULL == pBuild)
    {
        pstAck->wErrCode = ERR_IMPROVE_LUCK::ID_BUILD_NOT_EXIST;
        return CLIGS_BEGBLUE_ACK;
    }

    CBegBlue *poBegBlue = poPlayer->GetBegBlue();
    if ( NULL == poBegBlue)
    {
        pstAck->wErrCode = ERR_IMPROVE_LUCK::ID_BUILD_NOT_EXIST;
        return CLIGS_BEGBLUE_ACK;
    }

    pstAck->wErrCode = poBegBlue->BegBlue(pstAck->stBegBlueResult);

    return CLIGS_BEGBLUE_ACK;
}

// 求仙露改运
UINT16 CGTPktBuilder::OnImproveLuckReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_IMPROVELUCK_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_BEGBLUE);
    if ( NULL == pBuild)
    {
        pstAck->wErrCode = ERR_BEGBLUE::ID_BUILD_NOT_EXIST;
        return CLIGS_BEGBLUE_ACK;
    }

    CBegBlue *poBegBlue = poPlayer->GetBegBlue();
    if ( NULL == poBegBlue)
    {
        pstAck->wErrCode = ERR_BEGBLUE::ID_BUILD_NOT_EXIST;
        return CLIGS_BEGBLUE_ACK;
    }

    pstAck->wErrCode = poBegBlue->ImproveLuck(pstAck->stBegBlueResCli, pstAck->stImproveLuckInfo);
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_IMPROVELUCK_ACK;
}

// 确认祈求结果
UINT16 CGTPktBuilder::OnComfirmBegBlueReq(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COMFIRM_BEGBLUE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_BEGBLUE);
    if ( NULL == pBuild)
    {
        pstAck->wErrCode = ERR_COMFIRM_BEGBLUE::ID_BUILD_NOT_EXIST;
        return CLIGS_COMFIRM_BEGBLUE_ACK;
    }

    CBegBlue *poBegBlue = poPlayer->GetBegBlue();
    if ( NULL == poBegBlue)
    {
        pstAck->wErrCode = ERR_COMFIRM_BEGBLUE::ID_BUILD_NOT_EXIST;
        return CLIGS_COMFIRM_BEGBLUE_ACK;
    }

    pstAck->wErrCode = poBegBlue->ComfirmBegBlue(pstAck->stBegBlueInfo, pstAck->stEncourage);
    pstAck->stPlayerResInfo.qwGold  = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->stPlayerResInfo.qwBlue  =  poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->stPlayerResInfo.qwStory =  poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;

    return CLIGS_COMFIRM_BEGBLUE_ACK;
}

UINT16 CGTPktBuilder::OnChangeCareerReq(CPlayer* poPlayer, UINT8 byCareerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_CHANGE_CAREER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->byCareerID = byCareerID;
    pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    pstAck->wErrCode = poPlayer->Change2NewCareer(byCareerID);
    pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    return CLIGS_CHANGE_CAREER_ACK;
}


UINT16 CGTPktBuilder::OnChallengeMyselfReq(CPlayer* poPlayer, UINT8 byCareerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_CHALLENGE_MYSELF_ACK5);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = CBattleMgr::Instance()->BattleMyself(poPlayer, byCareerID, pstAck->stBattleInfo);

    return CLIGS_CHALLENGE_MYSELF_ACK5;
}



UINT16 CGTPktBuilder::OnOpenItemExchangeReq( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CItemExchangeMgr::Instance()->OpenItemExchange(poPlayer, *pstAck);

    return CLIGS_OPEN_ITEM_EXCHANGE_ACK;
}

UINT16 CGTPktBuilder::OnItemExchangeReq( CPlayer* poPlayer, UINT16 wExchangeIdx )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_ITEM_EXCHANGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CItemExchangeMgr::Instance()->ItemExchange(poPlayer, wExchangeIdx, *pstAck);

    return CLIGS_ITEM_EXCHANGE_ACK;
}


UINT16 CGTPktBuilder::OnOperatingActivityBattleReq(CPlayer* poPlayer, TCHAR* pszBattleKey)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    SDStrcpy(pstAck->aszBattleKey, pszBattleKey);
    COprateingActivityMgr::Instance()->CliBattle(poPlayer, pszBattleKey, pstAck);

    return CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5;
}


// 获取好友系统玩家列表
UINT16 CGTPktBuilder::OnGetFriendSysPlayerListAck(CPlayer* poPlayer, UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK;
    }

    pstAck->byGetListType  = byGetListType;
    pstAck->byPageNo       = byPageNo;
    pstAck->wErrCode = poFriends->GetFriendSysPlayerList(byGetListType, byPageCapacity, byPageNo, pstAck->dwTotalNum, pstAck->stPlayerList);

    return CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK;
}

// 获取好友系统玩家列表
UINT16 CGTPktBuilder::OnGetFriendSysPlayerListAck2(CPlayer* poPlayer, UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK;
    }

    pstAck->byGetListType  = byGetListType;
    pstAck->byPageNo       = byPageNo;
    pstAck->wErrCode = poFriends->GetFriendSysPlayerList2(byGetListType, byPageCapacity, byPageNo, pstAck->dwTotalNum, pstAck->stPlayerList);

    return CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2;
}


// 删除好友
UINT16 CGTPktBuilder::OnDelFriendAck(CPlayer* poPlayer, UINT32 dwFirendPlayerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_DEL_FRIEND_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_DEL_FRIEND::ID_NO_OPEN;
        return CLIGS_DEL_FRIEND_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_DEL_FRIEND::ID_NO_OPEN;
        return CLIGS_DEL_FRIEND_ACK;
    }
    pstAck->wErrCode = poFriends->DelFriend(dwFirendPlayerID);
    return CLIGS_DEL_FRIEND_ACK;
}

// 查找玩家
UINT16 CGTPktBuilder::OnSearchPlayerAck(CPlayer* poPlayer, TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SEARCH_PLAYER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEARCH_PLAYER::ID_NO_OPEN;
        return CLIGS_SEARCH_PLAYER_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEARCH_PLAYER::ID_NO_OPEN;
        return CLIGS_SEARCH_PLAYER_ACK;
    }

    pstAck->byPageNo = byPageNo;
    pstAck->wErrCode = poFriends->SearchPlayer(aszPlayerName, byPageCapacity, byPageNo, pstAck->dwTotalNum, pstAck->stSearchRltList);

    return CLIGS_SEARCH_PLAYER_ACK;
}

// 查找玩家
UINT16 CGTPktBuilder::OnSearchPlayerAck2(CPlayer* poPlayer, TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SEARCH_PLAYER_ACK2);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEARCH_PLAYER::ID_NO_OPEN;
        return CLIGS_SEARCH_PLAYER_ACK2;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEARCH_PLAYER::ID_NO_OPEN;
        return CLIGS_SEARCH_PLAYER_ACK2;
    }

    pstAck->byPageNo = byPageNo;
    pstAck->wErrCode = poFriends->SearchPlayer2(aszPlayerName, byPageCapacity, byPageNo, pstAck->dwTotalNum, pstAck->stSearchRltList);

    return CLIGS_SEARCH_PLAYER_ACK2;
}

// 查看玩家信息
UINT16 CGTPktBuilder::OnViewPlayerAck(CPlayer* poPlayer, UINT32 dwPlayerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_VIEW_PLAYER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_VIEW_PLAYER::ID_NO_OPEN;
        return CLIGS_VIEW_PLAYER_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_VIEW_PLAYER::ID_NO_OPEN;
        return CLIGS_VIEW_PLAYER_ACK;
    }

    pstAck->wErrCode = poFriends->ViewPlayerInfo(dwPlayerID, *pstAck);

    return CLIGS_VIEW_PLAYER_ACK;
}

// 添加好友
UINT16 CGTPktBuilder::OnAddFriendAck(CPlayer* poPlayer, CPlayer *poTargetPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_ADD_FRIEND_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_ADD_FRIEND::ID_NO_OPEN;
        return CLIGS_ADD_FRIEND_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_ADD_FRIEND::ID_NO_OPEN;
        return CLIGS_ADD_FRIEND_ACK;
    }

    CBuild *	poTargetBuild = poTargetPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poTargetBuild)
    {
        pstAck->wErrCode = ERR_ADD_FRIEND::ID_TARGET_NO_OPEN;
        return CLIGS_ADD_FRIEND_ACK;
    }
    poTargetBuild->OnOpen();

    CFriends *poTargetFriends = poTargetPlayer->GetFriends();
    if (NULL == poTargetFriends)
    {
        pstAck->wErrCode = ERR_ADD_FRIEND::ID_TARGET_NO_OPEN;
        return CLIGS_ADD_FRIEND_ACK;
    }

    pstAck->wErrCode = poFriends->AddFriend(poTargetPlayer->GetID());
    return CLIGS_ADD_FRIEND_ACK;
}

// 送鲜花
UINT16 CGTPktBuilder::OnSendFlowerAck(CPlayer* poPlayer, CPlayer* poTargetPlayer, UINT32 dwSendFlowerNum)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SEND_FLOWER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_SEND_FLOWER_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_SEND_FLOWER_ACK;
    }

    pstAck->wErrCode = poFriends->SendFlower(poTargetPlayer, dwSendFlowerNum, pstAck->qwRecverFlowerSum);
    pstAck->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCurGoodsNum = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;

    return CLIGS_SEND_FLOWER_ACK;
}

// 获取某玩家鲜花信息和收花记录
UINT16 CGTPktBuilder::OnGetFlowerByPlayerIDAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_FLOWER_BY_PLAYERID::ID_NO_OPEN;
        return CLIGS_GET_FLOWER_BY_PLAYERID_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_GET_FLOWER_BY_PLAYERID::ID_NO_OPEN;
        return CLIGS_GET_FLOWER_BY_PLAYERID_ACK;
    }

    pstAck->wErrCode = poFriends->GetFlowerByPlayerID(pstAck->qwFlowerSum, pstAck->dwMyFlowerRanking, pstAck->stRecvFlowerLogList);

    return CLIGS_GET_FLOWER_BY_PLAYERID_ACK;
}

// 获取与某玩家的私聊记录
UINT16 CGTPktBuilder::OnGetPrivateMsgLogListAck(CPlayer* poPlayer, UINT32 dwRecverID, UINT32 dwStartIdx, UINT8 byGetMsgNum)
{
    ZERO_PACKET(PKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_PRIVATE_MSG_LOG_LIST::ID_NO_OPEN;
        return CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_GET_PRIVATE_MSG_LOG_LIST::ID_NO_OPEN;
        return CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK;
    }

    pstAck->dwStartIdx = dwStartIdx;
    poFriends->DelUnReadEven(dwRecverID);

    pstAck->byPeerRole = (CChatMgr::Instance()->IsGm(dwRecverID) ? PCRT_GM : PCRT_NORMAL);
    pstAck->wErrCode = CPrivateMsgMgr::Instance()->GetPrivateMsgLogList(poPlayer->GetID(), dwRecverID, dwStartIdx, byGetMsgNum, pstAck->dwTotalNum, pstAck->stPrivateMsgList);
    return CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK;
}

// 发送私聊消息
UINT16 CGTPktBuilder::OnSendPrivateMsgAck(CPlayer* poPlayer, CPlayer* poTargetPlayer, TCHAR aszMsgContent[PRIVATE_MSG_CONTENT_LEN])
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SEND_PRIVATE_MSG_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEND_PRIVATE_MSG::ID_NO_OPEN;
        return CLIGS_SEND_PRIVATE_MSG_ACK;
    }
    poBuild->OnOpen();

	//检测是否有关键字过滤
	TCHAR	*pWordFilter = NULL;
	pWordFilter = CWordFilter::Instance()->VerifyString(aszMsgContent);
	if(NULL != pWordFilter)
	{
		//聊天内容中不允许包含<XXX>关键字
		std::vector<string> vecFillMsg;
		vecFillMsg.push_back(pWordFilter);
		SDStrncpy(pstAck->aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("WORDFILTER", &vecFillMsg) .c_str(), MAX_COMMON_TXT_NUM - 1);
		pstAck->wErrCode =  ERR_SEND_PRIVATE_MSG::ID_VERIFY_ERROR;
		return CLIGS_SEND_PRIVATE_MSG_ACK;
		
	}

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEND_PRIVATE_MSG::ID_NO_OPEN;
        return CLIGS_SEND_PRIVATE_MSG_ACK;
    }

    //强制检测一下聊天内容，以离数据库保存失败。
    CkEmoji(aszMsgContent);

    pstAck->wErrCode = CPrivateMsgMgr::Instance()->SendPrivateMsg(poPlayer, poTargetPlayer, aszMsgContent);
    return CLIGS_SEND_PRIVATE_MSG_ACK;
}

// 删除私聊记录
UINT16 CGTPktBuilder::OnDelPrivateMsgAck(CPlayer* poPlayer, UINT32 dwPlayerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    return CLIGS_DEL_PRIVATE_MSG_LOG_ACK;
}

// 拉入黑名单
UINT16 CGTPktBuilder::OnAddToBlackListAck(CPlayer* poPlayer, CPlayer *poTargetPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_ADD_TO_BLACKLIST_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_ADD_TO_BLACKLIST::ID_NO_OPEN;
        return CLIGS_ADD_TO_BLACKLIST_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_ADD_TO_BLACKLIST::ID_NO_OPEN;
        return CLIGS_ADD_TO_BLACKLIST_ACK;
    }

    pstAck->wErrCode = poFriends->AddToBlackList(poTargetPlayer);
    CFriends *poTargetFriends = poTargetPlayer->GetFriends();
    if (NULL != poTargetFriends)
    {
        poTargetFriends->AddToBlackList(poPlayer);
    }


    return CLIGS_ADD_TO_BLACKLIST_ACK;
}

// 移除黑名单
UINT16 CGTPktBuilder::OnMoveFromBlackListAck(CPlayer* poPlayer, UINT32 dwPlayerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_MOVE_FROM_BLACKLIST_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_MOVE_FROM_BLACKLIST_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_MOVE_FROM_BLACKLIST_ACK;
    }

    pstAck->wErrCode = poFriends->MoveFromBlackList(dwPlayerID);

    return CLIGS_MOVE_FROM_BLACKLIST_ACK;
}

// 打开送花面板
UINT16 CGTPktBuilder::OnOpenSendFlowerTabAck(CPlayer* poPlayer, UINT32 dwRecvPlayerID)
{
    //
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SEND_FLOWER_TAB::ID_NO_OPEN;
        return CLIGS_OPEN_SEND_FLOWER_TAB_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_OPEN_SEND_FLOWER_TAB::ID_NO_OPEN;
        return CLIGS_OPEN_SEND_FLOWER_TAB_ACK;
    }

    pstAck->wErrCode = poFriends->OpenSendFlowerTab(pstAck->stSendFlowerPropList, dwRecvPlayerID);

    return CLIGS_OPEN_SEND_FLOWER_TAB_ACK;
}

// 获取鲜花榜
UINT16 CGTPktBuilder::OnGetFlowerRankingAck(CPlayer* poPlayer)
{
    //
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GET_FLOWER_RANKING_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_GET_FLOWER_RANKING_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_GET_FLOWER_RANKING_ACK;
    }

    pstAck->wErrCode = CPlayerBaseDataCacheMgr::Instance()->GetFlowerRankingList(pstAck->stFlowerRankingList);

    return CLIGS_GET_FLOWER_RANKING_ACK;
}


// 获取发离线消息的玩家列表
UINT16 CGTPktBuilder::OnGetSendOfflineMsgPlayerListAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST::ID_NO_OPEN;
        return CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK;
    }

    pstAck->wErrCode = poFriends->GetSendOfflineMsgPlayerList(pstAck->stPlayerList);

    return CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK;
}

// 读取某个发方玩家的离线消息请求
UINT16 CGTPktBuilder::OnReadOfflineMsgAck(CPlayer* poPlayer, UINT32 dwPlayerID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_READ_OFFLINE_MSG_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_READ_OFFLINE_MSG::ID_NO_OPEN;
        return CLIGS_READ_OFFLINE_MSG_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_READ_OFFLINE_MSG::ID_NO_OPEN;
        return CLIGS_READ_OFFLINE_MSG_ACK;
    }

    pstAck->wErrCode = poFriends->ReadOfflineMsg(dwPlayerID);

    return CLIGS_READ_OFFLINE_MSG_ACK;
}

//打开珍品阁应答
UINT16 CGTPktBuilder::OnOpenRareItemAck(CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_RAREITEMROOM_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_RAREITEMROOM::ID_IS_UNOPEN;
        return CLIGS_OPEN_RAREITEMROOM_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_RAREITEMROOM::ID_IS_UNOPEN;
        return CLIGS_OPEN_RAREITEMROOM_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->GetOpenRareItem( pstAck->stRareItemRoomInfo, pstAck->stRareItemInfo );

    return CLIGS_OPEN_RAREITEMROOM_ACK;
}

UINT16 CGTPktBuilder::OnExchangeRareItemAck(CPlayer* poPlayer, UINT32 dwGoodID )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_BUY_RAREITEM_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_BUY_RAREITEM::ID_UNOPEN;
        return CLIGS_BUY_RAREITEM_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_BUY_RAREITEM::ID_UNOPEN;
        return CLIGS_BUY_RAREITEM_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->ExchangeRareItem( pstAck->stRareItemInfo, pstAck->dwLeftDoorsTributes, dwGoodID);

    return CLIGS_BUY_RAREITEM_ACK;
}

//珍品阁升级请求
UINT16 CGTPktBuilder::OnOpenUpgradeRareItemAck(CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_UPGRADERAREITEM_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_UPGRADERAREITEM::ID_UNOPEN;
        return CLIGS_OPEN_UPGRADERAREITEM_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_UPGRADERAREITEM::ID_UNOPEN;
        return CLIGS_OPEN_UPGRADERAREITEM_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->OpenUpgradeRareItem( pstAck->stUpgradeRequireInfo );

    return CLIGS_OPEN_UPGRADERAREITEM_ACK;
}

//升级确定请求
UINT16 CGTPktBuilder::OnUpgradeRareItemAck(CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_UPGRADE_RAREITEM_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_UPGRADE_RAREITEM::ID_UNOPEN;
        return CLIGS_UPGRADE_RAREITEM_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_UPGRADE_RAREITEM::ID_UNOPEN;
        return CLIGS_UPGRADE_RAREITEM_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->UpgradeRareItem(  *pstAck);

    return CLIGS_UPGRADE_RAREITEM_ACK;
}

//打开武堂应答
UINT16 CGTPktBuilder::OnOpenGymnasiumAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_GYMNASIUM_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_GYMNASIUM::ID_UNOPEN;
        return CLIGS_OPEN_GYMNASIUM_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_GYMNASIUM::ID_UNOPEN;
        return CLIGS_OPEN_GYMNASIUM_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OpenGymnasium( *pstAck);

    return CLIGS_OPEN_GYMNASIUM_ACK;
}

//学习武堂技能应答
UINT16 CGTPktBuilder::OnLearnGymnasiumSkillAck( CPlayer* poPlayer, UINT32 dwSkillID )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_LEARN_GYMNASIUM_SKILL::ID_UNOPEN;
        return CLIGS_LEARN_GYMNASIUM_SKILL_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_LEARN_GYMNASIUM_SKILL::ID_UNOPEN;
        return CLIGS_LEARN_GYMNASIUM_SKILL_ACK;
    }

    pstAck->stSkillInfo.bySkillID = static_cast<UINT8>(dwSkillID);
    pstAck->wErrCode = poFactionPlayer->LearnGymnasiumSkill( *pstAck);
    return CLIGS_LEARN_GYMNASIUM_SKILL_ACK;
}

//打开武堂升级应答
UINT16 CGTPktBuilder::OnOpenGymnasiumUpgradeAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_GYMNASIUM_UPGRADE::ID_UNOPEN;
        return CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_GYMNASIUM_UPGRADE::ID_UNOPEN;
        return CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OpenGymnasiumUpgrade( pstAck->stUpgradeRequireInfo );

    return CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK;
}

//确认武堂升级应答
UINT16 CGTPktBuilder::OnGymnasiumUpgradeAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_GYMNASIUM_UPGRADE_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GYMNASIUM_UPGRADE::ID_UNOPEN;
        return CLIGS_GYMNASIUM_UPGRADE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_GYMNASIUM_UPGRADE::ID_UNOPEN;
        return CLIGS_GYMNASIUM_UPGRADE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->GymnasiumUpgrade( *pstAck);

    return CLIGS_GYMNASIUM_UPGRADE_ACK;
}

//打开暗部
UINT16 CGTPktBuilder::OnOpenShadeAck( CPlayer* poPlayer )
{

    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_SHADE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SHADE::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_SHADE::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OpenShade( *pstAck );

    return CLIGS_OPEN_SHADE_ACK;
}

//打开黄金模式
UINT16 CGTPktBuilder::OnOpenGoldModeAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_GOLDMODE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_GOLDMODE::ID_UNOPEN;
        return CLIGS_OPEN_GOLDMODE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_GOLDMODE::ID_UNOPEN;
        return CLIGS_OPEN_GOLDMODE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->OpenGoldMode( pstAck->stCurMonsterInfo, pstAck->dwSyncGold );

    if (pstAck->wErrCode == ERR_OPEN_GOLDMODE::ID_SUCCESS)
    {
        PKT_CLIGS_OPEN_GOLD_NTF stNtf;
        stNtf.byOpenFlag = 1;

        CFaction* poFaction = poPlayer->GetFaction();
        if (NULL != poFaction)
        {
            poFaction->SendMessageToAllFactionPlayer((TCHAR*)&stNtf, CLIGS_OPEN_GOLD_NTF, poPlayer->GetID());
        }
    }
    return CLIGS_OPEN_GOLDMODE_ACK;
}

//打开暗部升级
UINT16 CGTPktBuilder::OnOpenShadeUpgradeAck( CPlayer* poPlayer )
{

    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_SHADE_UPGRADE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SHADE_UPGRADE::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_UPGRADE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_SHADE_UPGRADE::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_UPGRADE_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OpenShadeUpgrade( pstAck->stUpgradeInfo );

    return CLIGS_OPEN_SHADE_UPGRADE_ACK;
}

//暗部升级
UINT16 CGTPktBuilder::OnShadeUpgradeAck(CPlayer* poPlayer )
{

    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_SHADE_UPGRADE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SHADE_UPGRADE::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_UPGRADE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_SHADE_UPGRADE::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_UPGRADE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->ShadeUpgrade( *pstAck);

    return CLIGS_SHADE_UPGRADE_ACK;
}

//打开捉妖
UINT16 CGTPktBuilder::OnOpenHuntingAck(CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_HUNTING_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_HUNTING::ID_UNOPEN;
        return CLIGS_OPEN_HUNTING_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_HUNTING::ID_UNOPEN;
        return CLIGS_OPEN_HUNTING_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->OpenHunting(*pstAck );

    return CLIGS_OPEN_HUNTING_ACK;
}
//筋斗云OR千里眼
UINT16 CGTPktBuilder::OnFindMonsterAck(CPlayer* poPlayer, PKT_CLIGS_FIND_MONSTER_REQ& oReq)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_FIND_MONSTER_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_FIND_MONSTER::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_UPGRADE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_FIND_MONSTER::ID_UNOPEN;
        return CLIGS_OPEN_SHADE_UPGRADE_ACK;
    }
    UINT8 byFindType = oReq.byFindType;
    vector<UINT16> vecMonsterBlue;
    vector<UINT16> vecMonsterPurple;
    vector<UINT16> vecMonsterRed;
    vector<UINT16> vecMonsterOrange;

    UINT8 byMonsterNum = 0;
    for (byMonsterNum = 0; byMonsterNum < oReq.byMonsterNum; byMonsterNum++)
    {
        vecMonsterBlue.push_back(oReq.awMonsterBlue[byMonsterNum]);
    }
    for (byMonsterNum = 0; byMonsterNum < oReq.byMonsterNum; byMonsterNum++)
    {
        vecMonsterPurple.push_back(oReq.awMonsterPurple[byMonsterNum]);
    }
    for (byMonsterNum = 0; byMonsterNum < oReq.byMonsterNum; byMonsterNum++)
    {
        vecMonsterRed.push_back(oReq.awMonsterRed[byMonsterNum]);
    }
    for (byMonsterNum = 0; byMonsterNum < oReq.byMonsterNum; byMonsterNum++)
    {
        vecMonsterOrange.push_back(oReq.awMonsterOrange[byMonsterNum]);
    }
    switch(byFindType)
    {
        //开始寻找妖怪
    case 1:

        pstAck->wErrCode = poFactionPlayer->FirstFindMonster(vecMonsterBlue, vecMonsterPurple, vecMonsterRed, vecMonsterOrange, pstAck->stMonsterInfo);
        break;
    case 2://直接橙色
        pstAck->wErrCode = poFactionPlayer->FindMonster(vecMonsterOrange, pstAck->stMonsterInfo );
        break;
    case 3://更换怪
        pstAck->wErrCode = poFactionPlayer->ChangeMonster(vecMonsterBlue, vecMonsterPurple, vecMonsterRed, vecMonsterOrange,
                           pstAck->stMonsterInfo, pstAck->dwGoldNum );
        break;
    }
    poFactionPlayer->GetFreeMsg(pstAck->stFreeMsg);
    pstAck->byGoldModeFlag = poFactionPlayer->GetDT_SHADE_DATA().stHuntingInfo.byGoldModeFlag;
    pstAck->byFindType = byFindType;
    pstAck->dwGoldNum = static_cast<UINT32>(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold);
    return CLIGS_FIND_MONSTER_ACK;
}

UINT16 CGTPktBuilder::OnHuntingBattleAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_HUNTING_BATTLE_ACK5 );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_HUNTING_BATTLE::ID_UNOPEN;
        return CLIGS_HUNTING_BATTLE_ACK5;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_HUNTING_BATTLE::ID_UNOPEN;
        return CLIGS_HUNTING_BATTLE_ACK5;
    }

    pstAck->wErrCode = poFactionPlayer->OnHuntingBattle(*pstAck);
    CFaction* poFaction = poPlayer->GetFaction();
    if (NULL != poFaction)
    {
        DT_FACTION_PLAYER_CLI stCli = {0};
        poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, poPlayer->GetID());
        pstAck->dwSyncDoorsTribute = stCli.dwDoorsTributeValues;
    }
    return CLIGS_HUNTING_BATTLE_ACK5;
}

UINT16 CGTPktBuilder::OnHuntingBattleAck2(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_HUNTING_BATTLE_ACK2 );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_HUNTING_BATTLE::ID_UNOPEN;
        return CLIGS_HUNTING_BATTLE_ACK2;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_HUNTING_BATTLE::ID_UNOPEN;
        return CLIGS_HUNTING_BATTLE_ACK2;
    }

    pstAck->wErrCode = poFactionPlayer->OnHuntingBattle2(*pstAck);
    CFaction* poFaction = poPlayer->GetFaction();
    if (NULL != poFaction)
    {
        DT_FACTION_PLAYER_CLI stCli = {0};
        poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, poPlayer->GetID());
        pstAck->dwSyncDoorsTribute = stCli.dwDoorsTributeValues;
    }
    return CLIGS_HUNTING_BATTLE_ACK2;
}

UINT16 CGTPktBuilder::OnHuntingBattleAck3(CPlayer* poPlayer)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET( PKT_CLIGS_HUNTING_BATTLE_ACK3 );

	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_HUNTING_BATTLE::ID_UNOPEN;
		return CLIGS_HUNTING_BATTLE_ACK3;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_HUNTING_BATTLE::ID_UNOPEN;
		return CLIGS_HUNTING_BATTLE_ACK3;
	}

	pstAck->wErrCode = poFactionPlayer->OnHuntingBattle3(*pstAck);
	CFaction* poFaction = poPlayer->GetFaction();
	if (NULL != poFaction)
	{
		DT_FACTION_PLAYER_CLI stCli = {0};
		poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, poPlayer->GetID());
		pstAck->dwSyncDoorsTribute = stCli.dwDoorsTributeValues;
	}
	return CLIGS_HUNTING_BATTLE_ACK3;
}
//换档奖励通知
UINT16 CGTPktBuilder::ChangeSchedule(CPlayer* poPlayer, UINT32 dwCurSchedule)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_CHANGE_SCHEDEULE_NTF );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }


    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        return INVALID_MSGID;
    }

    poFactionPlayer->OnChangeSchedule(*pstAck);

    return CLIGS_CHANGE_SCHEDEULE_NTF;
}

UINT16 CGTPktBuilder::OnHotSpringAck( CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_HOT_SPRING_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_HOT_SPRING::ID_UNOPEN;
        return CLIGS_OPEN_HOT_SPRING_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_HOT_SPRING::ID_UNOPEN;
        return CLIGS_OPEN_HOT_SPRING_ACK;
    }
    //记录位置
    poFactionPlayer->WriteDownPosition(wPosX, wPosY);
    pstAck->wErrCode = poFactionPlayer->OnOpenHotSpring( *pstAck);

    return CLIGS_OPEN_HOT_SPRING_ACK;
}

UINT16 CGTPktBuilder::OnSelectWineAck(CPlayer* poPlayer, UINT32 dwWineID)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_SELECT_WINE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SELECT_WINE::ID_UNOPEN;
        return CLIGS_SELECT_WINE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_SELECT_WINE::ID_FACTION_NOT_EXIST;
        return CLIGS_SELECT_WINE_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OnSelectWine(dwWineID, pstAck->qwCoin, pstAck->qwGold, pstAck->stWineDataCli, pstAck->bySelectWineState);
    return CLIGS_SELECT_WINE_ACK;
}

UINT16 CGTPktBuilder::OnDrinkWineAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_DRINK_WINE_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    //pstAck->wErrCode = poFactionPlayer->OnDrinkWine(pstAck->stWineDataCli, pstAck->byRewardID1, pstAck->dwSyncValue1, pstAck->byRewardID2, pstAck->dwSyncValue2);
    return CLIGS_DRINK_WINE_ACK;
}

UINT16 CGTPktBuilder::OnDrinkWineMoveAck(CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_DRINK_WINE_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_DRINK_WINE_MOVE::ID_UNOPEN;
        return CLIGS_DRINK_WINE_MOVE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_DRINK_WINE_MOVE::ID_HOT_SPRING_NOT_EXIST;
        return CLIGS_DRINK_WINE_MOVE_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OnDrinkWineMove(wPosX, wPosY);

    return CLIGS_DRINK_WINE_MOVE_ACK;
}

UINT16 CGTPktBuilder::OnLeaveHotSpringAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_LEAVE_HOT_SPRING_ACK);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        return INVALID_MSGID;
    }
    poFactionPlayer->LeaveHotSpring();
    poPlayer->GetLocale().OnLeaveHotSpring();

    return CLIGS_LEAVE_HOT_SPRING_ACK;
}

UINT16 CGTPktBuilder::OnChangeOtherAck( CPlayer* poPlayer, UINT32 dwSendPlayerID )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_CHANGE_OTHER_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    poPlayer->GetID();
    pstAck->wErrCode = CFactionMgr::Instance()->ChangeOther( poPlayer->GetID(), dwSendPlayerID, *pstAck);
    return CLIGS_CHANGE_OTHER_ACK;
}

UINT16 CGTPktBuilder::OnGiveWineAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_GIVE_WINE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_GIVE_WINE::ID_UNOPEN;
        return CLIGS_GIVE_WINE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_GIVE_WINE::ID_UNOPEN;
        return CLIGS_GIVE_WINE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->GiveWine( pstAck->stGiveWineInfo);
    return CLIGS_GIVE_WINE_ACK;
}

UINT16 CGTPktBuilder::OnClickSelectWineAck( CPlayer* poPlayer, UINT8 byDrinkWineState)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_CLICK_SELECT_WINE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_CLICK_SELECT_WINE::ID_UNOPEN;
        return CLIGS_CLICK_SELECT_WINE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_CLICK_SELECT_WINE::ID_UNOPEN;
        return CLIGS_CLICK_SELECT_WINE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->ClickSelectWineGiveWine(byDrinkWineState, pstAck->stWineDataList);

    return CLIGS_CLICK_SELECT_WINE_ACK;
}

UINT16 CGTPktBuilder::OnSureGiveWineAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_SURE_GIVE_WINE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SURE_GIVE_WINE::ID_UNOPEN;
        return CLIGS_SURE_GIVE_WINE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_SURE_GIVE_WINE::ID_UNOPEN;
        return CLIGS_SURE_GIVE_WINE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->SureGiveWine(pstAck->qwActiviTime);

    return CLIGS_SURE_GIVE_WINE_ACK;
}

UINT16 CGTPktBuilder::OnClosedMonsterAck( CPlayer* poPlayer, UINT8 byChoice )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_CLOSED_MONSTER_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_CLOSED_MONSTER::ID_UNOPEN;
        return CLIGS_CLOSED_MONSTER_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_CLOSED_MONSTER::ID_UNOPEN;
        return CLIGS_CLOSED_MONSTER_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->ClosedMonster(byChoice, *pstAck);
    CFaction* poFaction = poPlayer->GetFaction();
    if (NULL != poFaction)
    {
        DT_FACTION_PLAYER_CLI stCli = {0};
        poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, poPlayer->GetID());
        pstAck->dwSyncDoorsTribute = stCli.dwDoorsTributeValues;
    }
    return CLIGS_CLOSED_MONSTER_ACK;
}

UINT16 CGTPktBuilder::OnFactionPlayerMarkAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_FACTION_PLAYER_CHANGE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_FACTION_PLAYER_CHANGE::ID_UNOPEN;
        return CLIGS_FACTION_PLAYER_CHANGE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_FACTION_PLAYER_CHANGE::ID_UNOPEN;
    }
    else
    {
        pstAck->byChangeFlag = poFactionPlayer->GetDT_PLAYER_MARK_DATA().byFactionPlayerMark;
        poFactionPlayer->GetDT_PLAYER_MARK_DATA().byFactionPlayerMark = ESM_NO;
        pstAck->wErrCode = ERR_FACTION_PLAYER_CHANGE::ID_SUCCESS;
    }



    return CLIGS_FACTION_PLAYER_CHANGE_ACK;
}

UINT16 CGTPktBuilder::OnOpenLeaveFactionAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_LEAVE_FACTION_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_LEAVE_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_LEAVE_FACTION_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_LEAVE_FACTION::ID_UNOPEN;
        return CLIGS_OPEN_LEAVE_FACTION_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OpenLeaveFaction(pstAck->dwIntervalTime, pstAck->byLeaveFactionMark);

    return CLIGS_OPEN_LEAVE_FACTION_ACK;
}

UINT16 CGTPktBuilder::OnOpenFactionUpgradeAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_FACTION_UPGRADE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = CFactionMgr::Instance()->OpenFactionUpgrade(poPlayer->GetID(), pstAck->stUpgradeAllInfo);

    return CLIGS_OPEN_FACTION_UPGRADE_ACK;
}

UINT16 CGTPktBuilder::OnFactionUpgradeAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_FACTION_UPGRADE_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = CFactionMgr::Instance()->FactionUpgrade(poPlayer->GetID(), *pstAck);

    return CLIGS_FACTION_UPGRADE_ACK;
}

UINT16 CGTPktBuilder::GetFactionBattleLogAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_PLAYER_FACTION_READ_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CFactionPlayer* poFactonPlayer = poPlayer->GetFactionPlayer();
    if (NULL != poFactonPlayer)
    {
        pstAck->wErrCode = poFactonPlayer->GetFactinBattleLogByCli(pstAck->byLogNum, pstAck->astPlayerFactionLogList);
    }


    return CLIGS_PLAYER_FACTION_READ_ACK;
}

UINT16 CGTPktBuilder::OpenFindDrinkAck( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET( PKT_CLIGS_OPEN_FIND_DRINK_ACK );

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_OPEN_FIND_DRINK::ID_FACTION_NOT_EXIST;
        return CLIGS_OPEN_FIND_DRINK_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->OpenFindDrink();

    return CLIGS_OPEN_FIND_DRINK_ACK;
}

// 装备精通：打开装备精通面板
UINT16 CGTPktBuilder::OnOpenEquipMasterAck(CPlayer& oPlayer)
{
    ZERO_PACKET(PKT_CLIGS_OPEN_EQUIP_MASTER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_OPEN_EQUIP_MASTER::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnOpenEquipMaster(*pstAck);
    return CLIGS_OPEN_EQUIP_MASTER_ACK;
}

// 装备精通：激活装备精通
UINT16 CGTPktBuilder::OnActivateEquipMasterAck(CPlayer& oPlayer, BYTE byEquipKind)
{
    ZERO_PACKET(PKT_CLIGS_ACTIVATE_MASTER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_ACTIVATE_MASTER::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnActivateEquipMaster(*pstAck, (EEquipPosType)byEquipKind);
    return CLIGS_ACTIVATE_MASTER_ACK;
}

// 装备精通：突破
UINT16 CGTPktBuilder::OnBreakMasterAck(CPlayer& oPlayer, BYTE byEquipKind)
{
    ZERO_PACKET(PKT_CLIGS_BREAK_MASTER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_BREAK_MASTER::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnBreakMaster(*pstAck, (EEquipPosType)byEquipKind);
    return CLIGS_BREAK_MASTER_ACK;
}

// 装备精通：培养精通之书
UINT16 CGTPktBuilder::OnGetMasterBookAck(CPlayer& oPlayer)
{
    ZERO_PACKET(PKT_CLIGS_GET_MASTER_BOOK_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_GET_MASTER_BOOK::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnCultivateMasterBook(*pstAck);
    return CLIGS_GET_MASTER_BOOK_ACK;
}

// 装备精通：刮开精通经验书面板
UINT16 CGTPktBuilder::OnWipeMasterBookAck(CPlayer& oPlayer)
{
    ZERO_PACKET(PKT_CLIGS_WIPE_MASTER_BOOK_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_WIPE_MASTER_BOOK::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnWipeMasterBook(*pstAck);
    return CLIGS_WIPE_MASTER_BOOK_ACK;
}

// 装备精通：出千
UINT16 CGTPktBuilder::OnMasterBookCheatAck(CPlayer& oPlayer)
{
    ZERO_PACKET(PKT_CLIGS_MASTER_CHEAT_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_MASTER_CHEAT::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnCheatMasterBook(*pstAck);
    return CLIGS_MASTER_CHEAT_ACK;
}

// 装备精通：确认使用精通经验书
UINT16 CGTPktBuilder::OnConfirmMasterBookAck(CPlayer& oPlayer)
{
    ZERO_PACKET(PKT_CLIGS_CONFIRM_MASTER_BOOK_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    if(NULL == oPlayer.GetEquipMaster())
    {
        return ERR_CONFIRM_MASTER_BOOK::ID_UNOPEN;
    }

    pstAck->wErrCode = oPlayer.GetEquipMaster()->OnConfirmMasterBook(*pstAck);
    return CLIGS_CONFIRM_MASTER_BOOK_ACK;
}

// 打开勇气试炼
UINT16 CGTPktBuilder::OnOpenCourageReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_COURAGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    pstAck->wErrCode = CCourageMgr::Instance()->OnOpenCourage(poPlayer, pstAck->stPlayerCourageData, pstAck->stGroupData, pstAck->bySelect);
    pstAck->qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCopper = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    pstAck->qwMedal = poPlayer->GetDT_PLAYER_BASE_DATA().qwMedal;
    pstAck->qwStory = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
    return CLIGS_OPEN_COURAGE_ACK;
}

//打开创建队伍
UINT16 CGTPktBuilder::OnCourageOpenCreateGroupReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageOpenCreateGroup(poPlayer, *pstAck);

    return CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK;
}

//确认创建队伍
UINT16 CGTPktBuilder::OnCourageCreateGroupReq(CPlayer* poPlayer, UINT8 byJoinType ,DT_INSTANCE_ID wInstanceID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_CREATE_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageCreateGroup(poPlayer, byJoinType, wInstanceID, pstAck->dwGroupID);

    return CLIGS_COURAGE_CREATE_GROUP_ACK;
}

//加入队伍或者协助队伍
UINT16 CGTPktBuilder::OnCourageJoinGroupReq( CPlayer* poPlayer , UINT8 byBattleType, UINT32 dwGroupID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_JOIN_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageJoinGroup(poPlayer, byBattleType, dwGroupID);

    return CLIGS_COURAGE_JOIN_GROUP_ACK;
}

//添加或者更改伙伴
UINT16 CGTPktBuilder::OnCourageOperateHeroReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_OPERATE_HERO_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageOperateHero(poPlayer, pstAck->stHeroData);

    return CLIGS_COURAGE_OPERATE_HERO_ACK;
}

//确认添加或者更改伙伴
UINT16 CGTPktBuilder::OnCourageConfirmHeroReq( CPlayer* poPlayer , UINT16 dwKindID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_CONFIRM_HERO_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageConfirmHero(poPlayer, dwKindID, pstAck->stHeroData);

    return CLIGS_COURAGE_CONFIRM_HERO_ACK;
}

//踢掉队员
UINT16 CGTPktBuilder::OnCourageFireGroupMemberReq( CPlayer* poPlayer, UINT32 dwGroupID, UINT32 dwPlayID)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageFireGroupMember(poPlayer,dwGroupID,dwPlayID, pstAck->dwPlayerID,pstAck->wAverageLevel);

    return CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK;
}

//购买试炼
UINT16 CGTPktBuilder::OnCourageBuyPractiseTimesReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageBuyPractiseTimes(poPlayer, *pstAck);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    return CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK;
}

//玩家准备
UINT16 CGTPktBuilder::OnCouragePlayerReadyReq( CPlayer* poPlayer , UINT8 byTpye)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_PLAYER_READY_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCouragePlayerReady(poPlayer, byTpye, pstAck->byReadyType);

    return CLIGS_COURAGE_PLAYER_READY_ACK;
}

//玩家开始
UINT16 CGTPktBuilder::OnCouragPlayerStartReq( CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    if(poPlayer == NULL)
    {
        return FALSE;
    }
	if ( poPlayer->GetCliVer() >= VERSION_PET )
	{
		ZERO_PACKET(PKT_CLIGS_COURAGE_PLAYER_START_ACK5);
		if (NULL == pstAck)
		{
			return INVALID_MSGID;
		}


		pstAck->wErrCode = CCourageMgr::Instance()->OnCouragPlayerStart(poPlayer, *pstAck);

		return CLIGS_COURAGE_PLAYER_START_ACK5;
	}
	else if (poPlayer->GetCliVer() >= VERSION_SOUL)
	{
		ZERO_PACKET(PKT_CLIGS_COURAGE_PLAYER_START_ACK3);
		if (NULL == pstAck)
		{
			return INVALID_MSGID;
		}


		pstAck->wErrCode = CCourageMgr::Instance()->OnCouragPlayerStart3(poPlayer, *pstAck);

		return CLIGS_COURAGE_PLAYER_START_ACK3;
	}
    else if ((poPlayer->GetCliVer() > 310))
    {
        ZERO_PACKET(PKT_CLIGS_COURAGE_PLAYER_START_ACK2);
        if (NULL == pstAck)
        {
            return INVALID_MSGID;
        }


        pstAck->wErrCode = CCourageMgr::Instance()->OnCouragPlayerStart2(poPlayer, *pstAck);

        return CLIGS_COURAGE_PLAYER_START_ACK2;
    }

    ZERO_PACKET(PKT_CLIGS_COURAGE_PLAYER_START_ACK5);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }


    pstAck->wErrCode = CCourageMgr::Instance()->OnCouragPlayerStart(poPlayer, *pstAck);

    return CLIGS_COURAGE_PLAYER_START_ACK5;

}

//玩家离开
UINT16 CGTPktBuilder::OnCouragLeaveGroupReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_LEAVE_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCouragLeaveGroup(poPlayer, pstAck->stPlayerCourageData, pstAck->stGroupData);

    return CLIGS_COURAGE_LEAVE_GROUP_ACK;
}

//打开购买试炼
UINT16 CGTPktBuilder::OnCourageOpenShopReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_COURAGE_SHOP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageOpenShop(poPlayer, pstAck->byLeftBuyTimes, pstAck->dwBuyGold);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    return CLIGS_OPEN_COURAGE_SHOP_ACK;
}


// 勇气战斗应答
UINT16 CGTPktBuilder::OnCourageBattleAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_BATTLE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    //pstAck->wErrCode = CCourageMgr::Instance()->OnCourageOpenShop(poPlayer, pstAck->byBuyTimes, pstAck->dwBuyGold);
    //pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    return CLIGS_COURAGE_BATTLE_ACK;
}

UINT16 CGTPktBuilder::OpenFactionBuildGroupAck(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_FACTION_BUILD_GROUP::ID_UNOPEN;
        return CLIGS_OPEN_FACTION_BUILD_GROUP_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        return ERR_OPEN_FACTION_BUILD_GROUP::ID_UNOPEN;
    }
    pstAck->wErrCode = poFactionPlayer->GetFactionBuileGroup( *pstAck);
    return CLIGS_OPEN_FACTION_BUILD_GROUP_ACK;
}

UINT16 CGTPktBuilder::HotSpringRewardNtf(CPlayer* poPlayer)
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_HOT_SPRING_REWARD_NTF);
    if ( NULL == pstAck)
    {
        return INVALID_MSGID;
    }


    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        return INVALID_MSGID;
    }

    poFactionPlayer->GetDrinkReward(pstAck);

    return CLIGS_HOT_SPRING_REWARD_NTF;
}

UINT16 CGTPktBuilder::OnCouragRefreshGroup( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_GROUP_REFRESH_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageRefreshGroup(poPlayer, pstAck->stGroupData);

    return CLIGS_COURAGE_GROUP_REFRESH_ACK;
}
UINT16 CGTPktBuilder::OnCourageEnterGroupReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_ENTER_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageEnterGroup(poPlayer, pstAck->stGroupData);

    return CLIGS_COURAGE_ENTER_GROUP_ACK;
}

//离开勇气试炼
UINT16 CGTPktBuilder::OnLeaveCourageReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_LEAVE_COURAGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnLeaveCourage(poPlayer);

    return CLIGS_LEAVE_COURAGE_ACK;
}
UINT16 CGTPktBuilder::OnHotSpringOverRewardAck( CPlayer* poPlayer )
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK);
    if ( NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_HOT_SPRING_OVER_REWARD::ID_UNOPEN;
        return CLIGS_HOT_SPRING_OVER_REWARD_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_HOT_SPRING_OVER_REWARD::ID_UNOPEN;
        return CLIGS_HOT_SPRING_OVER_REWARD_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->OverRewardAck();

    return CLIGS_HOT_SPRING_OVER_REWARD_ACK;
}

UINT16 CGTPktBuilder::HuntingIntegralAck(CPlayer* poPlayer)
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_HUNTING_ITEGRAL_ACK);
    if ( NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_HUNTING_ITEGRAL::ID_UNOPEN;
        return CLIGS_HOT_SPRING_OVER_REWARD_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_HUNTING_ITEGRAL::ID_UNOPEN;
        return CLIGS_HOT_SPRING_OVER_REWARD_ACK;
    }
    pstAck->wErrCode = poFactionPlayer->GetHuntingIntegral(*pstAck);

    return CLIGS_HOT_SPRING_OVER_REWARD_ACK;
}

UINT16 CGTPktBuilder::OnIgnorePromptAck( CPlayer* poPlayer, UINT8 byActiviID )
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_PROMPT_IGNORE_ACK);
    if ( NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_PROMPT_IGNORE::ID_UNOPEN;
        return CLIGS_PROMPT_IGNORE_ACK;
    }
    poBuild->OnOpen();

    pstAck->wErrCode = poPlayer->SetIgnorePrompt(byActiviID, *pstAck);

    return CLIGS_PROMPT_IGNORE_ACK;
}

UINT16 CGTPktBuilder::OnOpenLeaveHotSpring(CPlayer* poPlayer)
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK);
    if ( NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_LEAVE_HOT_SPRING::ID_IS_UNOPEN;
        return CLIGS_OPEN_LEAVE_HOT_SPRING_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer )
    {
        pstAck->wErrCode = ERR_OPEN_LEAVE_HOT_SPRING::ID_IS_UNOPEN;
        return CLIGS_OPEN_LEAVE_HOT_SPRING_ACK;
    }
    CHotSpring& oHotSpring = poFactionPlayer->GetHotSpring();
    pstAck->wErrCode = oHotSpring.GetOpenLeaveHotSpringMsg(pstAck->aszPromptMsg);
    return CLIGS_OPEN_LEAVE_HOT_SPRING_ACK;
}

//勇气战斗请求
UINT16 CGTPktBuilder::OnCourageBattleReq( CPlayer *poPlayer, UINT8 byIndex )
{
    return CLIGS_COURAGE_BATTLE_ACK;
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_BATTLE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    //pstAck->wErrCode = CCourageMgr::Instance()->CourageBattle(poPlayer,*pstAck,byIndex);

    return CLIGS_COURAGE_BATTLE_ACK;
}

//领取勇气奖励
UINT16 CGTPktBuilder::OnCourageGetAwardReq( CPlayer* poPlayer, UINT32 dwIndex)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_GET_AWARD_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageGetAwardReq(poPlayer,*pstAck,dwIndex);

    return CLIGS_COURAGE_GET_AWARD_ACK;
}

//勇气战斗结果
UINT16 CGTPktBuilder::OnCouragGetBattleResultReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCouragGetBattleResultReq(poPlayer,*pstAck);

    return CLIGS_COURAGE_GET_BATTLE_RESULT_ACK;
}


UINT16 CGTPktBuilder::OnAuditAllRefuseAck(CPlayer* poPlayer)
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_AUDIT_ALL_REFUSE_ACK);
    if ( NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_AUDIT_ALL_REFUSE::ID_UNOPEN;
        return CLIGS_AUDIT_ALL_REFUSE_ACK;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_AUDIT_ALL_REFUSE::ID_UNOPEN;
        return CLIGS_AUDIT_ALL_REFUSE_ACK;
    }

    pstAck->wErrCode = poFactionPlayer->AuditAllRefuse();

    return CLIGS_AUDIT_ALL_REFUSE_ACK;
}

UINT16 CGTPktBuilder::OnOpenRecvFlowerTabAck(CPlayer* poPlayer)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_OPEN_RECV_FLOWER_TAB_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_OPEN_RECV_FLOWER_TAB_ACK;
    }

    pstAck->wErrCode = poFriends->OpenRecvFlowerTab(pstAck->stRecvFlowerLogListCLiEx);
    return CLIGS_OPEN_RECV_FLOWER_TAB_ACK;
}


UINT16 CGTPktBuilder::OnAcceptFlowerAck(CPlayer* poPlayer, UINT32 dwSendPlayerID, UINT64 qwSendTime)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_ACCEPT_FLOWER_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_ACCEPT_FLOWER_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_ACCEPT_FLOWER_ACK;
    }
    pstAck->wErrCode = poFriends->AcceptFlower(dwSendPlayerID, qwSendTime, *pstAck);
    return CLIGS_ACCEPT_FLOWER_ACK;
}

// 打开送花面板
UINT16 CGTPktBuilder::OnOpenSendFlowerTabExAck(CPlayer* poPlayer)
{
    //
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_OPEN_SEND_FLOWER_TAB::ID_NO_OPEN;
        return CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK;
    }
    poBuild->OnOpen();

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_OPEN_SEND_FLOWER_TAB::ID_NO_OPEN;
        return CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK;
    }
    pstAck->wErrCode = poFriends->OpenSendFlowerTabEx(*pstAck);
    return CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK;
}



UINT16 CGTPktBuilder::OnSendFlowerExAck(CPlayer* poPlayer, CPlayer *poTargetPlayer, UINT32 dwKindID)
{
    //
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_SEND_FLOWER_EX_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild *	poBuild = poPlayer->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_SEND_FLOWER_EX_ACK;
    }
    poBuild->OnOpen();
	CFriends* poTargetFriends = poTargetPlayer->GetFriends();
	if ( NULL == poTargetFriends)
	{
		pstAck->wErrCode = ERR_SEND_FLOWER::ID_OTHER_NOT_OPEN_FRIENDS;
		return CLIGS_SEND_FLOWER_EX_ACK;
	}
	if( EFSRT_FRIEND != poTargetFriends->CkRelationType(poPlayer->GetID()))
	{
		pstAck->wErrCode = ERR_SEND_FLOWER::ID_YOU_NOT_OTHER_FRIEND;
		return CLIGS_SEND_FLOWER_EX_ACK;
	}
    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        pstAck->wErrCode = ERR_SEND_FLOWER::ID_NO_OPEN;
        return CLIGS_SEND_FLOWER_EX_ACK;
    }
    pstAck->wErrCode = poFriends->SendFlowerEx(poTargetPlayer, dwKindID, *pstAck);
	poPlayer->GetPhystrength().GetPhyStrengthInfo(pstAck->stRsyncResItemData.stCurPhyStrengthInfo);
    return CLIGS_SEND_FLOWER_EX_ACK;
}

UINT16 CGTPktBuilder::OnBuyRareItemAck(CPlayer* poPlayer, UINT32 dwGoodsID, UINT8 byLeftTime)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_BUY_RAREITEM_ACK2);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_BUY_RAREITEM::ID_UNOPEN;
        return CLIGS_BUY_RAREITEM_ACK2;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_BUY_RAREITEM::ID_UNOPEN;
        return CLIGS_BUY_RAREITEM_ACK2;
    }

    pstAck->wErrCode = poFactionPlayer->ExchangeRareItem( pstAck->stRareItemInfo, pstAck->dwLeftDoorsTributes, dwGoodsID, byLeftTime);
    return CLIGS_BUY_RAREITEM_ACK2;
}

UINT16 CGTPktBuilder::OnLearnGymnasiumSkillAck2(CPlayer* poPlayer, UINT32 dwSkillID, UINT8 bySkillCurLevel)
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2);

    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_LEARN_GYMNASIUM_SKILL::ID_UNOPEN;
        return CLIGS_LEARN_GYMNASIUM_SKILL_ACK2;
    }
    poBuild->OnOpen();

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        pstAck->wErrCode = ERR_LEARN_GYMNASIUM_SKILL::ID_UNOPEN;
        return CLIGS_LEARN_GYMNASIUM_SKILL_ACK2;
    }

    pstAck->stSkillInfo.bySkillID = static_cast<UINT8>(dwSkillID);
    pstAck->wErrCode = poFactionPlayer->LearnGymnasiumSkill( *pstAck, bySkillCurLevel);
    return CLIGS_LEARN_GYMNASIUM_SKILL_ACK2;
}

//战斗时候离开勇气试炼
UINT16 CGTPktBuilder::OnCourageLeaveBattleReq( CPlayer *poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnLeaveBattle(poPlayer);

    return CLIGS_LEAVE_COURAGE_BATTLE_ACK;
}

UINT16 CGTPktBuilder::OnCourageRandomJoinGroupReq( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }
    pstAck->wErrCode = CCourageMgr::Instance()->OnCourageRandomJoinGroup(poPlayer, *pstAck);

    return CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK;
}


UINT16 CGTPktBuilder::OnFactionPlayerInfoAck(CPlayer* poPlayer)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_FACTION_PLAYERS_INFO_ACK);

	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_FACTION_PLAYERS_INFO::ID_IS_UNOPEN;
		return CLIGS_FACTION_PLAYERS_INFO_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_FACTION_PLAYERS_INFO::ID_IS_UNOPEN;
		return CLIGS_FACTION_PLAYERS_INFO_ACK;
	}

	pstAck->wErrCode = poFactionPlayer->FactionPlayerInfoAck( *pstAck);
	return CLIGS_FACTION_PLAYERS_INFO_ACK;
}
UINT16 CGTPktBuilder::OnRegisterFactionNameAck(CPlayer* poPlayer, CHAR* pszDspName)
{
    ZERO_PACKET(PKT_CLILS_REGISTER_FACTION_ACK);
    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if(NULL == poBuild)
    {
        pstAck->wErrCode = ERR_REGISTER_FACTION::ID_IS_UNOPEN;
        return CLILS_REGISTER_FACTION_ACK;
    }
    poBuild->OnOpen();
    CFaction* poFaction = poPlayer->GetFaction();
    if (NULL == poFaction)
    {
        pstAck->wErrCode = ERR_REGISTER_FACTION::ID_IS_UNOPEN;
        return CLILS_REGISTER_FACTION_ACK;
    }

    pstAck->wErrCode = poFaction->RegisterFactionName(pszDspName, poPlayer);
    SDStrncpy(pstAck->aszDspName, pszDspName, USERNAME_LEN - 1);

    return CLILS_REGISTER_FACTION_ACK;
}
UINT16 CGTPktBuilder::OnOpenChangeIconAck(CPlayer* poPlayer)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK);

	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_OPEN_CHANGE_FACTION_ICON::ID_IS_UNOPEN;
		return CLIGS_OPEN_CHANGE_FACTION_ICON_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_OPEN_CHANGE_FACTION_ICON::ID_IS_UNOPEN;
		return CLIGS_OPEN_CHANGE_FACTION_ICON_ACK;
	}

	pstAck->wErrCode = poFactionPlayer->OnOpenChangeIconAck( *pstAck);
	return CLIGS_OPEN_CHANGE_FACTION_ICON_ACK;
}

UINT16 CGTPktBuilder::OnChangeIconAck(CPlayer* poPlayer, UINT8 byIconID)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_CHANGE_FACTION_ICON_ACK);

	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_CHANGE_FACTION_ICON::ID_IS_UNOPEN;
		return CLIGS_CHANGE_FACTION_ICON_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_CHANGE_FACTION_ICON::ID_IS_UNOPEN;
		return CLIGS_CHANGE_FACTION_ICON_ACK;
	}

	pstAck->wErrCode = poFactionPlayer->OnChangeIconAck( *pstAck, byIconID);
	return CLIGS_CHANGE_FACTION_ICON_ACK;
}

UINT16 CGTPktBuilder::OnOpenFindBackDoorsTributeAck(CPlayer* poPlayer)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK);

	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_OPEN_FIND_BACK_DOORSTRIBUTE::ID_IS_UNOPEN;
		return CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_OPEN_FIND_BACK_DOORSTRIBUTE::ID_IS_UNOPEN;
		return CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK;
	}

	pstAck->wErrCode = poFactionPlayer->OnOpenFindBackDoorsTributeAck( *pstAck);
	return CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK;
}

UINT16 CGTPktBuilder::OnFindBackDoorsTributeAck(CPlayer* poPlayer, UINT8 byFindBackType)
{
	if( NULL == poPlayer )
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK);

	if (NULL == pstAck)
	{
		return INVALID_MSGID;
	}

	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_FIND_BACK_DOORSTRIBUTE::ID_IS_UNOPEN;
		return CLIGS_FIND_BACK_DOORSTRIBUTE_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_FIND_BACK_DOORSTRIBUTE::ID_IS_UNOPEN;
		return CLIGS_FIND_BACK_DOORSTRIBUTE_ACK;
	}

	pstAck->wErrCode = poFactionPlayer->OnFindBackDoorsTributeAck(byFindBackType, *pstAck);
	return CLIGS_FIND_BACK_DOORSTRIBUTE_ACK;
}

UINT16 CGTPktBuilder::OnOpenEmbattleAck2(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_EMBATTLE_ACK2);
	CBuild* poBuild = poPlayer->GetBuild(EBK_FROMATION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_OPEN_EMBATTLE::ID_FORMATION_NOT_OPEN;
		return CLIGS_OPEN_EMBATTLE_ACK;
	}
	poBuild->OnOpen();

	CFormation &oFormation = poPlayer->GetFormation();

	pstAck->wErrCode = oFormation.OpenEmbattleTab2(*pstAck);

	return CLIGS_OPEN_EMBATTLE_ACK2;
}

UINT16 CGTPktBuilder::OnUpgradeFormationAck(CPlayer* poPlayer, UINT8 byFormationID, DT_FORMATION_DATA& stFormation)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_UPGRADE_FORMATION_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_FROMATION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_UPGRADE_FORMATION::ID_FORMATION_NOT_OPEN;
		return CLIGS_UPGRADE_FORMATION_ACK;
	}
	poBuild->OnOpen();

	CFormation &oFormation = poPlayer->GetFormation();

	pstAck->wErrCode = oFormation.OnUpgradeFormation(*pstAck, byFormationID, stFormation);

	return CLIGS_UPGRADE_FORMATION_ACK;
}

UINT16 CGTPktBuilder::OnOpenFormationScienceAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_SCIENCE);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_OPEN_FORMATION_SCIENCE::ID_SCIENCE_NOT_OPEN;
		return CLIGS_OPEN_FORMATION_SCIENCE_ACK;
	}
	poBuild->OnOpen();

	CFormation &oFormation = poPlayer->GetFormation();

	pstAck->wErrCode = oFormation.OnOpenFormationScience(*pstAck);

	return CLIGS_OPEN_FORMATION_SCIENCE_ACK;

}

UINT16 CGTPktBuilder::OnGetBlueEncourage( CPlayer* poPlayer )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }
    ZERO_PACKET(PKT_CLIGS_GET_BLUE_ENCOURAGE_ACK);
    if (NULL == pstAck)
    {
        return INVALID_MSGID;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_BEGBLUE);
    if ( NULL == pBuild)
    {
        pstAck->wErrCode = ERR_GET_BLUE_ENCOURAGE::ID_BUILD_NOT_EXIST;
        return CLIGS_GET_BLUE_ENCOURAGE_ACK;
    }

    CBegBlue *poBegBlue = poPlayer->GetBegBlue();
    if ( NULL == poBegBlue)
    {
        pstAck->wErrCode = ERR_GET_BLUE_ENCOURAGE::ID_BUILD_NOT_EXIST;
        return CLIGS_GET_BLUE_ENCOURAGE_ACK;
    }

    pstAck->wErrCode = poBegBlue->GetBlueEncourage(pstAck->stEncourage,pstAck->dwBlue);
    pstAck->stPlayerResInfo.qwGold  = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->stPlayerResInfo.qwBlue  =  poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    pstAck->stPlayerResInfo.qwStory =  poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;

    return CLIGS_GET_BLUE_ENCOURAGE_ACK;
}

UINT16 CGTPktBuilder::OnUpgradeFormationScienceAck(CPlayer* poPlayer, UINT8 byFormationID)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_SCIENCE);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_NOT_OPEN;
		return CLIGS_UPGRADE_FORMATION_ACK;
	}
	poBuild->OnOpen();

	CFormation &oFormation = poPlayer->GetFormation();

	pstAck->wErrCode = oFormation.OnUpgradeFormationScience(*pstAck, byFormationID);

	return CLIGS_UPGRADE_FORMATION_SCIENCE_ACK;
}
UINT16 CGTPktBuilder::OnGetExpeienceHotSpringAck(CPlayer* poPlayer, UINT32 dwExprienceValue)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_GET_EXPRIENCE_HOT_SPRING::ID_FACTION_NOT_OPEN;
		return CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_GET_EXPRIENCE_HOT_SPRING::ID_FACTION_NOT_OPEN;
		return CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK;
	}
	pstAck->wErrCode = poFactionPlayer->GetExprienceHotSpring(*pstAck, dwExprienceValue);

	return CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK;
}

UINT16 CGTPktBuilder::OnOpenFactionSkillAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_FACTION_SKILL_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_OPEN_FACTION_SKILL::ID_FACTION_NOT_OPEN;
		return CLIGS_OPEN_FACTION_SKILL_ACK;
	}
	poBuild->OnOpen();

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		pstAck->wErrCode = ERR_OPEN_FACTION_SKILL::ID_FACTION_NOT_OPEN;
		return CLIGS_OPEN_FACTION_SKILL_ACK;
	}
	pstAck->wErrCode = poFactionPlayer->OpenFactionSkill(*pstAck);

	return CLIGS_OPEN_FACTION_SKILL_ACK;
}

UINT16 CGTPktBuilder::OnApplyGvGAck(CPlayer& oPlayer)
{
    CFaction *poFaction = oPlayer.GetFaction();
    if(NULL == poFaction)
    {
        return INVALID_MSGID;
    }

    // 检测是否是门派掌门
    em_Faction_Job eJob = poFaction->GetJob(oPlayer.GetID());
    if(em_Faction_MartialHead != eJob)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_APPLY_GVG_ACK);
    pstAck->byExt = 0;

    return 0;
}

UINT16 CGTPktBuilder::OnOpenResolveTrigram( CPlayer* poPlayer, UINT8 byPos )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_OPEN_RESOLVE_TRIGRAM::ID_UNOPEN;
        return CLIGS_OPEN_RESOLVE_TRIGRAM_ACK;
    }

    pstAck->wErrCode = poAwaken->OnOpenResolveTrigram(byPos, *pstAck);

    return CLIGS_OPEN_RESOLVE_TRIGRAM_ACK;
}


UINT16 CGTPktBuilder::OnResolveTrigram( CPlayer* poPlayer, UINT8 byPos ,UINT8 byUseGold )
{
    if( NULL == poPlayer )
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_RESOLVE_TRIGRAM_ACK);

    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        pstAck->wErrCode = ERR_RESOLVE_TRIGRAM::ID_UNOPEN;
        return CLIGS_RESOLVE_TRIGRAM_ACK;
    }

    pstAck->wErrCode = poAwaken->OnResolveTrigramReq(byPos, byUseGold, *pstAck);
    pstAck->qwGoldNum = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    pstAck->qwCoinNum = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

    return CLIGS_RESOLVE_TRIGRAM_ACK;
}
UINT16 CGTPktBuilder::OnOpenPlayerPanelAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}
	if ( VERSION_PET <= poPlayer->GetCliVer() )
	{
		ZERO_PACKET(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410);

		pstAck->wCoachExpRate = CSinglePramMgr::Instance()->GetCoachExpRate();
		pstAck->wErrCode = poPlayer->OpenPlayerPanel_v410(*pstAck);
		return CLIGS_OPEN_PLAYER_PANEL_ACK_v410;
	}
	else
	{
		ZERO_PACKET(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK);

		pstAck->wCoachExpRate = CSinglePramMgr::Instance()->GetCoachExpRate();
		pstAck->wErrCode = poPlayer->OpenPlayerPanel(pstAck->stPlayerPanel, pstAck->stBagInfo);
		if ( poPlayer->GetCliVer() < VERSION_GVG_ACTIVITY)
		{
			PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD stOldAck;
			memset(&stOldAck, 0, sizeof(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD));
			stOldAck.stBagInfo = pstAck->stBagInfo;
			stOldAck.wCoachExpRate = pstAck->wCoachExpRate;
			stOldAck.stPlayerPanel.byGoodsCellNum = pstAck->stPlayerPanel.byGoodsCellNum;
			stOldAck.stPlayerPanel.byJewelryCellNum = pstAck->stPlayerPanel.byJewelryCellNum;
			stOldAck.stPlayerPanel.stEquipXiangqianDes = pstAck->stPlayerPanel.stEquipXiangqianDes;
			memcpy(&stOldAck.stPlayerPanel.stHeroInfo, &pstAck->stPlayerPanel.stHeroInfo, sizeof(stOldAck.stPlayerPanel.stHeroInfo));
			stOldAck.stPlayerPanel.stItemInfo = pstAck->stPlayerPanel.stItemInfo;
			stOldAck.stPlayerPanel.stJewelryInfo = pstAck->stPlayerPanel.stJewelryInfo;
			ZERO_PACKET(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD);
			memcpy(pstAck, &stOldAck, sizeof(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD));
			return CLIGS_OPEN_PLAYER_PANEL_ACK_OLD;
		}
		return CLIGS_OPEN_PLAYER_PANEL_ACK;
	}
	
}

UINT16 CGTPktBuilder::OnOpenPlayerInfoAwakenAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_AWAKEN_ACK);
	
	CAwaken* poAwaken = poPlayer->GetAwaken();
	if(NULL == poAwaken)
	{
		pstAck->byAwakenOpenState = ESM_NO;
		pstAck->wErrCode = ERR_PLAYER_INFO_AWAKEN::ID_SUCCESS;
		return CLIGS_PLAYER_INFO_AWAKEN_ACK;
	}
	poAwaken->GetAwakenVersion() = 2;
	pstAck->wErrCode = poAwaken->OpenPlayerInfoAwaken(*pstAck);
	pstAck->byAwakenOpenState = ESM_YES;
	return CLIGS_PLAYER_INFO_AWAKEN_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlayerInfoSkillAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_SKILL_ACK);

	pstAck->wErrCode = poPlayer->OpenPlayerInfoSkill(*pstAck);

	return CLIGS_PLAYER_INFO_SKILL_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlayerInfoGodWeaponAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_GODWEAPON);
	if(NULL == poBuild)
	{
		pstAck->wErrCode = ERR_PLAYER_INFO_GODWEAPON::ID_IS_UNOPEN;
		return CLIGS_PLAYER_INFO_GODWEAPON_ACK;
	}
	poBuild->OnOpen();
	
	pstAck->wErrCode = CGodweaponMgr::Instance()->OpenGodWeaponByPlayerInfo(poPlayer, pstAck->wGodweaponNum, pstAck->astGodweaponLst, pstAck->abyLockStatu);

	return CLIGS_PLAYER_INFO_GODWEAPON_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlayerInfoJewelryAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_JEWELRY_ACK);
	

	pstAck->wErrCode = poPlayer->OnGetPlayerInfoJewelry(*pstAck);

	return CLIGS_PLAYER_INFO_JEWELRY_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlayerInfoEquipAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_EQUIP_ACK);


	pstAck->wErrCode = poPlayer->GetPlayerInfoEquip(*pstAck);

	return CLIGS_PLAYER_INFO_EQUIP_ACK;
}
UINT16 CGTPktBuilder::OnOpenPlayerInfoPetAck(CPlayer* poPlayer)
{
	if(NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_PET_ACK);


	pstAck->wErrCode = CPetMng::Instance()->GetBattlePetInfo(poPlayer, *pstAck);
	
	return CLIGS_PLAYER_INFO_PET_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlayerInfoSoulAck(CPlayer* poPlayer) {
	if(NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_PLAYER_INFO_SOUL_ACK);

	CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		pstAck->wErrCode = ERR_CLIGS_PLAYER_INFO_SOUL::ID_IS_UNOPEN;
	} else {
		vector<CHero*> setHero;
		poPlayer->GetPlayerAllHero(setHero);
		if (setHero.empty()) {
			return ERR_CLIGS_PLAYER_INFO_SOUL::ID_COACH_HERO_LOST;
		}
		UINT32 unIndex = 0;
		for (vector<CHero*>::iterator itor = setHero.begin(), end = setHero.end()
			; end != itor
			;++ itor) 
		{
			if (NULL == (*itor)) {
				continue;
			}
			pstAck->astSoulInfo[unIndex].wSoulEquipNum = (*itor)->GetHeroSoulData().wSoulNum;
			pstAck->astSoulPotencyInfo[unIndex].wSoulEquipNum = pstAck->astSoulInfo[unIndex].wSoulEquipNum;
			for (size_t n = 0; n < pstAck->astSoulInfo[unIndex].wSoulEquipNum ; ++n) {
				CSoul soul;
				soul.Init(poPlayer, (*itor)->GetHeroSoulData().astSoulLstData[n], (*itor)->GetHeroSoulData().astSoulPotencyLstData[n]);
				soul.GetSoulCli(pstAck->astSoulInfo[unIndex].astSoulEquipLstData[n]);
				soul.GetSoulPotencyCli(pstAck->astSoulPotencyInfo[unIndex].astSoulEquipLstData[n]);
			}
			++ unIndex;
		}
		
		pstAck->byHeroNum = unIndex;
		pstAck->wErrCode = ERR_CLIGS_PLAYER_INFO_SOUL::ID_SUCCESS;
	}
	return CLIGS_PLAYER_INFO_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnDressJewelryAck(CPlayer* poPlayer, PKT_CLIGS_DRESS_JEWELRY_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_DRESS_JEWELRY_ACK);
	
	CHero* poHero = poPlayer->GetAllCarryHeroMap().GetData(stReq.wHeroKindID);
	if ( NULL == poHero)
	{
		pstAck->wErrCode = ERR_DRESS_JEWELRY::ID_HERO_ID_NOT_FIND;
		return CLIGS_DRESS_JEWELRY_ACK;
	}

	pstAck->wErrCode = poHero->OnDressJewelryAck(stReq, *pstAck);
	if ( 1 == stReq.byReqPos)//合成面板下发数据 
	{
		pstAck->byIsDown = ESM_YES;
		poPlayer->GetBag().GetDT_JEWELRY_DATA_LST_CLI( pstAck->astJewelryInfo[0]);
	}
	pstAck->stBuildPrompt.byBuildNum = 1;
	poPlayer->GetJewelryPrompt(pstAck->stBuildPrompt.astBuildPromptInfo[0]);
	return CLIGS_DRESS_JEWELRY_ACK;
}

UINT16 CGTPktBuilder::OnOpenHeroSelectPanelAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK);

	pstAck->wErrCode = poPlayer->OpenHeroSelectPanel( *pstAck );

	return CLIGS_OPEN_HERO_SELECT_PANEL_ACK;
}

UINT16 CGTPktBuilder::OnOpenJewlryGenerateAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK);
	
	CBag& oBag = poPlayer->GetBag();
	oBag.GetDT_JEWELRY_DATA_LST_CLI(pstAck->stJewelryInfo);
	UINT8 byResNum = 0;
	pstAck->stResInfo.astResList[byResNum].byResKind = ACTE_COIN;
	pstAck->stResInfo.astResList[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	byResNum++;
	pstAck->stResInfo.astResList[byResNum].byResKind = ACTE_GOLD;
	pstAck->stResInfo.astResList[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	byResNum++;
	pstAck->stResInfo.byResNum = byResNum;
	pstAck->wErrCode = ERR_OPEN_JEWELRY_GENERATE::ID_SUCCESS;
	return CLIGS_OPEN_JEWELRY_GENERATE_ACK;
}

UINT16 CGTPktBuilder::OnJewlryGenerateAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_GENERATE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_JEWELRY_GENERATE_ACK);

	pstAck->wErrCode = poPlayer->OnJewelryGenerate(stReq, *pstAck);

	return CLIGS_JEWELRY_GENERATE_ACK;
}

UINT16 CGTPktBuilder::OnJewlryStrengthAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_STRENGTH_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_JEWELRY_STRENGTH_ACK);
	CJewelry* poJewlry = poPlayer->GetJewelry(stReq.wJewelryID, stReq.wJewelryIdx);
	if ( NULL == poJewlry)
	{
		pstAck->wErrCode = ERR_JEWELRY_STRENGTH::ID_NOT_FIND_JEWELRY;
		return CLIGS_JEWELRY_STRENGTH_ACK;
	}
	pstAck->wErrCode = poJewlry->OnUpgrade(*pstAck);
	pstAck->stBuildPrompt.byBuildNum = 1;
	poPlayer->GetJewelryPrompt(pstAck->stBuildPrompt.astBuildPromptInfo[0]);
	return CLIGS_JEWELRY_STRENGTH_ACK;
}

UINT16 CGTPktBuilder::OnJewlryGenerateInfoAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_JEWELRY_GENERATE_INFO_ACK);
	
	pstAck->wErrCode = poPlayer->OnJewelryGenerateInfo(stReq, *pstAck);

	return CLIGS_JEWELRY_GENERATE_INFO_ACK;
}

UINT16 CGTPktBuilder::OnJewelryResolveAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_RESOLVE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}
	if ( VER_PET <= poPlayer->GetCliVer())
	{
		ZERO_PACKET(PKT_CLIGS_JEWELRY_RESOLVE_ACK2);

		pstAck->wErrCode = poPlayer->OnJewelryResolve(stReq, *pstAck);

		return CLIGS_JEWELRY_RESOLVE_ACK2;
	}
	else
	{
		ZERO_PACKET(PKT_CLIGS_JEWELRY_RESOLVE_ACK);

		pstAck->wErrCode = poPlayer->OnJewelryResolve(stReq, *pstAck);

		return CLIGS_JEWELRY_RESOLVE_ACK;
	}
	
}

UINT16 CGTPktBuilder::OnOpenJewelryStrengthAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK);

	pstAck->wErrCode = poPlayer->OpenJewelryStrength( *pstAck);

	return CLIGS_OPEN_JEWELRY_STRENGTH_ACK;
}

UINT16 CGTPktBuilder::OnJewelryCompareAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_COMPARE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_JEWELRY_COMPARE_ACK);

	pstAck->wErrCode = poPlayer->JewelryCompare( stReq, *pstAck);

	return CLIGS_JEWELRY_COMPARE_ACK;
}

UINT16 CGTPktBuilder::OnJewelryExtStrengAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK);
	CJewelry* poJewelry = poPlayer->GetJewelry(stReq.wJewelryID, stReq.wJewelryIdx);
	if ( NULL == poJewelry)
	{
		pstAck->wErrCode = ERR_JEWELRY_EXT_UPGRADE::ID_NOT_FIND_JEWELRY;
		return CLIGS_JEWELRY_EXT_UPGRADE_ACK;
	}
	pstAck->wErrCode = poJewelry->OnStrengExt( stReq.byExtID, *pstAck);
	pstAck->stBuildPrompt.byBuildNum = 1;
	poPlayer->GetJewelryPrompt(pstAck->stBuildPrompt.astBuildPromptInfo[0]);
	return CLIGS_JEWELRY_EXT_UPGRADE_ACK;
}

UINT16 CGTPktBuilder::OnOpenJewelryResolve(CPlayer* poPlayer, PKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK);
	CJewelry* poJewelry = poPlayer->GetJewelry(stReq.wJewelryID, stReq.wJewelryIdx);
	if ( NULL == poJewelry)
	{
		pstAck->wErrCode = ERR_OPEN_JEWELRY_RESOLVE::ID_NOT_FIND_JEWELRY;
		return CLIGS_OPEN_JEWELRY_RESOLVE_ACK;
	}
	pstAck->wErrCode = poJewelry->OnOpenJewelryResolve( *pstAck);

	return CLIGS_OPEN_JEWELRY_RESOLVE_ACK;
}

UINT16 CGTPktBuilder::ChallengeAck3( UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy )
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_CHALLENGE_ACK3);

    if(0 != wErrCode)
    {
        pstAck->wErrCode = wErrCode;
        return CLIGS_CHALLENGE_ACK3;
    }

    if(NULL == poEnemy)
    {
        pstAck->wErrCode = ERR_CHALLENGE::ID_ENEMY_NOT_EXIST_ERR;
        return CLIGS_CHALLENGE_ACK3;
    }

    if(poPlayer == poEnemy)
    {
        pstAck->wErrCode = ERR_CHALLENGE::ID_ENEMY_CANNOT_MYSELF_ERR;
        return CLIGS_CHALLENGE_ACK3;
    }

    if(FALSE == poEnemy->GetDT_ENEMY_HERO_DATA(pstAck->byEnemyHeroNum, pstAck->astEnemyInfoList))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_CHALLENGE_ACK3;
    }

    if(FALSE == CRaceMgr::Instance()->Challenge2(poPlayer, poEnemy, pstAck->stBattleInfo, pstAck->stChallengeEncourage))
    {
        SET_OTHER_ERR(pstAck->wErrCode);
        return CLIGS_CHALLENGE_ACK3;
    }

    //poPlayer->GetMainHeroUpgradeInfo(pstAck->stMainHeroUpgradeInfo);
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
    pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
    pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
    pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
    //pstAck->dwCurFame = stDT_PLAYER_BASE_DATA.dwFame;

    CRaceMgr::Instance()->GetChallengePlayerData(poEnemy->GetDT_PLAYER_BASE_DATA().dwID, pstAck->stEnemyPlayerData);
    //任务信息
    poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
    poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetSciencePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

    return CLIGS_CHALLENGE_ACK3;
}

UINT16	CGTPktBuilder::ChallengeAck4(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_CHALLENGE_ACK4);

	if(0 != wErrCode)
	{
		pstAck->wErrCode = wErrCode;
		return CLIGS_CHALLENGE_ACK4;
	}

	if(NULL == poEnemy)
	{
		pstAck->wErrCode = ERR_CHALLENGE::ID_ENEMY_NOT_EXIST_ERR;
		return CLIGS_CHALLENGE_ACK4;
	}

	if(poPlayer == poEnemy)
	{
		pstAck->wErrCode = ERR_CHALLENGE::ID_ENEMY_CANNOT_MYSELF_ERR;
		return CLIGS_CHALLENGE_ACK4;
	}

	if(FALSE == poEnemy->GetDT_ENEMY_HERO_DATA(pstAck->byEnemyHeroNum, pstAck->astEnemyInfoList))
	{
		SET_OTHER_ERR(pstAck->wErrCode);
		return CLIGS_CHALLENGE_ACK4;
	}

	if(FALSE == CRaceMgr::Instance()->Challenge3(poPlayer, poEnemy, pstAck->stBattleInfo, pstAck->stChallengeEncourage))
	{
		SET_OTHER_ERR(pstAck->wErrCode);
		return CLIGS_CHALLENGE_ACK4;
	}

	//poPlayer->GetMainHeroUpgradeInfo(pstAck->stMainHeroUpgradeInfo);
	const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
	pstAck->qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
	pstAck->qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
	pstAck->qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
	pstAck->qwCurJingJie = stDT_PLAYER_BASE_DATA.qwJingJie;
	//pstAck->dwCurFame = stDT_PLAYER_BASE_DATA.dwFame;

	CRaceMgr::Instance()->GetChallengePlayerData(poEnemy->GetDT_PLAYER_BASE_DATA().dwID, pstAck->stEnemyPlayerData);
	//任务信息
	poPlayer->GetDT_TASK_DATA_LIST_CLI(pstAck->stTaskInfoList, TRUE);
	poPlayer->GetDT_TASK_COND_RECORD_LIST(pstAck->stTaskRecordList);
	poPlayer->GetPVPPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poPlayer->GetGodweaponPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poPlayer->GetSciencePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
	poPlayer->GetJingJiePrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);

	return CLIGS_CHALLENGE_ACK4;
}

UINT16 CGTPktBuilder::OnOpenGVGFormationAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_GVG_FORMATION_ACK);
	
	pstAck->wErrCode = CGvGBuild::Instance()->OpenGVGFormation(*pstAck, poPlayer);
	return CLIGS_OPEN_GVG_FORMATION_ACK;
}

UINT16 CGTPktBuilder::OnOpenGVGActivityAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}
    
    if(!CGvGBuild::Instance()->IsOpenUp())
    {
        return OpenGVGNotOpenMsgAck(poPlayer);
    }

	GvGRound byActivityRound = CGvGBuild::Instance()->GetCurRound();
	switch (byActivityRound)
	{
    case GVG_SLEEPING:
    case GVG_APPLY://报名
        return OnGVGApplyAck(poPlayer);

    case GVG_MONEY_WAR://本服第一轮
        return OnGVGMoneyGameAck(poPlayer);

    case GVG_CHAOS_WAR://本服第二轮
        return OnGVGChaosGameAck(poPlayer);

    case GVG_16_TO_8://双服16进8
    case GVG_8_TO_4://双服8进4
        return OpenGVG16To8To4Ack(poPlayer);

    case GVG_FINAL_WAR: // 四服总决赛
        return OpenGVGFinalWarAck(poPlayer);

    case GVG_END:       // 本次跨服战已结束
        return OpenGVGChampionAck(poPlayer);
	}

	return CLIGS_GVG_APPLAY_ACK;
}

UINT16 CGTPktBuilder::OnGVGApplyAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_APPLAY_ACK);

	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	pstAck->wErrCode = CGvGBuild::Instance()->OpenGVGApplyPanel(*pstAck, poGvG);
	return CLIGS_GVG_APPLAY_ACK;
}

UINT16 CGTPktBuilder::OnGVGMoneyGameAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_MONEY_CHAOS_ACK);
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	pstAck->wErrCode = CGvGBuild::Instance()->OpenGvGMoneyWarAck(*pstAck, poGvG);
	return CLIGS_GVG_MONEY_CHAOS_ACK;
}

UINT16 CGTPktBuilder::OnGVGChaosGameAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_MONEY_CHAOS_ACK);
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	pstAck->wErrCode = CGvGBuild::Instance()->OpenGvGChaosWarAck(*pstAck, poGvG);
	return CLIGS_GVG_MONEY_CHAOS_ACK;
}

UINT16 CGTPktBuilder::OpenGVG16To8To4Ack(CPlayer* poPlayer)
{
    if ( NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_16_TO_8_TO_4_ACK);

    pstAck->wErrCode = CGvGBuild::Instance()->OpenGVG16To8To4Ack(poPlayer, *pstAck);
    return CLIGS_16_TO_8_TO_4_ACK;
}

UINT16 CGTPktBuilder::OpenGVGFinalWarAck(CPlayer* poPlayer)
{
    if ( NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_FINAL_WAR_ACK);

    pstAck->wErrCode = CGvGBuild::Instance()->OpenGVGFinalWarAck(poPlayer, *pstAck);
    return CLIGS_FINAL_WAR_ACK;
}

UINT16 CGTPktBuilder::OpenGVGChampionAck(CPlayer* poPlayer)
{
    if ( NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_GVG_CHAMPION_ACK);

    pstAck->wErrCode = CGvGBuild::Instance()->OpenGVGChampionAck(poPlayer, *pstAck);
    return CLIGS_GVG_CHAMPION_ACK;
}

UINT16 CGTPktBuilder::OpenGVGNotOpenMsgAck(CPlayer* poPlayer)
{
    if ( NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_GVG_ERR_MSG_ACK);

    CGvGBuild::Instance()->OpenGVGNotOpenMsgAck(poPlayer, *pstAck);
    return CLIGS_GVG_ERR_MSG_ACK;
}

UINT16 CGTPktBuilder::OnOpenAlreadApplayAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_ALREAD_APPLAY_ACK);
	GvGRound byActivityRound = CGvGBuild::Instance()->GetCurRound();
	if ( GVG_APPLY != byActivityRound)
	{
		pstAck->wErrCode = ERR_OPEN_ALREAD_APPLAY::ID_NOT_APPLAY_TIME;
		return CLIGS_OPEN_ALREAD_APPLAY_ACK;
	}
	pstAck->wErrCode = CGvGBuild::Instance()->GetApplayFaction(*pstAck);
	return CLIGS_OPEN_ALREAD_APPLAY_ACK;
}

UINT16 CGTPktBuilder::OnOpenGVGLastResultAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_LAST_RESULT_ACK);

    CGvGBuild::Instance()->GetLastResult(pstAck->stLastResult);
	pstAck->wErrCode = ERR_GVG_LAST_RESULT::ID_SUCCESS;

	return CLIGS_GVG_LAST_RESULT_ACK;
}

UINT16 CGTPktBuilder::OnGVGOpenMyGambleAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK);
	CPlayerGVG* poGVG = poPlayer->GetPlayerGVG();
	if ( NULL == poGVG)
	{
		pstAck->wErrCode = ERR_GVG_OPEN_MY_GAMBLE::ID_IS_UNOPEN;
		return CLIGS_GVG_OPEN_MY_GAMBLE_ACK;
	}
	pstAck->wErrCode = poGVG->OpenMyGamble( *pstAck );
	return CLIGS_GVG_OPEN_MY_GAMBLE_ACK;
}

UINT16 CGTPktBuilder::OnGVGOpenGambleAck(CPlayer* poPlayer, PKT_CLIGS_GVG_OPEN_GAMBLE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_OPEN_GAMBLE_ACK);
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	pstAck->wErrCode = CGvGBuild::Instance()->GetGambleInfoAck(stReq, *pstAck, poGvG);

	return CLIGS_GVG_OPEN_GAMBLE_ACK;
}

UINT16 CGTPktBuilder::OnGVGOpenRuleAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_GVG_RULE_ACK);

	pstAck->wErrCode = CGvGPropMgr::Instance()->GetGvGGameRule(pstAck->aszContent);

	return CLIGS_OPEN_GVG_RULE_ACK;
}

UINT16 CGTPktBuilder::OnGVGGambleAck(CPlayer* poPlayer, PKT_CLIGS_GVG_GAMBLE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_GAMBLE_ACK);

	CPlayerGVG* poGVG = poPlayer->GetPlayerGVG();
	if ( NULL == poGVG)
	{
		return ERR_GVG_GAMBLE::ID_IS_UNOPEN;
	}
	pstAck->wErrCode = poGVG->GambleFactionAck(stReq);
	
	return CLIGS_GVG_GAMBLE_ACK;
}

UINT16 CGTPktBuilder::OnOpenMyFactionGvGLogAck(CPlayer* poPlayer)
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_GVG_LOG_ACK);
    return CLIGS_OPEN_GVG_LOG_ACK;
}

UINT16 CGTPktBuilder::OnOpenGvGLogAck(CPlayer* poPlayer, UINT32 dwFactionID)
{
    if (NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_GVG_LOG_ACK);
    pstAck->wErrCode = CGvGBuild::Instance()->OpenGvGLog(*pstAck, *poPlayer, dwFactionID);
    return CLIGS_OPEN_GVG_LOG_ACK;
}

void ConvertVideo_v410_to_v400(const DT_EMPTY_HP_VIDEO &v410, DT_EMPTY_HP_VIDEO_v400 &v400){
    CBattleMgr::Instance()->Convert5To4(v410.stEmptyHPVideo, v400.stEmptyHPVideo);

    v400.stMyselfMaxHPInfo = v410.stMyselfMaxHPInfo;
    v400.stEnemyMaxHPInfo = v410.stEnemyMaxHPInfo;
    v400.stMyselfBeforeRecoverHPInfo = v410.stMyselfBeforeRecoverHPInfo;
    v400.stEnemyBeforeRecoverHPInfo = v410.stEnemyBeforeRecoverHPInfo;
}

UINT16 CGTPktBuilder::OnSeeGVGVideoAck(CPlayer &oPlayer, UINT64 qwVideoID, BYTE byVideoNum)
{
    if (oPlayer.GetCliVer() >= VERSION_FLY){
        ZERO_PACKET(PKT_CLIGS_SEE_GVG_VIDEO_ACK_v410);
        pstAck->wErrCode = CGvGBuild::Instance()->SeeGvGVideo(*pstAck, qwVideoID, byVideoNum);

        return CLIGS_SEE_GVG_VIDEO_ACK_v410;
    }else{
        PKT_CLIGS_SEE_GVG_VIDEO_ACK_v410 stAck_v410;
        memset(&stAck_v410, 0, sizeof(stAck_v410));
        stAck_v410.wErrCode = CGvGBuild::Instance()->SeeGvGVideo(stAck_v410, qwVideoID, byVideoNum);

        ZERO_PACKET(PKT_CLIGS_SEE_GVG_VIDEO_ACK_v400);
        pstAck->wErrCode = stAck_v410.wErrCode;

        pstAck->qwVideoID = stAck_v410.qwVideoID;
        pstAck->byVideoNum = stAck_v410.byVideoNum;
        pstAck->stG1 = stAck_v410.stG1;
        pstAck->stG2 = stAck_v410.stG2;
        pstAck->byHaveNextVideo = stAck_v410.byHaveNextVideo;
        pstAck->byEnegy1 = stAck_v410.byEnegy1;
        pstAck->byEnegy2 = stAck_v410.byEnegy2;
        pstAck->byIsWeak1 = stAck_v410.byIsWeak1;
        pstAck->byIsWeak2 = stAck_v410.byIsWeak2;

        ConvertVideo_v410_to_v400(stAck_v410.stVideo, pstAck->stVideo);        
        return CLIGS_SEE_GVG_VIDEO_ACK_v400;
    }

    return INVALID_MSGID;
}

UINT16 CGTPktBuilder::OnEndGvGVideoAck(CPlayer &oPlayer, UINT64 qwVideoID, BYTE byEndType)
{
    if (oPlayer.GetCliVer() >= VERSION_FLY){
        ZERO_PACKET(PKT_CLIGS_END_GVG_VIDEO_ACK_v410);
        pstAck->wErrCode = CGvGBuild::Instance()->EndGvGVideo(*pstAck, qwVideoID, byEndType);

        return CLIGS_END_GVG_VIDEO_ACK_v410;
    }else{
        PKT_CLIGS_END_GVG_VIDEO_ACK_v410 stAck_v410;
        memset(&stAck_v410, 0, sizeof(stAck_v410));
        stAck_v410.wErrCode = CGvGBuild::Instance()->EndGvGVideo(stAck_v410, qwVideoID, byEndType);

        ZERO_PACKET(PKT_CLIGS_END_GVG_VIDEO_ACK_v400);
        pstAck->wErrCode = stAck_v410.wErrCode;
        pstAck->qwVideoID = stAck_v410.qwVideoID;
        pstAck->stG1 = stAck_v410.stG1;
        pstAck->stG2 = stAck_v410.stG2;
        pstAck->stMVPList = stAck_v410.stMVPList;
        pstAck->byEndType = stAck_v410.byEndType;
        pstAck->stRound = stAck_v410.stRound;

        ConvertVideo_v410_to_v400(stAck_v410.stVideo, pstAck->stVideo);        
        return CLIGS_END_GVG_VIDEO_ACK_v400;
    }

    return INVALID_MSGID;
}

UINT16 CGTPktBuilder::OnShowGvGMVPAck(UINT64 qwVideoID)
{
    ZERO_PACKET(PKT_CLIGS_SHOW_GVG_MVP_ACK);
    pstAck->wErrCode = CGvGBuild::Instance()->ShowGvGMVP(*pstAck, qwVideoID);
    return CLIGS_SHOW_GVG_MVP_ACK;
}

UINT16 CGTPktBuilder::OnOpenGVGMoneyRankAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK);

	pstAck->wErrCode = CGvGBuild::Instance()->GetFactionMoneyRank(*pstAck);

	return CLIGS_OPEN_FACTION_MONEY_RANK_ACK;
}

UINT16 CGTPktBuilder::OnReadFormationPlayerTipAck(CPlayer* poPlayer)
{

	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK);
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	if ( NULL == poGvG)
	{
		pstAck->wErrCode = ERR_READ_FORMATION_PLAYER_TIP::ID_IS_UPOPEN;
		return CLIGS_READ_FORMATION_PLAYER_TIP_ACK;
	}

	pstAck->wErrCode = poGvG->GetGvGFormationPlayerTip(*pstAck);
	return CLIGS_READ_FORMATION_PLAYER_TIP_ACK;
}

UINT16 CGTPktBuilder::OnGvGFactionEmbattleAck(CPlayer* poPlayer, PKT_CLIGS_GVG_FACTION_EMBATTLE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_FACTION_EMBATTLE_ACK);
	
	CFaction* poFaction = poPlayer->GetFaction();
	if ( NULL == poFaction)
	{
		pstAck->wErrCode = ERR_GVG_FACTION_EMBATTLE::ID_NOT_HAVE_FACTION;
		return CLIGS_GVG_FACTION_EMBATTLE_ACK;
	}
	if( em_Faction_MartialHead != poFaction->GetJob(poPlayer->GetID()))
	{
		pstAck->wErrCode = ERR_GVG_FACTION_EMBATTLE::ID_NOT_HEAD;
		return CLIGS_GVG_FACTION_EMBATTLE_ACK;
	}
	if ( 0 == stReq.stFormation.byPlayerNum)
	{
		pstAck->wErrCode =  ERR_GVG_FACTION_EMBATTLE::ID_MORE_ONE;
		return CLIGS_GVG_FACTION_EMBATTLE_ACK;
	}
	pstAck->wErrCode = poFaction->GvGFactionEmbattle(stReq);
	return CLIGS_GVG_FACTION_EMBATTLE_ACK;
}

UINT16 CGTPktBuilder::OnOpenGvGiftAck(CPlayer* poPlayer)
{

	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_GVG_GIFT_ACK);

	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	if ( NULL == poGvG)
	{
		pstAck->wErrCode = ERR_OPEN_GVG_GIFT::ID_IS_UNOPEN;
		return CLIGS_OPEN_GVG_GIFT_ACK;
	}

	pstAck->wErrCode = poGvG->OpenMyGvGift(*pstAck);
	return CLIGS_OPEN_GVG_GIFT_ACK;
}

UINT16 CGTPktBuilder::OnGetGvGiftAck(CPlayer* poPlayer, PKT_CLIGS_GET_GVG_GIFT_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GET_GVG_GIFT_ACK);
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	if ( NULL == poGvG)
	{
		pstAck->wErrCode = ERR_GET_GVG_GIFT::ID_IS_UNOPEN;
		return CLIGS_GET_GVG_GIFT_ACK;
	}

	pstAck->wErrCode = poGvG->GetMyGift(stReq, *pstAck);
	return CLIGS_GET_GVG_GIFT_ACK;
}

UINT16 CGTPktBuilder::OnSelectGroupTypeAck(CPlayer* poPlayer, PKT_CLIGS_SELECT_GROUP_TYPE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_SELECT_GROUP_TYPE_ACK);
	CCourage* poCourage = poPlayer->GetCourage();
	if ( NULL == poCourage)
	{
		pstAck->wErrCode = ERR_SELECT_GROUP_TYPE::ID_IS_UNOPEN;
		return CLIGS_SELECT_GROUP_TYPE_ACK;
	}

	poCourage->SetSelectType(stReq.bySelect);
	poCourage->GetCourageGroup(pstAck->stGroupData);
	pstAck->bySelect = stReq.bySelect;
	pstAck->wErrCode = ERR_SELECT_GROUP_TYPE::ID_SUCCESS;
	return CLIGS_SELECT_GROUP_TYPE_ACK;
}

UINT16 CGTPktBuilder::OnGvGFactionApplayAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_GVG_FACTION_APPLAY_ACK);	

	pstAck->wErrCode = CGvGBuild::Instance()->FactionApplyAck(poPlayer);
	return CLIGS_GVG_FACTION_APPLAY_ACK;
}

UINT16 CGTPktBuilder::OnReadBattleLogAck(CPlayer* poPlayer, PKT_CLIGS_READ_BATTLE_LOG_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_READ_BATTLE_LOG_ACK);
	
	CPlayerLogMgr::Instance()->GetUnReadLogMsg(poPlayer->GetID(), stReq.byKindID, pstAck->stLogInfo);
	pstAck->byLogType = stReq.byKindID;
	pstAck->wErrCode = ERR_READ_BATTLE_LOG::ID_SUCCESS;

	return CLIGS_READ_BATTLE_LOG_ACK;
}

UINT16 CGTPktBuilder::OnOpenDeourSoulAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_DEVOUR_SOUL_REQ& stReq)
{
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_DEVOUR_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_OPEN_DEVOUR_SOUL::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->OpenDevourSoulAck(poPlayer, stReq, *pstAck);
	}
	
	return CLIGS_OPEN_DEVOUR_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnDeourSoulAck(CPlayer* poPlayer, PKT_CLIGS_DEVOUR_SOUL_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_DEVOUR_SOUL_ACK);

	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_DEVOUR_SOUL::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->DevourSoul(poPlayer, stReq, *pstAck);
		CHero* pHero = poPlayer->GetHero(stReq.stMainSoul.wHeroId);
		if (NULL != pHero) {
			pHero->GetHeroPanelInfo(pstAck->stHeroInfo);
		}
	}

	return CLIGS_DEVOUR_SOUL_ACK;
}
UINT16 CGTPktBuilder::OnOpenReflashSoulAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_REFLASH_SOUL_REQ& stReq) {
	if ( NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_REFLASH_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if ( NULL == poSoul)
	{
		pstAck->wErrCode = ERR_OPEN_REFLASH_SOUL::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->OpenReflash(poPlayer, stReq, *pstAck);
	}

	return CLIGS_OPEN_REFLASH_SOUL_ACK;
}
UINT16 CGTPktBuilder::OnReflashSoulAck(CPlayer* poPlayer, PKT_CLIGS_REFLASH_SOUL_REQ& stReq) {
	if ( NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_REFLASH_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if ( NULL == poSoul) {
		pstAck->wErrCode = ERR_REFLASH_SOUL::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->ReflashSoul(poPlayer, stReq, *pstAck);
	}
	return CLIGS_REFLASH_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnSaveReflashSoulAck(CPlayer* poPlayer, PKT_CLIGS_SAVE_REFLASH_SOUL_REQ& stReq) {
	if ( NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_SAVE_REFLASH_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if ( NULL == poSoul) {
		pstAck->wErrCode = ERR_SAVE_REFLASH_SOUL::ID_IS_UNOPEN;
		return CLIGS_REFLASH_SOUL_ACK;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->SaveReflashSoul(poPlayer, stReq, *pstAck);
		pstAck->stMainSoul = stReq.stMainSoul;
		CHero* pHero = poPlayer->GetHero(stReq.stMainSoul.wHeroId);
		if (NULL != pHero) {
			pHero->GetHeroPanelInfo(pstAck->stHeroInfo);
		}
	}
	return CLIGS_SAVE_REFLASH_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnSoulRepInfoAck(CPlayer* poPlayer, PKT_CLIGS_SOULREP_INFO_REQ& rMsgReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_SOULREP_INFO_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_SOULREP_INFO::ID_IS_UNOPEN;
	} else {
		poPlayer->GetLocale().OnEnterSoulRep();
		pstAck->wErrCode = CSoulManager::Instance()->GetSoulRepInfo(poPlayer, *pstAck);
	}

	return CLIGS_SOULREP_INFO_ACK;
}

UINT16 CGTPktBuilder::OnProductSoulAck(CPlayer* poPlayer, PKT_CLIGS_PRODUCT_SOUL_REQ& rMsgReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_PRODUCT_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_PRODUCT_SOUL::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->ProductSoul(poPlayer, rMsgReq, *pstAck);
	}

	return CLIGS_PRODUCT_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnEquipSoulAck(CPlayer* poPlayer, PKT_CLIGS_HERO_EQUIP_SOUL_REQ& rMsgReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_HERO_EQUIP_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_HERO_EQUIP_SOUL::ID_IS_UNOPEN;
	} else {
		CHero* pHero = poPlayer->GetHero(rMsgReq.wHeroId);
		if (NULL != pHero) {
			pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
			pstAck->wErrCode = CSoulManager::Instance()->EquipSoul(poPlayer, rMsgReq.wHeroId, rMsgReq.wSoulID, rMsgReq.dwSoulIdx);
			pstAck->wSoulID = rMsgReq.wSoulID;
			pstAck->dwSoulIdx = rMsgReq.dwSoulIdx;
			pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
			pHero->GetHeroPanelInfo(pstAck->stHeroInfo);
		} else {
			pstAck->wErrCode = ERR_HERO_EQUIP_SOUL::ID_NO_HERO;
		}
		// FillSoulHeroInfo(poPlayer, rMsgReq.wHeroId, pstAck->stHeroSoulInfo);
	}
	return CLIGS_HERO_EQUIP_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnUnDressSoulAck(CPlayer* poPlayer, PKT_CLIGS_HERO_UNDRESS_SOUL_REQ& rMsgReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_HERO_UNDRESS_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_HERO_UNDRESS_SOUL::ID_IS_UNOPEN;
	} else {
		CHero* pHero = poPlayer->GetHero(rMsgReq.wHeroId);
		if (NULL != pHero) {
			pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
			pstAck->wErrCode = CSoulManager::Instance()->UnDressSoul(poPlayer, rMsgReq.wHeroId, rMsgReq.wSoulID, rMsgReq.dwSoulIdx);
			pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
			pstAck->wSoulID = rMsgReq.wSoulID;
			pstAck->dwSoulIdx = rMsgReq.dwSoulIdx;
			pHero->GetHeroPanelInfo(pstAck->stHeroInfo);
		} else {
			pstAck->wErrCode = ERR_HERO_UNDRESS_SOUL::ID_NO_HERO;
		}
	}
	return CLIGS_HERO_UNDRESS_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnReplaceSoulAck(CPlayer* poPlayer, PKT_CLIGS_HERO_REPLACE_SOUL_REQ& rMsgReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_HERO_REPLACE_SOUL_ACK);
	pstAck->stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	pstAck->wErrCode = CSoulManager::Instance()->OnReplaceSoul(poPlayer, rMsgReq, *pstAck);
	pstAck->stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	pstAck->dwBagSoulIdx = rMsgReq.dwBagSoulIdx;
	pstAck->wBagSoulID = rMsgReq.wBagSoulID;
	pstAck->dwHeroSoulIdx = rMsgReq.dwHeroSoulIdx;
	pstAck->wHeroSoulID = rMsgReq.wHeroSoulID;
	return CLIGS_HERO_REPLACE_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnSoulExchangeInfoReq(CPlayer* poPlayer, PKT_CLIGS_SOULEXCHANGE_INFO_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_SOULEXCHANGE_INFO_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_SOULEXCHANGE_INFO::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->SoulExchangeInfo(poPlayer, *pstAck);
	}
	return CLIGS_SOULEXCHANGE_INFO_ACK;
}

UINT16 CGTPktBuilder::OnExchangeSoulReq(CPlayer* poPlayer, PKT_CLIGS_EXCHANGE_SOUL_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_EXCHANGE_SOUL_ACK);
	CPlayerSoul* poSoul = poPlayer->GetPlayerSoul();
	if (NULL == poSoul) {
		pstAck->wErrCode = ERR_EXCHANGE_SOUL::ID_IS_UNOPEN;
	} else {
		pstAck->wErrCode = CSoulManager::Instance()->ExchangeSoul(poPlayer, stReq, *pstAck);
	}
	return CLIGS_EXCHANGE_SOUL_ACK;
}

UINT16 CGTPktBuilder::OnOpenMyMedalAck(CPlayer* poPlayer)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_MY_MEDAL_ACK);

	CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();

	if (NULL == poPlayerMedal) 
	{
		pstAck->wErrCode = ERR_OPEN_MY_MEDAL::ID_IS_UNOPEN;
		return CLIGS_OPEN_MY_MEDAL_ACK;
	} 

	pstAck->wErrCode = poPlayerMedal->OnOpenMyMedal(*pstAck);
	
	return CLIGS_OPEN_MY_MEDAL_ACK;
}
UINT16 CGTPktBuilder::OnViewFactionMembersAck(UINT32 dwFactionID)
{
    ZERO_PACKET(PKT_CLIGS_VIEW_FACTION_MEMBERS_ACK);
    if (0 == dwFactionID)
    {
        pstAck->wErrCode = ERR_VIEW_FACTION_MEMBER::ID_NOT_FOUND_FACTION;
        return CLIGS_VIEW_FACTION_MEMBERS_ACK;
    }

    CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
    if (poFaction)
    {
        poFaction->GetFactionMemberInfoList(pstAck->stMemberList);
        pstAck->byIsLocalFaction = TRUE;
    }
    else
    {
        DT_GVG_FACTION *pstFaction = CGvGFactionMgr::Instance()->FindFactionInfo(dwFactionID);
        if (pstFaction)
        {
            pstAck->stMemberList = pstFaction->stMembers;
            pstAck->byIsLocalFaction = FALSE;
        }
        else
        {
            pstAck->wErrCode = ERR_VIEW_FACTION_MEMBER::ID_NOT_FOUND_FACTION;
            return CLIGS_VIEW_FACTION_MEMBERS_ACK;
        }
    }

    pstAck->wErrCode = ERR_VIEW_FACTION_MEMBER::ID_SUCCESS;
    return CLIGS_VIEW_FACTION_MEMBERS_ACK;
}

UINT16 CGTPktBuilder::OnOpenMapAck(CPlayer* poPlayer)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_OUT_BOUND_ACK);
	
	if ( !poPlayer->IsFly())
	{
		pstAck->wErrCode = ERR_OPEN_OUT_BOUND::ID_NOT_FRESH;
	}
	else
	{
		pstAck->wErrCode = poPlayer->GetInstance().GetOutBoundInfo(*pstAck);
	}
	
	
	return CLIGS_OPEN_OUT_BOUND_ACK;
}

UINT16 CGTPktBuilder::OnOpenOutBoundMapAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK);
	CInstance& oInstance = poPlayer->GetInstance();
	pstAck->wErrCode = oInstance.OnOpenOutBoundMapAck(stReq, *pstAck);
	
	return CLIGS_OPEN_OUT_BOUND_MAP_ACK;
}

UINT16 CGTPktBuilder::OnOutBoundBattleAck(CPlayer* poPlayer, PKT_CLIGS_OUT_BOUND_BATTLE_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OUT_BOUND_BATTLE_ACK);
	CInstance& oInstance = poPlayer->GetInstance();
	pstAck->wErrCode = oInstance.OnOutBoundBattleAck(stReq, *pstAck);

	return CLIGS_OUT_BOUND_BATTLE_ACK;
}

UINT16 CGTPktBuilder::OnTestAck(CPlayer* poPlayer, PKT_CLIGS_TEST_REQ& stReq)
{
	if ( NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_TEST_ACK);
	
	SDStrncpy( pstAck->aszWenzi, CMsgDefMgr::Instance()->GetErrMsg("TEST_XNQ").c_str(), 1024 - 1);
	return CLIGS_TEST_ACK;
}

UINT16 CGTPktBuilder::OnOpenPetRepAck(CPlayer* poPlayer, PKT_CLIGS_PETREP_INFO_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_PETREP_INFO_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenPetRep(poPlayer, stReq, *pstAck);
	return CLIGS_PETREP_INFO_ACK;
}

UINT16 CGTPktBuilder::OnProductPetAck(CPlayer* poPlayer, PKT_CLIGS_PRODUCT_PET_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_PRODUCT_PET_ACK);
	pstAck->wErrCode = CPetMng::Instance()->ProductPet(poPlayer, stReq, *pstAck);
	return CLIGS_PRODUCT_PET_ACK;
}

UINT16 CGTPktBuilder::OnOpenRefinePetAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_REFINEPET_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_REFINEPET_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenRefinePetPanel(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_REFINEPET_ACK;
}

UINT16 CGTPktBuilder::OnRefinePetAck(CPlayer* poPlayer, PKT_CLIGS_REFINEPET_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_REFINEPET_ACK);
	pstAck->wErrCode = CPetMng::Instance()->RefinePet(poPlayer, stReq, *pstAck);
	return CLIGS_REFINEPET_ACK;
}

UINT16 CGTPktBuilder::OnOpenPetPanel(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_PANEL_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_PET_PANEL_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenPetInformationPanel(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_PET_PANEL_ACK;
}

UINT16 CGTPktBuilder::OnOpenStarPanelAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_STAR_PANEL_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_STAR_PANEL_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenStarPanel(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_STAR_PANEL_ACK;
}

UINT16 CGTPktBuilder::OnUpPetStarAck(CPlayer* poPlayer, PKT_CLIGS_UP_STAR_LV_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_UP_STAR_LV_ACK);
	pstAck->wErrCode = CPetMng::Instance()->UpPetStar(poPlayer, stReq, *pstAck);
	return CLIGS_UP_STAR_LV_ACK;
}

UINT16 CGTPktBuilder::OnOpenPetFeedAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_PETFEED_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_OPEN_PETFEED_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenPetFeedFanel(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_PETFEED_ACK;
}

UINT16 CGTPktBuilder::OnFirstDrawAck(CPlayer* poPlayer, PKT_CLIGS_FIRST_DRAW_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_FIRST_DRAW_ACK);
	pstAck->wErrCode = CPetMng::Instance()->FirstDraw(poPlayer, stReq, *pstAck);
	return CLIGS_FIRST_DRAW_ACK;
}

UINT16 CGTPktBuilder::OnResetDrawAck(CPlayer* poPlayer, PKT_CLIGS_RESET_DRAW_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_RESET_DRAW_ACK);
	pstAck->wErrCode = CPetMng::Instance()->ResetDraw(poPlayer, stReq, *pstAck);
	return CLIGS_RESET_DRAW_ACK;
}

UINT16 CGTPktBuilder::OnConfirmFeedAck(CPlayer* poPlayer, PKT_CLIGS_CONFIRM_FEED_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_CONFIRM_FEED_ACK);
	pstAck->wErrCode = CPetMng::Instance()->ConfirmFeed(poPlayer, stReq, *pstAck);
	return CLIGS_CONFIRM_FEED_ACK;
}

UINT16 CGTPktBuilder::OnPetMerge(CPlayer* poPlayer, PKT_CLIGS_MERGE_PET_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_CLIGS_MERGE_PET_ACK);
	pstAck->wErrCode = CPetMng::Instance()->PetMerge(poPlayer, stReq, *pstAck);
	return CLIGS_MERGE_PET_ACK;
}

UINT16 CGTPktBuilder::OnOpenPetStoreAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_STORE_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_PET_STORE_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OnOpenPetStore(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_PET_STORE_ACK;
}

UINT16 CGTPktBuilder::OnBuyPetStoreItemAck(CPlayer* poPlayer, PKT_CLIGS_BUY_PET_STORE_ITEM_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_BUY_PET_STORE_ITEM_ACK);
	pstAck->wErrCode = CPetMng::Instance()->BuyStoreItem(poPlayer, stReq, *pstAck);
	return CLIGS_BUY_PET_STORE_ITEM_ACK;
}

UINT16 CGTPktBuilder::OnOpenWashQualityAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_WASH_QUALITY_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_WASH_QUALITY_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenWashQuality(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_WASH_QUALITY_ACK;
}

UINT16 CGTPktBuilder::OnWashQualityAck(CPlayer* poPlayer, PKT_CLIGS_WASH_QUALITY_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_WASH_QUALITY_ACK);
	pstAck->wErrCode = CPetMng::Instance()->WashQuality(poPlayer, stReq, *pstAck);
	return CLIGS_WASH_QUALITY_ACK;
}

UINT16 CGTPktBuilder::OnChangePetStateAck(CPlayer* poPlayer, PKT_CLIGS_CHANGE_PET_STATE_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_CHANGE_PET_STATE_ACK);
	pstAck->wErrCode = CPetMng::Instance()->ChangePetState(poPlayer, stReq, *pstAck);
	return CLIGS_CHANGE_PET_STATE_ACK;
}

UINT16 CGTPktBuilder::OnOpenInheritPetAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_INHERIT_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_PET_INHERIT_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenPetInherit(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_PET_INHERIT_ACK;
}

UINT16 CGTPktBuilder::OnPetInheritAck(CPlayer* poPlayer, PKT_CLIGS_PET_INHERIT_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_PET_INHERIT_ACK);
	pstAck->wErrCode = CPetMng::Instance()->InheritPet(poPlayer, stReq, *pstAck);
	return CLIGS_PET_INHERIT_ACK;
}

// 根据装备系列的等级由高到低进行排序
bool SortEquipSeriesByLevelDesc(const DT_EQUIP_SERIES &stSeries1, const DT_EQUIP_SERIES &stSeries2)
{
    return stSeries1.wSeriesLevel > stSeries2.wSeriesLevel;;
}

// 打开装备制作面板
UINT16 CGTPktBuilder::OnOpenEquipMakeAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_EQUIP_MAKE_REQ& stReq)
{
    if (NULL == poPlayer) {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_EQUIP_MAKE_ACK);

    CBuild *poBuild = poPlayer->GetBuild(EBK_EQUIP_MAKE);
    if (NULL == poBuild){
        pstAck->wErrCode = ERR_OPEN_EQUIP_MAKE::ID_IS_UNOPEN;
        return CLIGS_OPEN_EQUIP_MAKE_ACK;
    }

    poBuild->OnOpen();

    CBag &oBag = poPlayer->GetBag();
    CID2GoodsShmemMap& mapGoods  = oBag.GetGoodsMap();

    typedef std::vector<CGoods*> GoodsVec;
    typedef std::map<SeriesID, DT_EQUIP_SERIES> EquipSeriesMap;

    // 取出制作卷列表
    GoodsVec vecBook;
    for(CGoods* poGoods = mapGoods.GetFistData(); poGoods; poGoods = mapGoods.GetNextData()){
        if (EGMK_EQUIP_MAKE_BOOK == poGoods->GetGoodsMainKindID()){
            vecBook.push_back(poGoods);
        }
    }

    // 依次获得每个制作卷的制作信息
    EquipSeriesMap mapSeries;
    for(UINT16 i = 0; i < vecBook.size(); i++){
        CGoods* poBook = vecBook[i];
        const SEquipMakeProp* pstProp = CEquipMakePropMgr::Instance()->GetMakeProp(poBook->GetItemKindID());
        if (NULL == pstProp){
            continue;
        }

        DT_EQUIP_MAKE_BOOK stBookCli;
        CEquipMakeMgr::TakeEquipMakeBookInfo(*poPlayer, poBook->GetItemKindID(), stBookCli);
        
        DT_EQUIP_SERIES &stSeries = mapSeries[pstProp->wSeriesID];
        if (stBookCli.byIsMakable){
            stSeries.byMakeCnt++;
        }

        stSeries.astMakeBookList[stSeries.wMakeBookCnt++] = stBookCli;
    }

    // 将拥有的各装备系列的信息存入消息包
    for (EquipSeriesMap::iterator itr = mapSeries.begin(); itr != mapSeries.end(); ++itr){
        SeriesID seriesID = itr->first;
        DT_EQUIP_SERIES &stSeries = itr->second;

        SSeriesProp *pstSeriesProp = CEquipMakePropMgr::Instance()->GetSeriesProp(seriesID);
        if (NULL == pstSeriesProp){
            continue;
        }

        stSeries.wSeriesID = seriesID;
        stSeries.wSeriesLevel = pstSeriesProp->wLevel;
        SDStrncpy(stSeries.aszSeriesName, pstSeriesProp->m_strSeriesName.c_str(), MAX_EQUIP_SERIES_NAME_LEN - 1);
        pstAck->astSeriesList[pstAck->bySeriesCnt++] = stSeries;
    }

    // 将装备系列按等级由高到低进行排序
    std::sort(pstAck->astSeriesList, pstAck->astSeriesList + pstAck->bySeriesCnt, SortEquipSeriesByLevelDesc);

    const SFlyProp &stFlyProp = CFlyPropMgr::Instance()->GetFlyProp();
    pstAck->byBuyMaterialVipLvl = stFlyProp.byEquipMakeBuyMaterialVipLvl;
    pstAck->wErrCode = ERR_OPEN_EQUIP_MAKE::ID_SUCCESS;
    return CLIGS_OPEN_EQUIP_MAKE_ACK;
}

// 装备制作
UINT16 CGTPktBuilder::OnEquipMakeAck(CPlayer* poPlayer, PKT_CLIGS_EQUIP_MAKE_REQ& stReq){
    if(NULL == poPlayer){
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_EQUIP_MAKE_ACK);

    // 制作装备
    pstAck->wErrCode = CItemMgr::Instance()->MakeEquip(poPlayer, stReq.wEquipBookID, stReq.wCurBookNum, stReq.byIsBuyMaterial);
    if (pstAck->wErrCode != ERR_EQUIP_MAKE::ID_SUCCESS){
        return CLIGS_EQUIP_MAKE_ACK;
    }

    CEquipMakeMgr::TakeEquipMakeBookInfo(*poPlayer, stReq.wEquipBookID, pstAck->stEquipBook);

    pstAck->qwCoin = poPlayer->GetCoin();
    pstAck->dwGold = (UINT32)poPlayer->GetGold();

    const SEquipMakeProp* pstCurBookProp = CEquipMakePropMgr::Instance()->GetMakeProp(stReq.wEquipBookID);
    if (NULL == pstCurBookProp){
        pstAck->wErrCode = ERR_EQUIP_MAKE::ID_BOOK_NOT_EXIST;
        return CLIGS_EQUIP_MAKE_ACK;
    }

    // 取出本系列制作卷新的补齐材料消耗元宝
    CBag &oBag = poPlayer->GetBag();
    CID2GoodsShmemMap& mapGoods  = oBag.GetGoodsMap();

    typedef std::vector<CGoods*> GoodsVec;
    GoodsVec vecBook;
    for(CGoods* poGoods = mapGoods.GetFistData(); poGoods; poGoods = mapGoods.GetNextData()){
        if (EGMK_EQUIP_MAKE_BOOK == poGoods->GetGoodsMainKindID()){
            vecBook.push_back(poGoods);
        }
    }

    for(UINT16 i = 0; i < vecBook.size(); i++){
        CGoods* poBook = vecBook[i];
        const SEquipMakeProp* pstBookProp = CEquipMakePropMgr::Instance()->GetMakeProp(poBook->GetItemKindID());
        if (NULL == pstBookProp){
            continue;
        }

        if (pstBookProp->wSeriesID != pstCurBookProp->wSeriesID){
            continue;
        }

        DT_EQUIP_MAKE_BOOK stBookCli;
        CEquipMakeMgr::TakeEquipMakeBookInfo(*poPlayer, poBook->GetItemKindID(), stBookCli);

        if (0 == stBookCli.dwBuyGold){
            continue;
        }

        DT_EQUIP_MAKE_BOOK_INFO_CLI &stBook = pstAck->astUpdateMakeBookList[pstAck->wUpdateMakeBookCnt++];
        stBook.dwBuyGold = stBookCli.dwBuyGold;
        stBook.wBookID = stBookCli.wBookID;
    }

    poPlayer->GetSmithyPrompt(pstAck->stPromptInfo.astBuildPromptInfo[pstAck->stPromptInfo.byBuildNum++]);
    return CLIGS_EQUIP_MAKE_ACK;
}

UINT16 CGTPktBuilder::OnUpgradeHeroAck(CPlayer* poPlayer, PKT_CLIGS_UPGRADE_HERO_REQ& stReq)
{
    if(NULL == poPlayer){
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_UPGRADE_HERO_ACK);
    pstAck->wErrCode = poPlayer->UpgradeHero(stReq.wHeroID, *pstAck);
    return CLIGS_UPGRADE_HERO_ACK;
}

UINT16 CGTPktBuilder::OnOpenFlyAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_FLY_REQ& stReq)
{
    if(NULL == poPlayer){
        return CLIGS_OPEN_FLY_ACK;
    }

    ZERO_PACKET(PKT_CLIGS_OPEN_FLY_ACK);

    if (NULL == poPlayer->GetBuild(EBK_FLY)){
        pstAck->wErrCode = ERR_OPEN_FLY::ID_IS_UNOPEN;
        return CLIGS_OPEN_FLY_ACK;
    }

    CHero *poHero = poPlayer->GetHero(stReq.wHeroID);
    if (NULL == poHero){
        pstAck->wErrCode = ERR_OPEN_FLY::ID_IS_UNOPEN;
        return CLIGS_OPEN_FLY_ACK;
    }

    SHeroProp *pstHeroProp = poHero->GetHeroProp();
    if (NULL == pstHeroProp){
        pstAck->wErrCode = ERR_OPEN_FLY::ID_IS_UNOPEN;
        return CLIGS_OPEN_FLY_ACK;
    }

    const SFlyProp &stFlyProp = CFlyPropMgr::Instance()->GetFlyProp();

    DT_FLY_GOAL &stGoal1 = pstAck->stFlyGoals.astFlyFoalList[pstAck->stFlyGoals.byFlyGoalCnt++];
    stGoal1.byIsAchieveFlyGoal = TRUE;

    std::vector<string> vecFillMsg;
    CHAR szBuff[64] = {0};
    sprintf(szBuff, "%u", FLY_LEVEL);
    vecFillMsg.push_back(szBuff);
    sprintf(szBuff, "%u", stFlyProp.wFlyRequiredJingjieLevel);
    vecFillMsg.push_back(szBuff);

    // 飞升条件1: 等级{x}级，境界{y}级
    std::string strGoal1 = CMsgDefMgr::Instance()->GetErrMsg("FLY_GOAL_1", &vecFillMsg);
    SDStrncpy(stGoal1.aszFlyGoalText, strGoal1.c_str(), strGoal1.size());

    if(NULL == poPlayer->GetJingJie()){
        stGoal1.byIsAchieveFlyGoal = FALSE;
    }else{
        if(poPlayer->GetJingJie()->GetJingJieLevel() < stFlyProp.wFlyRequiredJingjieLevel){
            stGoal1.byIsAchieveFlyGoal = FALSE;
        }
    }
    
    if(poHero->GetLevel() < FLY_LEVEL){
        stGoal1.byIsAchieveFlyGoal = FALSE;
    }

    if (!poHero->IsCoachHero()){
        // 飞升条件2: 主角已飞升
        DT_FLY_GOAL &stGoal2 = pstAck->stFlyGoals.astFlyFoalList[pstAck->stFlyGoals.byFlyGoalCnt++];
        std::string strGoal2 = CMsgDefMgr::Instance()->GetErrMsg("FLY_GOAL_2");
        SDStrncpy(stGoal2.aszFlyGoalText, strGoal2.c_str(), strGoal2.size());

        stGoal2.byIsAchieveFlyGoal = poPlayer->IsFly();
    }

    // 获取飞升消耗材料信息
    DT_MATERIAL_LIST stFlyCostMaterials;
    CEquipMakeMgr::TakeMaterialListCli(*poPlayer, pstHeroProp->vecFlyMaterial, stFlyCostMaterials);

    for(UINT16 i = 0; i < stFlyCostMaterials.byMaterialCnt; i++){
        DT_MATERIAL &stMaterial = stFlyCostMaterials.astMaterialList[i];
        DT_FLY_COST &stFlyCost = pstAck->astFlyCostList[i];
        stFlyCost.wItemID      = stMaterial.wMaterialID;
        stFlyCost.wCurNum      = stMaterial.wCurNum;
        stFlyCost.wRequiredNum = stMaterial.wRequiredNum;
    }

    // 传入基础属性提升百分比，用于客户端显示：飞升后基础属性将提升xx%，
    pstAck->byBasicAttrPromote = stFlyProp.byFlyAttrPromote;
    pstAck->wErrCode = ERR_OPEN_FLY::ID_SUCCESS;
    return CLIGS_OPEN_FLY_ACK;
}

// 飞升
UINT16 CGTPktBuilder::OnFlyAck(CPlayer* poPlayer, PKT_CLIGS_FLY_REQ& stReq)
{
    if(NULL == poPlayer)
    {
        return INVALID_MSGID;
    }

    ZERO_PACKET(PKT_CLIGS_FLY_ACK);
    pstAck->wErrCode = poPlayer->Fly(stReq.dwHeroID, *pstAck);
    return CLIGS_FLY_ACK;
}

UINT16 CGTPktBuilder::OnOpenPlayerPetPanel(CPlayer* poPlayer, PKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK);
	pstAck->wErrCode = CPetMng::Instance()->OpenPlayerPetPanel(poPlayer, stReq, *pstAck);
	return CLIGS_OPEN_PLAYERPET_PANEL_ACK;
}

UINT16 CGTPktBuilder::OnRequestAllPetInfo(CPlayer* poPlayer, PKT_CLIGS_REQUEST_ALL_PET_INFO_REQ& stReq) {
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_REQUEST_ALL_PET_INFO_ACK);
	pstAck->wErrCode = CPetMng::Instance()->RequestAllPetInfo(poPlayer, stReq, *pstAck);
	return CLIGS_REQUEST_ALL_PET_INFO_ACK;
}

UINT16 CGTPktBuilder::OnOpenStoreAck(CPlayer* poPlayer)
{
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_STORE_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_FLY);
	if ( NULL == poBuild)
	{
		pstAck->wErrCode = ERR_OPEN_STORE::ID_IS_NOT_OPEN;
	}
	else
	{
		pstAck->wErrCode = CExchangePropMgr::Instance()->OpenExchagneStroe(poPlayer, pstAck->stResInfo, pstAck->stItemInfo);
	}
	return CLIGS_OPEN_STORE_ACK;
}

UINT16 CGTPktBuilder::OnExchangeGoodsAck(CPlayer* poPlayer, PKT_CLIGS_EXCHANGE_GOODS_REQ& stReq)
{
	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_EXCHANGE_GOODS_ACK);
	CBuild* poBuild = poPlayer->GetBuild(EBK_FLY);
	if ( NULL == poBuild)
	{
		pstAck->wErrCode = ERR_EXCHANGE_GOODS::ID_IS_NOT_OPEN;
	}
	else
	{
		pstAck->wErrCode = CExchangePropMgr::Instance()->ExchangeItem(poPlayer, stReq, *pstAck);
	}

	return CLIGS_EXCHANGE_GOODS_ACK;
}

UINT16 CGTPktBuilder::OnOpenPetRuleAck(CPlayer* poPlayer)
{

	if (NULL == poPlayer) {
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_PET_RULE_ACK);
	
	pstAck->wErrCode = CPetMng::Instance()->OnOpenPetRule(poPlayer, *pstAck);

	return CLIGS_OPEN_PET_RULE_ACK;
}

UINT16 CGTPktBuilder::OnOpenSoulUpdateTypeAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ& stReq)
{

	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK);

	pstAck->wErrCode = CSoulManager::Instance()->OpenSoulPotencyInfo(poPlayer, stReq, *pstAck);

	return CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK;
}

UINT16 CGTPktBuilder::OnSoulUpdateTypeAck(CPlayer* poPlayer, PKT_CLIGS_SOUL_UPDATE_TYPE_REQ& stReq)
{

	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_SOUL_UPDATE_TYPE_ACK);

	pstAck->wErrCode = CSoulManager::Instance()->SoulPotencyInfo(poPlayer, stReq, *pstAck);

	return CLIGS_SOUL_UPDATE_TYPE_ACK;
}

UINT16 CGTPktBuilder::OnOpenNeiDanAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_NEIDAN_REQ& stReq)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_NEIDAN_ACK);


	pstAck->wErrCode = CPetMng::Instance()->OnOpenNeiDan(poPlayer, stReq, *pstAck);

	return CLIGS_OPEN_NEIDAN_ACK;
}

UINT16 CGTPktBuilder::OnDressNeiDanAck(CPlayer* poPlayer, PKT_CLIGS_DRESS_NEIDAN_REQ& stReq)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_DRESS_NEIDAN_ACK);

	pstAck->wErrCode = CPetMng::Instance()->OnDressNeiDan(poPlayer, stReq, *pstAck);

	return CLIGS_DRESS_NEIDAN_ACK;
}

UINT16 CGTPktBuilder::OnUpdataNeiDanAck(CPlayer* poPlayer, PKT_CLIGS_UPDATA_NEIDAN_REQ& stReq)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_UPDATA_NEIDAN_ACK);

	pstAck->wErrCode = CPetMng::Instance()->OnUpdataNeiDan(poPlayer, stReq, *pstAck);

	return CLIGS_UPDATA_NEIDAN_ACK;
}

UINT16 CGTPktBuilder::OnOpenNeiDanInstanceAck(CPlayer* poPlayer)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK);

	pstAck->wErrCode = CInstanceMgr::Instance()->OpenNeiDanInstance(poPlayer, *pstAck);

	return CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK;
}

UINT16 CGTPktBuilder::OnNeiDanBattleAck(CPlayer* poPlayer, PKT_CLIGS_NEIDAN_BATTLE_REQ& stReq)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_NEIDAN_BATTLE_ACK);

	pstAck->wErrCode = CInstanceMgr::Instance()->NeiDanBattle(poPlayer, stReq, *pstAck);

	return CLIGS_NEIDAN_BATTLE_ACK;
}

UINT16 CGTPktBuilder::OnRecvNeiDanAwardAck(CPlayer* poPlayer, PKT_CLIGS_RECV_NEIDAN_AWARD_REQ& stReq)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_RECV_NEIDAN_AWARD_ACK);

	pstAck->wErrCode = CInstanceMgr::Instance()->RecvNeiDanAward(poPlayer, *pstAck);

	return CLIGS_RECV_NEIDAN_AWARD_ACK;
}

UINT16 CGTPktBuilder::OnSkipNeiDanBattleAck(CPlayer* poPlayer, PKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ& stReq)
{
	if (NULL == poPlayer) 
	{
		return INVALID_MSGID;
	}
	ZERO_PACKET(PKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK);

	pstAck->wErrCode = CInstanceMgr::Instance()->SkipNeiDanBattle(poPlayer, stReq, *pstAck);

	return CLIGS_SKIP_NEIDAN_BATTLE_ACK;
}