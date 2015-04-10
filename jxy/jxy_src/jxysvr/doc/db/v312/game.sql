/*
    跨服战
    Date: 2014-06-20 09:57:29
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

		INSERT into faction(FactionID, FactionPlayerID, FactionName, FactionRank, FactionLevel, FactionNotice, FactionAnnouncement, FactionLogDataLst, FactionCreateTime, 
		RareItemRoomLevel, GymnasiumLevel, FactionFunds, FactionTaskSchedule, ShadeLevel, HotSpringLevel, ColdPrisonLevel, FactionTowerLevel, CreateFactionPlayerID, FactionFundsSum, UpdateTime, GvGFormation, GvGLog, GVGFactionFunds, SumPower) VALUES(v_FactionID, v_PlayerID, v_FactionName, 0, 1, '', v_FactionAnnouncement, '', now(), 0, 0, 0, '', 0, 0, 0, 0, v_PlayerID, 0, NOW(), '', '', 0, 0);

		INSERT into factionplayer(FactionID, JoinPlayerID, JobType, UnLoginBattleLog, ContinueKillKey) VALUES (v_FactionID, v_PlayerID, 1, '', '');

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
		ALTER TABLE `faction` drop column `FactionPlayerDataLst`;  
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

DROP TABLE IF EXISTS `gvggift`;
CREATE TABLE `gvggift` (
  `PlayerID` int(11) unsigned NOT NULL DEFAULT '0',
  `SessionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `GvGRound` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BattleIdx` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BattleResult` smallint(5) unsigned NOT NULL DEFAULT '0',
  `FactionName` varchar(256) NOT NULL DEFAULT '',
  `GiftID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `GiftType` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Rank` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerID`,`SessionID`,`GvGRound`,`BattleIdx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for players_log
-- ----------------------------
DROP TABLE IF EXISTS `players_log`;
CREATE TABLE `players_log` (
  `PlayerID` int(11) unsigned NOT NULL DEFAULT '0',
  `ModuleID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Value` blob NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

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
