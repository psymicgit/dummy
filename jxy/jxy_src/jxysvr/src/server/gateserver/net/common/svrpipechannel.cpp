#include "svrpipechannel.h"
#include "framework/gtapplication.h"

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

            if (TRUE == bNeedSend)
            {
                //与ls通讯使用CGTClient的sessionID
                CGTClient* poCliSession = NULL;
                if((CLILS_LOGIN_ACK == pHeader->wMsgID)
                        || (CLILS_REGISTER_ACK == pHeader->wMsgID)
                        || (CLILS_BIND_ACK == pHeader->wMsgID)
                        || (CLILS_UNBIND_ACK == pHeader->wMsgID)
                        || (CLILS_LOGOUT_ACK == pHeader->wMsgID)
                        || (CLILS_CHANGE_PWD_ACK == pHeader->wMsgID)
						|| (CLILS_BIND_MOBILE_ACK == pHeader->wMsgID)
						|| (CLILS_BIND_CHECK_VER_CODE_ACK == pHeader->wMsgID)
						|| (CLILS_UNBIND_MOBILE_ACK == pHeader->wMsgID)
						|| (CLILS_UNBIND_CHECK_VER_CODE_ACK == pHeader->wMsgID)
						|| (CLILS_GET_USER_EXPAND_ACK == pHeader->wMsgID))
						
                {
                    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
                    poCliSession = poGTApplication->GetClientMgr()->FindCliSession(pHeader->dwTransID);
                }
                else
                {
                    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
                    CUser* poUser = CUserMgr::Instance()->FindUser(pHeader->dwTransID);
                    if(NULL == poUser)
                    {
                        return;
                    }
                    poCliSession = poUser->GetGTClient();
                }
                if (poCliSession)
                {
                    poCliSession->Send(pHeader->wMsgID, pData + sizeof(SDPipeMsgHeader), dwLen - sizeof(SDPipeMsgHeader));
                }
            }

        }
    }
}

