///<------------------------------------------------------------------------------
//< @file:   medal\limittime\gvgmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ30ÈÕ 10:33:1
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgmedal_h_
#define _gvgmedal_h_

#include "logic/medal/medal.h"

class CGvGMedal : public CTimeMedal
{
public:
	virtual BOOL IsOverTime();
	virtual BOOL CheckActivate();
	virtual BOOL SetActivateState(BOOL bIsActivate, UINT64 qwActivateTime = SGDP::SDTimeSecs());
};
#endif //_gvgmedal_h_