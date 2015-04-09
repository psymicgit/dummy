#include "noticemgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <db/autosqlbuf.h>
#include <framework/gsconfig.h>
#include <net/cli/usermgr.h>
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/activity/eatbreadpropmgr.h"
#include "logic/activity/bossbattlepropmgr.h"
#include <logic/other/errmsgmgr.h>
#include <framework/gsapi.h>

IMPLEMENT_SINGLETON(CNoticeMgr)


CNoticeMgr::CNoticeMgr()
{

}

CNoticeMgr::~CNoticeMgr()
{

}

BOOL CNoticeMgr::Init()
{
    return TRUE;
}

VOID CNoticeMgr::UnInit()
{

}

VOID CNoticeMgr::OnRankNotify()
{
	SPlayerBaseData *poPlayerBaseData = NULL;
	CPlayerID2PlayerBaseDataMapConstItr itr;
	const CPlayerID2PlayerBaseDataMap &  mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
	itr = mapPlayerID2PlayerBaseData.begin();
	for (itr; itr != mapPlayerID2PlayerBaseData.end(); itr++)
	{
		poPlayerBaseData = itr->second;
		if (NULL == poPlayerBaseData)
		{
			continue;
		}

		//检测玩家是否是机器人。
		if(ERFK_PLAYER != poPlayerBaseData->byRobotFunc)
		{
			continue;
		}

		//过虑掉可能刚开放竟技场的玩家默认不存在的数据
		if (0 == poPlayerBaseData->dwRank || 0 == poPlayerBaseData->dwNotifyRank)
		{
			continue;
		}


		//排名不变或上升，都不推送
		if (poPlayerBaseData->dwRank >= poPlayerBaseData->dwNotifyRank)
		{
			continue;
		}

		//最后登录时候大于登出时间，可能完家刚好在线，或者出错。
		if (poPlayerBaseData->qwLoginTime > poPlayerBaseData->qwLogoffTime)
		{
			poPlayerBaseData->qwLogoffTime = poPlayerBaseData->qwLoginTime;
		}

		//没有开启推送通知的，直接 放过。
		//if (0 == strlen((char*)poPlayerBaseData->abyToken))
		//{
		//	continue;
		//}

		CSDDateTime oNow(SDTimeSecs());
		CSDDateTime oLogoffTime(poPlayerBaseData->qwLogoffTime);
		//检测完家时间是否已达两天未登陆。并且每隔再从天发一次。
		UINT64 qwDiffDay = oNow.DiffDay(oLogoffTime);	//计算离线的天数。
		if ((qwDiffDay % 2 == 0) && (qwDiffDay / 2 != 0))
		{
			char szPayLoad[256] = {0};
			sprintf(szPayLoad, "rank %u down to %u", poPlayerBaseData->dwRank, poPlayerBaseData->dwNotifyRank);
			gsapi::NoticeMsg(poPlayerBaseData->dwID, ENT_RACE, (CHAR*)poPlayerBaseData->abyToken, (CHAR*)SDGBK2UTF8(szPayLoad).c_str());

			USR_INFO(_SDT("[%s: %d]: playerid:%u PayLoad:%s"), MSG_MARK, poPlayerBaseData->dwID, szPayLoad);
		}
		else
		{
			DBG_INFO(_SDT("[%s: %d]:playerid [%u] nod dif two day"), MSG_MARK, poPlayerBaseData->dwID);
		}
	}
}

VOID CNoticeMgr::OnActivityNotifyEatBread(UINT8 byStartHour, UINT8 byStartMin)
{
	SPlayerBaseData *poPlayerBaseData = NULL;
	CPlayerID2PlayerBaseDataMapConstItr itr;
	const CPlayerID2PlayerBaseDataMap &  mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
	itr = mapPlayerID2PlayerBaseData.begin();
	for (itr; itr != mapPlayerID2PlayerBaseData.end(); itr++)
	{
		poPlayerBaseData = itr->second;
		if (NULL == poPlayerBaseData)
		{
			continue;
		}

		//检测玩家是否是机器人。
		if(ERFK_PLAYER != poPlayerBaseData->byRobotFunc)
		{
			continue;
		}

		if (0 == poPlayerBaseData->bEatBreadNotify)
		{
			continue;
		}

		//没有开启推送通知的，直接 放过。
		//if (0 == strlen((char*)poPlayerBaseData->abyToken))
		//{
		//	continue;
		//}

		CHAR szPayLoad[256] = {0};
		//sprintf(szPayLoad, "吃包子活动将于%u:%u开始", byStartHour, byStartMin);
		//((CGSApplication*)SDGetApp())->NoticeMsg(poPlayerBaseData->dwID, ENT_EATBREAD, (CHAR*)poPlayerBaseData->abyToken, (CHAR*)SDGBK2UTF8(szPayLoad).c_str());
		vector<string> vecFillMsg;
		CHAR szBuff[32] = {0};
		sprintf(szBuff, "%u", byStartHour);
		vecFillMsg.push_back(szBuff);
		sprintf(szBuff, "%u", byStartMin);
		vecFillMsg.push_back(szBuff);
		gsapi::NoticeMsg(poPlayerBaseData->dwID, ENT_EATBREAD, (CHAR*)poPlayerBaseData->abyToken, CMsgDefMgr::Instance()->GetErrMsg("EATBREAD_PROMPT", &vecFillMsg).c_str());
	}
}

