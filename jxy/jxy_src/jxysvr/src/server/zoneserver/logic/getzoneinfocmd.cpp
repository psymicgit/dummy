
#include "framework/zsapplication.h"
#include "getzoneinfocmd.h"
#include <sdutil.h>
#include <common/client/errdef.h>




CGetZoneInfoCMD::CGetZoneInfoCMD()
{
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CGetZoneInfoCMD::~CGetZoneInfoCMD()
{
}

VOID CGetZoneInfoCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;

    PKT_CLIZS_GET_ZONE_INFO_REQ* pstReq = (PKT_CLIZS_GET_ZONE_INFO_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    if (FALSE == GetInfo(poDBConn, pstReq->wID))
    {
        m_bSuccess = FALSE;
    }
    else
    {
        m_bSuccess = TRUE;
    }
}


VOID CGetZoneInfoCMD::OnExecuted()
{
    ResponseMsg(CLIZS_GET_ZONE_INFO_ACK, (CHAR*)(&m_stAck));
}


BOOL CGetZoneInfoCMD::GetInfo(SGDP::ISDDBConnection* poDBConn, UINT16 wID)
{

    m_stAck.dwErrCode = ERR_GET_ZONE_INFO::ID_ZONE_NOT_EXIST_ERR;
    CHAR szSQL[1024] = {0};

    sprintf(szSQL, "select DomainName, Port from zone where ID = '%d'", wID);

	SYS_CRITICAL(_SDT("%s"), szSQL);
    ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            //m_stAck.dwZoneIP = inet_addr(SDUTF82A(pRecordSet->GetFieldValueByName("ZoneIP")).c_str());

            m_stAck.byDomainLen = pRecordSet->GetFieldLengthByName("DomainName");
            m_stAck.byDomainLen = m_stAck.byDomainLen > MAX_DOMAIN_NAME_LEN ? MAX_DOMAIN_NAME_LEN : m_stAck.byDomainLen;
            memcpy(m_stAck.abyDomainName, pRecordSet->GetFieldValueByName("DomainName"), m_stAck.byDomainLen);

            m_stAck.wZonePort = SDAtou(pRecordSet->GetFieldValueByName("ZonePort"));
            m_stAck.dwErrCode = ERR_GET_ZONE_INFO::ID_SUCCESS;
        }
    }
    else if(SDDB_NO_RECORDSET == nRet)
    {
        m_stAck.dwErrCode = ERR_GET_ZONE_INFO::ID_ZONE_NOT_EXIST_ERR;
    }

    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


