
#include "clipktbuilder.h"
#include "common/client/errdef.h"

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
    memcpy(pstNtf->abyKeyInfo, abyKeyInfo, ENDEC_KEY_LEN);
    return CLIZS_ENDEC_KEY_NTF;
}


UINT16 CCliPktBuilder::OnResetPasswdVerCodeReq(	UINT8 abyToken[MAX_RESET_PASSWD_TOKEN_NUM], UINT8 abyVerCode[MAX_MOBILE_LENGTH])
{
    ZeroPacketBuffer(sizeof(PKT_CLIZS_RESET_PASSWD_VER_CODE_ACK));
    PKT_CLIZS_RESET_PASSWD_VER_CODE_ACK* pstAck = (PKT_CLIZS_RESET_PASSWD_VER_CODE_ACK*)(GetPacketBuffer());
    return CLIZS_RESET_PASSWD_VER_CODE_ACK;
}
