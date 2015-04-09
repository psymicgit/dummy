///<------------------------------------------------------------------------------
//< @file:   pkapplication.h
//< @author: hongkunan
//< @date:   2014年1月2日 15:52:58
//< @brief:  跨服战服务器，专门负责跨服的战斗（并且，有可能不止是战斗，比如，全服竞技场排名），与各区的gameserver均维持一个连接
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pkapplication_h_
#define _pkapplication_h_

#include <sdframework.h>
#include <net/db/dbprocessor.h>
#include <common/server/packetparser.h>
#include <db/dbmgr.h>
#include <sdmemorypool.h>

class CGSPipeChannel;

// 与内部gameserver的通信管道列表，每个区id对应该区的gameserver，key = gameserver的区Id，val = 本服务器与该区gameserver的通信管道
typedef std::map<UINT16, CGSPipeChannel*> CZoneID2GSPipeChannelMap;

// 跨服战服务器
class CPKApplication : public CSDApplication
{
public:
    CPKApplication();

public:
    // 根据区Id查找与该区的gameserver的连接
    CGSPipeChannel*	GetGSPipeChannel(UINT16 wZoneID);

    ///<@impl>
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);

    ///<@impl>
    virtual VOID OnPipeChannelDisconnected(CSDPipeChannel* poChannel);

private:
    CDBProcessor			 m_oDBProcessor;      // 数据库语句成功回调时的处理器

    CZoneID2GSPipeChannelMap m_mapGSPipeChannel;  // 连接到本服务器的各个gameserver
    CSDPipeChannel*		     m_poGMPipeChannel;   // 与gm服务器的连接通道
    ISDListener*			 m_poListener;        // 侦听器，侦听来自于gameserver的连接

    CDBMgr				     m_oDBMgr;
    CSDFixMemoryPool		 m_oSQLMemoryPool;
    CSDMutex				 m_oSQLLock;
    
    CHttpCliPacketParser	 m_oRCPacketParser;

    UINT64				     m_qwServerStartTime; //服务启动时间
    BOOL					 m_bNeedSave2DB;
};

#endif //_pkapplication_h_