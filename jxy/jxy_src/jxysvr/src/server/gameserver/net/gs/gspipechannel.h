#ifndef _GS_PIPECHANNEL_H
#define _GT_PIPECHANNEL_H

#include <sdframework.h>

class CGSPipeChannel:public CSDPipeChannel
{
public:
    CGSPipeChannel() {};
    ~CGSPipeChannel() {};
protected:        
	virtual VOID    OnEstablish();
private:
};


#endif


