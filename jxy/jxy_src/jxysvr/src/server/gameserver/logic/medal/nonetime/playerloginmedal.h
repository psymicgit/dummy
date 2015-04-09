///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerloginmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:21:56
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playerloginmedal_h_
#define _playerloginmedal_h_
#include "logic/medal/medal.h"

class CPlayerLoginMedal : public CNoneTimeMedal
{
public:
	virtual BOOL CheckActivate();
	virtual BOOL IsActivate();
};
#endif //_playerloginmedal_h_