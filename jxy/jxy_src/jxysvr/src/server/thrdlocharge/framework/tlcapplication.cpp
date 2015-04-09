#include "tlcapplication.h"
#include "tlcconfig.h"
#include <sdutil.h>
#include <logic/auth/authmgr.h>
#include <logic/auth/authusermgr.h>
#include "net/ls/svrpipechannel.h"
#include "logic/recharge/rechargeusermgr.h"
#include "logic/recharge/rechargemgr.h"


CTLCApplication::CTLCApplication()
{    

}

CTLCApplication::~CTLCApplication()
{

}

BOOL CTLCApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
		printf(_SDT("CSDApplication::Init failed"));
        return FALSE;
    }

    if(FALSE == CTLCConfig::CreateInstance())
    {
        return FALSE;
    }
    if(!CTLCConfig::Instance()->Init(pszConfigFilename))
    {
        printf(_SDT("CTLCConfig::Init failed!"));
        return FALSE;
    }

	if (FALSE == CAuthUserMgr::CreateInstance())
	{
		return FALSE;
	}
	if(!CAuthUserMgr::Instance()->Init())
	{
		printf(_SDT("CAuthUserMgr::Init failed!"));
		return FALSE;
	}

	if (FALSE == CAuthMgr::CreateInstance())
	{
		return FALSE;
	}
	if(!CAuthMgr::Instance()->Init())
	{
		printf(_SDT("CAuthMgr::Init failed!"));
		return FALSE;
	}	

	if (FALSE == CRechargeUserMgr::CreateInstance())
	{
		return FALSE;
	}
	if(!CRechargeUserMgr::Instance()->Init())
	{
		printf(_SDT("CRechargeUserMgr::Init failed!"));
		return FALSE;
	}

	if (FALSE == CRechargeMgr::CreateInstance())
	{
		return FALSE;
	}
	if(!CRechargeMgr::Instance()->Init())
	{
		printf(_SDT("CRechargeMgr::Init failed!"));
		return FALSE;
	}	


	printf(_SDT("CTLCApplication::Init success!"));

    return TRUE;
}


VOID CTLCApplication::UnInit()
{
    //先停止底层网络模块,阻止新的消息过来
    CSDApplication::UnInit();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
    //已经退出程序，由系统回收

}

BOOL CTLCApplication::Run()
{
    //CAutoCycle oAutoCycle("CTLCApplication::Run");

    //如果服务器不处于繁忙状态进行SDSleep操作释放cpu资源
    if(FALSE == CSDApplication::Run())
    {
        SDSleep(1);
    }

	CAuthUserMgr::Instance()->Run();
	CAuthMgr::Instance()->Run();
  
    return TRUE;
}

CSDPipeChannel* CTLCApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel* poPipeChannel = NULL;
    CSDServerID oServerID(dwPipeID);

    if (TYPE_LOGIN_SERVER == oServerID.GetServerType())
    {
        poPipeChannel = SDNew CSvrPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oLSProcessor);
		((CSvrPipeChannel*)poPipeChannel)->SetZoneID(oServerID.GetAreaID());
        m_mapLSPipeChannel[oServerID.GetAreaID()] = poPipeChannel;

        printf(_SDT("TLCServer:recv login server pipe connect\n"));
        USR_INFO(_SDT("[%s: %d]:recv login server pipe connect\n"), MSG_MARK);
    }
    else
    {
        printf(_SDT("GameServer:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
        USR_INFO(_SDT("[%s: %d]:recv unknown pipe connect(Type:%d)\n"), MSG_MARK, oServerID.GetServerType());
    }

    return poPipeChannel;
}


VOID CTLCApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (poChannel)
    {
        CSDServerID oServerID(poChannel->GetPipeID());

        if(TYPE_LOGIN_SERVER == oServerID.GetServerType())
        {
            m_mapLSPipeChannel.erase(oServerID.GetAreaID());
        }			

        SDDelete poChannel;
        poChannel = NULL;
    }
}

VOID CTLCApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
	CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}


CSDApplication* SDGetApp()
{
	static CTLCApplication oGSApplication;
	return &oGSApplication ;
}

CSDPipeChannel*	CTLCApplication::GetLSPipeChannel(UINT16 wZoneID)
{
	CZoneID2PipeChannelMapItr itr = m_mapLSPipeChannel.find(wZoneID);
	if(itr != m_mapLSPipeChannel.end())
	{
		return itr->second;
	}

	return NULL;
}

