#include "sdatomic.h"
#include "sddebug.h"
#if (defined(WIN32) || defined(WIN64))
#pragma warning(disable: 4146)
#endif

namespace SGDP
{


    INT32  SDAtomicAdd32(volatile INT32 *mem, INT32 val)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedExchangeAdd((volatile LONG *)mem, val);
#else
        asm volatile ("lock; xaddl %0,%1"
                      : "=r" (val), "=m" (*mem)
                      : "0" (val), "m" (*mem)
                      : "memory", "cc");
        return val;
#endif
    }


    INT64  SDAtomicAdd64(volatile INT64 *mem, INT64 val)
    {
#if (defined(WIN64))
        return InterlockedExchangeAdd64(mem, val);
#elif (defined(LINUX64))
		asm volatile ("lock; xaddq %0,%1"
			: "=r" (val), "=m" (*mem)
			: "0" (val), "m" (*mem)
			: "memory", "cc");
		return val;
#endif
		SDASSERT(FALSE); 
		return *mem; 
    }


    INT32  SDAtomicSub32(volatile INT32 *mem, INT32 val)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedExchangeAdd((volatile LONG *)mem, -val);
#elif (defined(LINUX32) || defined(LINUX64))
        asm volatile ("lock; subl %1, %0"
                      : /* no output */
                      : "m" (*(mem)), "r" (val)
                      : "memory", "cc");
        return *mem + 1;
#endif
    }


    INT64  SDAtomicSub64(volatile INT64 *mem, INT64 val)
    {
#if (defined(WIN64))
        return InterlockedExchangeAdd64(mem, -val);
#elif (defined(LINUX64))
		asm volatile ("lock; subq %1, %0"
			: /* no output */
			: "m" (*(mem)), "r" (val)
			: "memory", "cc");
		return *mem + 1;
#endif
		SDASSERT(FALSE); 
		return *mem; 
    }


    INT32  SDAtomicInc32(volatile INT32 *mem)
    {
        /* we return old value, win32 returns SDNew value :( */
#if (defined(WIN32) || defined(WIN64))
        return InterlockedIncrement((volatile LONG *)mem) - 1;
#elif (defined(LINUX32) || defined(LINUX64))
        return SDAtomicAdd32(mem, 1);
#endif
    }



    INT64  SDAtomicInc64(volatile INT64 *mem)
    {
        /* we return old value, win32 returns SDNew value :( */
#if (defined(WIN64))
        return InterlockedIncrement64(mem) - 1;
#elif (defined(LINUX64))
        return SDAtomicAdd64(mem, 1);
#endif
		SDASSERT(FALSE); 
		return *mem; 
    }


    INT32  SDAtomicDec32(volatile INT32 *mem)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedDecrement((volatile LONG *)mem) + 1;
#elif (defined(LINUX32) || defined(LINUX64))
        unsigned char prev;
        asm volatile ("lock; decl %0; setnz %1"
                      : "=m" (*mem), "=qm" (prev)
                      : "m" (*mem)
                      : "memory");

        return prev + 1;
#endif
    }


    INT64  SDAtomicDec64(volatile INT64 *mem)
    {
#if (defined(WIN64))
        return InterlockedDecrement64(mem) + 1;
#elif (defined(LINUX64))
        return SDAtomicSub64(mem, 1);
#endif
		SDASSERT(FALSE); 
		return *mem; 
    }


    void  SDAtomicSet32(volatile INT32 *mem, INT32 val)
    {
#if (defined(WIN32) || defined(WIN64))
        InterlockedExchange((volatile LONG *)mem, val);
#else
        *mem = val;
#endif
    }


    void  SDAtomicSet64(volatile INT64 *mem, INT64 val)
    {
#if (defined(WIN64))
        InterlockedExchange64((INT64*)mem, val);
#elif (defined(LINUX64))
        *mem = val;
#endif
    }


    INT32  SDAtomicRead32(volatile INT32 *mem)
    {
        return *mem;
    }


    INT64  SDAtomicRead64(volatile INT64 *mem)
    {
        return *mem;
    }


    INT32  SDAtomicCas32(volatile INT32 *mem, INT32 with, INT32 cmp)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedCompareExchange((volatile LONG *)mem, with, cmp);
#elif (defined(LINUX32) || defined(LINUX64))
        UINT32 prev;
        asm volatile ("lock; cmpxchgl %1, %2"
                      : "=a" (prev)
                      : "r" (with), "m" (*(mem)), "0"(cmp)
                      : "memory", "cc");
        return prev;
#endif
    }


    INT64  SDAtomicCas64(volatile INT64 *mem, INT64 with, INT64 cmp)
    {
#if (defined(WIN64))
        return InterlockedCompareExchange64(mem,with,cmp);
#elif (defined(LINUX64))
		UINT32 prev;
		asm volatile ("lock; cmpxchgq %1, %2"
			: "=a" (prev)
			: "r" (with), "m" (*(mem)), "0"(cmp)
			: "memory", "cc");
		return prev;
#endif
		SDASSERT(FALSE); 
		return *mem; 
    }


    INT32  SDAtomicXchg32(volatile INT32 *mem, INT32 val)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedExchange((volatile LONG *)mem, val);
#elif (defined(LINUX32) || defined(LINUX64))
        INT32 prev = val;
        asm volatile ("xchgl %0, %1"
                      : "=r" (prev), "+m" (*mem)
                      : "0" (prev));
        return prev;
#endif
    }


    INT64  SDAtomicXchg64(volatile INT64 *mem, INT64 val)
    {
#if (defined(WIN64))
        return InterlockedExchange64(mem, val);
#elif (defined(LINUX64))
		INT32 prev = val;
		asm volatile ("xchg %0, %1"
			: "=r" (prev), "+m" (*mem)
			: "0" (prev));
		return prev;
#endif
		SDASSERT(FALSE); 
		return *mem; 
    }


    void*  SDAtomicCasptr(volatile void **mem, void *with, const void *cmp)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedCompareExchangePointer((void* volatile*)mem, with, (void*)cmp);
#elif defined(LINUX64)
        void * prev;
       asm volatile ("lock; cmpxchgq %q2, %1"
                      : "=a" (prev), "=m" (*mem)
                      : "r" ((unsigned long)with), "m" (*mem),
                      "0" ((unsigned long)cmp));
        return prev;
#else
        void * prev;
        asm volatile ("lock; cmpxchgl %2, %1"
                      : "=a" (prev), "=m" (*mem)
                      : "r" (with), "m" (*mem), "0" (cmp));
        return prev;
#endif
    }


    void*  SDAtomicXchgptr(volatile void **mem, void *with)
    {
#if (defined(WIN32) || defined(WIN64))
        return InterlockedExchangePointer((void**)mem, with);
#elif defined(LINUX64)
        void * prev;
        asm volatile ("xchgq %q2, %1"
                      : "=a" (prev), "+m" (*mem)
                     : "r" ((unsigned long)with));
        return prev;
#else
        void * prev;
        asm volatile ("xchgl %2, %1"
                      : "=a" (prev), "+m" (*mem)
                      : "0" (with));
        return prev;
#endif
    }

}

