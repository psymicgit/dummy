
#include "sdpacketprocessor.h"
#include <sddebug.h>
#include "sdpipechannel.h"
#include "sdutil.h"
#include "sdpkg.h"

namespace SGDP { 



    CCommandMap::CCommandMap()
    {
        m_wArraySize = COMMAND_COUNT;
        m_wArrayStart = COMMAND_STARTINDEX;
        m_wArrayEnd = m_wArrayStart + m_wArraySize;
        m_poFuncArray = SDNew PPROCESSFUNC[m_wArraySize];
	    Reset();
    }

    CCommandMap::~CCommandMap()
    {
	    Reset();
        SDDelete []m_poFuncArray;
        m_poFuncArray = NULL;
    }

    VOID  CCommandMap::Reset()
    {
        if (m_poFuncArray)
        {
            for(INT32 i = 0; i < m_wArraySize; i++)
            {
                m_poFuncArray[i] = NULL;
            }
        }
    }

    BOOL CCommandMap::AddCommand(UINT16 wMsgID, PPROCESSFUNC pFunc, BOOL bForce)
    {
	    if(wMsgID < m_wArrayStart || wMsgID > m_wArrayEnd)
	    {
		    return FALSE;
	    }
	    PPROCESSFUNC pResult = FindCommand(wMsgID);
	    if (NULL == pResult)//此前没有该Function，直接添加
	    {
		    pResult = pFunc;
		    m_poFuncArray[wMsgID - m_wArrayStart] = pResult;
	    }
	    else
	    {
		    if (bForce)//此前有该Function，强制添加
		    {
			    pResult = pFunc;
			    m_poFuncArray[wMsgID - m_wArrayStart] = pResult;
		    }
		    else//此前有该Function，不添加
		    {
			    pResult = NULL;
		    }
	    }
	    return pResult == NULL ? FALSE : TRUE;
    }

    PPROCESSFUNC CCommandMap::FindCommand(UINT16 wMsgID)
    {
	    if(wMsgID < m_wArrayStart || wMsgID > m_wArrayEnd)
	    {
		    return NULL;
	    }
	    return m_poFuncArray[wMsgID - m_wArrayStart];
    }

    PPROCESSFUNC CCommandMap::RemoveCommand(UINT16 wMsgID)
    {
	    if(wMsgID < m_wArrayStart || wMsgID > m_wArrayEnd)
	    {
		    return NULL;
	    }
	    PPROCESSFUNC pFunc = m_poFuncArray[wMsgID - m_wArrayStart];
	    m_poFuncArray[wMsgID - m_wArrayStart] = NULL;
	    return pFunc;
    }

    VOID CCommandMap::InitCommandSize(const UINT16 wArrayStart,const UINT16 wArraySize)
    {
        m_wArrayStart = wArrayStart;
        m_wArraySize = wArraySize;
        m_wArrayEnd = m_wArrayStart + m_wArraySize;
        if (m_poFuncArray)
        {
            SDDelete []m_poFuncArray;
            m_poFuncArray = NULL;
        }
        m_poFuncArray = SDNew PPROCESSFUNC[m_wArraySize];
        Reset();
    }

    //////////////////////////////////////////////////////////////////////////////////

    CSDPacketProcessor::CSDPacketProcessor()
    {
    }

    CSDPacketProcessor::~CSDPacketProcessor()
    {
    }

    PPROCESSFUNC CSDPacketProcessor::LookupProcessFunc(UINT16 wMsgID)
    {
	    return m_mapCommand.FindCommand(wMsgID);
    }

    BOOL CSDPacketProcessor::ProcessPacket(VOID* poReceiver, VOID* pHeader, const CHAR* pBody, UINT32 dwBodyLen)
    {
        SDPipeMsgHeader* pMsgHeader = (SDPipeMsgHeader*)pHeader;
	    PPROCESSFUNC pfnProcessFunc = LookupProcessFunc(pMsgHeader->wMsgID);

	    if (NULL == pfnProcessFunc)
	    {
		    return FALSE;
	    }
	    else
	    {
		    return pfnProcessFunc(poReceiver, pHeader, pBody, dwBodyLen);
	    }
	    return FALSE;
    }

    BOOL CSDPacketProcessor::RegisterCommand(UINT16 wMsgID, PPROCESSFUNC pFunc, BOOL bForce)
    {
	    return m_mapCommand.AddCommand(wMsgID, pFunc, bForce);
    }

    BOOL CSDPacketProcessor::UnRegisterCommand(UINT16 wMsgID)
    {
	    return m_mapCommand.RemoveCommand(wMsgID) == NULL ? FALSE : TRUE;
    }

    VOID CSDPacketProcessor::InitCommandSize(const UINT16 wArrayStart,const UINT16 wArraySize)
    {
        m_mapCommand.InitCommandSize(wArrayStart,wArraySize);
    }
}

