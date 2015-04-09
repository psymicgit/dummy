#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/build/buildmgr.h>
#include "scienceupdatepropmgr.h"
#include <logic/other/singleprammgr.h>
#include <sdframework/sdapplication.h>
#include <framework/gsapi.h>
#include "common/server/ngstring.h"

#include <db/dbmgr.h>

using namespace SGDP;

#define SQL_READ_SCIENCE_UNLOCK_STRING "select * from scienceunlock"
#define SQL_READ_SCIENCE_UPDATE_STRING "select * from scienceattrprop"

IMPLEMENT_SINGLETON_PROPMGR(CScienceUpdatePropMgr);

CScienceUpdatePropMgr::CScienceUpdatePropMgr()
{

}

CScienceUpdatePropMgr::~CScienceUpdatePropMgr()
{

}

BOOL CScienceUpdatePropMgr::Init()
{
    if(!LoadFromDBEx())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CScienceUpdatePropMgr::UnInit()
{
    m_mapUpdateProp.clear();
    m_vecSortScienceList.clear();
}

CScienceUpdatePropMgr::EGetScienceInfoError CScienceUpdatePropMgr::GetInfo( UINT8 byKind, UINT16 wLevel, UINT32 &dwCostVaue, UINT32 &dwAddValue )
{
    CSciencePropMap::iterator itr = m_mapUpdateProp.find( byKind );

    if( m_mapUpdateProp.end() == itr )
    {
        USR_INFO( "can't find science type:%d", byKind );
        return GUIE_CANNOT_FIND_SCIENCE;
    }

    if( wLevel > CSinglePramMgr::Instance()->GetMaxScienceLevel() )
    {
        DBG_INFO( "player update science level bigger than config,please check config, type:%d, level:%d", byKind, wLevel );
        return GUIE_AT_MAXMIUM_LEVEL;
    }

    if( wLevel == 0 )
    {
        //假如是0级的话，那么直接赋值0
        dwCostVaue = 0;
        dwAddValue = 0;
        return GUIE_SUCCESS;
    }

    dwCostVaue = itr->second.vecScienceUpdateData[wLevel - 1].dwCost;
    dwAddValue = itr->second.vecScienceUpdateData[wLevel - 1].dwAddValue;
    return GUIE_SUCCESS;
}

CScienceUpdatePropMgr::EGetScienceInfoError CScienceUpdatePropMgr::GetInfo( UINT8 byKind, UINT16 wLevel, UINT32 &dwCostVaue, UINT32 &dwAddValue, UINT16 &wUnlockLevel )
{
    INT32 nRet = GetInfo( byKind, wLevel, dwCostVaue, dwAddValue );
    if( GUIE_SUCCESS != nRet )
    {
        return (EGetScienceInfoError)nRet;
    }

    CSciencePropMap::iterator itr = m_mapUpdateProp.find( byKind );
    if( itr == m_mapUpdateProp.end() )
    {
        return GUIE_CANNOT_FIND_SCIENCE;
    }
    wUnlockLevel = itr->second.wUnlockLevel;

    return GUIE_SUCCESS;
}

INT32 CScienceUpdatePropMgr::GetUpdateNeedLevel( UINT8 byKind, UINT16 wSicenceLevel )
{
    CSciencePropMap::iterator itr = m_mapUpdateProp.find( byKind );
    if( itr == m_mapUpdateProp.end() )
    {
        USR_INFO( _SDT( "get update attr:%d need level error" ), byKind );
        //LogAllKind();
        return 0;
    }

    if( wSicenceLevel > itr->second.vecScienceUpdateData.size())
    {
        USR_INFO( _SDT( "science level too bigger, level:%d max level:%d science:%d" ), wSicenceLevel,
                  itr->second.vecScienceUpdateData.size(), itr->first );
        return 0;
    }

    if( wSicenceLevel < 1 )
    {
        return 0;
    }

    return itr->second.vecScienceUpdateData[wSicenceLevel - 1].wNeedPlayerLevel;
}

BOOL CScienceUpdatePropMgr::IsCanUpdateAttr( UINT64 qwCurStory, UINT16 wUpdateScienceLevel, UINT16 wPlayerLevel )
{
    CSciencePropMap::iterator itr = m_mapUpdateProp.begin();
    for( ; itr != m_mapUpdateProp.end(); ++itr )
    {
        if( !IsCanUpdateAttr( qwCurStory, wUpdateScienceLevel, wPlayerLevel, itr->first ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

vector<UINT8> CScienceUpdatePropMgr::GetSortScienceKindList()
{
    return m_vecSortScienceList;
}

BOOL CScienceUpdatePropMgr::IsCanUpdateAttr( UINT64 qwCurStory, UINT16 wUpdateScienceLevel, UINT16 wPlayerLevel, UINT8 byType )
{
    if( wUpdateScienceLevel == 0 )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: UpdateScienceLevel 0" ), MSG_MARK );
        return TRUE;
    }

    CSciencePropMap::iterator itr = m_mapUpdateProp.find( byType );
    if( itr == m_mapUpdateProp.end() )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: get attr:%d need level error" ), MSG_MARK, byType );
        //LogAllKind();
        return FALSE;
    }

    if( wUpdateScienceLevel >= CSinglePramMgr::Instance()->GetMaxScienceLevel())
    {
        return FALSE;
    }

    if( itr->second.wUnlockLevel > wPlayerLevel )
    {
        return FALSE;
    }

    if( itr->second.vecScienceUpdateData[wUpdateScienceLevel - 1].wNeedPlayerLevel > wPlayerLevel )
    {
        return FALSE;
    }

    if( itr->second.vecScienceUpdateData[wUpdateScienceLevel - 1].dwCost > qwCurStory )
    {
        return FALSE;
    }

    return TRUE;
}

VOID CScienceUpdatePropMgr::LogAllKind()
{
    CSciencePropMap::iterator itr = m_mapUpdateProp.begin();

    CNGString strlog;
    strlog += "all science：";
    for( ; itr != m_mapUpdateProp.end(); ++itr, strlog  += "," )
    {
        strlog += itr->first;
    }
    DBG_INFO( strlog.c_str() );
}

vector<UINT8> CScienceUpdatePropMgr::GetUnlockKindListAtLevel( UINT16 wPlayerLevel )
{
    vector<UINT8> vecKindList;
    CSciencePropMap::iterator itr = m_mapUpdateProp.begin();
    for( ; itr != m_mapUpdateProp.end(); ++itr )
    {
        if( itr->second.wUnlockLevel == wPlayerLevel )
        {
            vecKindList.push_back( itr->first );
        }
    }

    return vecKindList;
}

UINT16 CScienceUpdatePropMgr::GetMaxLevel( UINT8 byKind )
{
    CSciencePropMap::iterator itr = m_mapUpdateProp.find( byKind );
    if( itr == m_mapUpdateProp.end() )
    {
        return 0;
    }
    else
    {
        return itr->second.vecScienceUpdateData.size();
    }
}

CScienceUpdatePropMgr::EGetScienceInfoError CScienceUpdatePropMgr::GetUnlockInfo( UINT8 byKind, UINT16 wLevel, BOOL &bVisible, BOOL &bUnlock )
{
    bVisible = FALSE;
    bUnlock = FALSE;

    CSciencePropMap::iterator itr = m_mapUpdateProp.find( byKind );

    if( m_mapUpdateProp.end() == itr )
    {
        return GUIE_CANNOT_FIND_SCIENCE;
    }

    if( wLevel >= itr->second.wUnlockLevel )
    {
        bUnlock = TRUE;
    }
    else
    {
        bUnlock = FALSE;
    }

    if( wLevel >= itr->second.wVisibleLevel )
    {
        bVisible = TRUE;
    }
    else
    {
        bVisible = FALSE;
    }

    return GUIE_SUCCESS;
}

INT32 CScienceUpdatePropMgr::GetKindCount()
{
    return m_mapUpdateProp.size();
}

BOOL CScienceUpdatePropMgr::LoadFromDBEx()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("scienceunlock").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT8 byKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        if(0 == byKindID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: KindID[0] invalid!"), MSG_MARK);
            continue;
        }
        m_mapUpdateProp[byKindID].wUnlockLevel = SGDP::SDAtou( pRes->GetFieldValueByName( "UnlockLevel" ));
        m_mapUpdateProp[byKindID].wVisibleLevel = SGDP::SDAtou( pRes->GetFieldValueByName( "VisibleLevel" ));
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    //读取不同类型各个级别对应的内容
    nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("scienceattrprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while( pRes->GetRecord() )
    {
        UINT8 byKindID = SGDP::SDAtou(pRes->GetFieldValueByName("AttrKindID"));
        if(0 == byKindID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: KindID[0] invalid!"), MSG_MARK);
            return FALSE;
        }

        SScienceUpdateData data;
        data.dwAddValue = SGDP::SDAtou(pRes->GetFieldValueByName("AttrValue"));
        data.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("AttrLevel"));
        data.dwCost = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeSciencePoint"));
        data.wNeedPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedPlayerLevel"));
        m_mapUpdateProp[byKindID].vecScienceUpdateData.push_back( data );
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    CSciencePropMap::iterator itr = m_mapUpdateProp.begin();
    while(itr != m_mapUpdateProp.end())
    {
        m_vecSortScienceList.push_back( itr->first );
        ++itr;
    }

    std::sort( m_vecSortScienceList.begin(), m_vecSortScienceList.end(), SortByUnlockLevel );

    return TRUE;
}

bool CScienceUpdatePropMgr::SortByUnlockLevel( UINT8 kind1, UINT8 kind2 )
{
    return Instance()->m_mapUpdateProp.find(kind1)->second.wUnlockLevel < Instance()->m_mapUpdateProp.find(kind2)->second.wUnlockLevel;
}