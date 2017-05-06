///<------------------------------------------------------------------------------
//< @file:   server\protocol\netmsgmgr.h
//< @author: ������
//< @date:   2015��3��8�� 16:35:12
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netmsgmgr_h_
#define _netmsgmgr_h_

#include <net.pb.h>
#include <protocol.pb.h>
#include "net/serverlink.h"
#include "tool/servertool.h"

class NetMsgHandler
{
public:
	static void OnConnectServerReq(Link& link, ConnectReq& req, Timestamp receiveTime)
	{
		ServerType peerSvrType		= (ServerType)req.svrtype();
		int peerSvrId				= req.svrid();

		int svrId = Server::instance->getServerId(peerSvrType, peerSvrId);
		const std::string &authKey = req.authkey();

		ConnectResponse res;
		res.set_svrtype(Server::instance->m_svrType);
		res.set_ret(CONNECT_OK);

		if(authKey != g_serverHandshakeAuthKey) {
			LOG_WARN << "reject server<" << svrtool::getSvrName((ServerType)req.svrtype()) << ", zoneId=" << req.svrid() << "> connect : auth failed, auth key invalid! \n" << msgtool::getMsgDebugString(req);
			res.set_ret(CONNECT_FAIL_AUTH_KEY_INVALID);
			link.send(eConnectSvrAck, res);

			return;
		}

		if(Server::instance->isSvrLinkExist((ServerType)req.svrtype(), req.svrid())) {
			LOG_WARN << "reject server<" << svrtool::getSvrName((ServerType)req.svrtype()) << ", zoneId=" << req.svrid() << "> connect : found same server connection! \n" << msgtool::getMsgDebugString(req);
			res.set_ret(CONNECT_FAIL_FOUND_SAME_SERVER);
		} else {
			// LOG_DEBUG << "OnAcceptConnect : \n" << msgtool::getMsgString(*req);
			ServerLink *svrLink = Server::instance->onAcceptServer(link, (ServerType)req.svrtype(), req.svrid());
			if (NULL == svrLink) {
				res.set_ret(CONNECT_FAIL_UNKNOWN_SERVER_TYPE);
			} else {
				svrLink->m_link			= &link;
				svrLink->m_localSvrType	= Server::instance->m_svrType;
				svrLink->m_peerSvrType	= peerSvrType;
				svrLink->m_peerSvrId	= peerSvrId;
				svrLink->m_taskQueue	= &Server::instance->getTaskQueue();

				link.m_logic = svrLink;

				Server::instance->registerServer(svrId, svrLink);
				svrLink->onEstablish();
			}
		}

		link.send(eConnectSvrAck, res);
// 		LOG_DEBUG << "<<:" << req.GetDescriptor()->full_name();
// 		LOG_DEBUG << "<<:" << req.GetDescriptor()->DebugString();
// 		LOG_DEBUG << "<<:" << req.DebugString();
	}

	static void OnConnectServerAck(Link& link, ConnectResponse& res, Timestamp receiveTime)
	{
		ConnectResult ret = res.ret();
		if (ret != CONNECT_OK) {
			const char* reason = "";
			switch(ret) {
			case CONNECT_FAIL_UNKNOWN_SERVER_TYPE:
				reason = "unknown server type";
				break;

			case CONNECT_FAIL_FOUND_SAME_SERVER:
				reason = "found same server connection";
				break;

			case CONNECT_FAIL_AUTH_KEY_INVALID:
				reason = "auth key invalid";
				break;

			default:
				reason = "unknow reason";
				break;
			}

			LOG_ERROR << "connect failed: peer server<" << svrtool::getSvrName((ServerType)res.svrtype())
			          << ", zoneId=" << res.svrid() << "> reject connection : " << reason << "! \n" << msgtool::getMsgDebugString(res);

			link.close();
			return;
		}

		// LOG_DEBUG << "OnAcceptConnect : \n" << msgtool::getMsgString(*req);
		ServerLink *svrLink = Server::instance->onAcceptServer(link, (ServerType)res.svrtype(), res.svrid());
		if (svrLink) {
			svrLink->m_link		= &link;
			svrLink->m_peerSvrType = (ServerType)res.svrtype();
			svrLink->m_peerSvrId	= res.svrid();
			svrLink->m_taskQueue	= &Server::instance->getTaskQueue();

			link.m_logic			= svrLink;

			int svrId = Server::instance->getServerId((ServerType)res.svrtype(), res.svrid());
			Server::instance->registerServer(svrId, svrLink);
			svrLink->onEstablish();
		}
	}
};

#endif //_netmsgmgr_h_