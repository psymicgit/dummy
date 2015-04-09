#include "protogspk.h"
CProtoGSPK::CProtoGSPK()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoGSPK * CProtoGSPK::Instance()
{
   static CProtoGSPK oInst;
   return &oInst;
}

CProtoGSPK::~CProtoGSPK()
{
}

INT32 CProtoGSPK::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoGSPK pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoGSPK::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoGSPK pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoGSPK CProtoGSPK::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSPK_MSGID_OFFSET >= PROTOGSPK_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOGSPK_MSGID_OFFSET];
}

DeFuncCProtoGSPK CProtoGSPK::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOGSPK_MSGID_OFFSET >= PROTOGSPK_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOGSPK_MSGID_OFFSET];
}

BOOL CProtoGSPK::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[PKGS_GVG_GET_TOP_8_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_GVG_GET_TOP_8_REQ;
	m_DecodeFuncArray[PKGS_GVG_GET_TOP_8_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_GVG_GET_TOP_8_REQ;
	m_vecMsgList.push_back(PKGS_GVG_GET_TOP_8_REQ);
	if( (INT32)sizeof(PKT_PKGS_GVG_GET_TOP_8_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_GVG_GET_TOP_8_REQ);

	m_EncodeFuncArray[GSPK_GVG_RET_TOP_8_ACK - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_GVG_RET_TOP_8_ACK;
	m_DecodeFuncArray[GSPK_GVG_RET_TOP_8_ACK - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_GVG_RET_TOP_8_ACK;
	m_vecMsgList.push_back(GSPK_GVG_RET_TOP_8_ACK);
	if( (INT32)sizeof(PKT_GSPK_GVG_RET_TOP_8_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_GVG_RET_TOP_8_ACK);

	m_EncodeFuncArray[PKGS_GET_FACTIONS_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_GET_FACTIONS_REQ;
	m_DecodeFuncArray[PKGS_GET_FACTIONS_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_GET_FACTIONS_REQ;
	m_vecMsgList.push_back(PKGS_GET_FACTIONS_REQ);
	if( (INT32)sizeof(PKT_PKGS_GET_FACTIONS_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_GET_FACTIONS_REQ);

	m_EncodeFuncArray[GSPK_RET_FACTIONS_ACK - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_RET_FACTIONS_ACK;
	m_DecodeFuncArray[GSPK_RET_FACTIONS_ACK - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_RET_FACTIONS_ACK;
	m_vecMsgList.push_back(GSPK_RET_FACTIONS_ACK);
	if( (INT32)sizeof(PKT_GSPK_RET_FACTIONS_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_RET_FACTIONS_ACK);

	m_EncodeFuncArray[PKGS_GET_PLAYER_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_GET_PLAYER_REQ;
	m_DecodeFuncArray[PKGS_GET_PLAYER_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_GET_PLAYER_REQ;
	m_vecMsgList.push_back(PKGS_GET_PLAYER_REQ);
	if( (INT32)sizeof(PKT_PKGS_GET_PLAYER_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_GET_PLAYER_REQ);

	m_EncodeFuncArray[GSPK_RET_PLAYER_ACK - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_RET_PLAYER_ACK;
	m_DecodeFuncArray[GSPK_RET_PLAYER_ACK - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_RET_PLAYER_ACK;
	m_vecMsgList.push_back(GSPK_RET_PLAYER_ACK);
	if( (INT32)sizeof(PKT_GSPK_RET_PLAYER_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_RET_PLAYER_ACK);

	m_EncodeFuncArray[PKGS_GVG_NEXT_ROUND_NTF - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_GVG_NEXT_ROUND_NTF;
	m_DecodeFuncArray[PKGS_GVG_NEXT_ROUND_NTF - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_GVG_NEXT_ROUND_NTF;
	m_vecMsgList.push_back(PKGS_GVG_NEXT_ROUND_NTF);
	if( (INT32)sizeof(PKT_PKGS_GVG_NEXT_ROUND_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_GVG_NEXT_ROUND_NTF);

	m_EncodeFuncArray[PKGS_SYNC_GVG_NTF - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_SYNC_GVG_NTF;
	m_DecodeFuncArray[PKGS_SYNC_GVG_NTF - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_SYNC_GVG_NTF;
	m_vecMsgList.push_back(PKGS_SYNC_GVG_NTF);
	if( (INT32)sizeof(PKT_PKGS_SYNC_GVG_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_SYNC_GVG_NTF);

	m_EncodeFuncArray[GSPK_GET_VIDEO_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_GET_VIDEO_REQ;
	m_DecodeFuncArray[GSPK_GET_VIDEO_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_GET_VIDEO_REQ;
	m_vecMsgList.push_back(GSPK_GET_VIDEO_REQ);
	if( (INT32)sizeof(PKT_GSPK_GET_VIDEO_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_GET_VIDEO_REQ);

	m_EncodeFuncArray[PKGS_RET_VIDEO_ACK - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_RET_VIDEO_ACK;
	m_DecodeFuncArray[PKGS_RET_VIDEO_ACK - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_RET_VIDEO_ACK;
	m_vecMsgList.push_back(PKGS_RET_VIDEO_ACK);
	if( (INT32)sizeof(PKT_PKGS_RET_VIDEO_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_RET_VIDEO_ACK);

	m_EncodeFuncArray[PKGS_SYNC_VIDEO_NTF - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_SYNC_VIDEO_NTF;
	m_DecodeFuncArray[PKGS_SYNC_VIDEO_NTF - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_SYNC_VIDEO_NTF;
	m_vecMsgList.push_back(PKGS_SYNC_VIDEO_NTF);
	if( (INT32)sizeof(PKT_PKGS_SYNC_VIDEO_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_SYNC_VIDEO_NTF);

	m_EncodeFuncArray[GSPK_SUPPORT_FACTION_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_SUPPORT_FACTION_REQ;
	m_DecodeFuncArray[GSPK_SUPPORT_FACTION_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_SUPPORT_FACTION_REQ;
	m_vecMsgList.push_back(GSPK_SUPPORT_FACTION_REQ);
	if( (INT32)sizeof(PKT_GSPK_SUPPORT_FACTION_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_SUPPORT_FACTION_REQ);

	m_EncodeFuncArray[PKGS_SYNC_FACTION_SUPPORT_NTF - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_SYNC_FACTION_SUPPORT_NTF;
	m_DecodeFuncArray[PKGS_SYNC_FACTION_SUPPORT_NTF - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_SYNC_FACTION_SUPPORT_NTF;
	m_vecMsgList.push_back(PKGS_SYNC_FACTION_SUPPORT_NTF);
	if( (INT32)sizeof(PKT_PKGS_SYNC_FACTION_SUPPORT_NTF) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_SYNC_FACTION_SUPPORT_NTF);

	m_EncodeFuncArray[PKGS_GVG_ASK_READY_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_GVG_ASK_READY_REQ;
	m_DecodeFuncArray[PKGS_GVG_ASK_READY_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_GVG_ASK_READY_REQ;
	m_vecMsgList.push_back(PKGS_GVG_ASK_READY_REQ);
	if( (INT32)sizeof(PKT_PKGS_GVG_ASK_READY_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_GVG_ASK_READY_REQ);

	m_EncodeFuncArray[GSPK_GVG_REPLY_READY_ACK - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_GVG_REPLY_READY_ACK;
	m_DecodeFuncArray[GSPK_GVG_REPLY_READY_ACK - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_GVG_REPLY_READY_ACK;
	m_vecMsgList.push_back(GSPK_GVG_REPLY_READY_ACK);
	if( (INT32)sizeof(PKT_GSPK_GVG_REPLY_READY_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_GVG_REPLY_READY_ACK);

	m_EncodeFuncArray[GSPK_GET_GVG_ACTIVITY_DATA_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ;
	m_DecodeFuncArray[GSPK_GET_GVG_ACTIVITY_DATA_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ;
	m_vecMsgList.push_back(GSPK_GET_GVG_ACTIVITY_DATA_REQ);
	if( (INT32)sizeof(PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ);

	m_EncodeFuncArray[PKGS_GVG_ASK_OPEN_REQ - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_PKGS_GVG_ASK_OPEN_REQ;
	m_DecodeFuncArray[PKGS_GVG_ASK_OPEN_REQ - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_PKGS_GVG_ASK_OPEN_REQ;
	m_vecMsgList.push_back(PKGS_GVG_ASK_OPEN_REQ);
	if( (INT32)sizeof(PKT_PKGS_GVG_ASK_OPEN_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_PKGS_GVG_ASK_OPEN_REQ);

	m_EncodeFuncArray[GSPK_GVG_REPLY_OPEN_ACK - PROTOGSPK_MSGID_OFFSET] = &EncodePKT_GSPK_GVG_REPLY_OPEN_ACK;
	m_DecodeFuncArray[GSPK_GVG_REPLY_OPEN_ACK - PROTOGSPK_MSGID_OFFSET] = &DecodePKT_GSPK_GVG_REPLY_OPEN_ACK;
	m_vecMsgList.push_back(GSPK_GVG_REPLY_OPEN_ACK);
	if( (INT32)sizeof(PKT_GSPK_GVG_REPLY_OPEN_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_GSPK_GVG_REPLY_OPEN_ACK);

	return true;
}

INT32 EncodeDT_SYNC_PLAYER(void *pHost, CNetData* poNetData)
{
	DT_SYNC_PLAYER *pstIn = (DT_SYNC_PLAYER*)pHost;

	if(-1 == EncodeDT_PLAYER_NOPVP_DATA(&pstIn->stPlayerNoPvpData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SYNC_PLAYER(void *pHost, CNetData* poNetData)
{
	DT_SYNC_PLAYER *pstOut = (DT_SYNC_PLAYER*)pHost;

	if(-1 == DecodeDT_PLAYER_NOPVP_DATA(&pstOut->stPlayerNoPvpData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_GVG_NEXT_ROUND_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_NEXT_ROUND_NTF *pstIn = (PKT_PKGS_GVG_NEXT_ROUND_NTF*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byRound))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_GVG_NEXT_ROUND_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_NEXT_ROUND_NTF *pstOut = (PKT_PKGS_GVG_NEXT_ROUND_NTF*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byRound))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_GVG_GET_TOP_8_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_GET_TOP_8_REQ *pstIn = (PKT_PKGS_GVG_GET_TOP_8_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_GVG_GET_TOP_8_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_GET_TOP_8_REQ *pstOut = (PKT_PKGS_GVG_GET_TOP_8_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_GVG_RET_TOP_8_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GVG_RET_TOP_8_ACK *pstIn = (PKT_GSPK_GVG_RET_TOP_8_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byTop8Cnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GVG_TOP_FACTION_CNT; i++)
	{
		if(i >= (INT32)pstIn->byTop8Cnt)
			break;
		if(-1 == EncodeDT_GVG_FACTION(&pstIn->astTop8List[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_GVG_RET_TOP_8_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GVG_RET_TOP_8_ACK *pstOut = (PKT_GSPK_GVG_RET_TOP_8_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byTop8Cnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GVG_TOP_FACTION_CNT; i++)
	{
		if(i >= (INT32)pstOut->byTop8Cnt)
			break;
		if(-1 == DecodeDT_GVG_FACTION(&pstOut->astTop8List[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_GET_FACTIONS_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GET_FACTIONS_REQ *pstIn = (PKT_PKGS_GET_FACTIONS_REQ*)pHost;

	if(-1 == EncodeDT_ONE_ZONE_FACTION_ID_LIST(&pstIn->stFactionIDList, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_GET_FACTIONS_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GET_FACTIONS_REQ *pstOut = (PKT_PKGS_GET_FACTIONS_REQ*)pHost;

	if(-1 == DecodeDT_ONE_ZONE_FACTION_ID_LIST(&pstOut->stFactionIDList, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PK_FACTION_LIST(void *pHost, CNetData* poNetData)
{
	DT_PK_FACTION_LIST *pstIn = (DT_PK_FACTION_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFactionCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GVG_TOP_FACTION_CNT; i++)
	{
		if(i >= (INT32)pstIn->byFactionCnt)
			break;
		if(-1 == EncodeDT_PK_FACTION(&pstIn->astFactionList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PK_FACTION_LIST(void *pHost, CNetData* poNetData)
{
	DT_PK_FACTION_LIST *pstOut = (DT_PK_FACTION_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFactionCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GVG_TOP_FACTION_CNT; i++)
	{
		if(i >= (INT32)pstOut->byFactionCnt)
			break;
		if(-1 == DecodeDT_PK_FACTION(&pstOut->astFactionList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_RET_FACTIONS_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_RET_FACTIONS_ACK *pstIn = (PKT_GSPK_RET_FACTIONS_ACK*)pHost;

	if(-1 == EncodeDT_PK_FACTION_LIST(&pstIn->stRetFactions, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_RET_FACTIONS_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_RET_FACTIONS_ACK *pstOut = (PKT_GSPK_RET_FACTIONS_ACK*)pHost;

	if(-1 == DecodeDT_PK_FACTION_LIST(&pstOut->stRetFactions, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_GET_PLAYER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GET_PLAYER_REQ *pstIn = (PKT_PKGS_GET_PLAYER_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPKGroupID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGetCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_REQ_SYNC_PLAYER_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstIn->byGetCnt)
			break;
		if(-1 == poNetData->AddDword(pstIn->adwPlayerIDList[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_GET_PLAYER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GET_PLAYER_REQ *pstOut = (PKT_PKGS_GET_PLAYER_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPKGroupID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGetCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_REQ_SYNC_PLAYER_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstOut->byGetCnt)
			break;
		if(-1 == poNetData->DelDword(pstOut->adwPlayerIDList[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_RET_PLAYER_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_RET_PLAYER_ACK *pstIn = (PKT_GSPK_RET_PLAYER_ACK*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPKGroupID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRetCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RET_SYNC_PLAYER_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstIn->byRetCnt)
			break;
		if(-1 == EncodeDT_SYNC_PLAYER(&pstIn->astPlayerList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_RET_PLAYER_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_RET_PLAYER_ACK *pstOut = (PKT_GSPK_RET_PLAYER_ACK*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPKGroupID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRetCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RET_SYNC_PLAYER_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstOut->byRetCnt)
			break;
		if(-1 == DecodeDT_SYNC_PLAYER(&pstOut->astPlayerList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_GET_VIDEO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GET_VIDEO_REQ *pstIn = (PKT_GSPK_GET_VIDEO_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byEmpty))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_GET_VIDEO_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GET_VIDEO_REQ *pstOut = (PKT_GSPK_GET_VIDEO_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byEmpty))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_RET_VIDEO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_RET_VIDEO_ACK *pstIn = (PKT_PKGS_RET_VIDEO_ACK*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwVideoID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byStartSlot))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPvPVideoCnt))
		return -1;

	INT32 i;
	for(i = 0; i < GVG_PK_VIDEO_RET_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstIn->wPvPVideoCnt)
			break;
		if(-1 == EncodeDT_GVG_PK_VIDEO(&pstIn->astPvPVideoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_RET_VIDEO_ACK(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_RET_VIDEO_ACK *pstOut = (PKT_PKGS_RET_VIDEO_ACK*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwVideoID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byStartSlot))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPvPVideoCnt))
		return -1;

	INT32 i;
	for(i = 0; i < GVG_PK_VIDEO_RET_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstOut->wPvPVideoCnt)
			break;
		if(-1 == DecodeDT_GVG_PK_VIDEO(&pstOut->astPvPVideoList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_SYNC_VIDEO_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_SYNC_VIDEO_NTF *pstIn = (PKT_PKGS_SYNC_VIDEO_NTF*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wVideoCnt))
		return -1;

	INT32 i;
	for(i = 0; i < GVG_VIDEO_SYNC_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstIn->wVideoCnt)
			break;
		if(-1 == EncodeDT_GVG_VIDEO(&pstIn->astVideoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_SYNC_VIDEO_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_SYNC_VIDEO_NTF *pstOut = (PKT_PKGS_SYNC_VIDEO_NTF*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wVideoCnt))
		return -1;

	INT32 i;
	for(i = 0; i < GVG_VIDEO_SYNC_CNT_ONCE; i++)
	{
		if(i >= (INT32)pstOut->wVideoCnt)
			break;
		if(-1 == DecodeDT_GVG_VIDEO(&pstOut->astVideoList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_SYNC_GVG_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_SYNC_GVG_NTF *pstIn = (PKT_PKGS_SYNC_GVG_NTF*)pHost;

	if(-1 == EncodeDT_GVG_LOCAL_ACTIVITY_DATA(&pstIn->stGvGInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_SYNC_GVG_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_SYNC_GVG_NTF *pstOut = (PKT_PKGS_SYNC_GVG_NTF*)pHost;

	if(-1 == DecodeDT_GVG_LOCAL_ACTIVITY_DATA(&pstOut->stGvGInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_SUPPORT_INFO(void *pHost, CNetData* poNetData)
{
	DT_FACTION_SUPPORT_INFO *pstIn = (DT_FACTION_SUPPORT_INFO*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwFactionID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSupportCnt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_SUPPORT_INFO(void *pHost, CNetData* poNetData)
{
	DT_FACTION_SUPPORT_INFO *pstOut = (DT_FACTION_SUPPORT_INFO*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwFactionID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSupportCnt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_SUPPORT_LIST(void *pHost, CNetData* poNetData)
{
	DT_FACTION_SUPPORT_LIST *pstIn = (DT_FACTION_SUPPORT_LIST*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwFactionCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GVG_ARENA_FACTION_CNT; i++)
	{
		if(i >= (INT32)pstIn->dwFactionCnt)
			break;
		if(-1 == EncodeDT_FACTION_SUPPORT_INFO(&pstIn->astSupportList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_SUPPORT_LIST(void *pHost, CNetData* poNetData)
{
	DT_FACTION_SUPPORT_LIST *pstOut = (DT_FACTION_SUPPORT_LIST*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwFactionCnt))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GVG_ARENA_FACTION_CNT; i++)
	{
		if(i >= (INT32)pstOut->dwFactionCnt)
			break;
		if(-1 == DecodeDT_FACTION_SUPPORT_INFO(&pstOut->astSupportList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_SUPPORT_FACTION_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_SUPPORT_FACTION_REQ *pstIn = (PKT_GSPK_SUPPORT_FACTION_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwArenaID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBigRound))
		return -1;

	if(-1 == EncodeDT_FACTION_SUPPORT_LIST(&pstIn->stFactionSupportInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_SUPPORT_FACTION_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_SUPPORT_FACTION_REQ *pstOut = (PKT_GSPK_SUPPORT_FACTION_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwArenaID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBigRound))
		return -1;

	if(-1 == DecodeDT_FACTION_SUPPORT_LIST(&pstOut->stFactionSupportInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_SYNC_FACTION_SUPPORT_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_SYNC_FACTION_SUPPORT_NTF *pstIn = (PKT_PKGS_SYNC_FACTION_SUPPORT_NTF*)pHost;

	if(-1 == EncodeDT_FACTION_SUPPORT_LIST(&pstIn->stFactionSupportInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_SYNC_FACTION_SUPPORT_NTF(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_SYNC_FACTION_SUPPORT_NTF *pstOut = (PKT_PKGS_SYNC_FACTION_SUPPORT_NTF*)pHost;

	if(-1 == DecodeDT_FACTION_SUPPORT_LIST(&pstOut->stFactionSupportInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_GVG_ASK_READY_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_ASK_READY_REQ *pstIn = (PKT_PKGS_GVG_ASK_READY_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byStage))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_GVG_ASK_READY_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_ASK_READY_REQ *pstOut = (PKT_PKGS_GVG_ASK_READY_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byStage))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_GVG_REPLY_READY_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GVG_REPLY_READY_ACK *pstIn = (PKT_GSPK_GVG_REPLY_READY_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byStage))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byIsReady))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_GVG_REPLY_READY_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GVG_REPLY_READY_ACK *pstOut = (PKT_GSPK_GVG_REPLY_READY_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byStage))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byIsReady))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ *pstIn = (PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byNothing))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ *pstOut = (PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byNothing))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_PKGS_GVG_ASK_OPEN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_ASK_OPEN_REQ *pstIn = (PKT_PKGS_GVG_ASK_OPEN_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byNothing))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_PKGS_GVG_ASK_OPEN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_PKGS_GVG_ASK_OPEN_REQ *pstOut = (PKT_PKGS_GVG_ASK_OPEN_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byNothing))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_GSPK_GVG_REPLY_OPEN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GVG_REPLY_OPEN_ACK *pstIn = (PKT_GSPK_GVG_REPLY_OPEN_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byIsCanOpen))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_GSPK_GVG_REPLY_OPEN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_GSPK_GVG_REPLY_OPEN_ACK *pstOut = (PKT_GSPK_GVG_REPLY_OPEN_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byIsCanOpen))
		return -1;

	return sizeof(*pstOut);
}

