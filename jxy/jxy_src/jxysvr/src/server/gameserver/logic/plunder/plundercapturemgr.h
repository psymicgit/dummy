
#ifndef _PLUNDERCAPTUREMGR_H_
#define _PLUNDERCAPTUREMGR_H_

#include "plundercapture.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <common/client/commondef.h>

using namespace SGDP;
using namespace std;


typedef HashMap<UINT64, CPlunderCapture*> CID2PlunderCaptureMap;
typedef CID2PlunderCaptureMap::iterator	CID2PlunderCaptureMapItr;


class CPlunderCaptureMgr
{

public:
    DECLARE_SINGLETON(CPlunderCaptureMgr)

    BOOL Init();

    VOID UnInit();

public:

	CPlunderCapture*	CreatePlunderCapture(UINT32 dwPlayerID, UINT32 dwCapturePlayerID);
	VOID				RemovePlunderCapture(UINT32 dwPlayerID, UINT32 dwCapturePlayerID);

	
protected:
    CPlunderCaptureMgr();
    ~CPlunderCaptureMgr();


private:
	CID2PlunderCaptureMap				m_mapID2PlunderCapture;
	CSDObjectPool<CPlunderCapture, CSDMutex> m_oPlunderCaptureShmemObjMgr;
};



#endif

