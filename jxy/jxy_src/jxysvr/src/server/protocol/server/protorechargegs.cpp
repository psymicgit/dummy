#include "protorechargegs.h"
#include <sstream>

CProtoRechargeGS::CProtoRechargeGS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoRechargeGS * CProtoRechargeGS::Instance()
{
   static CProtoRechargeGS oInst;
   return &oInst;
}

CProtoRechargeGS::~CProtoRechargeGS()
{
}

INT32 CProtoRechargeGS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoRechargeGS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoRechargeGS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoRechargeGS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoRechargeGS CProtoRechargeGS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTORECHARGEGS_MSGID_OFFSET >= PROTORECHARGEGS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTORECHARGEGS_MSGID_OFFSET];
}

DeFuncCProtoRechargeGS CProtoRechargeGS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTORECHARGEGS_MSGID_OFFSET >= PROTORECHARGEGS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTORECHARGEGS_MSGID_OFFSET];
}

BOOL CProtoRechargeGS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[RECGS_RECHARGE_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_RECHARGE_REQ;
	m_DecodeFuncArray[RECGS_RECHARGE_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_RECHARGE_REQ;
	m_vecMsgList.push_back(RECGS_RECHARGE_REQ);
	if( (INT32)sizeof(PKT_RECGS_RECHARGE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_RECHARGE_REQ);

	m_EncodeFuncArray[RECGS_RECHARGE_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_RECHARGE_ACK;
	m_DecodeFuncArray[RECGS_RECHARGE_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_RECHARGE_ACK;
	m_vecMsgList.push_back(RECGS_RECHARGE_ACK);
	if( (INT32)sizeof(PKT_RECGS_RECHARGE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_RECHARGE_ACK);

	m_EncodeFuncArray[RECGS_HEART_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_HEART_REQ;
	m_DecodeFuncArray[RECGS_HEART_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_HEART_REQ;
	m_vecMsgList.push_back(RECGS_HEART_REQ);
	if( (INT32)sizeof(PKT_RECGS_HEART_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_HEART_REQ);

	m_EncodeFuncArray[RECGS_HEART_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_HEART_ACK;
	m_DecodeFuncArray[RECGS_HEART_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_HEART_ACK;
	m_vecMsgList.push_back(RECGS_HEART_ACK);
	if( (INT32)sizeof(PKT_RECGS_HEART_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_HEART_ACK);

	m_EncodeFuncArray[RECGS_GEN_GUID_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_GEN_GUID_REQ;
	m_DecodeFuncArray[RECGS_GEN_GUID_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_GEN_GUID_REQ;
	m_vecMsgList.push_back(RECGS_GEN_GUID_REQ);
	if( (INT32)sizeof(PKT_RECGS_GEN_GUID_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_GEN_GUID_REQ);

	m_EncodeFuncArray[RECGS_GEN_GUID_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_GEN_GUID_ACK;
	m_DecodeFuncArray[RECGS_GEN_GUID_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_GEN_GUID_ACK;
	m_vecMsgList.push_back(RECGS_GEN_GUID_ACK);
	if( (INT32)sizeof(PKT_RECGS_GEN_GUID_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_GEN_GUID_ACK);

	m_EncodeFuncArray[RECGS_PROCESS_ORDER_ID_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_PROCESS_ORDER_ID_REQ;
	m_DecodeFuncArray[RECGS_PROCESS_ORDER_ID_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_PROCESS_ORDER_ID_REQ;
	m_vecMsgList.push_back(RECGS_PROCESS_ORDER_ID_REQ);
	if( (INT32)sizeof(PKT_RECGS_PROCESS_ORDER_ID_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_PROCESS_ORDER_ID_REQ);

	m_EncodeFuncArray[RECGS_PROCESS_ORDER_ID_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_PROCESS_ORDER_ID_ACK;
	m_DecodeFuncArray[RECGS_PROCESS_ORDER_ID_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_PROCESS_ORDER_ID_ACK;
	m_vecMsgList.push_back(RECGS_PROCESS_ORDER_ID_ACK);
	if( (INT32)sizeof(PKT_RECGS_PROCESS_ORDER_ID_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_PROCESS_ORDER_ID_ACK);

	m_EncodeFuncArray[RECGS_CHECK_APP_ORDER_ID_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_CHECK_APP_ORDER_ID_REQ;
	m_DecodeFuncArray[RECGS_CHECK_APP_ORDER_ID_REQ - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_CHECK_APP_ORDER_ID_REQ;
	m_vecMsgList.push_back(RECGS_CHECK_APP_ORDER_ID_REQ);
	if( (INT32)sizeof(PKT_RECGS_CHECK_APP_ORDER_ID_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_CHECK_APP_ORDER_ID_REQ);

	m_EncodeFuncArray[RECGS_CHECK_APP_ORDER_ID_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &EncodePKT_RECGS_CHECK_APP_ORDER_ID_ACK;
	m_DecodeFuncArray[RECGS_CHECK_APP_ORDER_ID_ACK - PROTORECHARGEGS_MSGID_OFFSET] = &DecodePKT_RECGS_CHECK_APP_ORDER_ID_ACK;
	m_vecMsgList.push_back(RECGS_CHECK_APP_ORDER_ID_ACK);
	if( (INT32)sizeof(PKT_RECGS_CHECK_APP_ORDER_ID_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_RECGS_CHECK_APP_ORDER_ID_ACK);

	return true;
}

INT32 EncodePKT_RECGS_RECHARGE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_RECHARGE_REQ *pstIn = (PKT_RECGS_RECHARGE_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szReqStr, sizeof(pstIn->szReqStr)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_RECHARGE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_RECHARGE_REQ *pstOut = (PKT_RECGS_RECHARGE_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szReqStr, sizeof(pstOut->szReqStr)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_RECHARGE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_RECHARGE_ACK *pstIn = (PKT_RECGS_RECHARGE_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAckStr, sizeof(pstIn->szAckStr)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_RECHARGE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_RECHARGE_ACK *pstOut = (PKT_RECGS_RECHARGE_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAckStr, sizeof(pstOut->szAckStr)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_HEART_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_HEART_REQ *pstIn = (PKT_RECGS_HEART_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_HEART_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_HEART_REQ *pstOut = (PKT_RECGS_HEART_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_HEART_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_HEART_ACK *pstIn = (PKT_RECGS_HEART_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_HEART_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_HEART_ACK *pstOut = (PKT_RECGS_HEART_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_GEN_GUID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_GEN_GUID_REQ *pstIn = (PKT_RECGS_GEN_GUID_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szInfo, sizeof(pstIn->szInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_GEN_GUID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_GEN_GUID_REQ *pstOut = (PKT_RECGS_GEN_GUID_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szInfo, sizeof(pstOut->szInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_GEN_GUID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_GEN_GUID_ACK *pstIn = (PKT_RECGS_GEN_GUID_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szInfo, sizeof(pstIn->szInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_GEN_GUID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_GEN_GUID_ACK *pstOut = (PKT_RECGS_GEN_GUID_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szInfo, sizeof(pstOut->szInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_PROCESS_ORDER_ID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_PROCESS_ORDER_ID_REQ *pstIn = (PKT_RECGS_PROCESS_ORDER_ID_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szInfo, sizeof(pstIn->szInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_PROCESS_ORDER_ID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_PROCESS_ORDER_ID_REQ *pstOut = (PKT_RECGS_PROCESS_ORDER_ID_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szInfo, sizeof(pstOut->szInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_PROCESS_ORDER_ID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_PROCESS_ORDER_ID_ACK *pstIn = (PKT_RECGS_PROCESS_ORDER_ID_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szInfo, sizeof(pstIn->szInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_PROCESS_ORDER_ID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_PROCESS_ORDER_ID_ACK *pstOut = (PKT_RECGS_PROCESS_ORDER_ID_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szInfo, sizeof(pstOut->szInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_CHECK_APP_ORDER_ID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_CHECK_APP_ORDER_ID_REQ *pstIn = (PKT_RECGS_CHECK_APP_ORDER_ID_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szInfo, sizeof(pstIn->szInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_CHECK_APP_ORDER_ID_REQ(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_CHECK_APP_ORDER_ID_REQ *pstOut = (PKT_RECGS_CHECK_APP_ORDER_ID_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szInfo, sizeof(pstOut->szInfo)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_RECGS_CHECK_APP_ORDER_ID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_CHECK_APP_ORDER_ID_ACK *pstIn = (PKT_RECGS_CHECK_APP_ORDER_ID_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szInfo, sizeof(pstIn->szInfo)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_RECGS_CHECK_APP_ORDER_ID_ACK(void *pHost, CNetData* poNetData)
{
	PKT_RECGS_CHECK_APP_ORDER_ID_ACK *pstOut = (PKT_RECGS_CHECK_APP_ORDER_ID_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szInfo, sizeof(pstOut->szInfo)))
		return -1;

	return sizeof(*pstOut);
}

