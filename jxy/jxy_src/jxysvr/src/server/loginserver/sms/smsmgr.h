#ifndef _SMS_MGR_H_
#define _SMS_MGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <sdstring.h>
#include <sdthread.h>
#include <deque>
#include <map>
#include <dll/sddb/sddb.h>
#include <list>
#include "basesmscmd.h"
#include "bindsmscmd.h"
#include "unbindsmscmd.h"


using namespace SGDP;
using namespace std;






typedef std::list<CBaseSmsCMD*>		CBaseSmsCMDList;
typedef CBaseSmsCMDList::iterator	CBaseSmsCMDListItr;





class CSmsMgr: public CSDThread
{
    DECLARE_SINGLETON(CSmsMgr);
public:
    CSmsMgr();
    virtual ~CSmsMgr();
    BOOL Init();
    VOID UnInit();
    BOOL Run();

	CBaseSmsCMD*	CreateCmd(enum ESMSCmdType eSmsCmdType);
	VOID			FreeCmd(CBaseSmsCMD* poBaseDBCMD);

	BOOL			AddCommand(CBaseSmsCMD* poDBCommand);


private:
    const static UINT32 ms_dwSleepTime = 1; //睡眠的时间间隔
protected:
    //线程
    virtual VOID  ThrdProc();

private:
    VOID Stop();


private:
    BOOL m_bStop;
    time_t m_tLastWriteTime; //上次写入数据库的时间
    //正打算要执行的数据
    CBaseSmsCMDList	m_oBaseSmsCMDListing;
    CSDMutex			m_ListingMutex;
    //执行完成的数据
    CBaseSmsCMDList	m_oBaseSmsCMDListed;
    CSDMutex			m_ListedMutex;

    CSDObjectPool<CBindSmsCMD, CSDMutex>	m_oBindSmsCMDPool;
	CSDObjectPool<CUnBindSmsCMD, CSDMutex>	m_oUnBindSmsCMDPool;
};


#endif