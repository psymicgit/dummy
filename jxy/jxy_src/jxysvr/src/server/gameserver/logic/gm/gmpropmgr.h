
#ifndef _FORMATIONPROPMGR_H_
#define _FORMATIONPROPMGR_H_

#include <logic/base/propcontainer.h>
#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>


class CGmPropMgr
{
public:

    DECLARE_SINGLETON(CGmPropMgr);

public:

    BOOL Init();

    VOID UnInit();
public:
	

protected:
    CGmPropMgr();
    virtual ~CGmPropMgr();
protected:

	BOOL LoadFromDB();

private:
	
};

#endif // 
