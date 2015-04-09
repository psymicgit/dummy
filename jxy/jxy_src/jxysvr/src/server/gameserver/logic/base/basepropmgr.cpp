#include "basepropmgr.h"
#include <framework/gsapi.h>
#include <framework/gsconfig.h>

#include <db/dbmgr.h>

map<UINT16, CBasePopMgr*> CBasePopMgr::m_mapKind2Mgr;


string GetPropTable(string strTableName)
{
	string strSpeCfgTablePrefix = CGSConfig::Instance()->GetSpeCfgTablePrefix() ;
	if(strSpeCfgTablePrefix.empty())
	{
		return strTableName;
	}
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	string strSpeTableName = strSpeCfgTablePrefix + strTableName;
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	CHAR szBuff[256] = {0};	
	sprintf(szBuff, "select * from %s;", strSpeTableName.c_str());
	if(pDBSession->ExecuteSqlRs(szBuff, &pRes, &dwErrID,  &strErr) >= 0)
	{   
		return strSpeTableName;
	}	

	return strTableName;
}

string GetPropTableSelectSql(string strTableName, string strPostfixSql, string strPrefixSql)
{
	string strRealTableName = GetPropTable(strTableName);
	CHAR szSql[1024] = {0};
	sprintf(szSql, "%s %s %s ;", strPrefixSql.c_str(), strRealTableName.c_str(), strPostfixSql.c_str());
	return szSql;
}
