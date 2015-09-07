///<------------------------------------------------------------------------------
//< @file:   server\net\msghandler.h
//< @author: 洪坤安
//< @date:   2015年3月17日 23:59:29
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _msghandler_h_
#define _msghandler_h_

#include <google/protobuf/message.h>

// #include "basic/bind.h"
#include "tool/timetool.h"
#include "protocol/message.h"
#include "net/msgdispatcher.h"
#include "net/imsghandler.h"

class Link;

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

// 消息处理器
// （这里之所以提取出IMsgHandler接口是为了防止模板文件互相包含导致编译不过）
// （LinkType代表从连接实例身上得到的message，比如代表客户端连接的Client、代表底层网络连接的Link等）
template <typename LinkType>
class MsgHandler : public IMsgHandler<LinkType>
{
private:
	typedef tr1::unordered_map<int, Callback<LinkType>*> CallbackMap;

public:
	MsgHandler(MsgDispatcher<LinkType> *msgDispacher)
		: m_msgDispacher(msgDispacher)
	{
	}

	// 处理接收到的数据
	void onMessage(LinkType& link, int msgId, const char* data, int len, Timestamp receiveTime) const
	{
		// 寻找到对应的消息回调函数
		typename CallbackMap::const_iterator it = m_callbackMap.find(msgId);
		if (it == m_callbackMap.end()) {
			LOG_DEBUG << "Discard msg[id=" << msgId << "]";
			return;
		}

		// 处理消息包
		Callback<LinkType> *callback = it->second;
		callback->onMessage(link, data, len, receiveTime);
	}

	// 对某个消息id注册对应的回调函数
	template <typename MessageType>
	void registerMsg(int msgId, void (*callback)(LinkType*, MessageType* message, Timestamp))
	{
		CallbackT<LinkType, MessageType> *pd = new CallbackT<LinkType, MessageType>(callback);
		m_callbackMap[msgId] = pd;

		m_msgDispacher->Register(msgId, this);
	}

	void discardMessage(Link& link, const Message& message, Timestamp receiveTime) const
	{
		LOG_DEBUG << "Discarding " << msgtool::getMsgDebugString(message);
	}

	// 回收所有资源，包括内存
	void clear()
	{
		typename CallbackMap::iterator itr(m_callbackMap.begin());
		typename CallbackMap::iterator end(m_callbackMap.end());

		for (; itr != end; ++itr) {
			Callback<LinkType> *callBack = itr->second;

			// LOG_INFO << "delete";
			delete callBack;
		}

		m_callbackMap.clear();
	}

	// 消息回调map
	CallbackMap m_callbackMap;

	// 消息派发器
	MsgDispatcher<LinkType> *m_msgDispacher;
};

#endif //_msghandler_h_