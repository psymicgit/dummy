#ifndef _PLUNDER_H_
#define _PLUNDER_H_

#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <logic/base/baseobj.h>
#include <logic/shmem/shmemmap.h>
#include <set>
#include <common/server/utility.h>
#include <protocligs.h>

using namespace std;

class CPlunderCapture;
class CPlunderBattleLog;

//typedef CShmemMap<UINT8, CPlunderBattleLog, MAX_PLUNDER_BATTLELOG_NUM> CPlunderBattleLogMap;//key为序号1～30
//typedef CPlunderBattleLogMap::iterator CPlunderBattleLogMapItr;
typedef deque<CPlunderBattleLog*> CPlunderBattleLogDeq;
typedef CPlunderBattleLogDeq::iterator CPlunderBattleLogDeqItr;


typedef CShmemMap<UINT32, CPlunderCapture> CPlayerID2CaptureMap;//key为俘虏的玩家ID
//typedef CPlayerID2CaptureMap::iterator CPlayerID2CaptureMapItr;
typedef CShmemMap<UINT8, CPlunderCapture> CGridIdx2CaptureMap;//key为俘虏格子序号
//typedef CGridIdx2CaptureMap::iterator CGridIdx2CaptureMapItr;

typedef map<UINT32, PKT_CLIGS_PLUNDER_REQ> CPKT_CLIGS_PLUNDER_REQMap; //key为玩家ID
typedef CPKT_CLIGS_PLUNDER_REQMap::iterator CPKT_CLIGS_PLUNDER_REQMapItr;

class CPluderBuild;
class CPlayer;


class CPlunder:public CBaseObj
{
	friend class CPluderBuild;
public:
	CPlunder();
	~CPlunder();
public:
	BOOL Init(DT_PLUNDER_DATA& stDT_PLUNDER_DATA, CPlayer* poOwner);
	DT_PLUNDER_BASE_DATA& GetDT_PLUNDER_BASE_DATA() { return m_stDT_PLUNDER_BASE_DATA; }
	//CPlayer* GetOwner() { return m_poOwner; }
	/////////////////////////////////////////////////
	UINT16 CkContiction(CPlayer* poEnemy, CPlayer* poCapture, PKT_CLIGS_PLUNDER_REQ& stPlunderReq);//检查掠夺条件
	VOID SetReqParam(UINT32 dwPlayerID, PKT_CLIGS_PLUNDER_REQ& stPlunderReq);
	PKT_CLIGS_PLUNDER_REQ& GetReqParam(UINT32 dwPlayerID);
	//VOID   CkBeRelease();//检查是否被俘虏主释放
	////////////////////////////////////////////////
	BOOL AddBattleLog(DT_PLUNDER_BATTLELOG_DATA* pstDT_PLUNDER_BATTLELOG_DATA);
	CPlunderBattleLogDeq& GetBattleLogDeq() { return m_deqPlunderBattleLog; }
	VOID GetShowPlunderBattleLog(UINT8& byShowNum, DT_PLUNDER_BATTLELOG_DATA_CLI astPlunderBattleLogInfo[MAX_PLUNDER_BATTLELOG_NUM]);//获取给客户端显示的战报
	VOID OnPlunderComplete(BOOL bSuccess, PKT_CLIGS_PLUNDER_REQ& stReqRaram);//侵略完成回调(扣除元宝/道具等)
	VOID GetPlunderData(UINT16& wCanPlunderNum, UINT16& wFreePlunderNum, UINT32& dwPlunderGold, UINT16& wPlunderItemNum, UINT32& dwCaptureOwnerPlayerID);//获取掠夺信息
	/////////////////////////////////////////////////
	UINT16 OpenCaptureTab(DT_BECAPTURE_DATA& stOwner, UINT16& wPlunderNumPerDay, UINT16& wCanPlunderNum, UINT16& wVipExtNum, UINT16& wFreeCanPlunderNum, UINT8& byCaptureNum, UINT32 &dwPlunderGold, DT_CAPTURE_DATA_CLI astCaptureInfo[MAX_CAPTURE_NUM], 
		UINT16 awUnlockGridLevel[MAX_CAPTURE_NUM], UINT8& byUnreadPlunderBattleLogNum, DT_PLUNDER_BATTLELOG_DATA_CLI& stLastBattleLog,
		DT_PLUNDER_GRID_DATA astCaptureGridInfo[MAX_CAPTURE_NUM]);
	VOID GetBeCaptureInfo(DT_BECAPTURE_DATA& stBeCaptureInfo); //获取被俘信息
	BOOL AddCapture(UINT32 dwCapturePlayerID, UINT8 byGridIdx);//添加俘虏
	BOOL DelCapture(UINT32 dwCapturePlayerID);//释放俘虏,释放对象及管理	
	BOOL OnFree();//自己是俘虏获得自由时回调
	BOOL OnCapture(UINT32 dwPlayerIDr);//自己被俘虏时回调
	VOID GetCapture(UINT8& byCaptureNum, DT_CAPTURE_DATA_CLI astCaptureInfo[MAX_CAPTURE_NUM]);//获取俘虏信息给客户端
	VOID GetCapture(UINT32 dwCaptureID, DT_CAPTURE_DATA_CLI& stCaptureInfo);
	UINT16 FreeCapture(UINT32 dwCapturePlayerID, UINT32& dwCoinIncome, UINT32& dwScienceIncome);//主动释放俘虏
	UINT16 DriveAway(UINT32& dwDriveAwayGold);
	////////////////////////////////////////////////
	UINT16 CollectIncome(UINT32 dwCapturePlayerID, UINT32& dwIncomeValue, DT_CAPTURE_WORK_DATA_CLI& stWorkInfo, UINT8& byCaptureReleaseFlag, UINT8 &byHalved); //dwIncomeValue请求收益及最终实际收益
	UINT16 BleedCapture(UINT32 dwCapturePlayerID, UINT8& byIncomeType, UINT32& dwIncomeValue);
	UINT16 SetWorkType(UINT32 dwCapturePlayerID, UINT8 byIncomeType,DT_CAPTURE_WORK_DATA_CLI& stWorkInfo);
	UINT32 GetDriveAwayGold();
	UINT16 RecvLeftIncome(UINT8 byGridIdx, UINT8& byIncomeType, UINT32& dwIncomeValue);
	////////////////////////////////////////////////
	CPlayerID2CaptureMap& GetCaptureMap() { return m_mapCapture; }
	CPlunderCapture* GetCapture(UINT32 dwCapturePlayerID);	
	////////////////////////////////////////////////
	SRecommendPlayerRcd&	GetRecommendPlayerRcd() { return m_stRecommendPlayerRcd; }

