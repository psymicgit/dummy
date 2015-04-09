#include "netdata.h"
//#include "MGNetDataDecoder.h"
#include "winsock2.h"
#include "sdtranslate.h"

#ifdef __ARMCC_VERSION
#define PACKED __packed
#else
#define PACKED
#endif

PACKED INT16* pINT16;
PACKED UINT16* pUINT16;
PACKED INT32* pINT32;
PACKED UINT32* pUINT32;
PACKED INT64* pINT64;
PACKED UINT64* pUINT64;
PACKED FLOAT* pFLOAT;

using namespace SGDP;

UINT64  htonll(UINT64 number)
{
	return ( htonl( UINT32((number >> 32) & 0xFFFFFFFF)) |
		(UINT64(htonl(UINT32(number & 0xFFFFFFFF))) << 32));
}

UINT64  ntohll(UINT64 number)
{
	return ( ntohl( UINT32((number >> 32) & 0xFFFFFFFF) ) |
		(UINT64 (ntohl(UINT32(number & 0xFFFFFFFF)))  << 32));
}


static const int g_nCharset = 4;
static CHAR* g_szCharset[] =
{
	"ASCII",      // 0
	"GBK",        // 1
	"UTF-16LE",    // 2
	"UTF-8"     // 3
};

CNetData::CNetData()
{
	m_pBuf	= NULL;
	m_iSize = 0;
	m_iPos	= 0;
	m_iType = 0;
	m_iCharsetNet = -1;
	m_iCharsetHost = -1;
}

CNetData::~CNetData()
{
}

VOID  CNetData::Prepare(CHAR *pNetData, INT32 iSize, INT32 iType, INT32 iCharsetNet, INT32 iCharsetHost)
{
	m_pBuf	       = pNetData;
	m_iSize	       = iSize;
	m_iPos	       = 0;
	m_iType        = iType;
	m_iCharsetNet  = iCharsetNet;
	m_iCharsetHost = iCharsetHost;
}

VOID  CNetData::Prepare(CHAR *pNetData, INT32 iSize)
{
	m_pBuf	       = pNetData;
	m_iSize	       = iSize;
	m_iPos	       = 0;
}

VOID  CNetData::Reset()
{
	m_iPos = 0;
}

INT32 CNetData::AddByte(UCHAR byByte, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;
	switch(wLen)
	{
	case 1:
		{		
			*(UCHAR*)(m_pBuf + m_iPos) = byByte;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons(byByte);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl(byByte);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(byByte);
		}
		break;
	default:
		{		
			*(UCHAR*)(m_pBuf + m_iPos) = byByte;
		}
		break;
	}		
	m_iPos += wLen;

	return m_iPos; 
}

INT32 CNetData::DelByte(UCHAR &byByte, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			byByte = *(UCHAR*)(m_pBuf + m_iPos);
		}
		break;
	case 2:
		{		
			UINT16 wTmp = 0;
			memcpy(&wTmp, m_pBuf + m_iPos, sizeof(wTmp));
			wTmp = ntohs(wTmp);
			byByte = (UCHAR)wTmp;
		}
		break;
	case 4:
		{
			UINT32 dwTmp = 0;
			memcpy(&dwTmp, m_pBuf + m_iPos, sizeof(dwTmp));
			dwTmp = ntohl(dwTmp);
			byByte = (UCHAR)dwTmp;
		}
		break;
	case 8:
		{		
			UINT64 qwTmp = 0;
			memcpy(&qwTmp, m_pBuf + m_iPos, sizeof(qwTmp));
			qwTmp = ntohll(qwTmp);
			byByte = (UCHAR)qwTmp;
		}
		break;
	default:
		{		
			byByte = *(UCHAR*)(m_pBuf + m_iPos);
		}
		break;
	}	

	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::AddChar(CHAR chChar, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(CHAR*)(m_pBuf + m_iPos) = chChar;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons(chChar);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl(chChar);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(chChar);
		}
		break;
	default:
		{		
			*(CHAR*)(m_pBuf + m_iPos) = chChar;
		}
		break;
	}		

	m_iPos += wLen;

	return m_iPos; 
}

