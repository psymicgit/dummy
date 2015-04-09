
#include "dbmgr.h"
#include "basedbccmd.h"
#include "clrplayerdatacmd.h"
#include "createplayercmd.h"
#include "getplayerdatacmd.h"
#include "getplayerdatacmd2.h"
#include "getplayerdatacmd_old.h" //fortest
#include "registerusernamecmd.h"
#include "saveplayerdatacmd.h"
#include "recorplayerloginoff.h"
#include "saveplunderdatacmd.h"
#include "saveracedatacmd.h"
#include "recordbattledata.h"
#include "recordbattledata2.h"
#include "savebossbdatacmd.h"
#include "setapppayinfo.h"
#include "setapppayprocessed.h"
#include "settongbupayinfo.h"
#include "settongbupayprocessed.h"
#include "getbattledata.h"
#include "getbattledata2.h"
#include "getchatcmd.h"
#include "playerdiff.h"
#include "activityencouragecmd.h"
#include "updatecdkeycmd.h"
#include "savefactiondatacmd.h"
#include "getgsnoticecmd.h"
#include "getoprateingactivitycmd.h"
#include "creatfactiondatacmd.h"
#include "saveplayerconsume.h"
#include "getplayerconsume.h"
#include "setcenterpayprocessed.h"
#include "getprivatemsglog.h"
#include "saveprivatemsglog.h"
#include "savecouragedatacmd.h"
#include <db/autosqlbuf.h>
#include <framework/gsapi.h>
#include "leavefactiondatacmd.h"
#include "setretreatepayprocessed.h"
#include "savegvgvideocmd.h"
#include "savegvgcmd.h"
#include "playersnapshotcmd.h"

#include <fstream>
#include "logic/activity/bossbattlemgr.h"
#include "logic/faction/factionmgr.h"
#include "net/db/dbpktbuilder.h"
#include <framework/gsconfig.h>
#include <common/server/utility.h>
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <db/setmypayprocessed.h>
#include "logic/activity/couragemgr.h"
#include "logic/base/logicfunc.h"
#include "db/dbcommoperate.h"
#include "logic/vip/vippropmgr.h"
#include "logic/other/singleprammgr.h"
#include "db/savegvgbuildcmd.h"
#include "db/deleteovergvgift.h"
#include "db/addgvgift.h"
#include "db/addplayerslog.h"
#include "db/delplayerlog.h"
#include "db/setreadprivatemsgcmd.h"
#include "db/addplayergift.h"
#include "db/operatingactivitycmd.h"

#define MAX_SAVE_TIME 60


extern CSDApplication* SDGetApp();

CDBMgr::CDBMgr() :
    m_pDBModule(NULL),
    m_pDBSession(NULL),
    m_pCfgDBSession(NULL),
    m_pDBRecordModule(NULL),
    m_pDBRecordSession(NULL)
{
    m_qwLastFillQueueTick = 0;
    m_qwLastSaveTick = 0;
    m_bSaveProcessed = TRUE;
}

CDBMgr::~CDBMgr()
{
    int a = 1;
    a = 0;
}

BOOL CDBMgr::Init()
{
    SGDP::SDDBAccount account;
    CGSConfig::Instance()->GetDBAccount(account);
    m_pDBModule = SGDP::SDDBGetModule();
    if(NULL == m_pDBModule)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBSession = m_pDBModule->GetDBSession( &account,
                   CGSConfig::Instance()->GetPoolCoreSize(),
                   CGSConfig::Instance()->GetPoolMaxSize() );
    if(NULL == m_pDBSession)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
        return FALSE;
    }
    CGSConfig::Instance()->GetCfgDBAccount(account);
    m_pCfgDBSession = m_pDBModule->GetDBSession( &account,
                      CGSConfig::Instance()->GetCfgPoolCoreSize(),
                      CGSConfig::Instance()->GetCfgPoolMaxSize() );
    if(NULL == m_pCfgDBSession)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetCfgDBSession failed"), MSG_MARK);
        return FALSE;
    }

    SGDP::SDDBAccount recordAccount;
    CGSConfig::Instance()->GetRecordDBAccount(recordAccount);
    m_pDBRecordModule = SGDP::SDDBGetModule();
    if( NULL == m_pDBRecordModule )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBRecordSession = m_pDBRecordModule->GetDBSession( &recordAccount,
                         CGSConfig::Instance()->GetRecordPoolCoreSize(),
                         CGSConfig::Instance()->GetRecordPoolMaxSize() );

    if( NULL == m_pDBRecordSession )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetRecordDBSession failed"), MSG_MARK);
        return FALSE;
    }

    SGDP::SDDBAccount payAccount;
    CGSConfig::Instance()->GetPayDBAccount(payAccount);
    m_pDBPayModule = SGDP::SDDBGetModule();
    if( NULL == m_pDBPayModule )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }

    m_pDBPaySession = m_pDBPayModule->GetDBSession( &payAccount,
                      CGSConfig::Instance()->GetPayPoolCoreSize(),
                      CGSConfig::Instance()->GetPayPoolMaxSize() );

    if( NULL == m_pDBPaySession )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetPayDBSession failed"), MSG_MARK);
        return FALSE;
    }

    vector<string> vecStr;
    char szDir[1024] = {0};
    sprintf( szDir, "%s/../%s", (const CHAR*)SDGetModulePath(), DB_ERROR_DIR_MAIN_NAME);
    ListFile( szDir, ERROR_FILE_PREFIX_PLAYER, ".txt", TRUE, vecStr );

    for( INT32 nIndex = 0; nIndex < INT32( vecStr.size() ); ++nIndex )
    {
        UINT32 dwPos = vecStr[nIndex].rfind( '/' );

        //包括/和字母E
        string strFileName = vecStr[nIndex].substr( dwPos + 2, vecStr[nIndex].size() );
        if( strFileName.empty() )
        {
            continue;
        }

        strFileName = strFileName.substr(0, strFileName.size() - 1);

        vector<string> vecSplitStr = SDSplitStringsA(strFileName, '.');
        if( vecSplitStr.empty() )
        {
            continue;
        }

        //添加错误记录
        UINT32 dwID = SDAtou( vecSplitStr[0].c_str() );
        ifstream inFile( vecStr[nIndex].c_str() );
        if( !inFile.is_open() )
        {
            continue;
        }
        string strTimes;
        strTimes.resize( 256 );
        if( !inFile.eof() )
        {
            inFile.getline( (char *)strTimes.c_str(), strTimes.size() );
        }
        inFile.close();
        CSDDateTime oDateTime;
        if(!SDTimeFromString(strTimes, oDateTime))
        {
            continue;
        }
        m_mapErrorInfo[CLogicFunc::Get3232Key(ERROR_PLAYER, dwID)].qwFileTimes = oDateTime.GetTimeValue();
        m_mapErrorInfo[CLogicFunc::Get3232Key(ERROR_PLAYER, dwID)].strFilePath = vecStr[nIndex];
    }

    vecStr.clear();
    ListFile( szDir, ERROR_FILE_PREFIX_FACTION, ".txt", TRUE, vecStr );

    for( INT32 nIndex = 0; nIndex < INT32( vecStr.size() ); ++nIndex )
    {
        UINT32 dwPos = vecStr[nIndex].rfind( '/' );

        //包括/和字母E
        string strFileName = vecStr[nIndex].substr( dwPos + 2, vecStr[nIndex].size() );
        if( strFileName.empty() )
        {
            continue;
        }

        strFileName = strFileName.substr(0, strFileName.size() - 1);

        vector<string> vecSplitStr = SDSplitStringsA(strFileName, '.');
        if( vecSplitStr.empty() )
        {
            continue;
        }

        //添加错误记录
        UINT32 dwFactionID = SDAtou( vecSplitStr[0].c_str() );
        ifstream inFile( vecStr[nIndex].c_str() );
        if( !inFile.is_open() )
        {
            continue;
        }
        string strTimes;
        strTimes.resize( 256 );
        if( !inFile.eof() )
        {
            inFile.getline( (char *)strTimes.c_str(), strTimes.size() );
        }
        inFile.close();
        CSDDateTime oDateTime;
        if(!SDTimeFromString(strTimes, oDateTime))
        {
            continue;
        }
        m_mapErrorInfo[CLogicFunc::Get3232Key(ERROR_FACTION, dwFactionID)].qwFileTimes = oDateTime.GetTimeValue();
        m_mapErrorInfo[CLogicFunc::Get3232Key(ERROR_FACTION, dwFactionID)].strFilePath = vecStr[nIndex];

        //直接检测是否正常
        CheckAndProcessBackupFactionSql(dwFactionID);
    }
    return TRUE;
}

