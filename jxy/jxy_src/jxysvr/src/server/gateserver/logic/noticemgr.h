#ifndef _NOTICEMGR_H_
#define _NOTICEMGR_H_


#include <sdsingleton.h>
#include <string>
#include <map>
#include <memory>
#include <sdtime.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include <protocol/server/protocligs.h>





using namespace std;
using namespace SGDP;

typedef struct _tagDT_TIME_OUT
{
	TCHAR            aszNoticeContent[MAX_CONTENT_SIZE]; //¹«¸æÄÚÈÝ
	_tagDT_TIME_OUT()
	{
		memset(this, 0x00, sizeof(_tagDT_TIME_OUT));
	}
}DT_TIME_OUT;


typedef map<UINT8, DT_NOTICE_DATA> MAP_DT_NOTICE_DATA;
typedef MAP_DT_NOTICE_DATA::iterator MAP_DT_NOTICE_DATAItr;

typedef map<UINT8, DT_TIME_OUT> MAP_DT_TIME_OUT;
typedef MAP_DT_TIME_OUT::iterator MAP_DT_TIME_OUTItr;

class CNoticeMgr
{
    DECLARE_SINGLETON(CNoticeMgr);
    CNoticeMgr();
    ~CNoticeMgr();

public:
    BOOL Init();
    VOID UnInit();

	DT_NOTICE_DATA * GetDT_NOTICE_DATA(UINT8 byAutyType);

    MAP_DT_NOTICE_DATA &GetMAP_DT_NOTICE_DATA()
    {
        return m_mapDT_NOTICE_DATA;
    }

	DT_TIME_OUT * GetDT_TIME_OUT(UINT8 byAutyType);

	MAP_DT_TIME_OUT &GetMAP_DT_TIME_OUT()
	{
		return m_mapDT_TIME_OUT;
	}
private:
	MAP_DT_TIME_OUT		m_mapDT_TIME_OUT;
    MAP_DT_NOTICE_DATA	m_mapDT_NOTICE_DATA;
};

#endif