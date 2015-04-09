///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerlevelmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:5:48
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playerlevelmedal_h_
#define _playerlevelmedal_h_
#include "logic/medal/medal.h"

class CPlayerLevelMedal : public CNoneTimeMedal
{
public:
	virtual BOOL CheckActivate();
};
#endif //_playerlevelmedal_h_