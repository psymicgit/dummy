/*
    合区脚本
    Date: 2014-1-20 15:21:22
    @author by 翁沿海
*/

# 创建合区的存储过程
DROP PROCEDURE IF EXISTS `union_databases`;
DROP PROCEDURE IF EXISTS `union_table`;
DROP PROCEDURE IF EXISTS `union_table_check`;
DROP PROCEDURE IF EXISTS `union_table_delrebolt`;
DROP PROCEDURE IF EXISTS `exec_sql`;
DROP PROCEDURE IF EXISTS `exec_sql_print`;

DELIMITER $$
# 合并2个数据库
# 比如union_databases('ios_app_s003', '囧3区', 'ios_app_s004', '囧4区')
# 参数db1:	    数据库1，如ios_app_s001
# 参数db1_desc: 对数据库1的描述，如囧1区
# 参数db2:		数据库2，如ios_app_s002
# 参数db2_desc: 对数据库2的描述，如囧2区
CREATE DEFINER=`root`@`%` PROCEDURE `union_databases`(IN  db1 varchar(64), IN db1_desc varchar(64), IN db2 varchar(64), IN db2_desc varchar(64))
lable_1:
BEGIN
	declare v_sqlstr varchar(1024) default ''; 

	IF db1 is null or db2 is null or db1_desc is null or db2_desc is null then
		select '错误' as '不允许传入空的参数';
		leave lable_1; 
	END IF;

	IF LENGTH(trim(db1))<1 or LENGTH(trim(db2))<1 or LENGTH(trim(db1_desc))<1 or LENGTH(trim(db2_desc))<1 then
		select '错误' as '不允许传入空字符串';
		leave lable_1; 
	END IF;

	#开启事务
	start transaction; 

    #
	#ALTER TABLE `faction`
	#ADD COLUMN `Ext`  smallint NULL DEFAULT 0 COMMENT '是否可修改名称' AFTER `LastAuditTime`;

	#检测数据
	call union_table_check(db1, db2, 'faction', 'FactionID');
	IF  @g_int >= 1  THEN
					select '帮派Id重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	call union_table_check(db1, db2, 'faction', 'FactionPlayerID');
	IF  @g_int >= 1  THEN
					select '帮派玩家重复';
					rollback;
					leave lable_1; 
	END IF;

	call union_table_check(db1, db2, 'myrecharge', 'OrderID');
	IF  @g_int >= 1  THEN
					select '充值订单重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_bags', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_bags', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家背包重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;
	
	#删除
	call union_table_delrebolt(db1, 'player_builds', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_builds', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家建筑重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#检测
	call union_table_check(db1, db2, 'player_cdkey', 'CdKey');
	IF  @g_int >= 1  THEN
					select '玩家CDKEY重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	call union_table_check(db1, db2, 'player_consume', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家消费记录重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_ext', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_ext', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家扩展信息重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_hero_ext', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_hero_ext', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家武将扩展信息重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_heros', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_heros', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家武将信息重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_instances', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_instances', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家副本信息重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_pvp', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_pvp', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家PVP信息重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_recharge', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_recharge', 'OrderID');
	IF  @g_int >= 1  THEN
					select '玩家充值记录重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_relation', 'pidA');
	#检测
	call union_table_check(db1, db2, 'player_relation', 'pidA');
	IF  @g_int >= 1  THEN
					select '玩家关系重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'player_tasks', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'player_tasks', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家任务重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'players', 'PlayerID');
	#检测
	call union_table_check(db1, db2, 'players', 'PlayerID');
	IF  @g_int >= 1  THEN
					select '玩家信息重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'private_chat_log', 'sendPid');
	#检测
	call union_table_check(db1, db2, 'private_chat_log', 'sendPid');
	IF  @g_int >= 1  THEN
					select '玩家聊天重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'send_flower_log', 'sendPid');
	#检测
	call union_table_check(db1, db2, 'send_flower_log', 'sendPid');
	IF  @g_int >= 1  THEN
					select '玩家送花记录重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	#删除
	call union_table_delrebolt(db1, 'send_flower_log', 'sendPid');
	#检测
	call union_table_check(db1, db2, 'send_flower_log', 'sendPid');
	IF  @g_int >= 1  THEN
					select '玩家送花记录重复' as '模块信息', @g_int as '个数';
					rollback;
					leave lable_1; 
	END IF;

	###################################################
	##检测帮派是否重名
	set 		v_sqlstr = concat('select f1.FactionName as 名称发生冲突的门派, CONCAT(f1.FactionName, \'(', db1_desc, ')\') as ', db1_desc, '改名后的门派名称, CONCAT(f2.FactionName, \'(', db2_desc, ')\') as ', db2_desc, '改名后的门派名称 from ', db1, '.faction f1 inner join ', db2, '.faction f2 on f1.FactionName = f2.FactionName and f1.FactionName is not null and f1.FactionName <> \'\'');
	call exec_sql_print(v_sqlstr);

    set 		v_sqlstr = concat('UPDATE ', db1, '.faction f1 inner join ', db2, '.faction f2 on f1.FactionName = f2.FactionName and f1.FactionName is not null and f1.FactionName <> \'\' set f1.FactionName = CONCAT(f1.FactionName, \'(', db1_desc, ')\'), f1.Ext = 1, f2.FactionName = CONCAT(f2.FactionName, \'(', db2_desc, ')\'), f2.Ext = 1;');
	call exec_sql_print(v_sqlstr);

	##检测玩家昵称是否重名
	set 		v_sqlstr = concat('select p1.DispName as 昵称发生冲突的玩家, CONCAT(p1.DispName, \'(', db1_desc, ')\') as ', db1_desc, '改名后的玩家昵称, CONCAT(p2.DispName, \'(', db2_desc, ')\') as ', db2_desc, '改名后的玩家昵称 from ', db1, '.players p1 inner join ', db2, '.players p2 on p1.DispName = p2.DispName and p1.DispName is not null and p1.DispName <> \'\'');
	call exec_sql_print(v_sqlstr);

	set 		v_sqlstr = concat('UPDATE ', db1, '.players p1 inner join ', db2, '.players p2 on p1.DispName = p2.DispName and p1.DispName is not null and p1.DispName <> \'\' set p1.DispName = CONCAT(p1.DispName, \'(', db1_desc, ')\'), p1.ExtData = 3, p2.DispName = CONCAT(p2.DispName, \'(', db2_desc, ')\'), p2.ExtData = 3');
	call exec_sql_print(v_sqlstr);

	###################################################
	#数据并表
	call union_table(db1, db2, 'faction');
	call union_table(db1, db2, 'factionplayer');

	call union_table(db1, db2, 'myrecharge');
	call union_table(db1, db2, 'player_bags');

	call union_table(db1, db2, 'player_builds');
	call union_table(db1, db2, 'player_cdkey');

	call union_table(db1, db2, 'player_ext');
	call union_table(db1, db2, 'player_hero_ext');

	call union_table(db1, db2, 'player_heros');
	call union_table(db1, db2, 'player_instances');

	call union_table(db1, db2, 'player_pvp');
	call union_table(db1, db2, 'player_recharge');

	call union_table(db1, db2, 'player_relation');
	call union_table(db1, db2, 'player_tasks');

	call union_table(db1, db2, 'players');
	call union_table(db1, db2, 'private_chat_log');

	call union_table(db1, db2, 'send_flower_log');

	## 删除临时表
	set 		v_sqlstr = concat('drop table if exists ', db1, '.`conflict_faction`');
	call exec_sql_print(v_sqlstr);
	set 		v_sqlstr = concat('drop table if exists ', db1, '.`conflict_player`');
	call exec_sql_print(v_sqlstr);

	## 建立冲突门派临时表，存储门派表内的门派名冲突记录，用于之后的检测（用子查询的效率太慢太慢了）
	set 		v_sqlstr = concat('create table ', db1, '.`conflict_faction` as (select FactionName from ', db1, '.Faction group by FactionName having count(FactionName) > 1)');
	call exec_sql_print(v_sqlstr);

	## 建立冲突玩家临时表，存储玩家表内的昵称冲突记录
	set 		v_sqlstr = concat('create table ', db1, '.`conflict_player` as (select DispName from ', db1, '.players group by DispName having count(DispName) > 1)');
	call exec_sql_print(v_sqlstr);

    ## 再次检测帮派名称，如果还是重名，则在帮派名称后加上帮派ID
    set 		v_sqlstr = concat('update ', db1, '.faction f inner join ', db1, '.conflict_faction c on f.FactionName = c.FactionName and f.FactionName is not null and f.FactionName <> \'\' set f.FactionName = CONCAT(f.FactionName, \'(\', f.FactionID, \')\'), f.Ext = 1');
	call exec_sql_print(v_sqlstr);

    ## 再次检测玩家昵称，如果还是重名，则在玩家昵称后加上玩家ID
    set 		v_sqlstr = concat('update ', db1, '.players p inner join ', db1, '.conflict_player c on p.DispName = c.DispName and p.DispName is not null and p.DispName <> \'\' set p.DispName = CONCAT(p.DispName, \'(\', p.PlayerID, \')\'), p.ExtData = 3');
	call exec_sql_print(v_sqlstr);

	## 删除创建的临时表
	set 		v_sqlstr = concat('drop table if exists ', db1, '.`conflict_faction`');
	call exec_sql_print(v_sqlstr);
	set 		v_sqlstr = concat('drop table if exists ', db1, '.`conflict_player`');
	call exec_sql_print(v_sqlstr);

	commit; 
