

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

typedef map<string , CUser*> CSessionID2UserMap; //key为CUser的SessionID
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
    BOOL			CreateUser(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName);

    BOOL			CkUser(UINT32 &dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName);

    VOID			CkUserDisconnect();

    void			ReleaseUser(UINT32 dwUserID);

private:
    VOID			_ReleaseUser(CUser* poUser);
    CUser*		_FindUser(string strkey);

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

