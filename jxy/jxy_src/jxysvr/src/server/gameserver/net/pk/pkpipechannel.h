///<------------------------------------------------------------------------------
//< @file:   pkpipechannel.h
//< @author: hongkunan
//< @date:   2014年1月3日 13:52:8
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pkpipechannel_h_
#define _pkpipechannel_h_

#include <dll/sdframework/sdpipechannel.h>

// 游戏服务器持有的与跨服战服务器的连接
class CPKPipeChannel:public CSDPipeChannel
{
protected:
    ///<@impl>
    virtual void OnEstablish();
};

#endif //_pkpipechannel_h_