END
$$

CREATE DEFINER=`root`@`%` PROCEDURE `union_table`(`db1` varchar(64),`db2` varchar(64), `mytable` varchar(64))
BEGIN
	declare v_sqlstr  varchar(1024) default ''; 

	#Routine body goes here...
	set 		v_sqlstr = concat('INSERT into ', db1, '.', mytable, ' select * from ', db2, '.', mytable, ';');
	call exec_sql_print(v_sqlstr);
END
$$

#检测两表间是否有重复的字段
CREATE DEFINER=`root`@`%` PROCEDURE `union_table_check`(`db1` varchar(64),`db2` varchar(64),`mytable` varchar(64),`mycolumn` varchar(64))
BEGIN
	declare v_sqlstr  varchar(1024) default ''; 

	set 		@g_int = 0;
	set 		v_sqlstr = concat('select count(1) into @g_int from ',  db1, '.', mytable,' t1 inner join ', db2, '.', mytable, ' t2 on t1.', mycolumn, '= t2.', mycolumn, ';');
	call exec_sql(v_sqlstr);

	# 打印出冲突的id
	IF  @g_int >= 1  THEN
		set 		v_sqlstr = concat('select \'', mytable, '\' as 发生冲突的表, \'', mycolumn, '\' as 发生冲突的字段, t1.', mycolumn, ' as 值 from ', db1, '.', mytable,' t1 inner join ', db2, '.', mytable, ' t2 on t1.', mycolumn, '= t2.', mycolumn, ';');
		call exec_sql_print(v_sqlstr);
	END IF;
