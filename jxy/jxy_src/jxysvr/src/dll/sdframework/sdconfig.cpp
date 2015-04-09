#include "sdconfig.h"
#include "sdutil.h"
#include "sdstring.h"
#include "sdfile.h"
#include "sddir.h"
#include "sdserverid.h"

namespace SGDP
{
    CSDConfig::CSDConfig()
    {
	    m_strServerName.clear();
	    m_strFilename.clear();
        m_strPipeFile.clear();
        m_strIpList.clear();
	    m_dwServerID	= 0;
        m_dwNetModuleCount = -1;
        m_dwPipeModuleCount = -1;
        m_bShowConnectionStatus = FALSE;
    }

    CSDConfig::~CSDConfig()
    {
	    UnInit();
    }

    VOID CSDConfig::UnInit()
    {        
    }

    UINT32 CSDConfig::GetServerID()
    {
	    return m_dwServerID;
    }

    std::string CSDConfig::GetServerName()
    {
        return m_strServerName;
    }

    std::string CSDConfig::GetIpListFile()
    {
        return m_strIpList;
    }
    std::string CSDConfig::GetPipeFile()
    {
        return m_strPipeFile;
    }

    UINT32 CSDConfig::GetNetModuleCount()
    {
        return m_dwNetModuleCount;
    }

    UINT32 CSDConfig::GetPipeModuleCount()
    {
        return m_dwPipeModuleCount;
    }

    BOOL CSDConfig::GetShowConnectionStatus()
    {
        return m_bShowConnectionStatus;
    }

    VOID CSDConfig::SetShowConnectionStatus(const BOOL bShowConnectionStatus)
    {
        m_bShowConnectionStatus = bShowConnectionStatus;
    }

    UINT32 CSDConfig::GetMaxUsers()
    {
        return m_dwMaxUsers;
    }

    BOOL CSDConfig::Init(const TCHAR* pszFilename)
    {
	    BOOL  bResult = FALSE;
	    UnInit();
        CSDXMLFile  oSDXMLFile;
        bResult = oSDXMLFile.Load(_SDTT2A(pszFilename));
        if (TRUE != bResult)
        {
            return bResult;
        }
        CSDXMLNode rootNode = oSDXMLFile.GetRootNode();
        m_strServerName = rootNode(ATTRI_SERVER_NAME).AsString();
        string strServerId = rootNode(ATTRI_SERVER_ID).AsString();
        m_dwServerID = SDServerIDAtou(strServerId.c_str());
        m_dwNetModuleCount = rootNode[ELEMET_ITEM_NETMODULE_COUNT].AsInteger(-1);
        m_dwPipeModuleCount = rootNode[ELEMET_ITEM_PIPEMODULE_COUNT].AsInteger(-1);
        m_bShowConnectionStatus = rootNode[ELEMET_ITEM_SHOW_CONNECTION].AsBoolean();
        m_strPipeFile = _SDTT2A(SDGetModulePath());
        m_strPipeFile += rootNode[ELEM_PIPE](ATTRI_FILE).AsString();
        m_strIpList = _SDTT2A(SDGetModulePath());
        m_strIpList += rootNode[ELEM_IPLIST](ATTRI_FILE).AsString();
        m_dwRecvBufferSize = rootNode[ELEMET_ITEM_LISTEN](ATRRI_RECVBUFF).AsInteger();
        m_dwSendBufferSize = rootNode[ELEMET_ITEM_LISTEN](ATRRI_SENDBUFF).AsInteger();
        m_strListenIp = rootNode[ELEMET_ITEM_LISTEN](ATTRI_IP).AsString();
        m_wPort = rootNode[ELEMET_ITEM_LISTEN](ATTRI_PORT).AsInteger();
        m_dwMaxUsers = rootNode[ELEM_MAXUSER].AsInteger();


		m_dwNetLevel = rootNode[ELEM_NET_LOG_LEVEL].AsInteger();
		m_dwPipeLevel = rootNode[ELEM_PIPE_LOG_LEVEL].AsInteger();
		m_dwDbLevel = rootNode[ELEM_DB_LOG_LEVEL].AsInteger();

        m_strFilename = pszFilename;
	    return bResult;
    }

	UINT32 CSDConfig::GetRecvBufferSize()
	{
		return m_dwRecvBufferSize;
	}
	UINT32 CSDConfig::GetSendBufferSize()
	{
		return m_dwSendBufferSize;
	}

    std::string CSDConfig::GetListenIp()
    {
        return m_strListenIp;
    }

    UINT16 CSDConfig::GetListenPort()
    {
        return m_wPort;
    }

	UINT32	CSDConfig::GetNetLevel()
	{
		return m_dwNetLevel;
	}
	UINT32	CSDConfig::GetPipeLevel()
	{
		return m_dwPipeLevel;
	}
	UINT32	CSDConfig::GetDbLevel()
	{
		return m_dwDbLevel;
	}


}