class CAutoConn
{
public:
    CAutoConn(SGDP::ISDDBSession*	pDBSession):
        m_pDBSession(pDBSession)
    {
        m_pDBConn = m_pDBSession->GetDBConnection();
    }
    ~CAutoConn()
    {
        if (m_pDBConn)
        {
            m_pDBSession->FreeDBConnection(m_pDBConn);
        }
    }
    SGDP::ISDDBConnection *GetDBConnection()
    {
        return m_pDBConn;
    }
private:
    SGDP::ISDDBConnection *m_pDBConn;
    SGDP::ISDDBSession*	m_pDBSession;
};


BOOL CDBMgr::CheckAndProcessBackupFactionSql(UINT32 dwFactionID )
{
    //如果是在登录时获取，需要判断是否发生存储错误，如果发生存储错误，则需要再次保存数据，如果依然失败，则不允许登录，并返回错误
    if( gsapi::GetDBMgr()->IsExistErrorInfo(ERROR_FACTION, dwFactionID ))
    {
        SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
        if(NULL == pDBSession)
        {
            return FALSE;
        }

        CAutoConn oAuto(pDBSession);
        SGDP::ISDDBConnection *pDBConn = oAuto.GetDBConnection();
        if(NULL == pDBConn)
        {
            return FALSE;
        }

        ISDDBRecordSet* pRecordSet = NULL;
        string strError;
        CHAR szSql[512] = {0};
        UINT64 qwDBUpdateTime = 0;
        sprintf(szSql, "select UpdateTime from faction where FactionID='%u';", dwFactionID);


        INT32 nRet = pDBConn->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
        if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
        {
            GetDateTime2Stamp(qwDBUpdateTime, "UpdateTime");
        }
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwFactionID, strError.c_str());
            return REFUSE_LOGIN;
        }

        //若文件保存的时间比DB的更加新，则执行文件sql
        if(gsapi::GetDBMgr()->GetErrorFileUpdateTime(ERROR_FACTION, dwFactionID) > qwDBUpdateTime)
        {
            BOOL m_bError = FALSE;
            vector<string> vecStrList = gsapi::GetDBMgr()->GetSqlList(ERROR_FACTION,  dwFactionID );
            pDBConn->BeginTransaction();
            for( INT32 nIndex = 0; nIndex < INT32(vecStrList.size()); ++nIndex )
            {
                string strError;
                UINT64 qwInsertId = 0;
                UINT32 dwErr = 0;
                INT32 nRet = pDBConn->ExecuteSql(vecStrList[nIndex].c_str(), &qwInsertId, NULL, &dwErr, &strError );
                if(nRet < 0)
                {
                    m_bError = TRUE;
                    break;
                }
            }

            if( m_bError )
            {
                pDBConn->RollbackTransaction();
                return FALSE;
            }
            else
            {
                pDBConn->CommitTransaction();
            }
        }

        //删除失败，下次保存时再删除
        if( !gsapi::GetDBMgr()->DeleteErrorInfo(ERROR_FACTION, dwFactionID ) )
        {
            //删除文件失败
            SYS_CRITICAL(_SDT("[%s: %d]: DeleteErrorInfo failed, Key[%d]!"), MSG_MARK, dwFactionID);
        }
    }
    return SUCCESS;
}






VOID CDBMgr::Release()
{
    //CAutoCycle oAutoCycle(__FUNCTION__);
    //保证所有数据都保存
    //SaveAllPlayerDataToDB();
    //确保执行完所有的db命令
    UINT32 dwIdx = 0;
    if((m_pDBSession) && (m_pDBRecordSession))
    {
        while((m_pDBSession->GetExcuteCmdCount() > 0)
                || (m_pDBRecordSession->GetExcuteCmdCount()))
        {
            if(0 == dwIdx++ % 500)
            {
                DBG_INFO(_SDT("[%s: %d]: DBSession ExcuteCmdCount[%d], DBSession ExcuteCmdCount[%d]."), MSG_MARK,
                         m_pDBSession->GetExcuteCmdCount(), m_pDBRecordSession->GetExcuteCmdCount());
            }
            SDSleep(1);
            Run(TRUE);
        }
    }

    if(NULL != m_pDBModule)
    {
        if(NULL != m_pDBSession)
        {
            m_pDBModule->Close(m_pDBSession);
            m_pDBSession = NULL;
        }
        if(NULL != m_pCfgDBSession)
        {
            m_pDBModule->Close(m_pCfgDBSession);
            m_pCfgDBSession = NULL;
        }
        m_pDBModule->Release();
        m_pDBModule = NULL;
    }

    if(NULL != m_pDBRecordModule )
    {
        if( NULL != m_pDBRecordSession )
        {
            m_pDBRecordModule->Close( m_pDBRecordSession );
            m_pDBRecordSession = NULL;
        }
        m_pDBRecordModule->Release();
        m_pDBRecordModule = NULL;
    }

    if( NULL != m_pDBPayModule )
    {
        if( NULL != m_pDBPaySession )
        {
            m_pDBPayModule->Close( m_pDBPaySession );
            m_pDBPaySession = NULL;
        }

        m_pDBPayModule->Release();
        m_pDBPayModule = NULL;
    }
}

