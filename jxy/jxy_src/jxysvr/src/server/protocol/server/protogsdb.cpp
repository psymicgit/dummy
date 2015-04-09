#include "protogsdb.h"
CProtoGSDB::CProtoGSDB()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGSDB * CProtoGSDB::Instance()
{
   static CProtoGSDB oInst;
   return &oInst;
}

CProtoGSDB::~CProtoGSDB()
{
}

INT32 CProtoGSDB::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGSDB pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGSDB::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGSDB pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGSDB CProtoGSDB::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSDB_MSGID_OFFSET >= PROTOGSDB_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGSDB_MSGID_OFFSET];
}

DeFuncCProtoGSDB CProtoGSDB::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSDB_MSGID_OFFSET >= PROTOGSDB_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGSDB_MSGID_OFFSET];
}

BOOL CProtoGSDB::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	return true;
}

INT32 EncodeDT_GSDB_GET_PALYER_DATA_REQ(void *pHost, CNetData* poNetData)
{
	DT_GSDB_GET_PALYER_DATA_REQ *pstIn = (DT_GSDB_GET_PALYER_DATA_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byGetKind))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GSDB_GET_PALYER_DATA_REQ(void *pHost, CNetData* poNetData)
{
	DT_GSDB_GET_PALYER_DATA_REQ *pstOut = (DT_GSDB_GET_PALYER_DATA_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byGetKind))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_GSDB_GET_PALYER_DATA_ACK(void *pHost, CNetData* poNetData)
{
	DT_GSDB_GET_PALYER_DATA_ACK *pstIn = (DT_GSDB_GET_PALYER_DATA_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGetKind))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == EncodeDT_PLAYER_DATA(&pstIn->stPlayerData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GSDB_GET_PALYER_DATA_ACK(void *pHost, CNetData* poNetData)
{
	DT_GSDB_GET_PALYER_DATA_ACK *pstOut = (DT_GSDB_GET_PALYER_DATA_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGetKind))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == DecodeDT_PLAYER_DATA(&pstOut->stPlayerData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_GSDB_CREATE_PALYER_DATA_REQ(void *pHost, CNetData* poNetData)
{
	DT_GSDB_CREATE_PALYER_DATA_REQ *pstIn = (DT_GSDB_CREATE_PALYER_DATA_REQ*)pHost;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGmFlag))
		return -1;

	if(-1 == EncodeDT_PLAYER_DATA(&pstIn->stPlayerData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GSDB_CREATE_PALYER_DATA_REQ(void *pHost, CNetData* poNetData)
{
	DT_GSDB_CREATE_PALYER_DATA_REQ *pstOut = (DT_GSDB_CREATE_PALYER_DATA_REQ*)pHost;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGmFlag))
		return -1;

	if(-1 == DecodeDT_PLAYER_DATA(&pstOut->stPlayerData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_GSDB_CREATE_PALYER_DATA_ACK(void *pHost, CNetData* poNetData)
{
	DT_GSDB_CREATE_PALYER_DATA_ACK *pstIn = (DT_GSDB_CREATE_PALYER_DATA_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGmFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GSDB_CREATE_PALYER_DATA_ACK(void *pHost, CNetData* poNetData)
{
	DT_GSDB_CREATE_PALYER_DATA_ACK *pstOut = (DT_GSDB_CREATE_PALYER_DATA_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGmFlag))
		return -1;

	return sizeof(*pstOut);
}

