#include "protoksgs.h"
#include <sstream>

CProtoKSGS::CProtoKSGS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoKSGS * CProtoKSGS::Instance()
{
   static CProtoKSGS oInst;
   return &oInst;
}

CProtoKSGS::~CProtoKSGS()
{
}

INT32 CProtoKSGS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoKSGS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoKSGS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoKSGS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoKSGS CProtoKSGS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOKSGS_MSGID_OFFSET >= PROTOKSGS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOKSGS_MSGID_OFFSET];
}

DeFuncCProtoKSGS CProtoKSGS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOKSGS_MSGID_OFFSET >= PROTOKSGS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOKSGS_MSGID_OFFSET];
}

BOOL CProtoKSGS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[KSGS_ADD_PLAYER_INFO_REQ - PROTOKSGS_MSGID_OFFSET] = &EncodePKT_KSGS_ADD_PLAYER_INFO_REQ;
	m_DecodeFuncArray[KSGS_ADD_PLAYER_INFO_REQ - PROTOKSGS_MSGID_OFFSET] = &DecodePKT_KSGS_ADD_PLAYER_INFO_REQ;
	m_vecMsgList.push_back(KSGS_ADD_PLAYER_INFO_REQ);
	if( (INT32)sizeof(PKT_KSGS_ADD_PLAYER_INFO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_KSGS_ADD_PLAYER_INFO_REQ);

	m_EncodeFuncArray[KSGS_ADD_PLAYER_INFO_ACK - PROTOKSGS_MSGID_OFFSET] = &EncodePKT_KSGS_ADD_PLAYER_INFO_ACK;
	m_DecodeFuncArray[KSGS_ADD_PLAYER_INFO_ACK - PROTOKSGS_MSGID_OFFSET] = &DecodePKT_KSGS_ADD_PLAYER_INFO_ACK;
	m_vecMsgList.push_back(KSGS_ADD_PLAYER_INFO_ACK);
	if( (INT32)sizeof(PKT_KSGS_ADD_PLAYER_INFO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_KSGS_ADD_PLAYER_INFO_ACK);

	return true;
}

INT32 EncodePKT_KSGS_ADD_PLAYER_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_KSGS_ADD_PLAYER_INFO_REQ *pstIn = (PKT_KSGS_ADD_PLAYER_INFO_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAction))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam1))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwParam2))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszParam, sizeof(pstIn->aszParam)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_KSGS_ADD_PLAYER_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_KSGS_ADD_PLAYER_INFO_REQ *pstOut = (PKT_KSGS_ADD_PLAYER_INFO_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAction))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam1))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwParam2))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszParam, sizeof(pstOut->aszParam)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_KSGS_ADD_PLAYER_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_KSGS_ADD_PLAYER_INFO_ACK *pstIn = (PKT_KSGS_ADD_PLAYER_INFO_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_KSGS_ADD_PLAYER_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_KSGS_ADD_PLAYER_INFO_ACK *pstOut = (PKT_KSGS_ADD_PLAYER_INFO_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	return sizeof(*pstOut);
}

