
#ifndef _PLUNDERCAPTURE_H_
#define _PLUNDERCAPTURE_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>

class CPlayer;
class CPlunder;

struct tagDT_CAPTURE_DATA_CLI;
typedef tagDT_CAPTURE_DATA_CLI DT_CAPTURE_DATA_CLI;

struct tagDT_CAPTURE_WORK_DATA_CLI;
typedef tagDT_CAPTURE_WORK_DATA_CLI DT_CAPTURE_WORK_DATA_CLI;


class CPlunderCapture:public CBaseObj
{
public:
	CPlunderCapture();
	~CPlunderCapture();
public:
	BOOL Init(const DT_CAPTURE_DATA* pstDT_CAPTURE_DATA, CPlayer* poOwner, CPlunder* poPlunder);
	DT_CAPTURE_DATA& GetDT_CAPTURE_DATA() { return m_stDT_CAPTURE_DATA; }
	VOID GetDT_CAPTURE_DATA_CLI(DT_CAPTURE_DATA_CLI& stDT_CAPTURE_DATA_CLI); //判断俘虏是否需释放，并获取信息给客户端，需释放则返回FALSE，不需下发给客户端
	
	//BOOL CkNeedRelease();//检测是否需释放,返回TRUE表示需释放	
	/////////////////////////////////////
	UINT16 SetWorkType(UINT8 byIncomeType,DT_CAPTURE_WORK_DATA_CLI& stWorkInfo);
	UINT16 CollectIncome(UINT32& dwIncomeValue, DT_CAPTURE_WORK_DATA_CLI& stWorkInfo, UINT8 &byHalved);
	UINT16 BleedCapture(UINT8& byIncomeType, UINT32& dwIncomeValue);
protected:
protected:
	VOID GetDT_CAPTURE_WORK_DATA_CLI(DT_CAPTURE_WORK_DATA_CLI& stDT_CAPTURE_WORK_DATA_CLI);
private:
    DT_CAPTURE_DATA	m_stDT_CAPTURE_DATA;
    CPlunder*		m_poPlunder;
    UINT32			m_dwCaptureKeepSecond;		//俘虏拥有保持时间(单位小时，过了该时间则自动释放). 用于缓存，不需每次取
    UINT32			m_dwCaptureWorkSecond;		//俘虏工作时间,用于缓存，不需每次取
    //UINT32			m_dwWorkCoinPerSec;		//每秒工作收获铜币,用于缓存，不需每次取
    //UINT32			m_dwWorkSciencePerSec;		//每秒工作收获器魂,用于缓存，不需每次取
};


#endif

