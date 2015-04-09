

#ifndef _DBSQLSTRING_H_
#define _DBSQLSTRING_H_

#pragma once

//索引表indexmgr的索引类型
//索引表用于定位最新插入的数据的ID
enum EIndexKind
{
	INDEX_EQUIP = 1,//装备
	INDEX_GOODS = 2,//物品
	INDEX_BOX = 3,//宝箱
	INDEX_SCIENCEATTR = 4,//科技属性
};

///////////////////////////////////////////读取/////////////////////////////////////////////
/// 读取玩家信息
/*
#define SQL_READ_GET_PLAYER_DATA_BY_USERNAME_STRING "select * from player "\
	" where UserName = '%s'"
#define SQL_READ_GET_PLAYER_DATA_BY_DEVICEID_STRING "select * from player "\
	" where DeviceID = '%s'"
	*/
#define SQL_READ_GET_PLAYER_DATA_BY_PLAYERID_STRING "select * from player "\
	" where ID = '%u'"

/// 读取玩家技能信息
#define SQL_READ_GET_PLAYER_SKILL_DATA_STRING "select * from skill "\
	" where PlayerID = '%u' and DeleteFlag = '0'"

/// 读取武将信息
#define SQL_READ_GET_HERO_DATA_STRING "select * from hero "\
	" where PlayerID = '%u' and DeleteFlag = '0'"

/// 读取武将装备信息
#define SQL_READ_GET_HERO_EQUIP_DATA_STRING "select * from equip "\
	" where PlayerID = '%u' and HeroID = '%u' and DeleteFlag = '0' order by BagPos"

/// 读取玩家背包信息
#define SQL_READ_GET_PALYER_BAG_DATA_STRING "select * from bag "\
	" where PlayerID = '%u' and DeleteFlag = '0'"

/// 读取玩家背包装备信息
#define SQL_READ_GET_PALYER_BAG_EQUIP_ITEM_DATA_STRING "select * from equip "\
	" where PlayerID = '%u' and HeroID = '0' and DeleteFlag = '0'  order by BagPos"

/// 读取玩家背包物品信息
#define SQL_READ_GET_PALYER_BAG_GOODS_ITEM_DATA_STRING "select * from goods "\
	" where PlayerID = '%u' and DeleteFlag = '0'  order by BagPos"

/// 读取玩家背包宝箱信息
#define SQL_READ_GET_PALYER_BAG_BOX_ITEM_DATA_STRING "select * from box "\
	" where PlayerID = '%u' and DeleteFlag = '0'  order by BagPos"

/// 读取玩家建筑信息
#define SQL_READ_GET_PALYER_BUILD_DATA_STRING "select * from build "\
	" where PlayerID = '%u' and DeleteFlag = '0' order by Sort"

/// 读取玩家副本信息
#define SQL_READ_GET_PALYER_CURINSTANCE_DATA_STRING "select * from curinstance "\
	" where PlayerID = '%u'"
/// 读取玩家当前最大普通副本信息
#define SQL_READ_GET_PALYER_MAX_COMMON_INSTANCEID_DATA_STRING "select SceneIndex, TownIndex, InstanceIndex from instancerecord "\
	" where PlayerID='%u' and SceneIndex = '1' order by TownIndex DESC, InstanceIndex DESC limit 1 "
/// 读取玩家当前最大精英副本信息
#define SQL_READ_GET_PALYER_MAX_ELITE_INSTANCEID_DATA_STRING "select SceneIndex, TownIndex, InstanceIndex from instancerecord "\
	" where PlayerID='%u' and SceneIndex = '2' order by TownIndex DESC, InstanceIndex DESC limit 1 "

/// 读取玩家建筑信息
#define SQL_READ_GET_PLAYER_SCIENCE_ATTREXT_STRING "select * from scienceattr "\
	" where PlayerID = '%u'"

/// 读取玩家任务信息
#define SQL_READ_GET_PLAYER_TASK_STRING "select * from curtask "\
	" where PlayerID = '%u' order by TaskLine"

/// 读取玩家竞技信息
#define SQL_READ_GET_PLAYER_RACE_STRING "select * from race "\
	" where PlayerID = '%u'"

/// 读取玩家位置信息
#define SQL_READ_GET_PLAYER_LOCALE_STRING "select * from locale "\
	" where PlayerID = '%u'"

