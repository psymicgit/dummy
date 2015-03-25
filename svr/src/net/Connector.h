// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_CONNECTOR_H
#define MUDUO_NET_CONNECTOR_H

#include "InetAddress.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace net
{
    class Channel;
    class EventLoop;

    class Connector : boost::noncopyable,
        public boost::enable_shared_from_this<Connector>
    {
    public:
        typedef boost::function<void (int sockfd)> NewConnectionCallback;

        Connector(EventLoop* loop, const InetAddress& serverAddr);
        ~Connector();

        void setNewConnectionCallback(const NewConnectionCallback& cb){ m_newConnectionCallback = cb; }

        void start();  // can be called in any thread
        void restart();  // must be called in loop thread
        void stop();  // can be called in any thread

        const InetAddress& serverAddress() const { return m_serverAddr; }

    private:
        enum States { kDisconnected, kConnecting, kConnected };
        static const int kMaxRetryDelayMs;
        static const int kInitRetryDelayMs;

        void setState(States s) { state_ = s; }
        void startInLoop();
        void stopInLoop();
        void connect();
        
        void handleWrite();
        void handleError();
        
        int removeAndResetChannel();
        void resetChannel();

    public:
        void connected(int sockfd);
        void retry();

    public:
        EventLoop* m_loop;
        InetAddress m_serverAddr;
        bool m_enableConnect; // atomic
        States state_;  // FIXME: use atomic variable
        NewConnectionCallback m_newConnectionCallback;
        int retryDelayMs_;
    };
}

#endif  // MUDUO_NET_CONNECTOR_H