	///////////////
	UINT16	GetCanPlunderNum(); //获取可掠夺次数
	UINT16	GetFreePlunderNum(); //获取可掠夺次数
	UINT32   GetPlunderGold();		//下次要发费的元宝
	BOOL	HaveIdleGrid();//释放有空闲格子
	set<UINT32> GetEnemySet();
protected:
	BOOL InitBattleLog(const DT_PLUNDER_BATTLELOG_DATA_LIST& stPlunderBattleLog);
	BOOL InitCapture(const DT_CAPTURE_DATA_LIST& stCaptureData);
	VOID CkCaptureRelease();
	VOID CkBeRelease();
	/////////////////////////////////////////////////////////////	
	UINT16	CkIdleCaptureGrid(UINT8 byCaptureGridIdx);//检查是否有空闲俘虏格子
	UINT16	CkOutPlunderLevelRang(CPlayer* poEnemy, UINT8 byUseItemFlag);//检查是否超出掠夺等级范围	
	UINT16	CkYourCapture(CPlayer* poEnemy);	//检查是否已是你的俘虏
	UINT16	CkPlunderNeedGold();//检查收费掠夺元宝是否足够	
	UINT16	CkPlunderType(CPlayer* poEnemy, CPlayer* poCapture); //检查是否掠夺方式类型
	//////////////////////////////////////////
	VOID	CaptureBefree(UINT32 dwCapturePlayerID);//俘虏被释放
	/////////////////////////////////////////		

private:
	DT_PLUNDER_BASE_DATA	m_stDT_PLUNDER_BASE_DATA;
	UINT32					m_dwPlayerID;//用于缓存，不需每次查找	
	CPlunderBattleLogDeq	m_deqPlunderBattleLog;//用于按挑战顺序时间记录初始化对象
	CPlayerID2CaptureMap	m_mapCapture;//key为俘虏ID
	CGridIdx2CaptureMap		m_mapCridCapture;//key为俘虏格子序号
	CPKT_CLIGS_PLUNDER_REQMap	m_mapPlunderReq;//掠夺请求参数，用于保存掠夺请求参数，异步取数据。同个玩家瞬间多次请求则覆盖只取最后一次
	UINT16					m_wUseItemNum;//使用的道具个数，临时保存
	UINT32					m_dwPlunderNeedGold;//收费掠夺需要的元宝
	SRecommendPlayerRcd		m_stRecommendPlayerRcd;//用于记录上次推荐的玩家ID
	UINT32					m_dwCaptureKeepSecond;//俘虏拥有保持时间(单位小时，过了该时间则自动释放). 用于缓存，不需每次取
	//UINT32					m_dwDriveAwayBaseGold;	// 驱赶俘虏主所需基本元宝. 用于缓存，不需每次取
	//UINT32					m_dwDriveAwayIncGold;	// 每多驱赶俘虏主1次所需递增元宝. 用于缓存，不需每次取
	UINT16					m_wPlunderLowerLevelItemID;// 掠夺低自己5级以上的道具. 用于缓存，不需每次取
};
DECLARE_CLASS_POOL_MGR(CPlunder)


#endif
