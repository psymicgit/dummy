///<------------------------------------------------------------------------------
//< @file:   server\net\msgdispatcher.h
//< @author: ������
//< @date:   2015��3��8�� 15:1:8
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _msgdispatcher_h_
#define _msgdispatcher_h_

#include <google/protobuf/message.h>

#include "net/link.h"
#include "tool/timetool.h"

// ��Ϣ�ص�����
template <typename LinkType>
class Callback
{
public:
	virtual ~Callback() {}

	virtual void onMessage(LinkType&, const char* data, int len, Timestamp) const = 0;
};

// ���ĳ��������Ϣ���Ļص�
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
	// ��Ϣ�ص�����
	ProtobufMessageCallback m_callback;
};

// ��Ϣ�ɷ������������Ϣ���ɷ������ʵ���Ϣ������
// ע�⣺<��Ϣ�ɷ���>������<��Ϣ������>����ÿ��<��Ϣ������>����ע���˲�ͬ��<��Ϣ�ص�����>
template <typename LinkType>
class MsgDispatcher
{
private:
	typedef tr1::unordered_map<int, Callback<LinkType>*>	CallbackMap;

public:
	// ��ĳ����Ϣidע���Ӧ�Ļص�����
	template <typename MessageType>
	void registerMsg(int msgId, void(*callback)(LinkType*, MessageType* message, Timestamp))
	{
		CallbackT<LinkType, MessageType> *pd = new CallbackT<LinkType, MessageType>(callback);
		m_callbackMap[msgId] = pd;
	}

	// �ɷ���Ϣ��
	void dispatch(LinkType& link, int msgId, const char* data, int len, Timestamp receiveTime)
	{
		// ���ȸ�����Ϣid�ҵ�Ԥ��ע��õĶ�Ӧ����Ϣ��������
		// Ѱ�ҵ���Ӧ����Ϣ�ص�����
		typename CallbackMap::const_iterator it = m_callbackMap.find(msgId);
		if (it == m_callbackMap.end()) {
			LOG_DEBUG << "Discard msg[id=" << msgId << "][len=" << len << "]";
			return;
		}

		// ������Ϣ��
		Callback<LinkType> *callback = it->second;
		callback->onMessage(link, data, len, receiveTime);
	}

	void clear()
	{

	}

private:
	// ��Ϣ�ص�map
	CallbackMap		m_callbackMap;
};

#endif //_msgdispatcher_h_