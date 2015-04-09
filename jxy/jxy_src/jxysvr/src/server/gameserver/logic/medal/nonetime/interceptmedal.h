///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\interceptmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ27ÈÕ 17:41:39
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _interceptmedal_h_
#define _interceptmedal_h_
#include "logic/medal/medal.h"
class CInterceptMedal : public CNoneTimeMedal
{
public:
	BOOL CheckActivate();
};
#endif //_interceptmedal_h_/