/*
    跨服战
    1. 跨服战建筑解锁
    2. 
    Date: 2013-12-19 09:57:29
*/

-- ----------------------------
-- 修改存储过程：创建帮派
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_CreateFaction`;
DELIMITER $$
CREATE PROCEDURE `sp_CreateFaction`(IN `v_ZoneID` int, IN `v_PlayerID` int,IN `v_FactionName` varchar(255),IN `v_FactionAnnouncement` varchar(255))
lable:
BEGIN
		DECLARE v_FactionID int UNSIGNED DEFAULT 0;
		DECLARE v_MaxFactionID  int UNSIGNED DEFAULT 0;
		declare v_ErrCode int default 0;  
		declare v_ErrStr varchar(120) default 'Success!';

		declare exit handler for sqlexception
		BEGIN
		 		set v_ErrCode = 99;
		 		set v_FactionID = 0;
		 		select v_ErrCode as errcode, v_FactionID as factionid;
		 		rollback;
		END;

		
		start transaction; 

		IF  EXISTS(select 1 from faction where FactionName = v_FactionName ) THEN
		BEGIN
					set v_ErrCode = 5;
					set v_FactionID = 0;
					select v_ErrCode as errcode, v_FactionID as factionid;
					rollback;
					leave lable;
		END;
		END IF;

		IF  EXISTS(select 1 from factionplayer where JoinPlayerID = v_PlayerID and JoinState = 0) THEN
		BEGIN
					set v_ErrCode = 2;
					set v_FactionID = 0;
					select v_ErrCode as errcode, v_FactionID as factionid;
					rollback;
					leave lable;
		END;
		END IF;


		
		select MAX(FactionID % 1000000) into v_MaxFactionID from faction;

		IF v_MaxFactionID is NULL THEN
			BEGIN
					set v_MaxFactionID = 0;
			END;
		END IF;

		
		set 	  v_MaxFactionID = v_MaxFactionID % 1000000;
		set 		v_FactionID = (v_ZoneID * 1000000) + v_MaxFactionID + 1;


		
		DELETE  from factionplayer where JoinPlayerID = v_PlayerID; 

		INSERT into faction(FactionID, FactionPlayerID, FactionName, FactionRank, FactionLevel, FactionNotice, FactionAnnouncement, FactionPlayerDataLst, FactionLogDataLst, FactionCreateTime, 
		RareItemRoomLevel, GymnasiumLevel, FactionFunds, FactionTaskSchedule, ShadeLevel, HotSpringLevel, ColdPrisonLevel, FactionTowerLevel, CreateFactionPlayerID, FactionFundsSum, UpdateTime, GvGFormation, GvGLog, GVGFactionFunds, SumPower) VALUES(v_FactionID, v_PlayerID, v_FactionName, 0, 1, '', v_FactionAnnouncement, '', '', now(), 0, 0, 0, '', 0, 0, 0, 0, v_PlayerID, 0, NOW(), '', '', 0, 0);

		INSERT into factionplayer(FactionID, JoinPlayerID, JobType, UnLoginBattleLog) VALUES (v_FactionID, v_PlayerID, 1, '');

		select v_ErrCode as errcode, v_FactionID as factionid;

		commit;
END
$$

-- ----------------------------
-- 添加表字段：给门派添加字段
-- ----------------------------
DROP PROCEDURE if exists AlterTable;
CREATE PROCEDURE AlterTable ()
BEGIN

	declare v_sqlstr  varchar(1024) default ''; 
	set 		@g_cur_db = '';
	set 		v_sqlstr = concat('select database() into @g_cur_db');
	set 		@sqlstr = v_sqlstr;
	prepare m_str from @sqlstr;
	execute m_str;

	-- 检测原来的字段是否存在，如不存在则添加字段
	if exists(select 1 from information_schema.columns where table_schema = @g_cur_db and table_name = 'faction' and column_name = 'GvGFormation' ) then
		select 1 as 门派表的GvGFormation字段已存在;
	else
		-- 直接添加新字段
		ALTER TABLE `faction` ADD COLUMN `GvGFormation`  blob NOT NULL COMMENT '门派gvg布阵信息' AFTER `Ext`;
		ALTER TABLE `faction` ADD COLUMN `GvGLog`  blob NOT NULL COMMENT '门派gvg战报' AFTER `GvGFormation`;
		ALTER TABLE `faction` ADD COLUMN `GVGFactionFunds`  bigint(20) UNSIGNED NOT NULL DEFAULT 0 AFTER `GvGLog`;
		ALTER TABLE `faction` ADD COLUMN `SumPower`  int(11) NOT NULL DEFAULT 0 AFTER `GVGFactionFunds`;
		ALTER TABLE `factionplayer` ADD COLUMN `ContinueKillKey`  blob NOT NULL AFTER `JoinTime`;
	end if;
END
$$
delimiter ;

call AlterTable();
DROP PROCEDURE if exists AlterTable;

-- 跨服战战斗录像
DROP TABLE IF EXISTS `gvgvideo`;
CREATE TABLE `gvgvideo` (
  `VideoID` bigint NOT NULL comment '录像ID',
  `Time` datetime NOT NULL comment '录像时间',
  `BigRound` tinyint NOT NULL comment '大轮次，见 GvGRound',
  `SmallRound` tinyint NOT NULL comment '小轮次',
  `BattleResult` tinyint NOT NULL comment '战斗结果：未开始、左边门派赢、右边门派赢，见 GvGResult',
  `G1` blob NOT NULL comment '门派1信息',
  `G2` blob NOT NULL comment '门派1信息',
  `FinalRoundData` blob NOT NULL comment '最后一场的玩家信息',
  `PKVideoCnt` smallint NOT NULL comment '玩家PK场数',
    PRIMARY KEY (`VideoID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 门派战斗中的玩家PK录像
