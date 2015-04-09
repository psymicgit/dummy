
#include "score.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
using namespace SGDP;

IMPLEMENT_CLASS_POOL_MGR(CScore);
CScore::CScore()
{
    memset(&m_stScoreData, 0x00, sizeof(m_stScoreData));
}


CScore::~CScore()
{

}

BOOL CScore::Init(DT_SCORE_DATA_LST &stScoreData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
    memcpy(&m_stScoreData, &stScoreData, sizeof(DT_SCORE_DATA_LST));
    return TRUE;
}

BOOL CScore::IsRecvedChapter(UINT16 wSceneidx, UINT16 wChapterIdx)
{
    for (UINT8 byChapterNum = 0; byChapterNum < m_stScoreData.byChapterNum; byChapterNum++)
    {
        if (m_stScoreData.astChapterData[byChapterNum].wSceneidx == wSceneidx &&
                m_stScoreData.astChapterData[byChapterNum].wChapterIdx == wChapterIdx)
        {
            if (m_stScoreData.astChapterData[byChapterNum].qwtime > 0)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }

        }
    }
    return FALSE;
}

BOOL CScore::IsRecvedAccumulated(UINT16 wgrade)
{
    for (UINT8 byAccumulatedNum = 0; byAccumulatedNum < m_stScoreData.byAccumulatedNum; byAccumulatedNum++)
    {
        if (m_stScoreData.astAccumulatedData[byAccumulatedNum].wgrade == wgrade)
        {
            if (m_stScoreData.astAccumulatedData[byAccumulatedNum].qwtime > 0)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }

        }
    }
    return FALSE;
}

VOID CScore::AddRecvedChapter(UINT16 wSceneidx, UINT16 wChapterIdx)
{
    for (UINT8 byChapterNum = 0; byChapterNum < m_stScoreData.byChapterNum; byChapterNum++)
    {
        if (m_stScoreData.astChapterData[byChapterNum].wSceneidx == wSceneidx &&
                m_stScoreData.astChapterData[byChapterNum].wChapterIdx == wChapterIdx)
        {
            m_stScoreData.astChapterData[byChapterNum].qwtime = SDTimeSecs();
            return;
        }
    }
    DT_CHAPTER_SCORE_DATA &astChapterData = m_stScoreData.astChapterData[m_stScoreData.byChapterNum++];
    astChapterData.wSceneidx = wSceneidx;
    astChapterData.wChapterIdx = wChapterIdx;
    astChapterData.qwtime = SDTimeSecs();
}

VOID CScore::AddRecvedAccumulated(UINT16 wGrade)
{
    for (UINT8 byAccumulatedNum = 0; byAccumulatedNum < m_stScoreData.byAccumulatedNum; byAccumulatedNum++)
    {
        if (m_stScoreData.astAccumulatedData[byAccumulatedNum].wgrade == wGrade)
        {
            m_stScoreData.astAccumulatedData[byAccumulatedNum].qwtime = SDTimeSecs();
            return;
        }
    }
    DT_ACCUMULATED_SCORE_DATA &stAccumulatedData = m_stScoreData.astAccumulatedData[m_stScoreData.byAccumulatedNum++];
    stAccumulatedData.wgrade = wGrade;
    stAccumulatedData.qwtime = SDTimeSecs();
}


