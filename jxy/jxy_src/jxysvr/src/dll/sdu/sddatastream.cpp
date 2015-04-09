#include "sdnetutils.h"
#include <string.h>
#include "sddatastream.h"
#include "sdstring.h"

namespace SGDP
{

    CSDDataStream::CSDDataStream()
    {
        m_pBuf	= NULL;
        m_bufLen = 0;
        m_pos	= 0;
    }

    CSDDataStream::~CSDDataStream()
    {
    }

    void  CSDDataStream::Prepare(CHAR *pNetData, INT32 iSize)
    {
        m_pBuf	= pNetData;
        m_bufLen	= iSize;
        m_pos	= 0;
    }

    void  CSDDataStream::Reset()
    {
        m_pos = 0;
    }

    INT32  CSDDataStream::AddByte(UCHAR byByte)
    {
        if(m_pos + (INT32)sizeof(byByte) > m_bufLen)
            return -1;

        *(UCHAR*)(m_pBuf + m_pos) = byByte;
        m_pos += sizeof(byByte);

        return m_pos;
    }

    INT32  CSDDataStream::DelByte(UCHAR &byByte)
    {
        if(m_pos + (INT32)sizeof(byByte) > m_bufLen)
            return -1;

        byByte = *(UCHAR*)(m_pBuf + m_pos);
        m_pos += sizeof(byByte);

        return m_pos;
        return -1;
    }

    INT32  CSDDataStream::AddChar(CHAR chChar)
    {
        if(m_pos + (INT32)sizeof(chChar) > m_bufLen)
            return -1;

        *(CHAR*)(m_pBuf + m_pos) = chChar;
        m_pos += sizeof(chChar);

        return m_pos;
    }

    INT32  CSDDataStream::DelChar(CHAR &chChar)
    {
        if(m_pos + (INT32)sizeof(chChar) > m_bufLen)
            return -1;

        chChar = *(CHAR*)(m_pBuf + m_pos);
        m_pos += sizeof(chChar);

        return m_pos;
    }

    INT32  CSDDataStream::AddWord(UINT16 wWord)
    {
        if(m_pos + (INT32)sizeof(wWord) > m_bufLen)
            return -1;

        *(UINT16*)(m_pBuf + m_pos) = SDHtons(wWord);
        m_pos += sizeof(wWord);

        return m_pos;
    }

    INT32  CSDDataStream::DelWord(UINT16 &wWord)
    {
        if(m_pos + (INT32)sizeof(wWord) > m_bufLen)
            return -1;

        wWord = SDNtohs(*(UINT16*)(m_pBuf + m_pos));
        m_pos += sizeof(wWord);

        return m_pos;
    }

    INT32  CSDDataStream::AddShort(INT16 shShort)
    {
        if(m_pos + (INT32)sizeof(shShort) > m_bufLen)
            return -1;

        *(INT16*)(m_pBuf + m_pos) = SDHtons(shShort);
        m_pos += sizeof(shShort);

        return m_pos;
    }

    INT32  CSDDataStream::DelShort(INT16 &shShort)
    {
        if(m_pos + (INT32)sizeof(shShort) > m_bufLen)
            return -1;

        shShort = SDNtohs(*(INT16*)(m_pBuf + m_pos));
        m_pos += sizeof(shShort);

        return m_pos;
    }

    INT32  CSDDataStream::AddDword(UINT32 dwDword)
    {
        if(m_pos + (INT32)sizeof(dwDword) > m_bufLen)
            return -1;

        *(UINT32*)(m_pBuf + m_pos) = SDHtonl(dwDword);
        m_pos += sizeof(dwDword);

        return m_pos;
    }

    INT32  CSDDataStream::DelDword(UINT32 &dwDword)
    {
        if(m_pos + (INT32)sizeof(dwDword) > m_bufLen)
            return -1;

        dwDword = SDNtohl(*(UINT32*)(m_pBuf + m_pos));
        m_pos += sizeof(dwDword);

        return m_pos;
    }

    INT32  CSDDataStream::AddInt(INT32 iInt)
    {
        if(m_pos + (INT32)sizeof(iInt) > m_bufLen)
            return -1;

        *(INT32*)(m_pBuf + m_pos) = SDHtonl(iInt);
        m_pos += sizeof(iInt);

        return m_pos;
    }

    INT32  CSDDataStream::DelInt(INT32 &iInt)
    {
        if(m_pos + (INT32)sizeof(iInt) > m_bufLen)
            return -1;

        iInt = SDNtohl(*(INT32*)(m_pBuf + m_pos));
        m_pos += sizeof(iInt);

        return m_pos;
    }

    INT32  CSDDataStream::AddUint64(UINT64 qwUint64)
    {
        if(m_pos + (INT32)sizeof(qwUint64) > m_bufLen)
            return -1;

        *(UINT64*)(m_pBuf + m_pos) = SDHtonll(qwUint64);
        m_pos += sizeof(qwUint64);

        return m_pos;
    }

