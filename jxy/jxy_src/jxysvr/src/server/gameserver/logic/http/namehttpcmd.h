

#ifndef _NAME_HTTP_CMD_H_
#define _LOGIN_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SNameInfo
{
    SNameInfo()
    {
        dwPlayerID = 0;
        strName = "";
        strOldName = "";
		byAuthType = 0;
    }
    UINT32   dwPlayerID; //玩家ID
    string	strName;
    string	strOldName;
	UINT8 byAuthType;
};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CNameHttpCMD : public CBaseHttpCMD
{
public:
    CNameHttpCMD();
    ~CNameHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_NAME;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);
private:
    SNameInfo stNameInfo;

};

#endif //#ifndef _BASEDBCMD_H_

