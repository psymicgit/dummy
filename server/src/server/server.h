///<------------------------------------------------------------------------------
//< @file:   server\server\server.h
//< @author: 洪坤安
//< @date:   2014年11月20日 10:36:54
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _server_h_
#define _server_h_

#include "protocol/protocol.h"

#include "net/net.h"
#include "net/msgdispatcher.h"

#include "basic/taskqueue.h"
#include "tool/servertool.h"

class Buffer;
class ConnectReq;
class ServerLink;

// 服务器基类
class Server : public INetLogic
{
public:
	// 服务器实例，相对于调用Instance()方法来说效率高一点
	static Server *instance;

	// <服务器id，对应的服务器连接>map
	typedef tr1::unordered_map<int, ServerLink*> ServerLinkMap;

public:
	// 接收到新的服务器连接
	virtual ServerLink* onAcceptServer(Link&, ServerType peerSvrType/*对端服务器类别*/, int peerSvrId) = 0;

	// 与对端服务器断开连接
	virtual void onDisconnectServer(Link&, ServerType peerSvrType/*对端服务器类别*/, int peerSvrId) = 0;

public:
	Server();

	// 初始化服务器
	bool init();

	// 回收本服务器的所有资源
	virtual bool uninit();

	// 本服务器名称
	virtual std::string name();

	// 成功主动与其他服务器建立起原始tcp连接
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	// 成功接收到其他服务器的原始tcp连接
	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	// 与其他服务器断开连接
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	// 接收到其他服务器发送来的数据
	virtual void onRecv(Link*);

	// 获取服务器主循环上的任务队列
	virtual TaskQueue& getTaskQueue() { return m_taskQueue; }

	// 处理指定连接上已接收到的数据
	virtual void handleMsg(Link*);

	// 启动服务器
	void start();

	// 关闭服务器：调用此接口后，服务器将在处理完网络任务、数据库任务、逻辑业务之后才关闭
	void stop();

	// 回收服务器资源
	virtual void stopping();

	// 每次服务器循环所执行的任务
	virtual void run();

	// 是否已退出
	bool isquit() { return m_isquit; }

	// 获取本服务器id
	int getServerId(ServerType, int zoneId);

	// 是否与对应服务器保持连接
	bool isSvrLinkExist(ServerType, int zoneId);

	// 注册其他服务器
	void registerServer(int svrId, ServerLink*);

	// 取消注册服务器
	void unregisterServer(int svrId);

public:
	// 本服务器类别
	ServerType m_svrType;

	// 区Id
	int m_zoneId;

	// 内网中心：负责与内网中其他服务器进行通信
	Net m_lan;

protected:
	// 是否已退出标志位
	bool m_isquit;

	// 待执行的任务队列
	TaskQueue m_taskQueue;

	// 消息包派发器
	MsgDispatcher<Link> m_dispatcher;

	// 本服务器持有的与其他服务器的连接
	ServerLinkMap m_svrLinkMap;
};

#endif //_server_h_