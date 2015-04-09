

#include "rechargehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include <db/autosqlbuf.h>
#include "common/client/errdef.h"
#include "urlpropmgr.h"
#include "db/setretreatepayprocessed.h"
#include "net/db/dbpktbuilder.h"
#include <net/recharge/rcclient.h>
#include <framework/gsapi.h>
#include "common/client/errdef.h"
#include "common/server/utility.h"





CRechargeHttpCMD::CRechargeHttpCMD()
{
    m_strUrl = CUrlPropMgr::Instance()->GetUrlRetreate();
}

CRechargeHttpCMD::~CRechargeHttpCMD()
{

}

BOOL CRechargeHttpCMD::ProcessResult(Json::Value &datas)
{
    if (0 == datas.size())
    {
        return FALSE;
    }

    vector<SRecreatePayProcessed> vecInfo;

    for(UINT32 i = 0; i < datas.size(); i++)
    {
        Json::Value chargeInfo = datas[i];
        SRecreatePayProcessed stInfo;

        if (chargeInfo["playerid"].isInt())
        {
            stInfo.dwPlayerID =  chargeInfo["playerid"].asInt();
        }

        if (chargeInfo["orderid"].isString())
        {
            stInfo.strOrderID =  chargeInfo["orderid"].asString();
        }

        if (chargeInfo["zoneid"].isInt())
        {
            stInfo.wZoneID =  chargeInfo["zoneid"].asInt();
        }

        if (chargeInfo["rmb"].isInt())
        {
            stInfo.dwRMB =  chargeInfo["rmb"].asInt();
        }

        if (chargeInfo["gold"].isInt())
        {
            stInfo.dwGold =  chargeInfo["gold"].asInt();
        }

        if (chargeInfo["comment"].isString())
        {
            stInfo.strComment =  chargeInfo["comment"].asString();
        }


        if (chargeInfo["sandbox"].isInt())
        {
            stInfo.bySandBoxFlag =  chargeInfo["sandbox"].asInt();
        }

        if (chargeInfo["viplevel"].isInt())
        {
            stInfo.byVipLevel =  chargeInfo["viplevel"].asInt();
        }

        if (chargeInfo["payid"].isInt())
        {
            stInfo.dwPayID =  chargeInfo["payid"].asInt();
        }

        if(NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stInfo.dwPlayerID))
        {
            return FALSE;
        }

        vecInfo.push_back(stInfo);
    }


    if (vecInfo.size() == 0)
    {
        CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_dwTransID);

        if (poClinet != NULL)
        {
            poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_IS_NULL).c_str());
        }

        return FALSE;
    }

    CDBPktBuilder::Instance()->OnProcessRechargeOrderReq(vecInfo, m_dwTransID);

    return TRUE;
}

BOOL CRechargeHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("zoneid=");
    strTemp = stRechargeInfo.ZoneID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&index=");
    strTemp = stRechargeInfo.IndexMax;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    //地址
    string	strTempUrl = m_strUrl;
    UINT8	abyDigest[16] = {0};
    string	strInput = m_strfields;
    strInput.append(CENTER_SIGN);
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());
    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    strTempUrl.append("?_sig=");
    strTemp = szHexDigest;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    strTempUrl.append(efc);
    curl_free(efc);

    curl_easy_setopt(m_pEasyHandle, CURLOPT_URL, strTempUrl.c_str());
    curl_easy_setopt(m_pEasyHandle, CURLOPT_ERRORBUFFER, m_szErrorBuff);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POST, 1 );
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POSTFIELDS, m_strfields.c_str());
    return TRUE;
}

BOOL CRechargeHttpCMD::OnExecuted()
{
    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_dwTransID);

    if (poClinet == NULL)
    {
        return FALSE;
    }

    if (FALSE == m_bError)
    {
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_GET_ERR).c_str());
        return FALSE;
    }

    Json::Value root;
    Json::Reader reader;

    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_GET_ERR).c_str());
        return FALSE;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_GET_ERR).c_str());
        return FALSE;
    }

    //解析返回的状态码
    INT32 nErrCode = 0;

    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_GET_ERR).c_str());
        return FALSE;
    }
    nErrCode = root["errcode"].asInt();
    if (0 != nErrCode)
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_IS_NULL).c_str());
        return FALSE;
    }


    Json::Value datas = root["datas"];

    if (!datas.isArray())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_GET_ERR).c_str());
        return FALSE;
    }

    this->ProcessResult(datas);

    return TRUE;
}

VOID CRechargeHttpCMD::SetUserData(VOID* pRechargeInfo, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    stRechargeInfo = *((SRechargeInfo*)pRechargeInfo);
}