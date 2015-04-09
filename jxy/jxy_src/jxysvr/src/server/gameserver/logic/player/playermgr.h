
#ifndef _PLAYERMGR_H_
#define _PLAYERMGR_H_

#include "player.h"
#include <sdsingleton.h>
#include <protogsdb.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include <net/cli/user.h>


using namespace SGDP;

#define SAVE_PLAYER_DATA_SQL "../SavePlayerDataSql"

typedef map<UINT32, CPlayer*> CID2PlayerMap;
typedef CID2PlayerMap::iterator	CID2PlayerMapItr;
typedef CID2PlayerMap::const_iterator CID2PlayerMapConstItr;

typedef vector<UINT16>      SSkillVec;
typedef SSkillVec::iterator SSkillVecItr;

typedef vector<UINT32> CID2PlayerVec;
typedef CID2PlayerVec::iterator	CID2PlayerVecItr;


/**
* @brief 玩家管理类
*/
class CPlayerMgr
{
public:
    DECLARE_SINGLETON(CPlayerMgr)

    /**
    * @brief
    * 初始化函数,读取配置文件等
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Init();

    /**
    * @brief
    * 卸载函数
    */
    VOID UnInit();

public:
    CPlayer*			FindPlayer(UINT32 dwPlayerID);

    CPlayer*			CreatePlayer(UINT32 dwPlayerID);

    VOID				RemovePlayer(UINT32 dwPlayerID);

	const CID2PlayerMap &	GetID2PlayerMap(){return m_mapID2PlayerMap;};

	void				GetID2PlayerVec(CID2PlayerVec &vecPlayerCache);


public:
    BOOL	CreateNewPlayer(UINT32 dwTransID, CUser* poUser);//创建新玩家，CareerID无效返回FALSE
	BOOL	CreateNewPlayerInMem(UINT32 dwTransID, CUser* poUser);//创建新玩家先在内存，不用先入库	
    BOOL	CreateNewPlayerGM(UINT32 dwPlayerID, UINT8 byRobotFunc = ERFK_OTHER, UINT16 wLevel = 1, TCHAR* pszDspName = NULL);//创建新玩家(GM)
	BOOL	CreateNewPlayerGMMoreParam(UINT32 dwPlayerID, UINT16 wCareerID, UINT32 dwAmount, UINT32 dwTotalAmount, UINT8 byVipLevel, TCHAR* pszDspName, TCHAR* pszDeviceID, TCHAR* pszNotifyID);//创建新玩家

public:
    VOID	OnSaveOnlineEvent(UINT32 dwPlayerID);//保存玩家信息事件回调，保存玩家数据
    VOID	OnSaveCkOfflineEvent(UINT32 dwPlayerID);//保存/检测非登录玩家玩家数据，过期则从内存中清除
    VOID	SaveAllPlayerData();////保存所有玩家数据,用于退出程序时
	VOID    GetTodaySavePlayerPath(TCHAR* aszSavePath, UINT32 dwPlayerID);//玩家备份路径
	BOOL	RetreatePlayerData(const UINT32 &dwPlayerId,string strFile,UINT64 dwTime);//进行个人玩家的数据回档
	BOOL	AllPlayerRetreate(UINT32 dwTransID);//对所有玩家进行回档时，捞回丢失的充值记录

    //VOID	ClrPlayerData(CPlayer* poPlayer);//清除玩家数据（fortest）

protected:
	BOOL	SetFormationByHeroID(UINT16 wCoachHeroID, DT_NEW_FORMATION_DATA& stFormationInfo);
	BOOL	SetFormationByHeroID(vector<UINT16>& vecHeroID, DT_NEW_FORMATION_DATA& stFormationInfo);

protected:
    BOOL	InitPlayerDefault();

    BOOL	ReadPlayerDefaltBase();

protected:


protected:
    CPlayerMgr();
    ~CPlayerMgr();

private:
    DT_PLAYER_DATA					m_stPlayerDefault;//新增玩家初始默认配置
    CID2PlayerMap						m_mapID2PlayerMap;
    //CSDObjectPool<CPlayer, CSDMutex>	m_oPlayerShmemObjMgr;	
};



#endif //#ifndef _PLAYERMGR_H_

