#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include "errmsgmgr.h"

#include <db/dbmgr.h>
#include <framework/gsapi.h>

IMPLEMENT_SINGLETON_PROPMGR(CMsgDefMgr);

CMsgDefMgr::CMsgDefMgr()
{
    m_byCliRegionType = 0;
}

CMsgDefMgr::~CMsgDefMgr()
{
}

BOOL CMsgDefMgr::Init()
{
    return LoadFromDB();
}

string GetKey(UINT8 byCliRegionType, string strErrKey)
{
    CHAR szBuff[128] = {0};
    sprintf(szBuff, "%u_%s", byCliRegionType, strErrKey.c_str());
    return szBuff;
}


BOOL CMsgDefMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("msgdefprop", " order by CliRegionType asc;").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        return TRUE;
    }
    while(pRes->GetRecord())// 当有行结果时
    {
        SRegionErrMsgProp stProp;
        stProp.byCliRegionType = SDAtou(pRes->GetFieldValueByName("CliRegionType"));
        //stProp.wProtocolID = SDAtou(pRes->GetFieldValueByName("ProtocolID"));
        //if(0 == stProp.wProtocolID) //ProtocolID为0为表示当前使用的CliRegionType
        stProp.strErrKey = pRes->GetFieldValueByName("MsgKey");
        if(stProp.strErrKey == "0000")//为"0000"为表示当前使用的CliRegionType
        {
            m_byCliRegionType = stProp.byCliRegionType;
            continue;
        }
        string strMsg = pRes->GetFieldValueByName("Msg");
        stProp.strMsg = strMsg;

        std::string::size_type pos0 = 0;
        std::string::size_type pos1 = 0;
        pos0 = strMsg.find("{");
        while (pos0 != std::string::npos)
        {
            pos1 = strMsg.find("}", pos0);
            if(pos1 == std::string::npos)
            {
                SYS_CRITICAL(_SDT("[%s:%d]: ErrMsg[%s] format invalid, } not match!\n"), MSG_MARK, strMsg.c_str());
                return FALSE;
            }
            stProp.vecErrMsg.push_back(strMsg.substr(pos0, pos1 - pos0 + 1));
            pos0 = strMsg.find("{", pos1);
        }
        m_mapID2RegionErrMsgProp[GetKey(stProp.byCliRegionType, stProp.strErrKey)] = stProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

VOID CMsgDefMgr::UnInit()
{
    m_mapID2RegionErrMsgProp.clear();
}



string CMsgDefMgr::GetErrMsg(string strErrKey, vector<string>* pvecFillMsg)
{
    CID2RegionErrMsgPropMapItr itr = m_mapID2RegionErrMsgProp.find(GetKey(m_byCliRegionType, strErrKey));
    if(itr == m_mapID2RegionErrMsgProp.end())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CliRegionType[%u], ErrKey[%s] not found!"), MSG_MARK, m_byCliRegionType, strErrKey.c_str());
        return "Other error!";
    }

    SRegionErrMsgProp& stProp = itr->second;
    vector<string> vecErrMsg = stProp.vecErrMsg;
    string strErrMsg = stProp.strMsg;
    if (NULL == pvecFillMsg)
    {
        return strErrMsg;
    }
    size_t dwMsgSize = vecErrMsg.size();
    size_t dwFillSize = pvecFillMsg->size();
    if ( 0 == dwMsgSize )
    {
        return strErrMsg;
    }

    if (dwMsgSize != dwFillSize)
    {
        return string("Fill Msg Error!");
    }

    size_t pos = 0;
    while (vecErrMsg.size())
    {
        pos = strErrMsg.find(vecErrMsg.back());
        strErrMsg.replace(pos, vecErrMsg.back().size(), pvecFillMsg->back());
        vecErrMsg.pop_back();
        pvecFillMsg->pop_back();
    }
    return strErrMsg;

}


