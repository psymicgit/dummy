

#ifndef _SET_ACCOUNT_TEST_CMD_H_
#define _SET_ACCOUNT_TEST_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


typedef struct _tagSetAccountType
{
	UINT32		dwPlayerID;
	UINT8		byType;
	_tagSetAccountType()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagSetAccountType));
	}
} tSetAccountType;


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetAccountTypeCMD : public CBaseDBCMD
{
public:
    CSetAccountTypeCMD();
    ~CSetAccountTypeCMD();

    /**
    * @brief 异步执行SQL命令
    * @poDBConn 异步执行SQL命令的Connection连接
    * @return VOID
    */
    VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection* poDBConn);

    /**
    * @brief 同步执行SQL命令相关的逻辑部分
    * @return VOID
    */
    VOID SDAPI OnExecuted();

public:
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_SETACCOUNTTYPE ;
    }

protected:
    BOOL SetAccountType(SGDP::ISDDBConnection* poDBConn, tSetAccountType* pstReq);

private:
	UINT16 m_wErrCode;
};

#endif //#ifndef _LOGINCMD_H_

