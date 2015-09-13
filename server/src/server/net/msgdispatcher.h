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

// 消息回调基类
template <typename LinkType>
class Callback
{
public:
	virtual void onMessage(LinkType&, const char* data, int len, Timestamp) const = 0;
};

// 针对某个具体消息类别的回调
template <typename LinkType, typename MessageType>
class CallbackT : public Callback<LinkType>
{
public:
	typedef void (*ProtobufMessageCallback)(LinkType*, MessageType* message, Timestamp);

	CallbackT(const ProtobufMessageCallback &callback)
		: m_callback(callback)
	{
	}

	virtual void onMessage(LinkType& link, const char* data, int len, Timestamp receiveTime) const
	{
		MessageType t;

		bool ok = t.ParseFromArray(data, len);
		if (!ok) {
			LOG_ERROR << "decode packet <" << t.GetTypeName() << "> = <" << t.SerializeAsString() << "> error, data len = " << len;
			// LOG_INFO << "decode packet <" << msgtool::getMsgString(*t) << "> error";
			return;
		}

		m_callback(&link, &t, receiveTime);
	}

private:
	// 消息回调函数
	ProtobufMessageCallback m_callback;
};

// 消息派发器：负责把消息包派发给合适的消息处理器
// 注意：<消息派发器>管理多个<消息处理器>，而每个<消息处理器>内又注册了不同的<消息回调函数>
template <typename LinkType>
class MsgDispatcher
{
private:
	typedef tr1::unordered_map<int, Callback<LinkType>*> CallbackMap;
	typedef tr1::unordered_map<int, IMsgHandler<LinkType>*> MsgHandlerMap;
	typedef std::vector<IMsgHandler<LinkType>*> MsgHandlerVec;

public:
	template <typename MessageType>
	void registerMsg(int msgId, CallbackT<LinkType, MessageType> *pd)
	{
		m_callbackMap[msgId] = pd;
	}

	// 派发消息包
	void dispatch(LinkType& link, int msgId, const char* data, int len, Timestamp receiveTime)
	{
		// 首先根据消息id找到预先注册好的对应的消息包处理器
		// 寻找到对应的消息回调函数
		typename CallbackMap::const_iterator it = m_callbackMap.find(msgId);
		if (it == m_callbackMap.end()) {
			LOG_DEBUG << "Discard msg[id=" << msgId << "][len=" << len << "]";
			return;
		}

		// 处理消息包
		Callback<LinkType> *callback = it->second;
		callback->onMessage(link, data, len, receiveTime);
	}

	void addMsgHandler(IMsgHandler<LinkType> *msgHandler)
	{
		m_msgHandlerVec.push_back(msgHandler);
	}

	void clear()
	{
		// LOG_INFO << "clear";

		for (size_t i = 0; i < m_msgHandlerVec.size(); i++) {
			IMsgHandler<LinkType> *msgHandler = m_msgHandlerVec[i];
			msgHandler->clear();
			delete msgHandler;
		}

		m_msgHandlerVec.clear();
	}

private:
	// 消息回调map
	CallbackMap m_callbackMap;
	MsgHandlerVec m_msgHandlerVec;;
};

#endif //_msgdispatcher_h_