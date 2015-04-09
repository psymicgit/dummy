#include "oprateingactivitymgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include <framework/gsconfig.h>
#include <net/cli/usermgr.h>
#include <db/getchatcmd.h>
#include <logic/player/player.h>
#include <logic/monster/monstermgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/other/errmsgmgr.h>
#include <logic/instance/instance.h>
#include <logic/player/playermgr.h>
#include "common/server/ngstring.h"
#include "common/client/gmdef.h"

#include <errdef.h>

using namespace SGDP;


IMPLEMENT_SINGLETON(COprateingActivityMgr)




COprateingActivityMgr::COprateingActivityMgr()
{
    memset(&m_stOprateingActivityProp, 0, sizeof(m_stOprateingActivityProp));
    m_wZoneID = 0;
    m_bBeginSendFlag = FALSE;
    m_bEndSendFlag = FALSE;
}

COprateingActivityMgr::~COprateingActivityMgr()
{

}



BOOL COprateingActivityMgr::Init()
{
    if(!InitOprateingActivityFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitOprateingActivityFromDB failed!"), MSG_MARK);
        return FALSE;
    }


    return TRUE;
}

VOID COprateingActivityMgr::UnInit()
{

}


VOID COprateingActivityMgr::SetOprateingActivityData(SOprateingActivityProp	stProp)
{
    UINT16 wOpenOprateingActivityLevel = CSinglePramMgr::Instance()->GetOpenOprateingActivityLevel();
    UINT32 dwLastVersionID = m_stOprateingActivityProp.stProtData.dwVersionID;
    memcpy(&m_stOprateingActivityProp, &stProp, sizeof(stProp));
    if((m_stOprateingActivityProp.stProtData.dwVersionID != dwLastVersionID) &&
            (0 != m_stOprateingActivityProp.stProtData.dwVersionID) && (0 != m_stOprateingActivityProp.stProtData.byNum))//有更新
    {
        m_bBeginSendFlag = FALSE;
    }

    UINT64 qwCurTime = SDTimeSecs();
    //在活动时间内
    if((m_stOprateingActivityProp.qwStartTime <= qwCurTime) && (m_stOprateingActivityProp.qwEndTime > qwCurTime))
    {
        if ( !m_bBeginSendFlag)
        {
            m_bBeginSendFlag = TRUE;
            m_bEndSendFlag = FALSE;
            //给所有在线玩家发生通知
            PKT_CLIGS_OPERATING_ACTIVITY_NTF stNtf;
            memcpy(&stNtf.stOperatingActivityInfo, &m_stOprateingActivityProp.stProtData, sizeof(DT_OPERATING_ACTIVITY_DATA));
            const CSessionID2UserMap& mapUser = CUserMgr::Instance()->GetAllUserMap();
            for(CSessionID2UserMapConstItr itr = mapUser.begin(); itr != mapUser.end(); itr++)
            {
                UINT32 dwID = itr->first;
                CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
                if((NULL == poPlayer) || (!poPlayer->CkLogin()))
                {
                    continue;
                }
                if(poPlayer->GetLevel() < wOpenOprateingActivityLevel)
                {
                    continue;
                }
                //通知
                poPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_OPERATING_ACTIVITY_NTF);
            }
        }
    }

    if (m_stOprateingActivityProp.qwEndTime < qwCurTime || 0 == m_stOprateingActivityProp.stProtData.byNum )
    {
        if ( !m_bEndSendFlag)
        {
            m_bEndSendFlag = TRUE;
            m_bBeginSendFlag = FALSE;
            //给所有在线玩家发生通知
            PKT_CLIGS_OPERATING_ACTIVITY_NTF stNtf;
            memcpy(&stNtf.stOperatingActivityInfo, &m_stOprateingActivityProp.stProtData, sizeof(DT_OPERATING_ACTIVITY_DATA));
            stNtf.stOperatingActivityInfo.byNum = 0;
            const CSessionID2UserMap& mapUser = CUserMgr::Instance()->GetAllUserMap();
            for(CSessionID2UserMapConstItr itr = mapUser.begin(); itr != mapUser.end(); itr++)
            {
                UINT32 dwID = itr->first;
                CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
                if((NULL == poPlayer) || (!poPlayer->CkLogin()))
                {
                    continue;
                }
                if(poPlayer->GetLevel() < wOpenOprateingActivityLevel)
                {
                    continue;
                }
                //通知
                poPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_OPERATING_ACTIVITY_NTF);
            }
        }
    }
}


VOID COprateingActivityMgr::CloseActivityOnEnterInstance(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    if(poPlayer->GetLevel() < CSinglePramMgr::Instance()->GetOpenOprateingActivityLevel())
    {
        return;
    }

    PKT_CLIGS_OPERATING_ACTIVITY_NTF stNtf;
    memcpy(&stNtf.stOperatingActivityInfo, &m_stOprateingActivityProp.stProtData, sizeof(DT_OPERATING_ACTIVITY_DATA));
    stNtf.stOperatingActivityInfo.byNum = 0; //客户端关闭
    poPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_OPERATING_ACTIVITY_NTF);
}

