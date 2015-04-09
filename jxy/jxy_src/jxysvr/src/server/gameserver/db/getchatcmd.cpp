
#include <db/autosqlbuf.h>
#include "getchatcmd.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/base64.h>
#include <logic/chat/chatmgr.h>
#include <db/dbcommoperate.h>
#include "common/server/ngstring.h"
#include "logic/other/errmsgmgr.h"
#include "logic/chat/wordfilter.h"

CGetChatCMD::CGetChatCMD()
{
    m_mapSilenceByDeviceID.clear();
    m_mapSilenceByUserID.clear();
    m_mapSilenceByPlayerID.clear();
    m_mapWordFilterData.clear();
    m_mapDspNameWordFilterData.clear();
}


CGetChatCMD::~CGetChatCMD()
{

}

VOID CGetChatCMD::SetUserData(VOID* pUserData)
{
    m_stSyncSilenceWordfileter2Db = *(SSyncSilenceWordfileter2Db*)pUserData;
}


VOID CGetChatCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    AddChatSilenceByDeviceID(poDBConn);
    DelChatSilenceByDeviceID(poDBConn);

    AddChatSilenceByUserID(poDBConn);
    DelAddChatSilenceByUserID(poDBConn);

    AddChatSilenceByPlayerID(poDBConn);
    DelAddChatSilenceByPlayerID(poDBConn);

    AddModifyWordFilter(poDBConn);
    DelModifyWordFilter(poDBConn);

    AddGmDeviceID(poDBConn);
    DelGmDeviceID(poDBConn);

    AddGmPlayerID(poDBConn);
    DelGmPlayerID(poDBConn);

    AddSYSChatDataNotice(poDBConn);
    DelSYSChatDataNotice(poDBConn);

    GetDataFromDB(poDBConn);
}


BOOL CGetChatCMD::AddSYSChatDataNotice(SGDP::ISDDBConnection *poDBConn)
{
    string strSql = "";
    CSysChatNoticeList	 &m_listSYSChatDataNotice = m_stSyncSilenceWordfileter2Db.m_listSYSChatDataNotice;
    UINT32 dwNum = 0;

    if(m_listSYSChatDataNotice.size() > 0)
    {
        strSql  = "insert into syschat(ZoneID, NoticeContent, ExpiresTime) values ";
        for(CSysChatNoticeListItr itr = m_listSYSChatDataNotice.begin(); itr != m_listSYSChatDataNotice.end(); itr++)
        {
            if(1 != itr->byOptType)
            {
                continue;
            }
            
            CSDDateTime oDateTime(itr->qwExpiresTime);
            tstring strExpiresTime = SDTimeToString(oDateTime);

            if(0 == dwNum)
            {
                char v_szData[1024] = {0};
                sprintf(v_szData, "(%u, '%s', '%s')", m_stSyncSilenceWordfileter2Db.m_wZoneID, itr->stData.abyChatContent, strExpiresTime.c_str());
                strSql += v_szData;
            }
            else
            {
                char v_szData[1024] = {0};
                sprintf(v_szData, ",(%u, '%s', '%s')", m_stSyncSilenceWordfileter2Db.m_wZoneID, itr->stData.abyChatContent, strExpiresTime.c_str());
                strSql += v_szData;
            }
            dwNum++;
        }
    }
    if(0 == dwNum)
    {
        return TRUE;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySYSChatDataNotice = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddSYSChatDataNotice exec sql failed! error:%s, SQL:%s"), MSG_MARK, strError.c_str(), strSql.c_str());
        return FALSE;
    }

    return TRUE;
}



