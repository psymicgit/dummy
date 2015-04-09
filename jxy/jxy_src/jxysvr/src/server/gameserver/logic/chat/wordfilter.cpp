
#include "wordfilter.h"
#include <string.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <framework/gsapi.h>
#include <sdloggerimpl.h>

// KMP Next values generation function
__inline void	KMP_GetNext(TCHAR * sString, int * iNext)
{
    iNext[0] = -1;
    int i = -1;
    for(int j = 1; sString[j] != 0; j++)
    {
        while(sString[j] != sString[i + 1] && (i >= 0))
            i = iNext[i];
        if(sString[j] == sString[i + 1])
            iNext[j] = i + 1;
        else
            iNext[j] = -1;
        i = iNext[j];
    }
}

// KMP Match function
__inline bool	KMP_Match(const TCHAR * sString, TCHAR * sSubString, int * iNext)
{
    int i = 0, j = 0;

    string strWords = sString;
    //全转成小写再匹配
    string strTmp = SDStrlwr(strWords);
    CHAR* pszTmp = (CHAR*)(strTmp.c_str());
    int len = strlen(pszTmp);
    while (i < len && sSubString[j] != 0)
    {
        if (pszTmp[i] == sSubString[j])
        {
            ++j;
            ++i;
        }
        else
        {
            if(j == 0)
                ++i;
            j = (j > 0) ? (iNext[j - 1] + 1) : 0;
        }
    }

    return (sSubString[j] == 0);
}
IMPLEMENT_SINGLETON(CWordFilter);


BOOL	CWordFilter::Init()
{
    if(!InitWordFliter())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitWordFliter failed!"), MSG_MARK);
        return TRUE;
    }

    if(!InitDspNameWordFliter())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitWordFliter failed!"), MSG_MARK);
        return TRUE;
    }

    return TRUE;
}


BOOL CWordFilter::InitWordFliter()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from  wordfilter;", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    int nLen = 0;
    TWordFilterData stWordFilterData;
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        stWordFilterData.Init();
        nLen = pRes->GetFieldLengthByName("Words");
        nLen = nLen >= MAX_WORD_FILTER ? MAX_WORD_FILTER - 1 : nLen;

        string	strWords = pRes->GetFieldValueByName("Words");
        string	strTmp = SDStrlwr(strWords);

        _SDTStrncpy(stWordFilterData.m_szWordFilters, strTmp.c_str(), nLen);
        //_SDTStrncpy(stWordFilterData.m_szWordFilters, pRecordSet->GetFieldValueByName("Words"), nLen);
        string strkey = stWordFilterData.m_szWordFilters;
        if(m_mapWordFilterData.find(strkey) == m_mapWordFilterData.end())
        {
            m_mapWordFilterData[strkey] = stWordFilterData;
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    CWordFilterDataMapItr itr;
    for(itr = m_mapWordFilterData.begin(); itr != m_mapWordFilterData.end(); itr++)
    {
        KMP_GetNext(itr->second.m_szWordFilters, itr->second.m_kmpvalue);	// 得到一个与内容有关的数值m_kmpvalue[i]
    }

    return TRUE;
}

BOOL CWordFilter::InitDspNameWordFliter()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from  dspnamefilter;", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return TRUE;
    }

    int nLen = 0;
    TWordFilterData stWordFilterData;
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        stWordFilterData.Init();
        nLen = pRes->GetFieldLengthByName("Words");
        nLen = nLen >= MAX_WORD_FILTER ? MAX_WORD_FILTER - 1 : nLen;
        _SDTStrncpy(stWordFilterData.m_szWordFilters, pRes->GetFieldValueByName("Words"), nLen);
        string strkey = stWordFilterData.m_szWordFilters;
        if(m_mapDspNameWordFilterData.find(strkey) == m_mapDspNameWordFilterData.end())
        {
            m_mapDspNameWordFilterData[strkey] = stWordFilterData;
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    CWordFilterDataMapItr itr;
    for(itr = m_mapDspNameWordFilterData.begin(); itr != m_mapDspNameWordFilterData.end(); itr++)
    {
        KMP_GetNext(itr->second.m_szWordFilters, itr->second.m_kmpvalue);	// 得到一个与内容有关的数值m_kmpvalue[i]
    }

    return TRUE;
}


TCHAR*	CWordFilter::VerifyString(const TCHAR * sString)	// 验证字符是否合法
{
    CWordFilterDataMapItr itr;
    for(itr = m_mapWordFilterData.begin(); itr != m_mapWordFilterData.end(); itr++)
    {
        if(KMP_Match(sString, itr->second.m_szWordFilters, itr->second.m_kmpvalue))
        {
            return itr->second.m_szWordFilters;	// 关键词命中了，被fliter了
        }
    }
    return NULL;
}

bool	CWordFilter::VerifyDspNameString(const TCHAR * sString)	// 验证字符是否合法
{
    CWordFilterDataMapItr itr;
    for(itr = m_mapWordFilterData.begin(); itr != m_mapWordFilterData.end(); itr++)
    {
        if(KMP_Match(sString, itr->second.m_szWordFilters, itr->second.m_kmpvalue))
        {
            return false;	// 关键词命中了，被fliter了
        }
    }

    for(itr = m_mapDspNameWordFilterData.begin(); itr != m_mapDspNameWordFilterData.end(); itr++)
    {
        if(KMP_Match(sString, itr->second.m_szWordFilters, itr->second.m_kmpvalue))
        {
            return false;	// 关键词命中了，被fliter了
        }
    }

    return true;
}


void CWordFilter::Free()	//
{

}



VOID CWordFilter::SetWordFilterData(CWordFilterDataMap &mapWordFilterData, CWordFilterDataMap &mapDspNameWordFilterData)
{
    m_mapWordFilterData.clear();
    m_mapWordFilterData = mapWordFilterData;

    m_mapDspNameWordFilterData.clear();
    m_mapDspNameWordFilterData = mapDspNameWordFilterData;
}