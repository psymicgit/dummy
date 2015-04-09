///<------------------------------------------------------------------------------
//< @file:   gsprocessor.h
//< @author: hongkunan
//< @date:   2014年1月2日 21:9:41
//< @brief:  gameserver消息处理器，负责处理来自gameserver发过来的消息包
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gsprocessor_h_
#define _gsprocessor_h_

#include "sdframework.h"
#include "protocol/server/protogtgs.h"

using namespace std;

// gameserver消息处理器，来自gameserver的消息将交由本处理器负责
class CGSProcessor :public CSDPacketProcessor
{
public:
    static CGSProcessor* Instance()
    {
        static CGSProcessor oProcessor;
        return &oProcessor;
    }

    ///<@impl>
    virtual BOOL Init();

    ///<@impl>
    virtual CSDProtocol* GetProtocol();
};

#endif //_gsprocessor_h_