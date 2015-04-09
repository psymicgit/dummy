#ifndef MSMQWARPPER_H
#define MSMQWARPPER_H

#include "config.h"  // for  SGDP_HAS_MSMQ_LOGGER
#ifdef SGDP_HAS_MSMQ_LOGGER 

#include <wtypes.h> 
#include <objidl.h> 
#include <winerror.h>
#include <mq.h>


class CMsmqWarpper
{
public:
	CMsmqWarpper();
	virtual ~CMsmqWarpper();
	
	bool Init(const char* szPathName, int sendtype = 0);
	void Release();

	bool SendMessage(const char* pdata, int len);

private:
	bool CreatePublicQueue(LPCWSTR wszPathName);
	bool OpenPublicQueue(LPCWSTR wszPathName, bool connecttype);
	void ClosePublicQueue();

private:
	HANDLE m_hQueue;
	int m_sendtype;
};

#endif // 

#endif

