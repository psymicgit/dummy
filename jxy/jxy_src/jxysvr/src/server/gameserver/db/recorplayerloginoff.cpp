#include "recorplayerloginoff.h"
#include "common/server/utility.h"
#include "dll/sdframework/sdutil.h"
#include <sdloggerimpl.h>


CRecordPlayerLoginoff::CRecordPlayerLoginoff()
{
    m_strSql.resize( SQL_STATEMENT_MAX_LEN );
}

CRecordPlayerLoginoff::~CRecordPlayerLoginoff()
{

}

VOID SDAPI CRecordPlayerLoginoff::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    sprintf( const_cast<char *>( m_strSql.c_str() ), "insert into playerloginoff( PlayerID, Login, Time, LoginIP) \
													 values( %u, %d, now(), '%s' )",
             m_stLogin.dwPlayerID, m_stLogin.bLogin, m_stLogin.strIP.c_str() );

    //DBG_INFO( _SDT( "CRecordPlayerLoginoff£¬sql:%s" ), m_strSql.c_str() );

	UINT32 dwErr;
	string strError;

    INT32 nRet = poDBConn->ExecuteSql(m_strSql.c_str(), NULL, NULL, &dwErr, &strError);
    if( nRet < 0 )
    {
		SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), m_strSql.c_str()); 
    }
}

VOID SDAPI CRecordPlayerLoginoff::OnExecuted()
{

}

VOID CRecordPlayerLoginoff::SetUserData(VOID* pUserData)
{
    m_stLogin = *(SPlayerLogin *)pUserData;
}