
#include <framework/gsapi.h>
#include "getplayerdatacmd.h"
#include "getplayerdatacmd2.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include "dbcommoperate.h"

#include <net/db/dbprocessor.h>

/**
* @brief 用户保存类型
*/
/*
enum ESaveUserToDbType
{
    ESRTDT_UNVALID = -1,
    ESRTDT_SHUTDOWN_EXIT    ,       ///< 玩家断线
    ESRTDT_GATESHUTDOWN_EXIT,       ///< GT断线
    ESRTDT_TIMER            ,       ///< TIMER例行保存
    ESRTDT_KICKOUT_ROLE     ,       ///< 踢人
    ESRTDT_MAX
};
*/


CGetPlayerDataCMD2::CGetPlayerDataCMD2()
{
    m_pstGetPlayerData = NULL;
}


CGetPlayerDataCMD2::~CGetPlayerDataCMD2()
{
}

VOID CGetPlayerDataCMD2::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    SDTRY_BEGIN


    m_bSuccess = GetData(poDBConn);


    SDTRY_END

}

VOID CGetPlayerDataCMD2::OnExecuted()
{
    SDTRY_BEGIN

    gsapi::GetDBProcessor()->OnPlayerDataAck2(m_pstGetPlayerData, m_dwTransID);


    SDTRY_END
}

BOOL CGetPlayerDataCMD2::GetData(SGDP::ISDDBConnection* poDBConn)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    UINT32 dwPlayerID1 = m_pstGetPlayerData->dwPlayerID1;
    DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData1 = m_pstGetPlayerData->stDT_PLAYER_DATA1.stPlayerNoPvpData;
    DT_PLAYER_PVP_DATA& stPlayerPvpData1 = m_pstGetPlayerData->stDT_PLAYER_DATA1.stPlayerPvpData;

    //检测备份的SQL文件
    m_pstGetPlayerData->wErrCode = CGetPlayerDataCMD::CheckAndProcessBackupSql( poDBConn, dwPlayerID1 );
    if( m_pstGetPlayerData->wErrCode != SUCCESS )
    {
        return FALSE;
    }

    if(FALSE == CGetPlayerDataCMD::GetData(poDBConn, dwPlayerID1, stPlayerNoPvpData1, stPlayerPvpData1, m_pstGetPlayerData->wErrCode))
    {
        return FALSE;
    }

    UINT32 dwPlayerID2 = m_pstGetPlayerData->dwPlayerID2;
    DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData2 = m_pstGetPlayerData->stDT_PLAYER_DATA2.stPlayerNoPvpData;
    DT_PLAYER_PVP_DATA& stPlayerPvpData2 = m_pstGetPlayerData->stDT_PLAYER_DATA2.stPlayerPvpData;


    if(m_pstGetPlayerData->dwPlayerID2 != m_pstGetPlayerData->dwPlayerID1)
    {
        //检测备份的SQL文件
        m_pstGetPlayerData->wErrCode = CGetPlayerDataCMD::CheckAndProcessBackupSql( poDBConn, dwPlayerID2 );
        if( m_pstGetPlayerData->wErrCode != SUCCESS )
        {
            return FALSE;
        }

        if(FALSE == CGetPlayerDataCMD::GetData(poDBConn, dwPlayerID2, stPlayerNoPvpData2, stPlayerPvpData2, m_pstGetPlayerData->wErrCode))
        {
            return FALSE;
        }
    }

    return TRUE;
}

VOID CGetPlayerDataCMD2::SetUserData(VOID* pUserData)
{
    m_pstGetPlayerData = (SGetPlayerData2*)pUserData;
}
