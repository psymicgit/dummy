
#ifndef _FORMATIONMGR_H_
#define _FORMATIONMGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <string>

using namespace std;
using namespace SGDP;


class CGmMgr
{
public:
    DECLARE_SINGLETON(CGmMgr)

    BOOL Init();

    VOID UnInit();

protected:

protected:
    CGmMgr();
    ~CGmMgr();


private:
 
};




#endif //#ifndef _ROLEMGR_H_

