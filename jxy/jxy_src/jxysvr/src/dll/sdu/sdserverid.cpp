
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdserverid.h"
#include "sdstring.h"

namespace SGDP
{

    BOOL _IsNum(const CHAR* str)
    {
        INT32 nLen = (INT32)strlen(str);
        if (0 == nLen)
        {
            return FALSE;
        }

        for (INT32 n = 0; n < nLen; n++)
        {
            if (str[n] > ('9') || str[n] < ('0'))
            {
                return FALSE;
            }
        }
        return TRUE;
    }

    static void SDCalcMasks( UINT32 maskLen[] , UINT32 submask[], UINT32 dwMask)
    {
        for (INT32 i = 0; i < 4; i ++)
        {
            maskLen[i] = (dwMask >> i*8) &0xFF;
            submask[i] = 1;
            submask[i] = (submask[i] << (maskLen[i])) -1;
        }
    }

//////////////////////////////////////////////////////////////////////////
// exported functions

    tstring  SDServerIDUtoa(UINT32 dwServerID, UINT32 dwMask /*=0x08080808*/)
    {
		/*
        TCHAR pszServerID[128];
        INT32 dwBuffLen = sizeof(pszServerID);

        UINT32 maskLen[4];
        UINT32 submask[4];

        SDCalcMasks(maskLen,submask,dwMask);

        UINT32 szBytes[4] = {0};

		szBytes[0] = (dwServerID >> (maskLen[0]+ maskLen[1] + maskLen[2])) & submask[3];
		szBytes[1] = (dwServerID >> (maskLen[0]+ maskLen[1])) & submask[2];
		szBytes[2] = (dwServerID >> (maskLen[0] )) & submask[1];
		szBytes[3] = (dwServerID ) & submask[0];


		_SDTsnprintf(pszServerID, dwBuffLen-1, _SDT("%d-%d-%d-%d"),
		szBytes[3],
		szBytes[2],
		szBytes[1],
		szBytes[0]);

		pszServerID[dwBuffLen-1] = _SDT('\0');

		return pszServerID;
		*/
		
		UINT16	wAreaID = (dwServerID & 0xFFFF0000) >> 16;
		UINT8	byServerType = (dwServerID & 0x0000FF00) >> 8;
		UINT8	byServerIdx = (dwServerID & 0x000000FF);
		TCHAR pszServerID[128] = {0};
		INT32 dwBuffLen = sizeof(pszServerID);
		sprintf(pszServerID, "%u-%u-%u",
			wAreaID,
			byServerType,
			byServerIdx);

		return pszServerID;
    }

    UINT32  SDServerIDAtou(const CHAR* pszServerID,UINT32 dwMask )
    {
		/*
        CHAR szBuf[32]  = {0};
        CHAR* pBeg      = szBuf;
        CHAR* pPos      = NULL;
        INT32 nByteVal  = 0;
        UINT32 dwVal    = 0;

        CHAR ch         = ('-');

        INT32 values[4];
        UINT32 maskLen[4];
        UINT32 submask[4];
        SDCalcMasks(maskLen,submask,dwMask);

        strncpy(szBuf, pszServerID, sizeof(szBuf)-1);
        INT32 i =3;
        pPos = strchr(szBuf,ch);
        while (pPos != NULL)
        {
            *pPos = '\0';
            if (!_IsNum(pBeg))
            {
                return 0;
            }
            nByteVal = atoi(pBeg);
            nByteVal = submask[i]  & nByteVal;
            values[i] = nByteVal;
            i--;

            pBeg = pPos + 1;
            pPos = strchr(pBeg,ch);
        }
        values[0] = atoi(pBeg);

        dwVal = (values[0] << (maskLen[2] + maskLen[1] + maskLen[0]))
                | (values[1] << (maskLen[1] + maskLen[0]))
                | (values[2] << maskLen[0])
                | (values[3] );

		return dwVal;
		*/

		CHAR szBuf[32]  = {0};
		CHAR* pBeg      = szBuf;
		CHAR ch         = ('-');
		strncpy(szBuf, pszServerID, sizeof(szBuf)-1);
		INT32 i =3;
		CHAR* pPos = strchr(szBuf,ch);
		UINT8 byIdx = 0;
		UINT16	wAreaID = 0;
		UINT8	byServerType = 0;
		UINT8	byServerIdx = 0;
		while (pPos != NULL)
		{
			*pPos = '\0';
			if (!_IsNum(pBeg))
			{
				return 0;
			}
			switch(byIdx++)
			{
			case 0:
				wAreaID = SDAtou(pBeg);
				break;
			case 1:
				byServerType = SDAtou(pBeg);
				break;
			case 2:
				byServerIdx = SDAtou(pBeg);
				break;
			default:
				return 0;
				break;
			}

			pBeg = pPos + 1;
			pPos = strchr(pBeg,ch);
		}
		
		return ((0xFFFF0000 & (wAreaID<<16)) | (0x0000FF00 & (byServerType<<8)) | (0x000000FF & byServerIdx));		        
    }