BOOL CGetChatCMD::DelSYSChatDataNotice(SGDP::ISDDBConnection *poDBConn)
{
    string strSql = "";
    CSysChatNoticeList	 &m_listSYSChatDataNotice = m_stSyncSilenceWordfileter2Db.m_listSYSChatDataNotice;

    UINT32 dwNum = 0;
    BOOL bHaveData = FALSE;
    if(m_listSYSChatDataNotice.size() > 0)
    {
        strSql += "delete from syschat where ";
        for(CSysChatNoticeListItr itr = m_listSYSChatDataNotice.begin(); itr != m_listSYSChatDataNotice.end(); itr++)
        {
            if(0 != itr->byOptType)
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " NoticeID=";
                char v_szData[256] = {0};
                sprintf(v_szData, "%u", itr->uiNoticeID);
                strSql += v_szData;
            }
            else
            {
                strSql += " or NoticeID=";
                char v_szData[256] = {0};
                sprintf(v_szData, "%u", itr->uiNoticeID);
                strSql += v_szData;
            }
            dwNum++;
        }
        strSql += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySYSChatDataNotice = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelAddChatSilenceByUserID exec sql failed! error:%s, SQL:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::AddGmPlayerID(SGDP::ISDDBConnection *poDBConn)
{
    string strSql = "";
    C3232Map	mapGmPlayerID = m_stSyncSilenceWordfileter2Db.m_mapGmPlayerID;

    UINT32 dwNum = 0;
    if(mapGmPlayerID.size() > 0)
    {
        strSql += "insert into gmplayerid(PlayerID) values ";
        for(C3232MapItr itr = mapGmPlayerID.begin(); itr != mapGmPlayerID.end(); itr++)
        {
            if(1 != itr->second)
            {
                continue;
            }
            if(0 == itr->first)
            {
                continue;
            }

            if(0 == dwNum)
            {
                //ssSql << " ('" << itr->first << "')";
                char v_szData[256] = {0};
                sprintf(v_szData, "(%u)", itr->first);
                strSql += v_szData;
            }
            else
            {
                //ssSql << " ,('" << itr->first << "')";
                char v_szData[256] = {0};
                sprintf(v_szData, ",(%u)", itr->first);
                strSql += v_szData;
            }
            dwNum++;
        }
        strSql += "  on duplicate key update gmplayerid.PlayerID = values(gmplayerid.PlayerID);";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifyGmPlayerID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddChatSilenceByDeviceID exec sql failed! error:%s, SQL:%s"), MSG_MARK, strError.c_str(), strSql.c_str());
        return FALSE;
    }

    return TRUE;
}


BOOL CGetChatCMD::DelGmPlayerID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    C3232Map	mapGmPlayerID = m_stSyncSilenceWordfileter2Db.m_mapGmPlayerID;

    UINT32	dwNum = 0;
    BOOL		bHaveData = FALSE;
    if(mapGmPlayerID.size() > 0)
    {
        strSql += "delete from gmplayerid where ";
        for(C3232MapItr itr = mapGmPlayerID.begin(); itr != mapGmPlayerID.end(); itr++)
        {
            if(0 != itr->second)
            {
                continue;
            }
            if(0 == itr->first)
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " PlayerID='";
                strSql += itr->first;
                strSql +=  "'";
            }
            else
            {
                strSql += " or PlayerID='";
                strSql += itr->first;
                strSql +=  "'";
            }
            dwNum++;
        }
        strSql += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifyGmPlayerID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelGmPlayerID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::AddGmDeviceID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString	strSql;
    CStr32Map&	m_mapGmDeviceID = m_stSyncSilenceWordfileter2Db.m_mapGmDeviceID;

    UINT32 dwNum = 0;
    if(m_mapGmDeviceID.size() > 0)
    {
        strSql += "insert into gmdeviceid(DeviceID) values ";
        for(CStr32MapItr itr = m_mapGmDeviceID.begin(); itr != m_mapGmDeviceID.end(); itr++)
        {
            if(1 != itr->second)
            {
                continue;
            }
            if(0 == itr->first.compare("") || 0 == itr->first.compare(" "))
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " ('";
                strSql += itr->first.c_str();
                strSql += "')";
            }
            else
            {
                strSql += " ,('";
                strSql += itr->first.c_str();
                strSql += "')";
            }
            dwNum++;
        }
        strSql +=  "  on duplicate key update gmdeviceid.DeviceID = values(gmdeviceid.DeviceID);";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifyGmDeviceID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddChatSilenceByDeviceID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::DelGmDeviceID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString	strSql;
    CStr32Map&	m_mapGmDeviceID = m_stSyncSilenceWordfileter2Db.m_mapGmDeviceID;

    UINT32 dwNum = 0;
    BOOL bHaveData = FALSE;
    if(m_mapGmDeviceID.size() > 0)
    {
        strSql += "delete from gmdeviceid where ";
        for(CStr32MapItr itr = m_mapGmDeviceID.begin(); itr != m_mapGmDeviceID.end(); itr++)
        {
            if(0 != itr->second)
            {
                continue;
            }
            if(0 == itr->first.compare("") || 0 == itr->first.compare(" "))
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " DeviceID='";
                strSql += itr->first.c_str();
                strSql += "'";
            }
            else
            {
                strSql += " or DeviceID='";
                strSql += itr->first.c_str();
                strSql += "'";
            }
            dwNum++;
        }
        strSql +=  ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifyGmDeviceID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelGmDeviceID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::AddChatSilenceByDeviceID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString	strSql;
    CChatSilenceByDeviceIDMap&	mapModifySilenceByDeviceID = m_stSyncSilenceWordfileter2Db.m_mapModifySilenceByDeviceID;

    UINT32 dwNum = 0;
    if(mapModifySilenceByDeviceID.size() > 0)
    {
        strSql += "insert into silencedeviceid(DeviceID) values ";
        for(CChatSilenceByDeviceIDMapItr itr = mapModifySilenceByDeviceID.begin(); itr != mapModifySilenceByDeviceID.end(); itr++)
        {
            if(1 != itr->second)
            {
                continue;
            }
            if(0 == itr->first.compare("") || 0 == itr->first.compare(" "))
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql +=  " ('";
                strSql += itr->first.c_str();
                strSql += "')";
            }
            else
            {
                strSql +=  ", ('";
                strSql += itr->first.c_str();
                strSql += "')";
            }
            dwNum++;
        }
        strSql += "  on duplicate key update silencedeviceid.DeviceID = values(silencedeviceid.DeviceID);";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySilenceByDeviceID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddChatSilenceByDeviceID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::DelChatSilenceByDeviceID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString	strSql;
    CChatSilenceByDeviceIDMap&	mapModifySilenceByDeviceID = m_stSyncSilenceWordfileter2Db.m_mapModifySilenceByDeviceID;

    UINT32 dwNum = 0;
    BOOL bHaveData = FALSE;
    if(mapModifySilenceByDeviceID.size() > 0)
    {
        strSql += "delete from silencedeviceid where ";
        for(CChatSilenceByDeviceIDMapItr itr = mapModifySilenceByDeviceID.begin(); itr != mapModifySilenceByDeviceID.end(); itr++)
        {
            if(0 != itr->second)
            {
                continue;
            }
            if(0 == itr->first.compare("") || 0 == itr->first.compare(" "))
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " DeviceID='";
                strSql += itr->first.c_str();
                strSql +=  "'";
            }
            else
            {
                strSql += " or DeviceID='";
                strSql += itr->first.c_str();
                strSql +=  "'";
            }
            dwNum++;
        }
        strSql += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySilenceByDeviceID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelChatSilenceByDeviceID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::AddChatSilenceByUserID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    CChatSilenceByUserIDMap&	mapModifySilenceByUserID = m_stSyncSilenceWordfileter2Db.m_mapModifySilenceByUserID;

    UINT32 dwNum = 0;
    if(mapModifySilenceByUserID.size() > 0)
    {
        strSql += "insert into silenceuserid(UserID) values ";
        for(CChatSilenceByUserIDMapItr itr = mapModifySilenceByUserID.begin(); itr != mapModifySilenceByUserID.end(); itr++)
        {
            if(1 != itr->second)
            {
                continue;
            }
            if(0 == itr->first)
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " (";
                strSql += itr->first;
                strSql +=  ")";
            }
            else
            {
                strSql += " ,(";
                strSql += itr->first;
                strSql +=  ")";
            }
            dwNum++;
        }
        strSql += "  on duplicate key update silenceuserid.UserID = values(silenceuserid.UserID);";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySilenceByUserID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddChatSilenceByUserID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}



