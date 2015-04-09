///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerpowermedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:9:36
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playerpowermedal_h_
#define _playerpowermedal_h_
#include "logic/medal/medal.h"

class CPlayerPowerMedal : public CNoneTimeMedal
{
public: 
	virtual BOOL CheckActivate();
};
#endif //_playerpowermedal_h_