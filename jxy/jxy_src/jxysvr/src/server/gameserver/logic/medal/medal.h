///<------------------------------------------------------------------------------
//< @file:   logic\medal\medal.h
//< @author: Kang_zhiquan
//< @date:   2014年6月17日 19:21:34
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _medal_h_
#define _medal_h_
#include "logic/base/baseobj.h"
#include "protocol/server/protocommondata.h"
#include "logic/medal/medaldef.h"
#include "logic/player/player.h"

class CMedal : public CBaseObj
{
public:
	CMedal();
	virtual ~CMedal();
	 BOOL Init(CPlayer* poPlayer, const DT_MEDAL_DATA& stData);
public:
	 //获取大类型
	 virtual UINT8 GetBigType() = 0;
	 //获取小类型
	 UINT16 GetSmallType();
	 //是否可叠加 True不可叠加
	 BOOL GetIsOverlay();
	 //是否是等级勋章
	 BOOL IsLevelMedal();
	 //获取勋章ID
	 UINT16 GetMedalID();
	 //获取勋章LEVEL
	 UINT16 GetLevel();
	 //获取依赖的建筑ID
	 UINT16 GetBuildID();
	 //是否激活
	virtual BOOL IsActivate();
	const DT_MEDAL_DATA& GetMedalData();
	//检测激活
	virtual BOOL CheckActivate() = 0;
	//属性值
	BOOL GetMedalAttr(const CVecJsonAttrBase*&);
	//获取配置
	BOOL GetProp(const SMedalProp*& poProp);
	//设置激活
	virtual BOOL SetActivateState(BOOL bIsActivate, UINT64 qwActivateTime = SGDP::SDTimeSecs());
	//获取客户端信息
	virtual VOID GetMedalCli(DT_MEDAL_DATA_CLI& stCli);
	//获取勋章图标ID
	string GetMedalIconID();
protected:
	DT_MEDAL_DATA m_stMedalData;
};

class CTimeMedal : public CMedal
{
public:
	//TRUE为过期
	//FALSE为未过期
	virtual BOOL IsOverTime();
	virtual UINT8 GetBigType();
protected:
};

class CNoneTimeMedal : public CMedal
{
public:
	virtual UINT8 GetBigType();
protected:
};

#endif //_medal_h_