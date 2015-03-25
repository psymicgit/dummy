// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Channel.h"

#include <WinSock2.h>

#include <base/Logging.h>

#include "EventLoop.h"

#include <sstream>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>

using namespace net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


void read_cb(struct bufferevent *incoming, void *arg) 
{  
    Channel *pChannel = (Channel*)arg;
    pChannel->readCallback_(*incoming);
}

void error_cb(struct bufferevent *bev, short events, void *arg) 
{
    Channel *pChannel = (Channel*)arg;

    evutil_socket_t fd = bufferevent_getfd(bev);  
    if (events & BEV_EVENT_TIMEOUT) 
    {
        printf("connection<fd = %u> timed out\n", fd); //if bufferevent_set_timeouts() called  
    }  
    else if (events & BEV_EVENT_EOF) 
    {
        struct evbuffer *input = bufferevent_get_input(bev);

        size_t len = evbuffer_get_length(input);
        printf("Got a close from %s. We have %lu left.\n", "<><><><>", (unsigned long)len);

        pChannel->closeCallback_();
        std::cout << "connection <fd = " << fd << "> closed" << std::endl;  
        bufferevent_free(bev);
    }  
    else if (events & BEV_EVENT_ERROR) 
    {
        printf("connection<fd = %u> has some other error\n", fd);  
        if(pChannel->errorCallback_)
        {
            pChannel->errorCallback_();
        }
        if(pChannel->closeCallback_)
        {
            pChannel->closeCallback_();
        }

        bufferevent_free(bev);
    }
}  

void write_cb(struct bufferevent *bev, void *arg)
{
    Channel *pChannel = (Channel*)arg;
    pChannel->writeCallback_(*bev);
}


Channel::Channel(EventLoop* loop, int fd__)
  : loop_(loop),
    m_sockfd(fd__),
    tied_(false),
    eventHandling_(false)
{
}

Channel::~Channel()
{
    assert(!eventHandling_);
}

void Channel::Init()
{
    m_bev = bufferevent_socket_new(loop_->m_eventBase, m_sockfd, BEV_OPT_CLOSE_ON_FREE);  
    bufferevent_setcb(m_bev, read_cb, write_cb, error_cb, this);  

    bufferevent_setwatermark(m_bev, EV_WRITE, 0, 102400);
}


void Channel::tie(const boost::shared_ptr<void>& obj)
{
    tie_ = obj;
    tied_ = true;
}

bool Channel::send(const void* data, size_t len)
{
    return bufferevent_write(m_bev, data, len) == 0;
}


bool Channel::isWriting()
{
    short event = bufferevent_get_enabled(m_bev);
    return ((event & EV_WRITE) > 0);
}

void Channel::enableReading()
{
    bufferevent_enable(m_bev, EV_READ|EV_PERSIST);  
}

void Channel::enableWriting()
{
    bufferevent_enable(m_bev, EV_WRITE|EV_PERSIST);  
}

void Channel::disableWriting()
{ 
    bufferevent_disable(m_bev, EV_WRITE|EV_PERSIST);
}

void Channel::disableAll()
{ 
    bufferevent_disable(m_bev, EV_READ|EV_WRITE|EV_PERSIST);

}