#include "protogtls.h"
CProtoGTLS::CProtoGTLS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGTLS * CProtoGTLS::Instance()
{
   static CProtoGTLS oInst;
   return &oInst;
}

CProtoGTLS::~CProtoGTLS()
{
}

INT32 CProtoGTLS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGTLS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGTLS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGTLS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGTLS CProtoGTLS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGTLS_MSGID_OFFSET >= PROTOGTLS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGTLS_MSGID_OFFSET];
}

DeFuncCProtoGTLS CProtoGTLS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGTLS_MSGID_OFFSET >= PROTOGTLS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGTLS_MSGID_OFFSET];
}

BOOL CProtoGTLS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GTLS_LOGIN_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GTLS_LOGIN_REQ;
	m_DecodeFuncArray[GTLS_LOGIN_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GTLS_LOGIN_REQ;
	m_vecMsgList.push_back(GTLS_LOGIN_REQ);
	if( (INT32)sizeof(PKT_GTLS_LOGIN_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTLS_LOGIN_REQ);

	m_EncodeFuncArray[GTLS_LOGIN_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GTLS_LOGIN_ACK;
	m_DecodeFuncArray[GTLS_LOGIN_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GTLS_LOGIN_ACK;
	m_vecMsgList.push_back(GTLS_LOGIN_ACK);
	if( (INT32)sizeof(PKT_GTLS_LOGIN_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTLS_LOGIN_ACK);

	m_EncodeFuncArray[GTLS_LOGIN_EX_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GTLS_LOGIN_EX_REQ;
	m_DecodeFuncArray[GTLS_LOGIN_EX_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GTLS_LOGIN_EX_REQ;
	m_vecMsgList.push_back(GTLS_LOGIN_EX_REQ);
	if( (INT32)sizeof(PKT_GTLS_LOGIN_EX_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTLS_LOGIN_EX_REQ);

	m_EncodeFuncArray[GTLS_LOGIN_EX_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GTLS_LOGIN_EX_ACK;
	m_DecodeFuncArray[GTLS_LOGIN_EX_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GTLS_LOGIN_EX_ACK;
	m_vecMsgList.push_back(GTLS_LOGIN_EX_ACK);
	if( (INT32)sizeof(PKT_GTLS_LOGIN_EX_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTLS_LOGIN_EX_ACK);

	m_EncodeFuncArray[GTLS_CHANGE_PWD_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GTLS_CHANGE_PWD_REQ;
	m_DecodeFuncArray[GTLS_CHANGE_PWD_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GTLS_CHANGE_PWD_REQ;
	m_vecMsgList.push_back(GTLS_CHANGE_PWD_REQ);
	if( (INT32)sizeof(PKT_GTLS_CHANGE_PWD_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTLS_CHANGE_PWD_REQ);

	m_EncodeFuncArray[GTCOM_CLIENT_DOWN_RPT - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GTCOM_CLIENT_DOWN_RPT;
	m_DecodeFuncArray[GTCOM_CLIENT_DOWN_RPT - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GTCOM_CLIENT_DOWN_RPT;
	m_vecMsgList.push_back(GTCOM_CLIENT_DOWN_RPT);
	if( (INT32)sizeof(PKT_GTCOM_CLIENT_DOWN_RPT) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTCOM_CLIENT_DOWN_RPT);

	m_EncodeFuncArray[CLILS_LOGIN_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_REQ;
	m_DecodeFuncArray[CLILS_LOGIN_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_REQ;
	m_vecMsgList.push_back(CLILS_LOGIN_REQ);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_REQ);

	m_EncodeFuncArray[CLILS_LOGIN_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_ACK;
	m_DecodeFuncArray[CLILS_LOGIN_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_ACK;
	m_vecMsgList.push_back(CLILS_LOGIN_ACK);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_ACK);

	m_EncodeFuncArray[CLILS_REGISTER_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_REGISTER_REQ;
	m_DecodeFuncArray[CLILS_REGISTER_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_REGISTER_REQ;
	m_vecMsgList.push_back(CLILS_REGISTER_REQ);
	if( (INT32)sizeof(PKT_CLILS_REGISTER_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_REGISTER_REQ);

	m_EncodeFuncArray[CLILS_REGISTER_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_REGISTER_ACK;
	m_DecodeFuncArray[CLILS_REGISTER_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_REGISTER_ACK;
	m_vecMsgList.push_back(CLILS_REGISTER_ACK);
	if( (INT32)sizeof(PKT_CLILS_REGISTER_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_REGISTER_ACK);

	m_EncodeFuncArray[CLILS_BIND_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_REQ;
	m_DecodeFuncArray[CLILS_BIND_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_REQ;
	m_vecMsgList.push_back(CLILS_BIND_REQ);
	if( (INT32)sizeof(PKT_CLILS_BIND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_REQ);

	m_EncodeFuncArray[CLILS_BIND_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_ACK;
	m_DecodeFuncArray[CLILS_BIND_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_ACK;
	m_vecMsgList.push_back(CLILS_BIND_ACK);
	if( (INT32)sizeof(PKT_CLILS_BIND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_ACK);

	m_EncodeFuncArray[CLILS_UNBIND_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_REQ;
	m_DecodeFuncArray[CLILS_UNBIND_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_REQ;
	m_vecMsgList.push_back(CLILS_UNBIND_REQ);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_REQ);

	m_EncodeFuncArray[CLILS_UNBIND_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_ACK;
	m_DecodeFuncArray[CLILS_UNBIND_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_ACK;
	m_vecMsgList.push_back(CLILS_UNBIND_ACK);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_ACK);

	m_EncodeFuncArray[CLILS_LOGIN_EX_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_EX_REQ;
	m_DecodeFuncArray[CLILS_LOGIN_EX_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_EX_REQ;
	m_vecMsgList.push_back(CLILS_LOGIN_EX_REQ);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_EX_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_EX_REQ);

	m_EncodeFuncArray[CLILS_LOGIN_EX_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_EX_ACK;
	m_DecodeFuncArray[CLILS_LOGIN_EX_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_EX_ACK;
	m_vecMsgList.push_back(CLILS_LOGIN_EX_ACK);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_EX_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_EX_ACK);

	m_EncodeFuncArray[CLILS_LOGOUT_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGOUT_REQ;
	m_DecodeFuncArray[CLILS_LOGOUT_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGOUT_REQ;
	m_vecMsgList.push_back(CLILS_LOGOUT_REQ);
	if( (INT32)sizeof(PKT_CLILS_LOGOUT_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGOUT_REQ);

	m_EncodeFuncArray[CLILS_LOGOUT_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGOUT_ACK;
	m_DecodeFuncArray[CLILS_LOGOUT_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGOUT_ACK;
	m_vecMsgList.push_back(CLILS_LOGOUT_ACK);
	if( (INT32)sizeof(PKT_CLILS_LOGOUT_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGOUT_ACK);

	m_EncodeFuncArray[CLILS_CHANGE_PWD_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_CHANGE_PWD_REQ;
	m_DecodeFuncArray[CLILS_CHANGE_PWD_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_CHANGE_PWD_REQ;
	m_vecMsgList.push_back(CLILS_CHANGE_PWD_REQ);
	if( (INT32)sizeof(PKT_CLILS_CHANGE_PWD_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_CHANGE_PWD_REQ);

	m_EncodeFuncArray[CLILS_CHANGE_PWD_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_CHANGE_PWD_ACK;
	m_DecodeFuncArray[CLILS_CHANGE_PWD_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_CHANGE_PWD_ACK;
	m_vecMsgList.push_back(CLILS_CHANGE_PWD_ACK);
	if( (INT32)sizeof(PKT_CLILS_CHANGE_PWD_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_CHANGE_PWD_ACK);

	m_EncodeFuncArray[CLILS_BIND_MOBILE_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_MOBILE_REQ;
	m_DecodeFuncArray[CLILS_BIND_MOBILE_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_MOBILE_REQ;
	m_vecMsgList.push_back(CLILS_BIND_MOBILE_REQ);
	if( (INT32)sizeof(PKT_CLILS_BIND_MOBILE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_MOBILE_REQ);

	m_EncodeFuncArray[CLILS_BIND_MOBILE_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_MOBILE_ACK;
	m_DecodeFuncArray[CLILS_BIND_MOBILE_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_MOBILE_ACK;
	m_vecMsgList.push_back(CLILS_BIND_MOBILE_ACK);
	if( (INT32)sizeof(PKT_CLILS_BIND_MOBILE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_MOBILE_ACK);

	m_EncodeFuncArray[CLILS_BIND_CHECK_VER_CODE_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_CHECK_VER_CODE_REQ;
	m_DecodeFuncArray[CLILS_BIND_CHECK_VER_CODE_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_CHECK_VER_CODE_REQ;
	m_vecMsgList.push_back(CLILS_BIND_CHECK_VER_CODE_REQ);
	if( (INT32)sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_REQ);

	m_EncodeFuncArray[CLILS_BIND_CHECK_VER_CODE_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_CHECK_VER_CODE_ACK;
	m_DecodeFuncArray[CLILS_BIND_CHECK_VER_CODE_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_CHECK_VER_CODE_ACK;
	m_vecMsgList.push_back(CLILS_BIND_CHECK_VER_CODE_ACK);
	if( (INT32)sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_ACK);

	m_EncodeFuncArray[CLILS_UNBIND_MOBILE_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_MOBILE_REQ;
	m_DecodeFuncArray[CLILS_UNBIND_MOBILE_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_MOBILE_REQ;
	m_vecMsgList.push_back(CLILS_UNBIND_MOBILE_REQ);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_MOBILE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_MOBILE_REQ);

	m_EncodeFuncArray[CLILS_UNBIND_MOBILE_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_MOBILE_ACK;
	m_DecodeFuncArray[CLILS_UNBIND_MOBILE_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_MOBILE_ACK;
	m_vecMsgList.push_back(CLILS_UNBIND_MOBILE_ACK);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_MOBILE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_MOBILE_ACK);

	m_EncodeFuncArray[CLILS_UNBIND_CHECK_VER_CODE_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_CHECK_VER_CODE_REQ;
	m_DecodeFuncArray[CLILS_UNBIND_CHECK_VER_CODE_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_CHECK_VER_CODE_REQ;
	m_vecMsgList.push_back(CLILS_UNBIND_CHECK_VER_CODE_REQ);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_CHECK_VER_CODE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_CHECK_VER_CODE_REQ);

	m_EncodeFuncArray[CLILS_UNBIND_CHECK_VER_CODE_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_CHECK_VER_CODE_ACK;
	m_DecodeFuncArray[CLILS_UNBIND_CHECK_VER_CODE_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_CHECK_VER_CODE_ACK;
	m_vecMsgList.push_back(CLILS_UNBIND_CHECK_VER_CODE_ACK);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK);

	m_EncodeFuncArray[CLILS_GET_USER_EXPAND_REQ - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_GET_USER_EXPAND_REQ;
	m_DecodeFuncArray[CLILS_GET_USER_EXPAND_REQ - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_GET_USER_EXPAND_REQ;
	m_vecMsgList.push_back(CLILS_GET_USER_EXPAND_REQ);
	if( (INT32)sizeof(PKT_CLILS_GET_USER_EXPAND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_GET_USER_EXPAND_REQ);

	m_EncodeFuncArray[CLILS_GET_USER_EXPAND_ACK - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_CLILS_GET_USER_EXPAND_ACK;
	m_DecodeFuncArray[CLILS_GET_USER_EXPAND_ACK - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_CLILS_GET_USER_EXPAND_ACK;
	m_vecMsgList.push_back(CLILS_GET_USER_EXPAND_ACK);
	if( (INT32)sizeof(PKT_CLILS_GET_USER_EXPAND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_GET_USER_EXPAND_ACK);

	m_EncodeFuncArray[GSLS_PLAYER_UPGRADE_NTF - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GSLS_PLAYER_UPGRADE_NTF;
	m_DecodeFuncArray[GSLS_PLAYER_UPGRADE_NTF - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GSLS_PLAYER_UPGRADE_NTF;
	m_vecMsgList.push_back(GSLS_PLAYER_UPGRADE_NTF);
	if( (INT32)sizeof(PKT_GSLS_PLAYER_UPGRADE_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSLS_PLAYER_UPGRADE_NTF);

	m_EncodeFuncArray[GSLS_DSPNAME_UPGRADE_NTF - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GSLS_DSPNAME_UPGRADE_NTF;
	m_DecodeFuncArray[GSLS_DSPNAME_UPGRADE_NTF - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GSLS_DSPNAME_UPGRADE_NTF;
	m_vecMsgList.push_back(GSLS_DSPNAME_UPGRADE_NTF);
	if( (INT32)sizeof(PKT_GSLS_DSPNAME_UPGRADE_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSLS_DSPNAME_UPGRADE_NTF);

	m_EncodeFuncArray[GSLS_ACTIVITY_NOTIFY_NTF - PROTOGTLS_MSGID_OFFSET] = &EncodePKT_GSLS_ACTIVITY_NOTIFY_NTF;
	m_DecodeFuncArray[GSLS_ACTIVITY_NOTIFY_NTF - PROTOGTLS_MSGID_OFFSET] = &DecodePKT_GSLS_ACTIVITY_NOTIFY_NTF;
	m_vecMsgList.push_back(GSLS_ACTIVITY_NOTIFY_NTF);
	if( (INT32)sizeof(PKT_GSLS_ACTIVITY_NOTIFY_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSLS_ACTIVITY_NOTIFY_NTF);

	return true;
}

INT32 EncodePKT_GTLS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_REQ *pstIn = (PKT_GTLS_LOGIN_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliVersion))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == EncodeDT_EXT_FUNC_PARAM_LST(&pstIn->stExtFuncParamLst, poNetData))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszIP, sizeof(pstIn->aszIP)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GTLS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_REQ *pstOut = (PKT_GTLS_LOGIN_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliVersion))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == DecodeDT_EXT_FUNC_PARAM_LST(&pstOut->stExtFuncParamLst, poNetData))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszIP, sizeof(pstOut->aszIP)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GTLS_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_ACK *pstIn = (PKT_GTLS_LOGIN_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == EncodeDT_VERSION_DATA(&pstIn->stVersionInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo1, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo2, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszZoneName, sizeof(pstIn->aszZoneName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	if(-1 == EncodeDT_EXT_FUNC_PARAM_LST(&pstIn->stExtFuncParamLst, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GTLS_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_ACK *pstOut = (PKT_GTLS_LOGIN_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == DecodeDT_VERSION_DATA(&pstOut->stVersionInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo1, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo2, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszZoneName, sizeof(pstOut->aszZoneName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	if(-1 == DecodeDT_EXT_FUNC_PARAM_LST(&pstOut->stExtFuncParamLst, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GTLS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_EX_REQ *pstIn = (PKT_GTLS_LOGIN_EX_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliVersion))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszIP, sizeof(pstIn->aszIP)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GTLS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_EX_REQ *pstOut = (PKT_GTLS_LOGIN_EX_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliVersion))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszIP, sizeof(pstOut->aszIP)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GTLS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_EX_ACK *pstIn = (PKT_GTLS_LOGIN_EX_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == EncodeDT_VERSION_DATA(&pstIn->stVersionInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo1, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo2, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszZoneName, sizeof(pstIn->aszZoneName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GTLS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_LOGIN_EX_ACK *pstOut = (PKT_GTLS_LOGIN_EX_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == DecodeDT_VERSION_DATA(&pstOut->stVersionInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo1, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo2, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszZoneName, sizeof(pstOut->aszZoneName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GTLS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_CHANGE_PWD_REQ *pstIn = (PKT_GTLS_CHANGE_PWD_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byNewPwdLen))
		return -1;

	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byNewPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyNewUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GTLS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GTLS_CHANGE_PWD_REQ *pstOut = (PKT_GTLS_CHANGE_PWD_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byNewPwdLen))
		return -1;

	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byNewPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyNewUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	return sizeof(*pstOut);
}

