#ifndef _SCORE_H_
#define _SCORE_H_


#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/server/utility.h>

using namespace SGDP;

class CPlayer;


class CScore: public CBaseObj
{
public:
    CScore();
    ~CScore();

public:

    //初始化相关对象
    BOOL Init(DT_SCORE_DATA_LST &stScoreData, CPlayer* poOwner);

    DT_SCORE_DATA_LST& GetDT_SCORE_DATA_LST()
    {
        return m_stScoreData;
    };

    BOOL IsRecvedChapter(UINT16 wSceneidx, UINT16 wChapterIdx);

    BOOL IsRecvedAccumulated(UINT16 wgrade);

	VOID AddRecvedChapter(UINT16 wSceneidx, UINT16 wChapterIdx);

	VOID AddRecvedAccumulated(UINT16 wGrade);

private:
    DT_SCORE_DATA_LST			m_stScoreData;//
};
DECLARE_CLASS_POOL_MGR(CScore);
#endif
