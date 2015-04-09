#ifndef _GS_PIPECHANNEL_H
#define _GS_PIPECHANNEL_H

#include <sdframework.h>
#include <map>
using namespace std;


class CSvrPipeChannel:public CSDPipeChannel
{
public:
	VOID	SetZoneID(UINT16 wZoneID) { m_wZoneID = wZoneID;}
private:
    virtual VOID SDAPI   OnRecv(UINT16 wBusinessID, const CHAR *pData, UINT32 dwLen);

private:
	UINT16	m_wZoneID;
};

#endif


