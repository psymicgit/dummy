

#include "selectzonehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"


struct  DT_ZONE_DATA_IDX
{
    UINT32		dwIdx;
    DT_ZONE_DATA stDT_ZONE_DATA;
    DT_ZONE_DATA_IDX()
    {
        memset(this, 0x00, sizeof(DT_ZONE_DATA_IDX));
    }
};


struct  DT_ZONE_PLAYER_DATA_IDX
{
    UINT32				dwIdx;
    DT_ZONE_PLAYER_DATA	stDT_ZONE_PLAYER_DATA;
    DT_ZONE_PLAYER_DATA_IDX()
    {
        memset(this, 0x00, sizeof(DT_ZONE_PLAYER_DATA_IDX));
    }
};

typedef vector<DT_ZONE_DATA_IDX>	CZoneDataIdxVec;
typedef CZoneDataIdxVec::iterator	CZoneDataVecItr;

typedef vector<DT_ZONE_PLAYER_DATA_IDX>	CZonePlayerDataIdxVec;
typedef CZonePlayerDataIdxVec::iterator	CZonePlayerDataIdxVecItr;

//自定义排序函数
bool ZoneDataSortByIdx(const DT_ZONE_DATA_IDX & v1,  const DT_ZONE_DATA_IDX &v2)
{
    if (v1.dwIdx < v2.dwIdx)
    {
        return true;
    }
    return false;
}

//自定义排序函数
bool ZonePlayerDataSortByIdx(const DT_ZONE_PLAYER_DATA_IDX & v1,  const DT_ZONE_PLAYER_DATA_IDX &v2)
{
    if (v1.dwIdx < v2.dwIdx)
    {
        return true;
    }
    return false;
}




CSelectZoneHttpCMD::CSelectZoneHttpCMD()
{
    memset(&stReq, 0x00, sizeof(stReq));
    memset(&stAck, 0x00, sizeof(stAck));
    m_strUrl = ((CZSApplication*)SDGetApp())->GetZSConfig()->Getlogin();
}

CSelectZoneHttpCMD::~CSelectZoneHttpCMD()
{

}


BOOL  CSelectZoneHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("_ch=");
    strTemp = stReq.byAuthType == 0 ? AUTH_TYPE_MAXNET : stReq.byAuthType;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&deviceid=");
    strTemp = stReq.aszDeviceID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&username=");
    strTemp = stReq.aszUserName;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    char v_szUserPwd[USERNAME_LEN] = {0};
    sprintf(v_szUserPwd, (char*)stReq.abyUserPwd, stReq.byPwdLen);
    m_strfields.append("&userpwd=");
    strTemp = v_szUserPwd;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&zoneidx=");
    strTemp = stReq.wZoneIdx;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&authmethod=");
    strTemp = stReq.byAuthMethod;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&nickname=");
    strTemp = stReq.aszNickName;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&bindtype=");
    strTemp = stReq.byBindType;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&cliversion=");
    strTemp = stReq.dwCliVersion;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    //临时路径打包上签名
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
    curl_easy_setopt(m_pEasyHandle, CURLOPT_TIMEOUT, 2);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POSTFIELDS, m_strfields.c_str());
    return TRUE;
}

BOOL CSelectZoneHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        stAck.wErrCode = ERR_SELECT_ZONE::ID_INVALID_AUTH_TYPE;
        ResponseMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(&stAck));
    }
    else
    {
        ProcessResult();
    }
    return TRUE;
}

VOID CSelectZoneHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    memcpy(&stReq, pUserData, sizeof(stReq));
}