INT32 CNetData::DelChar(CHAR &chChar, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			chChar = *(CHAR*)(m_pBuf + m_iPos);
		}
		break;
	case 2:
		{		
			UINT16 wTmp = 0;
			memcpy(&wTmp, m_pBuf + m_iPos, sizeof(wTmp));
			wTmp = ntohs(wTmp);
			chChar = (CHAR)wTmp;
		}
		break;
	case 4:
		{		
			UINT32 dwTmp = 0;
			memcpy(&dwTmp, m_pBuf + m_iPos, sizeof(dwTmp));
			dwTmp = ntohl(dwTmp);
			chChar = (CHAR)dwTmp;
		}
		break;
	case 8:
		{		
			UINT64 qwTmp = 0;
			memcpy(&qwTmp, m_pBuf + m_iPos, sizeof(qwTmp));
			qwTmp = ntohll(qwTmp);
			chChar = (CHAR)qwTmp;
		}
		break;
	default:
		{		
			chChar = *(CHAR*)(m_pBuf + m_iPos);
		}
		break;
	}	
	m_iPos += wLen;

	return m_iPos; 
}

INT32 CNetData::AddWord(UINT16 wWord, UINT16 wLen)
{
	if(m_iPos + (INT32)sizeof(wWord) > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(UINT8*)(m_pBuf + m_iPos) = (UINT8)wWord;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons(wWord);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl(wWord);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(wWord);
		}
		break;
	default:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons(wWord);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos; 
}

//
//INT32 CNetData::DelWord(UINT16 &wWord)
//{
//    if(m_iPos + (INT32)sizeof(wWord) > m_iSize)
//	    return -1;
//
//	pUINT16 = (UINT16*)(m_pBuf + m_iPos);
//    wWord = ntohs(*pUINT16);
//    m_iPos += sizeof(wWord);
//
//    return m_iPos; 
//}

INT32 CNetData::DelWord(UINT16 &wWord, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			UINT8 byTmp = *(UCHAR*)(m_pBuf + m_iPos);
			wWord = byTmp;
		}
		break;
	case 2:
		{		
			memcpy(&wWord, m_pBuf + m_iPos, sizeof(wWord));
			wWord = ntohs(wWord);
		}
		break;
	case 4:
		{		
			UINT32 dwTmp = 0;
			memcpy(&dwTmp, m_pBuf + m_iPos, sizeof(dwTmp));
			dwTmp = ntohl(dwTmp);
			wWord = (UINT16)dwTmp;
		}
		break;
	case 8:
		{		
			UINT64 qwTmp = 0;
			memcpy(&qwTmp, m_pBuf + m_iPos, sizeof(qwTmp));
			qwTmp = ntohll(qwTmp);
			wWord = (UINT16)qwTmp;
		}
		break;
	default:
		{		
			memcpy(&wWord, m_pBuf + m_iPos, sizeof(wWord));
			wWord = ntohs(wWord);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos; 
}

INT32 CNetData::AddShort(INT16 shShort, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(UINT8*)(m_pBuf + m_iPos) = (UINT8)shShort;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons(shShort);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl(shShort);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(shShort);
		}
		break;
	default:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons(shShort);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}
//
//INT32 CNetData::DelShort(INT16 &shShort)
//{
//    if(m_iPos + (INT32)sizeof(shShort) > m_iSize)
//	    return -1;
//
//	pINT16 = (INT16 *)(m_pBuf + m_iPos);
//    shShort = ntohs(*pINT16);
//    m_iPos += sizeof(shShort);
//
//    return m_iPos;
//}

