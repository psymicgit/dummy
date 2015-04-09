

//  Defines the entry point for the console application.
//  Ê±¼ä£º2010-12-27¡£
//

#ifndef _GT_PIPECHANNEL_H
#define _GT_PIPECHANNEL_H

#include <sdframework.h>

class CGTPipeChannel:public CSDPipeChannel
{
public:
    CGTPipeChannel() { m_bRefusePkg = FALSE; };
    ~CGTPipeChannel() {};

public:
	inline VOID SetRefusePkg(BOOL bRefusePkg ) { m_bRefusePkg = bRefusePkg; }
	inline BOOL RefusePkg() { return m_bRefusePkg; }
protected:        
    virtual VOID SDAPI  OnRecv(UINT16 wBusinessID, const CHAR *pData, UINT32 dwLen);
private:
	BOOL	m_bRefusePkg;
};


#endif