/// 读取武将丹药信息
#define SQL_READ_GET_HERO_DRUG_DATA_STRING "select * from drug "\
	" where PlayerID = '%u' and HeroID = '%u' and DeleteFlag = '0' order by Level"

/// 读取玩家状态信息
#define SQL_READ_GET_PLAYER_STATE_STRING "select * from curstate "\
	" where PlayerID = '%u'"

//////////////////////////////////////更新//////////////////////////////////////////////////
/// 更新玩家信息

#define SQL_UPDATE_PLAYER_DATA_STRING "update player set DispName='%s', Coin='%u', Gold='%u' "\
	", PhyStrength = '%u', LastPhyStrengthRefreshTime = '%lld', UpdateTime = now() "\
	" where UserName = '%s' and DeviceID = '%s' and ID = '%u'"


#define SQL_UPDATE_PLAYER_BASE_DATA_STRING "update player set DispName='%s', Coin='%lld', Gold='%lld' "\
	", PhyStrength = '%u', Science = '%lld' "\
	", GuideRecord = '%lld', SkillGrade='%u', OpenDefenseSkillFlag='%u', UpdateTime = now() "\
	" where ID = '%u'"


/// 更新玩家技能信息
#define SQL_UPDATE_PLAYER_SKILL_DATA_STRING "update skill set Level = '%u', UpdateTime = now() "\
	" where PlayerID = '%u' and DeleteFlag = '0' and ID = '%u'"

/// 更新武将信息
#define SQL_UPDATE_HERO_DATA_STRING "update hero set Pos = '%u', FormationIdx = '%u', Level = '%u', Talent = '%u', Experience = '%u'"\
	", HP = '%u', Attack = '%u', HitRate='%u',DodgeRate='%u',CritRate='%u',DeCritRate='%u', RecuitedFlag='%u', VisitNum='%u', UpdateTime = now() "\
	" where PlayerID = '%u' and DeleteFlag = '0' and KindID = '%u'"

/// 更新玩家背包信息
#define SQL_UPDATE_PALYER_BAG_DATA_STRING "update bag set OpenNum = '%u', UpdateTime = now() "\
	" where DeleteFlag = '0' and PlayerID = '%u'"

/// 更新玩家装备信息
#define SQL_UPDATE_PALYER_EQUIP_DATA_STRING "update equip set HeroID = '%u', Level='%u', BagPos='%u', UpdateTime = now() "\
	" where PlayerID = '%u' and DeleteFlag = '0' and KindID = '%u' and Idx = '%u'"

/// 更新玩家物品信息
#define SQL_UPDATE_PALYER_GOODS_DATA_STRING "update goods set PileCount = '%u', LastUseTime = '%lld', BagPos='%u', UpdateTime = now() "\
	" where PlayerID = '%u' and DeleteFlag = '0' and KindID = '%u'"

/// 更新玩家宝箱信息
#define SQL_UPDATE_PALYER_BOX_DATA_STRING "update box set PileCount = '%u', BagKind='%u', BagPos='%u', UpdateTime = now() "\
	" where PlayerID = '%u' and DeleteFlag = '0' and KindID = '%u' and Idx='%u'"

/// 更新玩家建筑信息
#define SQL_UPDATE_PALYER_BUILD_DATA_STRING "update build set Level = '%u', Value1 = '%lld', Value2 = '%u', Sort = '%u', FunctionRecord = '%u', UpdateTime = now() "\
	" where PlayerID = '%u' and DeleteFlag = '0' and KindID = '%u'"

/// 更新玩家竞技信息
#define SQL_UPDATE_PALYER_RACE_DATA_STRING "update race set Rank = '%u', ContiWin = '%u', RecvEncourageTime = '%lld', LastChallTime = '%lld', ChallTimes = '%u', ClrCDFlag='%u', UnreadBattleLogNum='%u', RecvEncourageRank='%u', UpdateTime = now() "\
	" where PlayerID = '%u'"

/////////////////////////////新增///////////////////////////////////////////////////////////
/// 新增玩家信息
#define SQL_ADD_PLAYER_DATA_STRING "insert into player (ID, UserName,DeviceID,NotifyID,DispName,Coin,Gold,PhyStrength, CreateTime) values( "\
	" '%u','%s', '%s', '%s', '%s', '%lld', '%lld', '%u', now())"

