///<------------------------------------------------------------------------------
//< @file:   gameserver\db\getplayersnapshotcmd.cpp
//< @author: hongkunan
//< @date:   2014年3月19日 21:55:3
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "getplayersnapshotcmd.h"


BOOL CGetPlayerSnapShotCMD::GetPlayerData(SGDP::ISDDBConnection *poDBConn, UINT32 dwPlayerID, DT_PLAYER_DATA &stPlayerData)
{
    return FALSE;
}

VOID SDAPI CGetPlayerSnapShotCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    memset(&m_stPlayerData, 0, sizeof(m_stPlayerData));

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_snapshot where PlayerID='%u'", m_dwPlayerID);

    SGDP::ISDDBRecordSet* pRes = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRes, 0, &strError );

    if(SDDB_HAS_RECORDSET != nRet)
    {
        return;
    }

    if(pRes->GetRecord())
    {
        UINT32 dwLen = pRes->GetFieldLengthByName( "PlayerData" );

        // 先尝试解码
        if(-1 == m_stPlayerData.DecodeSaveData(pRes->GetFieldValueByName("PlayerData"), dwLen))
        {
            SYS_CRITICAL( _SDT("[%s: %d] video.DecodeSaveData Failed, len = <%u>!"), MSG_MARK, dwLen);
        }
    }

    if(pRes)
    {
        pRes->Release();
    }
}

VOID CGetPlayerSnapShotCMD::SetUserData(VOID* pUserData)
{
    m_dwPlayerID = *((UINT32*)pUserData);
}

VOID SDAPI CGetPlayerSnapShotCMD::OnExecuted()
{

}
