/*
    将玩家原本的元宝充值和消费记录从oprecord表单独拆分出来，新建goldrecord表，以区分出充值元宝和奖励元宝
    Date: 2014-3-11 16:19:10
    @author by 洪坤安
*/

/*
	经统计，1区中的oprecord关于元宝操作的记录数大概为每10天15万条，同时，这也意味着：每1000万条中约含有15万条元宝相关的操作，这样的比例是很小的，其他区的数据与此类似，所以玩家关于元宝的记录可以每个区只用一个表不用拆表（2014_3_11日统计）

	select count(1) from ios_app_s001_log.oprecord_20140302224806 WHERE ModuleType =12 or ModuleType = 13; = 124693条
	select count(1) from ios_app_s001_log.oprecord_20140222001126 WHERE ModuleType =12 or ModuleType = 13; = 136620条
	select count(1) from ios_app_s001_log.oprecord_20140213215555 WHERE ModuleType =12 or ModuleType = 13; = 137766条
	select count(1) from ios_app_s001_log.oprecord_20140205154914 WHERE ModuleType =12 or ModuleType = 13; = 160500条
	select count(1) from ios_app_s001_log.oprecord_20140128091958 WHERE ModuleType =12 or ModuleType = 13; = 172779条
*/

-- 建立goldrecord表，该表记录玩家元宝的每笔消费和获得情况，其中，玩家身上元宝数 = 充值元宝 + 奖励元宝
drop table if exists `goldrecord`;
create table `goldrecord`(
	ID int(10) unsigned not null auto_increment,
	PlayerID int(10) unsigned not null,
	PlayerLevel smallint(5) unsigned not null,
	VipLevel smallint(6) not null,
	ModuleType smallint(5) unsigned not null comment '元宝增加或减少类型，详见logdmoduletype表',
	OpType smallint(5) unsigned not null comment '获得或者消耗元宝的方式，详见moduletablename表',
	OpTime datetime not null comment '操作时间',
	OpRMBGold int(10) unsigned not null comment '该次操作有多少来自于充值元宝',
	OpRMBGoldGift int(10) unsigned not null comment '该次操作有多少来自于充值赠送元宝',
	OpGmGold int(10) unsigned not null comment '该次操作有多少来自于系统赠送元宝',
	OpGameGold int(10) unsigned not null comment '该次操作有多少来自于游戏内元宝',

	AfterRMBGold bigint(20) unsigned not null comment '之后的充值元宝数',
	AfterRMBGoldGift bigint(20) unsigned not null comment '之后的充值赠送元宝数',
	AfterGmGold bigint(20) unsigned not null comment '之后的系统赠送元宝数',
	AfterGameGold bigint(20) unsigned not null comment '之后的游戏元宝数',

	Param1 bigint(20) unsigned not null,
	Param2 bigint(20) unsigned not null,
	Param3 bigint(20) unsigned not null,
	Param4 bigint(20) unsigned not null,
	`comment` varchar(128) not null default '',
	primary key (`ID`)
) engine=innodb default charset=utf8;