END
$$

#删除机器人
CREATE DEFINER=`root`@`%` PROCEDURE `union_table_delrebolt`(`db1` varchar(64), `mytable` varchar(64),`mycolumn` varchar(64))
BEGIN
	declare v_sqlstr  varchar(1024) default ''; 
	set 		v_sqlstr = concat('delete from ', db1, '.', mytable, ' where ', mycolumn, ' <= 1010');
	call exec_sql(v_sqlstr);
END
$$

#执行sql语句
CREATE DEFINER=`root`@`%` PROCEDURE `exec_sql`(`v_sqlstr` varchar(1024))
BEGIN
	set 		@sqlstr = v_sqlstr;
	prepare m_str from @sqlstr;
	execute m_str;
END
$$

#执行sql语句并打印
CREATE DEFINER=`root`@`%` PROCEDURE `exec_sql_print`(`v_sqlstr` varchar(1024))
BEGIN
	select v_sqlstr;
	call exec_sql(v_sqlstr);
END
$$

# 在门派表中添加标识字段
DROP PROCEDURE if exists AddCloumn;
CREATE PROCEDURE AddCloumn ()
BEGIN
	declare v_sqlstr  varchar(1024) default ''; 
	set 		@g_cur_db = '';
	set 		v_sqlstr = concat('select database() into @g_cur_db');
	call exec_sql(v_sqlstr);

	-- 检测原来的字段是否存在，如不存在则添加字段
	if exists(select 1 from information_schema.columns where table_schema = @g_cur_db and table_name = 'faction' and column_name = 'Ext') then
		select 1 as 门派表的Ext字段已存在;
	else
		-- 然后再直接新添加字段
		ALTER TABLE `faction` ADD column `Ext` smallint(6) NOT NULL DEFAULT '0' COMMENT '门派是否需要重新注册门派名：1是/0否';
	end if;
END
$$

DELIMITER ;
call AddCloumn();
DROP PROCEDURE if exists AddCloumn;