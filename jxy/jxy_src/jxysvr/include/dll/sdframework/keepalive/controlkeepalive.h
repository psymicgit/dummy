#ifndef  CONTROL_H_
#define  CONTROL_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <sdshmem.h>
#include <sdsingleton.h>
#include <sdtype.h>


#include "ctdef.h"

using namespace SGDP;



class CControlKeepAlive
{
public:
     DECLARE_SINGLETON(CControlKeepAlive);
     BOOL Init();
     VOID UnInit();
	 VOID OnAlive();
	 BOOL IsStop();
	 BOOL SyncStop();

protected:
	CControlKeepAlive();
	~CControlKeepAlive();

private:
	SShmem			m_oShmem;
	SProcessDaemon*	m_poProcessDaemon;
};


#endif