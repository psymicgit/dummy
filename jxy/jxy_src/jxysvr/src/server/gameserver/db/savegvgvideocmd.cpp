///<------------------------------------------------------------------------------
//< @file:   gameserver\db\savegvgvideocmd.cpp
//< @author: hongkunan
//< @date:   2014年2月12日 10:13:9
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "savegvgvideocmd.h"
#include "logic/gvg/gvgvideomgr.h"
#include "db/dbcommoperate.h"
#include "db/autosqlbuf.h"
#include "common/server/ngstring.h"

VOID SDAPI CSaveGvGVideoCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    if (NULL == m_pstVideo)
    {
        return;
    }

    string strSql = BuildSql(poDBConn);
    m_pstVideo->Clear();
    if(!m_bOK)
    {
        return;
    }
    
    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs( strSql.c_str(), &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[%s %d] insert video fail: exec SQL error = [%s], len of SQL = [%u]" ), MSG_MARK, strError.c_str(), strSql.size());
        m_bOK = FALSE;

        return;
    }    
}

std::string CSaveGvGVideoCMD::BuildSql(SGDP::ISDDBConnection *poDBConn)
{
    CNGString strSql;

    std::string strVideoSql = BuildGvGVideoSql(*m_pstVideo, poDBConn);
    std::string strPKVideoSql = BuildPKVideoSql(*m_pstVideo, poDBConn);
    
    if(strVideoSql.empty() || strPKVideoSql.empty())
    {
        m_bOK = FALSE;
        return "";
    }

    strSql += strVideoSql;
    strSql += strPKVideoSql;

    return strSql.str();
}

std::string CSaveGvGVideoCMD::BuildGvGVideoSql(SGvGVideo &stVideo, SGDP::ISDDBConnection *poDBConn)
{
    CAutoSqlBuf oAutoSql;
    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oAutoTmp;

    TCHAR* pszSql = oAutoSql.GetSqlBuf();
    TCHAR* pszBuff = oAutoBuf.GetSqlBuf();
    TCHAR* pszTmp = oAutoTmp.GetSqlBuf();
    INT32 nRet = 0;

    if(NULL == pszSql || NULL == pszBuff || NULL == pszTmp)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CSaveGvGVideoCMD::BuildVideoSql alloc sqlbuf failed, not enough memory" ), MSG_MARK);
        return "";
    }
    
    CSDDateTime oDate(stVideo.stGvGInfo.qwTime);
    tstring strTime = SDTimeToString(oDate);

    DT_GVG_VIDEO &stGvGInfo = stVideo.stGvGInfo;

    // 64位是%llu
    sprintf(pszSql, "replace into gvgvideo(VideoID,Time,BigRound,SmallRound,BattleResult,PKVideoCnt,G1,G2,FinalRoundData) values ('%llu', '%s', %u, %u, %u, %u", 
        stGvGInfo.qwVideoID, 
        strTime.c_str(),
        stGvGInfo.stRound.byBigRound,
        stGvGInfo.stRound.bySmallRound,
        stGvGInfo.byBattleResult,
        stGvGInfo.byPKVideoCnt
    );

    INSERT_STRUCT(G1, stGvGInfo.stG1);
    INSERT_STRUCT(G2, stGvGInfo.stG2);
    INSERT_STRUCT(FinalRoundData, stGvGInfo.stFinalRoundInfo);    
    strcat(pszSql, ");");

    return pszSql;
}

std::string CSaveGvGVideoCMD::BuildPKVideoSql(SGvGVideo &stVideo, SGDP::ISDDBConnection *poDBConn)
{
    CAutoSqlBuf oResult;

    CAutoSqlBuf oAutoSql;
    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oAutoTmp;

    TCHAR* pszSql = oAutoSql.GetSqlBuf();
    TCHAR* pszBuff = oAutoBuf.GetSqlBuf();
    TCHAR* pszTmp = oAutoTmp.GetSqlBuf();

    TCHAR* pszResult = oResult.GetSqlBuf();
    INT32 nRet = 0;

    if(NULL == pszSql || NULL == pszBuff || NULL == pszTmp)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CSaveGvGVideoCMD::BuildVideoSql alloc sqlbuf failed, not enough memory" ), MSG_MARK);
        return "";
    }

    CSDDateTime oDate(stVideo.stGvGInfo.qwTime);
    tstring strTime = SDTimeToString(oDate);

    // 64位是%llu
    sprintf(pszSql, "replace into gvgpkvideo(VideoID,PlayerID1,PlayerID2,PKNum,BattleResult,VideoData) values ");

    char szFormatBuff[1024] = {0};

    DT_GVG_VIDEO &stGvGInfo = stVideo.stGvGInfo;
    for (UINT16 i = 0; i < stVideo.vecPvPVideo.size(); i++)
    {
        DT_GVG_PK_VIDEO &stPKVideo = stVideo.vecPvPVideo[i];

        if(i > 0)
        {
            strcat(pszSql, ",");
        }

        pszTmp[0] = '\0';
        sprintf(pszTmp, "(%llu,%u, %u, %u, %u", 
            stGvGInfo.qwVideoID, 
            stPKVideo.stPKVideo.stEmptyHPVideo.stMyselfInfo.dwPlayerID,
            stPKVideo.stPKVideo.stEmptyHPVideo.stEnemyInfo.dwPlayerID,
            i + 1,
            stPKVideo.stPKVideo.stEmptyHPVideo.byBattleResult
        );

        strcat(pszSql, pszTmp);
        pszTmp[0] = '\0';
        pszBuff[0] = '\0';

        INSERT_STRUCT(VideoData,stPKVideo);
        strcat(pszSql, ")");        
    }    

    strcat(pszSql, ";");
    return pszSql;
}

VOID CSaveGvGVideoCMD::SetUserData(VOID* pUserData)
{
    SGvGVideo *pstVideo = (SGvGVideo*)pUserData;
    
    m_pstVideo = new SGvGVideo;
    if(NULL == m_pstVideo)
    {
        m_videoID = 0;
        m_bOK = FALSE;
        return;
    }
        
    *m_pstVideo = *pstVideo;
    m_videoID = pstVideo->stGvGInfo.qwVideoID;
    m_bOK = TRUE;
}

VOID SDAPI CSaveGvGVideoCMD::OnExecuted()
{  
    if(NULL == m_pstVideo)
    {
        return;
    }

    CGvGVideoMgr::Instance()->OnSaveVideo(m_bOK, m_videoID);
    delete m_pstVideo;
}

VOID SDAPI CClearGvGVideoCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    string strSql = "truncate gvgvideo;truncate gvgpkvideo;";

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(strSql.c_str(), &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[%s %d] clear video fail: exec SQL error = [%s], SQL = [%s]" ), MSG_MARK, strError.c_str(), strSql.c_str());
        return;
    }
}
