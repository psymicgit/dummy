

#ifndef _BASE_HTTP_CMD_H_
#define _BASE_HTTP_CMD_H_

#include <string>
#include <sdtype.h>
#include "curl/curl.h"
using namespace std;

//HTTP命令类型
enum EHttpCmdType
{
	EHCT_LOGINEX = 1,
	EHCT_NOTICE = 2,
	EHCT_REGISTER = 3,	//注册
	EHCT_LOGOUT = 4,	//登出
	EHCT_CHANGEPWD = 5,	//修改密码
	EHCT_BIND_MOBILE = 6,//绑定手机
	EHCT_BIND_CHECK_VER_CODE = 7,	//绑定手机通过验证码
	EHCT_UNBIND_MOBILE = 8,	//解绑手机
	EHCT_UNBIND_CHECK_VER_CODE = 9,//解绑定手机通过验证码
	EHCT_GET_USER_EXPAND = 10,	//获取用户额外信息
	EHCT_LOGIN = 11,	//登入

};

#define CENTER_SIGN	"Bef^3rwsvn!29jcx57ng"

class CHttpMgr;
class CBaseHttpCMD
{
    friend class CHttpMgr;
public:
    CBaseHttpCMD();
    ~CBaseHttpCMD();

    virtual BOOL  Init() = 0;

    /**
    * @brief 释放DBCommand,用户请务必保证此DBCommand内容被完全释放
    * @return VOID
    */
    VOID  Release();

    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID) = 0;
public:
    virtual EHttpCmdType GetHttpCmdType() = 0;

protected:
    /**
    * @brief 发送回馈消息
    * @param pBuf 需要回馈的数据
    * @return VOID
    */
    //数据执行完后回调
    virtual BOOL OnExecuted() = 0;


	VOID ResponseMsg(UINT16 wMsgID, const CHAR* pBuf, BOOL bDisconnect = FALSE);

	virtual string UrlSig();

protected:
    CURL *		m_pEasyHandle; //URL的信息
    UINT64		m_qwLastAddTime;//上次添加URL请求时间
    CHAR			m_szErrorBuff[CURL_ERROR_SIZE]; //错误信息
    string		m_strResult;
    string		m_strUrl;
    string		m_strfields;
    UINT32		m_dwTransID;
	BOOL			m_bError;
};

#endif //#ifndef _BASEDBCMD_H_

