#include "httpmgr.h"
#include <sdloggerimpl.h>
#include <json/json.h>
#include <algorithm>
#include <string>
#include <algorithm/sdmd5.h>

#include "dll/sdframework/sdutil.h"
#include "json/json.h"
#include "common/server/ngstring.h"
#include "common/client/errdef.h"
#include "net/cli/clipktbuilder.h"
#include "selectzonehttpcmd.h"
#include "getuserinfohttpcmd.h"
#include "getversionnoticehttpcmd.h"
#include "registerusernamehttpcmd.h"
#include "reportuserdeviceinfohttpcmd.h"
#include "resetpasswdmobilehttpcmd.h"
#include "resetpasswdnewpasswdhttpcmd.h"
#include "resetpasswdvercodehttpcmd.h"
#include "selectzonehttpcmd.h"
#include "unbinddevicehttpcmd.h"


using namespace std;
using namespace SGDP;


IMPLEMENT_SINGLETON(CHttpMgr)

CHttpMgr::CHttpMgr()
{
    m_bInit = FALSE;
    m_pCurlHandle = NULL;
}

CHttpMgr::~CHttpMgr()
{

}

BOOL CHttpMgr::Init()
{
    CURL *curl = NULL;
    CURLcode code;

    code = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (code != CURLE_OK)
    {
        SYS_CRITICAL( _SDT( "[%s: %d]:Failed to global init default [%d]!" ), MSG_MARK, code );
        return FALSE;
    }
    m_pCurlHandle = curl_multi_init();

    if( !m_pCurlHandle )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]:curl_multi_init failed!" ), MSG_MARK );
        return FALSE;
    }

    m_bInit = TRUE;
    //重新启动服务器时，需要在数据库中读取未处理的订单

    return TRUE;
}

VOID CHttpMgr::UnInit()
{
    if( m_bInit )
    {
        if( m_pCurlHandle )
        {
            curl_multi_cleanup( m_pCurlHandle );
        }
        curl_global_cleanup();
    }
}

CBaseHttpCMD*	CHttpMgr::CreateCmd(EHttpCmdType eDbCmdType)
{
    CBaseHttpCMD* poBaseCMD = NULL;
    switch(eDbCmdType)
    {
    case EHCT_GET_USER_INFO:
        poBaseCMD = m_oGetUserInfoHttpCMDPool.Alloc();
        break;
    case EHCT_GET_VERSION_NOTICE:
        poBaseCMD = m_oGetVersionNoticeHttpCMDPool.Alloc();
        break;
    case EHCT_REGISTER_USERNAME:
        poBaseCMD = m_oRegisterUserNameHttpCMDPool.Alloc();
        break;
    case EHCT_REPORTUSERDEVICEINFO:
        poBaseCMD = m_oReportUserDeviceInfoHttpCMDPool.Alloc();
        break;
    case EHCT_RESETPASSWDMOBILE:
        poBaseCMD = m_oResetPasswdMobileHttpCMDPool.Alloc();
        break;
    case EHCT_RESETPASSWDNEWPASSWD:
        poBaseCMD = m_oResetPasswdNewPasswdHttpCMDPool.Alloc();
        break;
    case EHCT_RESETPASSWDVERCODE:
        poBaseCMD = m_oResetPasswdVerCodeHttpCMDPool.Alloc();
        break;
    case EHCT_SELECTZONE:
        poBaseCMD = m_oSelectZoneHttpCMDPool.Alloc();
        break;
    case EHCT_UNBINDDEVICE:
        poBaseCMD = m_oUnBindDeviceHttpCMDPool.Alloc();
        break;
    default:
        break;
    }
    return poBaseCMD;
}


