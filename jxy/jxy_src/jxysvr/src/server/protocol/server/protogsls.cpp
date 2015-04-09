#include "protogsls.h"
CProtoGSLS::CProtoGSLS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGSLS * CProtoGSLS::Instance()
{
   static CProtoGSLS oInst;
   return &oInst;
}

CProtoGSLS::~CProtoGSLS()
{
}

INT32 CProtoGSLS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGSLS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGSLS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGSLS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGSLS CProtoGSLS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSLS_MSGID_OFFSET >= PROTOGSLS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGSLS_MSGID_OFFSET];
}

DeFuncCProtoGSLS CProtoGSLS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSLS_MSGID_OFFSET >= PROTOGSLS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGSLS_MSGID_OFFSET];
}

BOOL CProtoGSLS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GSLS_PLAYER_UPGRADE_NTF - PROTOGSLS_MSGID_OFFSET] = &EncodePKT_GSLS_PLAYER_UPGRADE_NTF;
	m_DecodeFuncArray[GSLS_PLAYER_UPGRADE_NTF - PROTOGSLS_MSGID_OFFSET] = &DecodePKT_GSLS_PLAYER_UPGRADE_NTF;
	m_vecMsgList.push_back(GSLS_PLAYER_UPGRADE_NTF);
	if( (INT32)sizeof(PKT_GSLS_PLAYER_UPGRADE_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSLS_PLAYER_UPGRADE_NTF);

	m_EncodeFuncArray[GSLS_DSPNAME_UPGRADE_NTF - PROTOGSLS_MSGID_OFFSET] = &EncodePKT_GSLS_DSPNAME_UPGRADE_NTF;
	m_DecodeFuncArray[GSLS_DSPNAME_UPGRADE_NTF - PROTOGSLS_MSGID_OFFSET] = &DecodePKT_GSLS_DSPNAME_UPGRADE_NTF;
	m_vecMsgList.push_back(GSLS_DSPNAME_UPGRADE_NTF);
	if( (INT32)sizeof(PKT_GSLS_DSPNAME_UPGRADE_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSLS_DSPNAME_UPGRADE_NTF);

	m_EncodeFuncArray[GSLS_ACTIVITY_NOTIFY_NTF - PROTOGSLS_MSGID_OFFSET] = &EncodePKT_GSLS_ACTIVITY_NOTIFY_NTF;
	m_DecodeFuncArray[GSLS_ACTIVITY_NOTIFY_NTF - PROTOGSLS_MSGID_OFFSET] = &DecodePKT_GSLS_ACTIVITY_NOTIFY_NTF;
	m_vecMsgList.push_back(GSLS_ACTIVITY_NOTIFY_NTF);
	if( (INT32)sizeof(PKT_GSLS_ACTIVITY_NOTIFY_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSLS_ACTIVITY_NOTIFY_NTF);

	return true;
}

INT32 EncodePKT_GSLS_PLAYER_UPGRADE_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSLS_PLAYER_UPGRADE_NTF *pstIn = (PKT_GSLS_PLAYER_UPGRADE_NTF*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSLS_PLAYER_UPGRADE_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSLS_PLAYER_UPGRADE_NTF *pstOut = (PKT_GSLS_PLAYER_UPGRADE_NTF*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSLS_DSPNAME_UPGRADE_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSLS_DSPNAME_UPGRADE_NTF *pstIn = (PKT_GSLS_DSPNAME_UPGRADE_NTF*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDspName, sizeof(pstIn->aszDspName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSLS_DSPNAME_UPGRADE_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSLS_DSPNAME_UPGRADE_NTF *pstOut = (PKT_GSLS_DSPNAME_UPGRADE_NTF*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDspName, sizeof(pstOut->aszDspName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSLS_ACTIVITY_NOTIFY_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSLS_ACTIVITY_NOTIFY_NTF *pstIn = (PKT_GSLS_ACTIVITY_NOTIFY_NTF*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byActivityType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNotify))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSLS_ACTIVITY_NOTIFY_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSLS_ACTIVITY_NOTIFY_NTF *pstOut = (PKT_GSLS_ACTIVITY_NOTIFY_NTF*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byActivityType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNotify))
		return -1;

	return sizeof(*pstOut);
}

