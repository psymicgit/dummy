#include "protoclizs.h"
CProtoCliZS::CProtoCliZS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoCliZS * CProtoCliZS::Instance()
{
   static CProtoCliZS oInst;
   return &oInst;
}

CProtoCliZS::~CProtoCliZS()
{
}

INT32 CProtoCliZS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 2);

	EnFuncCProtoCliZS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoCliZS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 2);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoCliZS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoCliZS CProtoCliZS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLIZS_MSGID_OFFSET >= PROTOCLIZS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOCLIZS_MSGID_OFFSET];
}

DeFuncCProtoCliZS CProtoCliZS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLIZS_MSGID_OFFSET >= PROTOCLIZS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOCLIZS_MSGID_OFFSET];
}

BOOL CProtoCliZS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLIZS_ENDEC_KEY_NTF - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_ENDEC_KEY_NTF;
	m_DecodeFuncArray[CLIZS_ENDEC_KEY_NTF - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_ENDEC_KEY_NTF;
	m_vecMsgList.push_back(CLIZS_ENDEC_KEY_NTF);
	if( (INT32)sizeof(PKT_CLIZS_ENDEC_KEY_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_ENDEC_KEY_NTF);

	m_EncodeFuncArray[CLIZS_GET_ZONE_INFO_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_ZONE_INFO_REQ;
	m_DecodeFuncArray[CLIZS_GET_ZONE_INFO_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_ZONE_INFO_REQ;
	m_vecMsgList.push_back(CLIZS_GET_ZONE_INFO_REQ);
	if( (INT32)sizeof(PKT_CLIZS_GET_ZONE_INFO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_ZONE_INFO_REQ);

	m_EncodeFuncArray[CLIZS_GET_ZONE_INFO_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_ZONE_INFO_ACK;
	m_DecodeFuncArray[CLIZS_GET_ZONE_INFO_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_ZONE_INFO_ACK;
	m_vecMsgList.push_back(CLIZS_GET_ZONE_INFO_ACK);
	if( (INT32)sizeof(PKT_CLIZS_GET_ZONE_INFO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_ZONE_INFO_ACK);

	m_EncodeFuncArray[CLIGT_SELECT_ZONE_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_SELECT_ZONE_REQ;
	m_DecodeFuncArray[CLIGT_SELECT_ZONE_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_SELECT_ZONE_REQ;
	m_vecMsgList.push_back(CLIGT_SELECT_ZONE_REQ);
	if( (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_REQ);

	m_EncodeFuncArray[CLIGT_SELECT_ZONE_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_SELECT_ZONE_ACK;
	m_DecodeFuncArray[CLIGT_SELECT_ZONE_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_SELECT_ZONE_ACK;
	m_vecMsgList.push_back(CLIGT_SELECT_ZONE_ACK);
	if( (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_ACK);

	m_EncodeFuncArray[CLIGT_FIND_ZONE_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_FIND_ZONE_REQ;
	m_DecodeFuncArray[CLIGT_FIND_ZONE_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_FIND_ZONE_REQ;
	m_vecMsgList.push_back(CLIGT_FIND_ZONE_REQ);
	if( (INT32)sizeof(PKT_CLIGT_FIND_ZONE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_FIND_ZONE_REQ);

	m_EncodeFuncArray[CLIGT_FIND_ZONE_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_FIND_ZONE_ACK;
	m_DecodeFuncArray[CLIGT_FIND_ZONE_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_FIND_ZONE_ACK;
	m_vecMsgList.push_back(CLIGT_FIND_ZONE_ACK);
	if( (INT32)sizeof(PKT_CLIGT_FIND_ZONE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_FIND_ZONE_ACK);

	m_EncodeFuncArray[CLIZS_GET_VERSION_NOTICE_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_VERSION_NOTICE_REQ;
	m_DecodeFuncArray[CLIZS_GET_VERSION_NOTICE_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_VERSION_NOTICE_REQ;
	m_vecMsgList.push_back(CLIZS_GET_VERSION_NOTICE_REQ);
	if( (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_REQ);

	m_EncodeFuncArray[CLIZS_GET_VERSION_NOTICE_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_VERSION_NOTICE_ACK;
	m_DecodeFuncArray[CLIZS_GET_VERSION_NOTICE_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_VERSION_NOTICE_ACK;
	m_vecMsgList.push_back(CLIZS_GET_VERSION_NOTICE_ACK);
	if( (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_ACK);

	m_EncodeFuncArray[CLIZS_GET_USERINFO_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_USERINFO_REQ;
	m_DecodeFuncArray[CLIZS_GET_USERINFO_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_USERINFO_REQ;
	m_vecMsgList.push_back(CLIZS_GET_USERINFO_REQ);
	if( (INT32)sizeof(PKT_CLIZS_GET_USERINFO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_USERINFO_REQ);

	m_EncodeFuncArray[CLIZS_GET_USERINFO_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_USERINFO_ACK;
	m_DecodeFuncArray[CLIZS_GET_USERINFO_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_USERINFO_ACK;
	m_vecMsgList.push_back(CLIZS_GET_USERINFO_ACK);
	if( (INT32)sizeof(PKT_CLIZS_GET_USERINFO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_USERINFO_ACK);

	m_EncodeFuncArray[CLIZS_REGISTER_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_REGISTER_REQ;
	m_DecodeFuncArray[CLIZS_REGISTER_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_REGISTER_REQ;
	m_vecMsgList.push_back(CLIZS_REGISTER_REQ);
	if( (INT32)sizeof(PKT_CLIZS_REGISTER_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_REGISTER_REQ);

	m_EncodeFuncArray[CLIZS_REGISTER_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_REGISTER_ACK;
	m_DecodeFuncArray[CLIZS_REGISTER_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_REGISTER_ACK;
	m_vecMsgList.push_back(CLIZS_REGISTER_ACK);
	if( (INT32)sizeof(PKT_CLIZS_REGISTER_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_REGISTER_ACK);

	m_EncodeFuncArray[CLIGT_SELECT_ZONE_EX_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_SELECT_ZONE_EX_REQ;
	m_DecodeFuncArray[CLIGT_SELECT_ZONE_EX_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_SELECT_ZONE_EX_REQ;
	m_vecMsgList.push_back(CLIGT_SELECT_ZONE_EX_REQ);
	if( (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX_REQ);

	m_EncodeFuncArray[CLIGT_SELECT_ZONE_EX_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_SELECT_ZONE_EX_ACK;
	m_DecodeFuncArray[CLIGT_SELECT_ZONE_EX_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_SELECT_ZONE_EX_ACK;
	m_vecMsgList.push_back(CLIGT_SELECT_ZONE_EX_ACK);
	if( (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX_ACK);

	m_EncodeFuncArray[CLIZS_REPORT_USER_DEVICE_INFO_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ;
	m_DecodeFuncArray[CLIZS_REPORT_USER_DEVICE_INFO_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ;
	m_vecMsgList.push_back(CLIZS_REPORT_USER_DEVICE_INFO_REQ);
	if( (INT32)sizeof(PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ);

	m_EncodeFuncArray[CLIZS_REPORT_USER_DEVICE_INFO_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK;
	m_DecodeFuncArray[CLIZS_REPORT_USER_DEVICE_INFO_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK;
	m_vecMsgList.push_back(CLIZS_REPORT_USER_DEVICE_INFO_ACK);
	if( (INT32)sizeof(PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK);

	m_EncodeFuncArray[CLIZS_UN_BIND_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_UN_BIND_REQ;
	m_DecodeFuncArray[CLIZS_UN_BIND_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_UN_BIND_REQ;
	m_vecMsgList.push_back(CLIZS_UN_BIND_REQ);
	if( (INT32)sizeof(PKT_CLIZS_UN_BIND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_UN_BIND_REQ);

	m_EncodeFuncArray[CLIZS_UN_BIND_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_UN_BIND_ACK;
	m_DecodeFuncArray[CLIZS_UN_BIND_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_UN_BIND_ACK;
	m_vecMsgList.push_back(CLIZS_UN_BIND_ACK);
	if( (INT32)sizeof(PKT_CLIZS_UN_BIND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_UN_BIND_ACK);

	m_EncodeFuncArray[CLIGT_SELECT_ZONE_EX2_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_SELECT_ZONE_EX2_REQ;
	m_DecodeFuncArray[CLIGT_SELECT_ZONE_EX2_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_SELECT_ZONE_EX2_REQ;
	m_vecMsgList.push_back(CLIGT_SELECT_ZONE_EX2_REQ);
	if( (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX2_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX2_REQ);

	m_EncodeFuncArray[CLIGT_SELECT_ZONE_EX2_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIGT_SELECT_ZONE_EX2_ACK;
	m_DecodeFuncArray[CLIGT_SELECT_ZONE_EX2_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIGT_SELECT_ZONE_EX2_ACK;
	m_vecMsgList.push_back(CLIGT_SELECT_ZONE_EX2_ACK);
	if( (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK);

	m_EncodeFuncArray[CLIZS_GET_VERSION_NOTICE_REQ2 - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_VERSION_NOTICE_REQ2;
	m_DecodeFuncArray[CLIZS_GET_VERSION_NOTICE_REQ2 - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_VERSION_NOTICE_REQ2;
	m_vecMsgList.push_back(CLIZS_GET_VERSION_NOTICE_REQ2);
	if( (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_REQ2) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_REQ2);

	m_EncodeFuncArray[CLIZS_GET_VERSION_NOTICE_ACK2 - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_GET_VERSION_NOTICE_ACK2;
	m_DecodeFuncArray[CLIZS_GET_VERSION_NOTICE_ACK2 - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_GET_VERSION_NOTICE_ACK2;
	m_vecMsgList.push_back(CLIZS_GET_VERSION_NOTICE_ACK2);
	if( (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_ACK2) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_GET_VERSION_NOTICE_ACK2);

	m_EncodeFuncArray[CLIZS_RETRIEVE_PASSWD_MOBILE_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ;
	m_DecodeFuncArray[CLIZS_RETRIEVE_PASSWD_MOBILE_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ;
	m_vecMsgList.push_back(CLIZS_RETRIEVE_PASSWD_MOBILE_REQ);
	if( (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ);

	m_EncodeFuncArray[CLIZS_RETRIEVE_PASSWD_MOBILE_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK;
	m_DecodeFuncArray[CLIZS_RETRIEVE_PASSWD_MOBILE_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK;
	m_vecMsgList.push_back(CLIZS_RETRIEVE_PASSWD_MOBILE_ACK);
	if( (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK);

	m_EncodeFuncArray[CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ;
	m_DecodeFuncArray[CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ;
	m_vecMsgList.push_back(CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ);
	if( (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ);

	m_EncodeFuncArray[CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK;
	m_DecodeFuncArray[CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK;
	m_vecMsgList.push_back(CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK);
	if( (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK);

	m_EncodeFuncArray[CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ;
	m_DecodeFuncArray[CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ;
	m_vecMsgList.push_back(CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ);
	if( (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ);

	m_EncodeFuncArray[CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK - PROTOCLIZS_MSGID_OFFSET] = &EncodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK;
	m_DecodeFuncArray[CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK - PROTOCLIZS_MSGID_OFFSET] = &DecodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK;
	m_vecMsgList.push_back(CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK);
	if( (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK);

	return true;
}

INT32 EncodePKT_CLIZS_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_ENDEC_KEY_NTF *pstIn = (PKT_CLIZS_ENDEC_KEY_NTF*)pHost;

	INT32 i;
	for(i = 0; i < ENDEC_KEY_LEN; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyKeyInfo[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_ENDEC_KEY_NTF *pstOut = (PKT_CLIZS_ENDEC_KEY_NTF*)pHost;

	INT32 i;
	for(i = 0; i < ENDEC_KEY_LEN; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyKeyInfo[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_ZONE_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_ZONE_INFO_REQ *pstIn = (PKT_CLIZS_GET_ZONE_INFO_REQ*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_ZONE_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_ZONE_INFO_REQ *pstOut = (PKT_CLIZS_GET_ZONE_INFO_REQ*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_ZONE_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_ZONE_INFO_ACK *pstIn = (PKT_CLIZS_GET_ZONE_INFO_ACK*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDomainLen))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DOMAIN_NAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byDomainLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyDomainName[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wZonePort))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_ZONE_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_ZONE_INFO_ACK *pstOut = (PKT_CLIZS_GET_ZONE_INFO_ACK*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDomainLen))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DOMAIN_NAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byDomainLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyDomainName[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wZonePort))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ZONE_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_ZONE_PLAYER_DATA *pstIn = (DT_ZONE_PLAYER_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPlayerCareer))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPlayerLevel))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszPlayerDspName, sizeof(pstIn->aszPlayerDspName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ZONE_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_ZONE_PLAYER_DATA *pstOut = (DT_ZONE_PLAYER_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPlayerCareer))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPlayerLevel))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszPlayerDspName, sizeof(pstOut->aszPlayerDspName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ZONE_DATA(void *pHost, CNetData* poNetData)
{
	DT_ZONE_DATA *pstIn = (DT_ZONE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDomainLen))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DOMAIN_NAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byDomainLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyDomainName[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wZonePort))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszZoneName, sizeof(pstIn->aszZoneName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBusyFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPlayerNum))
		return -1;

	for(i = 0; i < MAX_PLAYER_NUM_PER_ZONE; i++)
	{
		if(i >= (INT32)pstIn->byPlayerNum)
			break;
		if(-1 == EncodeDT_ZONE_PLAYER_DATA(&pstIn->astPlayerInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ZONE_DATA(void *pHost, CNetData* poNetData)
{
	DT_ZONE_DATA *pstOut = (DT_ZONE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDomainLen))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DOMAIN_NAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byDomainLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyDomainName[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wZonePort))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszZoneName, sizeof(pstOut->aszZoneName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBusyFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPlayerNum))
		return -1;

	for(i = 0; i < MAX_PLAYER_NUM_PER_ZONE; i++)
	{
		if(i >= (INT32)pstOut->byPlayerNum)
			break;
		if(-1 == DecodeDT_ZONE_PLAYER_DATA(&pstOut->astPlayerInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_SELECT_ZONE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_REQ *pstIn = (PKT_CLIGT_SELECT_ZONE_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
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

	if(-1 == poNetData->AddWord(pstIn->wZoneIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_SELECT_ZONE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_REQ *pstOut = (PKT_CLIGT_SELECT_ZONE_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
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

	if(-1 == poNetData->DelWord(pstOut->wZoneIdx))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_SELECT_ZONE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_ACK *pstIn = (PKT_CLIGT_SELECT_ZONE_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byZoneNum)
			break;
		if(-1 == EncodeDT_ZONE_DATA(&pstIn->astZoneList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_SELECT_ZONE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_ACK *pstOut = (PKT_CLIGT_SELECT_ZONE_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byZoneNum)
			break;
		if(-1 == DecodeDT_ZONE_DATA(&pstOut->astZoneList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_SELECT_ZONE_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX_REQ *pstIn = (PKT_CLIGT_SELECT_ZONE_EX_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
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

	if(-1 == poNetData->AddWord(pstIn->wZoneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNickName, sizeof(pstIn->aszNickName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_SELECT_ZONE_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX_REQ *pstOut = (PKT_CLIGT_SELECT_ZONE_EX_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
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

	if(-1 == poNetData->DelWord(pstOut->wZoneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNickName, sizeof(pstOut->aszNickName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_SELECT_ZONE_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX_ACK *pstIn = (PKT_CLIGT_SELECT_ZONE_EX_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byZoneNum)
			break;
		if(-1 == EncodeDT_ZONE_DATA(&pstIn->astZoneList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_SELECT_ZONE_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX_ACK *pstOut = (PKT_CLIGT_SELECT_ZONE_EX_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byZoneNum)
			break;
		if(-1 == DecodeDT_ZONE_DATA(&pstOut->astZoneList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_SELECT_ZONE_EX2_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX2_REQ *pstIn = (PKT_CLIGT_SELECT_ZONE_EX2_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
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

	if(-1 == poNetData->AddWord(pstIn->wZoneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNickName, sizeof(pstIn->aszNickName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBindType))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_SELECT_ZONE_EX2_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX2_REQ *pstOut = (PKT_CLIGT_SELECT_ZONE_EX2_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
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

	if(-1 == poNetData->DelWord(pstOut->wZoneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNickName, sizeof(pstOut->aszNickName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBindType))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_SELECT_ZONE_EX2_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstIn = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byZoneNum)
			break;
		if(-1 == EncodeDT_ZONE_DATA(&pstIn->astZoneList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_SELECT_ZONE_EX2_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstOut = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byZoneNum)
			break;
		if(-1 == DecodeDT_ZONE_DATA(&pstOut->astZoneList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_FIND_ZONE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_FIND_ZONE_REQ *pstIn = (PKT_CLIGT_FIND_ZONE_REQ*)pHost;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserPwd, sizeof(pstIn->aszUserPwd)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszZoneName, sizeof(pstIn->aszZoneName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_FIND_ZONE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_FIND_ZONE_REQ *pstOut = (PKT_CLIGT_FIND_ZONE_REQ*)pHost;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserPwd, sizeof(pstOut->aszUserPwd)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszZoneName, sizeof(pstOut->aszZoneName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIGT_FIND_ZONE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_FIND_ZONE_ACK *pstIn = (PKT_CLIGT_FIND_ZONE_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byZoneNum)
			break;
		if(-1 == EncodeDT_ZONE_DATA(&pstIn->astZoneList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIGT_FIND_ZONE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIGT_FIND_ZONE_ACK *pstOut = (PKT_CLIGT_FIND_ZONE_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byZoneNum))
		return -1;

	INT32 i;
	for(i = 0; i < GET_ZONE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byZoneNum)
			break;
		if(-1 == DecodeDT_ZONE_DATA(&pstOut->astZoneList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_REQ *pstIn = (PKT_CLIZS_GET_VERSION_NOTICE_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliVersion))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_REQ *pstOut = (PKT_CLIZS_GET_VERSION_NOTICE_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliVersion))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_ACK *pstIn = (PKT_CLIZS_GET_VERSION_NOTICE_ACK*)pHost;

	if(-1 == EncodeDT_VERSION_DATA(&pstIn->stVersionInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_ACK *pstOut = (PKT_CLIZS_GET_VERSION_NOTICE_ACK*)pHost;

	if(-1 == DecodeDT_VERSION_DATA(&pstOut->stVersionInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_USERINFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_USERINFO_REQ *pstIn = (PKT_CLIZS_GET_USERINFO_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_USERINFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_USERINFO_REQ *pstOut = (PKT_CLIZS_GET_USERINFO_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_USERINFO(void *pHost, CNetData* poNetData)
{
	DT_USERINFO *pstIn = (DT_USERINFO*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNickName, sizeof(pstIn->aszNickName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_USERINFO(void *pHost, CNetData* poNetData)
{
	DT_USERINFO *pstOut = (DT_USERINFO*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNickName, sizeof(pstOut->aszNickName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_USERINFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_USERINFO_ACK *pstIn = (PKT_CLIZS_GET_USERINFO_ACK*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_USER_INFO_NUM; i++)
	{
		if(i >= (INT32)pstIn->byNum)
			break;
		if(-1 == EncodeDT_USERINFO(&pstIn->astUserInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_USERINFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_USERINFO_ACK *pstOut = (PKT_CLIZS_GET_USERINFO_ACK*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_USER_INFO_NUM; i++)
	{
		if(i >= (INT32)pstOut->byNum)
			break;
		if(-1 == DecodeDT_USERINFO(&pstOut->astUserInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_REGISTER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REGISTER_REQ *pstIn = (PKT_CLIZS_REGISTER_REQ*)pHost;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserPwd, sizeof(pstIn->aszUserPwd)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_REGISTER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REGISTER_REQ *pstOut = (PKT_CLIZS_REGISTER_REQ*)pHost;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserPwd, sizeof(pstOut->aszUserPwd)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_REGISTER_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REGISTER_ACK *pstIn = (PKT_CLIZS_REGISTER_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_REGISTER_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REGISTER_ACK *pstOut = (PKT_CLIZS_REGISTER_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ *pstIn = (PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ*)pHost;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszPlatform, sizeof(pstIn->aszPlatform)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszGameVersion, sizeof(pstIn->aszGameVersion)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszOSType, sizeof(pstIn->aszOSType)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszOSVersion, sizeof(pstIn->aszOSVersion)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszHardwareVersion, sizeof(pstIn->aszHardwareVersion)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszExtInfo, sizeof(pstIn->aszExtInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ *pstOut = (PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ*)pHost;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszPlatform, sizeof(pstOut->aszPlatform)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszGameVersion, sizeof(pstOut->aszGameVersion)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszOSType, sizeof(pstOut->aszOSType)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszOSVersion, sizeof(pstOut->aszOSVersion)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszHardwareVersion, sizeof(pstOut->aszHardwareVersion)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszExtInfo, sizeof(pstOut->aszExtInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK *pstIn = (PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK *pstOut = (PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_UN_BIND_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_UN_BIND_REQ *pstIn = (PKT_CLIZS_UN_BIND_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_UN_BIND_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_UN_BIND_REQ *pstOut = (PKT_CLIZS_UN_BIND_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_UN_BIND_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_UN_BIND_ACK *pstIn = (PKT_CLIZS_UN_BIND_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_UN_BIND_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_UN_BIND_ACK *pstOut = (PKT_CLIZS_UN_BIND_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_REQ2(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_REQ2 *pstIn = (PKT_CLIZS_GET_VERSION_NOTICE_REQ2*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliVersion))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_REQ2(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_REQ2 *pstOut = (PKT_CLIZS_GET_VERSION_NOTICE_REQ2*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliVersion))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_VERSION_DATA2(void *pHost, CNetData* poNetData)
{
	DT_VERSION_DATA2 *pstIn = (DT_VERSION_DATA2*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwVersionID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byForceUpdateFlag))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwResVer))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUIVer))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUpdateContent, sizeof(pstIn->aszUpdateContent)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUpdateURL, sizeof(pstIn->aszUpdateURL)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_VERSION_DATA2(void *pHost, CNetData* poNetData)
{
	DT_VERSION_DATA2 *pstOut = (DT_VERSION_DATA2*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwVersionID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byForceUpdateFlag))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwResVer))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUIVer))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUpdateContent, sizeof(pstOut->aszUpdateContent)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUpdateURL, sizeof(pstOut->aszUpdateURL)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_ACK2(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_ACK2 *pstIn = (PKT_CLIZS_GET_VERSION_NOTICE_ACK2*)pHost;

	if(-1 == EncodeDT_VERSION_DATA2(&pstIn->stVersionInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_ACK2(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_GET_VERSION_NOTICE_ACK2 *pstOut = (PKT_CLIZS_GET_VERSION_NOTICE_ACK2*)pHost;

	if(-1 == DecodeDT_VERSION_DATA2(&pstOut->stVersionInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ *pstIn = (PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyMObile[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ *pstOut = (PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyMObile[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK *pstIn = (PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RETRIEVE_PASSWD_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyToken[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK *pstOut = (PKT_CLIZS_RETRIEVE_PASSWD_MOBILE_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RETRIEVE_PASSWD_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyToken[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ *pstIn = (PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_RETRIEVE_PASSWD_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyToken[i]))
			return -1;
	}

	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyVerCode[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ *pstOut = (PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_RETRIEVE_PASSWD_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyToken[i]))
			return -1;
	}

	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyVerCode[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK *pstIn = (PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyVerCode[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wCountdown))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK *pstOut = (PKT_CLIZS_RETRIEVE_PASSWD_VER_CODE_ACK*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyVerCode[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wCountdown))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ *pstIn = (PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_RETRIEVE_PASSWD_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyToken[i]))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyNewPassWd[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ *pstOut = (PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_RETRIEVE_PASSWD_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyToken[i]))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyNewPassWd[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK *pstIn = (PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK *pstOut = (PKT_CLIZS_RETRIEVE_PASSWD_NEWPASSWD_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

