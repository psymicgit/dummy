#ifndef _xdsentry_h_
#define _xdsentry_h_

#include <stddef.h>

class xdsentry
{
public:
    xdsentry() :
        m_elements(NULL),
        m_elemcnt(0)
    {
    }

    bool isempty(){ return 0 == m_elemcnt || NULL == m_elements; }

public:
    int m_elemcnt;
    void* m_elements;
};

#endif // _xdsentry_h_
