
#ifndef _LOGINENCOURAGERECORD_H_
#define _LOGINENCOURAGERECORD_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>

using namespace std ;

class CPlayer;

struct tagDT_LOGIN_ENCOURAG_DATA;
typedef tagDT_LOGIN_ENCOURAG_DATA DT_LOGIN_ENCOURAG_DATA;


class CLoginEncourageRecord:public CBaseObj
{
public:
	CLoginEncourageRecord();
	~CLoginEncourageRecord();
public:
	//初始化相关对象
	BOOL Init(DT_LOGIN_ENCOURAGE_RECORD_DATA &stDT_LOGIN_ENCOURAGE_RECORD_DATA, CPlayer* poOwner);
	DT_LOGIN_ENCOURAGE_RECORD_DATA& GetDT_DATA() { return m_stDT_LOGIN_ENCOURAGE_RECORD_DATA; }


	///////////////////////////////////////////////////
	UINT16	GetEncourageInfo(DT_LOGIN_ENCOURAG_DATA& stLoginEncourageInfo);
	VOID	OnRate();//客户端随机回调
	UINT16	PutIntoBag(UINT8& byHaveEncourageFlag, DT_LOGIN_ENCOURAG_DATA& stLoginEncourageInfo);
	UINT8	GetLoginEncourageState();
protected:
		
private:
	DT_LOGIN_ENCOURAGE_RECORD_DATA	m_stDT_LOGIN_ENCOURAGE_RECORD_DATA;//

};

DECLARE_CLASS_POOL_MGR(CLoginEncourageRecord)
#endif //#ifndef 

