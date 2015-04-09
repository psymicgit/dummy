#include "netdata.h"
#include <sdnetutils.h>
#include <sdstring.h>
#include <sdtranslate.h>

using namespace SGDP;

namespace SGDP  { 
    static const int g_nCharset = 4;
	/*
    static CHAR* g_szCharset[] =
    {
        "ASCII",      // 0
        "GBK",        // 1
        "UTF-8",      // 2
        "UTF-16LE"    // 3        
    };
	*/
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

    INT32 CNetData::AddByte(UCHAR byByte)
    {
	    if(m_iPos + (INT32)sizeof(byByte) > m_iSize)
		    return -1;

	    *(UCHAR*)(m_pBuf + m_iPos) = byByte;
	    m_iPos += sizeof(byByte);

	    return m_iPos; 
    }

    INT32 CNetData::DelByte(UCHAR &byByte)
    {
	    if(m_iPos + (INT32)sizeof(byByte) > m_iSize)
		    return -1;

	    byByte = *(UCHAR*)(m_pBuf + m_iPos);
	    m_iPos += sizeof(byByte);

	    return m_iPos;
    }

    INT32 CNetData::AddChar(CHAR chChar)
    {
	    if(m_iPos + (INT32)sizeof(chChar) > m_iSize)
		    return -1;

	    *(CHAR*)(m_pBuf + m_iPos) = chChar;
	    m_iPos += sizeof(chChar);

	    return m_iPos; 
    }

    INT32 CNetData::DelChar(CHAR &chChar)
    {
	    if(m_iPos + (INT32)sizeof(chChar) > m_iSize)
		    return -1;

	    chChar = *(CHAR*)(m_pBuf + m_iPos);
	    m_iPos += sizeof(chChar);

	    return m_iPos; 
    }

    INT32 CNetData::AddWord(UINT16 wWord)
    {
	    if(m_iPos + (INT32)sizeof(wWord) > m_iSize)
		    return -1;

	    *(UINT16*)(m_pBuf + m_iPos) = SDNtohs(wWord);
	    m_iPos += sizeof(wWord);

	    return m_iPos; 
    }

    INT32 CNetData::DelWord(UINT16 &wWord)
    {
	    if(m_iPos + (INT32)sizeof(wWord) > m_iSize)
		    return -1;

	    wWord = SDNtohs(*(UINT16*)(m_pBuf + m_iPos));
	    m_iPos += sizeof(wWord);

	    return m_iPos; 
    }

    INT32 CNetData::AddShort(INT16 shShort)
    {
	    if(m_iPos + (INT32)sizeof(shShort) > m_iSize)
		    return -1;

	    *(INT16 *)(m_pBuf + m_iPos) = SDNtohs(shShort);
	    m_iPos += sizeof(shShort);

	    return m_iPos;
    }

    INT32 CNetData::DelShort(INT16 &shShort)
    {
	    if(m_iPos + (INT32)sizeof(shShort) > m_iSize)
		    return -1;

	    shShort = SDNtohs(*(INT16 *)(m_pBuf + m_iPos));
	    m_iPos += sizeof(shShort);

	    return m_iPos;
    }

    INT32 CNetData::AddDword(UINT32 dwDword)
    {
	    if(m_iPos + (INT32)sizeof(dwDword) > m_iSize)
		    return -1;

	    *(UINT32*)(m_pBuf + m_iPos) = SDHtonl(dwDword);
	    m_iPos += sizeof(dwDword);

	    return m_iPos;
    }

    INT32 CNetData::DelDword(UINT32 &dwDword)
    {
	    if(m_iPos + (INT32)sizeof(dwDword) > m_iSize)
		    return -1;

	    dwDword = SDNtohl(*(UINT32*)(m_pBuf + m_iPos));
	    m_iPos += sizeof(dwDword);

	    return m_iPos;
    }

    INT32 CNetData::AddInt(INT32 iInt)
    {
	    if(m_iPos + (INT32)sizeof(iInt) > m_iSize)
		    return -1;

	    *(INT32*)(m_pBuf + m_iPos) = SDHtonl(iInt);
	    m_iPos += sizeof(iInt);

	    return m_iPos;
    }

    INT32 CNetData::DelInt(INT32 &iInt)
    {
	    if(m_iPos + (INT32)sizeof(iInt) > m_iSize)
		    return -1;

	    iInt = SDNtohl(*(INT32*)(m_pBuf + m_iPos));
	    m_iPos += sizeof(iInt);

	    return m_iPos;
    }

    INT32 CNetData::AddUint64(UINT64 qwUint64)
    {
	    if(m_iPos + (INT32)sizeof(qwUint64) > m_iSize)
		    return -1;

	    *(UINT64*)(m_pBuf + m_iPos) = SDHtonll(qwUint64);
	    m_iPos += sizeof(qwUint64);

	    return m_iPos;
    }

