#ifndef _keyutil_h_
#define _keyutil_h_

namespace keyutil
{
    uint64_t Get8888Key(word x, word y, word z);
    
    uint64_t Get16161616Key(word w, word x, word y, word z);
    
    uint64_t Get161616Key(word x, word y, word z);
    
    uint64_t Get3232Key(dword x, dword y);
}

#endif // _keyutil_h_