BOOL CGetChatCMD::DelAddChatSilenceByUserID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    CChatSilenceByUserIDMap&	mapModifySilenceByUserID = m_stSyncSilenceWordfileter2Db.m_mapModifySilenceByUserID;

    UINT32 dwNum = 0;
    BOOL bHaveData = FALSE;
    if(mapModifySilenceByUserID.size() > 0)
    {
        strSql += "delete from silenceuserid where ";
        for(CChatSilenceByUserIDMapItr itr = mapModifySilenceByUserID.begin(); itr != mapModifySilenceByUserID.end(); itr++)
        {
            if(0 != itr->second)
            {
                continue;
            }
            if(0 == itr->first)
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql +=  " UserID=";
                strSql +=  itr->first;
            }
            else
            {
                strSql += " or UserID=";
                strSql +=  itr->first;
            }
            dwNum++;
        }
        strSql  += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySilenceByUserID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelAddChatSilenceByUserID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::AddChatSilenceByPlayerID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    CChatSilenceByPlayerIDMap& mapModifySilenceByPlayerID = m_stSyncSilenceWordfileter2Db.m_mapModifySilenceByPlayerID;

    UINT32 dwNum = 0;
    if(mapModifySilenceByPlayerID.size() > 0)
    {
        strSql += "insert into silenceplayerid(PlayerID) values ";
        for(CChatSilenceByPlayerIDMapItr itr = mapModifySilenceByPlayerID.begin(); itr != mapModifySilenceByPlayerID.end(); itr++)
        {
            if(1 != itr->second)
            {
                continue;
            }
            if(0 == itr->first)
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " (";
                strSql += itr->first;
                strSql +=  ")";
            }
            else
            {
                strSql += " ,(";
                strSql += itr->first;
                strSql +=  ")";
            }
            dwNum++;
        }
        strSql += "  on duplicate key update silenceplayerid.PlayerID = values(silenceplayerid.PlayerID);";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySilenceByPlayerID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddChatSilenceByPlayerID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}
