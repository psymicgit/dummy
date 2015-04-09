#include "protocliks.h"
CProtoCliKS::CProtoCliKS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoCliKS * CProtoCliKS::Instance()
{
   static CProtoCliKS oInst;
   return &oInst;
}

CProtoCliKS::~CProtoCliKS()
{
}

INT32 CProtoCliKS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoCliKS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoCliKS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoCliKS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoCliKS CProtoCliKS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLIKS_MSGID_OFFSET >= PROTOCLIKS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOCLIKS_MSGID_OFFSET];
}

DeFuncCProtoCliKS CProtoCliKS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLIKS_MSGID_OFFSET >= PROTOCLIKS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOCLIKS_MSGID_OFFSET];
}

BOOL CProtoCliKS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLIKS_LOGIN_REQ - PROTOCLIKS_MSGID_OFFSET] = &EncodePKT_CLIKS_LOGIN_REQ;
	m_DecodeFuncArray[CLIKS_LOGIN_REQ - PROTOCLIKS_MSGID_OFFSET] = &DecodePKT_CLIKS_LOGIN_REQ;
	m_vecMsgList.push_back(CLIKS_LOGIN_REQ);
	if( (INT32)sizeof(PKT_CLIKS_LOGIN_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIKS_LOGIN_REQ);

	m_EncodeFuncArray[CLIKS_LOGIN_ACK - PROTOCLIKS_MSGID_OFFSET] = &EncodePKT_CLIKS_LOGIN_ACK;
	m_DecodeFuncArray[CLIKS_LOGIN_ACK - PROTOCLIKS_MSGID_OFFSET] = &DecodePKT_CLIKS_LOGIN_ACK;
	m_vecMsgList.push_back(CLIKS_LOGIN_ACK);
	if( (INT32)sizeof(PKT_CLIKS_LOGIN_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLIKS_LOGIN_ACK);

	return true;
}

INT32 EncodePKT_CLIKS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIKS_LOGIN_REQ *pstIn = (PKT_CLIKS_LOGIN_REQ*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIKS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLIKS_LOGIN_REQ *pstOut = (PKT_CLIKS_LOGIN_REQ*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLIKS_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIKS_LOGIN_ACK *pstIn = (PKT_CLIKS_LOGIN_ACK*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwZoneIP))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZonePort))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLIKS_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLIKS_LOGIN_ACK *pstOut = (PKT_CLIKS_LOGIN_ACK*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwZoneIP))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZonePort))
		return -1;

	return sizeof(*pstOut);
}

