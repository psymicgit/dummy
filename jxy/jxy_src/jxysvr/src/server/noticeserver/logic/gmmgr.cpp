
#include "gmmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <framework/nsapplication.h>

#include "dbmgr.h"



extern CSDApplication* SDGetApp();
IMPLEMENT_SINGLETON(CGmMgr)


CGmMgr::CGmMgr()
{

}

CGmMgr::~CGmMgr()
{

}

BOOL CGmMgr::Init()
{
	return TRUE;
}


VOID CGmMgr::UnInit()
{

}