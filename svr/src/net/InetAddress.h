// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_INETADDRESS_H
#define MUDUO_NET_INETADDRESS_H

#include <winsock2.h>

#include <base/copyable.h>
// #include <base/StringPiece.h>

#include <string>

#include <boost/cstdint.hpp>

using namespace boost;
using std::string;

namespace net
{
    ///
    /// Wrapper of sockaddr_in.
    ///
    /// This is an POD interface class.
    class InetAddress : public copyable
    {
    public:
        /// Constructs an endpoint with given port number.
        /// Mostly used in TcpServer listening.
        explicit InetAddress(uint16_t port);

        InetAddress(const InetAddress& addr);

        /// Constructs an endpoint with given ip and port.
        /// @c ip should be "1.2.3.4"
        InetAddress(const std::string& ip, uint16_t port);

        /// Constructs an endpoint with given struct @c sockaddr_in
        /// Mostly used when accepting new connections
        InetAddress(const struct sockaddr_in& addr)
            : m_addr(addr)
        { }

        string Ip() const;
        string IpAndPort() const;

        // default copy/assignment are Okay

        const struct sockaddr_in& getSockAddrInet() const { return m_addr; }
        void setSockAddrInet(const struct sockaddr_in& addr) { m_addr = addr; }

        uint32_t ipNetEndian() const { return m_addr.sin_addr.s_addr; }
        uint16_t portNetEndian() const { return m_addr.sin_port; }

    public:
        struct sockaddr_in m_addr;
    };
}

#endif  // MUDUO_NET_INETADDRESS_H