VOID COprateingActivityMgr::OpenActivityOnEnterHome(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }
    if(poPlayer->GetLevel() < CSinglePramMgr::Instance()->GetOpenOprateingActivityLevel())
    {
        return;
    }
    UINT64 qwCurTime = SDTimeSecs();
    if((m_stOprateingActivityProp.qwStartTime <= qwCurTime) && (m_stOprateingActivityProp.qwEndTime > qwCurTime) && (m_stOprateingActivityProp.stProtData.byNum != 0))
    {
        PKT_CLIGS_OPERATING_ACTIVITY_NTF stNtf;
        memcpy(&stNtf.stOperatingActivityInfo, &m_stOprateingActivityProp.stProtData, sizeof(DT_OPERATING_ACTIVITY_DATA));
        poPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_OPERATING_ACTIVITY_NTF);
    }
}



VOID COprateingActivityMgr::GetOprateingActivityData(CPlayer* poPlayer, DT_OPERATING_ACTIVITY_DATA& stOperatingActivityInfo)
{
    memset(&stOperatingActivityInfo, 0, sizeof(stOperatingActivityInfo));
    if(poPlayer->GetLevel() < CSinglePramMgr::Instance()->GetOpenOprateingActivityLevel())
    {
        return;
    }
    UINT64 qwCurTime = SDTimeSecs();
    if(m_stOprateingActivityProp.qwStartTime <= qwCurTime && m_stOprateingActivityProp.qwEndTime > qwCurTime)
    {
        memcpy(&stOperatingActivityInfo, &m_stOprateingActivityProp.stProtData, sizeof(stOperatingActivityInfo));
    }
}

BOOL COprateingActivityMgr::IsOpenSoulAty(CPlayer* poPlayer) {
	if (NULL == poPlayer || poPlayer->GetLevel() < CSinglePramMgr::Instance()->GetOpenOprateingActivityLevel()) {
		return FALSE;
	}
	return m_stOprateingActivityProp.bOpenSoulAty;
}

UINT32 COprateingActivityMgr::OnEvent()
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GETOPRATEINGACTIVITY);
    poCMD->SetUserData(&m_wZoneID);
    if(NULL != poCMD)
    {
        gsapi::GetDBMgr()->AddPayCommand(poCMD);
    }

    return 0;
}


BOOL COprateingActivityMgr::InitOprateingActivityFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    m_wZoneID = gsapi::GetZoneID();

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from operatingactivityprop where (ZoneID='%u' or ZoneID='0') order by ZoneID Desc, VersionID desc limit 1;", m_wZoneID);
    ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        DT_OPERATING_ACTIVITY_DATA& stProtData = m_stOprateingActivityProp.stProtData;
        stProtData.byNum = SDAtou(pRecordSet->GetFieldValueByName("Num"));
        stProtData.dwVersionID = SDAtou(pRecordSet->GetFieldValueByName("VersionID"));
        INT32 nLen = pRecordSet->GetFieldLengthByName("Desc");
        nLen = nLen >= MAX_COMMON_TXT_LEN ? (MAX_COMMON_TXT_LEN - 1) : nLen;
        SDStrncpy(stProtData.aszDesc, pRecordSet->GetFieldValueByName("Desc"), nLen);
        nLen = pRecordSet->GetFieldLengthByName("Url");
        nLen = nLen >= MAX_COMMON_TXT_LEN ? (MAX_COMMON_TXT_LEN - 1) : nLen;
        SDStrncpy(stProtData.aszUrl, pRecordSet->GetFieldValueByName("Url"), nLen);
        stProtData.byWebTabType = SDAtou(pRecordSet->GetFieldValueByName("WebTabType"));
        nLen = pRecordSet->GetFieldLengthByName("ExtContext");
        nLen = nLen >= MAX_COMMON_TXT_LEN ? (MAX_COMMON_TXT_LEN - 1) : nLen;
        SDStrncpy(stProtData.aszExtContext, pRecordSet->GetFieldValueByName("ExtContext"), nLen);

        //m_stOprateingActivityProp.byTowerIdx = SDAtou(pRecordSet->GetFieldValueByName("TowerIdx"));
        //m_stOprateingActivityProp.byInstanceIdx = SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx"));
        //m_stOprateingActivityProp.byInstanceIdx2 = SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx2"));
        //m_stOprateingActivityProp.byInstanceIdx3 = SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx3"));

        string strTime(pRecordSet->GetFieldValueByName("StartTime"));
        CSDDateTime stDateTime;
        if(SDTimeFromString(strTime, stDateTime))
        {
            m_stOprateingActivityProp.qwStartTime = stDateTime.GetTimeValue();
        }
        else
        {
            m_stOprateingActivityProp.qwStartTime = SDTimeSecs();
        }

        strTime = pRecordSet->GetFieldValueByName("EndTime");
        if(SDTimeFromString(strTime, stDateTime))
        {
            m_stOprateingActivityProp.qwEndTime = stDateTime.GetTimeValue();
        }
        else
        {
            m_stOprateingActivityProp.qwEndTime = SDTimeSecs() + 36000;
        }

    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

