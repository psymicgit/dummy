///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\awakenmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ26ÈÕ 21:27:42
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _awakenmedal_h_
#define _awakenmedal_h_
#include "logic/medal/medal.h"

class CAwakenMedal : public CNoneTimeMedal
{
public:
	virtual BOOL CheckActivate();
};
#endif //_awakenmedal_h_