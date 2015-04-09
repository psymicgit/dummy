#ifndef _AUTHMGR_H
#define _AUTHMGR_H
#pragma warning( disable : 4267 )

#include <common/server/auth.h>
#include <sdframework.h>
#include <string>
#include <map>

using namespace std;

typedef CAuthModule* (*PFN_GetAuthModule)(VOID);

#ifdef __linux__
typedef VOID *HINSTANCE;
#endif


class CUser;

struct SAuthModule
{
	HINSTANCE hDll;
	PFN_GetAuthModule pGetModuleFunc;
	CAuthModule* pAuthModule;
	string strName;
};

typedef map<string, UINT8> CAuthTypeMap;
typedef CAuthTypeMap::iterator CAuthTypeMapItr;

class CAuthMgr
{
	DECLARE_SINGLETON(CAuthMgr)
	CAuthMgr(void);
	~CAuthMgr(void);

	typedef map<UINT8, SAuthModule*> CAuthModuleMap;
	typedef CAuthModuleMap::iterator CAuthModuleMapItr;
public:
	BOOL Init(void);
	BOOL Run(void);
	BOOL Auth(CUser* poUser);
protected:
	CAuthModule*	FindAuthModule(UINT8 byAuthType);
	VOID			InitAuthType();
	BOOL			LoadAuthDll();
	UINT8			GetAuthType(string strName);
private:
	CAuthModuleMap	m_mapAuthModules;
	CAuthTypeMap	m_mapAuthType;
};

#endif

