
//
//  时间：2010-12-27。
//
#include "sdutil.h"
#include "framework/lsapplication.h"
#include "sdframework/sdpipechannel.h"
#include "sdframework/sdapplication.h"
#include "sdframework/sdapplication.h"
#include "sdframework/sdloggerimpl.h"
#include "logic/lsevent.h"
#include "db/syncdata.h"
#include "net/cli/usermgr.h"
#include "sms/smsusermgr.h"
#include "sms/smsmgr.h"
#include <sdloggerimpl.h>
#include <net/tlc/authusermgr.h>



#include <fstream>
#include <sstream>
#include <ostream>
using namespace std;
using namespace std;

#define MAX_SQL_LEN		10240000 //sql最大长度
#define MAX_NET_LEN       1024*4
#define LS_EVENT_TIME     1
#define LS_SAVE_TIME		60
#define MAX_DATA_NUM		10000


CLSApplication::CLSApplication():
    m_oReactor(1, EXECUTE_LOOSE)
{
    m_pSQLMemoryPool = NULL;
    m_pNetBufMemoryPool = NULL;
    ms_nMaxDataNum = MAX_DATA_NUM;
    ms_deqData.clear();
	m_poTLCPipeChannel = NULL;
	m_poGTPipeChannel = NULL;
}

CLSApplication::~CLSApplication()
{
}