VOID COprateingActivityMgr::CkTimeOutBattleRcd()
{
    UINT64 qwCurTime = SDTimeSecs();
    for(CBattleRcdMapItr itr = m_mapBattleRcd.begin(); itr != m_mapBattleRcd.end(); )
    {
        if(qwCurTime - itr->second.qwBattleTime > 3600)
        {
            m_mapBattleRcd.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}


BOOL COprateingActivityMgr::PhpBattle(CPlayer* poPlayer, UINT32 dwActivityVersionID, UINT8 byTownerIdx, UINT8 byInstanceIdx, string& strRet)
{
    if(NULL == poPlayer)
    {
        strRet = "{\"status\":\"inner error!\"}";
        return FALSE;
    }
    DT_OPERATING_ACTIVITY_DATA& stProtData = m_stOprateingActivityProp.stProtData;
    if((0 == stProtData.byNum) || (0 == stProtData.dwVersionID))
    {
        strRet = "{\"status\":\"Current have no activity!\"}";
        return FALSE;
    }
    if(dwActivityVersionID != stProtData.dwVersionID)
    {
        strRet = "{\"status\":\"Current activity is not this!\"}";
        return FALSE;
    }
    UINT64 qwCurTime = SDTimeSecs();
    if(m_stOprateingActivityProp.qwStartTime > qwCurTime)
    {
        strRet = "{\"status\":\"activity not startup!\"}";
        return FALSE;
    }
    if(m_stOprateingActivityProp.qwEndTime < qwCurTime)
    {
        strRet = "{\"status\":\"activity overtime!\"}";
        return FALSE;
    }
    CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(EIT_OAI, byTownerIdx, byInstanceIdx, 1);
    if(NULL == poMonsterGroup)
    {
        strRet = "{\"status\":\"invalid InstanceIdx!\"}";
        return FALSE;
    }
    SBattleRcd stBattleRcd;
    PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5& stBattleAck = stBattleRcd.stBattleAck;
    DT_BATTLE_DATA5& stBattleInfo = stBattleAck.stBattleInfo;
    if(!CBattleMgr::Instance()->Battle4(poPlayer, poMonsterGroup, stBattleInfo))
    {
        strRet = "{\"status\":\"inner error!\"}";
        return FALSE;
    }

    stBattleRcd.qwBattleTime = SDTimeSecs();
    sprintf(stBattleRcd.szBattleKey, "%u_%llu_%d", poPlayer->GetID(), stBattleRcd.qwBattleTime, SDRandom(10000));
    SDStrcpy(stBattleAck.aszBattleKey, stBattleRcd.szBattleKey);
    if(EBT_FAILED == stBattleInfo.byBattleResult)
    {
        sprintf((CHAR*)strRet.c_str(), "%s%s%s", "{\"battle result\":\"Failed!\",\"battle key\":\"", stBattleRcd.szBattleKey, "\"}");
    }
    else
    {
        sprintf((CHAR*)strRet.c_str(), "%s%s%s", "{\"battle result\":\"Success!\",\"battle key\":\"", stBattleRcd.szBattleKey, "\"}");
        //赢，则给奖励
        stBattleAck.byPassFlag = 1;
        poPlayer->GetInstance().RatePassBoxEnc(EIT_OAI, byTownerIdx, byInstanceIdx, stBattleAck.astPassEncourageBox[0]);
        //检测背包是否满了
        DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage = {0};
        if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_BAG_FULL == poPlayer->GetInstance().PassBoxEnc(EIT_OAI, byTownerIdx, byInstanceIdx, stPassEncourage, &stBattleAck.astResSync[0]))
        {
            strRet = "{\"status\":\"bag full!\"}";
            return FALSE;
        }
    }

    m_mapBattleRcd[poPlayer->GetID()] = stBattleRcd;

    return TRUE;
}

VOID COprateingActivityMgr::CliBattle(CPlayer* poPlayer, string strBattleKey, PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5* pstAck)
{
    if(NULL == poPlayer)
    {
        sprintf(pstAck->aszErrInfo, "%s", "{\"status\":\"inner error!\"}");
        return;
    }

    CBattleRcdMapItr itr = m_mapBattleRcd.find(poPlayer->GetID());
    if(itr == m_mapBattleRcd.end())
    {
        string strTemp = CMsgDefMgr::Instance()->GetErrMsg("FIRST_GOIN_ACTIVITIES");
        sprintf(pstAck->aszErrInfo, "%s", strTemp.c_str());
        return;
    }
    SBattleRcd& stBattleRcd = itr->second;
    if(0 != strBattleKey.compare(stBattleRcd.szBattleKey))
    {
        string strTemp = CMsgDefMgr::Instance()->GetErrMsg("FIRST_GOIN_ACTIVITIES");
        sprintf(pstAck->aszErrInfo, "%s", strTemp.c_str());
        return;
    }
    memcpy(pstAck, &stBattleRcd.stBattleAck, sizeof(PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5));
}