#include "protoclitlc.h"
CProtoCliTlc::CProtoCliTlc()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoCliTlc * CProtoCliTlc::Instance()
{
   static CProtoCliTlc oInst;
   return &oInst;
}

CProtoCliTlc::~CProtoCliTlc()
{
}

INT32 CProtoCliTlc::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoCliTlc pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoCliTlc::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoCliTlc pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoCliTlc CProtoCliTlc::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLITLC_MSGID_OFFSET >= PROTOCLITLC_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOCLITLC_MSGID_OFFSET];
}

DeFuncCProtoCliTlc CProtoCliTlc::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLITLC_MSGID_OFFSET >= PROTOCLITLC_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOCLITLC_MSGID_OFFSET];
}

BOOL CProtoCliTlc::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLITLC_ENDEC_KEY_NTF - PROTOCLITLC_MSGID_OFFSET] = &EncodePKT_CLITLC_ENDEC_KEY_NTF;
	m_DecodeFuncArray[CLITLC_ENDEC_KEY_NTF - PROTOCLITLC_MSGID_OFFSET] = &DecodePKT_CLITLC_ENDEC_KEY_NTF;
	m_vecMsgList.push_back(CLITLC_ENDEC_KEY_NTF);
	if( (INT32)sizeof(PKT_CLITLC_ENDEC_KEY_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLITLC_ENDEC_KEY_NTF);

	m_EncodeFuncArray[CLITLC_CHECK_ORDERID_REQ - PROTOCLITLC_MSGID_OFFSET] = &EncodePKT_CLITLC_CHECK_ORDERID_REQ;
	m_DecodeFuncArray[CLITLC_CHECK_ORDERID_REQ - PROTOCLITLC_MSGID_OFFSET] = &DecodePKT_CLITLC_CHECK_ORDERID_REQ;
	m_vecMsgList.push_back(CLITLC_CHECK_ORDERID_REQ);
	if( (INT32)sizeof(PKT_CLITLC_CHECK_ORDERID_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLITLC_CHECK_ORDERID_REQ);

	m_EncodeFuncArray[CLITLC_CHECK_ORDERID_ACK - PROTOCLITLC_MSGID_OFFSET] = &EncodePKT_CLITLC_CHECK_ORDERID_ACK;
	m_DecodeFuncArray[CLITLC_CHECK_ORDERID_ACK - PROTOCLITLC_MSGID_OFFSET] = &DecodePKT_CLITLC_CHECK_ORDERID_ACK;
	m_vecMsgList.push_back(CLITLC_CHECK_ORDERID_ACK);
	if( (INT32)sizeof(PKT_CLITLC_CHECK_ORDERID_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLITLC_CHECK_ORDERID_ACK);

	m_EncodeFuncArray[CLITLC_GEN_ORDERID_REQ - PROTOCLITLC_MSGID_OFFSET] = &EncodePKT_CLITLC_GEN_ORDERID_REQ;
	m_DecodeFuncArray[CLITLC_GEN_ORDERID_REQ - PROTOCLITLC_MSGID_OFFSET] = &DecodePKT_CLITLC_GEN_ORDERID_REQ;
	m_vecMsgList.push_back(CLITLC_GEN_ORDERID_REQ);
	if( (INT32)sizeof(PKT_CLITLC_GEN_ORDERID_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLITLC_GEN_ORDERID_REQ);

	m_EncodeFuncArray[CLITLC_GEN_ORDERID_ACK - PROTOCLITLC_MSGID_OFFSET] = &EncodePKT_CLITLC_GEN_ORDERID_ACK;
	m_DecodeFuncArray[CLITLC_GEN_ORDERID_ACK - PROTOCLITLC_MSGID_OFFSET] = &DecodePKT_CLITLC_GEN_ORDERID_ACK;
	m_vecMsgList.push_back(CLITLC_GEN_ORDERID_ACK);
	if( (INT32)sizeof(PKT_CLITLC_GEN_ORDERID_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLITLC_GEN_ORDERID_ACK);

	return true;
}

INT32 EncodePKT_CLITLC_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_ENDEC_KEY_NTF *pstIn = (PKT_CLITLC_ENDEC_KEY_NTF*)pHost;

	INT32 i;
	for(i = 0; i < ENDEC_KEY_LEN; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyKeyInfo[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLITLC_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_ENDEC_KEY_NTF *pstOut = (PKT_CLITLC_ENDEC_KEY_NTF*)pHost;

	INT32 i;
	for(i = 0; i < ENDEC_KEY_LEN; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyKeyInfo[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLITLC_CHECK_ORDERID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_CHECK_ORDERID_REQ *pstIn = (PKT_CLITLC_CHECK_ORDERID_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szOrderInfo, sizeof(pstIn->szOrderInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLITLC_CHECK_ORDERID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_CHECK_ORDERID_REQ *pstOut = (PKT_CLITLC_CHECK_ORDERID_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szOrderInfo, sizeof(pstOut->szOrderInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLITLC_CHECK_ORDERID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_CHECK_ORDERID_ACK *pstIn = (PKT_CLITLC_CHECK_ORDERID_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szErrCode, sizeof(pstIn->szErrCode)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szOrderInfo, sizeof(pstIn->szOrderInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLITLC_CHECK_ORDERID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_CHECK_ORDERID_ACK *pstOut = (PKT_CLITLC_CHECK_ORDERID_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szErrCode, sizeof(pstOut->szErrCode)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szOrderInfo, sizeof(pstOut->szOrderInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLITLC_GEN_ORDERID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_GEN_ORDERID_REQ *pstIn = (PKT_CLITLC_GEN_ORDERID_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szFrom, sizeof(pstIn->szFrom)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPlayerLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLITLC_GEN_ORDERID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_GEN_ORDERID_REQ *pstOut = (PKT_CLITLC_GEN_ORDERID_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szFrom, sizeof(pstOut->szFrom)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPlayerLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLITLC_GEN_ORDERID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_GEN_ORDERID_ACK *pstIn = (PKT_CLITLC_GEN_ORDERID_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szErrCode, sizeof(pstIn->szErrCode)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szFrom, sizeof(pstIn->szFrom)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPlayerLevel))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszOrderID, sizeof(pstIn->aszOrderID)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLITLC_GEN_ORDERID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLITLC_GEN_ORDERID_ACK *pstOut = (PKT_CLITLC_GEN_ORDERID_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szErrCode, sizeof(pstOut->szErrCode)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szFrom, sizeof(pstOut->szFrom)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPlayerLevel))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszOrderID, sizeof(pstOut->aszOrderID)))
		return -1;

	return sizeof(*pstOut);
}

