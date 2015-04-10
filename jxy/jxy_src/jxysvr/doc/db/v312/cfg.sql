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
	`FakeGap` int unsigned NOT NULL DEFAULT 0 COMMENT '测试时各轮次间的间隔',
	`OpenUpRequiredFactionCnt` int unsigned NOT NULL DEFAULT 0 COMMENT '一个区需要多少门派达到等级才能开启门派跨服战活动',
	`AutoApplyExceptZone` int unsigned NOT NULL DEFAULT 0 COMMENT '禁止指定区自动报名',
	`DisplayVideoMVPCnt` smallint unsigned NOT NULL DEFAULT 0 COMMENT '观看战斗时连斩达人列表的默认显示数量',
	`UsePlayerSnapShot` smallint unsigned NOT NULL DEFAULT 0 COMMENT '是否使用玩家快照',
	`IsOpenActivity` smallint unsigned NOT NULL DEFAULT 0 COMMENT '是否开放门派跨服战，跨服门派战的开关'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `gvgprop`(ForbidFormationTime, SyncInterval, RePullInterval, GambleReward, HonorLevel, WinAddScore, LoseAddScore, ChaosWarEachRoundTime, FinalWarEachRoundTime, JoinGVGPlayerLevel, JoinGVGFactionLevel, JoinGvGRank, AutoJoinFactionCnt, AutoFormationCnt, IsFakeActTime, IsTest, FakeGap, OpenUpRequiredFactionCnt, AutoApplyExceptZone, DisplayVideoMVPCnt, UsePlayerSnapShot, IsOpenActivity) VALUES (3600,30,10,100,20,50,25, 30, 60, 40, 4, 200, 20, 11, 1, 0, 30, 10, 0, 10, 1, 1);

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

# curdate()
# insert into gvgopendateprop(SessionID, OpenDate) values (1, DATE_ADD('2014-05-27', INTERVAL 20 DAY));

drop procedure if exists generate_gvg_date;
create procedure generate_gvg_date()
begin
	declare v_sqlstr  varchar(1024) default ''; 	
	declare i int;
	declare adddays int;
	set i=0;
	
	while i<50 do
			set adddays=i*14;
			# set 		v_sqlstr = concat("insert into gvgopendateprop(SessionID, OpenDate) values (", i + 1, ", CURDATE())");
			set 		v_sqlstr = concat("insert into gvgopendateprop(SessionID, OpenDate) values (", i + 1, ", DATE_ADD('2014-07-27', INTERVAL ", i * 14, " DAY))");
			set 		@sqlstr = v_sqlstr;
			prepare m_str from @sqlstr;
			execute m_str;

	    set i=i+1;
	end while;
end;

call generate_gvg_date();
drop procedure if exists generate_gvg_date;

DROP TABLE IF EXISTS `gvgunlockprop`;
CREATE TABLE `gvgunlockprop` (
  `ArenaID` smallint unsigned NOT NULL comment '第几届',
  `UnLockSession` smallint NOT NULL default 0 comment '该赛区将在第几届解锁门派跨服战活动',
  PRIMARY KEY (ArenaID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into gvgunlockprop(ArenaID, UnLockSession) values (1, 1);

# 门派跨服战擂台配置表
DROP TABLE IF EXISTS `gvgarenaprop`;
CREATE TABLE `gvgarenaprop` (
  `ArenaID` smallint unsigned NOT NULL comment '擂台ID，范围：1~200',
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
  `DoorAward` int unsigned NOT NULL comment '奖励门贡',
  `AddScore` smallint unsigned NOT NULL comment '到达此连斩数时获得的积分',
  `RecoverHPPercent` smallint NOT NULL default 0 comment '回复血量百分比，范围1~100，20代表回复血量20%',
  PRIMARY KEY (KillCombo)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into gvgkillcomboprop values (1,  100,  0, 0);
insert into gvgkillcomboprop values (2,  200,  0, 0);
insert into gvgkillcomboprop values (3,  300,  1, 5);
insert into gvgkillcomboprop values (4,  400,  0, 0);
insert into gvgkillcomboprop values (5,  500,  0, 0);
insert into gvgkillcomboprop values (6,  600,  2, 10);
insert into gvgkillcomboprop values (7,  700,  0, 0);
insert into gvgkillcomboprop values (8,  800,  3, 15);
insert into gvgkillcomboprop values (9,  900,  0, 0);
insert into gvgkillcomboprop values (10, 1000, 4, 20);

DROP TABLE IF EXISTS `gvggambleawardprop`;
CREATE TABLE `gvggambleawardprop` (
  `PlayerLevel` smallint unsigned NOT NULL comment '玩家等级',
  `DoubleWarCoin` int unsigned NOT NULL comment '玩家等级对应的双服押注奖励',
  `FinalWarCoin` int unsigned NOT NULL comment '玩家等级对应的四服押注奖励',
  PRIMARY KEY (PlayerLevel)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into gvggambleawardprop values (80, 	 500000, 1000000);
insert into gvggambleawardprop values (100, 1000000, 1500000);
insert into gvggambleawardprop values (120, 2000000, 2000000);






replace into msgdefprop(CliRegionType, MsgKey, Msg) values(0, 'ZONE_NAME', '{x}区');
replace into msgdefprop(CliRegionType, MsgKey, Msg) values(0, 'GVG_ACTIVITY_WILL_OPEN', '跨服门派战将于{x}月{y}日正式开放，敬请期待');
replace into msgdefprop(CliRegionType, MsgKey, Msg) values(0, 'GVG_ACTIVITY_IS_CLOSED', '跨服门派战还未开放，敬请期待');