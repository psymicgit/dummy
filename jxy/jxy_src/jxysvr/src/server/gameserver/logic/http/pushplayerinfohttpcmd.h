///<------------------------------------------------------------------------------
//< @file:   logic\http\pushplayerinfohttpcmd.h
//< @author: hongkunan
//< @date:   2014年4月1日 9:28:43
//< @brief:  将最新的玩家信息<等级、昵称、职业>推送到用户中心
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pushplayerinfohttpcmd_h_
#define _pushplayerinfohttpcmd_h_

#include "basehttpcmd.h"

struct SPlayerBaseInfo
{
    SPlayerBaseInfo()
        : dwPlayerID(0)
        , wLevel(0)
        , wCoachHeroID(0)
		, byVipLv(0)
		, byAuthType(0)
    {
        dwPlayerID = 0;
    }

    UINT32 dwPlayerID;  // 玩家ID
    UINT16 wLevel;      // 该玩家当前等级
    UINT16 wCoachHeroID;        // 该玩家当前职业
	UINT8  byVipLv;		// 玩家vip等级
	UINT8 byAuthType; 
    string strNickName; // 该玩家当前昵称
};

// 将最新的玩家信息<等级、昵称、职业>推送到用户中心
class CPushPlayerInfoHttpCMD : public CBaseHttpCMD
{
public:
    CPushPlayerInfoHttpCMD();
    ~CPushPlayerInfoHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_PUSH_PLAYER_INFO;
    }

    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

private:
    void EscapeFields();
    void EscapeUrl(CHAR szUrl[], UINT16 wLen);

private:
    SPlayerBaseInfo m_stPlayerInfo;
};

#endif //_pushplayerinfohttpcmd_h_