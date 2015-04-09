
#ifndef _SCOREPROM_H_
#define _SCOREPROM_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/logicfunc.h>

//名称最大长度
#define MAX_ITEM_VALUE_NUM	32

typedef struct tagValue
{
    UINT8	byType;
    UINT32	dwValue;
    UINT8	byNum;
} CValue;

typedef struct tagScoreProp
{
    UINT16	wSceneIdx;
    UINT16	wChapterIdx;
    UINT16	wScore;
    UINT8	byNum;
    CValue	astValue[MAX_ITEM_VALUE_NUM];

    tagScoreProp()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(tagScoreProp));
    }
} SScoreProp;


typedef struct tagScoreAccumulateProp
{
    UINT16	wGrade;
    UINT16	wScore;
    UINT8	byNum;
    CValue	astValue[MAX_ITEM_VALUE_NUM];

    tagScoreAccumulateProp()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(tagScoreAccumulateProp));
    }
} CScoreAccumulateProp;


typedef std::map<UINT32, SScoreProp> CScorePropMap; //key为Level
typedef CScorePropMap::iterator CScorePropMapItr;

typedef std::map<UINT16, CScoreAccumulateProp> CScoreAccumulatePropMap; //key为Level
typedef CScoreAccumulatePropMap::iterator CScoreAccumulatePropMapItr;


class CScorePropMgr
{
public:
    DECLARE_SINGLETON(CScorePropMgr);

public:

    BOOL Init();

    VOID UnInit();
public:
    CScorePropMap & GetScorePropMap()
    {
        return m_mapScoreProp;
    }

	CScoreAccumulatePropMap & GetScoreAccumulatePropMap()
	{
		return m_mapScoreAccumulateProp;
	}

    SScoreProp * GetScoreProp(UINT16 wSceneIdx, UINT16 wChapterIdx)
    {
        CScorePropMapItr itr = m_mapScoreProp.find(CLogicFunc::Get1616Key(wSceneIdx, wChapterIdx));
        if (itr != m_mapScoreProp.end())
        {
            return &itr->second;
        }
        return NULL;
    }

    CScoreAccumulateProp * GetScoreAccumulateProp(UINT16 wGrade)
    {
        CScoreAccumulatePropMapItr itr = m_mapScoreAccumulateProp.find(wGrade);
        if (itr != m_mapScoreAccumulateProp.end())
        {
            return &itr->second;
        }
        return NULL;
    }

protected:
    CScorePropMgr();
    virtual ~CScorePropMgr();
protected:

    BOOL LoadFromDB();

private:
    CScorePropMap				m_mapScoreProp;
    CScoreAccumulatePropMap	m_mapScoreAccumulateProp;
};




#endif //#ifndef _ROLEMGR_H_

