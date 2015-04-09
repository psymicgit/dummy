#ifndef  _USER_H
#define _USER_H

#include <protocol/server/protocommondata.h>
#include <protocol/server/protoclils.h>

/**
* @brief 用户附加信息
*/

class CGTClient;
class CUserMgr;
class CUser
{
	friend class CUserMgr;
public:
	CUser();
	virtual ~CUser();
	UINT32		GetUserSessionID() { return m_dwPlayerID; }	
	CGTClient*	GetGTClient() { return m_poGTClient; }
	VOID			ReleaseGtClient(CGTClient* poGTClient);
	UINT32		GetRemoteIP();

public:
	inline TCHAR*	GetDeviceID() { return m_aszDeviceID; }
	inline TCHAR*	GetNotifyID() { return m_aszNotifyID; }
	inline TCHAR*	GetUserName() { return m_aszUserName; }
    inline UINT16	GetZoneID()  { return m_wZoneID; }
	inline UINT16	GetOriginalZoneID()  { return m_wOriZoneID; }
	inline UINT32	GetUserID()  { return m_dwUserID; }
	inline UINT32	GetPlayerID()  { return m_dwPlayerID; }
	inline UINT16	GetCareerID()  { return m_wCareerID; }
	inline UINT8	GetAuthType()  { return m_byAuthType; }
	inline UINT8	GetDaiChong()  { return m_byDaiChong; }
protected:
	VOID    SetUserProp(TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, UINT8 byAuthType, CGTClient* poGTClient, UINT8 byDaiChong);
	VOID		ResetGTClient(CGTClient* poGTClient);//断开之前的连接，并重置新的连接

private:

	//用户会话ID，用于同步GS/DB间的会话ID,不同于ISDSession的会话ID
	//用户重复登录时，将之前的连接踢下线，CUser重置新连接，User的SessionID不变，则与GS通讯的SessionID不变
	//用户下线时，GT断开连接，但CUser还缓存在内存中，等待GS通知清除

	//用于判断客户端发上来的和认证是否一致
	TCHAR            m_aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            m_aszNotifyID[NOTIFYID_LEN]; //通知号
	TCHAR            m_aszUserName[USERNAME_LEN]; //用户名
    UINT16			m_wZoneID;//区ID
	UINT16			m_wOriZoneID;//区ID
	UINT32			m_dwUserID;//用户ID
	UINT32           m_dwPlayerID; //玩家ID，老用户登录有效
	UINT16           m_wCareerID; //职业，创建新玩家(PlayerID为0)有效
	UINT8			m_byAuthType;//认证类型
	CGTClient*		m_poGTClient;
	UINT8			m_byDaiChong;//代充标志 
};


#endif

