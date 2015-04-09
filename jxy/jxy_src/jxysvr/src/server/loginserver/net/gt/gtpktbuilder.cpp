
#include "gtpktbuilder.h"

CGTPktBuilder::CGTPktBuilder()
{
}

CGTPktBuilder::~CGTPktBuilder()
{
}

/*
UINT16 CGTPktBuilder::LoginAck(PKT_CLILS_LOGIN_ACK& stAck)
{	
	ZeroPacketBuffer();
	PKT_CLILS_LOGIN_ACK* pstAck = (PKT_CLILS_LOGIN_ACK*)(GetPacketBuffer());

	memcpy(pstAck, &stAck, sizeof(PKT_CLILS_LOGIN_ACK));

	return CLILS_LOGIN_ACK;
}
*/

UINT16 CGTPktBuilder::RegisterAck(UINT32 dwErrCode, TCHAR* pszUserName)
{
	ZeroPacketBuffer(sizeof(PKT_CLILS_REGISTER_ACK));
	PKT_CLILS_REGISTER_ACK* pstAck = (PKT_CLILS_REGISTER_ACK*)(GetPacketBuffer());

	pstAck->wErrCode = dwErrCode;
	//只有成功才赋值，避免被外挂利用
	if(SUCCESS == dwErrCode)
	{
		_SDTStrcpy(pstAck->aszUserName, pszUserName);
	}

	return CLILS_REGISTER_ACK;
}

