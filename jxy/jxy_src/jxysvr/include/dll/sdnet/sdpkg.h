#ifndef _SGDP_SGPKG_H_
#define _SGDP_SGPKG_H_

#include "sdnet.h"
#include "sdnetutils.h"

namespace SGDP{

#define SDPKG_MARK16    0xAAEE
#define SDPKG_MARK32    0xAAEF

#pragma pack(push, 1)

    //
    //
    //
    struct SSDPkgHead16
    {
        UINT16  wMark;
        UINT16  wDataLen;
        UINT16  wCheckSum;
    };

    struct SSDPkgHead32
    {
        UINT16  wMark;
        UINT32  dwDataLen;
        UINT16  wCheckSum;
    };

    struct SDNetMsgHeader
    {
		UINT16	wMark;		///< 包头标示
        UINT16	wMsgID;     ///< 消息号
        UINT32	dwDataLen;   ///< 数据包长度
    };

    struct SDPipeMsgHeader
    {
		UINT16	wMark;		///< 包头标示
        UINT16	wMsgID;		
        UINT32	dwTransID;	
    };


#define MAX_PACKET_LEN				(1024*1024)

#pragma pack(pop)

    /**
    * @param wDataLen : data length after SSDPkgHead16, not include length of SSDPkgHead16
    * @return:  total packet length, include length of SSDPkgHead16 and length of 
    *          data
    */
    inline INT32 SDBuildPkgHead16(SSDPkgHead16* pstHead, UINT16 wDataLen)
    {
        pstHead->wMark		= SDPKG_MARK16;
        pstHead->wDataLen	= wDataLen;
        pstHead->wCheckSum	= (pstHead->wDataLen ^ 0xBBCC) & 0x88AA;

        pstHead->wMark		= SDHtons(pstHead->wMark);
        pstHead->wDataLen	= SDHtons(pstHead->wDataLen);
        pstHead->wCheckSum	= SDHtons(pstHead->wCheckSum);

        return wDataLen + sizeof(SSDPkgHead16);
    }

    inline INT32 SDBuildPkgHead32(SSDPkgHead32* pstHead, UINT32 dwDataLen)
    {
        pstHead->wMark		= SDPKG_MARK32;
        pstHead->dwDataLen	= dwDataLen;
        pstHead->wCheckSum	= ( (pstHead->dwDataLen>>16)^(pstHead->dwDataLen)^0xBBCC) & 0x88AA;

        pstHead->wMark		= SDHtons(pstHead->wMark);
        pstHead->dwDataLen	= SDHtonl(pstHead->dwDataLen);
        pstHead->wCheckSum	= SDHtons(pstHead->wCheckSum);

        return dwDataLen + sizeof(SSDPkgHead32);
    }

    inline INT32 SDBuildNetMsgHeader(SDNetMsgHeader* pstHead, UINT16 wMsgID ,UINT32 dwDataLen)
    {
		pstHead->wMark		  = SDNtohs(PKTHEADMAK);
        pstHead->wMsgID       = SDNtohs(wMsgID);
        pstHead->dwDataLen    = SDHtonl(dwDataLen);
        return dwDataLen + sizeof(SDNetMsgHeader);
    }

    inline INT32 SDBuildPipeMsgHeader(SDPipeMsgHeader* pstHead, UINT16 wMsgID ,UINT32 dwTransID)
    {
        pstHead->wMsgID       = SDNtohs(wMsgID);
        pstHead->dwTransID    = SDHtonl(dwTransID);
        return sizeof(SDPipeMsgHeader);
    }


    inline INT32 SDCheckPkgHead(const char* pPkgHead, UINT32 dwLen)
    {
        if(dwLen < sizeof(UINT16))
            return 0;

        UINT16 wMark = SDNtohs(*(UINT16*)pPkgHead);
        if(SDPKG_MARK16 == wMark)
        {
            if(dwLen < sizeof(SSDPkgHead16))
                return 0;

            SSDPkgHead16 stTmpHead16;
            stTmpHead16.wMark       = SDNtohs( ((SSDPkgHead16*)pPkgHead)->wMark );
            stTmpHead16.wDataLen    = SDNtohs( ((SSDPkgHead16*)pPkgHead)->wDataLen );
            stTmpHead16.wCheckSum   = SDNtohs( ((SSDPkgHead16*)pPkgHead)->wCheckSum );

            if(stTmpHead16.wCheckSum != ( (stTmpHead16.wDataLen ^ 0xBBCC) & 0x88AA ) )
                return -1;

            return stTmpHead16.wDataLen + sizeof(SSDPkgHead16);
        }
        else if(SDPKG_MARK32 == wMark)
        {
            if(dwLen < sizeof(SSDPkgHead32))
                return 0;

            SSDPkgHead32 stTmpHead32;
            stTmpHead32.wMark       = SDNtohs( ((SSDPkgHead32*)pPkgHead)->wMark );
            stTmpHead32.dwDataLen   = SDNtohl( ((SSDPkgHead32*)pPkgHead)->dwDataLen );
            stTmpHead32.wCheckSum   = SDNtohs( ((SSDPkgHead32*)pPkgHead)->wCheckSum );

            if(stTmpHead32.wCheckSum !=  ( ((stTmpHead32.dwDataLen>>16)^(stTmpHead32.dwDataLen)^0xBBCC) & 0x88AA) )
                return -1;

            return stTmpHead32.dwDataLen + sizeof(SSDPkgHead32);
        }

        return -1;
    }

    inline UINT32 SDGetPkgDataOffset(const char* pPkg, UINT32 dwLen)
    {
        UINT16 wMark = SDNtohs(*(UINT16*)pPkg);
        if(SDPKG_MARK16 == wMark)
        {
            return sizeof(SSDPkgHead16);
        }
        else if(SDPKG_MARK32 == wMark)
        {
            return sizeof(SSDPkgHead32);
        }
        else
        {
            return 0;
        }
    }

    class CSDPacketParser : public ISDPacketParser
    {
    public:
        CSDPacketParser(void){}
        virtual ~CSDPacketParser(void){}

        virtual INT32 SDAPI ParsePacket(const char* pBuf, UINT32 dwLen)
        {
            INT32 nPkgLen = SDCheckPkgHead(pBuf, dwLen);

            if(nPkgLen < 0)
                return -1;

            if(nPkgLen <= (INT32)dwLen)
                return nPkgLen;

            return 0;
        }
    };

    class CSDPacketBuilder
    {
    public:
        CSDPacketBuilder(){};
        ~CSDPacketBuilder(){};
    public:
        CHAR* GetPacketBuffer(){ static CHAR m_szBuf[MAX_PACKET_LEN] = {0}; return m_szBuf; }
        VOID  ZeroPacketBuffer(UINT32 dwSize = MAX_PACKET_LEN ) { memset(GetPacketBuffer(), 0, dwSize); }
    protected:
    };


}

#endif

