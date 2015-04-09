///<------------------------------------------------------------------------------
//< @file:   protocaltool.h
//< @author: yewenhui
//< @date:   2014年6月13日 15点20分
//< @brief:  协议工具 主要是抽出常用协议同步代码
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#pragma once
#ifndef _PROTOCAL_TOOL_H_
#define _PROTOCAL_TOOL_H_
#include "sdtype.h"
#include "protocol/server/protocligs.h"

class CPlayer;

namespace ProtocalTool
{
	VOID		FillBagItemData(CPlayer* pPlayer, UINT32 unItemId, DT_BAG_ITEM_INFO& rBagItemInfo);
	VOID		FillPetModuleRes(CPlayer* pPlayer, DT_PLAYER_RES_LST_DATA& rResLstData);

}

#endif // _PROTOCAL_TOOL_H_