/// 新增玩家技能信息
#define SQL_ADD_PLAYER_SKILL_DATA_STRING "insert into skill (PlayerID,KindID,Level, DeleteFlag, CreateTime) values( "\
	" '%u', '%u', '%u', '0', now())"

/// 新增武将信息
#define SQL_ADD_HERO_DATA_STRING "insert into hero (PlayerID,Coach,KindID,Pos,FormationIdx,Level,Talent,Experience"\
	",HP,Attack,HitRate,DodgeRate,CritRate,DeCritRate, RecuitedFlag, VisitNum, DeleteFlag, CreateTime) values( "\
	"'%u', '%u', '%u','%u', '%u', '%u', '%u','%u'"\
	",'%u', '%u', '%u', '%u', '%u', '%u','%u', '%u', '0', now())"

/// 新增玩家背包信息
#define SQL_ADD_PALYER_BAG_DATA_STRING "insert into bag (PlayerID,OpenNum, DeleteFlag, CreateTime) values( "\
	"'%u', '%u', '0', now())"

/// 新增玩家装备信息
#define SQL_ADD_PALYER_EQUIP_DATA_STRING "insert into equip (PlayerID,HeroID,KindID,Idx,Level, BagPos, ItemNewTime, DeleteFlag, CreateTime) values( "\
	"'%u', '%u', '%u', '%u', '%u', '%u', '%lld', '0', now())"

/// 新增玩家物品信息
#define SQL_ADD_PALYER_GOODS_DATA_STRING "insert into goods (PlayerID,KindID,PileCount,LastUseTime, BagPos, ItemNewTime, DeleteFlag, CreateTime) values( "\
	"'%u', '%u','%u', '%lld', '%u', '%lld', '0', now())"

/// 新增玩家宝箱信息
#define SQL_ADD_PALYER_BOX_DATA_STRING "insert into box (PlayerID,KindID, Idx, BagKind, PileCount, BagPos, ItemNewTime, DeleteFlag, CreateTime) values( "\
	"'%u', '%u', '%u', '%u', '%u', '%u', '%lld', '0', now())"

/// 新增玩家建筑信息
#define SQL_ADD_PALYER_BUILD_DATA_STRING "insert into build (PlayerID,KindID,Level,Value1,Value2,Sort, FunctionRecord, DeleteFlag, CreateTime) values( "\
	"'%u', '%u', '%u','%lld', '%u', '%u', '%u', '0', now())"

/// 新增玩家副本信息
#define SQL_ADD_PALYER_INSTANCE_DATA_STRING "insert into instance (PlayerID,InstanceIndexA,InstanceIndexB,InstanceIndexC,EnterTimesA, EnterTimesB, EnterTimesC, CreateTime) values( "\
	"'%u', '%s', '%s','%s', '%u', '%u','%u',now())"

/// 新增玩家任务信息
#define SQL_ADD_PALYER_TASK_DATA_STRING "insert into curtask (PlayerID,TaskLine,TaskIdx,TaskState,StateUpdateTime, TaskConditonType, FinishContitionRecord,CreateTime) values( "\
	"'%u', '%u', '%u', '%u', '%lld', '%u','%s', now())"

/// 新增玩家竞技信息
#define SQL_ADD_PALYER_RACE_DATA_STRING "insert into race (PlayerID,Rank,ContiWin,RecvEncourageTime,LastChallTime, ChallTimes, ClrCDFlag, UnreadBattleLogNum, RecvEncourageRank, CreateTime) values( "\
	"'%u', '%u', '%u', '%lld', '%lld', '%u','%u', '%u', '%u', now())"

/// 新增玩家丹药信息
#define SQL_ADD_PALYER_DRUG_DATA_STRING "insert into drug (PlayerID,HeroID,Level,Num,CreateTime) values( "\
	"'%u', '%u', '%u', '%u', now())"

/////////////////////////////删除///////////////////////////////////////////////////////////


/// 删除玩家装备信息
//#define SQL_DEL_PALYER_EQUIP_DATA_STRING "update equip set DeleteFlag = '1', DetelteTime = now() where PlayerID = '%u' and KindID = '%u' and Idx = '%u'"
#define SQL_DEL_PALYER_EQUIP_DATA_STRING "delete from equip where PlayerID = '%u' and KindID = '%u' and Idx = '%u'"

