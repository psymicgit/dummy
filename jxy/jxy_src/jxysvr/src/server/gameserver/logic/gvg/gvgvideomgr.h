///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgvideomgr.h
//< @author: hongkunan
//< @date:   2014年2月11日 21:1:22
//< @brief:  管理门派跨服战的录像
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgvideomgr_h_
#define _gvgvideomgr_h_

#include "gvgdef.h"
#include <sdsingleton.h>

typedef struct tagPKT_PKGS_RET_VIDEO_ACK PKT_PKGS_RET_VIDEO_ACK;

// 门派跨服战对战录像
struct SGvGVideo
{
    SGvGVideo() 
    {
        Clear();
    }

    void Clear()
    {
        memset(&stGvGInfo, 0, sizeof(stGvGInfo));
        vecPvPVideo.clear();
    }

    // 各玩家间的对战录像是否均已到位
    BOOL IsAllPKVideoHere()
    {
        if(0 == stGvGInfo.byPKVideoCnt)
        {
            return TRUE;
        }

        if(stGvGInfo.byPKVideoCnt > vecPvPVideo.size())
        {
            return FALSE;
        }

        for(UINT16 i = 0; i < vecPvPVideo.size(); i++)
        {
            DT_GVG_PK_VIDEO &stPKVideo = vecPvPVideo[i];
            if(0 == stPKVideo.byKillCombo1 + stPKVideo.byKillCombo2)
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    // 门派对战信息
    DT_GVG_VIDEO stGvGInfo;

    // 玩家对战录像列表
    SPvPVideoVec vecPvPVideo;
};

// 管理门派跨服战的录像，这里面的录像每当下一次跨服战开始时都会被全部清除
class CGvGVideoMgr
{
public:
    typedef std::map<VideoID, SGvGVideo> GvGVideoMap;
    typedef std::vector<SGvGVideo> GvGVideoVec;

public:
    DECLARE_SINGLETON(CGvGVideoMgr)

    CGvGVideoMgr()
    {
        m_mapVideo.clear();
        m_setNotSavedVideoID.clear();
    }

public:
    BOOL Init();

    BOOL LoadGvGVideoFromDB();

    BOOL LoadPKVideoFromDB();

    BOOL SaveToDB();

    VOID Clear();

    void OnSaveVideo(BOOL bOK, VideoID);

    void ClearNotSavedList(){ m_setNotSavedVideoID.clear(); }

    void AddVideo(SGvGVideo&);

    void AddPKVideo(PKT_PKGS_RET_VIDEO_ACK&);

    SGvGVideo* GetVideo(VideoID);

    GvGVideoMap& GetVideos(){ return m_mapVideo; }

    VideoIDSet& GetNotSaveVideoIDList(){ return m_setNotSavedVideoID; }

    VideoID MakeVideoID(UINT32 dwFactionID1, UINT32 dwFactionID2);

    BOOL IsEmpty(){ return m_mapVideo.empty(); }

    // 打印缺失玩家战斗录像的列表
    void PrintMissingList();

    UINT16 CalcHereVideoCnt(VideoIDSet&);

private:
    // 跨服战录像表，每个录像用一个id来标识
    GvGVideoMap m_mapVideo;

    // 未被存入数据库的录像列表
    VideoIDSet m_setNotSavedVideoID;
};

#endif //_gvgvideomgr_h_