INT32 CNetData::DelShort(INT16 &shShort, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			UINT8 byTmp = *(UCHAR*)(m_pBuf + m_iPos);
			shShort = byTmp;
		}
		break;
	case 2:
		{		
			memcpy(&shShort, m_pBuf + m_iPos, sizeof(shShort));
			shShort = ntohs(shShort);
		}
		break;
	case 4:
		{		
			UINT32 dwTmp = 0;
			memcpy(&dwTmp, m_pBuf + m_iPos, sizeof(dwTmp));
			dwTmp = ntohl(dwTmp);
			shShort = (INT16)dwTmp;
		}
		break;
	case 8:
		{		
			UINT64 qwTmp = 0;
			memcpy(&qwTmp, m_pBuf + m_iPos, sizeof(qwTmp));
			qwTmp = ntohll(qwTmp);
			shShort = (INT16)qwTmp;
		}
		break;
	default:
		{		
			memcpy(&shShort, m_pBuf + m_iPos, sizeof(shShort));
			shShort = ntohs(shShort);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::AddDword(UINT32 dwDword, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(CHAR*)(m_pBuf + m_iPos) = (UINT8)dwDword;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons((UINT16)dwDword);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl(dwDword);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(dwDword);
		}
		break;
	default:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl(dwDword);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

//INT32 CNetData::DelDword(UINT32 &dwDword)
//{
//    if(m_iPos + (INT32)sizeof(dwDword) > m_iSize)
//	    return -1;
//
//	pUINT32 = (UINT32*)(m_pBuf + m_iPos);
//    dwDword = ntohl(*pUINT32);
//    m_iPos += sizeof(dwDword);
//
//    return m_iPos;
//}

INT32 CNetData::DelDword(UINT32 &dwDword, UINT16 wLen)
{
	if(m_iPos + (INT32)sizeof(dwDword) > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			dwDword = *(UCHAR*)(m_pBuf + m_iPos);
		}
		break;
	case 2:
		{		
			UINT16 wTmp = 0;
			memcpy(&wTmp, m_pBuf + m_iPos, sizeof(wTmp));
			wTmp = ntohs(wTmp);
			dwDword = (UINT32)wTmp;
		}
		break;
	case 4:
		{		
			memcpy(&dwDword, m_pBuf + m_iPos, sizeof(dwDword));
			dwDword = ntohl(dwDword);
		}
		break;
	case 8:
		{		
			UINT64 qwTmp = 0;
			memcpy(&qwTmp, m_pBuf + m_iPos, sizeof(qwTmp));
			qwTmp = ntohll(qwTmp);
			dwDword = (UINT32)qwTmp;
		}
		break;
	default:
		{		
			memcpy(&dwDword, m_pBuf + m_iPos, sizeof(dwDword));
			dwDword = ntohl(dwDword);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::AddInt(INT32 iInt, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(CHAR*)(m_pBuf + m_iPos) = (UINT8)iInt;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons((UINT16)iInt);
		}
		break;
	case 4:
		{		
			*(INT32*)(m_pBuf + m_iPos) = htonl(iInt);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(iInt);
		}
		break;
	default:
		{		
			*(INT32*)(m_pBuf + m_iPos) = htonl(iInt);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

//INT32 CNetData::DelInt(INT32 &iInt)
//{
//	if(m_iPos + (INT32)sizeof(iInt) > m_iSize)
//		return -1;
//
//	pINT32 = (INT32*)(m_pBuf + m_iPos);
//	iInt = ntohl(*pINT32);
//	m_iPos += sizeof(iInt);
//
//	return m_iPos;
//}

INT32 CNetData::DelInt(INT32 &iInt, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			iInt = *(UCHAR*)(m_pBuf + m_iPos);
		}
		break;
	case 2:
		{		
			UINT16 wTmp = 0;
			memcpy(&wTmp, m_pBuf + m_iPos, sizeof((UINT16)wTmp));
			wTmp = ntohs(wTmp);
			iInt = (UINT32)wTmp;
		}
		break;
	case 4:
		{		
			memcpy(&iInt, m_pBuf + m_iPos, sizeof(iInt));
			iInt = ntohl(iInt);
		}
		break;
	case 8:
		{		
			UINT64 qwTmp = 0;
			memcpy(&qwTmp, m_pBuf + m_iPos, sizeof(qwTmp));
			qwTmp = ntohll(qwTmp);
			iInt = (UINT32)qwTmp;
		}
		break;
	default:
		{		
			memcpy(&iInt, m_pBuf + m_iPos, sizeof(iInt));
			iInt = ntohl(iInt);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}


INT32 CNetData::AddUint64(UINT64 qwUint64, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(CHAR*)(m_pBuf + m_iPos) = (UINT8)qwUint64;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons((UINT16)qwUint64);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl((UINT32)qwUint64);
		}
		break;
	case 8:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(qwUint64);
		}
		break;
	default:
		{		
			*(UINT64*)(m_pBuf + m_iPos) = htonll(qwUint64);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

//INT32 CNetData::DelUint64(UINT64& qwUint64)
//{
//    if(m_iPos + (INT32)sizeof(qwUint64) > m_iSize)
//	    return -1;
//
//	pUINT64 = (UINT64*)(m_pBuf + m_iPos);
//    qwUint64 = ntohll(*pUINT64);
//    m_iPos += sizeof(qwUint64);
//
//    return m_iPos;
//}

INT32 CNetData::DelUint64(UINT64& qwUint64, UINT16 wLen)
{
	if(m_iPos + (INT32)sizeof(qwUint64) > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			UINT8 byTmp = *(UCHAR*)(m_pBuf + m_iPos);
			qwUint64 = (UINT64)byTmp;
		}
		break;
	case 2:
		{		
			UINT16 wTmp = 0;
			memcpy(&wTmp, m_pBuf + m_iPos, sizeof(wTmp));
			wTmp = ntohs(wTmp);
			qwUint64 = (UINT64)wTmp;
		}
		break;
	case 4:
		{		
			UINT32 dwTmp = 0;
			memcpy(&dwTmp, m_pBuf + m_iPos, sizeof(dwTmp));
			dwTmp = ntohl(dwTmp);
			qwUint64 = (UINT64)dwTmp;
		}
		break;
	case 8:
		{		
			memcpy(&qwUint64, m_pBuf + m_iPos, sizeof(qwUint64));
			qwUint64 = ntohll(qwUint64);
		}
		break;
	default:
		{		
			memcpy(&qwUint64, m_pBuf + m_iPos, sizeof(qwUint64));
			qwUint64 = ntohll(qwUint64);
		}
		break;
	}


	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::AddInt64(INT64 llInt64, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			*(CHAR*)(m_pBuf + m_iPos) = (UINT8)llInt64;
		}
		break;
	case 2:
		{		
			*(UINT16*)(m_pBuf + m_iPos) = htons((UINT16)llInt64);
		}
		break;
	case 4:
		{		
			*(UINT32*)(m_pBuf + m_iPos) = htonl((UINT32)llInt64);
		}
		break;
	case 8:
		{		
			*(INT64*)(m_pBuf + m_iPos) = htonll(llInt64);
		}
		break;
	default:
		{		
			*(INT64*)(m_pBuf + m_iPos) = htonll(llInt64);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

//INT32 CNetData::DelInt64(INT64 &llInt64)
//{
//    if(m_iPos + (INT32)sizeof(llInt64) > m_iSize)
//	    return -1;
//
//	pINT64 = (INT64*)(m_pBuf + m_iPos);
//    llInt64 = ntohll(*pINT64);
//    m_iPos += sizeof(llInt64);
//
//    return m_iPos;
//}

INT32 CNetData::DelInt64(INT64 &llInt64, UINT16 wLen)
{
	if(m_iPos + (INT32)sizeof(llInt64) > m_iSize)
		return -1;

	switch(wLen)
	{
	case 1:
		{		
			UINT8 byTmp = *(UCHAR*)(m_pBuf + m_iPos);
			llInt64 = (INT64)byTmp;
		}
		break;
	case 2:
		{		
			UINT16 wTmp = 0;
			memcpy(&wTmp, m_pBuf + m_iPos, sizeof(wTmp));
			wTmp = ntohs(wTmp);
			llInt64 = (INT64)wTmp;
		}
		break;
	case 4:
		{		
			UINT32 dwTmp = 0;
			memcpy(&dwTmp, m_pBuf + m_iPos, sizeof(dwTmp));
			dwTmp = ntohl(dwTmp);
			llInt64 = (INT64)dwTmp;
		}
		break;
	case 8:
		{		
			memcpy(&llInt64, m_pBuf + m_iPos, sizeof(llInt64));
			llInt64 = ntohll(llInt64);
		}
		break;
	default:
		{		
			memcpy(&llInt64, m_pBuf + m_iPos, sizeof(llInt64));
			llInt64 = ntohll(llInt64);
		}
		break;
	}

	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::AddFloat(FLOAT fFloat, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	*(FLOAT*)(m_pBuf + m_iPos) = fFloat;
	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::DelFloat(FLOAT &fFloat, UINT16 wLen)
{
	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	pFLOAT = (FLOAT*)(m_pBuf + m_iPos);
	fFloat = (*pFLOAT);
	m_iPos += wLen;

	return m_iPos;
}

INT32 CNetData::AddString(const CHAR *pszString, INT32 iSize)
{
	static const INT32 BUFLEN = 4096;
	size_t iLen = 0;

	if ((m_iCharsetNet >= 0) || (m_iCharsetHost >= 0))
	{
		if (m_iCharsetNet != m_iCharsetHost)
		{
			static char szBuffer[BUFLEN];
			INT32 iResult = 0;
			iLen = BUFLEN;
			iResult = SDIconvCovert(g_szCharset[m_iCharsetNet], 
				g_szCharset[m_iCharsetHost], const_cast<char*>(pszString), iSize, szBuffer, iLen);
			if (0 != iResult)
			{
				return -1;
			}
			if((INT32)(m_iPos + (INT32)sizeof(UINT16) + iLen) > m_iSize)
			{
				return -1;
			}
			if(-1 == AddWord((UINT16)iLen, 2))
			{
				return -1;
			}
			memcpy(m_pBuf + m_iPos, szBuffer, iLen);
			m_iPos += iLen;
			return m_iPos;
		}
	}


	iLen = Strnlen(pszString, iSize-1);

	if((INT32)(m_iPos + (INT32)sizeof(UINT16) + iLen) > m_iSize)
		return -1;

	if(-1 == AddWord((UINT16)iLen, 2))
		return -1;

	memcpy(m_pBuf + m_iPos, pszString, iLen);
	m_iPos += iLen;

	return m_iPos;
}

INT32 CNetData::AddWString(const WCHAR *pszString, INT32 iSize)
{
	static const INT32 BUFLEN = 4096;
	size_t iLen = 0;
	if ((m_iCharsetNet >= 0) || (m_iCharsetHost >= 0))
	{
		if (m_iCharsetNet != m_iCharsetHost)
		{
			static char szBuffer[BUFLEN];
			INT32 iResult = 0;
			iLen = BUFLEN;
			iResult = SDIconvCovert(g_szCharset[m_iCharsetNet], 
				g_szCharset[m_iCharsetHost], (CHAR*)const_cast<WCHAR*>(pszString), SDWcslen(pszString) * sizeof(WCHAR), szBuffer, iLen);
			if (0 != iResult)
			{
				return -1;
			}
			if((INT32)(m_iPos + (INT32)sizeof(UINT16) + iLen) > m_iSize)
			{
				return -1;
			}
			if(-1 == AddWord(iLen, 2))
			{
				return -1;
			}
			memcpy(m_pBuf + m_iPos, szBuffer, iLen);
			m_iPos += iLen;
			return m_iPos;
		}
	}
	iLen = wcslen(pszString);
	if((INT32)(m_iPos + (INT32)sizeof(UINT16) + iLen * sizeof(WCHAR)) > m_iSize)
	{
		return -1;
	}
	if(-1 == AddWord(iLen* sizeof(WCHAR), 2))
	{
		return -1;
	}
	memcpy(m_pBuf + m_iPos, pszString, iLen* sizeof(WCHAR));
	m_iPos += iLen * sizeof(WCHAR);
	return m_iPos;
}

INT32 CNetData::AddTString(const TCHAR *pszString, INT32 iSize)
{
#ifdef UNICODE
	return AddWString(pszString,iSize);
#else
	return AddString(pszString,iSize);
#endif       
}

INT32 CNetData::DelWString(WCHAR *pszOut, INT32 iSize)
{
	UINT16 wLen = 0;
	size_t iLen = 0;

	if(-1 == DelWord(wLen, 2))
		return -1;

	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	if( INT32(wLen + 1) > iSize )
		return -1;


	if ((m_iCharsetNet >= 0) || (m_iCharsetHost >= 0))
	{
		if (m_iCharsetNet != m_iCharsetHost)
		{
			iLen = iSize * sizeof(WCHAR);
			INT32 iResult = 0;
			iResult = SDIconvCovert(g_szCharset[m_iCharsetHost],
				g_szCharset[m_iCharsetNet], (CHAR*)(m_pBuf+m_iPos), wLen, (CHAR*)pszOut, iLen);
			if (0 != iResult)
			{
				return -1;
			}
			pszOut[iLen / sizeof(WCHAR)] = L'\0';
			m_iPos += wLen;
			return m_iPos;
		}
	}

	memcpy(pszOut, m_pBuf+m_iPos, wLen);
	pszOut[wLen / sizeof(WCHAR)] = L'\0';
	m_iPos += wLen;
	return m_iPos;
}

INT32 CNetData::DelString(CHAR *pszOut, INT32 iSize)
{
	UINT16 wLen = 0;
	size_t iLen = 0;

	if(-1 == DelWord(wLen, 2))
		return -1;

	if(m_iPos + (INT32)wLen > m_iSize)
		return -1;

	/*
	if( INT32(wLen + 1) > iSize )
	return -1;
	*/
	INT32 wPos = wLen;
	if( INT32(wLen) > iSize )
	{	
		wLen = 	iSize;
	}

	if ((m_iCharsetNet >= 0) || (m_iCharsetHost >= 0))
	{
		if (m_iCharsetNet != m_iCharsetHost)
		{
			INT32 iResult = 0;
			iLen = iSize;
			iResult = SDIconvCovert(g_szCharset[m_iCharsetHost],
				g_szCharset[m_iCharsetNet], m_pBuf+m_iPos, wLen, pszOut, iLen);

			if (0 != iResult)
			{
				return -1;
			}
			pszOut[wLen] = '\0';
			m_iPos += wLen;
			return m_iPos;
		}		
	}


	memcpy(pszOut, m_pBuf+m_iPos, wLen);
	if((wLen == iSize) && (0 != wLen))
	{
		pszOut[wLen-1] = '\0';
	}
	else
	{
		pszOut[wLen] = '\0';
	}
	m_iPos += wPos;

	return m_iPos;
}


INT32 CNetData::DelTString(TCHAR *pszOut, INT32 iSize)
{
#ifdef UNICODE
	return DelWString(pszOut,iSize);
#else
	return DelString(pszOut,iSize);
#endif   
}

INT32 CNetData::Strnlen(const CHAR *pszString, INT32 iSize)
{
	INT32 i;
	const CHAR *ptr = pszString;

	for(i = 0; i < iSize; i++)
	{
		if('\0' == *ptr)
		{
			return i;
		}
		ptr++;
	}

	return iSize;
}

INT32 CNetData::AddBuf( const UCHAR* pbyBuf, INT32 iSize )
{
	if (iSize < 0 || NULL == pbyBuf)
	{
		return -1;
	}

	if(m_iPos + iSize > m_iSize)
		return -1;

	memcpy(m_pBuf + m_iPos, pbyBuf, iSize);
	m_iPos += iSize;

	return m_iPos;
}

INT32 CNetData::DelBuf( UCHAR* pbyBuf, INT32 iSize )
{
	if (NULL == pbyBuf)
	{
		return -1;
	}

	if(m_iPos + iSize > m_iSize)
		return -1;

	memcpy(pbyBuf, m_pBuf+m_iPos, iSize);

	m_iPos += iSize;

	return m_iPos;
}

INT32 CNetData::PassLen(UINT32 dwLen)
{
	if(m_iPos + (INT32)dwLen > m_iSize)
		return -1;

	m_iPos += dwLen;
	return m_iPos;
}

VOID  CSDProtocol::SetName(const std::string strName)
{
	m_strName = strName;
}

std::string CSDProtocol::GetName()
{
	return m_strName;
}

UINT32 CSDProtocol::GetMaxMsgId()
{
	return m_nMaxMsgId;
}

VOID  CSDProtocol::SetMaxMsgId(const UINT32 nMaxMsgId)
{
	m_nMaxMsgId = nMaxMsgId;
}

UINT32 CSDProtocol::GetMinMsgId()
{
	return m_nMinMsgId;
}

VOID  CSDProtocol::SetMinMsgId(const UINT32 nMinMsgId)
{
	m_nMinMsgId = nMinMsgId;
}

vector<INT32>* CSDProtocol::GetMsgList()
{
	return &m_vecMsgList;
}


