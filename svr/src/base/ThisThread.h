#include "Chrono.h"
#include <common/Platform.h>

namespace thread
{
    typedef pid_t id;
}

/// The namespace @c this_thread provides methods for dealing with the
/// calling thread.
namespace this_thread {
    /// Return the thread ID of the calling thread.
    thread::id get_id();

    /// Yield execution to another thread.
    /// Offers the operating system the opportunity to schedule another thread
    /// that is ready to run on the current processor.
    void yield();

    /// Blocks the calling thread for a period of time.
    /// @param[in] aTime Minimum time to put the thread to sleep.
    /// Example usage:
    /// @code
    /// // Sleep for 100 milliseconds
    /// this_thread::sleep_for(chrono::milliseconds(100));
    /// @endcode
    /// @note Supported duration types are: nanoseconds, microseconds,
    /// milliseconds, seconds, minutes and hours.
    /// 
/*
    template <class _Rep, class _Period> 
    void sleep_for(const chrono::duration<_Rep, _Period>& aTime)
    {
#ifdef WIN32
        Sleep(int(double(aTime.count()) * (1000.0 * _Period::_as_double()) + 0.5));
#else
        usleep(int(double(aTime.count()) * (1000000.0 * _Period::_as_double()) + 0.5));
#endif
    }
*/
}
