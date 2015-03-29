///<------------------------------------------------------------------------------
//< @file:   server\net\msgdispatcher.h
//< @author: 洪坤安
//< @date:   2015年3月8日 15:1:8
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _msgdispatcher_h_
#define _msgdispatcher_h_

#include <google/protobuf/message.h>

#include "net/link.h"
#include "tool/timetool.h"
#include "imsghandler.h"

template <typename LinkType>
class MsgDispatcher
{
private:
	typedef std::map<int, IMsgHandler<LinkType>*> MsgHandlerMap;

public:
	// 这里register会与原有命名冲突，所以用Register
	void Register(int msgId, IMsgHandler<LinkType> *msgMgr)
	{
		m_msgMap[msgId] = msgMgr;
	}

	void dispatch(LinkType& link, int msgId, const char* data, int len, Timestamp receiveTime)
	{
		typename MsgHandlerMap::const_iterator itr = m_msgMap.find(msgId);
		if (itr == m_msgMap.end()) {
			return;
		}

		IMsgHandler<LinkType> *msgMgr = itr->second;
		msgMgr->onMessage(link, msgId, data, len, receiveTime);
	}


private:
	MsgHandlerMap m_msgMap;
};

#endif //_msgdispatcher_h_