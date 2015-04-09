

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

/**
* @brief 客户端连接管理类
* 管理进入游戏服务器的所有客户端连接
*/
class CUserMgr
{

public:
    typedef map<UINT32 , CUser*> CSessionID2UserMap; //key为CUser的SessionID
    typedef CSessionID2UserMap::iterator CSessionID2UserMapItr;

	typedef map<UINT32 , CUser*> CPlayerID2UserMap; //key为CUser的PlayerID
	typedef CPlayerID2UserMap::iterator CPlayerID2UserMapItr;

	typedef map<UINT32 , UINT64> CPlayerID2TimeMap; //key为CUser的PlayerID
	typedef CPlayerID2TimeMap::iterator CPlayerID2TimeMapItr;

	typedef map<tstring , CUser*> CNameDevice2UserMap;//key为UserName+DeviceID
	typedef CNameDevice2UserMap::iterator CNameDevice2UserMapItr;

	static CUserMgr* Instance() { static CUserMgr oUserMgr; return &oUserMgr; }

public:

	//创建用户，若存在在返回存在用户
    CUser*          CreateUser(TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, UINT8 byAuthType, CGTClient* poGTClient, UINT8 byDaiChong);
   
    VOID            ReleaseUser(CUser* poUser);   

	CUser*			FindUser(UINT32 dwUserSessionID);//与GS交互
	CUser*			FindUserByPlayerID(UINT32 dwPlayerID);

	VOID			AddKickOutPlayer(UINT32 dwPlayerID);

	BOOL			BeKickOut(UINT32 dwPlayerID);
protected:
    CUserMgr();        
    ~CUserMgr();
protected:

private:
    //CUser对象池
	CSDObjectPool<CUser, CSDMutex>		m_oUserPool;
    //用户在线容器
    CSessionID2UserMap					m_mapSessionID2User;
	CPlayerID2UserMap					m_mapPlayerID2User;
	CPlayerID2TimeMap					m_mapPlayerID2Time;
	//static UINT32						m_dwUserSessionIDGen;//用于生成每个User的SessionID
};



#endif //#ifndef _SDF_OBJFACTORY_H_

