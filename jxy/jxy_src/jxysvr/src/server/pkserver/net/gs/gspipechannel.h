///<------------------------------------------------------------------------------
//< @file:   gspipechannel.h
//< @author: hongkunan
//< @date:   2014年1月2日 20:18:55
//< @brief:  跨服战服务器与gameserver的连接
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gspipechannel_h_
#define _gspipechannel_h_

#include <sdframework.h>

// 与gameserver的连接
class CGSPipeChannel : public CSDPipeChannel
{
    //@impl
    virtual VOID OnEstablish();
};

#endif //_gspipechannel_h_