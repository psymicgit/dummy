
#ifndef _ENCOURAGEMGR_H_
#define _ENCOURAGEMGR_H_

#include <sdsingleton.h>

using namespace std;
using namespace SGDP;

class CEncourageMgr
{
public:

	DECLARE_SINGLETON(CEncourageMgr);

public:
	BOOL Init();
	VOID UnInit();

public:


protected:
	CEncourageMgr();
	virtual ~CEncourageMgr();
};

#endif 
