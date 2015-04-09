#ifndef _gsapi_h_
#define _gsapi_h_

#include <sdtype.h>

class CDBMgr;
class CRechargeClientMgr;
class CRCClientMgr;
class CDBProcessor;
class CCliPacketParser;
class CGSApplication;

namespace SGDP
{
    class ISDListener;
    class CSDPipeChannel;
    class ISDNet;
}

using namespace SGDP;

namespace gsapi
{
    CDBMgr*             GetDBMgr();
    CRechargeClientMgr* GetRechargeClientMgr();
    CRCClientMgr*       GetRCClientMgr();
    CDBProcessor*		GetDBProcessor();
    ISDListener*		GetRechargeListener();
    CSDPipeChannel*		GetGTPipeChannel(UINT16 wZoneID);    
    CSDPipeChannel*		GetGSPipeChannel();

    CGSApplication*	GetGS();

    BOOL IsConnectingToGS(UINT16 wZoneID);

    VOID SendMsg2GS(UINT16 wZoneID, const CHAR* pData, const UINT32 dwMsgID);
    VOID SendMsg2PK(const CHAR* pData, const UINT32 dwMsgID);

    ISDNet*             GetNetModule();
    CCliPacketParser&   GetPacketParser();

    VOID*               GetSQLMemory();
    VOID                FreeSQL(VOID* p);

    //发送消息推送
    VOID NoticeMsg(UINT32 dwPlayerID, const CHAR* pBuf);
    VOID NoticeMsg(UINT32 dwPlayerID, UINT8 byAction, const CHAR* szToken, const CHAR* pBuf);

    UINT64 GetServerStartTime();
    BOOL& GetNeedSave2DB();

    VOID GMMsgResponse(UINT32 dwMsgID, UINT32 dwPlayerID, const CHAR* pBuf);

    UINT32 GetLocalID();
    UINT16 GetZoneID();
}

#endif // _gsapi_h_
