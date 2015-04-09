#include "gsnoticemgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsconfig.h>
#include <net/cli/usermgr.h>
#include <db/getchatcmd.h>
#include <logic/player/player.h>
#include <framework/gsapi.h>
#include <logic/player/playermgr.h>

using namespace SGDP;


IMPLEMENT_SINGLETON(CGsNoticeMgr)




CGsNoticeMgr::CGsNoticeMgr()
{
    memset(&m_stGsNoticeInfo, 0, sizeof(m_stGsNoticeInfo));
    m_wZoneID = gsapi::GetZoneID();
	m_bSendFlag = FALSE;
}

CGsNoticeMgr::~CGsNoticeMgr()
{

}



BOOL CGsNoticeMgr::Init()
{
    if(!InitGsNoticeFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitGsNoticeFromDB failed!"), MSG_MARK);
        return FALSE;
    }


    return TRUE;
}

VOID CGsNoticeMgr::UnInit()
{

}

VOID CGsNoticeMgr::SetGsNoticeData(const DT_NOTICE_DATA_DB& stGsNoticeInfoDB)
{
	UINT64 qwCurTime = SGDP::SDTimeSecs();
	if(0 == memcmp(&m_stGsNoticeInfoDB, &stGsNoticeInfoDB, sizeof(DT_NOTICE_DATA_DB)) )
	{
		if ( qwCurTime < stGsNoticeInfoDB.qwExpiresTime)
		{
			return ;
		}
		else if ( stGsNoticeInfoDB.qwExpiresTime <= qwCurTime && m_bSendFlag)
		{
			return ;
		}
	}
	memcpy(&m_stGsNoticeInfoDB, &stGsNoticeInfoDB, sizeof(DT_NOTICE_DATA_DB));
	
	if (stGsNoticeInfoDB.qwExpiresTime <= qwCurTime )
	{
		memset( &m_stGsNoticeInfo, 0, sizeof(DT_NOTICE_DATA));
		m_bSendFlag = TRUE;
	}
	else
	{
		m_stGsNoticeInfo.dwNoticeID = m_stGsNoticeInfoDB.dwNoticeID;
		m_stGsNoticeInfo.byUrlFlag = m_stGsNoticeInfoDB.byUrlFlag;
		m_stGsNoticeInfo.byForcePopFlag = m_stGsNoticeInfoDB.byForcePopFlag;
		SDStrncpy(m_stGsNoticeInfo.aszNoticeContent, m_stGsNoticeInfoDB.aszNoticeContent, MAX_NOTICE_CONTENT_LEN - 1); 
		m_bSendFlag = FALSE;
	}

    //给所有在线玩家发生通知
    PKT_CLIGS_NOTICE_NEW_NTF stNtf;
    memcpy(&stNtf.stNoticeInfo, &m_stGsNoticeInfo, sizeof(m_stGsNoticeInfo));
    const CSessionID2UserMap& mapUser = CUserMgr::Instance()->GetAllUserMap();
    for(CSessionID2UserMapConstItr itr = mapUser.begin(); itr != mapUser.end(); itr++)
    {
        UINT32 dwID = itr->first;
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
        if((NULL == poPlayer) )
        {
            continue;
        }
		if (!poPlayer->CkLogin())
		{
			continue;
		}
		if ( VER_PET <= poPlayer->GetCliVer())
		{
			//通知
			poPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_NOTICE_NEW_NTF);
		}
		else
		{
			PKT_CLIGS_NOTICE_NTF stOldNtf;
			stOldNtf.stNoticeInfo.dwNoticeID = stNtf.stNoticeInfo.dwNoticeID;
			stOldNtf.stNoticeInfo.byUrlFlag = stNtf.stNoticeInfo.byUrlFlag;
			stOldNtf.stNoticeInfo.byForcePopFlag = stNtf.stNoticeInfo.byForcePopFlag;
			SDStrncpy( stOldNtf.stNoticeInfo.aszNoticeContent, stNtf.stNoticeInfo.aszNoticeContent, sizeof(stOldNtf.stNoticeInfo.aszNoticeContent) - 1);
			//通知
			poPlayer->SendMsg((const CHAR*)(&stOldNtf), CLIGS_NOTICE_NTF);
		}
    }
}

VOID CGsNoticeMgr::GetGsNoticeData(DT_NOTICE_DATA_NEW& stGsNoticeInfo)
{
    memcpy(&stGsNoticeInfo, &m_stGsNoticeInfo, sizeof(m_stGsNoticeInfo));
}


UINT32 CGsNoticeMgr::OnEvent()
{

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GETGSNOTICE);
    poCMD->SetUserData(&m_wZoneID);
    if(NULL != poCMD)
    {
        gsapi::GetDBMgr()->AddPayCommand(poCMD);
    }

    return 0;
}


BOOL CGsNoticeMgr::InitGsNoticeFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    m_wZoneID = gsapi::GetZoneID();

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from gsnotice where (ZoneID='%u' or ZoneID='0') order by ZoneID desc, NoticeID desc limit 1;", m_wZoneID);
    ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        //日常公告
        m_stGsNoticeInfo.dwNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID"));
        m_stGsNoticeInfo.byForcePopFlag = SDAtou(pRecordSet->GetFieldValueByName("ForcePopFlag"));
        m_stGsNoticeInfo.byUrlFlag = SDAtou(pRecordSet->GetFieldValueByName("UrlFlag"));
        INT32 nLen = pRecordSet->GetFieldLengthByName("NoticeContent");
        nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
        SDStrncpy(m_stGsNoticeInfo.aszNoticeContent, pRecordSet->GetFieldValueByName("NoticeContent"), nLen);
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
