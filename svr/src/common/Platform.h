#ifndef Platform_h__
#define Platform_h__

// Which platform are we on?

// Platform specific includes
#ifdef WIN32    //windows    
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #define __UNDEF_LEAN_AND_MEAN
  #endif

  #include <windows.h>

  #ifdef __UNDEF_LEAN_AND_MEAN
    #undef WIN32_LEAN_AND_MEAN
    #undef __UNDEF_LEAN_AND_MEAN
  #endif
#else
  #include <pthread.h>
  #include <signal.h>
  #include <sched.h>
  #include <unistd.h>
#endif

#ifdef WIN32    //windows    
    // typedef unsigned long pid_t;
    typedef CRITICAL_SECTION mutex_type;
#else           //linux
    #include<sys/socket.h>
    typedef pthread_mutex_t mutex_type;
#endif

typedef int socklen_t;
#define INET_ADDRSTRLEN  22


typedef char                int8;
typedef unsigned char       uint8;
typedef short               int16;
typedef unsigned short      uint16;
typedef int                 int32;
typedef unsigned int        uint32;

#ifdef WIN32
    typedef __int64             int64;
    typedef unsigned __int64    uint64;

    typedef int ssize_t;

    typedef int pid_t;
#else
    #include <sys/types.h>

    typedef int64_t             int64;
    typedef u_int64_t           uint64;
#endif


#endif // Platform_h__