    INT32  CSDDataStream::DelUint64(UINT64& qwUint64)
    {
        if(m_pos + (INT32)sizeof(qwUint64) > m_bufLen)
            return -1;

        qwUint64 = SDNtohll(*(UINT64*)(m_pBuf + m_pos));
        m_pos += sizeof(qwUint64);

        return m_pos;
    }

    INT32  CSDDataStream::AddInt64(INT64 llInt64)
    {
        if(m_pos + (INT32)sizeof(llInt64) > m_bufLen)
            return -1;

        *(INT64*)(m_pBuf + m_pos) = SDHtonll(llInt64);
        m_pos += sizeof(llInt64);

        return m_pos;
    }

    INT32  CSDDataStream::DelInt64(INT64 &llInt64)
    {
        if(m_pos + (INT32)sizeof(llInt64) > m_bufLen)
            return -1;

        llInt64 = SDNtohll(*(INT64*)(m_pBuf + m_pos));
        m_pos += sizeof(llInt64);

        return m_pos;
    }

    INT32  CSDDataStream::AddFloat(FLOAT fFloat)//08-10-06 liujunhui add: 增加解析float
    {
        if(m_pos + (INT32)sizeof(fFloat) > m_bufLen)
            return -1;

        *(FLOAT*)(m_pBuf + m_pos) = fFloat;
        m_pos += sizeof(fFloat);

        return m_pos;
    }

    INT32  CSDDataStream::DelFloat(FLOAT &fFloat)//08-10-06 liujunhui add: 增加解析float
    {
        if(m_pos + (INT32)sizeof(fFloat) > m_bufLen)
            return -1;

        fFloat = (*(FLOAT*)(m_pBuf + m_pos));
        m_pos += sizeof(fFloat);

        return m_pos;
    }


    INT32  CSDDataStream::AddDouble(DOUBLE dbDouble )
    {
        if(m_pos + (INT32)sizeof(dbDouble) > m_bufLen)
            return -1;

        *(DOUBLE*)(m_pBuf + m_pos) = dbDouble;
        m_pos += sizeof(dbDouble);

        return m_pos;
    }

    INT32  CSDDataStream::DelDouble(DOUBLE &dbDouble )
    {
        if(m_pos + (INT32)sizeof(dbDouble) > m_bufLen)
            return -1;

        dbDouble = (*(DOUBLE*)(m_pBuf + m_pos));
        m_pos += sizeof(dbDouble);

        return m_pos;
    }

    INT32  CSDDataStream::AddString(const CHAR *pszString, INT32 iSize)
    {
        INT32 iLen = SDStrnlen(pszString, iSize-1);

        if(m_pos + (INT32)sizeof(UINT16) + iLen > m_bufLen)
            return -1;

        if(-1 == AddWord(iLen))
            return -1;

        memcpy(m_pBuf + m_pos, pszString, iLen);
        m_pos += iLen;

        return m_pos;
    }

    INT32  CSDDataStream::DelString(CHAR *pszOut, INT32 iSize)
    {
        UINT16 wLen = 0;
        if(-1 == DelWord(wLen))
            return -1;

        if(m_pos + (INT32)wLen > m_bufLen)
            return -1;

        if( INT32(wLen + 1) > iSize )
            return -1;

        memcpy(pszOut, m_pBuf+m_pos, wLen);
        pszOut[wLen] = '\0';
        m_pos += wLen;

        return m_pos;
    }

	INT32  CSDDataStream::AddWString(const WCHAR *wpszString, INT32 iSize)
	{
		INT32 iLen = SDWcsnlen(wpszString, iSize-1);

		if((INT32)(m_pos + (INT32)sizeof(UINT16) + iLen * sizeof(WCHAR)) > m_bufLen)
			return -1;

		if(-1 == AddDword(iLen*sizeof(WCHAR)))
			return -1;

		memcpy(m_pBuf + m_pos, wpszString, iLen* sizeof(WCHAR));
		m_pos += (iLen* sizeof(WCHAR));

		return m_pos;
	}

	INT32  CSDDataStream::DelWString(WCHAR *wpszOut, INT32 iSize)
	{
		UINT32 wLen = 0;
		if(-1 == DelDword(wLen))
			return -1;

		if(m_pos + (INT32)wLen > m_bufLen)
			return -1;

		if( INT32(wLen + 1) > iSize )
			return -1;

		memcpy(wpszOut, m_pBuf+m_pos, wLen);
		wpszOut[wLen] = '\0';
		m_pos += wLen;

		return m_pos;
	}




    INT32  CSDDataStream::AddBuf(const UCHAR *pbyBuf, INT32 iSize )
    {
        if (iSize < 0 || NULL == pbyBuf)
        {
            return -1;
        }

        if(m_pos + iSize > m_bufLen)
            return -1;

        memcpy(m_pBuf + m_pos, pbyBuf, iSize);
        m_pos += iSize;

        return m_pos;
    }

    INT32  CSDDataStream::DelBuf(UCHAR *pbyBuf, INT32 iSize )
    {
        if (NULL == pbyBuf)
        {
            return -1;
        }

        if(m_pos + iSize > m_bufLen)
            return -1;

        memcpy(pbyBuf, m_pBuf+m_pos, iSize);

        m_pos += iSize;

        return m_pos;
    }

};

