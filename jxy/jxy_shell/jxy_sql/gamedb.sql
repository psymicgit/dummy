/*********************************************************************************************
 *   @Brief: v306
 *   @Date:  2014-3-12 16:19:10
 *********************************************************************************************/



/*********************************************************************************************
 *   @brief: 记录玩家的详细元宝操作

			 1. 在player表中新增<充值元宝>、<充值赠送元宝>、<系统赠送元宝>字段

			 2. 将玩家原本的元宝充值和消费记录从oprecord表单独拆分出来，专门存入goldrecord表（每年一个表），并记录每笔元宝的消费记录中，有多少来自于充值元宝、充值赠送元宝、系统赠送元宝和游戏元宝

 *   @Date: 2014-3-11 16:19:10
 *   @author by 洪坤安
 *********************************************************************************************/

DROP PROCEDURE if exists AddColumn;

delimiter $$
CREATE PROCEDURE AddColumn()
BEGIN
	if exists(select 1 from information_schema.columns where table_schema in (select database()) and table_name = 'players') then
		-- 检测原来的字段是否存在，如不存在则添加字段
		if exists(select 1 from information_schema.columns where table_schema = (select database()) and table_name = 'players' and column_name = 'RMBGold') then
			select 1 as 检测到玩家表的RMBGold字段已存在，不必再添加;
		else
			-- 在<元宝>字段后添加<充值元宝>字段
			ALTER TABLE `players` ADD column `RMBGold` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '充值元宝 = 人民币元宝 = 玩家每次充值时的人民币（元） * 10' after `Gold`;
		end if;

		if exists(select 1 from information_schema.columns where table_schema = (select database()) and table_name = 'players' and column_name = 'RMBGoldGift') then
			select 1 as 检测到玩家表的RMBGoldGift字段已存在，不必再添加;
		else
			-- 在<充值元宝>字段后添加<充值赠送元宝>字段
			ALTER TABLE `players` ADD column `RMBGoldGift` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '充值赠送元宝 = 每次充值时实际获得的元宝 - 玩家当次充值的人民币（元） * 10，即每次充值时赠予的额外元宝' after `RMBGold`;
		end if;

		if exists(select 1 from information_schema.columns where table_schema = (select database()) and table_name = 'players' and column_name = 'GmGold') then
			select 1 as 检测到玩家表的GmGold字段已存在，不必再添加;
		else
			-- 在<充值赠送元宝>字段后添加<系统赠送元宝>字段
			ALTER TABLE `players` ADD column `GmGold` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '系统赠送元宝 = 通过GM方式给玩家添加的充值元宝（GM可以给玩家添加充值元宝和普通元宝）' after `RMBGoldGift`;
		end if;

		if exists(select 1 from information_schema.columns where table_schema = (select database()) and table_name = 'players' and column_name = 'OriZoneID') then
			select 1 as 检测到玩家表的OriZoneID字段已存在，不必再添加;
		else
			-- 在<PlayerID>字段后添加<玩家创建角色时所在的区>字段
			ALTER TABLE `players` ADD column `OriZoneID` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '玩家创建角色时所在的区ID（用于元宝日志：元宝日志需要获知玩家最早所在的区）' after `PlayerID`;
		end if;
	else
		select 1 as 错误：找不到玩家表;
	end if;

	if exists(select 1 from information_schema.columns where table_schema = (select database()) and table_name = 'goldrecord_2014') then
		ALTER TABLE `goldrecord_2014` MODIFY column PlayerLevel tinyint unsigned not null;
		ALTER TABLE `goldrecord_2014` MODIFY column VipLevel tinyint unsigned not null;
		ALTER TABLE `goldrecord_2014` MODIFY column ModuleType tinyint unsigned not null comment '元宝增加或减少类型，详见logdmoduletype表';
		ALTER TABLE `goldrecord_2014` MODIFY column OpType tinyint unsigned not null comment '获得或者消耗元宝的方式，详见moduletablename表';

		select 1 as 成功修改goldrecord_2014表;

		if exists(select 1 from information_schema.columns where table_schema = (select database()) and table_name = 'goldrecord_2014' and column_name = 'ZoneID') then
			select 1 as 检测到goldrecord_2014表的ZoneID字段已存在，不必再添加;
		else
			-- 在<PlayerID>字段后添加<玩家创建角色时所在的区>字段
			ALTER TABLE `goldrecord_2014` ADD column `ZoneID` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '玩家创建角色时所在的区ID' after `ID`;
		end if;
	end if;
