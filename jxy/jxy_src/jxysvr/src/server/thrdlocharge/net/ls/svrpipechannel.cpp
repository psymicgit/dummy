#include "svrpipechannel.h"
#include "sdutil.h"

VOID CSvrPipeChannel::OnRecv(UINT16 wBusinessID, const CHAR *pData, UINT32 dwLen)
{
    if (m_poPacketProcessor)
    {
        CSDProtocol* poProtocol = m_poPacketProcessor->GetProtocol();
        if (poProtocol)
        {
            SDPipeMsgHeader* pHeader = (SDPipeMsgHeader*)pData;
            pHeader->dwTransID = SGDP::SDNtohl(pHeader->dwTransID);
            pHeader->wMsgID = SGDP::SDNtohs(pHeader->wMsgID);
			pHeader->wMark = m_wZoneID; //¶àLSÁ¬½Ó
            BOOL  bNeedSend = FALSE;
            PPROCESSFUNC pFunc = m_poPacketProcessor->LookupProcessFunc(pHeader->wMsgID);
            if (NULL == pFunc)
            {
                bNeedSend = TRUE;
            }
            else
            {
                static CHAR  DecodeData[PACKET_LENGTH] = {0};
                INT32 nDecodeLen = poProtocol->Decode( pHeader->wMsgID,
                                                       (CHAR*)(pData + sizeof(SDPipeMsgHeader) ) ,
                                                       dwLen - sizeof(SDPipeMsgHeader),
                                                       DecodeData,
                                                       PACKET_LENGTH);
                if ( nDecodeLen > 0)
                {

                    BOOL bRet = m_poPacketProcessor->ProcessPacket(this,
                                pHeader,
                                DecodeData,
                                nDecodeLen);
                    if (TRUE == bRet)
                    {
                        bNeedSend = TRUE;
                    }
                }
                else
                {
                    CSDServerID servierid(GetPipeID());
                    SYS_CRITICAL(_SDT("decode packet error: pipeid=%s, ip=%s, MsgID:%u"), SDServerIDUtoa(GetPipeID()).c_str(), GetRemoteIPStr().c_str(), pHeader->wMsgID);
                }
            }
        }
    }
}

