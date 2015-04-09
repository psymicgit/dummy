#include "protogsgs.h"
CProtoGSGS::CProtoGSGS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGSGS * CProtoGSGS::Instance()
{
   static CProtoGSGS oInst;
   return &oInst;
}

CProtoGSGS::~CProtoGSGS()
{
}

INT32 CProtoGSGS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGSGS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGSGS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGSGS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGSGS CProtoGSGS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSGS_MSGID_OFFSET >= PROTOGSGS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGSGS_MSGID_OFFSET];
}

DeFuncCProtoGSGS CProtoGSGS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSGS_MSGID_OFFSET >= PROTOGSGS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGSGS_MSGID_OFFSET];
}

BOOL CProtoGSGS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[GSGS_START_SYNC_PLAYER_RPT - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_START_SYNC_PLAYER_RPT;
	m_DecodeFuncArray[GSGS_START_SYNC_PLAYER_RPT - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_START_SYNC_PLAYER_RPT;
	m_vecMsgList.push_back(GSGS_START_SYNC_PLAYER_RPT);
	if( (INT32)sizeof(PKT_GSGS_START_SYNC_PLAYER_RPT) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_START_SYNC_PLAYER_RPT);

	m_EncodeFuncArray[GSGS_SYNC_PLAYER_REQ - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_SYNC_PLAYER_REQ;
	m_DecodeFuncArray[GSGS_SYNC_PLAYER_REQ - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_SYNC_PLAYER_REQ;
	m_vecMsgList.push_back(GSGS_SYNC_PLAYER_REQ);
	if( (INT32)sizeof(PKT_GSGS_SYNC_PLAYER_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_SYNC_PLAYER_REQ);

	m_EncodeFuncArray[GSGS_SYNC_PLAYER_RPT - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_SYNC_PLAYER_RPT;
	m_DecodeFuncArray[GSGS_SYNC_PLAYER_RPT - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_SYNC_PLAYER_RPT;
	m_vecMsgList.push_back(GSGS_SYNC_PLAYER_RPT);
	if( (INT32)sizeof(PKT_GSGS_SYNC_PLAYER_RPT) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_SYNC_PLAYER_RPT);

	m_EncodeFuncArray[GSGS_SYNC_DSPNAME_REQ - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_SYNC_DSPNAME_REQ;
	m_DecodeFuncArray[GSGS_SYNC_DSPNAME_REQ - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_SYNC_DSPNAME_REQ;
	m_vecMsgList.push_back(GSGS_SYNC_DSPNAME_REQ);
	if( (INT32)sizeof(PKT_GSGS_SYNC_DSPNAME_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_SYNC_DSPNAME_REQ);

	m_EncodeFuncArray[GSGS_SYNC_DSPNAME_RPT - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_SYNC_DSPNAME_RPT;
	m_DecodeFuncArray[GSGS_SYNC_DSPNAME_RPT - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_SYNC_DSPNAME_RPT;
	m_vecMsgList.push_back(GSGS_SYNC_DSPNAME_RPT);
	if( (INT32)sizeof(PKT_GSGS_SYNC_DSPNAME_RPT) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_SYNC_DSPNAME_RPT);

	m_EncodeFuncArray[GSGS_SYNC_BOSSB_REQ - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_SYNC_BOSSB_REQ;
	m_DecodeFuncArray[GSGS_SYNC_BOSSB_REQ - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_SYNC_BOSSB_REQ;
	m_vecMsgList.push_back(GSGS_SYNC_BOSSB_REQ);
	if( (INT32)sizeof(PKT_GSGS_SYNC_BOSSB_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_SYNC_BOSSB_REQ);

	m_EncodeFuncArray[GSGS_SYNC_BOSSB_RPT - PROTOGSGS_MSGID_OFFSET] = &EncodePKT_GSGS_SYNC_BOSSB_RPT;
	m_DecodeFuncArray[GSGS_SYNC_BOSSB_RPT - PROTOGSGS_MSGID_OFFSET] = &DecodePKT_GSGS_SYNC_BOSSB_RPT;
	m_vecMsgList.push_back(GSGS_SYNC_BOSSB_RPT);
	if( (INT32)sizeof(PKT_GSGS_SYNC_BOSSB_RPT) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSGS_SYNC_BOSSB_RPT);

	return true;
}

INT32 EncodePKT_GSGS_START_SYNC_PLAYER_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_START_SYNC_PLAYER_RPT *pstIn = (PKT_GSGS_START_SYNC_PLAYER_RPT*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwServerStartTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_START_SYNC_PLAYER_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_START_SYNC_PLAYER_RPT *pstOut = (PKT_GSGS_START_SYNC_PLAYER_RPT*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwServerStartTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_USER_DATA(void *pHost, CNetData* poNetData)
{
	DT_USER_DATA *pstIn = (DT_USER_DATA*)pHost;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wOriZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwLoginIP))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCliVer))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_USER_DATA(void *pHost, CNetData* poNetData)
{
	DT_USER_DATA *pstOut = (DT_USER_DATA*)pHost;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wOriZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwLoginIP))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCliVer))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSGS_SYNC_PLAYER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_PLAYER_REQ *pstIn = (PKT_GSGS_SYNC_PLAYER_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerDataLen))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_PLAYER_DATA_LEN; i++)
	{
		if(i >= (INT32)pstIn->dwPlayerDataLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyPlayerDataInfo[i]))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byHaveUserFlag))
		return -1;

	for(i = 0; i < 1; i++)
	{
		if(i >= (INT32)pstIn->byHaveUserFlag)
			break;
		if(-1 == EncodeDT_USER_DATA(&pstIn->astUserInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_SYNC_PLAYER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_PLAYER_REQ *pstOut = (PKT_GSGS_SYNC_PLAYER_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerDataLen))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_PLAYER_DATA_LEN; i++)
	{
		if(i >= (INT32)pstOut->dwPlayerDataLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyPlayerDataInfo[i]))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byHaveUserFlag))
		return -1;

	for(i = 0; i < 1; i++)
	{
		if(i >= (INT32)pstOut->byHaveUserFlag)
			break;
		if(-1 == DecodeDT_USER_DATA(&pstOut->astUserInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSGS_SYNC_PLAYER_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_PLAYER_RPT *pstIn = (PKT_GSGS_SYNC_PLAYER_RPT*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_SYNC_PLAYER_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_PLAYER_RPT *pstOut = (PKT_GSGS_SYNC_PLAYER_RPT*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLAYER_DSPNAME_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_DSPNAME_DATA *pstIn = (DT_PLAYER_DSPNAME_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDspName, sizeof(pstIn->aszDspName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_DSPNAME_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_DSPNAME_DATA *pstOut = (DT_PLAYER_DSPNAME_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDspName, sizeof(pstOut->aszDspName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSGS_SYNC_DSPNAME_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_DSPNAME_REQ *pstIn = (PKT_GSGS_SYNC_DSPNAME_REQ*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSyncNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SYNC_DSPNAME_ONCE; i++)
	{
		if(i >= (INT32)pstIn->wSyncNum)
			break;
		if(-1 == EncodeDT_PLAYER_DSPNAME_DATA(&pstIn->astPlayerDspNameInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_SYNC_DSPNAME_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_DSPNAME_REQ *pstOut = (PKT_GSGS_SYNC_DSPNAME_REQ*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSyncNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SYNC_DSPNAME_ONCE; i++)
	{
		if(i >= (INT32)pstOut->wSyncNum)
			break;
		if(-1 == DecodeDT_PLAYER_DSPNAME_DATA(&pstOut->astPlayerDspNameInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSGS_SYNC_DSPNAME_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_DSPNAME_RPT *pstIn = (PKT_GSGS_SYNC_DSPNAME_RPT*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwLastPlayerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_SYNC_DSPNAME_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_DSPNAME_RPT *pstOut = (PKT_GSGS_SYNC_DSPNAME_RPT*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwLastPlayerID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSGS_SYNC_BOSSB_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_BOSSB_REQ *pstIn = (PKT_GSGS_SYNC_BOSSB_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byActivityID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCurHP))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStartTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_SYNC_BOSSB_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_BOSSB_REQ *pstOut = (PKT_GSGS_SYNC_BOSSB_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byActivityID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCurHP))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStartTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSGS_SYNC_BOSSB_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_BOSSB_RPT *pstIn = (PKT_GSGS_SYNC_BOSSB_RPT*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSGS_SYNC_BOSSB_RPT(void *pHost, CNetData* poNetData)
{
	PKT_GSGS_SYNC_BOSSB_RPT *pstOut = (PKT_GSGS_SYNC_BOSSB_RPT*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