END
$$

delimiter ;

call AddColumn();
DROP PROCEDURE if exists AddColumn;

/*
		元宝记录比较重要，需要做好备份，所以最好是有做主从，但由于目前日志库没有做从，游戏库则有，做日志库的从的话每台服务器都要专门多加一个mysql实例，比较浪费资源，
	因此，将元宝操作日志表放在游戏库里，而不是放在日志库。

		经统计，1区中的oprecord关于元宝操作的记录数大概为每个月60万条（2014_3_11日统计），每年大约720万条，按年进行存放还是可行的

	select count(1) from ios_app_s001_log.oprecord_20140302224806 WHERE ModuleType =12 or ModuleType = 13; = 124693条
	select count(1) from ios_app_s001_log.oprecord_20140222001126 WHERE ModuleType =12 or ModuleType = 13; = 136620条
	select count(1) from ios_app_s001_log.oprecord_20140213215555 WHERE ModuleType =12 or ModuleType = 13; = 137766条
	select count(1) from ios_app_s001_log.oprecord_20140205154914 WHERE ModuleType =12 or ModuleType = 13; = 160500条
	select count(1) from ios_app_s001_log.oprecord_20140128091958 WHERE ModuleType =12 or ModuleType = 13; = 172779条
*/

-- 拆出新的元宝统计表：玩家的元宝消费记录将按年来进行拆分
DROP PROCEDURE if exists SplitNewGoldRecordTable;

-- 按年拆分出新的goldrecord表，该表记录玩家元宝的每笔消费和获得情况
-- 传入的参数v_NewTableName将是goldrecord_和年的组合，如: goldrecord_2014
delimiter $$
CREATE DEFINER=`seruser`@`%` PROCEDURE `SplitNewGoldRecordTable`(IN `v_NewTableName` varchar(64))
	SQL SECURITY INVOKER
lable_1:
begin	
	-- 根据传入的表名来创建新表
	SET @v_Sql = concat(
	"create table if not exists ", v_NewTableName, "("
		"ID int(10) unsigned not null auto_increment,"
		"ZoneID smallint(6) unsigned not null DEFAULT '0' COMMENT '玩家创建角色时所在的区ID',"
		"PlayerID int(10) unsigned not null,"
		"PlayerLevel tinyint unsigned not null,"
		"VipLevel tinyint unsigned not null,"
		"ModuleType tinyint unsigned not null comment '元宝增加或减少类型，详见logdmoduletype表',"
		"OpType tinyint unsigned not null comment '获得或者消耗元宝的方式，详见moduletablename表',"
		"OpTime datetime not null comment '操作时间',"
		"OpRMBGold int(10) unsigned not null comment '该次操作有多少来自于充值元宝',"
		"OpRMBGoldGift int(10) unsigned not null comment '该次操作有多少来自于充值赠送元宝',"
		"OpGmGold int(10) unsigned not null comment '该次操作有多少来自于系统赠送元宝',"
		"OpGameGold int(10) unsigned not null comment '该次操作有多少来自于游戏内元宝',"
		"AfterRMBGold int(11) unsigned not null comment '之后的充值元宝数',"
		"AfterRMBGoldGift int(11) unsigned not null comment '之后的充值赠送元宝数',"
		"AfterGmGold int(11) unsigned not null comment '之后的系统赠送元宝数',"
		"AfterGameGold bigint(20) unsigned not null comment '之后的游戏元宝数',"
		"Param1 int(11) unsigned not null,"
		"Param2 int(11) unsigned not null,"
		"Param3 int(11) unsigned not null,"
		"Param4 int(11) unsigned not null,"
		"`comment` varchar(128) not null default '',"
		"primary key (`ID`)"
	") engine=innodb default charset=utf8;");

	prepare stml from @v_Sql;
	EXECUTE stml;
end
$$

delimiter ;
;