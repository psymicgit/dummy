#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "scorepropmgr.h"
#include <logic/base/basepropmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

#include <db/dbmgr.h>

#define SQL_SCORE_STRING				"select * from scoreencourage order by sceneidx, chapteridx"
#define SQL_SCORE_ACCUMULATED_STRING	"select * from scoretotalencourage order by sceneidx, grade"



IMPLEMENT_SINGLETON(CScorePropMgr);

CScorePropMgr::CScorePropMgr()
{

}

CScorePropMgr::~CScorePropMgr()
{

}

BOOL CScorePropMgr::Init()
{
    return LoadFromDB();
}

VOID CScorePropMgr::UnInit()
{
    m_mapScoreProp.clear();
    m_mapScoreAccumulateProp.clear();
}


// 从数据源获取数据
BOOL CScorePropMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("scoreencourage", " order by sceneidx, chapteridx").c_str(), &pRes, &dwErrID,  &strErr);
    if (SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    //查询的表结果
    while(SDDB_HAS_RECORDSET == nRet && pRes->GetRecord())// 当有行结果时
    {
        UINT16 wSceneIdx		= SGDP::SDAtou(pRes->GetFieldValueByName("sceneidx"));
        UINT16 wChapterIdx	= SGDP::SDAtou(pRes->GetFieldValueByName("chapteridx"));
        UINT16 wScore			= SGDP::SDAtou(pRes->GetFieldValueByName("score"));
        CScorePropMapItr itr = m_mapScoreProp.find(CLogicFunc::Get1616Key(wSceneIdx, wChapterIdx));
        if (itr != m_mapScoreProp.end())
        {
            SScoreProp &stScoreProp = itr->second;
            stScoreProp.astValue[stScoreProp.byNum].byType	= SGDP::SDAtou(pRes->GetFieldValueByName("type"));
            stScoreProp.astValue[stScoreProp.byNum].dwValue	= SGDP::SDAtou(pRes->GetFieldValueByName("value"));
            stScoreProp.astValue[stScoreProp.byNum].byNum	= SGDP::SDAtou(pRes->GetFieldValueByName("num"));
            stScoreProp.byNum++;
        }
        else
        {
            SScoreProp stScoreProp;
            stScoreProp.Init();
            stScoreProp.wSceneIdx		=	wSceneIdx;
            stScoreProp.wChapterIdx	=	wChapterIdx;
            stScoreProp.wScore		=	wScore;

            CValue &stValue	= stScoreProp.astValue[stScoreProp.byNum++];
            stValue.byType	= SGDP::SDAtou(pRes->GetFieldValueByName("type"));
            stValue.dwValue	= SGDP::SDAtou(pRes->GetFieldValueByName("value"));
            stValue.byNum		= SGDP::SDAtou(pRes->GetFieldValueByName("num"));
            m_mapScoreProp[CLogicFunc::Get1616Key(wSceneIdx, wChapterIdx)] = stScoreProp;
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    //////////////////////////////////////////////////////////////////////////
    nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("scoretotalencourage", " order by sceneidx, grade").c_str(), &pRes, &dwErrID,  &strErr);
    if (SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    //查询的表结果
    while(SDDB_HAS_RECORDSET == nRet && pRes->GetRecord())// 当有行结果时
    {
        UINT16 wGrade			= SGDP::SDAtou(pRes->GetFieldValueByName("grade"));
        UINT16 wScore			= SGDP::SDAtou(pRes->GetFieldValueByName("score"));
        CScoreAccumulatePropMapItr itr = m_mapScoreAccumulateProp.find(wGrade);
        if (itr != m_mapScoreAccumulateProp.end())
        {
            CScoreAccumulateProp &stScoreAccumulateProp = itr->second;
            CValue	&stValue = stScoreAccumulateProp.astValue[stScoreAccumulateProp.byNum++];
            stValue.byType	= SGDP::SDAtou(pRes->GetFieldValueByName("type"));
            stValue.dwValue	= SGDP::SDAtou(pRes->GetFieldValueByName("value"));
            stValue.byNum	= SGDP::SDAtou(pRes->GetFieldValueByName("num"));
        }
        else
        {
            CScoreAccumulateProp stScoreAccumulateProp;
            stScoreAccumulateProp.Init();
            stScoreAccumulateProp.wGrade	= wGrade;
            stScoreAccumulateProp.wScore	= wScore;

            CValue	&stValue = stScoreAccumulateProp.astValue[stScoreAccumulateProp.byNum++];
            stValue.byType	= SGDP::SDAtou(pRes->GetFieldValueByName("type"));
            stValue.dwValue	= SGDP::SDAtou(pRes->GetFieldValueByName("value"));
            stValue.byNum		= SGDP::SDAtou(pRes->GetFieldValueByName("num"));
            m_mapScoreAccumulateProp[wGrade] = stScoreAccumulateProp;
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}



