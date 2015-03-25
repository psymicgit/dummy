///<------------------------------------------------------------------------------
//< @file:   server\net\session.h
//< @author: 洪坤安
//< @date:   2015年3月14日 23:51:18
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _session_h_
#define _session_h_

// 会话，基于连接
class ISession
{
public:
	virtual void onEstablish() = 0;

	virtual void onTerminate() = 0;

	virtual void onRecv(Link&, Buffer&) = 0;

	virtual void setLink(Link&) = 0;
};

// 会话工厂，专门用于创建会话
class ISessionFactory
{
public:
	virtual ISession* createSession(Link&) = 0;
};

#endif //_session_h_