#include "protolstlc.h"
CProtoLSTLC::CProtoLSTLC()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoLSTLC * CProtoLSTLC::Instance()
{
   static CProtoLSTLC oInst;
   return &oInst;
}

CProtoLSTLC::~CProtoLSTLC()
{
}

INT32 CProtoLSTLC::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoLSTLC pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoLSTLC::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoLSTLC pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoLSTLC CProtoLSTLC::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOLSTLC_MSGID_OFFSET >= PROTOLSTLC_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOLSTLC_MSGID_OFFSET];
}

DeFuncCProtoLSTLC CProtoLSTLC::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOLSTLC_MSGID_OFFSET >= PROTOLSTLC_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOLSTLC_MSGID_OFFSET];
}

BOOL CProtoLSTLC::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[LSTLC_AUTH_REQ - PROTOLSTLC_MSGID_OFFSET] = &EncodePKT_LSTLC_AUTH_REQ;
	m_DecodeFuncArray[LSTLC_AUTH_REQ - PROTOLSTLC_MSGID_OFFSET] = &DecodePKT_LSTLC_AUTH_REQ;
	m_vecMsgList.push_back(LSTLC_AUTH_REQ);
	if( (INT32)sizeof(PKT_LSTLC_AUTH_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_LSTLC_AUTH_REQ);

	m_EncodeFuncArray[LSTLC_AUTH_ACK - PROTOLSTLC_MSGID_OFFSET] = &EncodePKT_LSTLC_AUTH_ACK;
	m_DecodeFuncArray[LSTLC_AUTH_ACK - PROTOLSTLC_MSGID_OFFSET] = &DecodePKT_LSTLC_AUTH_ACK;
	m_vecMsgList.push_back(LSTLC_AUTH_ACK);
	if( (INT32)sizeof(PKT_LSTLC_AUTH_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_LSTLC_AUTH_ACK);

	return true;
}

INT32 EncodePKT_LSTLC_AUTH_REQ(void *pHost, CNetData* poNetData)
{
	PKT_LSTLC_AUTH_REQ *pstIn = (PKT_LSTLC_AUTH_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < PWD_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwParam1))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam2))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam3))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwParam4))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszExtContext, sizeof(pstIn->aszExtContext)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_LSTLC_AUTH_REQ(void *pHost, CNetData* poNetData)
{
	PKT_LSTLC_AUTH_REQ *pstOut = (PKT_LSTLC_AUTH_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < PWD_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwParam1))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam2))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam3))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwParam4))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszExtContext, sizeof(pstOut->aszExtContext)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_LSTLC_AUTH_ACK(void *pHost, CNetData* poNetData)
{
	PKT_LSTLC_AUTH_ACK *pstIn = (PKT_LSTLC_AUTH_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < PWD_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwParam1))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam2))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam3))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwParam4))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszExtContext, sizeof(pstIn->aszExtContext)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_LSTLC_AUTH_ACK(void *pHost, CNetData* poNetData)
{
	PKT_LSTLC_AUTH_ACK *pstOut = (PKT_LSTLC_AUTH_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < PWD_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwParam1))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam2))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam3))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwParam4))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszExtContext, sizeof(pstOut->aszExtContext)))
		return -1;

	return sizeof(*pstOut);
}

