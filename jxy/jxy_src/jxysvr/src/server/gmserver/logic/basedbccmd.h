

#ifndef _BASEDBCMD_H_
#define _BASEDBCMD_H_

#include "sdframework.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CBaseDBCMD: public SGDP::ISDDBCommand
{
public:
	CBaseDBCMD();
	~CBaseDBCMD();

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
	VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

	
public:
	virtual EDBCmdType GetDBCmdType() = 0;

protected:

	/**
	* @brief 发送回馈消息
	* @param pBuf 需要回馈的数据
	* @return VOID
	*/
	VOID ResponseMsg(UINT16 wMsgID,const CHAR* pBuf);


protected:
	CHAR    m_pUserData[65535];
	INT32   m_dwDataLen;
	
    volatile BOOL m_bSuccess;		///< 数据库操作成功标志

	UINT32 m_dwTransID;
};

#endif //#ifndef _BASEDBCMD_H_

