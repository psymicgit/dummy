
#ifndef _PLAYERBASEDATACACHEMGR_H
#define _PLAYERBASEDATACACHEMGR_H

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <sddb.h>
#include <vector>

using namespace std;
using namespace SGDP;

typedef std::vector<UINT32> CPlayerIDVec;

//异步需同步信息
struct SSyncProp
{
    SSyncProp()
    {
        Clr();
    };
    VOID Clr()
    {
        vecPlunderLog.clear();
        vecReleaseCaptureID.clear();
    }

    vector<DT_PLUNDER_BATTLELOG_DATA> vecPlunderLog;//被动的掠夺战报
    vector<UINT32>				vecReleaseCaptureID;//被动的释放的俘虏ID
};

struct SPlayerBaseData
{
    SPlayerBaseData()
    {
        Init();
    }

    void Init()
    {
        dwID = 0;
        wMainHeroKindID = 0;
        wLevel = 0;
		byVipLevel = 0;
        dwRank = 0;
        byRobotFunc = 0;
        dwPower = 0;
        dwFirstAttack = 0;
        dwNotifyRank = 0;
        qwNotifyRankTime = 0;
        dwCaptureOwner = 0;
        bOpenPlunder = FALSE;
        qwLoginTime = 0;
        qwLogoffTime = 0;
        bEatBreadNotify = 0;
        bBossBattleNotify = 0;
        bCoruageNotify = 0;
        dwFlowerSum = 0;
        dwExperience = 0;
        memset(abyToken, 0, sizeof(abyToken));
        memset(aszDispName, 0, sizeof(aszDispName));
        stSyncProp.Clr();
    }

    UINT32          dwID;
    TCHAR           aszDispName[USERNAME_LEN];
    UINT16          wMainHeroKindID;
    UINT16          wLevel;
	UINT8           byVipLevel;
    UINT32          dwRank;//0表示没有记录
    UINT8			byRobotFunc;//机器人功能,>0则为机器人，不同的值为不同的功能。0为玩家
    UINT32			dwCaptureOwner;//俘虏主，0表示没有被俘虏
    UINT64			qwCaptureOwnerTime;
    BOOL				bOpenPlunder;//释放开放掠夺
    UINT32			dwPower;//战力
    UINT32			dwFirstAttack;//先攻
    UINT64			qwNotifyRankTime;//最后更新排名时间
    UINT32			dwNotifyRank;//最后更新排名
    UINT64			qwLoginTime;//最后上线时间
    UINT64			qwLogoffTime;//最后离线时间
    UINT8			abyToken[MAX_TOKEN_NUM];
    UINT32			dwFlowerSum;
    UINT32          dwExperience;

    BOOL				bEatBreadNotify;//吃包子通知
    BOOL				bBossBattleNotify;//BOSS战通知
    BOOL             bCoruageNotify;//勇气试炼通知
    SSyncProp		stSyncProp;//异步需同步信息
};

struct SPlayerRankData
{
    UINT32   dwPlayerID;                  //玩家ID
    UINT16   wLevel;                      //玩家等级
    TCHAR    aszDispName[USERNAME_LEN];   //玩家昵称
    UINT32   dwExperience;
};

typedef HashMap<UINT32, SPlayerBaseData*>			CPlayerID2PlayerBaseDataMap; //key为玩家ID
typedef CPlayerID2PlayerBaseDataMap::iterator		CPlayerID2PlayerBaseDataMapItr;
typedef CPlayerID2PlayerBaseDataMap::const_iterator	CPlayerID2PlayerBaseDataMapConstItr;

typedef vector<SPlayerBaseData*> CPlayerBaseDataVec;
typedef CPlayerBaseDataVec::iterator CPlayerBaseDataVecItr;
typedef CPlayerBaseDataVec::const_iterator CPlayerBaseDataVecConstItr;

typedef vector<SPlayerRankData> CPlayerRankDataVec;
typedef CPlayerRankDataVec::iterator CPlayerRankDataVecItr;
typedef CPlayerBaseDataVec::const_iterator CPlayerRankDataVecConstItr;


typedef HashMap<tstring, UINT32> CDspNameMap;
typedef CDspNameMap::iterator CDspNameMapItr;


#define MAX_PLAYER_CACHE_POOL_NUM	5000


class CRaceMgr;
class CPlayer;
class CPlunder;
class CNoticeMgr;
class CPlayerBaseDataCacheMgr
{
    //friend class CRaceMgr;
    //friend class CNoticeMgr;
public:
    DECLARE_SINGLETON(CPlayerBaseDataCacheMgr)

    BOOL Init();

    VOID UnInit();

public:
    BOOL AddPlayerBaseData(CPlayer* poPlayer); //用于添加新增玩家的基本信息
    const SPlayerBaseData* GetPlayerBaseData(UINT32 dwPlayerID);
    UINT16 GetLevel(UINT32 dwPlayerID);