VOID CSelectZoneHttpCMD::ProcessResult()
{
    DBG_INFO( _SDT( "[%s: %d]: Result(%s)" ), MSG_MARK, m_strResult.c_str() );

    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck(	m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck( m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return;
    }

    //解析返回的状态码
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck( m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
    }
    stAck.wErrCode = root["errcode"].asInt();

    if (stAck.wErrCode != ERR_SELECT_ZONE::ID_SUCCESS)
    {
        ResponseMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(&stAck));
        return;
    }

    if (!root["datas"].isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return;
    }
    Json::Value datas = root["datas"];
    if (!datas["zonelist"].isArray())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck( m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return;
    }

    Json::Value zonelst = datas["zonelist"];

    CZoneDataIdxVec vecZoneDataIdx;
    for (UINT32 idx = 0; idx < zonelst.size(); idx++)
    {
        Json::Value data = zonelst[idx];
        DT_ZONE_DATA_IDX stDT_ZONE_DATA_IDX;
        if (!data["order"].isInt())
        {
            continue;
        }

        stDT_ZONE_DATA_IDX.dwIdx = data["order"].asInt();
        DT_ZONE_DATA &stDT_ZONE_DATA = stDT_ZONE_DATA_IDX.stDT_ZONE_DATA;
        if (!data["id"].isInt())
        {
            continue;
        }
        stDT_ZONE_DATA.wID = data["id"].asInt();
        if (!data["domainname"].isString())
        {
            continue;
        }
        stDT_ZONE_DATA.byDomainLen = data["domainname"].asString().size();
        SDStrncpy((char*)stDT_ZONE_DATA.abyDomainName, data["domainname"].asCString(), MAX_DOMAIN_NAME_LEN);
        if (!data["zoneport"].isInt())
        {
            continue;
        }
        stDT_ZONE_DATA.wZonePort = data["zoneport"].asInt();
        if (!data["zonename"].isString())
        {
            continue;
        }
        SDStrncpy((char*)stDT_ZONE_DATA.aszZoneName, data["zonename"].asCString(), USERNAME_LEN );


        if (!data["busyflag"].isInt())
        {
            continue;
        }
        stDT_ZONE_DATA.byBusyFlag = data["busyflag"].asInt();

        CZonePlayerDataIdxVec vecZonePlayerDataIdx;
        if (data["playerinfo"].isArray())
        {
            Json::Value playerInfo = data["playerinfo"];
            for (UINT32 idy = 0; idy < playerInfo.size(); idy++)
            {
                Json::Value	 data = playerInfo[idy];

                DT_ZONE_PLAYER_DATA_IDX stDT_ZONE_PLAYER_DATA_IDX;
                if (!data["order"].isInt())
                {
                    continue;
                }
                stDT_ZONE_PLAYER_DATA_IDX.dwIdx = data["order"].asInt();

                DT_ZONE_PLAYER_DATA &stDT_ZONE_PLAYER_DATA = stDT_ZONE_PLAYER_DATA_IDX.stDT_ZONE_PLAYER_DATA;
                if (!data["playerid"].isInt())
                {
                    continue;
                }
                stDT_ZONE_PLAYER_DATA.dwPlayerID = data["playerid"].asInt();

                if (!data["playercareer"].isInt())
                {
                    continue;
                }
                stDT_ZONE_PLAYER_DATA.byPlayerCareer = data["playercareer"].asInt();

                if (!data["playerlevel"].isInt())
                {
                    continue;
                }
                stDT_ZONE_PLAYER_DATA.wPlayerLevel = data["playerlevel"].asInt();

                if (!data["playerdspname"].isString())
                {
                    continue;
                }
                SDStrncpy(stDT_ZONE_PLAYER_DATA.aszPlayerDspName, data["playerdspname"].asCString(), USERNAME_LEN);
                vecZonePlayerDataIdx.push_back(stDT_ZONE_PLAYER_DATA_IDX);
            }
        }

        //对玩家信息排序
        std::sort(vecZonePlayerDataIdx.begin(), vecZonePlayerDataIdx.end(), ZonePlayerDataSortByIdx);

        //对玩家信息排序一下
        CZonePlayerDataIdxVecItr itr;
        for ( itr = vecZonePlayerDataIdx.begin(); itr != vecZonePlayerDataIdx.end() && stDT_ZONE_DATA.byPlayerNum < MAX_PLAYER_NUM_PER_ZONE; itr++)
        {
            DT_ZONE_PLAYER_DATA &stDT_ZONE_PLAYER_DATA = stDT_ZONE_DATA.astPlayerInfo[stDT_ZONE_DATA.byPlayerNum++];
            memcpy(&stDT_ZONE_PLAYER_DATA, &(itr->stDT_ZONE_PLAYER_DATA), sizeof(DT_ZONE_PLAYER_DATA));
        }
        vecZoneDataIdx.push_back(stDT_ZONE_DATA_IDX);
    }

    //对区信息排序一下
    std::sort(vecZoneDataIdx.begin(), vecZoneDataIdx.end(), ZoneDataSortByIdx);
    CZoneDataVecItr itr;
    for ( itr = vecZoneDataIdx.begin(); itr != vecZoneDataIdx.end() && stAck.byZoneNum < MAX_ZONE_NUM; itr++)
    {
        DT_ZONE_DATA &stDT_ZONE_DATA = stAck.astZoneList[stAck.byZoneNum++];
        memcpy(&stDT_ZONE_DATA, &(itr->stDT_ZONE_DATA), sizeof(DT_ZONE_DATA));
    }

    ResponseMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(&stAck));
}


