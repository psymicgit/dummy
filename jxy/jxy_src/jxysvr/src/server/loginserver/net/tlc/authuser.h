

#ifndef _AUTHUSER_H_
#define _AUTHUSER_H_

#include <sdtime.h>
#include <sdstring.h>
#include <map>
#include <common/server/auth.h>
#include "protocol/server/protolstlc.h"
#include "protocol/server/protogtls.h"


using namespace std ;




class CAuthUser
{

public:
    CAuthUser();
    ~CAuthUser();
public:
	BOOL	SetUserProp(UINT32 dwTransID, PKT_GTLS_LOGIN_REQ* pstReq1, PKT_GTLS_LOGIN_EX_REQ* pstReq2);
	UINT32	GetTransID() { return m_dwTransID; }
	UINT64	GetLastRecvMsgTime() { return m_qwLastRecvMsgTime; }

	BOOL	ReqAuth();
	BOOL	OnAuthAck(UINT16 wErrCode);

protected:
	UINT16	Convert2CliErrCode(UINT16 wErrCode);
private:
	
	UINT32					m_dwTransID;
	PKT_GTLS_LOGIN_REQ		m_stReq1;
	PKT_GTLS_LOGIN_EX_REQ	m_stReq2;
	UINT8					m_byProtVerID;

	UINT64					m_qwLastRecvMsgTime; //上次收到信息时间
};


#endif