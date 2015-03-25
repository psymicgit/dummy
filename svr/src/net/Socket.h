#ifndef Socket_h__
#define Socket_h__

#include <boost/noncopyable.hpp>

#include <event2/util.h>


///
/// TCP networking.
///
namespace net
{
    class InetAddress;

    ///
    /// Wrapper of socket file descriptor.
    ///
    /// It closes the sockfd when desctructs.
    /// It's thread safe, all operations are delagated to OS.
    class Socket : boost::noncopyable
    {
    public:
        explicit Socket(evutil_socket_t sockfd);

        Socket();

        ~Socket();

        evutil_socket_t fd() const { return m_sockfd; }

        /// abort if address in use
        void bindAddr(const InetAddress& localaddr);
        /// abort if address in use
        void listen();

        /// On success, returns a non-negative integer that is
        /// a descriptor for the accepted socket, which has been
        /// set to non-blocking and close-on-exec. *peeraddr is assigned.
        /// On error, -1 is returned, and *peeraddr is untouched.
        evutil_socket_t accept(InetAddress& peeraddr);

        void shutdownWrite();

        ///
        /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
        ///
        void setTcpNoDelay(bool on);

        // 将描述符设置为非阻塞
        void NonBlocking();

        ///
        /// Enable/disable SO_REUSEADDR
        ///
        void enableReuseAddr();

        ///
        /// Enable/disable SO_REUSEPORT
        ///
        void setReusePort(bool on);

        ///
        /// Enable/disable SO_KEEPALIVE
        ///
        void setKeepAlive(bool on);

    public:
        evutil_socket_t m_sockfd;
    };
}

#endif // Socket_h__