SGDP::ISDDBSession* CDBMgr::GetDBSession()
{
    return m_pDBSession;
}

SGDP::ISDDBSession* CDBMgr::GetRecordDBSession()
{
    return m_pDBRecordSession;
}

SGDP::ISDDBSession* CDBMgr::GetCfgDBSession()
{
    return m_pCfgDBSession;
}

SGDP::ISDDBSession* CDBMgr::GetPayDBSession()
{
    return m_pDBPaySession;
}

BOOL CDBMgr::AddRecordData( EDBCmdType eDbCmdType, void *pUserData )
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(eDbCmdType);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!Cmd:%d"), MSG_MARK, eDbCmdType);
        return FALSE;
    }

    poCMD->SetUserData( pUserData );
    return gsapi::GetDBMgr()->AddRecordCommand(poCMD);
}

VOID CDBMgr::SetSaveProcessed()
{
    m_bSaveProcessed = TRUE;
}

BOOL CDBMgr::Run(BOOL bSaveAll)
{
    //CAutoCycle oAutoCycle("CDBMgr::Run");
    if (FALSE == bSaveAll)
    {
        SaveDataToDB();
    }

    UINT32 dwCount = 100;
    BOOL bRet1 = m_pDBPaySession->Run( dwCount );

    dwCount = 100;
    BOOL bRet2 = m_pDBSession->Run(dwCount);

    dwCount = 100;
    BOOL bRet3 = m_pCfgDBSession->Run(dwCount);

    return (bRet1 | bRet2 | bRet3);
}

BOOL CDBMgr::AddCommand(SGDP::ISDDBCommand* poDBCommand)
{
    return m_pDBSession->AddDBCommand(poDBCommand);
}

BOOL CDBMgr::AddRecordCommand( SGDP::ISDDBCommand* poDBCommand )
{
    return m_pDBRecordSession->AddDBCommand( poDBCommand );
}

BOOL CDBMgr::AddPayCommand( SGDP::ISDDBCommand *poDBCommand )
{
    return m_pDBPaySession->AddDBCommand( poDBCommand );
}

BOOL CDBMgr::AddCfgCommand( SGDP::ISDDBCommand *poDBCommand )
{
    return m_pCfgDBSession->AddDBCommand( poDBCommand );
}

VOID CDBMgr::SaveDataToDB()
{
    //模块数据保存
    CBossBattleMgr::Instance()->OnSave();

    if(CGvGMgr::Instance())
    {
        CGvGMgr::Instance()->SaveToDB();
    }

    //帮派数据保存
    CFactionMgr::Instance()->OnSave();

    CCourageMgr::Instance()->OnSave();

    //定时保存聊天数据
    CPrivateMsgMgr::Instance()->OnSave();

    //AUTOCYCLE;

    UINT64 curClock = SDTimeMilliSec();
    if( (!m_bSaveProcessed) && (curClock - m_qwLastSaveTick < 1000)) //1000毫秒后没返回则重试
    {
        return;
    }

    //判断玩家保存的时间间隔是不是太短
    if( curClock - m_qwLastSaveTick < CGSConfig::Instance()->GetSavePlayerInterval() )
    {
        return;
    }

    if( m_vecPlayerCache.empty())
    {
        //时间未到，则不刷新
        if( curClock - m_qwLastFillQueueTick < CGSConfig::Instance()->GetFillPlayerQueueInterval() )
        {
            return;
        }

        m_qwLastFillQueueTick = curClock;
        m_vecPlayerCache.clear();
        CPlayerMgr::Instance()->GetID2PlayerVec(m_vecPlayerCache);
        if( m_vecPlayerCache.empty())
        {
            return;
        }
    }

    CPlayer *poPlayer = NULL;
    while(!m_vecPlayerCache.empty())
    {
        UINT32 dwPlayerID = m_vecPlayerCache.back();
        m_vecPlayerCache.pop_back();

        poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);

        BOOL bRobot = (NULL != poPlayer && 0 != poPlayer->GetDT_PLAYER_BASE_DATA().byRobotFunc);
        if(NULL == poPlayer || bRobot)
        {
            poPlayer = NULL;
        }
        else
        {
            break;
        }
    }

    if(NULL == poPlayer)
    {
        return;
    }

    if (0 != poPlayer->GetDT_PLAYER_BASE_DATA().byRobotFunc)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: error: skip save player data, player[%u] is a robot!"), MSG_MARK, poPlayer->GetID());
        return;
    }

    //检测时间插入，十秒未到,直接退出
    if (SDTimeSecs() - poPlayer->GetLastInsertTime() < MAX_SAVE_TIME)
    {
        return;
    }

    poPlayer->GetLastInsertTime() = SDTimeSecs();

    //保存玩家数据
    if( CPlayerSaveMgr::Instance()->SavePlayerData( poPlayer ) )
    {
        m_bSaveProcessed = FALSE;
        m_qwLastSaveTick = curClock;
    }

    //如果离线，超过时间则清除
    if((!poPlayer->CkLogin()) &&
            (SDTimeSecs() - poPlayer->GetUpdateTime() > CGSConfig::Instance()->GetOfflineCacheTime() ))
    {
        CPlayerMgr::Instance()->RemovePlayer( poPlayer->GetID() );
    }
}


VOID	 CDBMgr::SaveAllDataToDB()
{
    m_vecPlayerCache.clear();
    CPlayerMgr::Instance()->GetID2PlayerVec(m_vecPlayerCache);
    CID2PlayerVecItr itr = m_vecPlayerCache.begin();
    CPlayer *poPlayer = NULL;
    for ( itr; itr != m_vecPlayerCache.end(); itr++)
    {
        CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer( *itr );
        if (NULL == poPlayer)
        {
            continue;
        }
        //保存玩家数据
        CPlayerSaveMgr::Instance()->SavePlayerData(poPlayer);

        //每投递一个，都执行一下数据库相关操作，防止内存积累太多的数据未处理而出现内存暴涨
        Run(TRUE);

        //减缓投递的速度，防止占用CPU过高
        SDSleep(1);
    }

    //模块数据保存
    CBossBattleMgr::Instance()->OnSave();

    //帮派数据保存
    CFactionMgr::Instance()->OnSave(TRUE);
    //勇气试炼队伍数据
    CCourageMgr::Instance()->OnSave();
    //定时保存聊天数据
    CPrivateMsgMgr::Instance()->OnSave();
}