/// 删除玩家物品信息
//#define SQL_DEL_PALYER_GOODS_DATA_STRING "update goods set DeleteFlag = '1', DetelteTime = now() where PlayerID = '%u' and KindID = '%u' and Idx = '%u'"
#define SQL_DEL_PALYER_GOODS_DATA_STRING "delete from goods where PlayerID = '%u' and KindID = '%u'"



/////////////////////////////xxx///////////////////////////////////////////////////////////

/// 获取场景副本战斗记录
#define SQL_READ_PALYER_INSTANCE_RECORD_DATA_STRING "select * from instancerecord "\
	" where PlayerID = '%u' order by SceneIndex,TownIndex,InstanceIndex "

/// 查询是否存在场景副本战斗记录
#define SQL_QUERY_PALYER_INSTANCE_RECORD_DATA_STRING "select * from instancerecord "\
	" where PlayerID = '%u' and SceneIndex = '%u' and TownIndex = '%u' and InstanceIndex = '%u'" 

/// 添加副本战斗记录
#define SQL_ADD_INSTANCE_RECORD_DATA_STRING "insert into instancerecord(PlayerID, SceneIndex, TownIndex, InstanceIndex, Score, EnterTimes, LastPassTime, CreateTime, UpdateTime)"\
	" values('%u', '%u', '%u', '%u', '%u', '%u', '%lld', now(), now())"

/// 更新副本战斗记录
#define SQL_UPDATE_INSTANCE_RECORD_DATA_STRING "update instancerecord set EnterTimes='%u', Score='%u', LastPassTime='%lld',  UpdateTime=now()"\
	" where PlayerID = '%u' and SceneIndex = '%u' and TownIndex = '%u' and InstanceIndex = '%u'" 

/// 添加当前副本记录
#define SQL_ADD_CURINSTANCE_RECORD_DATA_STRING "insert into curinstance(PlayerID, CurSceneIndex, CurTownIndex, CurInstanceIndex, CurBattleIndex, CreateTime, UpdateTime)"\
	" values('%u', '%u', '%u', '%u', '%u', now(), now())"

/// 更新当前副本记录
#define SQL_UPDATE_CURINSTANCE_RECORD_DATA_STRING "update curinstance set CurSceneIndex='%u',CurTownIndex='%u',CurInstanceIndex='%u',CurBattleIndex='%u',UpdateTime=now()"\
	" where PlayerID = '%u'" 

/// 添加科技属性加成记录
#define SQL_ADD_SCIENCE_ATTREXT_DATA_STRING "insert into scienceattr(PlayerID, AttrKindID, AttrLevel,CreateTime, UpdateTime)"\
	" values('%u', '%u', '%u', now(), now())"

/// 更新科技属性加成
#define SQL_UPDATE_SCIENCE_ATTREXT_DATA_STRING "update scienceattr set AttrLevel='%u', UpdateTime=now()"\
	" where PlayerID = '%u' and AttrKindID = '%u'" 

/// 更新任务信息
#define SQL_UPDATE_TASK_DATA_STRING "update curtask set TaskIdx='%u', StateUpdateTime='%lld',TaskState='%u', TaskConditonType='%u',FinishContitionRecord='%s', UpdateTime=now()"\
	" where PlayerID = '%u' and TaskLine='%u'" 


/// 添加当前位置信息
#define SQL_ADD_LOCALE_DATA_STRING "insert into locale(PlayerID, LocaleType, SceneIndex, TownIndex, InstanceIndex, BattleIndex, CreateTime, UpdateTime)"\
	" values('%u', '%u', '%u', '%u', '%u', '%u', now(), now())"

/// 更新当前位置信息
#define SQL_UPDATE_LOCALE_DATA_STRING "update locale set LocaleType='%u', SceneIndex='%u',TownIndex='%u', InstanceIndex='%u',BattleIndex='%u', UpdateTime=now()"\
	" where PlayerID = '%u'" 

/// 更新丹药信息
#define SQL_UPDATE_DRUG_DATA_STRING "update drug set Num='%u', UpdateTime=now()"\
	" where PlayerID = '%u' and HeroID = '%u' and Level = '%u'" 


