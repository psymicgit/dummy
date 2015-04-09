#ifndef _GSNOTICEMGR_H_
#define _GSNOTICEMGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>

#include <protocommondata.h>

using namespace SGDP;


class CPlayer;

class CGsNoticeMgr
{
public:
    DECLARE_SINGLETON(CGsNoticeMgr)

    BOOL Init();

    VOID UnInit();

    UINT32 OnEvent();

    VOID SetGsNoticeData(const DT_NOTICE_DATA_DB& stGsNoticeInfo);

	VOID GetGsNoticeData(DT_NOTICE_DATA_NEW& stGsNoticeInfo);
    
protected:
    CGsNoticeMgr();
    ~CGsNoticeMgr();

protected:
	BOOL InitGsNoticeFromDB();


private:
   DT_NOTICE_DATA_NEW		m_stGsNoticeInfo;//公告信息
   DT_NOTICE_DATA_DB    m_stGsNoticeInfoDB;
   UINT16				m_wZoneID;
   BOOL					m_bSendFlag; //0 还未发送， 1 过期已发送
};
#endif