-- 并在player表中新增<充值元宝>字段
delimiter $$
DROP PROCEDURE if exists AddColumn;
CREATE PROCEDURE AddColumn()
BEGIN
	declare v_sqlstr  varchar(1024) default ''; 
	set 		@g_cur_db = '';
	set 		v_sqlstr = concat('select database() into @g_cur_db');

	set 		@sqlstr = v_sqlstr;
	prepare m_str from @sqlstr;
	execute m_str;

	if exists(select 1 from information_schema.columns where table_schema = @g_cur_db and table_name = 'players') then
		-- 检测原来的字段是否存在，如不存在则添加字段
		if exists(select 1 from information_schema.columns where table_schema = @g_cur_db and table_name = 'players' and column_name = 'RMBGold') then
			select 1 as 检测到玩家表的RMBGold字段已存在，不必再添加;
		else
			-- 在<元宝>字段后添加<充值元宝>字段
			ALTER TABLE `players` ADD column `RMBGold` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '充值元宝 = 人民币元宝 = 玩家每次充值时的人民币（元） * 10' after `Gold`;
		end if;

		if exists(select 1 from information_schema.columns where table_schema = @g_cur_db and table_name = 'players' and column_name = 'RMBGoldGift') then
			select 1 as 检测到玩家表的RMBGoldGift字段已存在，不必再添加;
		else
			-- 在<充值元宝>字段后添加<充值赠送元宝>字段
			ALTER TABLE `players` ADD column `RMBGoldGift` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '充值赠送元宝 = 每次充值时实际获得的元宝 - 玩家当次充值的人民币（元） * 10，即每次充值时赠予的额外元宝' after `RMBGold`;
		end if;

		if exists(select 1 from information_schema.columns where table_schema = @g_cur_db and table_name = 'players' and column_name = 'GmGold') then
			select 1 as 检测到玩家表的GmGold字段已存在，不必再添加;
		else
			-- 在<充值赠送元宝>字段后添加<系统赠送元宝>字段
			ALTER TABLE `players` ADD column `GmGold` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '系统赠送元宝 = 通过GM方式给玩家添加的充值元宝（GM可以给玩家添加充值元宝和普通元宝）' after `RMBGoldGift`;
		end if;
	else
		select 1 as 错误：找不到玩家表;
	end if;
END
$$

delimiter ;

call AddColumn();
DROP PROCEDURE if exists AddColumn;


delimiter $$
# 拆出新的元宝统计表：玩家的元宝消费记录将按月来进行拆分
DROP PROCEDURE if exists SplitNewGoldRecordTable;

# 传入的参数v_NewTableName将是goldrecord_和年月的组合，如: goldrecord_2014_3
CREATE DEFINER=`seruser`@`%` PROCEDURE `SplitNewGoldRecordTable`(IN v_NewTableName varchar(64))
    SQL SECURITY INVOKER
lable_1:
begin	
	# 根据传入的表名来创建新表
	SET @v_Sql = concat(
	"create table if not exists ", v_NewTableName, "("
		"ID int(10) unsigned not null auto_increment,"
		"PlayerID int(10) unsigned not null,"
		"PlayerLevel smallint(5) unsigned not null,"
		"VipLevel smallint(6) not null,"
		"ModuleType smallint(5) unsigned not null comment '元宝增加或减少类型，详见logdmoduletype表',"
		"OpType smallint(5) unsigned not null comment '获得或者消耗元宝的方式，详见moduletablename表',"
		"OpTime datetime not null comment '操作时间',"
		"OpRMBGold int(10) unsigned not null comment '该次操作有多少来自于充值元宝',"
		"OpRMBGoldGift int(10) unsigned not null comment '该次操作有多少来自于充值赠送元宝',"
		"OpGmGold int(10) unsigned not null comment '该次操作有多少来自于系统赠送元宝',"
		"OpGameGold int(10) unsigned not null comment '该次操作有多少来自于游戏内元宝',"
		"AfterRMBGold bigint(20) unsigned not null comment '之后的充值元宝数',"
		"AfterRMBGoldGift bigint(20) unsigned not null comment '之后的充值赠送元宝数',"
		"AfterGmGold bigint(20) unsigned not null comment '之后的系统赠送元宝数',"
		"AfterGameGold bigint(20) unsigned not null comment '之后的游戏元宝数',"
		"Param1 bigint(20) unsigned not null,"
		"Param2 bigint(20) unsigned not null,"
		"Param3 bigint(20) unsigned not null,"
		"Param4 bigint(20) unsigned not null,"
		"`comment` varchar(128) not null default '',"
		"primary key (`ID`)"
	") engine=innodb default charset=utf8;")
	;

	prepare stml from @v_Sql;
	EXECUTE stml;
end
$$
delimiter ;