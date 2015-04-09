

#ifndef _SGDP_GAMESERVER_APPLICATION_H_
#define _SGDP_GAMESERVER_APPLICATION_H_

#include "sdframework.h"
#include <net/gt/gtprocessor.h>
#include <net/db/dbprocessor.h>
#include <net/ns/nsprocessor.h>
#include <net/ks/ksprocessor.h>
#include <net/recharge/rechargeclient.h>
#include <net/recharge/rcclient.h>
#include <net/recharge/httppacketparser.h>
#include <common/server/packetparser.h>
#include <list>
#include <db/dbmgr.h>
#include <sdmemorypool.h>
#include "logic/player/playersavemgr.h"
#include "logic/base/basedefine.h"

using namespace std;
using namespace SGDP;

class CGSPipeChannel;

typedef map<UINT16, CSDPipeChannel*> CZoneID2PipeChannelMap;
typedef CZoneID2PipeChannelMap::iterator CZoneID2PipeChannelMapItr;

class CGSApplication : public CSDApplication
{
private:
    // 与游戏服务器的连接列表，key = 区id， val = 与该区的游戏服务器的连接管道
    typedef std::map<UINT16, CSDPipeChannel*> CZoneID2GSPipeChannelMap;

public:
    CGSApplication();
    virtual ~CGSApplication();

public:
    virtual BOOL        Init(const TCHAR* pszConfigFilename);
    virtual VOID        UnInit();
    virtual BOOL        Run();
    virtual VOID        OnHandleUserCmd(const TCHAR* szCmd);

    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID        OnPipeChannelDisconnected(CSDPipeChannel* poChannel);

    virtual VOID			UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect);

	inline CZoneID2PipeChannelMap&	GetAllGTPipeChannel() { return m_mapGTPipeChannel; } 
	CSDPipeChannel*		GetGTPipeChannel(UINT16 wZoneID);
    CSDPipeChannel*		GetGSPipeChannelByZoneID(UINT16 wZoneID);

    CSDPipeChannel*		GetNSPipeChannel();
    CSDPipeChannel*		GetGSPipeChannel();
	CSDPipeChannel*		GetPKPipeChannel();
	CDBProcessor*		GetDBProcessor() { return &m_oDBProcessor; }


	inline	CDBMgr*					GetDBMgr() { return &m_oDBMgr; }
	inline CRechargeClientMgr*		GetRechargeClientMgr() { return &m_oRechargeClientMgr; }
	inline CRCClientMgr*			GetRCClientMgr() { return &m_oRCClientMgr; }

	////////////////////////////////
	VOID* GetSQLMemory();
	VOID FreeSQL(VOID* p);


	//发送消息推送
	VOID NoticeMsg(UINT32 dwPlayerID, const CHAR* pBuf);
	VOID NoticeMsg(UINT32 dwPlayerID, UINT8 byAction, const CHAR* szToken, const CHAR* pBuf);
	
	VOID GMMsgResponse(UINT32 dwMsgID, UINT32 dwPlayerID, const CHAR* pBuf);

	UINT64				GetServerStartTime() { return m_qwServerStartTime; }

	CCliPacketParser    &GetPacketParser(){ return m_oCliPacketParser; }
	ISDListener*		GetRechargeListener() { return m_poListener; }	
	BOOL&				GetNeedSave2DB() { return m_bNeedSave2DB; }

    // 获取本服区列表，仅当本服为跨服战服务器时才有效
    ZoneIDSet GetZoneList();

    // 是不是PK服务器
    BOOL				IsServerType(eServerType eST);

protected:
	BOOL				InitRecharge();

protected:
	///////////////////GM///////////////////////
	BOOL				DealUserCmd(const TCHAR* szCmd);

private:
    // 停止游戏服务器的各个任务，比如定时回写玩家数据、定时取公告等（当本服是跨服战服务器时，虽然共用了游戏服务器的逻辑，但某些游戏服务器的任务是不需要执行的）
    VOID				StopGameServerTask();

    // 初始化跨服战服务器的各项游戏逻辑
    BOOL				InitGvGSvrLogic();

    eServerType	GetServerType();

private:
    eServerType					m_eThisServerType; // 本服务器的类型：游戏服务器/跨服战服务器

    CGTProcessor				m_oGTProcessor;
	CNSProcessor				m_oNSProcessor;
	CGMProcessor				m_oGMProcessor;
    CDBProcessor				m_oDBProcessor;    

	CZoneID2PipeChannelMap	    m_mapGTPipeChannel;//所有的网关，为了适配合区，key为区ID
    CZoneID2GSPipeChannelMap    m_mapGSPipeChannel; // 连接到本服务器的游戏服务器列表（仅当本服为跨服战服务器时才有效）
	CSDPipeChannel*		        m_poNSPipeChannel;
	CSDPipeChannel*		        m_poGMPipeChannel;
	CSDPipeChannel*		        m_poGSPipeChannel;
    CSDPipeChannel*		        m_poPKPipeChannel; // 与跨服战服务器的连接

	CDBMgr				        m_oDBMgr;

	CSDFixMemoryPool*		    m_pSQLMemoryPool;
	CSDMutex				    m_oSQLLock;

	//////
	CRechargeClientMgr		    m_oRechargeClientMgr;
	CRCClientMgr			    m_oRCClientMgr;
	ISDListener*			    m_poListener;
	CCliPacketParser		    m_oCliPacketParser;
	CHttpCliPacketParser	    m_oRCPacketParser;

	UINT64				        m_qwServerStartTime; //服务启动时间
	BOOL					    m_bNeedSave2DB;
};


#endif


