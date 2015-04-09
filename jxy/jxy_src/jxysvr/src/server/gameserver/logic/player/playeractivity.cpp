#include "player.h"
#include <logic/activity/bossbattlemgr.h>

#include "logic/lua/luamgr.h"
#include <logic/other/singleprammgr.h>
#include <framework/gsconfig.h>
#include <logic/activity/couragemgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/activity/luaactivity.h>
#include "logic/activity/eatbread.h"
#include "logic/gvg/local/gvgbuild.h"

UINT16 CPlayer::ActivityNotifyReq(UINT8 byActivityType, UINT8 byNotify)
{
    UINT16  wErrCode = ERR_ACTIVITY_NOTIFY::ID_SUCCESS;
    switch (byActivityType)
    {
    case EAK_EATBREAD:
    {
        if(m_poEatBread)
        {
            m_poEatBread->GetDT_EAT_BREAD_DATA().byNotity = byNotify;
            CPlayerBaseDataCacheMgr::Instance()->SetEatBreadNotify(GetID(), byNotify);
        }
        else
        {
            wErrCode = ERR_ACTIVITY_NOTIFY::ID_ACTIVITY_NOT_OPEN;
        }
    }
    break;
    case EAK_BOSSBATTLE:
    {
        if(m_poBossBattle)
        {
            m_poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().byNotity = byNotify;
            CPlayerBaseDataCacheMgr::Instance()->SetBossBattleNotify(GetID(), byNotify);
        }
        else
        {
            wErrCode = ERR_ACTIVITY_NOTIFY::ID_ACTIVITY_NOT_OPEN;
        }
    }
    break;
    case EAK_COURAGE:
    {
        if ((m_poCourage) && (GetCliVer() >= 310))
        {

            m_poCourage->GetDT_COURAGE_DATA().byNotity = byNotify;
            CPlayerBaseDataCacheMgr::Instance()->SetBossBattleNotify(GetID(), byNotify);
        }
        else
        {
            wErrCode = ERR_ACTIVITY_NOTIFY::ID_ACTIVITY_NOT_OPEN;
        }
    }
	
    break;
    //其他活动
    default:
        wErrCode = wErrCode = ERR_ACTIVITY_NOTIFY::ID_ACTIVITY_TYPE_NOT_EXIT;
    }

    //通知LS更新通知表,注意别用CGTPktBuilder方法
    PKT_GSLS_ACTIVITY_NOTIFY_NTF stNtf;
    memset(&stNtf, 0, sizeof(stNtf));
    stNtf.dwPlayerID = m_dwPlayerID;
    stNtf.byActivityType = byActivityType;
    stNtf.byNotify = byNotify;
    SendMsg((CHAR*)(&stNtf), GSLS_ACTIVITY_NOTIFY_NTF);
    return wErrCode;
}



UINT16 CPlayer::GetActivityCenterData(UINT8& byActivityNum, DT_ACTIVITY_BASE_DATA astActivityInfo[MAX_ACTIVITY_NUM], UINT8 abyActivityStartCountDown[MAX_ACTIVITY_NUM])
{
    byActivityNum = 0;
    if(m_poEatBread)
    {
        abyActivityStartCountDown[byActivityNum] = 0;
        m_poEatBread->GetDT_ACTIVITY_BASE_DATA(astActivityInfo[byActivityNum++]);
    }
    if(m_poBossBattle)
    {
        CBossBPropMap& mapProp = CBossBattlePropMgr::Instance()->GetBossBPropMap();
        if(mapProp.size() > 0)
        {
            SBossBProp& stProp = mapProp.begin()->second;
            INT32 nTmp = SDNow().DiffDateDay(CGSConfig::Instance()->GetServerOpentTime());
            abyActivityStartCountDown[byActivityNum] = nTmp < stProp.wOpenActivityPassDay ? stProp.wOpenActivityPassDay - nTmp : 0;
        }
        m_poBossBattle->GetDT_ACTIVITY_BASE_DATA(astActivityInfo[byActivityNum++]);
    }
    if ((m_poCourage) && (this->GetCliVer() >= 310))
    {
        abyActivityStartCountDown[byActivityNum] = 0;
        m_poCourage->GetDT_ACTIVITY_BASE_DATA(astActivityInfo[byActivityNum++]);
    }
    
    /*
    // 门派跨服战
    abyActivityStartCountDown[byActivityNum] = 0;
    CGvGBuild::Instance()->GetDT_ACTIVITY_BASE_DATA(astActivityInfo[byActivityNum++]);
	*/

    //其他活动

    return ERR_OPNE_ACTIVITY_CENTER::ID_SUCCESS;
}


