///<------------------------------------------------------------------------------
//< @file:   server\net\netaddress.cpp
//< @author: ������
//< @date:   2014��11��25�� 15:14:30
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "netaddress.h"

#include "tool/endiantool.h"

#ifndef WIN
	#include <netdb.h>
	#include <stdio.h>
#endif

NetAddress::NetAddress(uint16 port)
{
	bzero(&m_addr, sizeof m_addr);
	m_addr.sin_family		= AF_INET;
	m_addr.sin_addr.s_addr	= endiantool::HostToNetwork32(0);
	m_addr.sin_port			= endiantool::HostToNetwork16(port);
}

NetAddress::NetAddress(const std::string &ip, uint16 port)
{
	bzero(&m_addr, sizeof m_addr);

	m_addr.sin_family	= AF_INET;
	m_addr.sin_port		= endiantool::HostToNetwork16(port);

	if (::inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr) <= 0) {
		LOG_ERROR << "NetAddress parse <ip:port> = <" << ip << ":" << port << "> failed";
	}
}

string NetAddress::toIpPort() const
{
	char port[128] = {};
	sprintf_s(port, sizeof(port), "%u", toPort());

	return toIp() + ":" + port;
}

string NetAddress::toIp() const
{
	char buf[32];
	assert(sizeof buf >= INET_ADDRSTRLEN);

	::inet_ntop(AF_INET, (void*)&m_addr.sin_addr, buf, static_cast<socklen_t>(sizeof buf));
	return buf;
}

uint16 NetAddress::toPort() const
{
	return endiantool::NetworkToHost16(m_addr.sin_port);
}

bool NetAddress::resolve(string hostname, NetAddress* out)
{
#ifdef WIN
	struct hostent *remoteHost = gethostbyname(hostname.c_str());
	if (remoteHost == NULL) {
		int err = WSAGetLastError();

		if (err != 0) {
			if (err == WSAHOST_NOT_FOUND) {
				LOG_ERROR << "Host not found\n";
				return false;
			} else if (err == WSANO_DATA) {
				LOG_ERROR << "No data record found\n";
				return false;
			} else {
				LOG_ERROR << "Function failed with error: " << err;
				return false;
			}
		}
	}

	assert(remoteHost->h_addrtype == AF_INET && remoteHost->h_length == sizeof(uint32));
	out->m_addr.sin_addr = *reinterpret_cast<struct in_addr*>(remoteHost->h_addr);
	return true;

#else
	assert(out != NULL);
	struct hostent	hent;
	struct hostent* he		= NULL;
	int				herrno	= 0;

	bzero(&hent, sizeof(hent));

	char resolveBuffer[1024];

	int ret = gethostbyname_r(hostname.c_str(), &hent, resolveBuffer, sizeof resolveBuffer, &he, &herrno);
	if (ret == 0 && he != NULL) {
		assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32));
		out->m_addr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
		return true;
	} else {
		if (ret) {
			LOG_SYSTEM_ERR << "InetAddress::resolve";
		}

		return false;
	}
#endif
}
