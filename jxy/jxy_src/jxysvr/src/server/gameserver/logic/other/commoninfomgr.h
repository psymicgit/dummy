
#ifndef _COMMONINFOMGR_H_
#define _COMMONINFOMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <map>
#include <logic/base/basepropmgr.h>
#include <protocligs.h>

using namespace std;


enum em_CliType
{
	em_IOS = 0,
	em_ANDROID = 1,
};

//客户端版本资源配置
struct SCliVerInfoProp
{
    UINT32 dwCliVer;
    UINT32 dwResVer;
    UINT32 dwUIVer;
	TCHAR  aszResVer[MAX_RES_VER_LEN]; //版本信息
	SCliVerInfoProp()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(SCliVerInfoProp));
	}
};

typedef map<UINT32, SCliVerInfoProp>	CCliVerInfoMap;
typedef CCliVerInfoMap::iterator		CCliVerInfoMapItr;

//客户端版本资源配置
struct SCliTypeVerInfoProp
{
	CCliVerInfoMap mapCliVerInfo;
};

typedef map<UINT32, SCliTypeVerInfoProp>		CCliTypeVerInfoMap;
typedef CCliTypeVerInfoMap::iterator		CCliTypeVerInfoMapItr;


typedef vector<tstring> CDspNameVec;
typedef CDspNameVec::iterator CDspNameVecItr;

typedef map<UINT32, tstring> CPlayerID2NameMap;
typedef CPlayerID2NameMap::iterator CPlayerID2NameMapItr;
typedef map<tstring, UINT32> CName2PlayerIDMap;
typedef CName2PlayerIDMap::iterator CName2PlayerIDMapItr;


class CPlayer;
class CGSProcessor;

class CCommonInfoMgr: public CBasePopMgr
{
    friend class CGSProcessor;
public:

    DECLARE_SINGLETON_PROPMGR(CCommonInfoMgr);

    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_COMMONINFO ;
    }
public:

    BOOL Init();

    VOID UnInit();
public:

    VOID		GetCliVerInfo(UINT8 byAuthType, UINT32 dwCliVer, UINT32& dwResVer, UINT32& dwUIVer, TCHAR aszResVer[MAX_RES_VER_LEN]);
    tstring	GetRdDspName(CPlayer* poPlayer, UINT8 byReRandom = 1);
    BOOL		CkDspnameInCache(const TCHAR* pszDispName, UINT32 dwPlayerID);
    VOID		OnRegDspname(UINT32 dwPlayerID);

    CCliTypeVerInfoMap		&GetCliTypeVerInfo()
    {
        return m_mapCliTypeVerInfo;
    };

public:
    CCommonInfoMgr();
    virtual ~CCommonInfoMgr();
protected:
    BOOL LoadFromDB();
    BOOL LoadCliVerInfoFromDB();
    BOOL LoadDspnamePoolFromDB();

private:
    CCliTypeVerInfoMap		m_mapCliTypeVerInfo;
    CDspNameVec				m_vecFirstName;
    CDspNameVec				m_vecMaleName;
    CDspNameVec				m_vecFemaleName;
    CPlayerID2NameMap			m_mapPlayerID2CacheName;
    CName2PlayerIDMap			m_mapCacheName2PlayerID;
};

#endif // 
