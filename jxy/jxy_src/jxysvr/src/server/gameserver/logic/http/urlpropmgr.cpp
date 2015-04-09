#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "urlpropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>
#include <db/dbmgr.h>

IMPLEMENT_SINGLETON(CUrlPropMgr);

CUrlPropMgr::CUrlPropMgr()
{
}

CUrlPropMgr::~CUrlPropMgr()
{
}

BOOL CUrlPropMgr::Init()
{
    return LoadFromDB();
}

VOID CUrlPropMgr::UnInit()
{

}

// 从数据源获取数据
BOOL CUrlPropMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;

    char v_szSql[256] = {0};
    sprintf(v_szSql, "select * from httpurl where (ZoneID='%u' or ZoneID='0') order by ZoneID ", gsapi::GetLocalID());
    INT32 nRet = pDBSession->ExecuteSqlRs(v_szSql, &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT16 wType = SGDP::SDAtou(pRes->GetFieldValueByName("type"));
        const CHAR* pszAddr = pRes->GetFieldValueByName("address");

        switch(wType)
        {
        case HTTP_APPPAY:
            m_strUrlApp = pszAddr;
            break;
        case HTTP_LEVEL:
            m_strUrlLevel = pszAddr;
            break;
        case HTTP_NAME:
            m_strUrlName = pszAddr;
            break;
        case HTTP_BUGGOLDINFO:
            m_strUrlBuyGoldInfo = pszAddr;
            break;
        case HTTP_CHATENCOURAGE:
            m_strUrlChatEncourage = pszAddr;
            break;
        case HTTP_RETREATE:
            m_strUrlRetreate = pszAddr;
            break;
        case HTTP_PUSH_PLAYER_INFO:
            m_strUrlPushPlayerInfo = pszAddr;
            break;
		case HTTP_GET_MAX_PLAYERID:
			m_strUrlGetMaxPlayerID = pszAddr;
			break;
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    if (m_strUrlApp == ""
            || m_strUrlLevel == ""
            || m_strUrlName == ""
            || m_strUrlBuyGoldInfo == ""
            || m_strUrlChatEncourage == ""
            || m_strUrlRetreate == ""
			|| m_strUrlPushPlayerInfo == ""
			|| m_strUrlGetMaxPlayerID == "")
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data! UrlApp:%s,UrlLevel:%s,UrlName:%s,UrlRetreate"), MSG_MARK, m_strUrlApp.c_str(), m_strUrlLevel.c_str(), m_strUrlName.c_str(),m_strUrlRetreate.c_str());
        return FALSE;
    }
    return TRUE;
}
