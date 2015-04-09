

#include "gsapplication.h"
#include "gsapi.h"
#include "dll/sdframework/sdapplication.h"
using namespace SGDP;
namespace gsapi
{
    CDBMgr* GetDBMgr()
    {
        return ((CGSApplication*)SDGetApp())->GetDBMgr();
    }
    
    CRechargeClientMgr* GetRechargeClientMgr()
    {
        return ((CGSApplication*)SDGetApp())->GetRechargeClientMgr();
    }
    
    CRCClientMgr* GetRCClientMgr()
    {
        return ((CGSApplication*)SDGetApp())->GetRCClientMgr();
    }

    ISDListener* GetRechargeListener()
    { 
        return ((CGSApplication*)SDGetApp())->GetRechargeListener();
    }	

    CDBProcessor* GetDBProcessor()
    {
        return ((CGSApplication*)SDGetApp())->GetDBProcessor();
    }

    CSDPipeChannel* GetGTPipeChannel(UINT16 wZoneID)
    {
        return ((CGSApplication*)SDGetApp())->GetGTPipeChannel(wZoneID);
    }

    CSDPipeChannel*	GetGSPipeChannel()
    {
        return ((CGSApplication*)SDGetApp())->GetGSPipeChannel();
    }

    CGSApplication*	GetGS()
    {
        return (CGSApplication*)SDGetApp();
    }

    BOOL IsConnectingToGS(UINT16 wZoneID)
    {
        CSDPipeChannel *poPipeChannel = ((CGSApplication*)SDGetApp())->GetGSPipeChannelByZoneID(wZoneID);
        if(NULL == poPipeChannel)
        {
            return FALSE;
        }

        return TRUE;
    }

    VOID SendMsg2GS(UINT16 wZoneID, const CHAR* pData, const UINT32 dwMsgID)
    {
        CSDPipeChannel *poPipeChannel = ((CGSApplication*)SDGetApp())->GetGSPipeChannelByZoneID(wZoneID);
        if(NULL == poPipeChannel)
        {
            return;
        }

        poPipeChannel->SendMsg(wZoneID, dwMsgID, pData);
    }

    VOID SendMsg2PK(const CHAR* pData, const UINT32 dwMsgID)
    {
        CSDPipeChannel *poPipeChannel = ((CGSApplication*)SDGetApp())->GetPKPipeChannel();
        if(NULL == poPipeChannel)
        {
            return;
        }

        poPipeChannel->SendMsg(GetZoneID(), dwMsgID, pData);
    }

    ISDNet* GetNetModule()
    {
        return ((CGSApplication*)SDGetApp())->GetNetModule();
    }

    CCliPacketParser& GetPacketParser()
    {
        return ((CGSApplication*)SDGetApp())->GetPacketParser();
    }

    VOID* GetSQLMemory()
    {
        return ((CGSApplication*)SDGetApp())->GetSQLMemory();
    }

    VOID FreeSQL(VOID* p)
    {
        return ((CGSApplication*)SDGetApp())->FreeSQL(p);
    }

    //发送消息推送
    VOID NoticeMsg(UINT32 dwPlayerID, const CHAR* pBuf)
    {
        ((CGSApplication*)SDGetApp())->NoticeMsg(dwPlayerID, pBuf);
    }

    VOID NoticeMsg(UINT32 dwPlayerID, UINT8 byAction, const CHAR* szToken, const CHAR* pBuf)
    {
        ((CGSApplication*)SDGetApp())->NoticeMsg(dwPlayerID, byAction, szToken, pBuf);
    }

    UINT32 GetLocalID()
    {
        return ((CSDApplication*)SDGetApp())->GetLocalID();
    }

    UINT16 GetZoneID()
    {
        return ((CSDApplication*)SDGetApp())->GetZoneID();
    }

    UINT64 GetServerStartTime()
    {
        return ((CGSApplication*)SDGetApp())->GetServerStartTime();
    }

    BOOL& GetNeedSave2DB()
    {
        return ((CGSApplication*)SDGetApp())->GetNeedSave2DB();
    }

    VOID GMMsgResponse(UINT32 dwMsgID, UINT32 dwPlayerID, const CHAR* pBuf)
    {
        ((CGSApplication*)SDGetApp())->GMMsgResponse(dwMsgID, dwPlayerID, pBuf);
    }
}