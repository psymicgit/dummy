///<------------------------------------------------------------------------------
//< @file:   gvg\local\gvgfactionmgr.h
//< @author: Kang_zhiquan
//< @date:   2014Äê5ÔÂ4ÈÕ 11:30:51
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgfactionmgr_h_
#define _gvgfactionmgr_h_

#include <map>
#include <sdsingleton.h>
#include <protocligs.h>

using namespace std;
typedef map<UINT32, DT_GVG_FACTION> CMapGvGFaction;
typedef CMapGvGFaction::iterator CMapGvGFactionItr;

class CGvGFactionMgr
{
	DECLARE_SINGLETON(CGvGFactionMgr)

public:
	CGvGFactionMgr()
	{
		Clear();
	}

public:
	BOOL Init();
	BOOL LoadFromDB();
	BOOL GetFactionGambleInfo(UINT32 dwFactionID, DT_FACTION_GAMBLE_INFO& stInfo);
	BOOL GetFactionInfo(UINT32 dwFactionID, DT_GVG_FACTION& stFaction);

    DT_GVG_FACTION* FindFactionInfo(UINT32 dwFactionID);

    void AddGvGFaction(DT_GVG_FACTION& stFaction);

    void Clear(){ m_mapGvGFaction.clear(); }

private:
	CMapGvGFaction m_mapGvGFaction;
};
#endif //_gvgfactionmgr_h_