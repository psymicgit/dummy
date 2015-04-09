///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgvideomgr.cpp
//< @author: hongkunan
//< @date:   2014年2月11日 21:1:35
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "protogspk.h"
#include "gvgvideomgr.h"
#include "framework/gsapi.h"

#include "db/savegvgvideocmd.h"
#include "db/dbcommoperate.h"
#include "db/autosqlbuf.h"

namespace gvgvideoutil
{
    std::string EchoBattleResult(UINT16 eResult)
    {
        std::string strResult;
        switch(eResult)
        {
        case GR_NOT_START:
            strResult = "not start";
            break;

        case GR_LEFT_WIN:
            strResult = "left win";
            break;

        case GR_RIGHT_WIN:
            strResult = "right win";
            break;
        }

        return strResult;
    }
}

IMPLEMENT_SINGLETON(CGvGVideoMgr)

BOOL CGvGVideoMgr::Init()
{
    Clear();

    if(!LoadGvGVideoFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGVideoFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadPKVideoFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadPKVideoFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

BOOL CGvGVideoMgr::LoadGvGVideoFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oEscapeBuf;
    CHAR* pszBuff=oAutoBuf.GetSqlBuf();
    CHAR* pszEscapeBuff=oEscapeBuf.GetSqlBuf();

    // 先尝试解码
    SGvGVideo stVideo;
    stVideo.Clear();

    INT32 nRet = stVideo.stGvGInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
    if(nRet <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: m_stBattleData.oBattleData is failed"), MSG_MARK);
        return FALSE;
    }

    UINT32 dwRetCode = pDBSession->EscapeString(pszBuff, nRet, pszEscapeBuff, sizeof(DT_GVG_VIDEO));
    if(dwRetCode <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    char buf[512] = {0};
    sprintf(buf, "select * from gvgvideo");

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    nRet = pDBSession->ExecuteSqlRs(buf, &pRecordSet, NULL, &strError);
    if(SDDB_HAS_RECORDSET != nRet && SDDB_NO_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGVideoMgr::LoadGvGVideoFromDB execute sql failed! sql = [%s], err = [%s]"), MSG_MARK, buf, strError.c_str());
        return FALSE;
    }

    if(SDDB_HAS_RECORDSET == nRet && pRecordSet)
    {
        DT_GVG_VIDEO &stGvGInfo = stVideo.stGvGInfo;

        while(pRecordSet->GetRecord()) // 当有行结果时
        {
            stVideo.Clear();

            stGvGInfo.qwVideoID = SDAtou64(pRecordSet->GetFieldValueByName("VideoID"));

            const CHAR *pszVideoTime = pRecordSet->GetFieldValueByName("Time");
            CSDDateTime oStartTime;
            if(!SDTimeFromString(pszVideoTime, oStartTime))
            {
                SYS_CRITICAL( _SDT("[%s: %d] CGvGBuild::LoadDataFromDB parse VideoTime %s of Video<%llu> failed, aborted!"), MSG_MARK, pszVideoTime, stGvGInfo.qwVideoID);
                return FALSE;
            }

            stGvGInfo.qwTime = oStartTime.GetTimeValue();
            stGvGInfo.stRound.byBigRound = SDAtou(pRecordSet->GetFieldValueByName("BigRound"));
            stGvGInfo.stRound.bySmallRound = SDAtou(pRecordSet->GetFieldValueByName("SmallRound"));
            stGvGInfo.byBattleResult = SDAtou(pRecordSet->GetFieldValueByName("BattleResult"));
            stGvGInfo.byPKVideoCnt   = SDAtou(pRecordSet->GetFieldValueByName("PKVideoCnt"));
               
            DecodeSaveDataFromDB("G1", stGvGInfo.stG1);
            DecodeSaveDataFromDB("G2", stGvGInfo.stG2);
            DecodeSaveDataFromDB("FinalRoundData", stGvGInfo.stFinalRoundInfo);
            
            m_mapVideo[stVideo.stGvGInfo.qwVideoID] = stVideo;
        }

        pRecordSet->Release();
    }	

    return TRUE;
}

BOOL CGvGVideoMgr::LoadPKVideoFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oEscapeBuf;

    CHAR* pszBuff=oAutoBuf.GetSqlBuf();
    CHAR* pszEscapeBuff=oEscapeBuf.GetSqlBuf();

    char buf[256] = {0};
    sprintf(buf, "select * from gvgpkvideo order by VideoID,PKNum asc");

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(buf, &pRecordSet, NULL, &strError);
    if(SDDB_HAS_RECORDSET != nRet && SDDB_NO_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGVideoMgr::LoadPKVideoFromDB execute sql failed! sql = [%s], err = [%s]"), MSG_MARK, buf, strError.c_str());
        return FALSE;
    }

    if(SDDB_HAS_RECORDSET == nRet && pRecordSet)
    {
        VideoID videoID = 0;
        UINT16 wPKNum = 0;

        while(pRecordSet->GetRecord()) // 当有行结果时
        {
            videoID = SDAtou64(pRecordSet->GetFieldValueByName("VideoID"));
            SGvGVideo *pstVideo = GetVideo(videoID);
            if(NULL == pstVideo)
            {
                continue;
            }

            wPKNum = SDAtou(pRecordSet->GetFieldValueByName("PKNum"));
            pstVideo->vecPvPVideo.resize(wPKNum);

            DT_GVG_PK_VIDEO &stPKVideo = pstVideo->vecPvPVideo[wPKNum - 1];
            memset(&stPKVideo, 0, sizeof(stPKVideo));

            DecodeSaveDataFromDB("VideoData", stPKVideo);
        }

        pRecordSet->Release();
    }

    PrintMissingList();
    return TRUE;
}

void CGvGVideoMgr::PrintMissingList()
{
    BOOL bSomeVideoMission = FALSE;
    for(GvGVideoMap::iterator itr = m_mapVideo.begin(); itr != m_mapVideo.end(); ++itr)
    {
        VideoID videoID = itr->first;
        SGvGVideo &stVideo = itr->second;
        if (!stVideo.IsAllPKVideoHere())
        {
            bSomeVideoMission = TRUE;
        }
    }

    if(!bSomeVideoMission)
    {
        return;
    }

    DBG_INFO(_SDT("----------------------print gvgvideo list missing some pkvideo----------------------"));
    UINT32 dwVideoIdx = 0;

    for(GvGVideoMap::iterator itr = m_mapVideo.begin(); itr != m_mapVideo.end(); ++itr)
    {
        VideoID videoID = itr->first;
        SGvGVideo &stVideo = itr->second;
        if (!stVideo.IsAllPKVideoHere())
        {
            DBG_INFO(_SDT("<%u>: video[%-8llu] require %u pkvideo, current pkvideo count = %u"), ++dwVideoIdx, videoID, stVideo.stGvGInfo.byPKVideoCnt, stVideo.vecPvPVideo.size());
        }
    }
}

UINT16 CGvGVideoMgr::CalcHereVideoCnt(VideoIDSet &setVideoID)
{
    UINT16 wHereCnt = 0;
    for(VideoIDSet::iterator itr = setVideoID.begin(); itr != setVideoID.end(); ++itr)
    {
        VideoID videoID = *itr;

        SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(videoID);
        if (NULL == pstVideo)
        {
            continue;
        }

        if (pstVideo->IsAllPKVideoHere())
        {
            wHereCnt++;
        }
    }

    return wHereCnt;
}

BOOL CGvGVideoMgr::SaveToDB()
{
    if(m_setNotSavedVideoID.empty())
    {
        return TRUE;
    }

    for(VideoIDSet::iterator itr = m_setNotSavedVideoID.begin(); itr != m_setNotSavedVideoID.end(); ++itr)
    {
        VideoID videoID = *itr;

        SGvGVideo *pstVideo = GetVideo(videoID);
        if(NULL == pstVideo)
        {
            continue;
        }

        CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_GVG_VIDEO);
        if(NULL == poCMD)
        {
            return FALSE;
        }

        gsapi::GetDBMgr()->AddCommand(poCMD);
        poCMD->SetUserData(pstVideo);
    }

    return TRUE;
}


