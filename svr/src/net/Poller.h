#ifndef Poller_h__
#define Poller_h__

#include <vector>
#include <boost/noncopyable.hpp>

#include <base/Timestamp.h>
#include "EventLoop.h"

namespace net
{
    class Channel;
}

using namespace net;

namespace net
{

    ///
    /// Base class for IO Multiplexing
    ///
    /// This class doesn't own the Channel objects.
    class Poller : boost::noncopyable
    {
    public:
        typedef std::vector<Channel*> ChannelList;

        Poller(EventLoop* loop);
        virtual ~Poller();

        /// Polls the I/O events.
        /// Must be called in the loop thread.
        virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

        /// Changes the interested I/O events.
        /// Must be called in the loop thread.
        virtual void updateChannel(Channel* channel) = 0;

        /// Remove the channel, when it destructs.
        /// Must be called in the loop thread.
        virtual void removeChannel(Channel* channel) = 0;

        static Poller* newDefaultPoller(EventLoop* loop)
        {
            return NULL;
        }

        void assertInLoopThread()
        {
            ownerLoop_->assertInLoopThread();
        }

    private:
        EventLoop* ownerLoop_;
    };
}
#endif // Poller_h__