BOOL CGetChatCMD::DelAddChatSilenceByPlayerID(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    CChatSilenceByPlayerIDMap&	mapModifySilenceByPlayerID = m_stSyncSilenceWordfileter2Db.m_mapModifySilenceByPlayerID;

    UINT32 dwNum = 0;
    BOOL bHaveData = FALSE;
    if(mapModifySilenceByPlayerID.size() > 0)
    {
        strSql += "delete from silenceplayerid where ";
        for(CChatSilenceByPlayerIDMapItr itr = mapModifySilenceByPlayerID.begin(); itr != mapModifySilenceByPlayerID.end(); itr++)
        {
            if(0 != itr->second)
            {
                continue;
            }
            if(0 == itr->first)
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " PlayerID=";
                strSql += itr->first;
            }
            else
            {
                strSql += " or PlayerID=";
                strSql += itr->first;
            }
            dwNum++;
        }
        strSql += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }


    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifySilenceByPlayerID = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelAddChatSilenceByPlayerID exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

BOOL CGetChatCMD::AddModifyWordFilter(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    map<string, UINT32>&	mapModifyWordFilter = m_stSyncSilenceWordfileter2Db.m_mapModifyWordFilter;

    UINT32 dwNum = 0;
    if(mapModifyWordFilter.size() > 0)
    {
        strSql += "insert into wordfilter(Words) values ";
        for(map<string, UINT32>::iterator itr = mapModifyWordFilter.begin(); itr != mapModifyWordFilter.end(); itr++)
        {
            if(1 != itr->second)
            {
                continue;
            }
            if(0 == itr->first.compare("") || 0 == itr->first.compare(" "))
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += "(\"";
                strSql += itr->first.c_str();
                strSql += "\")";
            }
            else
            {
                strSql += " ,(\"";
                strSql += itr->first.c_str();
                strSql += "\")";
            }
            dwNum++;
        }
        strSql += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifyWordFilter = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: AddModifyWordFilter exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}
