
#ifndef _COLLECTGASPROPMGR_H_
#define _COLLECTGASPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basepropmgr.h>
#include <common/client/commondef.h>
#include <common/server/utility.h>
#include <map>
#include <vector>

using namespace std;

/// 技能名称名称最大长度
#define NAME_LEN_MAX	32
/// 技能介绍名称最大长度
#define INTRODUCTION_LEN_MAX	255





//生成获取的精气属性
struct SGenGasProp
{
    UINT8		byGasType;		//精气类型
    UINT8		byLevel;			//精气等级
	UINT32		dwValue;			//产生的值
    UINT8		byCoinRate;		//生成概率
	UINT8		byGoldRate;		//生成概率
};

//提升精气等级属性
struct SUpGasLevelProp
{
    UINT8		byType;		//精气类型
    UINT8		byIncLevel;	//提升等级
    UINT8		byRate;		//概率
};

//提升精气等级属性
struct SGasNumProp
{
	UINT8		byVipLevel;		//VIP等级
	UINT8		byLevel;			//等级
	UINT8		byTownIdx;		//城镇ID
	UINT8		byInstanceIdx;	//副本ID
	UINT8		byValue;			//次数
};



//提升精气等级消耗铜币
typedef map<UINT16, UINT32> CUpGasLevelCostCoinMap; //按顺序map，key为使用次数， value为使用铜币
typedef CUpGasLevelCostCoinMap::iterator CUpGasLevelCostCoinMapItr;


//提升精气等级消耗铜币
typedef vector<SGasNumProp> CGasNumPropVector;
typedef CGasNumPropVector::iterator CGasNumPropVectorItr;


class CPlayer;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CCollectGasPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CCollectGasPropMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_COLLECTGAS ;
    }
    BOOL Init();
    VOID UnInit();
public:
	UINT32	GetGas(CPlayer *poPlayer, UINT8 byGasType, UINT8 byLevel);
    VOID		GenGas(CPlayer *poPlayer, UINT8 byFlag, DT_GAS_BALL_DATA& stDT_GAS_BALL_DATA);
    VOID		UpGasBallLevel(CPlayer *poPlayer, UINT8 byFullLevelFlag, DT_GAS_BALL_DATA &stDT_GAS_BALL_DATA);
    UINT32	GetGuidePurpleGas(CPlayer *poPlayer);

	//获得聚气的次数
	UINT32 GetGenGasNum(UINT8 byVipLevel, UINT8 byLevel, UINT8 byTownIdx, UINT8 byInstanceIdx);
	
	SGasNumProp * GetNextGenGasNumProp(UINT8 byVipLevel, UINT8 byLevel, UINT8 byTownIdx, UINT8 byInstanceIdx);


public:
    CCollectGasPropMgr();
    virtual ~CCollectGasPropMgr();

protected:
    BOOL LoadGenGasPropFromDB();
	BOOL LoadGenGasNumPropFromDB();
    BOOL LoadUpGasLevelRatePropFromDB();

private:
    /////////////////////////////////////////
    vector<SGenGasProp>			m_vecGenGasProp;					//生成气配置
    CRandomVec					m_vecCoinGenGasPropRandom;			//生成精气配置 概率vec
	CRandomVec					m_vecGoldGenGasPropRandom;			//生成精气配置 概率vec
    HashMap<UINT16, SGenGasProp>	m_mapGenGasProp;					//生成精气配置
    vector<SUpGasLevelProp>		m_vecUpBlueGasLevelProp;			//提升蓝气等级配置
    CRandomVec					m_vecUpBlueGasLevelPropRandom;		//提升蓝气等级配置 概率vec
    vector<SUpGasLevelProp>		m_vecUpPurpleGasLevelProp;			//提升紫气等级配置
    CRandomVec					m_vecUpPurpleGasLevelPropRandom;	//提升紫气等级配置 概率vec
    UINT8						m_byGasBallLevelNum;				//精气球等级数

	CGasNumPropVector				m_vectorGasNumProp;
};

#endif // #ifndef _SKILLPROPMGR_H_
