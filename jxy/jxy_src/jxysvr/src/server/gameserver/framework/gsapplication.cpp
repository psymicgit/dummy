#include "gsapplication.h"
#include "gsconfig.h"
#include <net/gt/gtpipechannel.h>
#include <net/gs/gspipechannel.h>
#include <net/pk/pkpipechannel.h>
#include <net/pk/pkprocessor.h>

#include <logic/logicmodel.h>
#include <sdutil.h>
#include <logic/event/timereventmgr.h>
#include <common/server/utility.h>
#include <protocol/server/protogsns.h>
#include <protocol/server/protogmgs.h>
#include <net/gs/gsprocessor.h>
#include "logic/http/httpmgr.h"
#include "logic/activity/bossbattlemgr.h"
#include <logic/other/jsonopmgr.h>
#include "logic/friendsys/privatemsgmgr.h"
#include <logic/faction/factionmgr.h>
#include "logic/activity/couragemgr.h"
#include <db/autosqlbuf.h>
#include "logic/gvg/local/gvgbuild.h"



CGSApplication::CGSApplication()
    : m_eThisServerType(TYPE_INVALID_SERVER)
{
    m_poNSPipeChannel = NULL;
    m_poGMPipeChannel = NULL;
    m_poGSPipeChannel = NULL;
    m_poPKPipeChannel = NULL;

    m_poListener = NULL;

    m_qwServerStartTime = SDTimeSecs();

    m_bNeedSave2DB = TRUE;
}

CGSApplication::~CGSApplication()
{
    CLogicModel::Instance()->Destroy();
    CGSConfig::Instance()->DestroyInstance();
}

