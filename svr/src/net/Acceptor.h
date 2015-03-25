#ifndef Acceptor_h__
#define Acceptor_h__

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "Channel.h"
#include "Socket.h"

#include <net/InetAddress.h>

namespace net
{
    class EventLoop;
    class InetAddress;

    ///
    /// Acceptor of incoming TCP connections.
    ///
    class Acceptor : boost::noncopyable
    {
    public:
        typedef boost::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

        Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
        ~Acceptor();

        void setNewConnectionCallback(const NewConnectionCallback& cb){ newConnectionCallback_ = cb; }

        bool listenning() const { return m_listenning; }
        void listen();

    public:

        const InetAddress m_listenAddr;

        EventLoop* m_loop;
        Socket m_acceptSocket;
        NewConnectionCallback newConnectionCallback_;
        bool m_listenning;
        int idleFd_;
    };

}

#endif // Acceptor_h__