    INT32 CNetData::DelUint64(UINT64& qwUint64)
    {
	    if(m_iPos + (INT32)sizeof(qwUint64) > m_iSize)
		    return -1;

	    qwUint64 = SDNtohll(*(UINT64*)(m_pBuf + m_iPos));
	    m_iPos += sizeof(qwUint64);

	    return m_iPos;
    }

    INT32 CNetData::AddInt64(INT64 llInt64)
    {
	    if(m_iPos + (INT32)sizeof(llInt64) > m_iSize)
		    return -1;

	    *(INT64*)(m_pBuf + m_iPos) = SDHtonll(llInt64);
	    m_iPos += sizeof(llInt64);

	    return m_iPos;
    }

    INT32 CNetData::DelInt64(INT64 &llInt64)
    {
	    if(m_iPos + (INT32)sizeof(llInt64) > m_iSize)
		    return -1;

	    llInt64 = SDNtohll(*(INT64*)(m_pBuf + m_iPos));
	    m_iPos += sizeof(llInt64);

	    return m_iPos;
    }

    INT32 CNetData::AddFloat(FLOAT fFloat)//08-10-06 liujunhui add: 增加解析float
    {
	    if(m_iPos + (INT32)sizeof(fFloat) > m_iSize)
		    return -1;

	    *(FLOAT*)(m_pBuf + m_iPos) = fFloat;
	    m_iPos += sizeof(fFloat);

	    return m_iPos;
    }

    INT32 CNetData::DelFloat(FLOAT &fFloat)//08-10-06 liujunhui add: 增加解析float
    {
	    if(m_iPos + (INT32)sizeof(fFloat) > m_iSize)
		    return -1;

	    fFloat = (*(FLOAT*)(m_pBuf + m_iPos));
	    m_iPos += sizeof(fFloat);

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
                iResult = SDIconvCovert(g_szCharset[m_iCharsetHost], 
                    g_szCharset[m_iCharsetNet], const_cast<char*>(pszString), iSize, szBuffer, iLen);
                if (0 != iResult)
                {
                    return -1;
                }
                if(m_iPos + (INT32)sizeof(UINT16) + iLen > m_iSize)
                {
                    return -1;
                }
	            if(-1 == AddWord((UINT16)iLen))
                {
		            return -1;
                }
                memcpy(m_pBuf + m_iPos, szBuffer, iLen);
	            m_iPos += iLen;
                return m_iPos;
            }
        }

        iLen = Strnlen(pszString, iSize-1);

        if(m_iPos + (INT32)sizeof(UINT16) + iLen > m_iSize)
	        return -1;

        if(-1 == AddWord((UINT16)iLen))
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
                iResult = SDIconvCovert(g_szCharset[m_iCharsetHost], 
                    g_szCharset[m_iCharsetNet], (CHAR*)const_cast<WCHAR*>(pszString), SDWcslen(pszString) * sizeof(WCHAR), szBuffer, iLen);
                if (0 != iResult)
                {
                    return -1;
                }
                if(m_iPos + (INT32)sizeof(UINT16) + iLen > m_iSize)
                {
                    return -1;
                }
                if(-1 == AddWord(iLen))
                {
                    return -1;
                }
                memcpy(m_pBuf + m_iPos, szBuffer, iLen);
	            m_iPos += iLen;
                return m_iPos;
            }
        }
        iLen = wcslen(pszString);
        if(m_iPos + (INT32)sizeof(UINT16) + iLen * sizeof(WCHAR) > m_iSize)
        {
            return -1;
        }
        if(-1 == AddWord(iLen* sizeof(WCHAR)))
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

    INT32 CNetData::DelString(CHAR *pszOut, INT32 iSize)
    {
	    UINT16 wLen = 0;
        size_t iLen = 0;

	    if(-1 == DelWord(wLen))
		    return -1;

	    if(m_iPos + (INT32)wLen > m_iSize)
		    return -1;

	    if( INT32(wLen + 1) > iSize )
		    return -1;

        if ((m_iCharsetNet >= 0) || (m_iCharsetHost >= 0))
        {
            if (m_iCharsetNet != m_iCharsetHost)
            {
                INT32 iResult = 0;
                iLen = iSize;
                iResult = SDIconvCovert(g_szCharset[m_iCharsetNet],
                    g_szCharset[m_iCharsetHost], m_pBuf+m_iPos, wLen, pszOut, iLen);

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
	    pszOut[wLen] = '\0';
	    m_iPos += wLen;

	    return m_iPos;
    }

    INT32 CNetData::DelWString(WCHAR *pszOut, INT32 iSize)
    {
        UINT16 wLen = 0;
        size_t iLen = 0;

        if(-1 == DelWord(wLen))
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
                iResult = SDIconvCovert(g_szCharset[m_iCharsetNet],
                    g_szCharset[m_iCharsetHost], (CHAR*)(m_pBuf+m_iPos), wLen, (CHAR*)pszOut, iLen);
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

}