/// 添加当前状态信息
#define SQL_ADD_STATE_DATA_STRING "insert into curstate(PlayerID, InstanceMaxHP, InstanceCurHP, OpenBoxFlag, UpdateTime)"\
	" values('%u', '%u', '%u', '%u', now())"

/// 更新当前状态信息
#define SQL_UPDATE_STATE_DATA_STRING "update curstate set InstanceMaxHP='%u', InstanceCurHP='%u', DoubleExpNum='%u', OpenBoxFlag='%u', UpdateTime=now()"\
	" where PlayerID = '%u'" 


/// 获取在线奖励信息
#define SQL_GET_ONLINE_ENCOURAGE_RECORD_STRING "select * from onlineencouragerecord where PlayerID='%u' "

/// 添加在线奖励信息
#define SQL_ADD_ONLINE_ENCOURAGE_RECORD_STRING "insert into onlineencouragerecord(PlayerID, RecvDays, LastRecvTime, LastRecvIndex, CreateTime)"\
	" values('%u', 0, 0, 0, now())"

/// 更新在线奖励信息
#define SQL_UPDATE_ONLINE_ENCOURAGE_RECORD_STRING "update onlineencouragerecord set RecvDays='%u', LastRecvTime='%lld', LastRecvIndex='%u', UpdateTime=now()"\
	" where PlayerID = '%u'" 



/// 获取登录奖励信息
#define SQL_GET_LOGIN_ENCOURAGE_RECORD_STRING "select * from loginencouragerecord where PlayerID='%u' "

/// 添加登录奖励信息
#define SQL_ADD_LOGIN_ENCOURAGE_RECORD_STRING "insert into loginencouragerecord(PlayerID, KeepDays, LastRecvNum, LastRecvTime, LastSelectIndex, StateFlag, CreateTime)"\
	" values('%u', 0, 0, 0, 0, 0, now())"

/// 更新登录奖励信息
#define SQL_UPDATE_LOGIN_ENCOURAGE_RECORD_STRING "update loginencouragerecord set KeepDays='%u', LastRecvNum='%u', LastRecvTime='%lld', LastSelectIndex='%u', StateFlag = '%u',"\
	" UpdateTime=now() where PlayerID = '%u'" 



/// 获取竞技场战报信息
#define SQL_GET_RACE_BATTLE_LOG_STRING "select * from racebattlelog where PlayerID='%u' order by ChallengeTime asc"

/// 添加竞技场战报信息
#define SQL_ADD_RACE_BATTLE_LOG_STRING "insert into racebattlelog(PlayerID, Idx, ChallengeFlag, ChallengeTime, EnemyPlayerID, EnemyDispName, Result, BeginChallengeRank, AfterChallengeRank, CreateTime)"\
	" values('%u', '%u', '%u', '%lld', '%u', '%s', '%u', '%u', '%u', now())"

/// 更新竞技场战报信息
#define SQL_UPDATE_RACE_BATTLE_LOG_STRING "update racebattlelog set ChallengeFlag='%u', ChallengeTime='%lld', EnemyPlayerID='%u', EnemyDispName='%s', Result = '%u',"\
	" BeginChallengeRank='%u', AfterChallengeRank='%u', UpdateTime=now() where PlayerID = '%u' and Idx = '%u' " 



/// 获取掠夺基本信息
#define SQL_GET_PLUNDER_BASE_STRING "select * from plunderbase where PlayerID='%u' "
/// 获取掠夺战报信息
#define SQL_GET_PLUNDER_BATTLE_LOG_STRING "select * from plunderbattlelog where PlayerID='%u' order by PlunderTime asc"
/// 获取掠夺工作位置信息
#define SQL_GET_PLUNDER_WORK_POS_STRING "select * from plundeworkpos where PlayerID='%u' "
/// 获取掠夺俘虏信息
#define SQL_GET_PLUNDER_CAPTURE_STRING "select * from plundercapture where PlayerID='%u' "

