#ifndef  CONTROL_PROM_H123_
#define  CONTROL_PROM_H123_

#include <sdtype.h>
#include <sdhashmap.h>
#include <sdshmem.h>
#include <sdsingleton.h>
#include <keepalive/ctdef.h>
#include "msconfig.h"
#include "protocol/server/protomsgm.h"



using namespace SGDP;



class CMonitor
{
public:
     DECLARE_SINGLETON(CMonitor);
     BOOL Init();
     VOID UnInit();
	 BOOL OnEvent();
	 BOOL SetUseFlag(UINT32 dwAreaID, char *szName, UINT8	byState);
	 BOOL Quit();

	 void GET_SERVER_INFO_ACK(PKT_GMGS_GET_SERVER_INFO_ACK & stAck);


protected:
	CMonitor();
	~CMonitor();

private:

	BOOL Check(const UINT32 dwAreaID,SGroupInfo &stGroupInfo);
	BOOL Check(SProcessInfo &stProcessInfo);
	BOOL Start(SProcessInfo &stProcessInfo);
	void Kill(SProcessInfo &stProcessInfo);

	SProcessDaemon * GetProcessDaemon(SShmem	 & shmem, SProcessInfo &stProcessInfo);
	int watch_procps_scan(SProcessInfo &stProcessInfo);

private:
	void OnMail(const UINT32 dwAreaID, const SProcessInfo stProcessInfo);
};


#endif