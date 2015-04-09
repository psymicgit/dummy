#include "authmgr.h"
#include <sderrno.h>
#include "framework/tlcapplication.h"
#include "framework/tlcconfig.h"
#include "logic/usermgr.h"

#ifdef __linux__
#include <dlfcn.h>
#endif

IMPLEMENT_SINGLETON(CAuthMgr)

CAuthMgr::CAuthMgr(VOID)
{
}

CAuthMgr::~CAuthMgr(VOID)
{
	CAuthModuleMapItr itr = m_mapAuthModules.begin();
	for(; itr != m_mapAuthModules.end(); ++ itr)
	{
		if (NULL != itr->second->pAuthModule)
		{
			itr->second->pAuthModule->Release();
		}
#ifdef _WIN32
		FreeLibrary(itr->second->hDll);
#else
		dlclose(itr->second->hDll);
#endif
		SDDelete itr->second;
	}

	m_mapAuthModules.clear();
}

BOOL CAuthMgr::Init( VOID )
{
	InitAuthType();

	if(!LoadAuthDll())
	{
		SYS_CRITICAL(_SDT("[%s: %d]:LoadAuthDll failed!"), MSG_MARK);
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthMgr::LoadAuthDll( VOID )
{
    CTLCApplication* poApplication = (CTLCApplication*)SDGetApp();
    const CAuthModuleConfigsVect& vecAuthModulesConfigs = CTLCConfig::Instance()->GetAuthModuleConfigs();
	
	//读取认证模块
	for (CAuthModuleConfigsVectItr itr = vecAuthModulesConfigs.begin(); itr != vecAuthModulesConfigs.end(); itr ++)
	{
		SAuthModule* psTmp = SDNew SAuthModule;
#ifdef _WIN32
		psTmp->hDll = LoadLibrary(_SDTA2T((*itr).strDll.c_str()));
#else
		psTmp->hDll = dlopen((*itr).strDll.c_str(),RTLD_LAZY);
#endif
		if (NULL == psTmp->hDll)
		{
			SYS_CRITICAL(_SDT("[%s: %d]:Load library: %s failed, fail error: %d"), MSG_MARK, (*itr).strDll.c_str(), SDGetLastError());
			return FALSE;
		}

#ifdef _WIN32
		psTmp->pGetModuleFunc = (PFN_GetAuthModule)GetProcAddress(psTmp->hDll, (*itr).strGetModuleFunc.c_str());
#else
		psTmp->pGetModuleFunc = (PFN_GetAuthModule)dlsym(psTmp->hDll, (*itr).strGetModuleFunc.c_str());
#endif
		if (NULL == psTmp->pGetModuleFunc)
		{
			SYS_CRITICAL(_SDT("[%s: %d]:GetProcAddress: %s failed, fail error: %d"), MSG_MARK, (*itr).strGetModuleFunc.c_str(), SDGetLastError());
#ifdef _WIN32
			FreeLibrary(psTmp->hDll);
#else
			dlclose(psTmp->hDll);
#endif
			return FALSE;
		}

        psTmp->pAuthModule = psTmp->pGetModuleFunc();
        if (NULL == psTmp->pAuthModule)
        {
            SYS_CRITICAL(_SDT("[%s: %d]:Auth module: %s, GetModule failed."), MSG_MARK, psTmp->strName.c_str());
#ifdef _WIN32
            FreeLibrary(psTmp->hDll);
#else
            dlclose(psTmp->hDll);
#endif
            return FALSE;
        }

 		psTmp->strName = (*itr).strName;

		if (!psTmp->pAuthModule->Init(psTmp->strName.c_str(), (*itr).strConfFile.c_str(), CUserMgr::Instance()))
		{
			SYS_CRITICAL(_SDT("[%s: %d]:Auth module: %s, Init failed."), MSG_MARK, psTmp->strName.c_str());
			psTmp->pAuthModule->Release();
#ifdef _WIN32
			FreeLibrary(psTmp->hDll);
#else
			dlclose(psTmp->hDll);
#endif
			return FALSE;
		}

		m_mapAuthModules[GetAuthType(psTmp->strName)] = psTmp;
	}
	return TRUE;
}

VOID CAuthMgr::InitAuthType()
{
	m_mapAuthType["tongbu"] = 1;
	m_mapAuthType["91"] = 2;
	m_mapAuthType["maxnet"] = 3;
	m_mapAuthType["pp"] = 4;
}

UINT8 CAuthMgr::GetAuthType(string strName)
{
	CAuthTypeMapItr itr = m_mapAuthType.find(strName.c_str());
	if(itr != m_mapAuthType.end())
	{
		return itr->second;
	}

	return 0;
}


BOOL CAuthMgr::Run( VOID )
{
	BOOL bRet = FALSE;
	CAuthModuleMapItr itr = m_mapAuthModules.begin();
	for (; itr != m_mapAuthModules.end(); ++ itr)
	{
		if (NULL != itr->second->pAuthModule)
		{
			if (itr->second->pAuthModule->Run())
				bRet = TRUE;
		}
	}
	return bRet;
}

CAuthModule* CAuthMgr::FindAuthModule( UINT8 byAuthType )
{
	CAuthModuleMapItr itr = m_mapAuthModules.find(byAuthType);
	if (itr != m_mapAuthModules.end())
	{
		return itr->second->pAuthModule;
	}
	return NULL;
}

BOOL CAuthMgr::Auth(CUser* poUser)
{
	if(NULL == poUser)
	{
		return FALSE;
	}

	PKT_LSTLC_AUTH_REQ& stReq = poUser->GetAuthReq();
	CAuthModule* poAuthModule = CAuthMgr::Instance()->FindAuthModule(stReq.byAuthType);
	if (NULL == poAuthModule)
	{		
		return FALSE;
	}

	return poAuthModule->Auth(poUser->GetZoneID(), poUser->GetTransID(), stReq.aszUserName,  stReq.byPwdLen, stReq.abyUserPwd, stReq.aszDeviceID, 
		stReq.dwParam1, stReq.dwParam2, stReq.dwParam3, stReq.qwParam4, stReq.aszExtContext);
}