CBaseDBCMD*	CDBMgr::CreateCmd(EDBCmdType eDbCmdType)
{
    CBaseDBCMD* poBaseDBCMD = NULL;
    switch(eDbCmdType)
    {
    case EDBCT_CREATE:
    {
        poBaseDBCMD = m_oCCreatePlayerCMDPool.Alloc();
    }
    break;
    case EDBCT_GET1:
    {
        poBaseDBCMD = m_oCGetPlayerDataCMDPool.Alloc();
    }
    break;
    case EDBCT_GET2:
    {
        poBaseDBCMD = m_oCGetPlayerDataCMD2Pool.Alloc();
    }
    break;
    //case EDBCT_GET_OLD:
    //{
    //    poBaseDBCMD = m_oCGetPlayerDataCMDPoolOld.Alloc();
    //}
    //break;
    case EDBCT_SAVE:
    {
        poBaseDBCMD = m_oCSavePlayerDataCMDPool.Alloc();
    }
    break;
    case EDBCT_CLR:
    {
        poBaseDBCMD = m_oCClrPlayerDataCMDPool.Alloc();
    }
    break;

    case EDBCT_LOGINOFFINFO:
    {
        poBaseDBCMD = m_oCCRecordPlayerLoginoffPool.Alloc();
    }
    break;

    case EDBCT_SAVEPLUNDER:
    {
        poBaseDBCMD = m_oCSavePlayersPlunderDataCMDPool.Alloc();
    }
    break;

    case EDBCT_SAVERACE:
    {
        poBaseDBCMD = m_oCSavePlayersRaceDataCMDPool.Alloc();
    }
    break;

    case EDBCT_SAVEBATTLEDATA:
    {
        poBaseDBCMD = m_oCRecordPlayerBattleDataCMDPool.Alloc();
    }
    break;
    case EDBCT_SAVEBATTLEDATA2:
    {
        poBaseDBCMD = m_oCRecordPlayerBattleData2CMDPool.Alloc();
    }
    break;
    case EDBCT_GETBATTLEDATA:
    {
        poBaseDBCMD = m_oCGetPlayerBattleDataCMDPool.Alloc();
    }
    break;
    case EDBCT_GETBATTLEDATA2:
    {
        poBaseDBCMD = m_oCGetPlayerBattleData2CMDPool.Alloc();
    }
    break;
    case EDBCT_CHAT:
    {
        poBaseDBCMD = m_oCGetChatCMDPool.Alloc();
    }
    break;
    case EDBCT_SAVEBOSSB:
    {
        poBaseDBCMD = m_oSaveBossBDataCMDPool.Alloc();
    }
    break;
    case EDBCT_SETPAYINFO:
    {
        poBaseDBCMD = m_oCSetAppPayCMDPool.Alloc();
    }
    break;

    case EDBCT_SETPAYPPROCESSED:
    {
        poBaseDBCMD = m_oCSetAppPayProcessedCMDPool.Alloc();
    }
    break;

    case EDBCT_SETTONGBUPAYINFO:
    {
        poBaseDBCMD = m_oCSetTongBuPayCMDPool.Alloc();
    }
    break;

    case EDBCT_SETTONGBUPAYPPROCESSED:
    {
        poBaseDBCMD = m_oCSetTongBuPayProcessedCMDPool.Alloc();
    }
    break;

    case EDBCT_ACTIVITYENCOURAGE:
    {
        poBaseDBCMD = m_oCActivityEncourageCMDPool.Alloc();
    }
    break;
    case EDBCT_UPDATE_CK_KEY:
    {
        poBaseDBCMD = m_oUpdateCdKeyCMDPool.Alloc();
    }
    break;
    case EDBCT_SETMYPAYPPROCESSED:
    {
        poBaseDBCMD = m_oSetMyPayProcessedCMDPool.Alloc();
    }
    break;
    case EDBCT_FACTION:
    {
        poBaseDBCMD = m_oSaveFactionCMDPool.Alloc();
    }
    break;
    case EDBCT_GETGSNOTICE:
    {
        poBaseDBCMD = m_oGetGsNoticeCMDPool.Alloc();
    }
    break;
    case EDBCT_GETOPRATEINGACTIVITY:
    {
        poBaseDBCMD = m_oGetOprateingActivityCMDPool.Alloc();
    }
    break;

    case EDBCT_CREATE_FACTION:
    {
        poBaseDBCMD = m_oCreateFactionDataCMDPool.Alloc();
    }
    break;
    case EDBCT_SAVE_PLAYER_CONSUME:
    {
        poBaseDBCMD = m_oSavePlayerConsumeCMDPool.Alloc();
    }
    break;
    case EDBCT_GET_PLAYER_CONSUME:
    {
        poBaseDBCMD = m_oGetPlayerConsumeCMDPool.Alloc();
    }
    break;
    case EDBCT_SETCENTERPAYPPROCESSED:
    {
        poBaseDBCMD = m_oSetCenterPayProcessedCMDPool.Alloc();
    }
    break;
	case EDBCT_SETRECREATEPAYPROCESSED:
	{
		poBaseDBCMD = m_oSetRecreateProcessedCMDPool.Alloc();
	}
	break;
    case EDBCT_GET_PRIVATEMSGLOG:
    {
        poBaseDBCMD = m_oGetPrivateMsgLogCMDPool.Alloc();
    }
    break;
    case EDBCT_SAVEPRIVATEMSGLOG:
    {

        poBaseDBCMD = m_oSavePrivateMsgLogCMDPool.Alloc();
    }
    break;
    case EDBCT_SAVE_COURAGE_DATA:
    {

        poBaseDBCMD = m_oSaveCourageCMDPool.Alloc();
    }
    break;
    case EDBCT_LEAVE_FACTION:
        poBaseDBCMD = m_oLeaveFactionDataCMDPool.Alloc();
        break;

    case EDBCT_SAVE_GVG_VIDEO:
        poBaseDBCMD = m_oSaveGvGVideoCMDPool.Alloc();
        break;

    case EDBCT_SAVE_GVG_ACTIVITY:
        poBaseDBCMD = m_oSaveGvGActivityCMDPool.Alloc();
        break;

	case EDBCT_SAVE_GVG_BUILD:
		poBaseDBCMD = m_oSaveGvGBuildCMDPool.Alloc();
		break;

    case EDBCT_SAVE_PLAYER_SNAP_SHOT:
        poBaseDBCMD = m_oSavePlayerSnapShotCMDPool.Alloc();
        break;
    case EDBCT_DEL_PLAYER_SNAP_SHOT:
        poBaseDBCMD = m_oDelPlayerSnapShotCMDPool.Alloc();
        break;
	case EDBCT_DELETE_GVG_GIFT:
		poBaseDBCMD = m_oDeleteGvGiftCMDPool.Alloc();
		break;
	case EDBCT_ADD_GVG_GIFT:
		poBaseDBCMD = m_oAddGvGiftCMDPool.Alloc();
		break;
	case EDBCT_ADD_PLAYER_LOG:
		poBaseDBCMD = m_oAddPlayersLogCMDPool.Alloc();
		break;
	case EDBCT_DEL_PLAYER_LOG:
		poBaseDBCMD = m_oDelPlayersLogCMDPool.Alloc();
        break;
    case EDBCT_SAVE_GVG_ARENA:
        poBaseDBCMD = m_oSaveGvGArenaCMDPool.Alloc();
        break;

    case EDBCT_CLEAR_GVG_VIDEO:
        poBaseDBCMD = m_oClearGvGVideoCMDPool.Alloc();
        break;

    case EDBCT_CLEAR_GVG_ACTIVITY:
        poBaseDBCMD = m_oClearGvGActivityCMDPool.Alloc();
        break;
	case EDBCT_SET_READ_PRIVATE_MSG:
		poBaseDBCMD = m_oSetReadPrivateMsgCMDPool.Alloc();
		break;
	case EDBCT_ADD_PLAYER_GIFT:
		poBaseDBCMD = m_oAddPlayerGiftCMDPool.Alloc();
		break;
	case EDBCT_OPERATING_ACTIVITY:
		poBaseDBCMD = m_oOperatingActivityCMDPool.Alloc();
		break;
    default:
        break;
    }

    return poBaseDBCMD;
}

