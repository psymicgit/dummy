#include "protomsgm.h"
CProtoMSGM::CProtoMSGM()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoMSGM * CProtoMSGM::Instance()
{
   static CProtoMSGM oInst;
   return &oInst;
}

CProtoMSGM::~CProtoMSGM()
{
}

INT32 CProtoMSGM::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoMSGM pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoMSGM::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoMSGM pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoMSGM CProtoMSGM::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOMSGM_MSGID_OFFSET >= PROTOMSGM_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOMSGM_MSGID_OFFSET];
}

DeFuncCProtoMSGM CProtoMSGM::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOMSGM_MSGID_OFFSET >= PROTOMSGM_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOMSGM_MSGID_OFFSET];
}

BOOL CProtoMSGM::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GMGS_CONTROL_REQ - PROTOMSGM_MSGID_OFFSET] = &EncodePKT_GMGS_CONTROL_REQ;
	m_DecodeFuncArray[GMGS_CONTROL_REQ - PROTOMSGM_MSGID_OFFSET] = &DecodePKT_GMGS_CONTROL_REQ;
	m_vecMsgList.push_back(GMGS_CONTROL_REQ);
	if( (INT32)sizeof(PKT_GMGS_CONTROL_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GMGS_CONTROL_REQ);

	m_EncodeFuncArray[GMGS_CONTROL_ACK - PROTOMSGM_MSGID_OFFSET] = &EncodePKT_GMGS_CONTROL_ACK;
	m_DecodeFuncArray[GMGS_CONTROL_ACK - PROTOMSGM_MSGID_OFFSET] = &DecodePKT_GMGS_CONTROL_ACK;
	m_vecMsgList.push_back(GMGS_CONTROL_ACK);
	if( (INT32)sizeof(PKT_GMGS_CONTROL_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GMGS_CONTROL_ACK);

	m_EncodeFuncArray[GMGS_GET_SERVER_INFO_REQ - PROTOMSGM_MSGID_OFFSET] = &EncodePKT_GMGS_GET_SERVER_INFO_REQ;
	m_DecodeFuncArray[GMGS_GET_SERVER_INFO_REQ - PROTOMSGM_MSGID_OFFSET] = &DecodePKT_GMGS_GET_SERVER_INFO_REQ;
	m_vecMsgList.push_back(GMGS_GET_SERVER_INFO_REQ);
	if( (INT32)sizeof(PKT_GMGS_GET_SERVER_INFO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GMGS_GET_SERVER_INFO_REQ);

	m_EncodeFuncArray[GMGS_GET_SERVER_INFO_ACK - PROTOMSGM_MSGID_OFFSET] = &EncodePKT_GMGS_GET_SERVER_INFO_ACK;
	m_DecodeFuncArray[GMGS_GET_SERVER_INFO_ACK - PROTOMSGM_MSGID_OFFSET] = &DecodePKT_GMGS_GET_SERVER_INFO_ACK;
	m_vecMsgList.push_back(GMGS_GET_SERVER_INFO_ACK);
	if( (INT32)sizeof(PKT_GMGS_GET_SERVER_INFO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GMGS_GET_SERVER_INFO_ACK);

	return true;
}

INT32 EncodePKT_GMGS_CONTROL_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_CONTROL_REQ *pstIn = (PKT_GMGS_CONTROL_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAreaID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszServer, sizeof(pstIn->aszServer)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUseFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GMGS_CONTROL_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_CONTROL_REQ *pstOut = (PKT_GMGS_CONTROL_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAreaID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszServer, sizeof(pstOut->aszServer)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUseFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GMGS_CONTROL_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_CONTROL_ACK *pstIn = (PKT_GMGS_CONTROL_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GMGS_CONTROL_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_CONTROL_ACK *pstOut = (PKT_GMGS_CONTROL_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SERVER_DATA(void *pHost, CNetData* poNetData)
{
	DT_SERVER_DATA *pstIn = (DT_SERVER_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byKeepAlive))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszServer, sizeof(pstIn->aszServer)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SERVER_DATA(void *pHost, CNetData* poNetData)
{
	DT_SERVER_DATA *pstOut = (DT_SERVER_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byKeepAlive))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszServer, sizeof(pstOut->aszServer)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_GROUP_DATA(void *pHost, CNetData* poNetData)
{
	DT_GROUP_DATA *pstIn = (DT_GROUP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byUseFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < SERVER_COUNT; i++)
	{
		if(i >= (INT32)pstIn->byNum)
			break;
		if(-1 == EncodeDT_SERVER_DATA(&pstIn->astServerDataList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GROUP_DATA(void *pHost, CNetData* poNetData)
{
	DT_GROUP_DATA *pstOut = (DT_GROUP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byUseFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < SERVER_COUNT; i++)
	{
		if(i >= (INT32)pstOut->byNum)
			break;
		if(-1 == DecodeDT_SERVER_DATA(&pstOut->astServerDataList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GMGS_GET_SERVER_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_GET_SERVER_INFO_REQ *pstIn = (PKT_GMGS_GET_SERVER_INFO_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAreaID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GMGS_GET_SERVER_INFO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_GET_SERVER_INFO_REQ *pstOut = (PKT_GMGS_GET_SERVER_INFO_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAreaID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GMGS_GET_SERVER_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_GET_SERVER_INFO_ACK *pstIn = (PKT_GMGS_GET_SERVER_INFO_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAreaID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGroupNum))
		return -1;

	INT32 i;
	for(i = 0; i < GROUP_COUNT; i++)
	{
		if(i >= (INT32)pstIn->byGroupNum)
			break;
		if(-1 == EncodeDT_GROUP_DATA(&pstIn->astGroupDataList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GMGS_GET_SERVER_INFO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GMGS_GET_SERVER_INFO_ACK *pstOut = (PKT_GMGS_GET_SERVER_INFO_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAreaID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGroupNum))
		return -1;

	INT32 i;
	for(i = 0; i < GROUP_COUNT; i++)
	{
		if(i >= (INT32)pstOut->byGroupNum)
			break;
		if(-1 == DecodeDT_GROUP_DATA(&pstOut->astGroupDataList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

