
#include "framework/zsapplication.h"
#include "userdeviceinfo.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>

//通过新的mac获得旧的mac
BOOL GetIdentifierMac(const CHAR* pszAfterMac, CHAR *pszBuff, UINT8 bySize)
{
    const char* mac = pszAfterMac;

    if(mac == NULL)
        return false;

    int macLength = strlen(mac);

    if (0 == macLength)
    {
        return false;
    }

    UINT8 xorserialBegin = 0xC0;

    string s("");

    char temp[8];

    try
    {
        for (int i = 0 ; i < (bySize) / 2; i++)
        {
            sprintf(temp, "%02x", mac[i % macLength] ^ xorserialBegin);
            s.append(temp);
            xorserialBegin += 5;
        }
    }
    catch(...)
    {
		return false;
    }


    sprintf(pszBuff,  "%s", s.c_str());

    return true;
}


CUserDeviceInfoCMD::CUserDeviceInfoCMD()
{
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CUserDeviceInfoCMD::~CUserDeviceInfoCMD()
{
}

VOID CUserDeviceInfoCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ* pstReq = (PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    CHAR		szMac[256] = {0};
    CHAR		szAdid[256] = {0};
    UINT32	dwIndex = 0;
    UINT8	byExtNum = pstReq->aszExtInfo[dwIndex++];
    for(UINT8 i = 0; i < byExtNum; i++)
    {
        UINT8 byExtLen = pstReq->aszExtInfo[dwIndex++];
        switch (i)
        {
        case 0:
            SDStrncpy(szMac, pstReq->aszExtInfo + dwIndex, byExtLen);
            break;
        case 1:
            SDStrncpy(szAdid, pstReq->aszExtInfo + dwIndex, byExtLen);
            break;
        default:
            break;
        }
        dwIndex += byExtLen;
    }

    CHAR szMacBuff[41] = {0};
    BOOL bHaveOldData = FALSE;
    if(GetIdentifierMac(szMac, szMacBuff, 41))
    {
        CHAR		szSQL[2048] = {0};
        sprintf(szSQL, "select 1 from userdeviceinfo where DeviceID = '%s';", szMacBuff);
        UINT32	dwErr = 0;
        string	strErr;
        ISDDBRecordSet* pRecordSet = NULL;
        INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, &dwErr, &strErr);
        if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))//有记录，则更新为新的DeviceID和信息
        {
            bHaveOldData = TRUE;
            pRecordSet->Release();
        }
        pRecordSet = NULL;

    }

    CHAR		szSQL[2048] = {0};
    if(bHaveOldData)
    {
        sprintf(szSQL, "insert into userdeviceinfo (DeviceID, Platform, GameVersion, OSType, OSVersion, HardwareVersion, Mac, Adid, CreateTime) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', now())"\
                " on duplicate key update "\
                "userdeviceinfo.Platform = values(userdeviceinfo.Platform)"\
                ", userdeviceinfo.GameVersion = values(userdeviceinfo.GameVersion)"\
                ", userdeviceinfo.OSType = values(userdeviceinfo.OSType)"\
                ", userdeviceinfo.OSVersion = values(userdeviceinfo.OSVersion)"\
                ", userdeviceinfo.HardwareVersion = values(userdeviceinfo.HardwareVersion)"\
                ", userdeviceinfo.Mac = values(userdeviceinfo.Mac)"\
                ", userdeviceinfo.Adid = values(userdeviceinfo.Adid)"\
                ", userdeviceinfo.DeviceID='%s'"
                ,
                szMacBuff, pstReq->aszPlatform, pstReq->aszGameVersion, pstReq->aszOSType,
                pstReq->aszOSVersion, pstReq->aszHardwareVersion, szMac, szAdid,
                pstReq->aszDeviceID);//将之前的DeviceID改成新的
    }
    else
    {
        sprintf(szSQL, "insert into userdeviceinfo (DeviceID, Platform, GameVersion, OSType, OSVersion, HardwareVersion, Mac, Adid, CreateTime) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', now())"\
                " on duplicate key update "\
                "userdeviceinfo.Platform = values(userdeviceinfo.Platform)"\
                ", userdeviceinfo.GameVersion = values(userdeviceinfo.GameVersion)"\
                ", userdeviceinfo.OSType = values(userdeviceinfo.OSType)"\
                ", userdeviceinfo.OSVersion = values(userdeviceinfo.OSVersion)"\
                ", userdeviceinfo.HardwareVersion = values(userdeviceinfo.HardwareVersion)"\
                ", userdeviceinfo.Mac = values(userdeviceinfo.Mac)"\
                ", userdeviceinfo.Adid = values(userdeviceinfo.Adid)"\
                ,
                pstReq->aszDeviceID, pstReq->aszPlatform, pstReq->aszGameVersion, pstReq->aszOSType,
                pstReq->aszOSVersion, pstReq->aszHardwareVersion, szMac, szAdid);
    }

    UINT32	dwErr = 0;
    string	strErr;
    INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strErr);
    if(nRet < 0)
    {
        m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
        SYS_CRITICAL(_SDT("Eexe SQL Faild: Err(%u:%s), SQL:%s"), dwErr, strErr.c_str(), szSQL);
    }
    else
    {
        m_stAck.wErrCode = ERR_REPORT_USER_DEVICE_INFO::ID_SUCCESS;
    }
}

VOID CUserDeviceInfoCMD::OnExecuted()
{
    ResponseMsg(CLIZS_REPORT_USER_DEVICE_INFO_ACK, (CHAR*)(&m_stAck));
}