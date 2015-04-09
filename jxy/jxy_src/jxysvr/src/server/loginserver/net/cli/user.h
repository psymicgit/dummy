

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
	UINT32		GetUserSessionID() { return m_dwPlayerID; }
	UINT32		GetPlayerID() { return m_dwPlayerID; }
	UINT32		GetUserID() { return m_dwUserID; }
	UINT16		GetCareerID() { return m_wCareerID; }

	TCHAR*		GetUserName() { return m_szUserName; }
	TCHAR*		GetDeviceID() { return m_szDeviceID; }
	TCHAR*		GetNotifyID() { return m_szNotifyID; }
	UINT16		GetZoneID() { return m_wZoneID; }
	UINT32		GetCliVer() { return m_dwCliVer; }

	BOOL			CkUserDisconnect();

	VOID			SetUserProp(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, TCHAR *pszZoneName, UINT16 wCareerID, UINT16 wZoneID, UINT32 dwCliVer);
	UINT16		CkUser(UINT32 &dwUserID, TCHAR *pszZoneName, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wCareerID, UINT16 wZoneID, UINT32 dwCliVer);
	VOID			ChangePwd(TCHAR* pszPasswd);
private:
	
	UINT32			m_dwUserID;//用户ID
	UINT32			m_dwPlayerID;//用户会话ID用PlayerID
	UINT16			m_wCareerID;//职业ID，新创建用户有效
	UINT16			m_wZoneID;//区ID
	UINT8			m_byAuthType;
	UINT8			m_byAuthMethod;
	UINT32			m_dwCliVer;//客户端版本
	TCHAR			m_szUserName[USERNAME_LEN];
	TCHAR			m_szPasswd[USERPWD_LEN];
	TCHAR			m_szDeviceID[DEVICEID_LEN];
	TCHAR			m_szNotifyID[NOTIFYID_LEN];
	TCHAR			m_aszZoneName[USERNAME_LEN];
	UINT64			m_qwLastRecvMsgTime; //上次收到信息时间

};


#endif //#ifndef _SDF_CLISESSION_H_

