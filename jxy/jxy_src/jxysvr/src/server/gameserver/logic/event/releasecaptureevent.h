
#ifndef _RELEASECAPTUREEVENT_H_ 
#define _RELEASECAPTUREEVENT_H_


#include "eventreactor.h"

using namespace SGDP;

class CReleaseCaptureEvent: public CEvent
{
public:
	CReleaseCaptureEvent();
	virtual ~CReleaseCaptureEvent();
	virtual UINT32 OnEvent();
	virtual BOOL OnDestroy();
public:
	VOID SetParam(UINT32 dwOwnerID, UINT32 dwCaptureID);
private:
	UINT32 m_dwOwnerID;
	UINT32 m_dwCaptureID;
	UINT64 m_qwCaptureTime;

};

#endif