BOOL CGSApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
        return FALSE;
    }
    PrintScreen(_SDT("GameServer::Init successful"));

    if(FALSE == CGSConfig::CreateInstance())
    {
        return FALSE;
    }
    if(!CGSConfig::Instance()->Init(pszConfigFilename))
    {
        PrintScreen(_SDT("CGSConfig::Init failed!"));
        return FALSE;
    }

    if(FALSE == CUserMgr::CreateInstance())
    {
        return FALSE;
    }
    if(FALSE == CUserMgr::Instance()->Init())
    {
        return FALSE;
    }

    m_pSQLMemoryPool = SDNew CSDFixMemoryPool;
    if(!m_pSQLMemoryPool->Create(MAX_SQL_LEN , MAX_SQL_LEN * 4))
    {
        SDDelete m_pSQLMemoryPool;
        m_pSQLMemoryPool = NULL;
        return FALSE;
    }

    //初始化db模块
    if(FALSE == m_oDBMgr.Init())
    {
        USR_INFO(_SDT("[%s: %d]:CDBMgr init failed"), MSG_MARK);
        return FALSE;
    }

    //初始化游戏逻辑模块
    if (FALSE == CLogicModel::CreateInstance())
    {
        USR_INFO(_SDT("[%s: %d]:Create CLogicModel failed"), MSG_MARK);
        return FALSE;
    }

    if(FALSE == CLogicModel::Instance()->Init())
    {
        USR_INFO(_SDT("[%s: %d]:Create CLogicModel Init failed"), MSG_MARK);
        return FALSE;
    }

    if(FALSE == InitRecharge())
    {
        USR_INFO(_SDT("[%s: %d]:InitRecharge failed"), MSG_MARK);
        return FALSE;
    }

    // 根据配置表中的选项决定本服务器是跨服战服务器还是游戏服务器
    m_eThisServerType = GetServerType();
    if(FALSE == InitGvGSvrLogic())
    {
        SYS_CRITICAL(_SDT("[%s: %d]:InitGvGSvrLogic failed"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

// 停止游戏服务器的各个任务，比如定时回写玩家数据、定时取公告等
VOID CGSApplication::StopGameServerTask()
{
    // 把定时器全都清掉
    CTimerEventMgr::Instance()->Clear();

    // 终结json解析线程
    CJsonOpMgr::Instance()->Terminate();
}

BOOL CGSApplication::InitGvGSvrLogic()
{
    // 如果是跨服战服务器，终结游戏服务器初始化时预订的各项游戏任务
    if(!IsServerType(TYPE_PK_SERVER))
    {
        return TRUE;   
    }

    StopGameServerTask();
    USR_INFO(_SDT("[%s:%d]:PKServer: Terminate the gameserver tasks OK!"), MSG_MARK);

    return CLogicModel::Instance()->InitGvG();
}

BOOL CGSApplication::InitRecharge()
{
#if defined(WIN32) || defined (WIN64)
    m_poListener = GetNetModule()->CreateListener(NETIO_COMPLETIONPORT);
#else
    m_poListener = GetNetModule()->CreateListener(NETIO_EPOLL);
#endif
    m_poListener->SetPacketParser(&m_oRCPacketParser);
    m_poListener->SetSessionFactory(&m_oRCClientMgr);
    SGateListenerInfo& stInfo = CGSConfig::Instance()->GetGateListenerInfo();
    m_poListener->SetBufferSize(stInfo.m_dwRecvBuf, stInfo.m_dwSendBuf);
    if (m_poListener->Start(stInfo.m_strIP.c_str(), stInfo.m_wPort, FALSE) != TRUE)
    {
        USR_INFO(_SDT("[%s:%d]:Listener Start[%s:%u] Failed"), MSG_MARK, stInfo.m_strIP.c_str(), stInfo.m_wPort);
        //return FALSE;
    }
    if(!m_oRCClientMgr.Init())
    {
        USR_INFO(_SDT("m_oRCClientMgr init Failed"));
        return FALSE;
    }

    if(!m_oRechargeClientMgr.Init())
    {
        USR_INFO(_SDT("CRechargeClientMgr init Failed"));
        return FALSE;
    }

    return TRUE;
}


VOID CGSApplication::UnInit()
{
    //先停止底层网络模块,阻止新的消息过来
    if(m_poListener)
    {
        m_poListener->Stop();
    }
    CSDApplication::UnInit();
    CLogicModel::Instance()->OnAtExit();	//逻辑模块退出前的回调(保存工作等等)
    m_oDBMgr.Release();
    CGSConfig::Instance()->UnInit();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
    //已经退出程序，由系统回收
    //SDDelete m_pSQLMemoryPool;
    //m_pSQLMemoryPool = NULL;
}

BOOL CGSApplication::Run()
{
	BOOL bSleep = FALSE;
    //如果服务器不处于繁忙状态进行SDSleep操作释放cpu资源
    if(FALSE == CSDApplication::Run())
    {
        bSleep = TRUE;
    }

    CTimerEventMgr::Instance()->Run();

    CUserMgr::Instance()->CkUserDisconnect();

    // 根据本服务器类型决定是否执行哪些数据库语句
    if(FALSE == m_oDBMgr.Run(IsServerType(TYPE_PK_SERVER)))
    {
        bSleep = TRUE;
    }

    if( CGSConfig::Instance()->GetPayType() == PAY_TYPE_TONGBU )
    {
        //只有同步充值才需要连接专门的充值服务器
        // m_oRechargeClientMgr.Run();
        m_oRCClientMgr.ChkSessionOnTimer();
    }

	CLogicModel::Instance()->Run();
	if ( bSleep )
	{
		SDSleep(1);
	}
    return TRUE;
}

CSDPipeChannel* CGSApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel* poPipeChannel = NULL;
    CSDServerID oServerID(dwPipeID);

    switch(oServerID.GetServerType())
    {
    case TYPE_GATE_SERVER:
        poPipeChannel = SDNew CGTPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oGTProcessor);
        m_mapGTPipeChannel[oServerID.GetAreaID()] = poPipeChannel;

        printf(_SDT("GameServer:recv gate server pipe connect\n"));
        USR_INFO(_SDT("[%s: %d]:recv gate server pipe connect\n"), MSG_MARK);
        break;

    case TYPE_NOTICE_SERVER:
        poPipeChannel = SDNew CSDPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oNSProcessor);
        m_poNSPipeChannel = poPipeChannel;

        printf(_SDT("GameServer:recv notice server pipe connect\n"));
        USR_INFO(_SDT("[%s: %d]:recv notice server pipe connect\n"), MSG_MARK);
        break;

    case TYPE_GM_SERVER:
        poPipeChannel = SDNew CSDPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oGMProcessor);
        m_poGMPipeChannel = poPipeChannel;

        printf(_SDT("GameServer:recv gm server pipe connect\n"));
        USR_INFO(_SDT("[%s: %d]:recv gm server pipe connect\n"), MSG_MARK);
        break;

    case TYPE_GAME_SERVER:
        // 根据本服务器是游戏服务器还是跨服战服务器来决定应对方式
        if(IsServerType(TYPE_GAME_SERVER))
        {
            // 游戏服务器：与对方游戏服务器建立一对一连接
            poPipeChannel = SDNew CGSPipeChannel;
            poPipeChannel->SetPacketProcessor(CGSProcessor::Instance());
            m_poGSPipeChannel = poPipeChannel;

            printf(_SDT("[%s: %d]:gameserver recv gameserver[zoneid=%u] pipe connect\n"), MSG_MARK, oServerID.GetAreaID());
            USR_INFO(_SDT("[%s: %d]:gameserver  recv gameserver[zoneid=%u] pipe connect\n"), MSG_MARK, oServerID.GetAreaID());
        }
        else if(IsServerType(TYPE_PK_SERVER))
        {
            // 跨服战服务器：将对方游戏服务器存入本地列表
            poPipeChannel = SDNew CSDPipeChannel;
            poPipeChannel->SetPacketProcessor(CPKProcessor::Instance());
            m_mapGSPipeChannel[oServerID.GetAreaID()] = poPipeChannel;

            printf(_SDT("[%s: %d]:pkserver recv gameserver[zoneid=%u] pipe connect\n"), MSG_MARK, oServerID.GetAreaID());
            USR_INFO(_SDT("[%s: %d]:pkserver recv gameserver[zoneid=%u] pipe connect\n"), MSG_MARK, oServerID.GetAreaID());
        }
        break;

    case TYPE_PK_SERVER:
        poPipeChannel = SDNew CPKPipeChannel;
        poPipeChannel->SetPacketProcessor(CPKProcessor::Instance());
        m_poPKPipeChannel = poPipeChannel;

        USR_INFO(_SDT("[%s: %d][zoneid=%u]:recv pkserver pipe connect\n"), MSG_MARK, GetZoneID());
        break;

    default:
        printf(_SDT("GameServer:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
        USR_INFO(_SDT("[%s: %d]:recv unknown pipe connect(Type:%d)\n"), MSG_MARK, oServerID.GetServerType());
        break;
    }

    return poPipeChannel;
}


VOID CGSApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (NULL == poChannel)
    {
        return;
    }

    CSDServerID oPeerServerID(poChannel->GetPipeID());

    switch(oPeerServerID.GetServerType())
    {
    case TYPE_NOTICE_SERVER:
        m_poNSPipeChannel = NULL;
        break;

    case TYPE_GM_SERVER:
        m_poGMPipeChannel = NULL;
        break;

    case TYPE_GATE_SERVER:
        m_mapGTPipeChannel.erase(oPeerServerID.GetAreaID());
        break;

    case TYPE_GAME_SERVER:
        // 根据本服务器是游戏服务器还是跨服战服务器来决定应对方式
        if(IsServerType(TYPE_GAME_SERVER))
        {
            // 游戏服务器：断开与对方游戏服务器的一对一连接
            m_poGSPipeChannel = NULL;
            if(m_poListener)
            {
                SGateListenerInfo& stInfo = CGSConfig::Instance()->GetGateListenerInfo();
                if (m_poListener->Start(stInfo.m_strIP.c_str(), stInfo.m_wPort) != TRUE)
                {
                    USR_INFO(_SDT("Listener Start Failed 2"));
                    printf("Listener Start Failed2, IP[%s], Port[%d]", stInfo.m_strIP.c_str(), stInfo.m_wPort);
                }
            }

            printf(_SDT("[%s: %d]:recv gameserver[zoneid=%u] pipe disconnect\n"), MSG_MARK, oPeerServerID.GetAreaID());
            USR_INFO(_SDT("[%s: %d]:recv gameserver[zoneid=%u] pipe disconnect\n"), MSG_MARK, oPeerServerID.GetAreaID());
        }
        else if(IsServerType(TYPE_PK_SERVER))
        {
            // 跨服战服务器：将对方游戏服务器从本地列表中移除
            m_mapGSPipeChannel.erase(oPeerServerID.GetAreaID());

            printf(_SDT("[%s: %d]:gameserver[zoneid=%u] pipe disconnect\n"), MSG_MARK, oPeerServerID.GetAreaID());
            USR_INFO(_SDT("[%s: %d]:gameserver[zoneid=%u] pipe disconnect\n"), MSG_MARK, oPeerServerID.GetAreaID());
        }

        break;

    case TYPE_PK_SERVER:
        m_poPKPipeChannel = NULL;
        break;
    }

    SDDelete poChannel;
}

