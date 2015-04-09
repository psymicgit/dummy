// SHANDA GAMES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Shanda Games and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1999-2010 Shanda Games.
//      All Rights Reserved.
//
// Shanda Games Limited, Shanghai, China, 201203
// http://www.shandagames.com

#include "clipktparser.h"
#include "clidefine.h"
#include <sdnetutils.h>

CCliPacketParser::CCliPacketParser()
{
}

CCliPacketParser::~CCliPacketParser()
{
}

/// 获取实例
CCliPacketParser* CCliPacketParser::Instance()
{
    static CCliPacketParser sm_Parser;
    return &sm_Parser;
}

/// 解析报文
INT32 CCliPacketParser::ParsePacket(const CHAR *pBuf, UINT32 dwLen)
{
    if(dwLen < sizeof(STNetMsgHeader))
    {
        return 0;
    }
    STNetMsgHeader* pNetMsgHeader = (STNetMsgHeader*)pBuf;
    UINT16 wMsgID = SGDP::SDNtohs(pNetMsgHeader->wMsgID);
    UINT32 dwDataLen = SGDP::SDNtohl(pNetMsgHeader->dwDataLen);
    INT32 nPkgLen = dwDataLen + sizeof(STNetMsgHeader);
    if(nPkgLen < 0)
    {
        return -1;
    }
    if(nPkgLen <= (INT32)dwLen)
    {
        return nPkgLen;
    }
    return 0;
}

