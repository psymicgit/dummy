

#ifndef _CHAGNE_PWD_CMD_H_
#define _CHAGNE_PWD_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


typedef struct _tagChangePwd
{
	UINT8	byDevice;
	UINT32	dwPlayerID;
	TCHAR	szUserPwd[USERNAME_LEN];
	_tagChangePwd()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagChangePwd));
	}
} tChangePwd;


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CChangePwdCMD : public CBaseDBCMD
{
public:
    CChangePwdCMD();
    ~CChangePwdCMD();

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
        return EDBCT_CHANGE_PWD ;
    }

protected:
    BOOL ChangePwd(SGDP::ISDDBConnection* poDBConn, tChangePwd* pstReq);

private:
	UINT16	m_wErrCode;
	UINT32	m_dwUserID;
};

#endif //#ifndef _LOGINCMD_H_

