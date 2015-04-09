

#ifndef _BASESMSCMD_H_
#define _BASESMSCMD_H_

#include <sdframework.h>
#include <sddb.h>
#include <curl/curl.h>

#include "common//client/errdef.h"



#define SAVE_USER_DATA			10000

//命令类型
enum ESMSCmdType
{
	ESMS_RESETPASSWD = 1,//重置密码
};


#define ACTION_MT		"http://vip.4001185185.com/sdk/smssdk!mt.action"
#define SDK			"13779953612"
#define CODE			"qtk88888"
#define PHONES		"18150155123"
#define MSG			"工作愉快"
#define RESULTTYPE	"xml"
#define SUBCODE		"2278"
#define RESULTSTR		"<result>发送成功</result>"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CBaseSmsCMD
{
public:
    CBaseSmsCMD();
    ~CBaseSmsCMD();

    /**
    * @brief 释放DBCommand,用户请务必保证此DBCommand内容被完全释放
    * @return VOID
    */
    VOID SDAPI Release();

	/////////////////////////////////////////////////////////////////////

    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID, UINT32 dwUserID = 0, VOID* pParm = NULL);


	//数据执行
	virtual VOID OnExecute(void);

	//数据执行完后回调
	virtual VOID OnExecuted(void);

public:
	virtual ESMSCmdType GetSMSCmdType() = 0;

protected:

    /**
    * @brief 发送回馈消息
    * @param pBuf 需要回馈的数据
    * @return VOID
    */
    VOID ResponseMsg(UINT16 wMsgID, const CHAR* pBuf);

	static size_t OnRecv(void *buffer, size_t size, size_t count, void *user_p);

protected:
    CHAR				m_pUserData[65535];
    INT32			m_dwDataLen;
    volatile BOOL		m_bSuccess;		///< 数据库操作成功标志
    UINT32			m_dwTransID;
	UINT32			m_dwUserID;
    CHAR				m_szParm[1024];
	string			m_strNetData;
};

#endif //#ifndef _BASEDBCMD_H_

