

#ifndef _SMSUSER_H_
#define _SMSUSER_H_

#include <sdtime.h>
#include <sdstring.h>
#include <map>
#include <protocol/server/protocommondata.h>


using namespace std ;


//短信超时时间
#define MAX_MOBILE_TIME_OUT	60
#define MAX_MOBILE_DISCONNECT 	3600

class CSmsUserMgr;
/**
* @brief 客户端连接类
* 接收到网关汇报，有客户端接入时构造该类实例
* 接收到网关汇报，有客户端离开时释放该类实例
*/
class CSmsUser
{
	friend class CSmsUserMgr;
public:
    CSmsUser();
    ~CSmsUser();
public:
	UINT32		GetUserID() { return m_dwUserID; }
	TCHAR	*	GetUserName(){return m_szUserName;}
	TCHAR	*	GetVerCode(){return m_szVerCode;}
	TCHAR	*	GetMobile(){return m_szMobile;}
	BOOL			CkUserDisconnect();
	BOOL			SetUserProp(UINT32 dwUserID, TCHAR* pszUserName, TCHAR* pszMobile, TCHAR* pszVerCode);
	BOOL			CkUser(TCHAR* pszVerCode, TCHAR *pszUserName);
	BOOL			GetAuth() { return m_bAuth; }

	BOOL			CkUser(UINT16 &wCountdown);
private:
	BOOL				m_bAuth;
	UINT32			m_dwUserID;//用户ID
	TCHAR			m_szUserName[USERNAME_LEN];
	TCHAR			m_szMobile[MAX_MOBILE_LENGTH];
	TCHAR			m_szVerCode[MAX_MOBILE_LENGTH];
	UINT64			m_qwRecvMsgTime;		//上次收到信息时间
	UINT64			m_qwCreateTime;		//创建时间
};


#endif //#ifndef _SDF_CLISESSION_H_

