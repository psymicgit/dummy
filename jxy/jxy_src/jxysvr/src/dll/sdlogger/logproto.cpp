#include "loggerutils.h"
#include "logproto.h"
#include <memory.h>
CLogProto::CLogProto()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	Init();
}

CLogProto::~CLogProto()
{
}

int CLogProto::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);

	EnFuncCLogProto pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (this->*pfnEncode)(pHost);
}

int CLogProto::Decode(int iMsgID, char *pNet,int iNetSize, void *pHost)
{
	m_oData.Prepare(pNet, iNetSize);

	DeFuncCLogProto pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (this->*pfnDecode)(pHost);
}

EnFuncCLogProto CLogProto::FindEncodeFunc(int iMsgID)
{
	if(iMsgID >= 8192)
		return NULL;
	return m_EncodeFuncArray[iMsgID];
}

DeFuncCLogProto CLogProto::FindDecodeFunc(int iMsgID)
{
	if(iMsgID >= 8192)
		return NULL;
	return m_DecodeFuncArray[iMsgID];
}

int CLogProto::EncodeCliDate(void *pHost)
{
	CliDate *pstIn = (CliDate*)pHost;

	if(-1 == m_oData.AddDword(pstIn->dwYear))
		return -1;

	if(-1 == m_oData.AddDword(pstIn->dwMonth))
		return -1;

	if(-1 == m_oData.AddDword(pstIn->dwDay))
		return -1;

	return m_oData.GetDataLen();
}

int CLogProto::DecodeCliDate(void *pHost)
{
	CliDate *pstOut = (CliDate*)pHost;

	if(-1 == m_oData.DelDword(pstOut->dwYear))
		return -1;

	if(-1 == m_oData.DelDword(pstOut->dwMonth))
		return -1;

	if(-1 == m_oData.DelDword(pstOut->dwDay))
		return -1;

	return sizeof(*pstOut);
}

int CLogProto::EncodeCLIPKG_LOG(void *pHost)
{
	CLIPKG_LOG *pstIn = (CLIPKG_LOG*)pHost;

	if(-1 == m_oData.AddByte(pstIn->byServerType))
		return -1;

	if(-1 == m_oData.AddByte(pstIn->byServerID))
		return -1;

	if(-1 == m_oData.AddByte(pstIn->byLogFileType))
		return -1;

	if(-1 == EncodeCliDate(&pstIn->stDate))
		return -1;

	if(-1 == m_oData.AddString(pstIn->szLog, sizeof(pstIn->szLog)))
		return -1;

	return m_oData.GetDataLen();
}

int CLogProto::DecodeCLIPKG_LOG(void *pHost)
{
	CLIPKG_LOG *pstOut = (CLIPKG_LOG*)pHost;

	if(-1 == m_oData.DelByte(pstOut->byServerType))
		return -1;

	if(-1 == m_oData.DelByte(pstOut->byServerID))
		return -1;

	if(-1 == m_oData.DelByte(pstOut->byLogFileType))
		return -1;

	if(-1 == DecodeCliDate(&pstOut->stDate))
		return -1;

	if(-1 == m_oData.DelString(pstOut->szLog, sizeof(pstOut->szLog)))
		return -1;

	return sizeof(*pstOut);
}

bool CLogProto::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLIID_LOG] = &CLogProto::EncodeCLIPKG_LOG;
	m_DecodeFuncArray[CLIID_LOG] = &CLogProto::DecodeCLIPKG_LOG;

	return true;
}


