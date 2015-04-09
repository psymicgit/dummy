///<------------------------------------------------------------------------------
//< @file:   logic\gvg\playergvg.h
//< @author: kang_zhiquan
//< @date:   2014年4月25日 14:22:27
//< @brief:  玩家身上的跨服门派战对象，管理押注历史、已领取礼包等信息
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playergvg_h_
#define _playergvg_h_

#include <protocol/server/protocligs.h>

#include "logic/base/baseobj.h"
#include "logic/base/basedefine.h"
#include "common/client/commondef.h"

#include "common/server/utility.h"

class CPlayerGVG : public CBaseObj
{

	typedef std::map<UINT64, DT_GAMBLE_DATA> CMapGambleData;
	typedef CMapGambleData::iterator CMapGambleDataItr;
public:
	CPlayerGVG()
	{
		Clear();
	}
	~CPlayerGVG()
	{
		Clear();
	}

    void Clear()
    {
        memset(&m_stGVGData, 0, sizeof(DT_PLAYER_GVG_DATA));
        m_mapGambleData.clear();
    }

	BOOL Init(DT_PLAYER_GVG_DATA& stData, CPlayer* poOwner);
	DT_PLAYER_GVG_DATA& GetPlayerGVGData();

	VOID ResetGambleInfo();

	VOID GetGambleReward();

	UINT32 GetGambleFactionID(DT_GVG_ROUND&);

	//返回值：TRUE 为 已押注， FALSE 为未押注
	BOOL IsGamble(DT_GVG_ROUND&);
public:
	UINT16 OpenMyGamble(PKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK& stAck);
	UINT16 GambleFactionAck(PKT_CLIGS_GVG_GAMBLE_REQ& stReq);
	UINT16 GetGvGFormationPlayerTip(PKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK& stAck);
	UINT16 OpenMyGvGift(PKT_CLIGS_OPEN_GVG_GIFT_ACK& stAck);
	UINT16 GetMyGift( PKT_CLIGS_GET_GVG_GIFT_REQ& stReq, PKT_CLIGS_GET_GVG_GIFT_ACK& stAck);
	UINT8 GetUnGetGiftNum();

    // 获取未读本派战报条数
    UINT16 GetUnReadLogCnt();

    // 设置玩家的战况阅读情况为：已阅读过全部战况
    void ClearUnReadLogCnt();

private:
	BOOL GetIdxByGambleRound(DT_GVG_ROUND& eRound, UINT8& byIdx );
	//添加押注信息
	BOOL AddGambleInfo(DT_GVG_ROUND& eGvGGamble, DT_GAMBLE_DATA& stData);
	//获取押注信息
	BOOL GetGambleFactionInfo(UINT64 qwGambleID, GvGGambleResult eResult, UINT32 dwSuportFactionID, DT_GAMBLE_GAME_INFO& stInfo);
	//领取一个礼包
	UINT16 GetSingleGift(UINT64 qwKey);
	//领取全部礼包
	UINT16 GetAllGift();

private:
	DT_PLAYER_GVG_DATA m_stGVGData; // 玩家身上的门派跨服战实例数据
	C64Set m_setGiftKey;	//礼包信息
	CMapGambleData m_mapGambleData;	//押注信息
	
};
DECLARE_CLASS_POOL_MGR(CPlayerGVG)

#endif //_playergvg_h_