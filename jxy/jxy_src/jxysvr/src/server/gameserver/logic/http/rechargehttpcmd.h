

#ifndef _HTTP_RECHARGE_CMD_H_
#define _HTTP_RECHARGE_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include  "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>
#include <json/json.h>

struct SRechargeInfo
{
    SRechargeInfo()
    {
        ZoneID = 0;
        IndexMax = 0;
    }
    UINT32   ZoneID; //玩家ID
    UINT32	IndexMax;//当前player_recharge最大的索引ID
};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CRechargeHttpCMD : public CBaseHttpCMD
{
public:
    CRechargeHttpCMD();
    ~CRechargeHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_RECHARGE;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pRechargeInfo, UINT32 dwLen, UINT32 dwTransID);
public:
	BOOL ProcessResult(Json::Value &datas);
private:
    SRechargeInfo stRechargeInfo;

};

#endif //#ifndef _BASEDBCMD_H_

