#ifndef DBSESSIONMGR_H_20110504
#define DBSESSIONMGR_H_20110504

#include <sdidpool.h>
#include "sdpipechannel.h"
#include <map>

	/*
	用于db执行回复消息时查找session的管理，防止回复使用session时该session已经释放
	*/

#define MAX_SESSION_NUM 10000

class CDbSessionMgr
{
public:
	~CDbSessionMgr();
	static CDbSessionMgr* Instance() { static CDbSessionMgr oInstance; return &oInstance; }

	UINT32 AddSession(CSDPipeChannel* poSession);
	VOID DelSession(UINT32 dwSessionId);
	CSDPipeChannel* FindSession(UINT32 dwSessionId);
protected:
	CDbSessionMgr();
private:	
	CSDIDPool<CSDPipeChannel*>		m_oIDSessionPool;//session ID 对象池
};

#endif //#ifndef DBSESSIONMGR_H_20110504