DROP TABLE IF EXISTS `gvgpkvideo`;
CREATE TABLE `gvgpkvideo` (
  `VideoID` bigint NOT NULL comment '录像ID',
  `PKNum` int NOT NULL comment '该场是第几场',
  `PlayerID1` int NOT NULL comment '左边玩家ID',
  `PlayerID2` int NOT NULL comment '右边玩家ID',  
  `BattleResult` tinyint NOT NULL comment '战斗结果：0为我方赢，1对方赢，见 EBattleResult',
  `VideoData` blob NOT NULL comment 'pk录像数据',
    PRIMARY KEY (`VideoID`,`PKNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- 部分玩家的快照，供跨服战使用
DROP TABLE IF EXISTS `player_snapshot`;
CREATE TABLE `player_snapshot` (
    `PlayerID`  int(11) unsigned NOT NULL COMMENT '角色ID',
    `PlayerData` blob not null comment '玩家数据',
    `SizeOfPlayerData` int not null comment '玩家数据的最大长度 = sizeof(DT_PLAYER_NOPVP_DATA)',
    PRIMARY KEY (`PlayerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 跨服战门派的活动数据
DROP TABLE IF EXISTS `gvgactivity`;
CREATE TABLE `gvgactivity` (
	`SessionID` smallint not null comment '当前是第几届',
    `StartTime` datetime NOT NULL comment '本次跨服门派战活动的开始时间',
    `CurStage` tinyint not null comment '本次门派跨服战的当前阶段',
    `ArenaCnt` smallint not null comment '赛区数',
    PRIMARY KEY (`SessionID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# 门派跨服中的竞技场表
DROP TABLE IF EXISTS `gvgarena`;
CREATE TABLE `gvgarena` (
  `ArenaID` tinyint unsigned NOT NULL comment '赛区ID，范围：1~200',
  `CurStage` tinyint not null comment '本次门派跨服战的当前阶段',
  `IsDone`  tinyint unsigned NOT NULL comment '该赛区是否已完成当前阶段的任务',
  `ZoneCnt` tinyint unsigned NOT NULL comment '该赛区由多少个区组成',
  `ZoneListData` blob NOT NULL comment '该赛区的区成员列表',
  `GroupZoneIDList` blob NOT NULL comment '该赛区的组列表',
  `War16To8To4Data` blob NOT NULL comment '后2个服的16进8、8进4的数据',
  `FinalWarData` blob NOT NULL comment '四服积分总决赛的数据',
  `FactionsGvGLogData` blob NOT NULL comment '各个参赛选手的门派战况',
  `StageDoneFlagList` blob NOT NULL comment '标识本赛区的各个阶段是否已完成任务',
  PRIMARY KEY (ArenaID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 游戏服务器中本区的门派跨服战数据
DROP TABLE IF EXISTS `gvgbuild`;
CREATE TABLE `gvgbuild` (
  `SessionID` smallint not null comment '当前是第几届',
  `ArenaID` smallint unsigned NOT NULL comment '本区所属赛区ID',
  `StartTime` datetime NOT NULL comment '本次跨服门派战活动的开始时间',
  `CurStage` tinyint not null comment '本次门派跨服战的当前阶段',
  `ApplayFactionData` blob NOT NULL,
  `LastResult` blob NOT NULL,
  `MoneyResult` blob NOT NULL,
  `ChaosWarData` blob NOT NULL,
  `DoubleWarInfo` blob NOT NULL,
  `War16To8List` blob NOT NULL,
  `FinalWarData` blob NOT NULL comment '四服积分总决赛数据',
  `FactionsGvGLogData` blob NOT NULL comment '各个参赛选手的门派战况',
  `StageFlagList` blob NOT NULL comment '标记各个阶段是否已完成任务',  
  PRIMARY KEY (`SessionID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

DROP TABLE IF EXISTS `players_log`;
CREATE TABLE `players_log` (
	`PlayerID`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
	`ModuleID`  smallint(5) UNSIGNED NOT NULL DEFAULT 0 ,
	`Value`  blob NOT NULL ,
	`LogTime`  datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- ----------------------------
-- 添加表：跨服战配置表
-- ----------------------------
use ios_app_cfg;

DROP TABLE IF EXISTS `gvgprop`;
CREATE TABLE `gvgprop` (
	`ForbidFormationTime` int(11) unsigned NOT NULL COMMENT '门派跨服战中，比赛开始前多长时间内停止布阵',
	`SyncInterval` int(11) unsigned NOT NULL COMMENT 'gvg中定时同步数据到各服的间隔',
	`RePullInterval` int(11) unsigned NOT NULL COMMENT 'gvg中定时到各服调取数据的间隔',
	`GambleReward` int(11) unsigned NOT NULL COMMENT '押注奖励铜钱',
	`HonorLevel` smallint unsigned NOT NULL COMMENT '荣誉奖励要求等级',
	`WinAddScore` smallint unsigned NOT NULL COMMENT '总决赛中获胜者获得的积分',
	`LoseAddScore` smallint unsigned NOT NULL COMMENT '总决赛中失败者获得的积分',
	`ChaosWarEachRoundTime` int NOT NULL COMMENT '门派大乱斗每轮的时间',
	`FinalWarEachRoundTime` int NOT NULL COMMENT '四服积分总决赛每轮的时间',
	`JoinGVGPlayerLevel` smallint(5) unsigned NOT NULL DEFAULT '40' comment '加入GVG要求玩家等级',
	`JoinGVGFactionLevel` smallint(5) unsigned NOT NULL DEFAULT '4' comment '加入GVG要求门派等级',
	`JoinGvGRank` smallint(5) unsigned NOT NULL DEFAULT '200' comment '加入GVG要求的门派排名',

	`AutoJoinFactionCnt` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT '门派跨服战活动开启时让多少个门派自动报名',
	`AutoFormationCnt` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT '门派报名时让多少个成员自动上阵',
	`IsFakeActTime` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT '是否伪造跨服战时间',

	`IsTest` int unsigned NOT NULL DEFAULT 0 COMMENT '是否是测试，测试时不发送任何奖励和公告',
	`FakeGap` int unsigned NOT NULL DEFAULT 0 COMMENT '测试时各轮次间的间隔'
  -- `gvgretryinterval` smallint(11) not null comment 'gvg中定时到各服调取数据的间隔',
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `gvgprop`(ForbidFormationTime, SyncInterval, RePullInterval, GambleReward, HonorLevel, WinAddScore, LoseAddScore, ChaosWarEachRoundTime, FinalWarEachRoundTime, JoinGVGPlayerLevel, JoinGVGFactionLevel, JoinGvGRank, AutoJoinFactionCnt, AutoFormationCnt, IsFakeActTime, IsTest, FakeGap) VALUES (3600,30,10,100,20,10,3, 30, 60, 40, 4, 200, 20, 11, 1, 0, 30);

-- ----------------------------
-- Table structure for gvgroundprop
-- ----------------------------
DROP TABLE IF EXISTS `gvgroundprop`;
CREATE TABLE `gvgroundprop` (
  `RoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ContinuKill` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`RoundID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of gvgroundprop
-- ----------------------------
INSERT INTO `gvgroundprop` VALUES ('2', '100');
INSERT INTO `gvgroundprop` VALUES ('3', '200');
INSERT INTO `gvgroundprop` VALUES ('4', '300');
INSERT INTO `gvgroundprop` VALUES ('5', '400');
INSERT INTO `gvgroundprop` VALUES ('6', '500');


# 门派跨服战的活动各轮次时间
DROP TABLE IF EXISTS `gvgtimeprop`;
CREATE TABLE `gvgtimeprop` (
  `Round` tinyint(4) unsigned NOT NULL comment '门派跨服战轮次',
  `BeginDay` tinyint unsigned NOT NULL COMMENT '开始的天数，在一周中的第几天开始该轮活动，1-7分别代表周一~周日，8-14分别代表下周的周一~周日',
  `BeginTime` char(6) NOT NULL COMMENT '开始的秒数，在当天的第几秒开始该轮活动，如18:00',
  `EndDay` tinyint unsigned NOT NULL COMMENT '结束该轮活动的天数',
  `EndTime` char(6) NOT NULL COMMENT '结束该轮活动的秒数',
  PRIMARY KEY (`Round`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `gvgtimeprop` VALUES (1, 1,  '00:00', 7,  '00:00');
INSERT INTO `gvgtimeprop` VALUES (2, 8,  '00:00', 0,  '');
INSERT INTO `gvgtimeprop` VALUES (3, 10, '19:00', 10, '22:00');
INSERT INTO `gvgtimeprop` VALUES (4, 11, '00:00', 11, '20:00');
INSERT INTO `gvgtimeprop` VALUES (5, 12, '00:00', 12, '20:00');
INSERT INTO `gvgtimeprop` VALUES (6, 13, '00:00', 13, '18:00');
INSERT INTO `gvgtimeprop` VALUES (7, 13, '21:00', 0,  '');

/*
DROP TABLE IF EXISTS `gvgtimeprop_new`;
CREATE TABLE `gvgtimeprop_new` (
  `Round` tinyint(4) unsigned NOT NULL comment '门派跨服战轮次',
  `BeginTime` datetime NOT NULL COMMENT '进入该轮次的时间，日期部分表示在一周中的第几天开始该轮次，1-7分别代表周一~周日，8-14分别代表下周的周一~周日，时分秒部分则对应在当天的第几秒开始该轮活动，如0000-00-01 18:00:00表示周一18点开始',
  `EndTime` datetime NOT NULL default 0 COMMENT '结束该轮活动的时间，为0时无效',
  PRIMARY KEY (`Round`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `gvgtimeprop_new` VALUES (1, '0000-00-07 00:00:00', '0');
INSERT INTO `gvgtimeprop_new` VALUES (2, '0000-00-08 00:00:00', '0');
INSERT INTO `gvgtimeprop_new` VALUES (3, '0000-00-10 19:00:00', '0000-00-10 22:00:00');
INSERT INTO `gvgtimeprop_new` VALUES (4, '0000-00-11 00:00:00', '0000-00-11 20:00:00');
INSERT INTO `gvgtimeprop_new` VALUES (5, '0000-00-12 00:00:00', '0000-00-12 20:00:00');
INSERT INTO `gvgtimeprop_new` VALUES (6, '0000-00-13 00:00:00', '0000-00-13 18:00:00');
INSERT INTO `gvgtimeprop_new` VALUES (7, '0000-00-13 21:00:00', '0');
*/

# 门派跨服战的活动开启时间表
DROP TABLE IF EXISTS `gvgopendateprop`;
CREATE TABLE `gvgopendateprop` (
  `SessionID` smallint unsigned NOT NULL comment '第几届',
  `OpenDate` date NOT NULL default 0 comment '该届跨服门派战活动的开启日期',
  PRIMARY KEY (SessionID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# 门派跨服战各赛区的活动开启时间表
DROP TABLE IF EXISTS `gvgarenaopendateprop`;
CREATE TABLE `gvgopendateprop` (
  `ArenaID` smallint unsigned NOT NULL comment '第几届',
  `OpenDate` date NOT NULL default 0 comment '该届跨服门派战活动的开启日期',
  PRIMARY KEY (SessionID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# curdate()
# insert into gvgopendateprop(SessionID, OpenDate) values (1, DATE_ADD('2014-05-27', INTERVAL 20 DAY));

drop procedure if exists generate_gvg_date;
create procedure generate_gvg_date()
begin
	declare v_sqlstr  varchar(1024) default ''; 	
	declare i int;
	declare adddays int;
	set i=0;
	
	while i<200 do
			set adddays=i*14;
			# set 		v_sqlstr = concat("insert into gvgopendateprop(SessionID, OpenDate) values (", i + 1, ", CURDATE())");
			set 		v_sqlstr = concat("insert into gvgopendateprop(SessionID, OpenDate) values (", i + 1, ", DATE_ADD('2014-06-02', INTERVAL ", i * 14, " DAY))");
			set 		@sqlstr = v_sqlstr;
			prepare m_str from @sqlstr;
			execute m_str;

	    set i=i+1;
	end while;
end;

call generate_gvg_date();
drop procedure if exists generate_gvg_date;

# 门派跨服战擂台配置表
DROP TABLE IF EXISTS `gvgarenaprop`;
CREATE TABLE `gvgarenaprop` (
  `ArenaID` tinyint unsigned NOT NULL comment '擂台ID，范围：1~200',
  `GroupID` smallint unsigned NOT NULL comment '组别',
  `ZoneID` smallint unsigned NOT NULL comment '区ID',
  `OpenDate` date NOT NULL default 0 comment '该区开启跨服战的日期',
  PRIMARY KEY (ArenaID, ZoneID), unique key(ZoneID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into gvgarenaprop(ArenaID, GroupID, ZoneID, OpenDate) values (1, 1, 106, '2014-06-03');
insert into gvgarenaprop(ArenaID, GroupID, ZoneID, OpenDate) values (1, 2, 107, '2014-06-03');
insert into gvgarenaprop(ArenaID, GroupID, ZoneID, OpenDate) values (1, 1, 108, '2014-06-03');
insert into gvgarenaprop(ArenaID, GroupID, ZoneID, OpenDate) values (1, 2, 109, '2014-06-03');

DROP TABLE IF EXISTS `gvgkillcomboprop`;
CREATE TABLE `gvgkillcomboprop` (
  `KillCombo` tinyint unsigned NOT NULL comment '连斩数',
  `AddScore` smallint unsigned NOT NULL comment '到达此连斩数时获得的积分',
  `RecoverHPPercent` smallint NOT NULL default 0 comment '回复血量百分比，范围1~100，20代表回复血量20%',
  PRIMARY KEY (KillCombo)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into gvgkillcomboprop(KillCombo, AddScore, RecoverHPPercent) values (3, 1, 5);
insert into gvgkillcomboprop(KillCombo, AddScore, RecoverHPPercent) values (5, 2, 10);
insert into gvgkillcomboprop(KillCombo, AddScore, RecoverHPPercent) values (10, 3, 20);



replace into msgdefprop(CliRegionType, MsgKey, Msg) values(0, 'ZONE_NAME', '{x}区');