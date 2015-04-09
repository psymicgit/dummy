

#ifndef _FORBID_REGISTERED_CMD_H_
#define _FORBID_REGISTERED_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


typedef struct _tagForbidRegistered
{
	UINT32		dwUserID;
	TCHAR		aszDeviceID[DEVICEID_LEN]; //设备号
	_tagForbidRegistered()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagForbidRegistered));
	}
}tForbidRegistered ;


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CForbidRegisteredCMD : public CBaseDBCMD
{
public:
    CForbidRegisteredCMD();
    ~CForbidRegisteredCMD();

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
        return EDBCT_FORBID_REGISTERED ;
    }

protected:
    BOOL ForbidRegistered(SGDP::ISDDBConnection* poDBConn, tForbidRegistered* pstReq);

private:
	UINT16 m_wErrCode;
};

#endif //#ifndef _LOGINCMD_H_

