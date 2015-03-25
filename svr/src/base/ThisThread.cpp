#include "ThisThread.h"

// #include <pthread.h>


thread::id this_thread::get_id()
{
#ifdef WIN32
    return thread::id((unsigned long int) GetCurrentThreadId());
#elif
    return _pthread_t_to_ID(pthread_self());
#endif
}

void this_thread::yield()
{
#ifdef WIN32
    Sleep(0);
#else
    sched_yield();
#endif
}