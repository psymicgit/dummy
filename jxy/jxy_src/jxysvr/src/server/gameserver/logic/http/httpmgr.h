#ifndef _HTTP_MGR_H
#define _HTTP_MGR_H


#include "curl/curl.h"
#include <sdsingleton.h>
#include <string>
#include <sdobjectpool.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include "common/server/ngstring.h"
#include <protocol/server/protoclils.h>
#include <algorithm/sdmd5.h>


// 发送到用户中心的http请求类型
enum EHttpCmdType
{
	EHCT_APP_PAY = 1,
	EHCT_LEVEL = 2,
	EHCT_NAME = 3,
	EHCT_GETCONFIGS = 4,
	EHCT_CHATENCOURAGE = 5,
    EHCT_RECHARGE = 6,
	EHCT_PUSH_PLAYER_INFO = 7, // 推送最新的玩家信息到用户中心
	EHCT_GET_MAX_PLAYERID_ADD_GIFT = 8,//获取最大玩家ID
};

#define CENTER_SIGN	"Bef^3rwsvn!29jcx57ng"


using namespace std;
using namespace SGDP;

class CBaseHttpCMD;
class CLevelHttpCMD;
class CNameHttpCMD;
class CGetConfigsHttpCMD;
class CAppPayHttpCMD;
class CChatEncourageHttpCMD;
class CRechargeHttpCMD;
class CPushPlayerInfoHttpCMD;
class CGetMaxPlayerID_AddGift;

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
    CSDObjectPool<CAppPayHttpCMD, CSDMutex>		m_oCAppPayHttpCMDPool;
    CSDObjectPool<CLevelHttpCMD, CSDMutex>		m_oLevelHttpCMDPool;
    CSDObjectPool<CNameHttpCMD, CSDMutex>		m_oNameHttpCMDPool;
    CSDObjectPool<CGetConfigsHttpCMD, CSDMutex>	m_oGetConfigsHttpCMDPool;
	CSDObjectPool<CChatEncourageHttpCMD, CSDMutex>	m_oChatEncourageHttpCMDPool;
    CSDObjectPool<CRechargeHttpCMD, CSDMutex>	m_oRechargeCMDPool;
	CSDObjectPool<CPushPlayerInfoHttpCMD, CSDMutex>	m_oPushPlayerInfoCMDPool;
	CSDObjectPool<CGetMaxPlayerID_AddGift, CSDMutex> m_oGetPlayerMax_AddGiftPool;

    CURLM *			m_pCurlHandle;
    LIST_HTTP_INFO	m_listHttpInfo;
    BOOL				m_bInit;
};

#endif