VOID CGvGVideoMgr::Clear()
{
    if(m_mapVideo.empty())
    {
        return;
    }

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_CLEAR_GVG_VIDEO);
    if(NULL == poCMD)
    {
        return;
    }

    gsapi::GetDBMgr()->AddCommand(poCMD);
    m_mapVideo.clear();
    m_setNotSavedVideoID.clear();
}

void CGvGVideoMgr::OnSaveVideo(BOOL bOK, VideoID videoID)
{
    if (bOK)
    {
        m_setNotSavedVideoID.erase(videoID);
    }
}

void CGvGVideoMgr::AddPKVideo(PKT_PKGS_RET_VIDEO_ACK &stAck)
{
    SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(stAck.qwVideoID);
    if(NULL == pstVideo)
    {
        return;
    }

    if(pstVideo->IsAllPKVideoHere())
    {
        return;
    }

    pstVideo->vecPvPVideo.resize(stAck.byStartSlot + stAck.wPvPVideoCnt);
    for(UINT16 i = 0; i < stAck.wPvPVideoCnt; i++)
    {
        pstVideo->vecPvPVideo[stAck.byStartSlot + i] = stAck.astPvPVideoList[i];
    }

    if(pstVideo->IsAllPKVideoHere())
    {
        USR_INFO(_SDT("CGvGVideoMgr::AddPKVideo[%llu], [%s] vs [%s] %s, round = [%u:%u], all pk video is here, PKVideoCnt = %u"), 
            pstVideo->stGvGInfo.qwVideoID, 
            pstVideo->stGvGInfo.stG1.aszFactionName,
            pstVideo->stGvGInfo.stG2.aszFactionName,
            gvgvideoutil::EchoBattleResult(pstVideo->stGvGInfo.byBattleResult).c_str(),
            pstVideo->stGvGInfo.stRound.byBigRound, pstVideo->stGvGInfo.stRound.bySmallRound,
            pstVideo->stGvGInfo.byPKVideoCnt
        );

        m_setNotSavedVideoID.insert(stAck.qwVideoID);
    }
}

