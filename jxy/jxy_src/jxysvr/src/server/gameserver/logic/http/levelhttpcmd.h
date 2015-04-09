

#ifndef _LEVEL_HTTP_CMD_H_
#define _LEVEL_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SLevelInfo
{
    SLevelInfo()
    {
        dwPlayerID = 0;
        wLevel = 0;
		strDeviceID = "";
		strIp = "";
		byAuthType = 0;
    }
    UINT32	dwPlayerID; //实际订单对应的玩家ID
    UINT16	wLevel;
	string	strDeviceID;
	string	strIp;
	UINT8 byAuthType;
};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CLevelHttpCMD : public CBaseHttpCMD
{
public:
    CLevelHttpCMD();
    ~CLevelHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_LEVEL;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);
private:
    SLevelInfo stLevelInfo;

};

#endif //#ifndef _BASEDBCMD_H_

