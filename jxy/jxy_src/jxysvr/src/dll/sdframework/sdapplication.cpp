#include "sdapplication.h"
#include "sdutil.h"
#include "sdloggerimpl.h"
#include "sdfile.h"
#include "sddir.h"
#include "sddebug.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include "sdstring.h"
#include "sdserverid.h"
#include "sddb.h"

#include "keepalive/controlkeepalive.h"

namespace SGDP
{
#define CONSOLE_CONNECTION_STARTLINE        1
    
    CSDApplication* g_pSDApplication = NULL;

    CSDApplication::CSDApplication()
    {
        m_poNetModule           = NULL;
        m_poPipeModule          = NULL;
        m_bTerminated           = FALSE;
        g_pSDApplication        = this;
        _DeleteAllPipeInfo();
    }

    CSDApplication::~CSDApplication()
    {
    }

    BOOL CSDApplication::Init(const TCHAR* pszConfigFilename)
    {
        m_strConfigFilename = pszConfigFilename;
        if ( FALSE == m_oConfig.Init(pszConfigFilename))
        {
			printf("Init config error\n");
            m_oConfig.UnInit();
            return FALSE;
        }

        //加载配置文件
        if (FALSE == CSDLoggerImpl::Init(pszConfigFilename))
        {
			printf("Init CSDLoggerImpl error\n");
            return FALSE;
        }

        if (FALSE == _InitComponent())
        {
			printf("Init _InitComponent error\n");
            return FALSE;
        }

        m_oNetLogger.Init(_SDT("[SDNET] "));
        SGDP::SDNetSetLogger(&m_oNetLogger, m_oConfig.GetNetLevel());

        m_oPipeLogger.Init(_SDT("[SDPIPE] "));
        SGDP::SDPipeSetLogger(&m_oPipeLogger, m_oConfig.GetPipeLevel());

		m_oDBLogger.Init(_SDT("[SDDB] "));
		SGDP::SDDBSetLogger(&m_oDBLogger, m_oConfig.GetDbLevel());

        if (!m_poPipeModule->Init(m_oConfig.GetPipeFile().c_str(), m_oConfig.GetIpListFile().c_str(), &m_oPipeChannelMgr, m_poNetModule))
        {
			printf(_SDT("[%s: %d]:CSDApplication::_InitComponent, Init PipeModule failed"), MSG_MARK);
            USR_INFO(_SDT("[%s: %d]:CSDApplication::_InitComponent, Init PipeModule failed"), MSG_MARK);
            return FALSE;
        }
        _InitPipeInfoList(m_oConfig.GetPipeFile());
        m_oPipeChannelMgr.Init(m_poPipeModule);
        if (m_oConfig.GetShowConnectionStatus())
        {
            _UpdateSvrStatus();
        }


		//////////////////////////////////////////////////////////////////////////
		if(FALSE == CControlKeepAlive::CreateInstance()) //初始化控制参数
		{
			printf("CreateInstance CControlKeepAlive error\n");
			USR_INFO(_SDT("[%s: %d]:CSDApplication::CControlKeepAlive, CreateInstance failed"), MSG_MARK);
			return FALSE;
		}

		if(FALSE == CControlKeepAlive::Instance()->Init())
		{
			printf("Init CControlKeepAlive error\n");
			USR_INFO(_SDT("[%s: %d]:CSDApplication::CControlKeepAlive Init Faild\n"), MSG_MARK);
			return FALSE;
		}

        return TRUE;
    }

    VOID CSDApplication::OnSetCmdLine(INT32 argc, TCHAR** argv)
    {
        tstring strCommandLine = _SDT(""); 
        for (INT32 n = 0; n < argc; n ++ )
        {
            strCommandLine += _SDT(" ");
            strCommandLine += argv[n];
        }
        GetCmdLine()->SetCommandLine(strCommandLine.c_str());
    }