VOID CGSApplication::NoticeMsg(UINT32 dwTransID, const CHAR* pBuf)
{
    if (NULL != m_poNSPipeChannel)
    {
        m_poNSPipeChannel->SendMsg(dwTransID, GSNS_PLAYER_DEFAULT_NTF, pBuf);
    }
}


VOID CGSApplication::NoticeMsg(UINT32 dwTransID, UINT8 byAction, const CHAR* szToken, const CHAR* pBuf)
{
    PKT_GSNS_PLAYER_DEFAULT_NTF stNtf;
    memset(&stNtf, 0x00, sizeof(PKT_GSNS_PLAYER_DEFAULT_NTF));
    stNtf.dwPlayerID = dwTransID;
    stNtf.byAction = byAction;
    memcpy(stNtf.abyToken, szToken, MAX_TOKEN_NUM);
    memcpy(stNtf.aszPayLoad, pBuf, strlen(pBuf));
    if (NULL != m_poNSPipeChannel)
    {
        m_poNSPipeChannel->SendMsg(dwTransID, GSNS_PLAYER_DEFAULT_NTF, (CHAR*)&stNtf);
    }
}


//VOID CGSApplication::GMMsgResponse(UINT32 dwMsgID, UINT32 dwPlayerID, UINT32 dwSerialNumbe, UINT16 wErrCode)
//{
//    PKT_GMGS_PARM_INFO_ACK stAck;
//    memset(&stAck, 0x00, sizeof(PKT_GMGS_PARM_INFO_ACK));
//	stAck.wErrCode = wErrCode;
//    stAck.dwPlayerID = dwPlayerID;
//    stAck.dwSerialNumber = dwSerialNumbe;
//    if (NULL != m_poGMPipeChannel)
//    {
//        m_poGMPipeChannel->SendMsg(dwPlayerID, dwMsgID, (CHAR*)&stAck);
//    }
//}

