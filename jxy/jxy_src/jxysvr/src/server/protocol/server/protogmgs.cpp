#include "protogmgs.h"
#include <sstream>

CProtoGMGS::CProtoGMGS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGMGS * CProtoGMGS::Instance()
{
   static CProtoGMGS oInst;
   return &oInst;
}

CProtoGMGS::~CProtoGMGS()
{
}

INT32 CProtoGMGS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGMGS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGMGS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGMGS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGMGS CProtoGMGS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGMGS_MSGID_OFFSET >= PROTOGMGS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGMGS_MSGID_OFFSET];
}

DeFuncCProtoGMGS CProtoGMGS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGMGS_MSGID_OFFSET >= PROTOGMGS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGMGS_MSGID_OFFSET];
}

BOOL CProtoGMGS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GMGS_PARM_INFO_REQ - PROTOGMGS_MSGID_OFFSET] = &EncodePKT_GMGS_PARM_INFO_REQ;
	m_DecodeFuncArray[GMGS_PARM_INFO_REQ - PROTOGMGS_MSGID_OFFSET] = &DecodePKT_GMGS_PARM_INFO_REQ;
	m_vecMsgList.push_back(GMGS_PARM_INFO_REQ);
	if( (INT32)sizeof(PKT_GMGS_PARM_INFO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GMGS_PARM_INFO_REQ);

	m_EncodeFuncArray[GMGS_PARM_INFO_ACK - PROTOGMGS_MSGID_OFFSET] = &EncodePKT_GMGS_PARM_INFO_ACK;
	m_DecodeFuncArray[GMGS_PARM_INFO_ACK - PROTOGMGS_MSGID_OFFSET] = &DecodePKT_GMGS_PARM_INFO_ACK;
	m_vecMsgList.push_back(GMGS_PARM_INFO_ACK);
	if( (INT32)sizeof(PKT_GMGS_PARM_INFO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GMGS_PARM_INFO_ACK);

	return true;
}

INT32 EncodePKT_GMGS_PARM_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_PARM_INFO_REQ *pstIn = (PKT_GMGS_PARM_INFO_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAction))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAreaID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam1))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwParam2))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwParam3))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszParam4, sizeof(pstIn->aszParam4)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GMGS_PARM_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_PARM_INFO_REQ *pstOut = (PKT_GMGS_PARM_INFO_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAction))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAreaID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam1))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwParam2))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwParam3))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszParam4, sizeof(pstOut->aszParam4)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GMGS_PARM_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_PARM_INFO_ACK *pstIn = (PKT_GMGS_PARM_INFO_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GMGS_PARM_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_PARM_INFO_ACK *pstOut = (PKT_GMGS_PARM_INFO_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	return sizeof(*pstOut);
}

