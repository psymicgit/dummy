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




using namespace std;
using namespace SGDP;

//HTTP√¸¡Ó¿‡–Õ
enum EHttpCmdType
{
    EHCT_GET_USER_INFO = 1,
    EHCT_GET_VERSION_NOTICE = 2,
    EHCT_REGISTER_USERNAME = 3,
    EHCT_REPORTUSERDEVICEINFO = 4,
    EHCT_RESETPASSWDMOBILE = 5,
    EHCT_RESETPASSWDNEWPASSWD = 6,
    EHCT_RESETPASSWDVERCODE = 7,
    EHCT_SELECTZONE = 8,
    EHCT_UNBINDDEVICE = 9,
};
class CBaseHttpCMD;
class CGetUserInfoHttpCMD;
class CGetVersionNoticeHttpCMD;
class CRegisterUserNameHttpCMD;
class CReportUserDeviceInfoHttpCMD;
class CResetPasswdMobileHttpCMD;
class CResetPasswdNewPasswdHttpCMD;
class CResetPasswdVerCodeHttpCMD;
class CSelectZoneHttpCMD;
class CUnBindDeviceHttpCMD;



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
    CSDObjectPool<CGetUserInfoHttpCMD, CSDMutex>				m_oGetUserInfoHttpCMDPool;
	CSDObjectPool<CGetVersionNoticeHttpCMD, CSDMutex>			m_oGetVersionNoticeHttpCMDPool;
	CSDObjectPool<CRegisterUserNameHttpCMD, CSDMutex>			m_oRegisterUserNameHttpCMDPool;
	CSDObjectPool<CReportUserDeviceInfoHttpCMD, CSDMutex>		m_oReportUserDeviceInfoHttpCMDPool;
	CSDObjectPool<CResetPasswdMobileHttpCMD, CSDMutex>		m_oResetPasswdMobileHttpCMDPool;
	CSDObjectPool<CResetPasswdNewPasswdHttpCMD, CSDMutex>		m_oResetPasswdNewPasswdHttpCMDPool;
	CSDObjectPool<CResetPasswdVerCodeHttpCMD, CSDMutex>		m_oResetPasswdVerCodeHttpCMDPool;
	CSDObjectPool<CSelectZoneHttpCMD, CSDMutex>				m_oSelectZoneHttpCMDPool;
	CSDObjectPool<CUnBindDeviceHttpCMD, CSDMutex>			m_oUnBindDeviceHttpCMDPool;
    CURLM *			m_pCurlHandle;
    LIST_HTTP_INFO	m_listHttpInfo;
    BOOL				m_bInit;
};

#endif