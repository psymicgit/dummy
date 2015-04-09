

#ifndef _BINDSMSCMD_H_
#define _BINDSMSCMD_H_

#include <sdframework.h>
#include <sddb.h>
#include <curl/curl.h>

#include "basesmscmd.h"
#include "protocol/server/protoclizs.h"



#define SAVE_USER_DATA			10000


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CResetPasswdSmsCMD : public CBaseSmsCMD
{
public:
    CResetPasswdSmsCMD();
    ~CResetPasswdSmsCMD();
	//数据执行
	virtual VOID OnExecute(void);

	//数据执行完后回调
	virtual VOID OnExecuted(void);

	virtual ESMSCmdType GetSMSCmdType()
	{
		return ESMS_RESETPASSWD;
	}

protected:
	PKT_CLIZS_RESET_PASSWD_MOBILE_ACK m_stAck;
};

#endif //#ifndef _BASEDBCMD_H_