VOID CDBMgr::FreeCmd(CBaseDBCMD* poBaseDBCMD)
{
    if(NULL == poBaseDBCMD)
    {
        return;
    }

    switch(poBaseDBCMD->GetDBCmdType())
    {
    case EDBCT_CREATE:
    {
        m_oCCreatePlayerCMDPool.Free((CCreatePlayerCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_GET1:
    {
        m_oCGetPlayerDataCMDPool.Free((CGetPlayerDataCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_GET2:
    {
        m_oCGetPlayerDataCMD2Pool.Free((CGetPlayerDataCMD2*)poBaseDBCMD);
    }
    break;
    //case EDBCT_GET_OLD:
    //{
    //    m_oCGetPlayerDataCMDPoolOld.Free((CGetPlayerDataCMD_OLD*)poBaseDBCMD);
    //}
    //break;
    case EDBCT_SAVE:
    {
        m_oCSavePlayerDataCMDPool.Free((CSavePlayerDataCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_CLR:
    {
        m_oCClrPlayerDataCMDPool.Free((CClrPlayerDataCMD*)poBaseDBCMD);
    }
    break;

    case EDBCT_LOGINOFFINFO:
    {
        m_oCCRecordPlayerLoginoffPool.Free( (CRecordPlayerLoginoff*)poBaseDBCMD );
    }
    break;

    case EDBCT_SAVEPLUNDER:
    {
        m_oCSavePlayersPlunderDataCMDPool.Free( ( CSavePlayersPlunderDataCMD *)poBaseDBCMD );
    }
    break;

    case EDBCT_SAVERACE:
    {
        m_oCSavePlayersRaceDataCMDPool.Free( ( CSavePlayersRaceDataCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_SAVEBATTLEDATA:
    {
        m_oCRecordPlayerBattleDataCMDPool.Free( (CRecordBattleData *)poBaseDBCMD );
    }
    break;
    case EDBCT_SAVEBATTLEDATA2:
    {
        m_oCRecordPlayerBattleData2CMDPool.Free( (CRecordBattleData2 *)poBaseDBCMD );
    }
    break;
    case EDBCT_GETBATTLEDATA:
    {
        m_oCGetPlayerBattleDataCMDPool.Free( (CGetBattleData *)poBaseDBCMD );
    }
    break;
    case EDBCT_GETBATTLEDATA2:
    {
        m_oCGetPlayerBattleData2CMDPool.Free( (CGetBattleData2 *)poBaseDBCMD );
    }
    break;
    case EDBCT_CHAT:
    {
        m_oCGetChatCMDPool.Free( (CGetChatCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_SAVEBOSSB:
    {
        m_oSaveBossBDataCMDPool.Free( (CSaveBossBDataCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_SETPAYINFO:
    {
        m_oCSetAppPayCMDPool.Free( (CSetAppPayInfo *)poBaseDBCMD );
    }
    break;
    case EDBCT_SETPAYPPROCESSED:
    {
        m_oCSetAppPayProcessedCMDPool.Free( (CSetAppPayProcessed *)poBaseDBCMD );
    }
    break;
    case EDBCT_SETTONGBUPAYINFO:
    {
        m_oCSetTongBuPayCMDPool.Free( (CSetTongBuPayInfo *)poBaseDBCMD );
    }
    break;

    case EDBCT_SETTONGBUPAYPPROCESSED:
    {
        m_oCSetTongBuPayProcessedCMDPool.Free( (CSetTongBuPayProcessed *)poBaseDBCMD );
    }
    break;

    case EDBCT_ACTIVITYENCOURAGE:
    {
        m_oCActivityEncourageCMDPool.Free( (CActivityEncourageCMD *)poBaseDBCMD );
    }
    break;

    case EDBCT_UPDATE_CK_KEY:
    {
        m_oUpdateCdKeyCMDPool.Free( (CUpdateCdKeyCmd *)poBaseDBCMD );
    }
    break;
    case EDBCT_SETMYPAYPPROCESSED:
    {
        m_oSetMyPayProcessedCMDPool.Free((CSetMyPayProcessed*)poBaseDBCMD);
    }
    break;
    case EDBCT_FACTION:
    {
        m_oSaveFactionCMDPool.Free( (CSaveFactionDataCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_GETGSNOTICE:
    {
        m_oGetGsNoticeCMDPool.Free( (CGetGsNoticeCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_GETOPRATEINGACTIVITY:
    {
        m_oGetOprateingActivityCMDPool.Free( (CGetOprateingActivityCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_CREATE_FACTION:
    {
        m_oCreateFactionDataCMDPool.Free( (CCreateFactionDataCMD *)poBaseDBCMD );
    }
    break;
    case EDBCT_SAVE_PLAYER_CONSUME:
    {
        m_oSavePlayerConsumeCMDPool.Free( (CSavePlayerConsume *)poBaseDBCMD );
    }
    break;
    case EDBCT_GET_PLAYER_CONSUME:
    {
        m_oGetPlayerConsumeCMDPool.Free( (CGetPlayerConsume *)poBaseDBCMD );
    }
    break;
    case EDBCT_SETCENTERPAYPPROCESSED:
    {
        m_oSetCenterPayProcessedCMDPool.Free((CSetCenterPayProcessed*)poBaseDBCMD);
    }
    break;
	case EDBCT_SETRECREATEPAYPROCESSED:
		{
			m_oSetRecreateProcessedCMDPool.Free((CSetRecreatePayProcessed*)poBaseDBCMD);
		}
		break;
    case EDBCT_GET_PRIVATEMSGLOG:
    {
        m_oGetPrivateMsgLogCMDPool.Free((CGetPrivateMsgLogCmd*)poBaseDBCMD);
    }
    break;
    case EDBCT_SAVEPRIVATEMSGLOG:
    {
        m_oSavePrivateMsgLogCMDPool.Free((CSavePrivateMsgLogCmd*)poBaseDBCMD);
    }
    break;
    case EDBCT_SAVE_COURAGE_DATA:
    {
        m_oSaveCourageCMDPool.Free((CSaveCourageDataCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_LEAVE_FACTION:
    {
        m_oLeaveFactionDataCMDPool.Free((CLeaveFactionDataCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_SAVE_GVG_VIDEO:
        m_oSaveGvGVideoCMDPool.Free((CSaveGvGVideoCMD*)poBaseDBCMD);
        break;
    case EDBCT_SAVE_GVG_ACTIVITY:
        m_oSaveGvGActivityCMDPool.Free((CSaveGvGActivityCMD*)poBaseDBCMD);
        break;
	case EDBCT_SAVE_GVG_BUILD:
		m_oSaveGvGBuildCMDPool.Free((CSaveGvGBuildCMD*)poBaseDBCMD);
		break;
    case EDBCT_SAVE_PLAYER_SNAP_SHOT:
        m_oSavePlayerSnapShotCMDPool.Free((CSavePlayerSnapShotCMD*)poBaseDBCMD);
        break;
    case EDBCT_DEL_PLAYER_SNAP_SHOT:
        m_oDelPlayerSnapShotCMDPool.Free((CDelPlayerSnapShotCMD*)poBaseDBCMD);
        break;
	case EDBCT_DELETE_GVG_GIFT:
		m_oDeleteGvGiftCMDPool.Free((CDeleteOverGvGift*)poBaseDBCMD);
		break;
	case EDBCT_ADD_GVG_GIFT:
		m_oAddGvGiftCMDPool.Free((CAddGvGiftCMD*)poBaseDBCMD);
		break;
	case EDBCT_ADD_PLAYER_LOG:
		m_oAddPlayersLogCMDPool.Free((CAddPlayersLogCMD*)poBaseDBCMD);
		break;
	case EDBCT_DEL_PLAYER_LOG:
		m_oDelPlayersLogCMDPool.Free((CDelPlayerLogCMD*)poBaseDBCMD);
		break;
    case EDBCT_SAVE_GVG_ARENA:
        m_oSaveGvGArenaCMDPool.Free((CSaveGvGArenaCMD*)poBaseDBCMD);
        break;
    case EDBCT_CLEAR_GVG_ACTIVITY:
        m_oClearGvGActivityCMDPool.Free((CClearGvGActivityCMD*)poBaseDBCMD);
        break;
    case EDBCT_CLEAR_GVG_VIDEO:
        m_oClearGvGVideoCMDPool.Free((CClearGvGVideoCMD*)poBaseDBCMD);
        break;
	case EDBCT_SET_READ_PRIVATE_MSG:
		m_oSetReadPrivateMsgCMDPool.Free((CSetReadPrivateMsgCMD*)poBaseDBCMD);
		break;
	case EDBCT_ADD_PLAYER_GIFT:
		m_oAddPlayerGiftCMDPool.Free((CAddPlayerGiftCMD*)poBaseDBCMD);
		break;
	case EDBCT_OPERATING_ACTIVITY:
		m_oOperatingActivityCMDPool.Free((COperatingActivityCMD*)poBaseDBCMD);
    default:
        break;
    }
}

const DT_PLAYER_NOPVP_DATA* CDBMgr::GetLastData( UINT32 dwPlayerID )
{
    CPlayerDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr != m_mapPlayerSaveDiff.end())
    {
        return &itr->second->GetLastData();
    }
    return NULL;
}

VOID CDBMgr::SetStartData( UINT32 dwPlayerID, const DT_PLAYER_NOPVP_DATA &data )
{
    CPlayerDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr == m_mapPlayerSaveDiff.end())
    {
        CPlayerDiff *poDiff = CPlayerDiffPoolMgr::Instance()->Create();
        if (NULL != poDiff)
        {
            m_mapPlayerSaveDiff[dwPlayerID] = poDiff;
            poDiff->SetStartData(data);
        }
    }
    else
    {
        itr->second->SetStartData(data);
    }
}

VOID CDBMgr::StartDiff( UINT32 dwPlayerID, const DT_PLAYER_NOPVP_DATA &data )
{
    CPlayerDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr != m_mapPlayerSaveDiff.end())
    {
        itr->second->StartDiff(data);
    }
}

VOID CDBMgr::EndDiff(UINT32 dwPlayerID)
{
    CPlayerDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr != m_mapPlayerSaveDiff.end())
    {
        itr->second->EndDiff();
    }
}

CPlayerDiff* CDBMgr::FindDiff(UINT32 dwPlayerID)
{
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    CPlayerDiffMapItr itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr != m_mapPlayerSaveDiff.end())
    {
        return itr->second;
    }

    return NULL;
}

BOOL CDBMgr::HaveChanged(UINT32 dwPlayerID, ESaveDataType eTableType, UINT16 wKey1, UINT16 wKey2, UINT16 wKey3)
{
    CPlayerDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr != m_mapPlayerSaveDiff.end())
    {
        return itr->second->HaveChanged(eTableType, wKey1, wKey2, wKey3);
    }

    // 2014年4月3日修复回档bug: 如果找不到对应的CPlayerDiff旧数据用来比对，则默认返回TRUE，
    // 如果返回FALSE的话，因为逻辑线程随时有可能删除掉对应的CPlayerDiff，会导致玩家部分数据根本得不到保存，非常危险
    return TRUE;
}

VOID CDBMgr::OnPlayerRelease(UINT32 dwPlayerID)
{
    CPlayerDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapPlayerSaveDiff.find(dwPlayerID);
    if(itr != m_mapPlayerSaveDiff.end())
    {
        CPlayerDiffPoolMgr::Instance()->Remove(itr->second);
        m_mapPlayerSaveDiff.erase(dwPlayerID);
    }
}

BOOL CDBMgr::Changed(UINT32 dwFactionID, const DT_FACTION_DATA &stDT_FACTION_DATA)
{
    CFactionDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapFactionSaveDiff.find(dwFactionID);
    if(itr == m_mapFactionSaveDiff.end())
    {
        CFactionDiff *poDiff = CFactionDiffPoolMgr::Instance()->Create();
        if (NULL != poDiff)
        {
            m_mapFactionSaveDiff[dwFactionID] = poDiff;
            return poDiff->Changed(stDT_FACTION_DATA);
        }
    }
    else
    {
        return itr->second->Changed(stDT_FACTION_DATA);
    }
    return FALSE;
}

VOID CDBMgr::OnFactionRelease(UINT32 dwFactionID)
{
    CFactionDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapFactionSaveDiff.find(dwFactionID);
    if(itr != m_mapFactionSaveDiff.end())
    {
        CFactionDiffPoolMgr::Instance()->Remove(itr->second);
        m_mapFactionSaveDiff.erase(dwFactionID);
    }
}


BOOL CDBMgr::HaveChanged(UINT32 dwFactionID, const DT_FACTION_DATA &stDT_FACTION_DATA)
{
    CFactionDiffMapItr itr;
    CSDLock<CSDMutex> lock(m_oDiffMutex);
    itr = m_mapFactionSaveDiff.find(dwFactionID);
    if(itr == m_mapFactionSaveDiff.end())
    {
        CFactionDiff *poDiff = CFactionDiffPoolMgr::Instance()->Create();
        if (NULL != poDiff)
        {
            m_mapFactionSaveDiff[dwFactionID] = poDiff;
            return poDiff->HaveChanged(stDT_FACTION_DATA);
        }
    }
    else
    {
        return itr->second->HaveChanged(stDT_FACTION_DATA);
    }
    return FALSE;
}

VOID CDBMgr::AddErrorInfo(ERROR_SQL_MODULE emModule, UINT32 dwKey, const vector<string> &vecStrErrorSql )
{
    if(vecStrErrorSql.empty())
    {
        return;
    }
    CSDLock<CSDMutex> lock(m_muErrorInfo);
    char szDir[1024] = {0};
    sprintf( szDir, "%s../%s/%s_%d", (const CHAR*)SDGetModulePath(), DB_ERROR_DIR_MAIN_NAME, DB_ERROR_DIR_MAIN_NAME, dwKey % 10 );
    if( !SDDirectoryExists( szDir ) )
    {
        if( !SDCreateDirectory( szDir, TRUE ) )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: create directory failed!path:%s"), MSG_MARK, szDir);
            return;
        }
    }

    char szFilePath[1024] = {0};
    if (ERROR_PLAYER == emModule)
    {
        sprintf( szFilePath, "%s/%s%d.txt", szDir, ERROR_FILE_PREFIX_PLAYER, dwKey );
    }
    else if (ERROR_FACTION == emModule)
    {
        sprintf( szFilePath, "%s/%s%d.txt", szDir, ERROR_FILE_PREFIX_FACTION, dwKey );
    }

    std::ofstream outFile;
    outFile.open( szFilePath, ios::trunc | ios::in);

    if( !outFile.is_open() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: create file failed!path:%s"), MSG_MARK, szFilePath);
        return;
    }

    CSDDateTime oDateTime(SDTimeSecs());
    string strDateTime = oDateTime.ToString();
    strDateTime.append("\n");
    outFile.write(strDateTime.c_str(), strDateTime.size());
    for( INT32 nIndex = 0; nIndex < (INT32)vecStrErrorSql.size(); ++nIndex )
    {
        outFile.write( vecStrErrorSql[nIndex].c_str(), vecStrErrorSql[nIndex].size() );
        outFile << endl;
    }
    outFile.close();
    m_mapErrorInfo[CLogicFunc::Get3232Key(emModule, dwKey)].qwFileTimes = oDateTime.GetTimeValue();
    m_mapErrorInfo[CLogicFunc::Get3232Key(emModule, dwKey)].strFilePath = szFilePath;
}

BOOL CDBMgr::IsExistErrorInfo(ERROR_SQL_MODULE emModule, UINT32 dwKey)
{
    CSDLock<CSDMutex> lock(m_muErrorInfo);
    CErrorInfoMapItr itrFind = m_mapErrorInfo.find(CLogicFunc::Get3232Key(emModule, dwKey));
    if(itrFind != m_mapErrorInfo.end())
    {
        if(SDFileExists(itrFind->second.strFilePath.c_str()))
        {
            return TRUE;
        }
        else
        {
            m_mapErrorInfo.erase(itrFind);
        }
    }
    return FALSE;
}

UINT64 CDBMgr::GetErrorFileUpdateTime( ERROR_SQL_MODULE emModule, UINT32 dwKey )
{
    CSDLock<CSDMutex> lock(m_muErrorInfo);
    CErrorInfoMapItr itrFind = m_mapErrorInfo.find(CLogicFunc::Get3232Key(emModule, dwKey));
    if( itrFind == m_mapErrorInfo.end())
    {
        return 0;
    }

    return (UINT64)itrFind->second.qwFileTimes;
}

vector<string> CDBMgr::GetSqlList( ERROR_SQL_MODULE emModule, UINT32 dwKey )
{
    CSDLock<CSDMutex> lock(m_muErrorInfo);
    vector<string> vecStrSqlInfo;
    CErrorInfoMapItr itrFind = m_mapErrorInfo.find(CLogicFunc::Get3232Key(emModule, dwKey));
    if( itrFind == m_mapErrorInfo.end() )
    {
        return vecStrSqlInfo;
    }

    ifstream inFile( itrFind->second.strFilePath.c_str() );
    if( !inFile.is_open() )
    {
        return vecStrSqlInfo;
    }

    //第一行为时间
    string strTimes;
    strTimes.resize( 256 );
    if( !inFile.eof() )
    {
        inFile.getline( (char *)strTimes.c_str(), strTimes.size() );
    }
    //最多只读取100行，防止出现死循环
    int lines = 100;
    string strSqlRead;
    strSqlRead.resize( MAX_SQL_LEN );
    while( !inFile.eof() && lines > 0)
    {
        lines--;
        inFile.getline( (char *)strSqlRead.c_str(), strSqlRead.size() );
        if( 0 == strSqlRead[0] )
        {
            continue;
        }

        vecStrSqlInfo.push_back( strSqlRead );
    }
    inFile.close();
    return vecStrSqlInfo;
}

BOOL CDBMgr::ReadRetreatePlayerData( const char* FileName ,UINT32 unAddGold,UINT32 unAddRMB,UINT32 dwPlayerID)
{
	if (NULL == FileName)
	{
		return FALSE;
	}

	vector<string> vecStrSqlInfo;
	ifstream inFile( FileName );

	if( !inFile.is_open() )
	{
		return FALSE;
	}

	//最多只读取100行，防止出现死循环
	string strSqlRead;
	CAutoSqlBuf oAutoBuf;
	TCHAR* aszSqlRead = oAutoBuf.GetSqlBuf();
	int lines = 100;

	while( !inFile.eof() && lines > 0 )
	{
		lines--;
		inFile.getline( aszSqlRead, MAX_SQL_LEN );

		if(strlen(aszSqlRead) == 0)
		{
			continue;
		}

		strSqlRead = aszSqlRead;
		vecStrSqlInfo.push_back(strSqlRead);
	}

	inFile.close();

	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();

	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CAutoConn oAuto(pDBSession);

	SGDP::ISDDBConnection *pDBConn = oAuto.GetDBConnection();
	if(NULL == pDBConn)
	{
		return FALSE;
	}

	ISDDBRecordSet* pRecordSet = NULL;
	BOOL m_bError = FALSE;
	pDBConn->BeginTransaction();

	for( INT32 nIndex = 0; nIndex < INT32(vecStrSqlInfo.size()); ++nIndex )
	{
		string strError;
		UINT64 qwInsertId = 0;
		UINT32 dwErr = 0;
		INT32 nRet = pDBConn->ExecuteSql(vecStrSqlInfo[nIndex].c_str(), &qwInsertId, NULL, &dwErr, &strError );

		if(nRet < 0)
		{
			m_bError = TRUE;
			break;
		}
	}

	if( m_bError )
	{
		pDBConn->RollbackTransaction();
		return FALSE;
	}
	else
	{
		char aszGetSql[256] = {0};
		char aszUpdateSql[256] = {0};
		sprintf(aszGetSql,"select Gold,TotalRMB,Pay from players where PlayerID = '%u'",dwPlayerID);

		string strError;
		UINT64 qwInsertId = 0;
		UINT32 dwErr = 0;
		INT32 nGetInfoRet = pDBConn->ExecuteSqlRs(aszGetSql, &pRecordSet, 0, &strError );
		UINT32 qwPay = 0;
		UINT32 qwVipLevel = 0;

		if (SDDB_HAS_RECORDSET == nGetInfoRet && (pRecordSet->GetRecord()))
		{
			qwPay = unAddGold + SDAtou( pRecordSet->GetFieldValueByName("Pay") );
			qwVipLevel = CVipPropMgr::Instance()->GetVipLevel( qwPay );
			unAddGold = unAddGold + SDAtou( pRecordSet->GetFieldValueByName("Gold") );
			unAddRMB = unAddRMB + SDAtou( pRecordSet->GetFieldValueByName("TotalRMB") );
		}
		else
		{
			if(NULL != pRecordSet)
			{
				pRecordSet->Release();
				pRecordSet = NULL;
			}

			pDBConn->RollbackTransaction();
			return FALSE;
		}

		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}

		sprintf(aszUpdateSql,"update players set Gold = '%u',TotalRMB = '%u',Pay='%u',VipLevel='%u' where PlayerID='%u'",unAddGold,unAddRMB,qwPay,qwVipLevel,dwPlayerID);
		INT32 nRet = pDBConn->ExecuteSql(aszUpdateSql, &qwInsertId, NULL, &dwErr, &strError );

		if (nRet < 0)
		{
			pDBConn->RollbackTransaction();
			return FALSE;
		}

		pDBConn->CommitTransaction();
	}

	return TRUE;
}

BOOL CDBMgr::GetRechargeGold( UINT32 dwPlayerId,UINT32 &dwAddGold,UINT32 &dwAddRMB,UINT64 dwTime)
{
	
	CHAR szSQL[128] = {0};
	sprintf(szSQL, "select * from player_recharge where PlayerID='%u'", dwPlayerId);

	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();

	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CAutoConn oAuto(pDBSession);
	SGDP::ISDDBConnection *pDBConn = oAuto.GetDBConnection();

	if(NULL == pDBConn)
	{
		return FALSE;
	}

	string strError;
	ISDDBRecordSet* pRecordSet = NULL;
	UINT64 qwInsertId = 0;
	UINT32 dwErr = 0;
	INT32 nRet = pDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

	if (SDDB_HAS_RECORDSET == nRet)
	{
		while(pRecordSet->GetRecord())
		{
			string strTime =  pRecordSet->GetFieldValueByName("CreateTime") ;
			UINT32 unGold = SDAtou( pRecordSet->GetFieldValueByName("Gold") );
			UINT32 unRmb = SDAtou( pRecordSet->GetFieldValueByName("RMB") );
			CSDDateTime oDateTime;

			if(!SDTimeFromString(strTime, oDateTime))
			{
				continue;
			}

			UINT64 dwRechargeTime = oDateTime.GetTimeValue();

			if (dwRechargeTime > dwTime)
			{
				dwAddGold = dwAddGold + unGold;
				dwAddRMB = dwAddRMB + unRmb;
			}
		}
	}
	else
	{
		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}

		if (SDDB_NO_RECORDSET == nRet)
		{
			return TRUE;
		}

		return FALSE;
	}

	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}

	return TRUE;
}

BOOL CDBMgr::GetRetreateMaxIndex(UINT32 &dwIndex)
{
	string strError;
	UINT64 qwInsertId = 0;
	UINT32 dwErr = 0;
	ISDDBRecordSet* pRecordSet = NULL;

	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();

	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CAutoConn oAuto(pDBSession);
	SGDP::ISDDBConnection *pDBConn = oAuto.GetDBConnection();

	if(NULL == pDBConn)
	{
		return FALSE;
	}

	CHAR szSQL[128] = {0};
	sprintf(szSQL, "select max(PayID) as maxPayID from player_recharge");

	INT32 nRet = pDBConn->ExecuteSqlRs(szSQL,&pRecordSet, 0, &strError );

	if (SDDB_HAS_RECORDSET == nRet && (pRecordSet->GetRecord()))
	{
		if (NULL == pRecordSet->GetFieldValueByName("maxPayID"))
		{
			return TRUE;
		}

		dwIndex = SDAtou( pRecordSet->GetFieldValueByName("maxPayID") );

		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}

		return TRUE;
	}
	else
	{
		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}
	}

	return FALSE;
}

BOOL CDBMgr::DeleteErrorInfo( ERROR_SQL_MODULE emModule, UINT32 dwKey )
{

    string strDir;
    CSDLock<CSDMutex> lock(m_muErrorInfo);
    SErrorInfo stErrorInfo = m_mapErrorInfo[CLogicFunc::Get3232Key(emModule, dwKey)];

    UINT32 dwLen = stErrorInfo.strFilePath.rfind( "/" );
    strDir = stErrorInfo.strFilePath.substr( 0, dwLen );


    //删除文件保证先删除Key值，再删除目录文件，可能存在文件删除失败的可能，导致玩家进不去
    m_mapErrorInfo.erase( CLogicFunc::Get3232Key(emModule, dwKey) );
    if( !SDFileRemove( stErrorInfo.strFilePath.c_str() ) )
    {
        return FALSE;
    }

    if( strDir.empty() )
    {
        return TRUE;
    }
    //删除多余目录
    if( SDIsEmptyDirectory( strDir.c_str() ) )
    {
        if( !SDDeleteDirectory( strDir.c_str() ) )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: delete dir failed!path:%s"), MSG_MARK, strDir.c_str());
        }
    }

    strDir = SDFileExtractPath( strDir.c_str() );
    if( SDIsEmptyDirectory( strDir.c_str() ) )
    {
        if( !SDDeleteDirectory( strDir.c_str() ) )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: delete dir failed!path:%s"), MSG_MARK, strDir.c_str());
        }
    }

    return TRUE;
}

BOOL CDBMgr::FindPlayerSaveTime(UINT32 dwPlayerID)
{
    CSDLock<CSDMutex> lock(m_oSaveMutex);

    CMapSavePlayerTimeItr itr = m_mapSavePlayerTime.find(dwPlayerID);
    if ( itr == m_mapSavePlayerTime.end())
    {
        m_mapSavePlayerTime[dwPlayerID] = SGDP::SDTimeSecs();
        return TRUE;
    }
    else
    {
        if ( CSinglePramMgr::Instance()->GetSavePlayerInterval() < SGDP::SDTimeSecs() - itr->second )
        {
            itr->second = SGDP::SDTimeSecs();
            return TRUE;
        }
    }
    return FALSE;
}