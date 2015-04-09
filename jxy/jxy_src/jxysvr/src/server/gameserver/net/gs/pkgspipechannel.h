///<------------------------------------------------------------------------------
//< @file:   pkgspipechannel.h
//< @author: hongkunan
//< @date:   2014Äê1ÔÂ3ÈÕ 16:43:17
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pkgspipechannel_h_
#define _pkgspipechannel_h_

#include <sdframework.h>

class CPKGSPipeChannel : public CSDPipeChannel
{
public:
    CPKGSPipeChannel() {};
    ~CPKGSPipeChannel() {};
protected:        
    virtual VOID    OnEstablish();
private:
};

#endif //_pkgspipechannel_h_