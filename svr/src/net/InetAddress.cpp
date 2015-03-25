// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "InetAddress.h"

#include "Endian.h"
#include "SocketsOps.h"

#include <boost/static_assert.hpp>

// INADDR_ANY use (type)value casting.
static const uint32_t kInaddrAny = INADDR_ANY;

//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

using namespace net;

BOOST_STATIC_ASSERT(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
    memset(&m_addr, 0, sizeof m_addr);
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = endian::hostToNetwork32(kInaddrAny);
    m_addr.sin_port = endian::hostToNetwork16(port);
}

InetAddress::InetAddress(const InetAddress &addr)
{
    m_addr = addr.m_addr;
}

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    memset(&m_addr, 0, sizeof m_addr);
    sockets::fromIpPort(ip.c_str(), port, &m_addr);
}

string InetAddress::IpAndPort() const
{
    char buf[32];
    sockets::toIpPort(buf, sizeof buf, m_addr);
    return buf;
}

string InetAddress::Ip() const
{
    char buf[32];
    sockets::toIp(buf, sizeof buf, m_addr);
    return buf;
}