BOOL CLSApplication::Init(const TCHAR* pszConfigFilename)
{
    if(FALSE == CSDApplication::Init(pszConfigFilename))
    {
        printf(_SDT("[%s: %d]: CSDApplication Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSDApplication Init failed!"), MSG_MARK);
        return FALSE ;
    }

    if(FALSE == m_oDBConfig.Init(pszConfigFilename))
    {
        printf(_SDT("[%s: %d]: CDBConfig Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CDBConfig Init failed!"), MSG_MARK);
        return FALSE;
    }

    m_pNetBufMemoryPool = SDNew CSDFixMemoryPool;
    if(!m_pNetBufMemoryPool->Create(MAX_NET_LEN, MAX_NET_LEN))
    {
        printf(_SDT("[%s: %d]: CSDFixMemoryPool Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSDFixMemoryPool Init failed!"), MSG_MARK);

        SDDelete m_pNetBufMemoryPool;
        m_pNetBufMemoryPool = NULL;
        return FALSE;
    }
    m_pSQLMemoryPool = SDNew CSDFixMemoryPool;
    if(!m_pSQLMemoryPool->Create(MAX_SQL_LEN * 1 , MAX_SQL_LEN * 1))
    {
        printf(_SDT("[%s: %d]: CSDFixMemoryPool Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSDFixMemoryPool Init failed!"), MSG_MARK);

        SDDelete m_pSQLMemoryPool;
        m_pSQLMemoryPool = NULL;
        return FALSE;
    }

    if(FALSE == m_oDBMgr.Init())
    {
        printf(_SDT("[%s: %d]: CDBMgr Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CDBMgr Init failed!"), MSG_MARK);

        return FALSE;
    }

    CLSEvent* poLSEvent = m_oReactor.CreateEvent();
    m_oReactor.SetEvent(poLSEvent, LS_EVENT_TIME);

    if(FALSE == CUserMgr::CreateInstance())
    {
        printf(_SDT("[%s: %d]: CUserMgr CreateInstance failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CUserMgr CreateInstance failed!"), MSG_MARK);

        return FALSE;
    }

    if(FALSE == CUserMgr::Instance()->Init())
    {
        printf(_SDT("[%s: %d]: CUserMgr Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CUserMgr Init failed!"), MSG_MARK);

        return FALSE;
    }

    if (FALSE == CSmsMgr::CreateInstance())
    {
        printf(_SDT("[%s: %d]: CSmsMgr CreateInstance failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSmsMgr CreateInstance failed!"), MSG_MARK);

        return FALSE;
    }
    if(FALSE == CSmsMgr::Instance()->Init())
    {
        printf(_SDT("[%s: %d]: CSmsMgr Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSmsMgr Init failed!"), MSG_MARK);

        return FALSE;
    }

    if (FALSE == CSmsUserMgr::CreateInstance())
    {
        printf(_SDT("[%s: %d]: CSmsUserMgr CreateInstance failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSmsUserMgr CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CSmsUserMgr::Instance()->Init())
    {
        printf(_SDT("[%s: %d]: CSmsUserMgr Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSmsUserMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

	if (FALSE == CAuthUserMgr::CreateInstance())
	{
		printf(_SDT("[%s: %d]: CAuthUserMgr CreateInstance failed!"), MSG_MARK);
		SYS_CRITICAL(_SDT("[%s: %d]: CAuthUserMgr CreateInstance failed!"), MSG_MARK);
		return FALSE;
	}
	if(FALSE == CAuthUserMgr::Instance()->Init())
	{
		printf(_SDT("[%s: %d]: CAuthUserMgr Init failed!"), MSG_MARK);
		SYS_CRITICAL(_SDT("[%s: %d]: CAuthUserMgr Init failed!"), MSG_MARK);
		return FALSE;
	}
	

    //定时取通知版本数据
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_SYNC_DATA);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);

    PrintScreen(_SDT("CLSApplication::Init successful")) ;
    return TRUE ;
}

VOID CLSApplication::UnInit()
{
    //保存日志
    SaveData();
    CSDApplication::UnInit();
    m_oDBMgr.Release();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();

    if (m_pNetBufMemoryPool)
    {
        SDDelete m_pNetBufMemoryPool;
        m_pNetBufMemoryPool = NULL;
    }

    if (m_pSQLMemoryPool)
    {
        SDDelete m_pSQLMemoryPool;
        m_pSQLMemoryPool = NULL;
    }
}

BOOL CLSApplication::Run()
{
    CSDApplication::Run() ;
    m_oDBMgr.Run();
    m_oReactor.Run();
	CAuthUserMgr::Instance()->Run();
    CSmsMgr::Instance()->Run();
    SDSleep(1);
    return TRUE ;
}

CSDPipeChannel* CLSApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel*	poPipeChannel = NULL;
    CSDServerID		oServerID(dwPipeID);

    if (TYPE_GATE_SERVER == oServerID.GetServerType())
    {
        poPipeChannel = SDNew CSDPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oGTProcessor);
        m_poGTPipeChannel = poPipeChannel;

        printf(_SDT("loginserver:recv gate server pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
        DBG_INFO(_SDT("loginserver:recv gate server pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
    }
	else if(TYPE_TLC_SERVER == oServerID.GetServerType())
	{
		poPipeChannel = SDNew CSDPipeChannel;
		poPipeChannel->SetPacketProcessor(&m_oLTCProcessor);
		m_poTLCPipeChannel = poPipeChannel;

		printf(_SDT("loginserver:recv tlc server pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
		DBG_INFO(_SDT("loginserver:recv tlc server pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
	}
    else
    {
        printf(_SDT("loginserver:recv unknown pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
        DBG_INFO(_SDT("loginserver:recv unknown pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
    }

    return poPipeChannel;
}

VOID CLSApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (poChannel)
    {
        CSDServerID		oServerID(poChannel->GetPipeID());
        if (poChannel->GetSvrType() == TYPE_GATE_SERVER || poChannel == m_poGTPipeChannel)
        {
            m_poGTPipeChannel = NULL;
			printf(_SDT("loginserver:recv gate server pipe disconnected(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
			DBG_INFO(_SDT("loginserver:recv gate server pipe disconnected(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
        }
		else if(poChannel->GetSvrType() == TYPE_TLC_SERVER || poChannel == m_poTLCPipeChannel )
		{
			m_poTLCPipeChannel = NULL;
			printf(_SDT("loginserver:recv tlc server pipe disconnected(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
			DBG_INFO(_SDT("loginserver:recv tlc server pipe disconnected(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
		}


        SDDelete poChannel;
        poChannel = NULL;
    }
}

VOID CLSApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}

VOID CLSApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    CSDApplication::OnHandleUserCmd(szCmd);
}

void * CLSApplication::GetSQLMemory()
{
    void * p = NULL;
    m_oSQLLock.Lock();
    p = m_pSQLMemoryPool->Malloc();
    m_oSQLLock.Unlock();
    return p;
}

void CLSApplication::FreeSQL(void * p)
{
    m_oSQLLock.Lock();
    m_pSQLMemoryPool->Free(p);
    m_oSQLLock.Unlock();
}

void CLSApplication::FreeNetBuf(void * p)
{
    m_oNetBufLock.Lock();
    m_pNetBufMemoryPool->Free(p);
    m_oNetBufLock.Unlock();
}

void* CLSApplication::GetNetBufMemory()
{
    void * p = NULL;
    m_oNetBufLock.Lock();
    p = m_pNetBufMemoryPool->Malloc();
    m_oNetBufLock.Unlock();
    return p;
}

CDBMgr* CLSApplication::GetDBMgr()
{
    return &m_oDBMgr;
}

CDBConfig*	CLSApplication::GetDBConfig()
{
    return &m_oDBConfig;
}

CSDApplication* SDGetApp()
{
    static CLSApplication oCLSApplication;
    return &oCLSApplication;
}

VOID CLSApplication::OnEvent()
{
    //检测网络数据是否过期删除
    CkNetData();

    //定时检测删除过期用户数据
    CUserMgr::Instance()->CkUserDisconnect();

    //定时检测删除过期用户数据
    CSmsUserMgr::Instance()->CkUserDisconnect();

    //定时保存日志
    SaveData();


    //定时取通知版本数据
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_SYNC_DATA);
    if(NULL == poCMD)
    {
        return;
    }
    ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL CLSApplication::CkNetData()
{
    CSDMutexLock				v_lock(m_oMutex);
    CPKT_GTLS_LOGIN_ACK_MAP_Itr	itr = m_mapPKT_GTLS_LOGIN_ACK.begin();
    for(itr; itr != m_mapPKT_GTLS_LOGIN_ACK.end();)
    {
        if(SDTimeSecs() >=  itr->second->qwTime + LS_SAVE_TIME)
        {
            m_oPKT_GTLS_LOGIN_ACKPool.Free(itr->second);
            m_mapPKT_GTLS_LOGIN_ACK.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
    return TRUE;
}


VOID	 CLSApplication::AddLoginAck(const string key, const PKT_GTLS_LOGIN_ACK &stAck)
{
    CSDMutexLock				v_lock(m_oMutex);
    pPKT_GTLS_LOGIN_ACK			v_pPKT_GTLS_LOGIN_ACK = NULL;
    CPKT_GTLS_LOGIN_ACK_MAP_Itr	v_itr = m_mapPKT_GTLS_LOGIN_ACK.find(key);
    if ( v_itr != m_mapPKT_GTLS_LOGIN_ACK.end())
    {
        v_pPKT_GTLS_LOGIN_ACK = v_itr->second;
        if (NULL == v_pPKT_GTLS_LOGIN_ACK)
        {
            return;
        }
        v_pPKT_GTLS_LOGIN_ACK->qwTime = SDTimeSecs();
        memcpy(&v_pPKT_GTLS_LOGIN_ACK->stAck, &stAck, sizeof(PKT_GTLS_LOGIN_EX_ACK));
        m_mapPKT_GTLS_LOGIN_ACK[key] = v_pPKT_GTLS_LOGIN_ACK;
    }
    else
    {
        v_pPKT_GTLS_LOGIN_ACK = m_oPKT_GTLS_LOGIN_ACKPool.Alloc();
        if (NULL == v_pPKT_GTLS_LOGIN_ACK)
        {
            return;
        }
        v_pPKT_GTLS_LOGIN_ACK->qwTime = SDTimeSecs();
        memcpy(&v_pPKT_GTLS_LOGIN_ACK->stAck, &stAck, sizeof(PKT_GTLS_LOGIN_EX_ACK));
        m_mapPKT_GTLS_LOGIN_ACK[key] = v_pPKT_GTLS_LOGIN_ACK;
    }

}

BOOL CLSApplication::GetLoginAck(const string key, PKT_GTLS_LOGIN_ACK &stAck)
{
    CSDMutexLock					v_lock(m_oMutex);
    pPKT_GTLS_LOGIN_ACK			v_pPKT_GTLS_LOGIN_ACK = NULL;
    CPKT_GTLS_LOGIN_ACK_MAP_Itr	v_itr = m_mapPKT_GTLS_LOGIN_ACK.find(key);
    if ( v_itr != m_mapPKT_GTLS_LOGIN_ACK.end())
    {
        v_pPKT_GTLS_LOGIN_ACK = v_itr->second;
        if (NULL == v_pPKT_GTLS_LOGIN_ACK)
        {
            return FALSE;
        }
        memcpy(&stAck, &v_pPKT_GTLS_LOGIN_ACK->stAck, sizeof(PKT_GTLS_LOGIN_EX_ACK));
        return TRUE;
    }
    return FALSE;
}

BOOL CLSApplication::AddData( const string &strInfo)
{
    CSDMutexLock lock(ms_deqDataMutex);
    if( ms_deqData.size() > ms_nMaxDataNum )
    {
        return FALSE;
    }
    ms_deqData.push_back( strInfo );
    return TRUE;
}

VOID	CLSApplication::SaveData()
{
    static char s_szSQL[MAX_SQL_LEN] = {0};
    memset(s_szSQL, 0x00, sizeof(s_szSQL));
    strcat(s_szSQL, "insert into loginlog (UserID, PlayerID, DeviceID, LoginIP, LoginTime, Type) values ");

    BOOL bHaveData = FALSE;
    ms_deqDataMutex.Lock();

    if( ms_deqData.size() )
    {
        bHaveData = TRUE;
        strcat(s_szSQL, ms_deqData[0].c_str());
    }
    for( UINT32 dwIndex = 1; dwIndex < ms_deqData.size(); ++dwIndex )
    {
        strcat(s_szSQL, ",");
        strcat(s_szSQL, ms_deqData[dwIndex].c_str());
    }
    ms_deqData.clear();
    ms_deqDataMutex.Unlock();

    if( !bHaveData )
    {
        return;
    }

    UINT32 dwErr = 0;
    string strError;
    INT32 nRet;
    ISDDBSession *m_pDBSession = ((CLSApplication*)SDGetApp())->GetDBMgr()->GetDBSession();
    if (NULL == m_pDBSession)
    {
        return;
    }

    nRet = m_pDBSession->ExecuteSql(s_szSQL);
    if( nRet < 0 )
    {
        SYS_CRITICAL( _SDT( "[%s %d]exec sql error,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), s_szSQL );
    }

}