void CGvGVideoMgr::AddVideo(SGvGVideo &stVideo)
{
    if(0 == stVideo.stGvGInfo.qwVideoID)
    {
        return;
    }

    if(GetVideo(stVideo.stGvGInfo.qwVideoID))
    {
        USR_INFO(_SDT("CGvGVideoMgr::AddVideo[%llu], [%s] vs [%s] %s, round = [%u:%u], PKVideoCnt = %u, found exist video, skip."), 
            stVideo.stGvGInfo.qwVideoID,
            stVideo.stGvGInfo.stG1.aszFactionName,
            stVideo.stGvGInfo.stG2.aszFactionName,
            gvgvideoutil::EchoBattleResult(stVideo.stGvGInfo.byBattleResult).c_str(),
            stVideo.stGvGInfo.stRound.byBigRound, stVideo.stGvGInfo.stRound.bySmallRound,
            stVideo.stGvGInfo.byPKVideoCnt
        );

        return;
    }

    m_mapVideo[stVideo.stGvGInfo.qwVideoID] = stVideo;

    if(stVideo.IsAllPKVideoHere())
    {
        USR_INFO(_SDT("CGvGVideoMgr::AddVideo[%llu], [%s] vs [%s] %s, round = [%u:%u], all pk video is here, PKVideoCnt = %u"), 
            stVideo.stGvGInfo.qwVideoID,
            stVideo.stGvGInfo.stG1.aszFactionName,
            stVideo.stGvGInfo.stG2.aszFactionName,
            gvgvideoutil::EchoBattleResult(stVideo.stGvGInfo.byBattleResult).c_str(),
            stVideo.stGvGInfo.stRound.byBigRound, stVideo.stGvGInfo.stRound.bySmallRound,
            stVideo.stGvGInfo.byPKVideoCnt
        );

        m_setNotSavedVideoID.insert(stVideo.stGvGInfo.qwVideoID);
    }
    else
    {
        USR_INFO(_SDT("CGvGVideoMgr::AddVideo[%llu], [%s] vs [%s] %s, round = [%u:%u], not all pk video is here, PKVideoCnt = %u"), 
            stVideo.stGvGInfo.qwVideoID,
            stVideo.stGvGInfo.stG1.aszFactionName,
            stVideo.stGvGInfo.stG2.aszFactionName,
            gvgvideoutil::EchoBattleResult(stVideo.stGvGInfo.byBattleResult).c_str(),
            stVideo.stGvGInfo.stRound.byBigRound, stVideo.stGvGInfo.stRound.bySmallRound,
            stVideo.stGvGInfo.byPKVideoCnt
        );
    }
}

SGvGVideo* CGvGVideoMgr::GetVideo(VideoID videoID)
{
    GvGVideoMap::iterator itr = m_mapVideo.find(videoID);
    if(m_mapVideo.end() == itr)
    {
        return NULL;
    }

    SGvGVideo *pstVideo = &itr->second;
    return pstVideo;
}

VideoID CGvGVideoMgr::MakeVideoID(UINT32 dwFactionID1, UINT32 dwFactionID2)
{
    if(0 == dwFactionID1 || 0 == dwFactionID2)
    {
        return 0;
    }

    UINT64 qwVideo = dwFactionID1;
    qwVideo *= 1000000000;
    qwVideo += dwFactionID2;

    return qwVideo;
}