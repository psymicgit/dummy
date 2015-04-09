#include "protogsns.h"
#include <sstream>

CProtoGSNS::CProtoGSNS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGSNS * CProtoGSNS::Instance()
{
   static CProtoGSNS oInst;
   return &oInst;
}

CProtoGSNS::~CProtoGSNS()
{
}

INT32 CProtoGSNS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGSNS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGSNS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGSNS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGSNS CProtoGSNS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSNS_MSGID_OFFSET >= PROTOGSNS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGSNS_MSGID_OFFSET];
}

DeFuncCProtoGSNS CProtoGSNS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSNS_MSGID_OFFSET >= PROTOGSNS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGSNS_MSGID_OFFSET];
}

BOOL CProtoGSNS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GSNS_PLAYER_DEFAULT_NTF - PROTOGSNS_MSGID_OFFSET] = &EncodePKT_GSNS_PLAYER_DEFAULT_NTF;
	m_DecodeFuncArray[GSNS_PLAYER_DEFAULT_NTF - PROTOGSNS_MSGID_OFFSET] = &DecodePKT_GSNS_PLAYER_DEFAULT_NTF;
	m_vecMsgList.push_back(GSNS_PLAYER_DEFAULT_NTF);
	if( (INT32)sizeof(PKT_GSNS_PLAYER_DEFAULT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSNS_PLAYER_DEFAULT_NTF);

	return true;
}

INT32 EncodePKT_GSNS_PLAYER_DEFAULT_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSNS_PLAYER_DEFAULT_NTF *pstIn = (PKT_GSNS_PLAYER_DEFAULT_NTF*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAction))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyToken[i]))
			return -1;
	}

	if(-1 == poNetData->AddTString(pstIn->aszPayLoad, sizeof(pstIn->aszPayLoad)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSNS_PLAYER_DEFAULT_NTF(void *pHost, CNetData* poNetData)
{
	PKT_GSNS_PLAYER_DEFAULT_NTF *pstOut = (PKT_GSNS_PLAYER_DEFAULT_NTF*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAction))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyToken[i]))
			return -1;
	}

	if(-1 == poNetData->DelTString(pstOut->aszPayLoad, sizeof(pstOut->aszPayLoad)))
		return -1;

	return sizeof(*pstOut);
}