BOOL CGetChatCMD::DelModifyWordFilter(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;
    map<string, UINT32>&	mapModifyWordFilter = m_stSyncSilenceWordfileter2Db.m_mapModifyWordFilter;

    UINT32 dwNum = 0;
    if(mapModifyWordFilter.size() > 0)
    {
        strSql +=  "delete from wordfilter where ";
        for(map<string, UINT32>::iterator itr = mapModifyWordFilter.begin(); itr != mapModifyWordFilter.end(); itr++)
        {
            if(0 != itr->second)
            {
                continue;
            }
            if(0 == itr->first.compare("") || 0 == itr->first.compare(" "))
            {
                continue;
            }
            if(0 == dwNum)
            {
                strSql += " Words='";
                strSql +=  itr->first.c_str();
                strSql +=  "'";
            }
            else
            {
                strSql += " or Words='";
                strSql +=  itr->first.c_str();
                strSql +=  "'";
            }
            dwNum++;
        }
        strSql += ";";
    }
    if(0 == dwNum)
    {
        return TRUE;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stSyncSilenceWordfileter2Db.m_bModifyWordFilter = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: DelModifyWordFilter exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }

    return TRUE;
}


BOOL CGetChatCMD::GetSilenceDataFromDB(SGDP::ISDDBConnection *poDBConn)
{
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    UINT32 dwPlayerID	=	0;
    UINT32 dwUserID	=	0;
    string strDeviceID = "";
    INT32 nRet = poDBConn->ExecuteSqlRs("select * from  silencedeviceid;", &pRecordSet);
    if(nRet < 0)
    {
        return FALSE;
    }
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            strDeviceID	= pRecordSet->GetFieldValueByName("DeviceID");
            if (0 != strDeviceID.length())
            {
                m_mapSilenceByDeviceID[strDeviceID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    /////////////////////////////////////

    nRet = poDBConn->ExecuteSqlRs("select * from  silenceplayerid;", &pRecordSet);
    if(nRet < 0)
    {
        return FALSE;
    }
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            dwPlayerID	= SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
            if (0 != dwPlayerID)
            {
                m_mapSilenceByPlayerID[dwPlayerID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    /////////////////////////////////////

    nRet = poDBConn->ExecuteSqlRs("select * from  silenceuserid;", &pRecordSet);
    if(nRet < 0)
    {
        return FALSE;
    }
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            dwUserID	= SDAtou(pRecordSet->GetFieldValueByName("UserID"));
            if (0 != dwUserID)
            {
                m_mapSilenceByUserID[dwUserID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;

    }

    return TRUE;
}

BOOL CGetChatCMD::GetSysChatFromDB(SGDP::ISDDBConnection *poDBConn)
{
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    CHAR szSql[1024] = {0};
    sprintf(szSql, "select NoticeID, NoticeContent, ExpiresTime from syschat where ZoneID='%u' or ZoneID='0' order by ZoneID asc, NoticeID asc;", m_stSyncSilenceWordfileter2Db.m_wZoneID);
    INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet);
    if(nRet < 0)
    {
        return FALSE;
    }
    UINT64 qwNow = SDTimeSecs();

    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            SYS_CHAT_DATA stSYS_CHAT_DATA;
            stSYS_CHAT_DATA.uiNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID"));
            stSYS_CHAT_DATA.stData.dwPlayerID = 0;
            string strDspName = ""; // 这里昵称为空就行，由客户端根据类型来判断 // CMsgDefMgr::Instance()->GetErrMsg("NOTICE");
            SDStrcpy(stSYS_CHAT_DATA.stData.aszUserName, strDspName.c_str());
            stSYS_CHAT_DATA.stData.byChatType = ECT_SYS;
            stSYS_CHAT_DATA.stData.dwChatLen = pRecordSet->GetFieldLengthByName("NoticeContent");
            stSYS_CHAT_DATA.stData.dwChatLen = stSYS_CHAT_DATA.stData.dwChatLen >= MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : stSYS_CHAT_DATA.stData.dwChatLen;
            memcpy(stSYS_CHAT_DATA.stData.abyChatContent, (VOID*)(pRecordSet->GetFieldValueByName("NoticeContent")), stSYS_CHAT_DATA.stData.dwChatLen);
            GetDateTime2Stamp(stSYS_CHAT_DATA.qwExpiresTime, "ExpiresTime");

            stSYS_CHAT_DATA.stData.qwChatTime = (qwNow > stSYS_CHAT_DATA.qwExpiresTime ? stSYS_CHAT_DATA.qwExpiresTime : qwNow);
            m_listSYSChatDataNotice.push_back(stSYS_CHAT_DATA);
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetChatCMD::GetWordFilterDataFromDB(SGDP::ISDDBConnection *poDBConn)
{
    SGDP::ISDDBRecordSet* pRecordSet = NULL;

    /////////////////////////////////////
    INT32 nRet = poDBConn->ExecuteSqlRs("select * from  wordfilter;", &pRecordSet);
    if(nRet < 0)
    {
        return FALSE;
    }
    if(SDDB_HAS_RECORDSET == nRet)
    {
        int nLen = 0;
        TWordFilterData stWordFilterData;
        //查询的表结果
        while(pRecordSet->GetRecord())// 当有行结果时
        {
            stWordFilterData.Init();
            nLen = pRecordSet->GetFieldLengthByName("Words");
            nLen = nLen >= MAX_WORD_FILTER ? MAX_WORD_FILTER - 1 : nLen;

            string strWords	= pRecordSet->GetFieldValueByName("Words");
            string strTmp		= SDStrlwr(strWords);
            _SDTStrncpy(stWordFilterData.m_szWordFilters, strTmp.c_str(), nLen);
            //_SDTStrncpy(stWordFilterData.m_szWordFilters, pRecordSet->GetFieldValueByName("Words"), nLen);
            string strkey = stWordFilterData.m_szWordFilters;
            if(m_mapWordFilterData.find(strkey) == m_mapWordFilterData.end())
            {
                m_mapWordFilterData[strkey] = stWordFilterData;
            }
        }
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }

        CWordFilterDataMapItr itr;
        for(itr = m_mapWordFilterData.begin(); itr != m_mapWordFilterData.end(); itr++)
        {
            KMP_GetNext(itr->second.m_szWordFilters, itr->second.m_kmpvalue);	// 得到一个与内容有关的数值m_kmpvalue[i]
        }
    }


    nRet = poDBConn->ExecuteSqlRs("select * from  dspnamefilter;", &pRecordSet);
    if(nRet < 0)
    {
        return FALSE;
    }
    int nLen = 0;
    TWordFilterData stWordFilterData;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        //查询的表结果
        while(pRecordSet->GetRecord())// 当有行结果时
        {
            stWordFilterData.Init();
            nLen = pRecordSet->GetFieldLengthByName("Words");
            nLen = nLen >= MAX_WORD_FILTER ? MAX_WORD_FILTER - 1 : nLen;
            _SDTStrncpy(stWordFilterData.m_szWordFilters, pRecordSet->GetFieldValueByName("Words"), nLen);
            string strkey = stWordFilterData.m_szWordFilters;
            if(m_mapDspNameWordFilterData.find(strkey) == m_mapDspNameWordFilterData.end())
            {
                m_mapDspNameWordFilterData[strkey] = stWordFilterData;
            }
        }
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
    }
    CWordFilterDataMapItr itr;
    for(itr = m_mapDspNameWordFilterData.begin(); itr != m_mapDspNameWordFilterData.end(); itr++)
    {
        KMP_GetNext(itr->second.m_szWordFilters, itr->second.m_kmpvalue);	// 得到一个与内容有关的数值m_kmpvalue[i]
    }

    return TRUE;
}



BOOL CGetChatCMD::GetGMDataFromDB(SGDP::ISDDBConnection *poDBConn)
{
    UINT32 dwPlayerID	=	0;
    UINT32 dwUserID	=	0;
    string strDeviceID = "";

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs("select * from  gmdeviceid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            strDeviceID	= pRecordSet->GetFieldLengthByName("DeviceID");
            if (0 != strDeviceID.length())
            {
                m_mapGmDeviceID[strDeviceID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    /////////////////////////////////////

    nRet = poDBConn->ExecuteSqlRs("select * from  gmplayerid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            dwPlayerID	= SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
            if (0 != dwPlayerID)
            {
                m_mapGmPlayerID[dwPlayerID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }


    return TRUE;
}

BOOL CGetChatCMD::GetDataFromDB(SGDP::ISDDBConnection *poDBConn)
{
    if(!m_stSyncSilenceWordfileter2Db.m_bGetDataFromDB)
    {
        return TRUE;
    }

    if(!GetSilenceDataFromDB(poDBConn))
    {
        return FALSE;
    }

    if(!GetSysChatFromDB(poDBConn))
    {
        return FALSE;
    }

    if(!GetWordFilterDataFromDB(poDBConn))
    {
        return FALSE;
    }

    if(!GetGMDataFromDB(poDBConn))
    {
        return FALSE;
    }

    m_stSyncSilenceWordfileter2Db.m_bGetDataFromDBSuccess = TRUE;

    return TRUE;
}



VOID CGetChatCMD::OnExecuted()
{
    SSyncSilenceWordfileter2Db& m_oSyncSilenceWordfileter2Db = CChatMgr::Instance()->GetSyncSilenceWordfileter2Db();
    if(m_stSyncSilenceWordfileter2Db.m_bModifySilenceByDeviceID)
    {
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByDeviceID.clear();
    }
    if(m_stSyncSilenceWordfileter2Db.m_bModifySilenceByUserID)
    {
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByUserID.clear();
    }
    if(m_stSyncSilenceWordfileter2Db.m_bModifySilenceByPlayerID)
    {
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByPlayerID.clear();
    }
    if(m_stSyncSilenceWordfileter2Db.m_bModifyWordFilter)
    {
        m_oSyncSilenceWordfileter2Db.m_mapModifyWordFilter.clear();
    }

    if(m_stSyncSilenceWordfileter2Db.m_bModifyGmDeviceID)
    {
        m_oSyncSilenceWordfileter2Db.m_mapGmDeviceID.clear();
    }
    if(m_stSyncSilenceWordfileter2Db.m_bModifyGmPlayerID)
    {
        m_oSyncSilenceWordfileter2Db.m_mapGmPlayerID.clear();
    }

    if(m_stSyncSilenceWordfileter2Db.m_bModifySYSChatDataNotice)
    {
        m_oSyncSilenceWordfileter2Db.m_listSYSChatDataNotice.clear();
    }

    if(m_stSyncSilenceWordfileter2Db.m_bGetDataFromDB && m_stSyncSilenceWordfileter2Db.m_bGetDataFromDBSuccess)
    {
        CChatMgr::Instance()->SetSilenceData(m_mapSilenceByDeviceID, m_mapSilenceByUserID, m_mapSilenceByPlayerID);
        CChatMgr::Instance()->SetSYSChatDataNoticeData(m_listSYSChatDataNotice);
        CChatMgr::Instance()->SetWordFilterData(m_mapWordFilterData, m_mapDspNameWordFilterData);
        CChatMgr::Instance()->SetGMData(m_mapGmDeviceID, m_mapGmPlayerID);
    }
}

