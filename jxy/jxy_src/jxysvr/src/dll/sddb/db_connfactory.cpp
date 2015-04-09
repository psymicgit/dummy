#include "db_connfactory.h"
#include "db_mysqlconn.h"
#include "db_odbcconn.h"
#include "db_adoconn.h"
#include "sddbconfig.h"
#include "db_mockconn.h"
#include "sddebug.h"

CConnFactory::CConnFactory()
{
}

CConnFactory::~CConnFactory()
{
}

IConn * CConnFactory::GetConnection(SGDP::SDDBAccount *pstDBAccount)
{
	return GetConnection(pstDBAccount->m_wDBType, pstDBAccount->m_szHostName, pstDBAccount->m_wConnPort,
		pstDBAccount->m_szDBName, pstDBAccount->m_szLoginName, pstDBAccount->m_szLoginPwd, pstDBAccount->m_szCharactSet,
		pstDBAccount->m_szUnixSocket);
}

IConn * CConnFactory::GetConnection(INT32 wDBType, const CHAR * pszServerAddr, int nServerPort, const CHAR * pszDBName, 
			const CHAR * pszLoginName, const CHAR * pszPassword, const CHAR * pszCharactSet, const CHAR* pszUnixSocket)
{
	IConn * pConn = NULL;
	switch(wDBType)
	{
	case SGDP::SDDB_DBTYPE_MYSQL:
		{
			CMySQLConn * pMySQLConn = SDNew CMySQLConn();
			if(!pMySQLConn->Connect(pszServerAddr, nServerPort, pszDBName, pszLoginName, pszPassword, pszCharactSet, pszUnixSocket))
			{
				pMySQLConn->Release();
				delete pMySQLConn;
			}
			else
			{
				pConn = pMySQLConn;
			}
		}
		break;

#if (defined(WIN32) || defined(WIN64))
	case SGDP::SDDB_DBTYPE_ODBC:
		{
			CODBCConn * pODBCConn = SDNew CODBCConn();
			if(!pODBCConn->Connect(pszServerAddr, nServerPort, pszDBName, pszLoginName, pszPassword, pszCharactSet))
			{
				pODBCConn->Release();
				delete pODBCConn;
			}
			else
			{
				pConn = pODBCConn;
			}
		}
		break;

	case SGDP::SDDB_DBTYPE_ADO:
		{
			CADOConn * pADOConn = SDNew CADOConn();
			if(!pADOConn->Connect(pszServerAddr, nServerPort, pszDBName, pszLoginName, pszPassword, pszCharactSet))
			{
				pADOConn->Release();
				delete pADOConn;
			}
			else
			{
				pConn = pADOConn;
			}
		}
		break;
#endif

	case SGDP::SDDB_DBTYPE_MOCK:
		{
			pConn = SDNew CMockConn();
		}

	default:
		{
		}
	}
	return pConn;
}

IConn * CConnFactory::GetMockConnection()
{
	return CConnFactory::GetConnection(SGDP::SDDB_DBTYPE_MYSQL, NULL, 0, NULL, NULL, NULL, NULL, NULL);
}