    CSDServerID::CSDServerID(UINT32 dwID , UINT32 dwMask )
    {
        m_dwMask = dwMask;
        this->SetID(dwID, dwMask);
    }


    void  CSDServerID::SetID(UINT32 dwID, UINT32 dwMask )
    {
        m_dwID = dwID;
        m_dwMask = dwMask;
		SDCalcMasks(m_dwMaskLen,m_dwSubMask,m_dwMask);
        m_strID = SDServerIDUtoa(dwID,m_dwMask);
    }

    void  CSDServerID::SetID(const TCHAR* pszID, UINT32 dwMask )
    {
        m_strID = pszID;
        m_dwMask = dwMask;
		SDCalcMasks(m_dwMaskLen,m_dwSubMask,m_dwMask);
        m_dwID  = SDServerIDAtou(_SDTT2A(pszID),m_dwMask);
    }

    void  CSDServerID::SetMask(UINT32 dwMask)
    {
        m_dwMask = dwMask;
    }

    UINT32  CSDServerID::GetMask()
    {
        return m_dwMask;
    }

    UINT32   CSDServerID::GetAreaID()
    {		
		//return (m_dwID) & m_dwSubMask[0];
		return (m_dwID & 0xFFFF0000) >> 16;			
    }

    UINT32   CSDServerID::GetGroupID()
    {
		//return (m_dwID >> m_dwMaskLen[0] ) & m_dwSubMask[1];
		return 0;
    }

    UINT32   CSDServerID::GetServerType()
    {
		//return (m_dwID >> (m_dwMaskLen[0] +m_dwMaskLen[1])) & m_dwSubMask[2];
		return (m_dwID & 0x0000FF00) >> 8;
    }

    UINT32   CSDServerID::GetServerIndex()
    {
		//return (m_dwID >> (m_dwMaskLen[0] +m_dwMaskLen[1]+m_dwMaskLen[2])) & m_dwSubMask[3];
		return  (m_dwID & 0x000000FF);
    }

    tstring   CSDServerID::AsString()
    {
        return m_strID;
    }

    UINT32  CSDServerID::AsNumber()
    {
        return m_dwID;
    }

//////////////////////////////////////////////////////////////////////////
// CSDServerIDEx


