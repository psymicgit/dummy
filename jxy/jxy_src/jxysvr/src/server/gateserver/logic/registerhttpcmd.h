#ifndef _REGISTER_HTTP_CMD_H_
#define _REGISTER_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>

struct SResgisterInfo
{
	SResgisterInfo()
	{
		strDeviceid = "";
		strUserName = "";
		strUserPwd = "";
		byAuthType = 0;
		dwPlayerID = 0;
		dwUserID = 0;
	}
	string strDeviceid;
	string strUserName;
	string strUserPwd;
	UINT8  byAuthType; //认证类型
	UINT32 dwPlayerID;
	UINT32 dwUserID;
};

class CRegisterHttpCMD : public CBaseHttpCMD
{
public:
	CRegisterHttpCMD();
	~CRegisterHttpCMD();

	  virtual BOOL  Init();

    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);
public:
	virtual EHttpCmdType GetHttpCmdType(){ return EHCT_REGISTER; }

protected:
    /**
    * @brief 发送回馈消息
    * @param pBuf 需要回馈的数据
    * @return VOID
    */
    //数据执行完后回调
    virtual BOOL OnExecuted();
	BOOL SendMessage(UINT16 wErrcode, const TCHAR* aszUserName);
private:
	SResgisterInfo m_stRegisterInfo;
};
#endif