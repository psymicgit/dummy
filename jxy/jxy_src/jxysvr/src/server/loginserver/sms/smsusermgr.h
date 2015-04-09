

#ifndef _SMSUSERMGR_H_
#define _SMSUSERMGR_H_

#include "smsuser.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdtype.h>
#include <sdidpool.h>
#include <map>

using namespace std ;
using namespace SGDP;


#define MAX_USER	10000

typedef map<UINT64 , CSmsUser*> CSmsID2UserMap; //key为CUser的userID
typedef CSmsID2UserMap::iterator CSmsID2UserMapItr;

enum em_OptType
{
	em_BIND = 1,//绑定手机号
	em_UNBIND = 2,//解绑手机号
};


/**
* @brief 客户端连接管理类
* 管理进入游戏服务器的所有客户端连接
*/
class CSmsUserMgr
{
public:
    DECLARE_SINGLETON(CSmsUserMgr)

public:
    BOOL			Init();
    //创建用户，若存在在返回存在用户
    BOOL			CreateUser(UINT32 dwUserID, TCHAR* pszMobile, em_OptType emOpt, TCHAR* pszVerCode);

	BOOL			CkUser(UINT32 dwUserID, TCHAR *pszMobile, em_OptType emOpt, UINT16 &wCountdown);

	BOOL			CkUser(UINT32 dwUserID, em_OptType emOpt, TCHAR* pszVerCode, TCHAR *pszMobile);

	BOOL			GetMobile(UINT32 dwUserID, em_OptType emOpt, TCHAR* pszMobile);

    VOID			CkUserDisconnect();

protected:
    CSmsUserMgr();
    ~CSmsUserMgr();
protected:

private:
    //CUser对象池
    CSDObjectPool<CSmsUser, CSDMutex>		m_oUserPool;
    //用户在线容器
    CSmsID2UserMap						m_mapSmsID2User;
    CSDMutex								m_oMutex	;
};



#endif //#ifndef _SDF_OBJFACTORY_H_

