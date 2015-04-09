#ifndef _CHANGEPWD_HTTP_CMD_H_
#define _CHANGEPWD_HTTP_CMD_H_


#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>


struct SChangePwdInfo
{
	SChangePwdInfo()
	{
		strDeviceid = "";
		dwUserID = 0;
		strNewPwd = "";
		strOldPwd = "";
		dwPlayerID = 0;
	}
	string strDeviceid;
	UINT32 dwUserID;
	string strNewPwd;
	string strOldPwd;
	UINT32 dwPlayerID;
};

class CChangePwdHttpCMD : public CBaseHttpCMD
{
public:
	CChangePwdHttpCMD();
	~CChangePwdHttpCMD();

	virtual BOOL  Init();
	 /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);
public:
	virtual EHttpCmdType GetHttpCmdType(){ return EHCT_CHANGEPWD; }

protected:
    /**
    * @brief 发送回馈消息
    * @param pBuf 需要回馈的数据
    * @return VOID
    */
    //数据执行完后回调
    virtual BOOL OnExecuted();
	BOOL SendMessage(UINT16 wErrcode);
private:
	SChangePwdInfo m_stChangePwdInfo;
};

#endif