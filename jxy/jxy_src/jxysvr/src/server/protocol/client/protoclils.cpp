#include "protoclils.h"
CProtoCliLS::CProtoCliLS()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoCliLS * CProtoCliLS::Instance()
{
   static CProtoCliLS oInst;
   return &oInst;
}

CProtoCliLS::~CProtoCliLS()
{
}

INT32 CProtoCliLS::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 2);

	EnFuncCProtoCliLS pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoCliLS::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 2);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoCliLS pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoCliLS CProtoCliLS::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLILS_MSGID_OFFSET >= PROTOCLILS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOCLILS_MSGID_OFFSET];
}

DeFuncCProtoCliLS CProtoCliLS::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCLILS_MSGID_OFFSET >= PROTOCLILS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOCLILS_MSGID_OFFSET];
}

BOOL CProtoCliLS::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLILS_LOGIN_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_REQ;
	m_DecodeFuncArray[CLILS_LOGIN_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_REQ;
	m_vecMsgList.push_back(CLILS_LOGIN_REQ);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_REQ);

	m_EncodeFuncArray[CLILS_LOGIN_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_ACK;
	m_DecodeFuncArray[CLILS_LOGIN_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_ACK;
	m_vecMsgList.push_back(CLILS_LOGIN_ACK);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_ACK);

	m_EncodeFuncArray[CLILS_REGISTER_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_REGISTER_REQ;
	m_DecodeFuncArray[CLILS_REGISTER_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_REGISTER_REQ;
	m_vecMsgList.push_back(CLILS_REGISTER_REQ);
	if( (INT32)sizeof(PKT_CLILS_REGISTER_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_REGISTER_REQ);

	m_EncodeFuncArray[CLILS_REGISTER_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_REGISTER_ACK;
	m_DecodeFuncArray[CLILS_REGISTER_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_REGISTER_ACK;
	m_vecMsgList.push_back(CLILS_REGISTER_ACK);
	if( (INT32)sizeof(PKT_CLILS_REGISTER_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_REGISTER_ACK);

	m_EncodeFuncArray[CLILS_BIND_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_REQ;
	m_DecodeFuncArray[CLILS_BIND_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_REQ;
	m_vecMsgList.push_back(CLILS_BIND_REQ);
	if( (INT32)sizeof(PKT_CLILS_BIND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_REQ);

	m_EncodeFuncArray[CLILS_BIND_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_ACK;
	m_DecodeFuncArray[CLILS_BIND_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_ACK;
	m_vecMsgList.push_back(CLILS_BIND_ACK);
	if( (INT32)sizeof(PKT_CLILS_BIND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_ACK);

	m_EncodeFuncArray[CLILS_UNBIND_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_REQ;
	m_DecodeFuncArray[CLILS_UNBIND_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_REQ;
	m_vecMsgList.push_back(CLILS_UNBIND_REQ);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_REQ);

	m_EncodeFuncArray[CLILS_UNBIND_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_UNBIND_ACK;
	m_DecodeFuncArray[CLILS_UNBIND_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_UNBIND_ACK;
	m_vecMsgList.push_back(CLILS_UNBIND_ACK);
	if( (INT32)sizeof(PKT_CLILS_UNBIND_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_UNBIND_ACK);

	m_EncodeFuncArray[CLILS_LOGIN_EX_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_EX_REQ;
	m_DecodeFuncArray[CLILS_LOGIN_EX_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_EX_REQ;
	m_vecMsgList.push_back(CLILS_LOGIN_EX_REQ);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_EX_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_EX_REQ);

	m_EncodeFuncArray[CLILS_LOGIN_EX_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGIN_EX_ACK;
	m_DecodeFuncArray[CLILS_LOGIN_EX_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGIN_EX_ACK;
	m_vecMsgList.push_back(CLILS_LOGIN_EX_ACK);
	if( (INT32)sizeof(PKT_CLILS_LOGIN_EX_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGIN_EX_ACK);

	m_EncodeFuncArray[CLILS_LOGOUT_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGOUT_REQ;
	m_DecodeFuncArray[CLILS_LOGOUT_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGOUT_REQ;
	m_vecMsgList.push_back(CLILS_LOGOUT_REQ);
	if( (INT32)sizeof(PKT_CLILS_LOGOUT_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGOUT_REQ);

	m_EncodeFuncArray[CLILS_LOGOUT_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_LOGOUT_ACK;
	m_DecodeFuncArray[CLILS_LOGOUT_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_LOGOUT_ACK;
	m_vecMsgList.push_back(CLILS_LOGOUT_ACK);
	if( (INT32)sizeof(PKT_CLILS_LOGOUT_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_LOGOUT_ACK);

	m_EncodeFuncArray[CLILS_CHANGE_PWD_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_CHANGE_PWD_REQ;
	m_DecodeFuncArray[CLILS_CHANGE_PWD_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_CHANGE_PWD_REQ;
	m_vecMsgList.push_back(CLILS_CHANGE_PWD_REQ);
	if( (INT32)sizeof(PKT_CLILS_CHANGE_PWD_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_CHANGE_PWD_REQ);

	m_EncodeFuncArray[CLILS_CHANGE_PWD_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_CHANGE_PWD_ACK;
	m_DecodeFuncArray[CLILS_CHANGE_PWD_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_CHANGE_PWD_ACK;
	m_vecMsgList.push_back(CLILS_CHANGE_PWD_ACK);
	if( (INT32)sizeof(PKT_CLILS_CHANGE_PWD_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_CHANGE_PWD_ACK);

	m_EncodeFuncArray[CLILS_BIND_MOBILE_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_MOBILE_ACK;
	m_DecodeFuncArray[CLILS_BIND_MOBILE_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_MOBILE_ACK;
	m_vecMsgList.push_back(CLILS_BIND_MOBILE_ACK);
	if( (INT32)sizeof(PKT_CLILS_BIND_MOBILE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_MOBILE_ACK);

	m_EncodeFuncArray[CLILS_BIND_MOBILE_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_BIND_MOBILE_REQ;
	m_DecodeFuncArray[CLILS_BIND_MOBILE_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_BIND_MOBILE_REQ;
	m_vecMsgList.push_back(CLILS_BIND_MOBILE_REQ);
	if( (INT32)sizeof(PKT_CLILS_BIND_MOBILE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_BIND_MOBILE_REQ);

	m_EncodeFuncArray[CLILS_VER_CODE_REQ - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_VER_CODE_REQ;
	m_DecodeFuncArray[CLILS_VER_CODE_REQ - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_VER_CODE_REQ;
	m_vecMsgList.push_back(CLILS_VER_CODE_REQ);
	if( (INT32)sizeof(PKT_CLILS_VER_CODE_REQ) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_VER_CODE_REQ);

	m_EncodeFuncArray[CLILS_VER_CODE_ACK - PROTOCLILS_MSGID_OFFSET] = &EncodePKT_CLILS_VER_CODE_ACK;
	m_DecodeFuncArray[CLILS_VER_CODE_ACK - PROTOCLILS_MSGID_OFFSET] = &DecodePKT_CLILS_VER_CODE_ACK;
	m_vecMsgList.push_back(CLILS_VER_CODE_ACK);
	if( (INT32)sizeof(PKT_CLILS_VER_CODE_ACK) > m_iMaxPkgSize)
		m_iMaxPkgSize = (INT32)sizeof(PKT_CLILS_VER_CODE_ACK);

	return true;
}

INT32 EncodePKT_CLILS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_REQ *pstIn = (PKT_CLILS_LOGIN_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliVersion))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSerialNumber))
		return -1;

	if(-1 == EncodeDT_EXT_FUNC_PARAM_LST(&pstIn->stExtFuncParamLst, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_LOGIN_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_REQ *pstOut = (PKT_CLILS_LOGIN_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliVersion))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSerialNumber))
		return -1;

	if(-1 == DecodeDT_EXT_FUNC_PARAM_LST(&pstOut->stExtFuncParamLst, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_ACK *pstIn = (PKT_CLILS_LOGIN_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == EncodeDT_VERSION_DATA(&pstIn->stVersionInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszZoneName, sizeof(pstIn->aszZoneName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == EncodeDT_EXT_FUNC_PARAM_LST(&pstIn->stExtFuncParamLst, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_LOGIN_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_ACK *pstOut = (PKT_CLILS_LOGIN_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == DecodeDT_VERSION_DATA(&pstOut->stVersionInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszZoneName, sizeof(pstOut->aszZoneName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == DecodeDT_EXT_FUNC_PARAM_LST(&pstOut->stExtFuncParamLst, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_EX_REQ *pstIn = (PKT_CLILS_LOGIN_EX_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliVersion))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthMethod))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_EX_REQ *pstOut = (PKT_CLILS_LOGIN_EX_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliVersion))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthMethod))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_EX_ACK *pstIn = (PKT_CLILS_LOGIN_EX_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == EncodeDT_VERSION_DATA(&pstIn->stVersionInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_NOTICE_DATA(&pstIn->stNoticeInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNotifyID, sizeof(pstIn->aszNotifyID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wZoneID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszZoneName, sizeof(pstIn->aszZoneName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCareerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGIN_EX_ACK *pstOut = (PKT_CLILS_LOGIN_EX_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == DecodeDT_VERSION_DATA(&pstOut->stVersionInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_NOTICE_DATA(&pstOut->stNoticeInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOtherDeviceLoginFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNotifyID, sizeof(pstOut->aszNotifyID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wZoneID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszZoneName, sizeof(pstOut->aszZoneName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCareerID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_REGISTER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_REGISTER_REQ *pstIn = (PKT_CLILS_REGISTER_REQ*)pHost;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserPwd, sizeof(pstIn->aszUserPwd)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_REGISTER_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_REGISTER_REQ *pstOut = (PKT_CLILS_REGISTER_REQ*)pHost;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserPwd, sizeof(pstOut->aszUserPwd)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_REGISTER_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_REGISTER_ACK *pstIn = (PKT_CLILS_REGISTER_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_REGISTER_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_REGISTER_ACK *pstOut = (PKT_CLILS_REGISTER_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_BIND_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_REQ *pstIn = (PKT_CLILS_BIND_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNickName, sizeof(pstIn->aszNickName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTokenLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byTokenLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyTokenKey[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_BIND_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_REQ *pstOut = (PKT_CLILS_BIND_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNickName, sizeof(pstOut->aszNickName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTokenLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byTokenLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyTokenKey[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_BIND_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_ACK *pstIn = (PKT_CLILS_BIND_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_BIND_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_ACK *pstOut = (PKT_CLILS_BIND_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_BIND_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_EX_REQ *pstIn = (PKT_CLILS_BIND_EX_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNickName, sizeof(pstIn->aszNickName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTokenLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byTokenLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyTokenKey[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_BIND_EX_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_EX_REQ *pstOut = (PKT_CLILS_BIND_EX_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNickName, sizeof(pstOut->aszNickName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTokenLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byTokenLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyTokenKey[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_BIND_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_EX_ACK *pstIn = (PKT_CLILS_BIND_EX_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_BIND_EX_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_EX_ACK *pstOut = (PKT_CLILS_BIND_EX_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_UNBIND_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_UNBIND_REQ *pstIn = (PKT_CLILS_UNBIND_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUserName, sizeof(pstIn->aszUserName)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_UNBIND_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_UNBIND_REQ *pstOut = (PKT_CLILS_UNBIND_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUserName, sizeof(pstOut->aszUserName)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_UNBIND_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_UNBIND_ACK *pstIn = (PKT_CLILS_UNBIND_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_UNBIND_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_UNBIND_ACK *pstOut = (PKT_CLILS_UNBIND_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_LOGOUT_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGOUT_REQ *pstIn = (PKT_CLILS_LOGOUT_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAuthType))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDeviceID, sizeof(pstIn->aszDeviceID)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_LOGOUT_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGOUT_REQ *pstOut = (PKT_CLILS_LOGOUT_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAuthType))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDeviceID, sizeof(pstOut->aszDeviceID)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_LOGOUT_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGOUT_ACK *pstIn = (PKT_CLILS_LOGOUT_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_LOGOUT_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_LOGOUT_ACK *pstOut = (PKT_CLILS_LOGOUT_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_CHANGE_PWD_REQ *pstIn = (PKT_CLILS_CHANGE_PWD_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byNewPwdLen))
		return -1;

	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstIn->byNewPwdLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyNewUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_CHANGE_PWD_REQ *pstOut = (PKT_CLILS_CHANGE_PWD_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byPwdLen))
		return -1;

	INT32 i;
	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byNewPwdLen))
		return -1;

	for(i = 0; i < USERNAME_LEN; i++)
	{
		if(i >= (INT32)pstOut->byNewPwdLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyNewUserPwd[i]))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_CHANGE_PWD_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_CHANGE_PWD_ACK *pstIn = (PKT_CLILS_CHANGE_PWD_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_CHANGE_PWD_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_CHANGE_PWD_ACK *pstOut = (PKT_CLILS_CHANGE_PWD_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_BIND_MOBILE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_MOBILE_REQ *pstIn = (PKT_CLILS_BIND_MOBILE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyMObile[i]))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_BIND_MOBILE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_MOBILE_REQ *pstOut = (PKT_CLILS_BIND_MOBILE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyMObile[i]))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_BIND_MOBILE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_MOBILE_ACK *pstIn = (PKT_CLILS_BIND_MOBILE_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCountdown))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_BIND_MOBILE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_BIND_MOBILE_ACK *pstOut = (PKT_CLILS_BIND_MOBILE_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCountdown))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_VER_CODE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_VER_CODE_REQ *pstIn = (PKT_CLILS_VER_CODE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyVerCode[i]))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwUserID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_VER_CODE_REQ(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_VER_CODE_REQ *pstOut = (PKT_CLILS_VER_CODE_REQ*)pHost;

	INT32 i;
	for(i = 0; i < MAX_MOBILE_LENGTH; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyVerCode[i]))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwUserID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodePKT_CLILS_VER_CODE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_VER_CODE_ACK *pstIn = (PKT_CLILS_VER_CODE_ACK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodePKT_CLILS_VER_CODE_ACK(void *pHost, CNetData* poNetData)
{
	PKT_CLILS_VER_CODE_ACK *pstOut = (PKT_CLILS_VER_CODE_ACK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

