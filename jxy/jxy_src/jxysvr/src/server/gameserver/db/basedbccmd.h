

#ifndef _BASEDBCMD_H_
#define _BASEDBCMD_H_


#include "dbmgr.h"
#include <sddb.h>

#define SAVE_USER_DATA			10000

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CBaseDBCMD: public SGDP::ISDDBCommand
{
public:
	CBaseDBCMD();
	virtual ~CBaseDBCMD();

	/**
	* @brief 释放DBCommand,用户请务必保证此DBCommand内容被完全释放
	* @return VOID
	*/
    VOID SDAPI Release();

/////////////////////////////////////////////////////////////////////

	/**
	* @brief 设置用户特定数据,回调子类的SetUserData
	* @param pUserData 设置的用户数据
	* @param dwTransID 设置的TransID
	*/
	VOID SetUserData(VOID* pUserData, UINT32 dwTransID );
	virtual VOID SetUserData(VOID* pUserData) = 0;
	
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
	
    volatile BOOL m_bSuccess;		///< 数据库操作成功标志    
	UINT32 m_dwTransID;
};

#endif //#ifndef _BASEDBCMD_H_

