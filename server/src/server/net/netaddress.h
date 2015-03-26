///<------------------------------------------------------------------------------
//< @file:   server\net\netaddress.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:14:15
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netaddress_h_
#define _netaddress_h_

#include <string>

using namespace std;

class NetAddress
{
public:
	/// Constructs an endpoint with given port number.
	/// Mostly used in TcpServer listening.
	explicit NetAddress(uint16_t port = 0, bool loopbackOnly = false);

	/// Constructs an endpoint with given ip and port.
	/// @c ip should be "1.2.3.4"
	NetAddress(const std::string &ip, uint16_t port);

	/// Constructs an endpoint with given struct @c sockaddr_in
	/// Mostly used when accepting new connections
	NetAddress(const struct sockaddr_in& addr)
		: m_addr(addr)
	{
	}

	string toIp() const;
	string toIpPort() const;
	uint16_t toPort() const;

	// default copy/assignment are Okay

	const struct sockaddr_in& getSockAddr() const { return m_addr; }
	void setSockAddr(const struct sockaddr_in& addr) { m_addr = addr; }

	uint32_t ipNetEndian() const { return m_addr.sin_addr.s_addr; }
	uint16_t portNetEndian() const { return m_addr.sin_port; }

	static bool resolve(string hostname, NetAddress* result);

public:
	struct sockaddr_in m_addr;
};

#endif //_netaddress_h_