    UINT16 RegDspName(UINT32 dwPlayerID, const TCHAR* pszDispName);//注册昵称，成功返回TRUE,否则昵称冲突
    BOOL OnLevelChange(UINT32 dwPlayerID, UINT16 wLevel);//
    //BOOL OnFameChange(UINT32 dwPlayerID, UINT32 dwFame);//
    BOOL OnRankNew(UINT32 dwPlayerID, UINT32 dwRank);//Rank变化时回调,用于新增(删除)排名时
    BOOL OnRankExchange(UINT32 dwPlayerID1, UINT32 dwRank1, UINT32 dwPlayerID2, UINT32 dwRank2);//排名交换时，用于竞技场挑战
    BOOL OnCapture(UINT32 dwPlayerID, UINT32 dwCaptureOwnerID);//被俘虏/被俘虏释放回调
    //VOID OnClrPlayerData(CPlayer* poPlayer); //fortest，清除玩家数据时回调
    BOOL OnPowerChange(UINT32 dwPlayerID, UINT32 dwPower);//战力变化回调
    BOOL OnFirstAttackChange(UINT32 dwPlayerID, UINT32 dwFirstAttack); //先攻变化回调
    BOOL OnOpenPlunder(UINT32 dwPlayerID); //开启俘虏回调

    const CPlayerID2PlayerBaseDataMap& GetPlunderRobot()
    {
        return m_mapPlunderRobot;    //获取用于掠夺的机器人
    }
    const CPlayerID2PlayerBaseDataMap &GetPlayerID2PlayerBaseDataMap()
    {
        return m_mapPlayerID2PlayerBaseData;
    };

	const CPlayerBaseDataVec &GetPlayerBaseDataVec()
	{
		return m_vecPlayerBaseData;
	};

    void ReSetLoginTime(UINT32 dwPlayerID);
    void ReSetLogoffTime(UINT32 dwPlayerID);
    //VOID GetPlayerByLevel(UINT16 wLevel, UINT8 byGetNum, CPlayerBaseDataVec& vecPlayer, BOOL bNotBecapture = FALSE);//获取指定等级的指定数目的玩家
    VOID GetPlayerByLevel(UINT32 dwMyPlayerID, UINT16 wLevel, UINT8 byGetNum, CPlayerBaseDataVec& vecPlayer,
                          UINT32 dwStartPlayerID, CPlunder &oOwnPlunder, BOOL bNotBecapture = FALSE);//获取指定等级的指定数目的玩家

    VOID GetPlayerByLevel(UINT32 dwMyPlayerID, UINT16 wLevel, UINT8 byGetNum, CPlayerBaseDataVec& vecPlayer,
                          UINT32 dwStartPlayerID, BOOL bHaveRobot = FALSE);//获取指定等级的指定数目的玩家

    BOOL	GetEatBreadNotify(UINT32 dwPlayerID);//吃包子通知
    BOOL	GetBossBattleNotify(UINT32 dwPlayerID);//BOSS战通知
    BOOL	GetCourageNotify(UINT32 dwPlayerID);//勇气试炼通知

    VOID	SetEatBreadNotify(UINT32 dwPlayerID, BOOL bValue);//吃包子通知
    VOID	SetBossBattleNotify(UINT32 dwPlayerID, BOOL bValue);//BOSS战通知
    VOID	SetCourageNotify(UINT32 dwPlayerID, BOOL bValue);//勇气试炼通知
    BOOL InitDataFromDB();

    BOOL CkHaveDspName(const TCHAR* pszDispName);

    CPlayerIDVec FindContainDspName(const TCHAR* pszDispName);

    UINT32 FindPlayer(const TCHAR* pszDispName);

	BOOL	AddFlower(UINT32 dwPlayerID, UINT32 dwFlower);

	BOOL	SetFlower(UINT32 dwPlayerID, UINT32 dwFlower);

	UINT32	GetFlower(UINT32 dwPlayerID);

	UINT32	GetFlowerRanking(UINT32 dwPlayerID);

	// 获取鲜花榜
	UINT16 GetFlowerRankingList(DT_FLOWER_RANKING_LIST_CLI& stFlowerRankingList);
	
	VOID SetVipLevel(UINT32 dwPlayerID, UINT8 byVIPLevel);

    //获取一定数量的等级排名
    VOID GetRankByLvEx(UINT32 dwNum, CPlayerRankDataVec &m_vecPlayerRankData);

	VOID SetExperience(UINT32 dwPlayerID, UINT32 dwExperience);

	const CPlayerBaseDataVec& GetPlayerBaseDatas() const;

protected:
    CPlayerBaseDataCacheMgr();
    ~CPlayerBaseDataCacheMgr();
protected:

private:
	CPlayerBaseDataVec							m_vecPlayerBaseData;
    CPlayerBaseDataVec							m_vecPlayerBaseData_LvEx;
    CPlayerID2PlayerBaseDataMap					m_mapPlayerID2PlayerBaseData; //key为PlayerID
    CSDObjectPool<SPlayerBaseData, CSDMutex>			m_oPlayerBaseDataPool;
    CPlayerID2PlayerBaseDataMap					m_mapPlunderRobot;//掠夺的机器人
    UINT16										m_wMaxPlayerLevel;//最大玩家等级
    CPlayerID2PlayerBaseDataMap*					m_apmapPlayerLevel;//玩家等级对应的所有该等级的玩家
    CDspNameMap									m_mapDspName;
};



#endif

