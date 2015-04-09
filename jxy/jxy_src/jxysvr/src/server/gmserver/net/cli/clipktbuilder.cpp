
#include "clipktbuilder.h"

CCliPktBuilder::CCliPktBuilder()
{
}

CCliPktBuilder::~CCliPktBuilder()
{
}


UINT16 CCliPktBuilder::EncryptDecryptKeyNtf(UINT8 abyKeyInfo[])
{
	ZeroPacketBuffer(sizeof(PKT_CLIZS_ENDEC_KEY_NTF));
	PKT_CLIZS_ENDEC_KEY_NTF* pstNtf = (PKT_CLIZS_ENDEC_KEY_NTF*)(GetPacketBuffer());
	memcpy(pstNtf->abyKeyInfo,abyKeyInfo, ENDEC_KEY_LEN);
	
	return CLIZS_ENDEC_KEY_NTF;
}