VOID CNoticeMgr::OnActivityNotifyBossBattle(UINT8 byStartHour, UINT8 byStartMin)
{
	SPlayerBaseData *poPlayerBaseData = NULL;
	CPlayerID2PlayerBaseDataMapConstItr itr;
	const CPlayerID2PlayerBaseDataMap &  mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
	itr = mapPlayerID2PlayerBaseData.begin();
	for (itr; itr != mapPlayerID2PlayerBaseData.end(); itr++)
	{
		poPlayerBaseData = itr->second;
		if (NULL == poPlayerBaseData)
		{
			continue;
		}

		//检测玩家是否是机器人。
		if(ERFK_PLAYER != poPlayerBaseData->byRobotFunc)
		{
			continue;
		}

		if (0 == poPlayerBaseData->bBossBattleNotify)
		{
			continue;
		}

		//没有开启推送通知的，直接 放过。
		//if (0 == strlen((char*)poPlayerBaseData->abyToken))
		//{
		//	continue;
		//}

		CHAR szPayLoad[256] = {0};
		//sprintf(szPayLoad, "击杀BOSS战活动将于%u:%u开始", byStartHour, byStartMin);
		//((CGSApplication*)SDGetApp())->NoticeMsg(poPlayerBaseData->dwID, ENT_BOSSBATTLE, (CHAR*)poPlayerBaseData->abyToken, (CHAR*)SDGBK2UTF8(szPayLoad).c_str());
		vector<string> vecFillMsg;
		CHAR szBuff[32] = {0};
		sprintf(szBuff, "%u", byStartHour);
		vecFillMsg.push_back(szBuff);
		sprintf(szBuff, "%u", byStartMin);
		vecFillMsg.push_back(szBuff);
		gsapi::NoticeMsg(poPlayerBaseData->dwID, ENT_BOSSBATTLE, (CHAR*)poPlayerBaseData->abyToken, CMsgDefMgr::Instance()->GetErrMsg("BOSSB_PROMPT", &vecFillMsg).c_str());
	}
}

VOID CNoticeMgr::OnActivityNotifyCourage( UINT8 byStartHour, UINT8 byStartMin )
{
    SPlayerBaseData *poPlayerBaseData = NULL;
    CPlayerID2PlayerBaseDataMapConstItr itr;
    const CPlayerID2PlayerBaseDataMap &  mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
    itr = mapPlayerID2PlayerBaseData.begin();
    for (itr; itr != mapPlayerID2PlayerBaseData.end(); itr++)
    {
        poPlayerBaseData = itr->second;
        if (NULL == poPlayerBaseData)
        {
            continue;
        }

        //检测玩家是否是机器人。
        if(ERFK_PLAYER != poPlayerBaseData->byRobotFunc)
        {
            continue;
        }

        if (0 == poPlayerBaseData->bCoruageNotify)
        {
            continue;
        }

        //没有开启推送通知的，直接 放过。
        //if (0 == strlen((char*)poPlayerBaseData->abyToken))
        //{
        //	continue;
        //}

        CHAR szPayLoad[256] = {0};
        //sprintf(szPayLoad, "吃包子活动将于%u:%u开始", byStartHour, byStartMin);
        //((CGSApplication*)SDGetApp())->NoticeMsg(poPlayerBaseData->dwID, ENT_EATBREAD, (CHAR*)poPlayerBaseData->abyToken, (CHAR*)SDGBK2UTF8(szPayLoad).c_str());
        vector<string> vecFillMsg;
        CHAR szBuff[32] = {0};
        sprintf(szBuff, "%u", byStartHour);
        vecFillMsg.push_back(szBuff);
        sprintf(szBuff, "%u", byStartMin);
        vecFillMsg.push_back(szBuff);
        gsapi::NoticeMsg(poPlayerBaseData->dwID, ENT_COURAGETTLE, (CHAR*)poPlayerBaseData->abyToken, CMsgDefMgr::Instance()->GetErrMsg("COURAGE_PROMPT", &vecFillMsg).c_str());
    }
}