	/*
    tstring  SDServerIDExUtoa(UINT64 dwServerID, UINT32 dwMask )
    {
        TCHAR szServerId[128];

        UINT32 maskLen[4];
        UINT32 submask[4];
        SDCalcMasks(maskLen,submask,dwMask);
        UINT32 serverId = (UINT32)(dwServerID >> 32);

        UINT32 gameId = (UINT32)(dwServerID & 0xFFFFFFFF);

        UINT32 szBytes[4] = {0};

        szBytes[0] = (serverId >> (maskLen[0]+ maskLen[1] + maskLen[2])) & submask[3];
        szBytes[1] = (serverId >> (maskLen[0]+ maskLen[1])) & submask[2];
        szBytes[2] = (serverId >> (maskLen[0] )) & submask[1];
        szBytes[3] = (serverId ) & submask[0];

		_SDTsnprintf(szServerId,sizeof(szServerId) -1,_SDT("%d-%d-%d-%d-%d"),
                   gameId,
                   szBytes[3],
                   szBytes[2],
                   szBytes[1],
                   szBytes[0]);


        szServerId[sizeof(szServerId)-1] = _SDT('\0');

        return szServerId;
    }

    UINT64  SDServerIDExAtou(const CHAR* pszServerID,UINT32 dwMask )
    {
        CHAR szBuf[32]  = {0};
        CHAR* pBeg      = szBuf;
        CHAR* pPos      = NULL;
        INT32 nByteVal  = 0;
        UINT32 dwVal    = 0;

        CHAR ch         = ('-');

        INT32 values[4];

        UINT32 maskLen[4];
        UINT32 submask[4];

        SDCalcMasks(maskLen,submask,dwMask);

        const CHAR * pServerId = strchr(pszServerID,ch);
        INT32 gameId = atoi(pszServerID);

        strncpy(szBuf, pServerId +1, sizeof(szBuf)-1);
        INT32 i = 3;
        pPos = strchr(szBuf,ch);
        while (pPos != NULL)
        {
            *pPos = ('\0');
            if (!_IsNum(pBeg))
            {
                return 0;
            }
            nByteVal = atoi(pBeg);
            nByteVal = submask[i]  & nByteVal;
            values[i] = nByteVal;
            i--;

            pBeg = pPos + 1;
            pPos = strchr(pBeg,ch);
        }
        values[0] = atoi(pBeg);

        dwVal = (values[0] << (maskLen[2] + maskLen[1] + maskLen[0]))
                | (values[1] << (maskLen[1] + maskLen[0]))
                | (values[2] << maskLen[0])
                | (values[3] );

        UINT64 result = dwVal;
        return (result << 32) | gameId;
    }

    CSDServerIDEx::CSDServerIDEx(UINT64 dwId , UINT32 dwMask )
    {
        m_mask = dwMask;
        this->SetID(dwId, m_mask);
    }


    void  CSDServerIDEx::SetID(UINT64 dwID, UINT32 dwMask )
    {
        m_id = dwID;
        m_mask = dwMask;
		SDCalcMasks(m_maskLen,m_subMask,m_mask);
        m_strId = SDServerIDExUtoa(dwID,m_mask);
    }

    void  CSDServerIDEx::SetID(const TCHAR* pszID, UINT32 dwMask )
    {
        m_strId = pszID;
        m_mask = dwMask;
        SDCalcMasks(m_maskLen,m_subMask,m_mask);
        m_id = SDServerIDAtou(_SDTT2A(pszID),m_mask);
    }

    void  CSDServerIDEx::SetMask(UINT32 dwMask)
    {
        m_mask = dwMask;
        SDCalcMasks(m_maskLen,m_subMask,dwMask);
    }

    UINT32  CSDServerIDEx::GetMask()
    {
        return m_mask;
    }
    UINT32  CSDServerIDEx::GetGameId()
    {
        return (UINT32)m_id & 0xFFFFFFFF;
    }

    UINT32   CSDServerIDEx::GetAreaId()
    {
		return (UINT32)(m_id >> 32 ) & m_subMask[0];
	}

	UINT32   CSDServerIDEx::GetGroupId()
	{
		return ((UINT32)(m_id >> 32 ) >> m_maskLen[0] ) & m_subMask[1];
	}

	UINT32   CSDServerIDEx::GetServerType()
	{
		return ((UINT32)(m_id >> 32 ) >> (m_maskLen[0] +m_maskLen[1])) & m_subMask[2];
	}

	UINT32  CSDServerIDEx::GetServerIndex()
	{
		return ((UINT32)(m_id >> 32 ) >> (m_maskLen[0] +m_maskLen[1]+m_maskLen[2])) & m_subMask[3];
	}

	tstring   CSDServerIDEx::AsString()
	{
        return m_strId;
    }

    UINT64   CSDServerIDEx::AsNumber()
    {
        return m_id;
    }


	tstring GetServerShortName(UINT8 byServerType)
	{
		tstring strResult;
		if(TYPE_GAME_SERVER == byServerType)
		{
			strResult = _SDT("GS");
		}
		else if(TYPE_LOGIN_SERVER == byServerType)
		{
			strResult = _SDT("LG");
		}
		else if (TYPE_GATE_SERVER == byServerType)
		{
			strResult = _SDT("GT");
		}
		else if (TYPE_DB_SERVER == byServerType)
		{
			strResult = _SDT("DB");
		}
		else if (TYPE_BALANCE_SERVER == byServerType)
		{
			strResult = _SDT("BL");
		}
		else if (TYPE_CENTER_SERVER == byServerType)
		{
			strResult = _SDT("CS");
		}		
		else if (TYPE_NOTICE_SERVER == byServerType)
		{
			strResult = _SDT("NS");
		}
		else
			strResult = _SDT("Unknown");

		return strResult;
	}
	*/

} // namespace SGDP


