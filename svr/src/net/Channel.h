// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <base/Timestamp.h>

using std::string;

struct bufferevent;

namespace net
{

    class EventLoop;

    ///
    /// A selectable I/O channel.
    ///
    /// This class doesn't own the file descriptor.
    /// The file descriptor could be a socket,
    /// an eventfd, a timerfd, or a signalfd
    class Channel : boost::noncopyable
    {
    public:
        typedef boost::function<void()> EventCallback;
        // typedef boost::function<void(Timestamp)> ReadEventCallback;
        
        typedef boost::function<void(bufferevent&)> ReadEventCallback;
        typedef boost::function<void(bufferevent&)> WriteEventCallback;

        Channel(EventLoop* loop, int fd);
        ~Channel();

        void Init();

        void handleEvent(Timestamp receiveTime);
        void setReadCallback(const ReadEventCallback& cb){ readCallback_ = cb; }
        void setWriteCallback(const WriteEventCallback& cb){ writeCallback_ = cb; }
        void setCloseCallback(const EventCallback& cb){ closeCallback_ = cb; }
        void setErrorCallback(const EventCallback& cb){ errorCallback_ = cb; }

        // 发送数据，调用后，当其发送完成，将会自动调用writeCallback_
        bool send(const void* data, size_t len);

        /// Tie this channel to the owner object managed by shared_ptr,
        /// prevent the owner object being destroyed in handleEvent.
        void tie(const boost::shared_ptr<void>&);

        int fd() const { return m_sockfd; }
        // int revents() const { return revents_; }
        void enableReading();
        void enableWriting();
        void disableWriting();
        void disableAll();
        bool isWriting();// const { return (events_ & kWriteEvent) == 0 ? false : true; }

        EventLoop* ownerLoop() { return loop_; }

    private:

        static const int kNoneEvent;
        static const int kReadEvent;
        static const int kWriteEvent;

        EventLoop* loop_;
        

        boost::weak_ptr<void> tie_;
        bool tied_;
        bool eventHandling_;

        struct bufferevent *m_bev;

    public:
        int  m_sockfd;

        ReadEventCallback readCallback_;
        WriteEventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;
    };

}
#endif  // MUDUO_NET_CHANNEL_H
