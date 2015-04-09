

#ifndef _USERMGR_H_
#define _USERMGR_H_

#include "user.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdtype.h>
#include <sdidpool.h>
#include <map>

using namespace std ;
using namespace SGDP;


#define MAX_USER	10000

typedef map<UINT32 , CUser*> CSessionID2UserMap; //key为CUser的SessionID
typedef CSessionID2UserMap::iterator CSessionID2UserMapItr;

/**
* @brief 客户端连接管理类
* 管理进入游戏服务器的所有客户端连接
*/
class CUserMgr
{
public:
    DECLARE_SINGLETON(CUserMgr)

public:
    BOOL			Init();
    //创建用户，若存在在返回存在用户
	BOOL			CreateUser(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, UINT16 wCareerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, TCHAR *pszZoneName, UINT16 wZoneID, UINT32 dwCliVer);
	
	UINT16		CkUser(UINT32 &dwUserID, TCHAR *pszZoneName, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, UINT16 wCareerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT32 dwCliVer);

	VOID			CkUserDisconnect();

	VOID			ChangePwd(UINT32 dwUserID, TCHAR* pszPasswd);
private:
	VOID			ReleaseUser(CUser* poUser);

	CUser*		FindUser(UINT32 dwPlayerID);



protected:
    CUserMgr();
    ~CUserMgr();
protected:

private:
    //CUser对象池
    CSDObjectPool<CUser, CSDMutex>		m_oUserPool;
    //用户在线容器
    CSessionID2UserMap				m_mapPlayerID2User;
	CSDMutex							m_oMutex	;
};



#endif //#ifndef _SDF_OBJFACTORY_H_