    VOID CSDApplication::OnHandleUserCmd(const TCHAR* szCmd)
    {
        if (_SDTStrncmp(_SDTStrlwr((TCHAR*)szCmd),_SDT("quit"),_SDTStrlen(szCmd))==0)
        {
            Terminate();
        } 
        else if (_SDTStrncmp(_SDTStrlwr((TCHAR*)szCmd),_SDT("exit"),_SDTStrlen(szCmd))==0)
        {
            Terminate();
        }
        else if (_SDTStrncmp(_SDTStrlwr((TCHAR*)szCmd),_SDT("show"),_SDTStrlen(szCmd))==0)
        {
            m_oConfig.SetShowConnectionStatus(TRUE);
            _UpdateSvrStatus();
        }
        else if (_SDTStrncmp(_SDTStrlwr((TCHAR*)szCmd),_SDT("hide"),_SDTStrlen(szCmd))==0)
        {
            m_oConfig.SetShowConnectionStatus(FALSE);
            _UpdateSvrStatus();
        }
    }

    CSDCommandLine* CSDApplication::GetCmdLine()
    {
        return &m_oSDCommandLine;
    }

    VOID CSDApplication::SetFixedLineCount(INT32 nCount)
    {
        //m_nMaxFixedAreaLines = nCount;
    }

    VOID CSDApplication::UnInit()
    {
        _DeleteAllPipeInfo();
        _UnInitComponent();	
		
		//检测一下是否已经创建，防止异常
		if(TRUE == CControlKeepAlive::CreateInstance()) //初始化控制参数
		{
			CControlKeepAlive::Instance()->UnInit();
		}
    }

	VOID CSDApplication::UnInitLog()
	{
		CSDLoggerImpl::UnInit();
	}

	VOID CSDApplication::UnInitConsole()
	{
	}

    BOOL CSDApplication::Run()
    {
        BOOL bBusy = FALSE;
        if (!m_bTerminated)
        {
            if (m_poNetModule->Run(m_oConfig.GetNetModuleCount()))
                bBusy = TRUE;

            if (m_poPipeModule->Run(m_oConfig.GetPipeModuleCount()))
                bBusy = TRUE;

			//修改共享内存标志，标记自己还存活着。
			CControlKeepAlive::Instance()->OnAlive();

			//检测程序是否要关闭
			if(CControlKeepAlive::Instance()->IsStop())
				Terminate();
            //m_poConsole->Run();
        }
        return bBusy;
    }


    BOOL CSDApplication::_InitComponent()
    {
		m_poNetModule = SGDP::SDNetGetModule(&SDNET_VERSION);
		if(NULL == m_poNetModule)
		{
			USR_INFO(_SDT("GetNetModule failed"));
			return FALSE;
		}
		m_poPipeModule = SGDP::SDPipeGetModule(&SDPIPE_VERSION);
		if(!m_poPipeModule)
		{
			USR_INFO(_SDT("GetPipeModule failed"));
			return FALSE;
		}
        return TRUE;
    }

    VOID CSDApplication::_UnInitComponent()
    {
        if (m_poPipeModule)
        {
            m_poPipeModule->Release();
            m_poPipeModule = NULL;
        }  
        if (m_poNetModule)
        {
            m_poNetModule->Release();
            m_poNetModule = NULL;
        }
		//if(m_poConsole)
		//{
		//	m_poConsole->Release();
		//	m_poConsole = NULL;
		//}
		//SDCloseShmem(&m_shmemPipeInfo);		
		m_oConfig.UnInit();
    }

    ISDNet* CSDApplication::GetNetModule()
    {
        return m_poNetModule;
    }

    VOID CSDApplication::Terminate()
    {
        m_bTerminated = TRUE;
    }

    CSDConfig*  CSDApplication::GetConfig()
    {
        return &m_oConfig;
    }
    //CPipeInfoVect*  CSDApplication::GetPipeVect()
    //{
    //    return m_pvecPipeInfo;
    //}

    BOOL CSDApplication::IsTerminated()
    {
        return m_bTerminated;
    }

    CSDPipeChannelMgr*	CSDApplication::GetPipeChannelMgr()
    {
        return &m_oPipeChannelMgr;
    }

    VOID  CSDApplication::PrintScreen(const TCHAR *format, ...)
    {
        va_list args;
        va_start(args, format);
        TCHAR szMsg[1024];
        INT32 n2 = _SDTvsnprintf(szMsg,1024, format, args);
        szMsg[n2] = _SDT('\0');
        va_end(args);

        //if (m_poConsole)
        //{
        //    m_poConsole->WriteString(szMsg);
        //}
    }

