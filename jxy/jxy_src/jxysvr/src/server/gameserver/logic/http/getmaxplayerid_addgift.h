///<------------------------------------------------------------------------------
//< @file:   logic\http\getmaxplayerid_addgift.h
//< @author: Kang_zhiquan
//< @date:   2014年7月28日 16:30:36
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _getmaxplayerid_addgift_h_
#define _getmaxplayerid_addgift_h_

#include "basehttpcmd.h"
#include "db/addplayergift.h"
#include "logic/http/httpmgr.h"

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetMaxPlayerID_AddGift : public CBaseHttpCMD
{
public:
    CGetMaxPlayerID_AddGift();
    ~CGetMaxPlayerID_AddGift();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_GET_MAX_PLAYERID_ADD_GIFT;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);
private:
	SActivityGift m_stGift;
};

#endif //_getmaxplayerid_addgift_h_