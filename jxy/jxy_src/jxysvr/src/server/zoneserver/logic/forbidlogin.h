

#ifndef _FORBID_LOGIN_CMD_H_
#define _FORBID_LOGIN_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


typedef struct _tagForbidLogin
{
	UINT32		dwPlayerID;
	UINT32		dwUserID;
	TCHAR		aszDeviceID[DEVICEID_LEN]; //设备号
	UINT8		byType;
	_tagForbidLogin()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagForbidLogin));
	}
} tForbidLogin;


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CForbidLoginCMD : public CBaseDBCMD
{
public:
    CForbidLoginCMD();
    ~CForbidLoginCMD();

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
        return EDBCT_FORBID_LOGIN ;
    }

protected:
    BOOL ForbidLogin(SGDP::ISDDBConnection* poDBConn, tForbidLogin* pstReq);

private:
	UINT16 m_wErrCode;
};

#endif //#ifndef _LOGINCMD_H_

