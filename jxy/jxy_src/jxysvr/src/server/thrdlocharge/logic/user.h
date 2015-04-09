

#ifndef _USER_H_
#define _USER_H_

#include <sdtime.h>
#include <sdstring.h>
#include <map>
#include <common/server/auth.h>
#include "protocol/server/protolstlc.h"


using namespace std ;




class CUserMgr;
class CUser:public CAuthClient
{
	friend class CUserMgr;
public:
    CUser();
    ~CUser();
public:
	virtual	VOID SDAPI	OnAuthSuccess();	
	virtual	VOID SDAPI	OnAuthFailed(UINT16 wErrCode, TCHAR* pszErrCode);

public:
	VOID SetUserProp(UINT16 wZoneID, UINT32 dwTransID, PKT_LSTLC_AUTH_REQ& stReq);

	inline UINT32	GetZoneID() { return m_wZoneID; }
	inline UINT32	GetTransID() { return m_dwTransID; }
	inline PKT_LSTLC_AUTH_REQ& GetAuthReq() { return m_stReq; }


	UINT64 GetLastRecvMsgTime() { return m_qwLastRecvMsgTime; }
private:
	
	UINT16				m_wZoneID;
	UINT32				m_dwTransID;
	PKT_LSTLC_AUTH_REQ	m_stReq;

	UINT64				m_qwLastRecvMsgTime; //上次收到信息时间
};


#endif 

