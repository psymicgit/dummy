

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
typedef CSessionID2UserMap::const_iterator CSessionID2UserMapConstItr;

/**
* @brief 客户端连接管理类
* 管理进入游戏服务器的所有客户端连接
*/
class CUserMgr
{
	friend class CChatMgr;	
	friend class CNoticeMgr;
public:


    typedef map<tstring , CUser*> CNameDevice2UserMap;//key为UserName+DeviceID
    typedef CNameDevice2UserMap::iterator CNameDevice2UserMapItr;

    DECLARE_SINGLETON(CUserMgr)

public:
    BOOL				Init();
    //创建用户，若存在在返回存在用户
    CUser*          CreateUser(UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, 
        UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwLoginIP, UINT32 dwCliVer, UINT8 byAuthType, UINT8 byDaichong = 0);

    VOID            ReleaseUser(CUser* poUser);

    //以dwUserSessionID为准
    //CUser*          FindUser(TCHAR* pszUserName, TCHAR* pszDeviceID);

    CUser*          FindUser(UINT32 dwPlayerID);

	VOID				CkUserDisconnect();
	//提供常引用方式，防止map数据被改变
	inline const CSessionID2UserMap& GetAllUserMap(){ return m_mapSessionID2User; }

	inline	UINT32	GetConnectedNum() { return m_dwConnectedNum; }
	inline	VOID		IncConnectedNum() { m_dwConnectedNum++; }
	inline	VOID		DecConnectedNum() { if(m_dwConnectedNum>0) m_dwConnectedNum--; }

protected:
    CUserMgr();
    ~CUserMgr();
protected:

private:
    //CUser对象池
    CSDObjectPool<CUser, CSDMutex>		m_oUserPool;
    //用户在线容器
    CSessionID2UserMap				m_mapSessionID2User;

	CSessionID2UserMapItr			m_itrCurDealUser;//

	UINT32							m_dwConnectedNum;
};



#endif //#ifndef _SDF_OBJFACTORY_H_

