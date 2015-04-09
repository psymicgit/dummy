

#include "getconfigshttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include <db/autosqlbuf.h>
#include "common/client/errdef.h"
#include "urlpropmgr.h"
#include "logic/other/commoninfomgr.h"
#include "framework/gsconfig.h"
#include "logic/vip/vippropmgr.h"

bool my_sort(const SBuyGoldProp & m1, const SBuyGoldProp & m2)
{
    return m1.dwGold < m2.dwGold;
}




CGetConfigsHttpCMD::CGetConfigsHttpCMD()
{
    m_strUrl = CUrlPropMgr::Instance()->GetUrlBuyGoldInfo();
}

CGetConfigsHttpCMD::~CGetConfigsHttpCMD()
{

}

VOID CGetConfigsHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    stGetConfigs = *((SGetConfigs*)pUserData);
}

BOOL CGetConfigsHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("zoneid=");
    strTemp = stGetConfigs.wZoneID;
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

VOID CGetConfigsHttpCMD::OnSyncExecute()
{
    CURLcode		code;
    curl_easy_setopt(m_pEasyHandle, CURLOPT_WRITEFUNCTION, OnRecv);
    code = curl_easy_perform(m_pEasyHandle);
    if(code == CURLE_OK)
    {
        m_bError = TRUE;
    }
}

size_t CGetConfigsHttpCMD::OnRecv(void *buffer, size_t size, size_t count, void *pData)
{
    if( NULL == pData )
    {
        return 0;
    }
    CGetConfigsHttpCMD *poCmd = (CGetConfigsHttpCMD*)pData;
    poCmd->m_strResult.append((char *)buffer, size * count );
    return size * count;
}



BOOL CGetConfigsHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        return FALSE;
    }

    DBG_INFO( _SDT( "json result:%s" ), m_strResult.c_str() );
    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    //解析返回的状态码
    INT32 nErrCode = 0;
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    nErrCode = root["errcode"].asInt();
    if (0 != nErrCode)
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    if (!root["datas"].isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    Json::Value datas = root["datas"];

    BOOL bRet = TRUE;
    //查看當前版本更新
    if (datas["resversions"].isArray())
    {
        bRet &= ResVersionsResult(datas["resversions"]);
    }

    //獲取當前產品IDeas信息
    if (datas["products"].isArray())
    {
        bRet &= ProductsResult(datas["products"]);
    }

    //獲取當前產品IDeas信息
    if (datas["viplevels"].isArray())
    {
        bRet &= VipLevelResult(datas["viplevels"]);
    }

    //開啟時間
    if (datas["serveropenttime"].isString())
    {
        CSDDateTime stDateTime;
        if(SDTimeFromString(datas["serveropenttime"].asCString(), stDateTime))
        {
            CGSConfig::Instance()->GetServerOpentTime() = stDateTime.GetTimeValue();
        }
    }
    return bRet;
}

BOOL CGetConfigsHttpCMD::ResVersionsResult(Json::Value datas)
{
    CCliTypeVerInfoMap mapCliTypeVerInfo;
    for (UINT32 idx = 0; idx < datas.size(); idx++)
    {
        UINT32	dwCliType = 0;
        UINT32	dwCliVer = 0;
        if(!datas[idx].isObject())
        {
            continue;
        }
        Json::Value data = datas[idx];
        if (!data["clitype"].isInt())
        {
            continue;
        }
        dwCliType = data["clitype"].asInt();
        if (!data["cliver"].isInt())
        {
            continue;
        }
        dwCliVer = data["cliver"].asInt();
        if (!data["vers"].isString())
        {
            continue;
        }
        string strVers = data["vers"].asString();
        SCliVerInfoProp stProt;
        stProt.dwCliVer = dwCliVer;
        stProt.dwResVer = 0;
        stProt.dwResVer = 0;
        SDStrncpy(stProt.aszResVer, strVers.c_str(), strVers.size() > MAX_RES_VER_LEN - 1 ? MAX_RES_VER_LEN - 1 : strVers.size());
        mapCliTypeVerInfo[dwCliType].mapCliVerInfo[dwCliVer] = stProt;
    }
    CCommonInfoMgr::Instance()->GetCliTypeVerInfo() = mapCliTypeVerInfo;
    return TRUE;
}

