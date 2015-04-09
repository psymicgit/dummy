#include "sdrandom.h"
#include <time.h>
#include <stdlib.h>

namespace SGDP
{

    VOID SDSetSeed(UINT32 seed)
    {
        srand(seed);
    }

    INT32 SDRandom()
    {
        return rand();
    }

    UINT32 SDRandom(UINT32 range)
    {
		if (0 == range)
		{
			return 0;
		}
		if (range > RAND_MAX)
		{
			return rand() / (RAND_MAX + 1) * range; 
		}
		return rand() % range; 
    }

    CSDRandom::CSDRandom()
    {
        m_seed = (UINT32)time(NULL);
    }

    UINT32 CSDRandom::Random()
    {
        UINT32 next = m_seed;
        UINT32 result;

        next *= 1103515245;
        next += 12345;
        result = (UINT32) (next / 65536) % 2048;

        next *= 1103515245;
        next += 12345;
        result <<= 10;
        result ^= (UINT32) (next / 65536) % 1024;

        next *= 1103515245;
        next += 12345;
        result <<= 10;
        result ^= (UINT32) (next / 65536) % 1024;

        m_seed = next;
        return result;
    }


    UINT32 CSDRandom::Random(UINT32 unRange)
    {
        if ( unRange == 0 )
        {
            return 0;
        }
        return Random() % unRange;
    }

    UINT32 CSDRandom::Random(UINT32 unMinRange, UINT32 unMaxRange)
    {
        if ( unMinRange == unMaxRange )
        {
            return unMaxRange;
        }

        return Random(unMaxRange - unMinRange) + unMinRange;
    }
}