/// 添加掠夺基本信息
#define SQL_ADD_PLUNDER_BASE_STRING "insert into plunderbase(PlayerID, CaptureOwnerPlayerID, BeCaptureTime, ExtPlunderNum, LastRefreashWorkPosTime, LastRefreashWorkPosNum, "\
	" LastPlunderSuccessFlag, LastPlunderTime, LastPlunderNum, CaptureGridNum, UnreadBattleLogNum, ClrCDFlag, CreateTime)"\
	" values('%u', '%u', '%lld', '%u', '%lld', '%u',  '%u', '%lld', '%u', '%u', '%u', '%u',now())"
// 更新掠夺基本信息
#define SQL_UPDATE_PLUNDER_BASE_STRING "update plunderbase set CaptureOwnerPlayerID='%u', BeCaptureTime='%lld', ExtPlunderNum='%u', LastRefreashWorkPosTime='%lld', LastRefreashWorkPosNum='%u', "\
	" LastPlunderSuccessFlag='%u', LastPlunderTime='%lld', LastPlunderNum='%u', CaptureGridNum='%u', UnreadBattleLogNum='%u', ClrCDFlag='%u', UpdateTime=now() where PlayerID = '%u'"
/// 添加掠夺战报信息
#define SQL_ADD_PLUNDER_BATTLE_LOG_STRING "insert into plunderbattlelog(PlayerID, Idx, PlunderFlag, PlunderTime, ActivePlayerID, PassivePlayerID, CapturePlayerID, "\
	" ActiveDispName, PassiveDispName, CaptureDispName, Result, PlunderCoin, PlunderScience, CreateTime)"\
	" values('%u', '%u', '%u', '%lld', '%u', '%u', '%u', '%s', '%s', '%s', '%u', '%lld', '%lld', now())"
/// 更新掠夺战报信息
#define SQL_UPDATE_PLUNDER_BATTLE_LOG_STRING "update plunderbattlelog set PlunderFlag='%u', PlunderTime='%lld', ActivePlayerID='%u', PassivePlayerID='%u', CapturePlayerID='%u', "\
	" ActiveDispName='%s', PassiveDispName='%s', CaptureDispName='%s', Result = '%u', PlunderCoin='%lld', PlunderScience='%lld', "\
	" UpdateTime=now() where PlayerID = '%u' and Idx = '%u' " 
/// 添加掠夺工作位置信息
#define SQL_ADD_PLUNDER_WORK_POS_STRING "insert into plundeworkpos(PlayerID, WorkPosIdx, CoinPosLevel, SciencePosLevel, WorkCaptureID, NeedWorkTime, HaveWorkTime, StartWorkTime, CreateTime)"\
	" values('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%lld', now())"
/// 更新掠夺工作位置信息
#define SQL_UPDATE_PLUNDER_WORK_POS_STRING "update plundeworkpos set CoinPosLevel='%u', SciencePosLevel='%u',  WorkCaptureID='%u', NeedWorkTime='%u', HaveWorkTime='%u', StartWorkTime='%lld', "\
	" UpdateTime=now() where PlayerID = '%u' and WorkPosIdx = '%u' " 
/// 添加掠夺俘虏信息
#define SQL_ADD_PLUNDER_CAPTURE_STRING "insert into plundercapture(PlayerID, CapturePlayerID, CaptureTime, ReleaseFlag, ReleaseTime, WorkPosIdx, CreateTime)"\
	" values('%u', '%u', '%lld', '%u', '%lld', '%u', now())"
/// 更新掠夺俘虏信息
#define SQL_UPDATE_PLUNDER_CAPTURE_STRING "update plundercapture set CaptureTime='%lld', ReleaseFlag='%u', ReleaseTime='%lld', WorkPosIdx='%u', "\
	" UpdateTime=now() where PlayerID = '%u' and CapturePlayerID='%u' " 
/// 清除俘虏信息
#define SQL_DELETE_PLUNDER_CAPTURE_STRING "delete from plundercapture where PlayerID = '%u' and CapturePlayerID='%u' " 
/// 添加掠夺工作报信息
#define SQL_ADD_PLUNDER_WORK_LOG_STRING "insert into plunderworklog(PlayerID, Idx, WorkFlag, WorkTime, WorkCaptureID, CaptureOwnerID, UpperOwnerID, "\
	" WorkCaptureDispName, CaptureOwnerDispName, UpperOwnerDispName, WorkCoin, HandinCoin, CreateTime)"\
	" values('%u', '%u', '%u', '%lld', '%u', '%u', '%u', '%s', '%s', '%s', '%lld', '%lld', now())"
