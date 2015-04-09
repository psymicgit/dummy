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

#ifndef _PROTPCOLMGR_H_
#define _PROTPCOLMGR_H_

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#define SDF_MAX_PROCTOL_MSG     30000

/**
 * @file 协议管理容器
 *
 * 用于存放协议与消息号映射
 *
 * @author 技术中心服务器引擎部
 */
#include "netdata.h"
#include <map>
#include <string>

using namespace std;
//using namespace SGDP;

/**
 * @brief 协议管理容器
 */
class CProtocolMgr: public map<string ,CSDProtocol*>
{
private:
    CProtocolMgr();
    ~CProtocolMgr();

public:
    /**
     * @brief 获取实例
     * @return 获取实例对象指针
     */
    static CProtocolMgr*  Instance();

    /**
     * @brief 清理所有协议
     */
    VOID                  DeleteAll();

    /**
     * @brief 增加协议
     */
    CSDProtocol*		  AddProtocol( CSDProtocol *pProtocol);

    /**
     * @brief 根据消息号查找协议
     */
    CSDProtocol*		  FindProtocol(UINT32 nMsgID);

    /**
     * @brief 根据名称查找协议
     */
    CSDProtocol*		  FindProtocol(string strName);

    /**
     * @brief 根据名称删除指定协议
     */
    CSDProtocol*		  RemoveProtocol(string strName);

private:
    /// 协议数组
    CSDProtocol*          m_SDProtocol[SDF_MAX_PROCTOL_MSG];
};

#endif // #ifndef _PROTPCOLMGR_H_

