///<------------------------------------------------------------------------------
//< @file:   server\protocol\netmsgmgr.h
//< @author: 洪坤安
//< @date:   2015年3月8日 16:35:12
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netmsgmgr_h_
#define _netmsgmgr_h_

#include <net.pb.h>
#include <protocol.pb.h>

#include "net/msghandler.h"
#include "net/serverlink.h"
#include "tool/servertool.h"

class NetMsgHandler : public MsgHandler<Link>
{
public:
	NetMsgHandler(MsgDispatcher<Link> *msgDispacher)
		: MsgHandler<Link>(msgDispacher)
	{
		init();
	}

	void init()
	{
		registerMsg(eConnectSvrReq, OnConnectServerReq);
		registerMsg(eConnectSvrAck, OnConnectServerAck);
	}

private:
	static void OnConnectServerReq(Link* link, ConnectReq *req, Timestamp receiveTime)
	{
		ServerType peerSvrType = (ServerType)req->svrtype();
		int peerSvrId = req->svrid();
		int svrId = Server::instance->getServerId(peerSvrType, peerSvrId);
		const std::string &authKey = req->authkey();

		ConnectResponse res;
		res.set_svrtype(Server::instance->m_svrType);
		res.set_ret(CONNECT_OK);

		if(authKey != g_serverHandshakeAuthKey) {
			LOG_WARN << "reject server<" << svrtool::getSvrName((ServerType)req->svrtype()) << ", zoneId=" << req->svrid() << "> connect : auth failed, auth key invalid! \n" << msgtool::getMsgString(*req);
			res.set_ret(CONNECT_FAIL_AUTH_KEY_INVALID);
			link->send(eConnectSvrAck, res);

			return;
		}

		if(Server::instance->isSvrLinkExist((ServerType)req->svrtype(), req->svrid())) {
			LOG_WARN << "reject server<" << svrtool::getSvrName((ServerType)req->svrtype()) << ", zoneId=" << req->svrid() << "> connect : found same server connection! \n" << msgtool::getMsgString(*req);
			res.set_ret(CONNECT_FAIL_FOUND_SAME_SERVER);
		} else {
			// LOG_DEBUG << "OnAcceptConnect : \n" << msgtool::getMsgString(*req);
			ServerLink *svrLink = Server::instance->onAcceptServer(*link, (ServerType)req->svrtype(), req->svrid());
			if (NULL == svrLink) {
				res.set_ret(CONNECT_FAIL_UNKNOWN_SERVER_TYPE);
			} else {
				svrLink->m_link = link;
				svrLink->m_remoteSvrType = peerSvrType;
				svrLink->m_svrId = peerSvrId;
				svrLink->m_taskQueue = &Server::instance->getTaskQueue();

				link->m_pNetReactor = svrLink;

				Server::instance->registerServer(svrId, svrLink);
				svrLink->onEstablish();
			}
		}

		link->send(eConnectSvrAck, res);
// 		LOG_DEBUG << "<<:" << req->GetDescriptor()->full_name();
// 		LOG_DEBUG << "<<:" << req->GetDescriptor()->DebugString();
// 		LOG_DEBUG << "<<:" << req->DebugString();
	}

	static void OnConnectServerAck(Link* link, ConnectResponse *res, Timestamp receiveTime)
	{
		ConnectResult ret = res->ret();
		if (ret != CONNECT_OK) {
			std::string failMsg = "";
			switch(ret) {
			case CONNECT_FAIL_UNKNOWN_SERVER_TYPE:
				failMsg = "unknown server type";
				break;

			case CONNECT_FAIL_FOUND_SAME_SERVER:
				failMsg = "found same server connection";
				break;

			case CONNECT_FAIL_AUTH_KEY_INVALID:
				failMsg = "auth key invalid";
				break;

			default:
				failMsg = "unknow reason";
				break;
			}

			LOG_ERROR << "connect failed: peer server<" << svrtool::getSvrName((ServerType)res->svrtype()) << ", zoneId=" << res->svrid() << "> reject connection : " << failMsg << "! \n" << msgtool::getMsgString(*res);

			link->close();
			return;
		}

		// LOG_DEBUG << "OnAcceptConnect : \n" << msgtool::getMsgString(*req);
		ServerLink *svrLink = Server::instance->onAcceptServer(*link, (ServerType)res->svrtype(), res->svrid());
		if (svrLink) {
			svrLink->m_link = link;
			svrLink->m_remoteSvrType = (ServerType)res->svrtype();
			svrLink->m_svrId = res->svrid();
			svrLink->m_taskQueue = &Server::instance->getTaskQueue();

			link->m_pNetReactor = svrLink;

			int svrId = Server::instance->getServerId((ServerType)res->svrtype(), res->svrid());
			Server::instance->registerServer(svrId, svrLink);
			svrLink->onEstablish();
		}
	}
};

#endif //_netmsgmgr_h_