/// 更新掠夺战报信息
#define SQL_UPDATE_PLUNDER_WORK_LOG_STRING "update plunderworklog set WorkFlag='%u', WorkTime='%lld', WorkCaptureID='%u', CaptureOwnerID='%u', UpperOwnerID='%u', "\
	" WorkCaptureDispName='%s', CaptureOwnerDispName='%s', UpperOwnerDispName='%s', PlunderCoin='%lld', PlunderScience='%lld', "\
	" UpdateTime=now() where PlayerID = '%u' and Idx = '%u' " 


/// 获取仇敌信息
#define SQL_GET_ENEMY_STRING "select * from enemy where PlayerID='%u' order by AddTime asc"
/// 添加仇敌信息
#define SQL_ADD_ENEMY_STRING "insert into enemy(PlayerID, EnemyPlayerID, BattleNum, AddTime, CreateTime)"\
	" values('%u', '%u', '%u', '%lld',now())"
/// 更新仇敌信息
#define SQL_UPDATE_ENEMY_STRING "update enemy set BattleNum='%u', UpdateTime=now() where PlayerID = '%u' and EnemyPlayerID='%u' " 
/// 清除仇敌信息
#define SQL_DELETE_ENEMY_STRING "delete from enemy where PlayerID = '%u' and EnemyPlayerID='%u' " 


/// 获取摇钱树信息
#define SQL_GET_MONEYTREE_STRING "select * from moneytree where PlayerID='%u' "
/// 添加摇钱树信息
#define SQL_ADD_MONEYTREE_STRING "insert into moneytree(PlayerID, RockNum, RockTime, MoodValue, MoodStartTime, CreateTime)"\
	" values('%u', '%u','%lld','%u','%lld',now())"
/// 更新摇钱树信息
#define SQL_UPDATE_MONEYTREE_STRING "update moneytree set RockNum='%u', RockTime='%lld', MoodValue='%u', MoodStartTime='%lld',"\
	" UpdateTime=now() where PlayerID = '%u'" 


/// 读取武将器魂信息
#define SQL_READ_GET_HERO_GODWEAPON_DATA_STRING "select * from godweapon "\
	" where PlayerID = '%u' and HeroKindID = '%u'"
/// 添加武将器魂信息
#define SQL_ADD_HERO_GODWEAPON_STRING "insert into godweapon(PlayerID, HeroKindID, Quality, Level, CreateTime)"\
	" values('%u','%u','%u','%u',now())"
/// 更新武将器魂信息
#define SQL_UPDATE_HERO_GODWEAPON_STRING "update godweapon set Quality='%u', Level='%u', "\
	" UpdateTime=now() where PlayerID = '%u' and HeroKindID='%u' " 
/// 删除武将器魂信息
#define SQL_DELETE_HERO_GODWEAPON_STRING "delete from godweapon where PlayerID = '%u' and HeroKindID='%u' " 



/// 读取玩家悟道信息
#define SQL_READ_GET_AWAKEN_DATA_STRING "select * from awaken "\
	" where PlayerID = '%u'"
/// 读取玩家心得信息
#define SQL_READ_GET_PLAYER_STUDY_DATA_STRING "select * from study "\
	" where PlayerID = '%u' order by HeroKindID"
/// 读取玩家心得碎片信息
#define SQL_READ_GET_STUDY_CLIP_DATA_STRING "select * from studyclip "\
	" where PlayerID = '%u'"
/// 添加玩家悟道信息
#define SQL_ADD_AWAKEN_STRING "insert into awaken(PlayerID, Level, Auto2ExpFlag, StudyExp, CreateTime)"\
	" values('%u','%u','%u','%lld',now())"
/// 更新玩家悟道信息
#define SQL_UPDATE_AWAKEN_STRING "update awaken set Level='%u', Auto2ExpFlag='%u', StudyExp='%lld' "\
	" UpdateTime=now() where PlayerID = '%u' " 
/// 新增玩家心得信息
#define SQL_ADD_STUDY_DATA_STRING "insert into study (PlayerID, ID, HeroKindID,GridIdx,Kind,Color, Level, CreateTime) values( "\
	"'%u', '%u', '%u', '%u', '%u', '%u', '%u', now())"
