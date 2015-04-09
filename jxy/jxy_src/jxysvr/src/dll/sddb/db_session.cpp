#include "db_session.h"

CDBSession::CDBSession() :
    m_pPool(NULL)
{
}

CDBSession::CDBSession(CConnPool * pPool) :
    m_pPool(pPool)
{
}

CDBSession::~CDBSession()
{
    if(NULL != this->m_pPool)
    {
        //this->m_pPool->Terminate();
        delete this->m_pPool;
        this->m_pPool = NULL;
    }
}

UINT32 CDBSession::EscapeString(const CHAR *pSrc, INT32 nSrcSize,
                                CHAR *pDest,    INT32 nDstSize, INT32 timeout)
{
    SGDP::ISDDBConnection * pConn = this->m_pPool->GetConnection(-1);
    UINT32 ret = pConn->EscapeString(pSrc, nSrcSize, pDest, nDstSize);
    this->m_pPool->Return(pConn);
    return ret;
}

INT32 CDBSession::ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId, INT32 timeout)
{
    SGDP::ISDDBConnection * pConn = this->m_pPool->GetConnection(-1);
    INT32 ret = pConn->ExecuteSql(pSQL, pInsertId);
    this->m_pPool->Return(pConn);
    return ret;
}

INT32 CDBSession::ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs,  UINT32* pdwErr, string* pstrErr, INT32 timeout)
{
    SGDP::ISDDBConnection * pConn = this->m_pPool->GetConnection(-1);
    INT32 ret = pConn->ExecuteSqlRs(pSQL, ppoRs, pdwErr, pstrErr);
    this->m_pPool->Return(pConn);
    return ret;
}

/**
* @brief 进入一个事务
*/
SGDP::ISDDBConnection* CDBSession:: GetDBConnection()
{
    return this->m_pPool->GetConnection(-1);
}

/**
* @brief 完成一个事务
*/
void CDBSession:: FreeDBConnection(SGDP::ISDDBConnection*pConn)
{
    if (NULL == pConn)
    {
        return;
    }
    this->m_pPool->Return(pConn);
}


bool CDBSession::CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet, INT32 timeout)
{
    SGDP::ISDDBConnection * pConn = this->m_pPool->GetConnection(-1);
    bool ret = pConn->CreateDB(pcDBName, bForce, pcCharSet);
    this->m_pPool->Return(pConn);
    if(ret)
    {
        ret = this->m_pPool->SelectDB(pcDBName);
    }
    return ret;
}

bool CDBSession::SelectDB(const CHAR *pcDBName, INT32 timeout)
{
    bool ret = this->m_pPool->SelectDB(pcDBName);
    //SGDP::ISDDBConnection * pConn = this->m_pPool->GetConnection(-1);
    //bool ret = pConn->SelectDB(pcDBName);
    //this->m_pPool->Return(pConn);
    return ret;
}

bool CDBSession::AddDBCommand(SGDP::ISDDBCommand *poDBCommand)
{
    return this->m_pPool->AddDBCommand(poDBCommand);
}

bool CDBSession::QuickAddDBCommand(SGDP::ISDDBCommand *poDBCommand)
{
    return this->m_pPool->QuickAddDBCommand(poDBCommand);
}

bool CDBSession::Run(UINT32& dwCount)
{
    return this->m_pPool->Run(dwCount);
}

UINT32 CDBSession::GetExcuteCmdCount()
{
    return this->m_pPool->GetExcuteCmdCount();
}

UINT32 CDBSession::GetExcutedCmdCount()
{
    return this->m_pPool->GetExcutedCmdCount();
}

