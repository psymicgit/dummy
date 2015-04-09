#ifndef __KS_PACKETPROCESSOR_INCLUDE_H_
#define __KS_PACKETPROCESSOR_INCLUDE_H_

#include "sdframework.h"
#include <protocol/server/protogtgs.h>
#include <net/cli/usermgr.h>
#include "common/server/ngstring.h"


using namespace SGDP;
using namespace std;


class CGMProcessor :public CSDPacketProcessor
{
public:
    CGMProcessor();
    ~CGMProcessor();
public:
    virtual BOOL  Init() ;
    virtual CSDProtocol* GetProtocol() ;

public:
	static UINT16 GmReq(UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR aszParam4[MAX_PARM_LEN], UINT32 dwTransID = 0, string strDesc = "");
	static UINT16 GmReq(CPlayer* poPlayer, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR aszParam4[MAX_PARM_LEN], UINT32 dwTransID = 0, string strDesc = "");

	
	static UINT16 InsanceBattle(UINT32 dwPlayerID, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byInstanceNum, UINT16 wBattleNum, CNGString& strRet);

    static VOID	PassAllCommonInstance(CPlayer* poPlayer);

private:
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////// 未创建CUser时的协议,使用CGTPipeChannel处理 ///////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    static BOOL	OnParmInfoReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//static BOOL	OnGetEquipInfoReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    VOID JoinFaction(TCHAR robotName[]);

};

#endif

