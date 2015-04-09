#ifndef _YABIAO_MGR_H_
#define _YABIAO_MGR_H_

#include "yabiaoinfo.h"

#include <sdsingleton.h>
#include <logic/base/basedefine.h>
#include <common/client/commondef.h>
#include <logic/shmem/shmemmap.h>
#include <set>
#include <logic/base/basepropmgr.h>

#include <sdobjectpool.h>
#include <sdmutex.h>

struct SYabiaoEncourage
{
    SYabiaoEncourage()
    {
        dbCoinFactor = 0;
        nScience = 0;
    }
    SYabiaoEncourage( DOUBLE _dbCoinFactor, INT32 _nScience, INT32 _nJingjie )
    {
        dbCoinFactor = _dbCoinFactor;
        nScience = _nScience;
        nJingjie = _nJingjie;
    }

    DOUBLE dbCoinFactor;
    INT32 nScience;
    INT32 nJingjie;
};

struct SBiaoBaseInfo
{
    SBiaoBaseInfo()
    {
        nTime = 0;
        nFlushUpdateRate = 0;
    }
    //镖的时间
    INT32 nTime;
    //奖励
    SYabiaoEncourage oEncourage;

    //刷新升级的概率
    INT32 nFlushUpdateRate;
};

struct SYabiaoCfgInfo
{
    SYabiaoCfgInfo()
    {
        Reset();
    }
    VOID Reset()
    {
        vecBiaoInfo.clear();
        vecFlushCostGold.clear();
    }

    vector<SBiaoBaseInfo> vecBiaoInfo; //镖的基本信息,按顺序读取，低级在前，高级在后
    vector<INT32> vecFlushCostGold; //刷新消耗的元宝
};

struct SYabiaoPlayerInfo
{
    SYabiaoPlayerInfo()
    {
        dwPlayerID = 0;
    }

    UINT32 dwPlayerID;

};

struct SJiebiaoInfo
{
    SJiebiaoInfo()
    {
        dwCoinEncourage = 0;
    }

    UINT32 dwCoinEncourage;
};

class CPlayer;

typedef map<UINT16, C32Vec> CLevel2PlayerVecMap;
typedef CLevel2PlayerVecMap::iterator CLevel2PlayerVecMapItr;

class CYabiaoMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CYabiaoMgr);
    //劫镖信息缓存
    CYabiaoMgr();
    ~CYabiaoMgr();
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_YABIAO ;
    }
    BOOL Init();
    VOID UnInit();

    UINT8 GetTotalYabiaoCnt();
    UINT8 GetTotalJiebiaoCnt();
    UINT8 GetTotalBeJiebiaoCnt();

    //获得最高品质的等级
    INT32 GetMaxQualityLevel();

    //获得对应等级押镖所需的时间(等级从1开始)
    INT32 GetCostTime( INT32 nQuality );

    //直接最高等级消耗的元宝
    INT32 GetDirectMaxLevelCostGold();

    //获得刷新消耗的元宝
    INT32 GetFlushCostGoldByCount( INT32 nCount );

    //获得指定品质的奖励
    SYabiaoEncourage GetEncourage( INT32 nQuality );

    //增加正在押镖的玩家
    INT32 AddPlayer( UINT32 dwPlayer );

    //减少正在押镖的玩家
    BOOL DeletePlayer( UINT32 dwPlayer );

    //失败返回0
    UINT32 GetPlayerIDByIndex( INT32 nIndex );
    INT32 GetPlayerCount();

    //获得在押镖的仇敌数量
    vector<UINT32> GetEnemy( UINT16 wPlayerLevel, const set<UINT32> &setEnemy, UINT8 byNum = 0, UINT16 wTimeToEnd = 0 );

    vector<UINT32> GetYabiaoPlayer( UINT16 wPlayerLevel, UINT16 wTimeToEnd = 0, UINT8 byNum = 0 );

    //返回成功或者和四百
    BOOL FlushToUpdateQuality( INT32 nQuality );

    BOOL FillPlayerYabiaoInfo( PKT_CLIGS_OPEN_JIEBIAO_ACK &ack, const vector<UINT32> &vecPlayer, UINT32 dwViewerID, UINT16 wJiebiaoLevel,CPlayer *pOwner );

    UINT16 GetJiebiaoCD();

	INT32 BeginJiebiao( CPlayer &playerSelf, UINT32 dwBeJiebiaoPlayerID, UINT64 &qwEncourageCoin, UINT32 &dwEncourageScience,
		DT_BATTLE_DATA5 &stDT_BATTLE_DATA, UINT64 &qwBattleIndexOut );
    //VOID OnCrossDay();

    //PKT_CLIGS_BEGIN_JIEBIAO_ACK *GetJiebiaoAck( UINT32 dwPlayerID, UINT32 dwCount );
    //VOID AddJiebiaoAck( UINT32 dwPlayerID, UINT32 dwCount, const PKT_CLIGS_BEGIN_JIEBIAO_ACK &ack );

    vector<SBiaoBaseInfo> GetBiaoInfoList();

    //byLevel是指品质，wPlayerLevel是指玩家等级
    DT_BIAO_INFO GetBiaoInfo( UINT8 byLevel, UINT16 wPlayerLevel );

    //添加押镖的玩家
    BOOL AddPlayerToContainer( CPlayer* poPlayer);
private:
    BOOL LoadFromDBEx();

    vector<UINT32> GetLimitYabiaoPlayer( UINT16 wPlayerLevel,  BOOL bGetEnemy, UINT32 dwGetNum, const vector<UINT32> &vecSrc, UINT16 wTimeToEnd );

    VOID FillBiaoAckInfo( PKT_CLIGS_FLUSH_BIAO_QUALITY_ACK &ack, CPlayer &oPlayer );
    UINT64 GetJiebiaoCacheKey( UINT32 dwPlayerID, UINT32 dwCount ); //劫镖的玩家ID，劫镖次数

    //VOID ClearJiebiaoCache();

    UINT32 CalcCoin( UINT8 byQuality, UINT16 wLevel );

private:
    SYabiaoCfgInfo m_oCfgInfo; //配置信息
    vector< UINT32 > m_vecYabiaoPlayer; //押镖的玩家信息
    set< UINT32 > m_setYabiaoPlayer; //押镖的玩家信息
    CLevel2PlayerVecMap	m_mapLevel2PlayerVecMap;////押镖的玩家信息,key为玩家等级
    //MAP_JIEBIAO_CACHE_TYPE m_mapJiebiaoAckCache;
};


#endif
