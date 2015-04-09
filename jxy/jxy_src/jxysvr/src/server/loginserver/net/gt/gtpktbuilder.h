

#ifndef _GT_PACKAGE_BUILDER_H_
#define _GT_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include "protogtls.h"

class CGTPktBuilder: public CSDPacketBuilder
{
public:
	static CGTPktBuilder* Instance() { static CGTPktBuilder oGTPkgBuilder; return &oGTPkgBuilder;}

protected:
	CGTPktBuilder();        
	~CGTPktBuilder();
public: 

	//UINT16 LoginAck(PKT_CLILS_LOGIN_ACK& stAck);

	UINT16 RegisterAck(UINT32 dwErrCode, TCHAR* pszUserName);
};

#endif 