///更新玩家心得信息
#define SQL_UPDATE_STUDY_DATA_STRING "update study set HeroKindID='%u',GridIdx='%u',Kind='%u',Color='%u', Level='%u' "\
	" UpdateTime=now() where PlayerID = '%u' and ID='%u' " 
/// 新增玩家心得碎片信息
#define SQL_ADD_STUDYCLIP_DATA_STRING "insert into studyclip (PlayerID, ID, GridIdx,PickupFlag, Color, Num, CreateTime) values( "\
	"'%u', '%u', '%u', '%u', '%u', '%u', now())"
///更新玩家心得碎片信息
#define SQL_UPDATE_STUDYCLIP_DATA_STRING "update studyclip set GridIdx='%u', PickupFlag='%u', Color='%u', Num='%u' "\
	" UpdateTime=now() where PlayerID = '%u' and ID='%u' " 



/// 添加挂机信息
#define SQL_ADD_ONHOOK_RECORD_STRING "insert into onhookrecord(PlayerID, SceneIndex, TownIndex, InstanceIndex, LastOnhookResultReqTime, LastOnhookResult, CreateTime)"\
	" values('%u', '%u', '%u', '%u', '%lld', '%s', now())"
/// 更新挂机信息
#define SQL_UPDATE_ONHOOK_RECORD_STRING "update onhookrecord set SceneIndex='%u', TownIndex='%u', InstanceIndex='%u', LastOnhookResultReqTime='%lld', LastOnhookResult='%s', "\
	" UpdateTime=now() where PlayerID = '%u' " 
/// 获取挂机信息
#define SQL_GET_ONHOOK_RECORD_STRING "select * from onhookrecord where PlayerID = '%u' " 


/// 添加副本战斗记录信息信息
#define SQL_ADD_COMMON_BATTLE_RECORD_STRING "insert into instancebattlerecord(PlayerID, SceneIndex, TownIndex, InstanceIndex, BattleIndex, BattleTime, CreateTime)"\
	" values('%u', 1, 0, 0, 0, 0, now())"
#define SQL_ADD_ELITE_BATTLE_RECORD_STRING "insert into instancebattlerecord(PlayerID, SceneIndex, TownIndex, InstanceIndex, BattleIndex, BattleTime, CreateTime)"\
	" values('%u', 2, 0, 0, 0, 0, now())"
/// 更新副本战斗记录信息信息
#define SQL_UPDATE_BATTLE_RECORD_STRING "update instancebattlerecord set TownIndex='%u', InstanceIndex='%u', BattleIndex='%u', BattleTime='%lld', "\
	" UpdateTime=now() where PlayerID = '%u' and SceneIndex = '%u' " 
/// 获取副本战斗记录信息信息
#define SQL_GET_BATTLE_RECORD_STRING "select * from  instancebattlerecord where PlayerID = '%u' and SceneIndex = '%u' " 

/// 读取玩家技能信息
#define SQL_READ_GET_PLAYER_SKILL_STRING "select * from skill "\
	" where PlayerID = '%u'"
/// 添加技能信息
#define SQL_ADD_SKILL_STRING "insert into skill(PlayerID, ID, Level, SlotIdx, CreateTime)"\
	" values('%u', '%u', '%u', '%u',now())"
/// 更新技能信息
#define SQL_UPDATE_SKILL_STRING "update skill set Level='%u', SlotIdx='%u', UpdateTime=now() where PlayerID = '%u' and ID='%u' " 


/// 添加体力信息
#define SQL_ADD_PHYSTRENGTH_STRING "insert into phystrength(PlayerID, LastPhyStrengthRefreshTime, LastBuyPhyStrengthTime, LastBuyPhyStrengthNum, CreateTime)"\
	" values('%u', 0, 0, 0, now())"

/// 获取体力信息
#define SQL_GET_PHYSTRENGTH_STRING "select * from  phystrength where PlayerID = '%u' " 
/// 更新体力信息
#define SQL_UPDATE_PHYSTRENGTH_STRING "update phystrength set LastPhyStrengthRefreshTime='%lld', LastBuyPhyStrengthTime='%lld', LastBuyPhyStrengthNum='%u', UpdateTime=now()"\
	" where PlayerID = '%u'" 











































#endif //#ifndef _DBSQLSTRING_H_

