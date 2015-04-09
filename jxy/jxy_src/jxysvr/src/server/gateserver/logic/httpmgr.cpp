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
#include "framework/gtapplication.h"
#include "net/cli/clipktbuilder.h"
#include "loginexhttpcmd.h"
#include "noticehttpcmd.h"
#include "registerhttpcmd.h"
#include "logouthttpcmd.h"
#include "changepwdhttpcmd.h"
#include "getuserexpandhttpcmd.h"
#include "bindmobilehttpcmd.h"
#include "unbindcheckvercode.h"
#include "unbindmobilehttpcmd.h"
#include "bindcheckvercode.h"

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
    case EHCT_LOGINEX:
        poBaseCMD = m_oLoginExHttpCMDPool.Alloc();
        break;
    case EHCT_NOTICE:
        poBaseCMD =  m_oNoticeHttpCMDPool.Alloc();
        break;
	case EHCT_REGISTER:
		poBaseCMD = m_oRegisterHttpCMDPool.Alloc();
		break;
	case EHCT_LOGOUT:
		poBaseCMD = m_oLogoutHttpCMDPool.Alloc();
		break;
	case EHCT_CHANGEPWD:
		poBaseCMD = m_oChangePwdHttpCMDPool.Alloc();
		break;
	case EHCT_GET_USER_EXPAND:
		poBaseCMD = m_oGetUserExpandHttpCMDPool.Alloc();
		break;
    case EHCT_BIND_MOBILE:
        poBaseCMD = m_oBindMobileHttpCMDPool.Alloc();
        break;
    case EHCT_BIND_CHECK_VER_CODE:
        poBaseCMD = m_oBindCheckVerCodeHttpCMDPool.Alloc();
        break;
    case EHCT_UNBIND_MOBILE:
        poBaseCMD = m_oUnBindMobileHttpCMDPool.Alloc();
        break;
    case EHCT_UNBIND_CHECK_VER_CODE:
        poBaseCMD = m_oUnBindCheckVerCodeHttpCMDPool.Alloc();
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
    case EHCT_LOGINEX:
        m_oLoginExHttpCMDPool.Free((CLoginExHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_NOTICE:
        m_oNoticeHttpCMDPool.Free((CNoticeHttpCMD*)poBaseDBCMD);
        break;
	case EHCT_REGISTER:
		m_oRegisterHttpCMDPool.Free((CRegisterHttpCMD*)poBaseDBCMD);
		break;
	case EHCT_LOGOUT:
		m_oLogoutHttpCMDPool.Free((CLogoutHttpCMD*)poBaseDBCMD);
		break;
	case EHCT_CHANGEPWD:
		m_oChangePwdHttpCMDPool.Free((CChangePwdHttpCMD*)poBaseDBCMD);
		break;
	case EHCT_GET_USER_EXPAND:
		m_oGetUserExpandHttpCMDPool.Free((CGetUserExpandHttpCMD*)poBaseDBCMD);
		break;
    case EHCT_BIND_MOBILE:
        m_oBindMobileHttpCMDPool.Free((CBindMobileHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_BIND_CHECK_VER_CODE:
        m_oBindCheckVerCodeHttpCMDPool.Free((CBindCheckVerCodeHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_UNBIND_MOBILE:
        m_oUnBindMobileHttpCMDPool.Free((CUnBindMobileHttpCMD*)poBaseDBCMD);
        break;
    case EHCT_UNBIND_CHECK_VER_CODE:
        m_oUnBindCheckVerCodeHttpCMDPool.Free((CUnBindCheckVerCodeHttpCMD*)poBaseDBCMD);
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

