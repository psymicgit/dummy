#ifndef APP_PAY_MGR_H
#define APP_PAY_MGR_H


#include "curl/curl.h"
#include <sdsingleton.h>
#include <string>
#include <map>
#include <memory>
#include <sdtime.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include "common/server/ngstring.h"
#include <protocol/server/protoclils.h>
#include <algorithm/sdmd5.h>
#include "basehttpcmd.h"



using namespace std;
using namespace SGDP;

class CLoginExHttpCMD;
class CNoticeHttpCMD;
class CRegisterHttpCMD;
class CLogoutHttpCMD;
class CChangePwdHttpCMD;
class CGetUserExpandHttpCMD;
class CLoginHttpCMD;
class CBindMobileHttpCMD;
class CBindCheckVerCodeHttpCMD;
class CUnBindMobileHttpCMD;
class CUnBindCheckVerCodeHttpCMD;

class CHttpMgr
{
    DECLARE_SINGLETON(CHttpMgr);
    typedef list<CBaseHttpCMD*> LIST_HTTP_INFO;
    typedef LIST_HTTP_INFO::iterator LIST_HTTP_INFOItr;
    CHttpMgr();
    ~CHttpMgr();
public:
    BOOL Init();

    VOID UnInit();


    BOOL Process();

	CBaseHttpCMD*	CreateCmd(EHttpCmdType eDbCmdType);

	VOID			FreeCmd(CBaseHttpCMD* poBaseDBCMD);

    BOOL			AddCommand(CBaseHttpCMD* poCommand);

private:
    static size_t OnRecv(void *buffer, size_t size, size_t count, void *user_p);
private:
	CSDObjectPool<CLoginExHttpCMD, CSDMutex>		m_oLoginExHttpCMDPool;
	CSDObjectPool<CNoticeHttpCMD, CSDMutex>		m_oNoticeHttpCMDPool;
	CSDObjectPool<CRegisterHttpCMD, CSDMutex>   m_oRegisterHttpCMDPool;
	CSDObjectPool<CLogoutHttpCMD, CSDMutex>		m_oLogoutHttpCMDPool;
	CSDObjectPool<CChangePwdHttpCMD, CSDMutex>  m_oChangePwdHttpCMDPool;
	CSDObjectPool<CGetUserExpandHttpCMD, CSDMutex> m_oGetUserExpandHttpCMDPool;
    CSDObjectPool<CBindMobileHttpCMD, CSDMutex> m_oBindMobileHttpCMDPool;
    CSDObjectPool<CBindCheckVerCodeHttpCMD, CSDMutex> m_oBindCheckVerCodeHttpCMDPool;
    CSDObjectPool<CUnBindMobileHttpCMD, CSDMutex> m_oUnBindMobileHttpCMDPool;
    CSDObjectPool<CUnBindCheckVerCodeHttpCMD, CSDMutex> m_oUnBindCheckVerCodeHttpCMDPool;
    CURLM *			m_pCurlHandle;
    LIST_HTTP_INFO	m_listHttpInfo;
    BOOL				m_bInit;
};

#endif