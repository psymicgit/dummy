#include "protogtcommon.h"
#include <sstream>

CProtoGTCommon::CProtoGTCommon()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGTCommon * CProtoGTCommon::Instance()
{
   static CProtoGTCommon oInst;
   return &oInst;
}

CProtoGTCommon::~CProtoGTCommon()
{
}

INT32 CProtoGTCommon::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGTCommon pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGTCommon::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGTCommon pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGTCommon CProtoGTCommon::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGTCOMMON_MSGID_OFFSET >= PROTOGTCOMMON_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGTCOMMON_MSGID_OFFSET];
}

DeFuncCProtoGTCommon CProtoGTCommon::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGTCOMMON_MSGID_OFFSET >= PROTOGTCOMMON_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGTCOMMON_MSGID_OFFSET];
}

BOOL CProtoGTCommon::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GTCOM_CLIENT_DOWN_RPT - PROTOGTCOMMON_MSGID_OFFSET] = &EncodePKT_GTCOM_CLIENT_DOWN_RPT;
	m_DecodeFuncArray[GTCOM_CLIENT_DOWN_RPT - PROTOGTCOMMON_MSGID_OFFSET] = &DecodePKT_GTCOM_CLIENT_DOWN_RPT;
	m_vecMsgList.push_back(GTCOM_CLIENT_DOWN_RPT);
	if( (INT32)sizeof(PKT_GTCOM_CLIENT_DOWN_RPT) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GTCOM_CLIENT_DOWN_RPT);

	return true;
}

INT32 EncodePKT_GTCOM_CLIENT_DOWN_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GTCOM_CLIENT_DOWN_RPT *pstIn = (PKT_GTCOM_CLIENT_DOWN_RPT*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byDownType))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GTCOM_CLIENT_DOWN_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GTCOM_CLIENT_DOWN_RPT *pstOut = (PKT_GTCOM_CLIENT_DOWN_RPT*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byDownType))
		return -1;

	return sizeof(*pstOut);
}

