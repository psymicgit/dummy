/********************************************************************
	@file:   dealframepower.h
	@author: yewenhui
	@date:   2014年5月6日 15点56分
	@brief:  后台获取战斗力排行榜

	Copyright (c) 2014 heihuo. All rights reserved.
*********************************************************************/
#pragma once
#include "gmdealframe.h"
#include "protocol/server/protocommondata.h"


class DealFramePower
	: public InterfaceDealBase::CGmDealFrame
{
	enum {
		eServerRankNum		= 300,		// 全服排行的数量
		eZoneRankNum		= 100,		// 全区排行的数量
	};

	struct PlayerPowerRankData {
		UINT32   unPlayerID;					//玩家ID
		UINT32   unPower;						//玩家战斗力
		TCHAR    szPlayerName[USERNAME_LEN];		//玩家昵称

		PlayerPowerRankData() : unPlayerID(0), unPower(0) {
			memset(szPlayerName, 0, sizeof(szPlayerName));
		}
	};

	typedef vector<PlayerPowerRankData> VecPowerRankData;

	class SortPlayerPowerRank
	{
	public:
		BOOL operator()(const PlayerPowerRankData& p1, const PlayerPowerRankData& p2) {
			return p1.unPower > p2.unPower;
		}
	};

public:
	DealFramePower(InterfaceDealBase::CGmManager* pGmManager);
	~DealFramePower();

	virtual void		Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc);   //处理协议数据
	virtual void		Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField);
	virtual void		Interface(CRCClient* poClinet);
	virtual UINT16		GetFrameType();          //获取该协议类返回的协议类型号
	virtual string		GetStrFrameType();          //获取该协议类返回的协议类型号
	virtual string		GetStrFrameDesc();
	virtual void		Release();               //释放对象，提供外部
	virtual bool		init(InterfaceDealBase::CGmManager * IN_pManager);//初始化协议对象本身

};
