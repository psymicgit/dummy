

#ifndef _BUILD_H_
#define _BUILD_H_

#include <logic/base/baseobj.h>
#include "buildpropmgr.h"

using namespace std ;

class CPlayer;

struct tagDT_BUILD_DATA_CLI;
typedef tagDT_BUILD_DATA_CLI DT_BUILD_DATA_CLI;

class CBuild:public CBaseObj
{
public:
	CBuild();
	~CBuild();
public:	
	EBuildKind GetBuildKind() { return (EBuildKind)(m_stDT_BUILD_BASE_DATA.byKindID); } //	
	BOOL AddFunctionModuleAndGuide(); //功能模块及引导，若已有则直接返回，有相关功能的建筑需实现
public:
	VOID GetDT_BUILD_DATA_CLI(DT_BUILD_DATA_CLI& stDT_BUILD_DATA_CLI);//获取给客户端的建筑信息
	DT_BUILD_BASE_DATA& GetDT_BUILD_BASE_DATA() { return m_stDT_BUILD_BASE_DATA; };
	//初始化相关对象
	BOOL Init(DT_BUILD_BASE_DATA* pstDT_BUILD_BASE_DATA, CPlayer* poOwner);
	//CPlayer* GetOwner() { return m_poOwner; }	
	VOID OnOpen(); //打开建筑回调
protected:
	BOOL AddFunctionModuleAndGuide_Awaken();//悟道 添加功能模块及引导	
	BOOL AddFunctionModuleAndGuide_EliteInstance();//精英副本 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Godweapon();//神器 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_MoneyTree();//摇钱树 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Plunder();//掠夺 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Race();//竞技场 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Talent();//丹药房 添加功能模块及引导	
	BOOL AddFunctionModuleAndGuide_Recuit();//招募馆 添加功能模块及引导	
	BOOL AddFunctionModuleAndGuide_Smithy();//铁匠铺 添加功能模块及引导	
	BOOL AddFunctionModuleAndGuide_Maintown();//主城 添加功能模块及引导	
	//BOOL AddFunctionModuleAndGuide_Skill();//技能 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_ScienceTree(); //科技 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Crop();//种植 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Experience();//经验丹 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_UpdateAttr();//属性升级 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Levy();//属性增收 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Melting();//属性熔炼 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_EatBread();//吃包子 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Yabiao();// 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_ClimbTower();//爬塔 添加功能模块及引导	
	BOOL AddFunctionModuleAndGuide_JingJie();//境界 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Skill();//技能 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_CollectGas();//境界收集精气 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Adventure();//境界 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Xiangqian(); //镶嵌 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_BossB();       //BOSS战 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_RDC();         //随机挑战 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_OpenLight();   //装备开光 添加功能模块及引导
    BOOL AddFunctionModuleAndGuide_EquipMaster(); //装备精通 添加功能模块及引导
    BOOL AddFunctionModuleAndGuide_Courage(); //勇气试炼 添加功能模块及引导
	BOOL AddFunctionModuleAndGuide_Friends(); //好友系统
	BOOL AddFunctionModuleAndGuide_BegBlue(); //好友系统
	BOOL AddFunctionModuleAndGuide_Faction();	//门派
	BOOL AddFunctionModuleAndGuide_Fromation();	//布阵
	BOOL AddFunctionModuleAndGuide_GVG();//跨服战
	BOOL AddFunctionModuleAndGuide_Soul();//战魂
	BOOL AddFunctionModuleAndGuide_Medal();//勋章
	BOOL AddFunctionModuleAndGuide_Pet();//宠物
	BOOL AddFunctionModuleAndGuide_OnHook();	//挂机
protected:	
	DT_BUILD_BASE_DATA		m_stDT_BUILD_BASE_DATA; //建筑信息	
};


#endif //#ifndef 

