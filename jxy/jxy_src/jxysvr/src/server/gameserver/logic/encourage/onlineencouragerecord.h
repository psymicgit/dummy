
#ifndef _ONLINEENCOURAGERECORD_H_
#define _ONLINEENCOURAGERECORD_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>

using namespace std ;

struct tagDT_ONLINE_ENCOURAGE_DATA;
typedef tagDT_ONLINE_ENCOURAGE_DATA DT_ONLINE_ENCOURAGE_DATA;

struct tagDT_DATA_AFTER_ONLINE_ENCOURAGE;
typedef tagDT_DATA_AFTER_ONLINE_ENCOURAGE DT_DATA_AFTER_ONLINE_ENCOURAGE;

class CPlayer;
class COnlineEncourageRecord:public CBaseObj
{
public:
	COnlineEncourageRecord();
	~COnlineEncourageRecord();
public:
	//初始化相关对象
	BOOL Init(DT_ONLINE_ENCOURAGE_RECORD_DATA &stDT_ONLINE_ENCOURAGE_RECORD_DATA, CPlayer* poOwner);
	DT_ONLINE_ENCOURAGE_RECORD_DATA& GetDT_DATA() { return m_stDT_ONLINE_ENCOURAGE_RECORD_DATA; }

	///////////////////////////////////////////////////

	UINT16 GetCurEncourage(UINT16& wRecvCountDown, DT_ONLINE_ENCOURAGE_DATA& stOnlineEncourage);

	UINT16 RecvEncourage(UINT16& wRecvCountDown, UINT8& byNextOnlineEncourageFlag, 
		DT_ONLINE_ENCOURAGE_DATA& stNextOnlineEncourage, DT_DATA_AFTER_ONLINE_ENCOURAGE& stCurPlayerInfo);

protected:
	VOID Encourage(DT_ONLINE_ENCOURAGE_DATA& stDT_ONLINE_ENCOURAGE_DATA);
private:
	DT_ONLINE_ENCOURAGE_RECORD_DATA	m_stDT_ONLINE_ENCOURAGE_RECORD_DATA;

};
DECLARE_CLASS_POOL_MGR(COnlineEncourageRecord)

#endif //#ifndef 

