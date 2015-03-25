#include "keyutil.h"

namespace keyutil
{
    uint64_t Get16161616Key(word w, word x, word y, word z)
    {
        uint64_t key_w = w;
        key_w <<= 32;

        uint64_t key_x = x;
        key_x <<= 32;

        uint64_t key_y = y;
        key_y <<= 32;

        uint64_t key = key_w + key_x + key_y + z;
        return key;
    }

    uint64_t Get161616Key(word x, word y, word z)
    {
        uint64_t key_x = x;
        key_x <<= 32;

        uint64_t key_y = y;
        key_y <<= 32;

        uint64_t key = key_x + key_y + z;
        return key;
    }

    uint64_t Get3232Key(dword x, dword y)
    {
        uint64_t key_x = x;
        key_x <<= 32;

        uint64_t key = key_x + y;
        return key;
    }
}