VOID CGSApplication::GMMsgResponse(UINT32 dwMsgID, UINT32 dwPlayerID, const CHAR* pBuf)
{
    if (NULL != m_poGMPipeChannel)
    {
        m_poGMPipeChannel->SendMsg(dwPlayerID, dwMsgID, (CHAR*)pBuf);
    }
}

VOID CGSApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    if(!DealUserCmd(szCmd))
    {
        CSDApplication::OnHandleUserCmd(szCmd);
    }
}

VOID CGSApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}


CSDPipeChannel* CGSApplication::GetGTPipeChannel(UINT16 wZoneID)
{
    CZoneID2PipeChannelMapItr itr = m_mapGTPipeChannel.find(wZoneID);
    if(itr != m_mapGTPipeChannel.end())
    {
        return itr->second;
    }
    return NULL;
}

CSDPipeChannel*	CGSApplication::GetGSPipeChannelByZoneID(UINT16 wZoneID)
{
    CZoneID2PipeChannelMapItr itr = m_mapGSPipeChannel.find(wZoneID);
    if(itr != m_mapGSPipeChannel.end())
    {
        return itr->second;
    }
    return NULL;
}

CSDPipeChannel* CGSApplication::GetNSPipeChannel()
{
    return m_poNSPipeChannel;
}

CSDPipeChannel*	CGSApplication::GetGSPipeChannel()
{
    return m_poGSPipeChannel;
}

CSDPipeChannel*	CGSApplication::GetPKPipeChannel()
{
    return m_poPKPipeChannel;
}

CSDApplication* SDGetApp()
{
    static CGSApplication oGSApplication;
    return &oGSApplication ;
}

BOOL CGSApplication::DealUserCmd(const TCHAR* szCmd)
{
    USR_INFO(_SDT("[%s: %d]: DealUserCmd[%s]"), MSG_MARK, szCmd);
    return TRUE;
}

VOID* CGSApplication::GetSQLMemory()
{
    void * p = NULL;
    m_oSQLLock.Lock();
    p = m_pSQLMemoryPool->Malloc();
    m_oSQLLock.Unlock();
    return p;
}

VOID CGSApplication::FreeSQL(VOID* p)
{
    m_oSQLLock.Lock();
    m_pSQLMemoryPool->Free(p);
    m_oSQLLock.Unlock();
}

ZoneIDSet CGSApplication::GetZoneList()
{
    ZoneIDSet setZoneID;
    for(CZoneID2GSPipeChannelMap::iterator itr = m_mapGSPipeChannel.begin(); itr != m_mapGSPipeChannel.end(); ++itr)
    {
        UINT16 wZoneID = itr->first;
        setZoneID.insert(wZoneID);
    }

    return setZoneID;
}

BOOL CGSApplication::IsServerType(eServerType eST)
{
	return eST == m_eThisServerType;
}

eServerType	CGSApplication::GetServerType()
{
    CSDServerID	oServerID(GetLocalID());
    return (eServerType)oServerID.GetServerType();
}