    VOID  CSDApplication::UpdateScreen(const UINT16 wLine ,const TCHAR *format, ...)
    {
        va_list args;
        va_start(args, format);

        TCHAR szMsg[1024];

        INT32 n2 = _SDTvsnprintf(szMsg,1024, format, args);

        szMsg[n2] = _SDT('\0');
        va_end(args);

        //if (m_poConsole)
        //{
        //    m_poConsole->WriteStringFixed(wLine, szMsg);
        //}
    }

    VOID CSDApplication::UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect)
    {
        SPipeInfo* pPipeInfo = _FindPipeInfo(dwID);
        if (NULL != pPipeInfo)
        {
            pPipeInfo->bConnect = bConnect;
        }
        else
		{	
			//if(m_pstAllPipeInfo->byPipeNum >= MAX_PIPE_CONNECT_NUM)
			//{
			//	//异常。。。
			//	return;
			//}			
            //pPipeInfo = SDNew SPipeInfo;
			//SPipeInfo* pPipeInfo = &(m_pstAllPipeInfo->astPipeInfo[m_pstAllPipeInfo->byPipeNum++]);
			SPipeInfo stPipeInfo;
            memset(&stPipeInfo,0,sizeof(SPipeInfo));
            stPipeInfo.bConnect = bConnect;
            stPipeInfo.bDirection = TRUE;
            stPipeInfo.dwID = dwID;
            stPipeInfo.dwIP = dwIP;
            _SDTStrcpy(stPipeInfo.szIP,SDInetNtoa(dwIP).c_str());
			m_vecPipeInfo.push_back(stPipeInfo);
            //m_pvecPipeInfo->push_back(pPipeInfo);

        }
        //if (m_oConfig.GetShowConnectionStatus())
        //    _UpdateSvrStatus();
    }


    VOID CSDApplication::_UpdateSvrStatus()
    {
        //INT32 nLineNo = CONSOLE_CONNECTION_STARTLINE + 1;
        //CPipeInfoVectItr itr = m_pvecPipeInfo->begin();
        //for (; itr != m_pvecPipeInfo->end(); ++ itr )
        //{
        //    SPipeInfo* pInfo = (*itr);
        //    if (pInfo)
        //    {
        //        CSDServerID serverId(pInfo->dwID);
        //        tstring strServerName = GetServerShortName(serverId.GetServerType());
        //        tstring strIP         = SDInetNtoa(pInfo->dwIP);
        //        UINT16 wPort          = pInfo->wPort;
        //        tstring strStatus     = pInfo->bConnect ? _SDT("Connected") : _SDT("Disconnected");
        //        tstring strDirection  = pInfo->bDirection ? _SDT("Listener") : _SDT("Connection");

        //        TCHAR szBuf[256];
        //        _SDTsnprintf(szBuf,256, _SDT("%2s%8s%16s:%6d %s%18s"), 
        //                            serverId.AsString().c_str(), 
        //                            strServerName.c_str(),
        //                            strIP.c_str(),
        //                            wPort,
        //                            strStatus.c_str(),
        //                            strDirection.c_str());

        //        //m_poConsole->WriteStringFixed(nLineNo++, _SDT("%s"), szBuf);
        //    }
        //}
    }

    SPipeInfo* CSDApplication::_FindPipeInfo(UINT32 dwServerId)
    {
        //CPipeInfoVectItr itr = m_pvecPipeInfo->begin();
        //for (; itr != m_pvecPipeInfo->end(); itr++ )
        //{
        //    SPipeInfo* pPipeInfo = *itr;
        //    if (pPipeInfo && pPipeInfo->dwID == dwServerId)
        //    {
        //        return pPipeInfo;
        //    }
        //}
		/*
		for(UINT8 byIdx = 0; byIdx < m_pstAllPipeInfo->byPipeNum; byIdx++)
		{
			SPipeInfo* pstInfo = &(m_pstAllPipeInfo->astPipeInfo[byIdx]);
			if(pstInfo->dwID == dwServerId)
			{
				return pstInfo;
			}
		}
		*/
		for(vector<SPipeInfo>::iterator itr = m_vecPipeInfo.begin(); itr != m_vecPipeInfo.begin(); itr++)
		{
			SPipeInfo& stInfo = *itr;
			if(stInfo.dwID == dwServerId)
			{
				return &stInfo;
			}
		}

        return NULL;
    }

    VOID  CSDApplication::_InitPipeInfoList(const string strPipeFile)
    {
        _DeleteAllPipeInfo();
        TiXmlDocument xml;
        if ( xml.LoadFile( strPipeFile.c_str() ) )
        {
            TiXmlElement* pRoot = xml.RootElement();
            if (pRoot)
            {
                TiXmlElement* pConnectionsElement = pRoot->FirstChildElement(ELEMET_ITEM_CONNECTIONS);
                if (pConnectionsElement)
                {
                    TiXmlElement* pConnectionElement = pConnectionsElement->FirstChildElement(ELEMET_ITEM_CONNECTION);
                    while(pConnectionElement)
                    {
                        CHAR* pszMode = (CHAR*)pConnectionElement->Attribute(ATTR_MODE);
                        if (pszMode && strcmp ( pszMode , ATTR_MODE_CONNECT ) == 0)
                        {
                            TiXmlElement* pItemElement = pConnectionElement->FirstChildElement(ELEMET_ITEM_ITEM_SMALL);
                            while (pItemElement)
                            {
								//if(m_pstAllPipeInfo->byPipeNum >= MAX_PIPE_CONNECT_NUM)
								//{
								//	//异常
								//	assert(1);
								//	return;
								//}			
								//pPipeInfo = SDNew SPipeInfo;
								//SPipeInfo* pSPipeInfo = &(m_pstAllPipeInfo->astPipeInfo[m_pstAllPipeInfo->byPipeNum++]);
                                //SPipeInfo* pSPipeInfo = SDNew SPipeInfo;
								SPipeInfo stPipeInfo;
                                memset(&stPipeInfo,0,sizeof(SPipeInfo));
                                CHAR* pszValue = (CHAR*)pItemElement->Attribute(ATTRI_ID);
                                if (pszValue)
                                {
                                    stPipeInfo.dwID = SDServerIDAtou(pszValue);
                                }
                                pszValue = (CHAR*)pItemElement->Attribute(ATTRI_REMOTE_IP);
                                if (pszValue)
                                {
                                    stPipeInfo.dwIP = SDInetAddr(_SDTA2T(pszValue));
                                }
                                pszValue = (CHAR*)pItemElement->Attribute(ATTRI_REMOTE_PORT);
                                if (pszValue)
                                {
                                    stPipeInfo.wPort = SDAtoi(pszValue);
                                }    
                                stPipeInfo.bConnect = FALSE;
                                //m_pvecPipeInfo->push_back(pSPipeInfo);
                                pItemElement = pItemElement->NextSiblingElement(ELEMET_ITEM_ITEM_SMALL);
                            }
                        }
                        pConnectionElement = pConnectionElement->NextSiblingElement(ELEMET_ITEM_CONNECTION);
                    }
                }
            }
        }
    }

    VOID  CSDApplication::_DeleteAllPipeInfo()
	{
		//if(NULL == m_pvecPipeInfo)
		//{
		//	return;
		//}
		//CPipeInfoVectItr itr = m_pvecPipeInfo->begin();
		//for (; itr != m_pvecPipeInfo->end(); itr++ )
		//{
		//	SPipeInfo* pPipeInfo = *itr;
		//	if (pPipeInfo)
		//		SDDelete pPipeInfo;
		//}
		//m_pvecPipeInfo->clear();
    }

	UINT32 CSDApplication::GetLocalID()
	{
		if(m_poPipeModule)
		{
			return m_poPipeModule->GetLocalID();
		}
		return 0;
	}

	UINT16 CSDApplication::GetZoneID()
	{
		CSDServerID	oServerID(GetLocalID());
		return (UINT16)(oServerID.GetAreaID());
	}
}

