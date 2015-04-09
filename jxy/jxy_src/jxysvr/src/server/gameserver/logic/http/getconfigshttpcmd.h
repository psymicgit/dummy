

#ifndef _GETCONFIGS_HTTP_CMD_H_
#define _GETCONFIGS_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>
#include <json/json.h>



struct SGetConfigs
{
    SGetConfigs()
    {
		wZoneID = 0;
    }
	UINT16 wZoneID;
};



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetConfigsHttpCMD : public CBaseHttpCMD
{
public:
    CGetConfigsHttpCMD();
    ~CGetConfigsHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_GETCONFIGS;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);


	//同步
	virtual VOID OnSyncExecute();

	static size_t OnRecv(void *buffer, size_t size, size_t count, void *pData);

private:
	BOOL ResVersionsResult(Json::Value datas);
	BOOL ProductsResult(Json::Value datas);
	BOOL VipLevelResult(Json::Value datas);

private:
    SGetConfigs stGetConfigs;

};

#endif //#ifndef _BASEDBCMD_H_

