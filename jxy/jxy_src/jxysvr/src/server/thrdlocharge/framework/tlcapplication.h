

#ifndef _TLCSERVER_APPLICATION_H_
#define _TLCSERVER_APPLICATION_H_

#include "sdframework.h"
#include <list>
#include <net/ls/lsprocessor.h>
#include "framework/tlcconfig.h"



using namespace std;
using namespace SGDP;


typedef map<UINT16, CSDPipeChannel*> CZoneID2PipeChannelMap;
typedef CZoneID2PipeChannelMap::iterator CZoneID2PipeChannelMapItr;

class CTLCApplication : public CSDApplication
{
public:
    CTLCApplication();
    virtual ~CTLCApplication();
public:
    virtual BOOL        Init(const TCHAR* pszConfigFilename);
    virtual VOID        UnInit();
    virtual BOOL        Run();

    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID        OnPipeChannelDisconnected(CSDPipeChannel* poChannel);

    virtual VOID		UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect);

	CSDPipeChannel*		GetLSPipeChannel(UINT16 wZoneID);

	////////////////////////////////

protected:

	
private:
	CZoneID2PipeChannelMap	m_mapLSPipeChannel;//所有的认证服务器
	CLSProcessor			m_oLSProcessor;
};


#endif


