///<------------------------------------------------------------------------------
//< @file:   server\net\imsghandler.h
//< @author: 洪坤安
//< @date:   2015年3月17日 23:37:25
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _imsghandler_h_
#define _imsghandler_h_

// 消息处理器
template <typename LinkType>
class IMsgHandler
{
public:
	virtual ~IMsgHandler() {}

	virtual void onMessage(LinkType& link, int msgId, const char* data, int len, Timestamp receiveTime) const = 0;

	virtual void clear() = 0;
};

#endif //_imsghandler_h_