BOOL CGetConfigsHttpCMD::ProductsResult(Json::Value datas)
{
    CAuthBuyGoldPropMap mapAuthBuyGoldProp;
    for (UINT32 idx = 0; idx < datas.size(); idx++)
    {
        if(!datas[idx].isObject())
        {
            continue;
        }
        Json::Value data = datas[idx];
        if (!data["authtype"].isInt())
        {
            continue;
        }
        if (!data["procontexthead"].isString())
        {
            continue;
        }
        if (!data["procontext"].isString())
        {
            continue;
        }
        if (!data["weburl"].isString())
        {
            continue;
        }
        if (!data["props"].isArray())
        {
            continue;
        }
        UINT8 byAuthType = data["authtype"].asInt();
        SDStrncmp(mapAuthBuyGoldProp[byAuthType].m_aszProContextHead, data["procontexthead"].asCString(), MAX_COMMON_TXT_NUM);
        SDStrncmp(mapAuthBuyGoldProp[byAuthType].m_aszProContext, data["procontext"].asCString(), MAX_VIP_PRO_CONTEXT_LEN);
        SDStrncmp(mapAuthBuyGoldProp[byAuthType].m_aszBuyUrl, data["weburl"].asCString(), MAX_BUY_URL_LEN);
        Json::Value props = data["props"];

        for (UINT32 idy = 0; idy < props.size(); idy++)
        {
            SBuyGoldProp stProp;
            if(!props[idy].isObject())
            {
                continue;
            }
            Json::Value prop = props[idy];
            if (!prop["productid"].isString())
            {
                continue;
            }
            if (!prop["info"].isString())
            {
                continue;
            }

            if (!prop["gold"].isInt())
            {
                continue;
            }

            if (!prop["rmb"].isInt())
            {
                continue;
            }
            stProp.dwProductID = SGDP::SDAtou(prop["productid"].asCString());
            SDStrncpy(stProp.aszProductID, prop["productid"].asCString(), MAX_PRODUCTID_LEN - 1);
            SDStrncpy(stProp.aszBuyGoldInfo, prop["info"].asCString(), MAX_COMMON_TXT_NUM - 1);
            stProp.dwGold = prop["gold"].asInt();
            stProp.wRMB	= prop["rmb"].asInt();
            mapAuthBuyGoldProp[byAuthType].m_vecBuyGoldProp.push_back(stProp);
        }
        std::sort(mapAuthBuyGoldProp[byAuthType].m_vecBuyGoldProp.begin(), mapAuthBuyGoldProp[byAuthType].m_vecBuyGoldProp.end(), my_sort);
    }

    if (mapAuthBuyGoldProp.size())
    {
        CVipPropMgr::Instance()->GetAuthBuyGoldProp() = mapAuthBuyGoldProp;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CGetConfigsHttpCMD::VipLevelResult(Json::Value datas)
{
    UINT8				byMaxVipLevel = 0;
    UINT32				dwMaxVipLevelRechargeGold = 0;
    CVipLevelPropMap		mapVipLevelProp;
    CGold2VipLevelPropMap	mapGold2VipLevelProp;
    for (UINT32 idx = 0; idx < datas.size(); idx++)
    {
        SVipLevelProp stProp;
        if(!datas[idx].isObject())
        {
            continue;
        }
        Json::Value data = datas[idx];
        if (!data["viplevel"].isInt())
        {
            continue;
        }
        stProp.byVipLevel = data["viplevel"].asInt();
        if (!data["needrechargegold"].isInt())
        {
            continue;
        }
        stProp.dwNeedRechargeGold = data["needrechargegold"].asInt();

        if(stProp.byVipLevel > byMaxVipLevel)
        {
            byMaxVipLevel = stProp.byVipLevel;
        }
        if (stProp.dwNeedRechargeGold > dwMaxVipLevelRechargeGold)
        {
            dwMaxVipLevelRechargeGold = stProp.dwNeedRechargeGold;
        }
        mapVipLevelProp[stProp.byVipLevel] = stProp;
        mapGold2VipLevelProp[stProp.dwNeedRechargeGold] = stProp;
    }
    if (mapVipLevelProp.size() && mapGold2VipLevelProp.size())
    {
        CVipPropMgr::Instance()->GetMaxVipLevel()			= byMaxVipLevel;
        CVipPropMgr::Instance()->GetMaxVipLevelRechargeGold() = dwMaxVipLevelRechargeGold;
        CVipPropMgr::Instance()->GetVipLevelProp()			= mapVipLevelProp;
        CVipPropMgr::Instance()->GetGold2VipLevelProp()		= mapGold2VipLevelProp;
        CVipPropMgr::Instance()->InitVipTabInfo();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

