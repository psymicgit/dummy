///<------------------------------------------------------------------------------
//< @file:   pkapplication.cpp
//< @author: hongkunan
//< @date:   2014年1月2日 15:53:7
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "pkapplication.h"

#include "net/gs/gspipechannel.h"
#include "net/gs/gsprocessor.h"

#include "framework/pkconfig.h"


CSDApplication* SDGetApp()
{
    static CPKApplication s_oPKApplication;
    return &s_oPKApplication ;
}

CPKApplication::CPKApplication()
    : m_poGMPipeChannel(NULL)
    , m_bNeedSave2DB(TRUE)
    , m_poListener(NULL)
{
}

CPKApplication::~CPKApplication()
{
    CLogicModel::Instance()->Destroy();
    CPKConfig::Instance()->DestroyInstance();
}

// 根据区Id查找与该区的gameserver的连接
CGSPipeChannel*	CPKApplication::GetGSPipeChannel(UINT16 wZoneID)
{
    CZoneID2GSPipeChannelMap itr = m_mapGSPipeChannel.find(wZoneID);
    if(itr != m_mapGSPipeChannel.end())
    {
        return itr->second;
    }

    return NULL;
}

BOOL CPKApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CPKApplication::Init(pszConfigFilename))
    {
        return FALSE;
    }

    PrintScreen(_SDT("PKApplication::Init successful"));

    if(FALSE == CPKConfig::CreateInstance())
    {
        return FALSE;
    }
    if(!CPKConfig::Instance()->Init(pszConfigFilename))
    {
        USR_INFO(_SDT("CPKConfig::Init failed!"));
        return FALSE;
    }

    if(!m_oSQLMemoryPool.Create(MAX_SQL_LEN , MAX_SQL_LEN * 4))
    {
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

    return TRUE;
}


CSDPipeChannel* CPKApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel* poPipeChannel = NULL;
    CSDServerID oServerID(dwPipeID);

    switch(oServerID.GetServerType())
    {
    case TYPE_GM_SERVER:
    {

        break;
    }

    case TYPE_GAME_SERVER:
    {
        poPipeChannel = SDNew CGSPipeChannel;
        poPipeChannel->SetPacketProcessor(CGSProcessor::Instance());

        m_mapGSPipeChannel[oServerID.GetAreaID()] = poPipeChannel;
        
        printf(_SDT("PKServer: recv gameserver「zoneid=%u」pipe connect\n"), oServerID.GetAreaID());
        USR_INFO(_SDT("[%s: %d]: recv gameserver「zoneid=%u」pipe connect\n"), MSG_MARK, oServerID.GetAreaID());
        break;
    }

    default:
        printf(_SDT("PKServer:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
        USR_INFO(_SDT("[%s: %d]:recv unknown pipe connect(Type:%d)\n"), MSG_MARK, oServerID.GetServerType());

        break;
    }

    return poPipeChannel;
}

VOID CPKApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (NULL == poChannel)
    {
        return;
    }

    CSDServerID oServerID(poChannel->GetPipeID());

    switch(oServerID.GetServerType())
    {
    case TYPE_GM_SERVER:
        break;

    case TYPE_GAME_SERVER:
        m_mapGSPipeChannel.erase(oServerID.GetAreaID());
        break;
    }

    SDDelete poChannel;
}

VOID CPKApplication::UnInit()
{
    //先停止底层网络模块,阻止新的消息过来
    if(m_poListener)
    {
        m_poListener->Stop();
    }

    CSDApplication::UnInit();
    CLogicModel::Instance()->OnAtExit();	//逻辑模块退出前的回调(保存工作等等)
    CLogicModel::Instance()->Stop();
    m_oDBMgr.Release();

    CPKConfig::Instance()->UnInit();

    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
}
