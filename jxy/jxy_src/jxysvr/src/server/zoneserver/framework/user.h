

#ifndef _SDF_CLISESSION_H_
#define _SDF_CLISESSION_H_

#include <sdtime.h>
#include <sdstring.h>
#include <map>
#include <protocol/server/protocommondata.h>


using namespace std ;




class CUserMgr;
/**
* @brief 客户端连接类
* 接收到网关汇报，有客户端接入时构造该类实例
* 接收到网关汇报，有客户端离开时释放该类实例
*/
class CUser
{
	friend class CUserMgr;
public:
    CUser();
    ~CUser();
public:
	UINT32		GetUserID() { return m_dwUserID; }
	UINT8		GetAuthType() { return m_byAuthType; }
	UINT8		GetAuthMethod() { return m_byAuthMethod; }
	TCHAR*		GetUserName() { return m_szUserName; }
	TCHAR*		GetDeviceID() { return m_szDeviceID; }
	TCHAR*		GetNickName() { return m_szNickName; }
	TCHAR*		GetPasswd() { return m_szPasswd; }

	void			SetPasswd(TCHAR* pszPasswd);
	BOOL			CkUserDisconnect();
	VOID			SetUserProp(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName);
	BOOL			CkUser(UINT32 &dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName);
private:
	
	UINT32			m_dwUserID;//用户ID
	UINT8			m_byAuthType;
	UINT8			m_byAuthMethod;
	TCHAR			m_szUserName[USERNAME_LEN];
	TCHAR			m_szPasswd[USERPWD_LEN];
	TCHAR			m_szNickName[USERNAME_LEN];
	TCHAR			m_szDeviceID[DEVICEID_LEN];
	UINT64			m_qwLastRecvMsgTime; //上次收到信息时间
};


#endif //#ifndef _SDF_CLISESSION_H_

