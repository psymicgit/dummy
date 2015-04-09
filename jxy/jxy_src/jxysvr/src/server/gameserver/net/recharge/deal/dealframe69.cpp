// DealFrame69.cpp: implementation of the CDealFrame69 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe69.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/player/playermgr.h"
#include "logic/hero//heropropmgr.h"
#include <framework/gsapi.h>
#include <db/getgsnoticecmd.h>
#include "common/client/errdef.h"
#include <db/autosqlbuf.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//转码后 6个字符相当于 1个中文 1500为250个字
#define MAX_NOTICE_LEN   1024 

#define ALL_ZONE  1
#define SINGLE_ZONE 2
#define AREA_ZONE 3

CDealFrame69::CDealFrame69()
{

}

CDealFrame69::~CDealFrame69()
{

}

void CDealFrame69::Release()
{

}

UINT16 CDealFrame69::GetFrameType()
{
    return 69;
}

void CDealFrame69::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }

    std::string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg);
        return;
    }

    ISDDBRecordSet* pRecordSet = NULL;
 
    Json::Value root;
    Json::Reader reader;
    UINT32 dwLength = SDStrlen(pszParam5);
    if (dwLength > MAX_NOTICE_LEN)
    {
        strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_TOO_LONG_MESS);
        poClinet->Rspone(strErrorMsg);
        return;
    }

    std::string strMessage = pszParam5;

    CAutoSqlBuf oAutoSqlBufSelect;
    CHAR *pszSqlSelect = oAutoSqlBufSelect.GetSqlBuf();
    ISDDBRecordSet*pRecordSetSelect = NULL;
	memset(pszSqlSelect, 0, MAX_SQL_LEN);
	
	SDSnprintf(pszSqlSelect, MAX_SQL_LEN - 1,"delete from gsnotice where ZoneID=%d;insert into gsnotice ( NoticeContent, ExpiresTime, ZoneID, ForcePopFlag) values( '%s', '%s', %d, %u);", dwParam1, 
		strMessage.c_str(), pszParam4, dwParam1,1);
  
    INT32 nRet = pDBSession->ExecuteSqlRs(pszSqlSelect, &pRecordSet);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    if (nRet < 0)
    {
        strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
    } 
    
    poClinet->Rspone(strErrorMsg);
}

void CDealFrame69::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
    return;
}

string CDealFrame69::GetStrFrameType()
{
    return CHANGE_NOTICE;
}

void CDealFrame69::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string,string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;

    RsponeMsg(poClinet,mapField);
}

string CDealFrame69::GetStrFrameDesc()
{
    return SDGBK2UTF8(CHANGE_NOTICE_DESC);
}