VOID	 CHttpMgr::FreeCmd(CBaseHttpCMD* poBaseDBCMD)
{
    if (NULL == poBaseDBCMD)
    {
        return;
    }
    switch(poBaseDBCMD->GetHttpCmdType())
    {
    case EHCT_GET_USER_INFO:
        m_oGetUserInfoHttpCMDPool.Free((CGetUserInfoHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_GET_VERSION_NOTICE:
        m_oGetVersionNoticeHttpCMDPool.Free((CGetVersionNoticeHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_REGISTER_USERNAME:
        m_oRegisterUserNameHttpCMDPool.Free((CRegisterUserNameHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_REPORTUSERDEVICEINFO:
        m_oReportUserDeviceInfoHttpCMDPool.Free((CReportUserDeviceInfoHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_RESETPASSWDMOBILE:
        m_oResetPasswdMobileHttpCMDPool.Free((CResetPasswdMobileHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_RESETPASSWDNEWPASSWD:
        m_oResetPasswdNewPasswdHttpCMDPool.Free((CResetPasswdNewPasswdHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_RESETPASSWDVERCODE:
        m_oResetPasswdVerCodeHttpCMDPool.Free((CResetPasswdVerCodeHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_SELECTZONE:
        m_oSelectZoneHttpCMDPool.Free((CSelectZoneHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_UNBINDDEVICE:
        m_oUnBindDeviceHttpCMDPool.Free((CUnBindDeviceHttpCMD*)poBaseDBCMD);
        break;
    default:
        break;
    }
}

BOOL	 CHttpMgr::AddCommand(CBaseHttpCMD* poCommand)
{
    curl_easy_setopt(poCommand->m_pEasyHandle, CURLOPT_WRITEFUNCTION, OnRecv);
    //设置DNS过期时间
    curl_easy_setopt(poCommand->m_pEasyHandle, CURLOPT_DNS_CACHE_TIMEOUT, 20);
    if( CURLM_OK != curl_multi_add_handle( m_pCurlHandle, poCommand->m_pEasyHandle ) )
    {
        DBG_INFO( _SDT( "[%s: %d]:curl_multi_add_handle failed!" ), MSG_MARK );
        poCommand->Release();
        return FALSE;
    }
    else
    {
        m_listHttpInfo.push_back(poCommand);
        return TRUE;
    }
}

BOOL CHttpMgr::Process()
{
    if( m_listHttpInfo.empty() )
    {
        return TRUE;
    }

    int nRunningHandles = 0;
    CURLMcode nRet = curl_multi_perform( m_pCurlHandle, &nRunningHandles );
    if( CURLM_OK != nRet )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]:curl_multi_perform failed!" ), MSG_MARK );
        return FALSE;
    }

    int				nMsgLeft = 0;
    CURLMsg *		pMsg = NULL;
    LIST_HTTP_INFO	listHttpInfo; //用于缓存当前的完成的数据
    while( pMsg = curl_multi_info_read( m_pCurlHandle, &nMsgLeft ) )
    {
        if( pMsg->msg != CURLMSG_DONE )
        {
            continue;
        }
        LIST_HTTP_INFOItr itr = m_listHttpInfo.begin();
        for( ; itr != m_listHttpInfo.end(); ++itr )
        {
            if( (*itr)->m_pEasyHandle != pMsg->easy_handle )
            {
                continue;
            }
            break;
        }

        if( itr == m_listHttpInfo.end() )
        {
            SYS_CRITICAL( _SDT( "[%s: %d]:find easy handle failed!" ), MSG_MARK );
            break;
        }

        if( CURLE_OK != pMsg->data.result )
        {
            (*itr)->m_bError = FALSE;
            listHttpInfo.push_back((*itr));
            //释放
            curl_multi_remove_handle( m_pCurlHandle, (*itr)->m_pEasyHandle );
            m_listHttpInfo.erase( itr );
        }
        else
        {
            (*itr)->m_bError = TRUE;
            listHttpInfo.push_back((*itr));
            //释放
            curl_multi_remove_handle( m_pCurlHandle, (*itr)->m_pEasyHandle );
            m_listHttpInfo.erase( itr );
        }
    }

    //检查超时
    LIST_HTTP_INFOItr itr;
    UINT64 qwCurTime = SDTimeSecs();
    for( itr = m_listHttpInfo.begin(); itr != m_listHttpInfo.end();)
    {
        if(qwCurTime - (*itr)->m_qwLastAddTime > 30)
        {
            (*itr)->m_bError = FALSE;
            listHttpInfo.push_back((*itr));
            //释放
            curl_multi_remove_handle( m_pCurlHandle, (*itr)->m_pEasyHandle );
            m_listHttpInfo.erase( itr++);
        }
        else
        {
            itr++;
        }
    }

    for(itr = listHttpInfo.begin(); itr != listHttpInfo.end(); itr++)
    {
        (*itr)->OnExecuted();
        (*itr)->Release();
    }
    listHttpInfo.clear();
    return TRUE;
}

size_t CHttpMgr::OnRecv(void *buffer, size_t size, size_t count, void *pData)
{
    if( NULL == pData )
    {
        return 0;
    }
    //接收到数据
    CBaseHttpCMD *poHttpCmd = ( CBaseHttpCMD * )( pData );
    poHttpCmd->m_strResult.append( (char *)buffer, size * count );
    return size * count;
}

