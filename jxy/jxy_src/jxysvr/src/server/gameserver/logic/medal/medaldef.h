///<------------------------------------------------------------------------------
//< @file:   logic\medal\medaldef.h
//< @author: Kang_zhiquan
//< @date:   2014年6月18日 14:3:11
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _medaldef_h_
#define _medaldef_h_
#include <sdtype.h>
#include "protocol/server/protocligs.h"
#include <map>
#include "logic/base/jsonencourage.h"
using namespace std;
struct SMedalProp
{
	UINT16 wMedalID;//勋章ID
	string strMedalName;//勋章名称
	string strMedalIconName;//图标ID
	UINT8 byOpenBuildID;//建筑ID
	UINT8 byIsLevelMedal;//是否有等级
	UINT16 wSmallType;//小类别（用于区分不同类型勋章）
	string strFinishDesc;//完成条件
	CVecJsonAttrBase vecAttr;//属性
	UINT16 wLevel;//勋章等级
	UINT32 dwOverTime;//有效期
	UINT64 qwFirstCondition;//起始条件
	UINT64 qwSecondCondition;//末尾条件
	UINT64 qwThirdCondition;//末尾条件
	UINT8 byIsOverly;//是否可叠加
	UINT8 byMedalColor;
	SMedalProp()
	{
		wMedalID = 0;//勋章ID
		strMedalName ="";//勋章名称
		strMedalIconName = "";//图标ID
		wSmallType = 0;//小类别（用于区分资源勋章）
		strFinishDesc = "";//完成条件
		vecAttr.clear();//属性
		wLevel = 0;//勋章等级
		dwOverTime = 0;//有效期
		qwFirstCondition = 0;//起始条件
		qwSecondCondition = 0;//起始条件
		qwThirdCondition = 0;
		byOpenBuildID = 0;//建筑ID
		byIsOverly = 0;
		byMedalColor = 0;
	}
};

typedef map<UINT16, SMedalProp> CMapMedalProp;
typedef CMapMedalProp::iterator CMapMedalPropItr;

typedef map<UINT32, SMedalProp> CMap32MedalProp;
typedef CMap32MedalProp::iterator CMap32MedalPropItr;

#endif //_medaldef_h_