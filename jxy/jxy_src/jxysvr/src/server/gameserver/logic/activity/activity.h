
#ifndef _ACTIVITY_H_
#define _ACTIVITY_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>

struct	tagDT_ACTIVITY_BASE_DATA;
typedef	tagDT_ACTIVITY_BASE_DATA DT_ACTIVITY_BASE_DATA;

class CActivity: public CBaseObj
{
public:
    CActivity() {} ;
    ~CActivity() {} ;
public:
    virtual VOID GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA) = 0;
protected:

private:

};


#endif

