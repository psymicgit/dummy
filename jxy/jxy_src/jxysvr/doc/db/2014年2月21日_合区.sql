/*
    合区脚本
    Date: 2014-1-20 15:21:22
    @author by 翁延海
*/

# 创建合区的存储过程
DROP PROCEDURE IF EXISTS `union_databases`;
DROP PROCEDURE IF EXISTS `union_table`;
DROP PROCEDURE IF EXISTS `union_table_check`;
DROP PROCEDURE IF EXISTS `union_table_delrebolt`;
DROP PROCEDURE IF EXISTS `exec_sql`;
DROP PROCEDURE IF EXISTS `exec_sql_print`;
DROP PROCEDURE IF EXISTS `del_idx`;
DROP PROCEDURE IF EXISTS `add_idx`;

DELIMITER $$
# 合并2个数据库
# 比如union_databases('ios_app_s003', '囧3区', 'ios_app_s004', '囧4区')
# 参数db1:	    数据库1，如ios_app_s001
# 参数db1_desc: 对数据库1的描述，如囧1区
# 参数db2:		数据库2，如ios_app_s002
# 参数db2_desc: 对数据库2的描述，如囧2区
CREATE DEFINER=`root`@`%` PROCEDURE `union_databases`(IN  db1 varchar(64), IN db1_name varchar(64), IN db2 varchar(64), IN db2_name varchar(64))
lable_1:
BEGIN
	declare v_sqlstr varchar(1024) default ''; 

	declare db1_desc varchar(128) default ''; 
	declare db2_desc varchar(128) default '';

	set db1_desc = '1';
	set db2_desc = '2';

	IF db1 is null or db2 is null or db1_name is null or db2_name is null or db1_desc is null or db2_desc is null then
		select '错误' as '不允许传入空的参数';
		leave lable_1; 
	END IF;

	IF LENGTH(trim(db1))<1 or LENGTH(trim(db2))<1 or LENGTH(trim(db1_name))<1 or LENGTH(trim(db2_name))<1 or LENGTH(trim(db1_desc))<1 or LENGTH(trim(db2_desc))<1 then
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

	call union_table_delrebolt(db1, 'player_consume', 'PlayerID');
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

	# 首先在db2的门派和玩家的名称字段上建立索引（这个对性能非常重要）
	call add_idx(db1, 'players', 'DispName', 'player_name_idx');
	call add_idx(db2, 'players', 'DispName', 'player_name_idx');
	call add_idx(db1, 'faction', 'FactionName', 'faction_name_idx');
	call add_idx(db2, 'faction', 'FactionName', 'faction_name_idx');

	# 2. 打印出冲突的门派和玩家名，以及对应的id
	set @rowid = 0;
	set v_sqlstr = concat("select (@rowid := @rowid + 1) as 行数, f1.FactionName as 名称冲突的门派, CONCAT(f1.FactionName, '#", db1_desc, "') as ", db1_name, "改名后的门派, f1.FactionID as ", db1_name, "冲突门派ID, CONCAT(f2.FactionName, '#", db2_desc, "') as ", db2_name, "改名后的门派, f2.FactionID as ", db2_name, "冲突门派ID from ", db1, ".faction f1 inner join ", db2, ".faction f2 on f1.FactionName = f2.FactionName and f1.FactionName is not null and f1.FactionName <> '' and binary(f1.FactionName) = binary(f2.FactionName);");
	call exec_sql_print(v_sqlstr);

	set @rowid = 0;
	set v_sqlstr = concat("select (@rowid := @rowid + 1) as 行数, p1.DispName as 昵称冲突的玩家, CONCAT(p1.DispName, '#", db1_desc, "') as ", db1_name, "改名后的玩家, p1.PlayerID as ", db1_name, "冲突玩家ID, CONCAT(p2.DispName, '#", db2_desc, "') as ", db2_name, "改名后的玩家, p2.PlayerID as ", db2_name, "冲突玩家ID from ", db1, ".players p1 inner join ", db2, ".players p2 on p1.DispName = p2.DispName and p1.DispName is not null and p1.DispName <> '' and binary(p1.DispName) = binary(p2.DispName);");
	call exec_sql_print(v_sqlstr);

	set 		v_sqlstr = concat("UPDATE ", db1, ".faction f1 inner join ", db2, ".faction f2 on f1.FactionName = f2.FactionName and f1.FactionName is not null and f1.FactionName <> '' and binary(f1.FactionName) = binary(f2.FactionName) set f1.FactionName = CONCAT(f1.FactionName, '#", db1_desc, "'), f1.Ext = 1, f2.FactionName = CONCAT(f2.FactionName, '#", db2_desc, "'), f2.Ext = 1;");
	call exec_sql_print(v_sqlstr);

	set 		v_sqlstr = concat("UPDATE ", db1, ".players p1 inner join ", db2, ".players p2 on p1.DispName = p2.DispName and p1.DispName is not null and p1.DispName <> '' and binary(p1.DispName) = binary(p2.DispName) set p1.DispName = CONCAT(p1.DispName, '#", db1_desc, "'), p1.ExtData = 3, p2.DispName = CONCAT(p2.DispName, '#", db2_desc, "'), p2.ExtData = 3");
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
	commit; 

	###################################################
	# 最后一步：检测门派表和玩家表中是否仍然存在名称冲突的记录

	## 打印出冲突门派和冲突玩家列表
	set @rowid = 0;
	set v_sqlstr = concat("select (@rowid := @rowid + 1) as 行数, f1.FactionName as 冲突的门派, CONCAT(f1.FactionName, '(', f1.FactionID, ')') as 改名后的门派1, f1.FactionID as 门派1的ID, CONCAT(f2.FactionName, '(', f2.FactionID, ')') as 改名后的门派2, f2.FactionID as 门派2的ID from ", db1, ".faction f1 inner join ", db1, ".faction f2 on f1.FactionName = f2.FactionName and f1.FactionID < f2.FactionID and f1.FactionName is not null and f1.FactionName <> '' and binary(f1.FactionName) = binary(f2.FactionName)");
	call exec_sql_print(v_sqlstr);

	set @rowid = 0;
	set v_sqlstr = concat("select (@rowid := @rowid + 1) as 行数, p1.DispName as 冲突的玩家, CONCAT(p1.DispName, '(', p1.PlayerID, ')') as 改名后的玩家1, p1.PlayerID as 玩家1的ID, CONCAT(p2.DispName, '(', p2.PlayerID, ')') as 改名后的玩家2, p2.PlayerID as 玩家2的ID from ", db1, ".players p1 inner join ", db1, ".players p2 on p1.DispName = p2.DispName and p1.PlayerID < p2.PlayerID and p1.DispName is not null and p1.DispName <> '' and binary(p1.DispName) = binary(p2.DispName and p1.PlayerID > 1010)") ;
	call exec_sql_print(v_sqlstr);

    ## 在冲突门派和冲突玩家的名称后加上"(ID)"作为后缀
	set v_sqlstr = concat("update ", db1, ".faction f1 inner join ", db1, ".faction f2 on f1.FactionName = f2.FactionName and f1.FactionID < f2.FactionID and f1.FactionName is not null and f1.FactionName <> '' and binary(f1.FactionName) = binary(f2.FactionName) set f1.FactionName = CONCAT(f1.FactionName, '(', f1.FactionID, ')'), f2.FactionName = CONCAT(f2.FactionName, '(', f2.FactionID, ')')");
	call exec_sql_print(v_sqlstr);

	set @rowid = 0;
	set v_sqlstr = concat("update ", db1, ".players p1 inner join ", db1, ".players p2 on p1.DispName = p2.DispName and p1.PlayerID < p2.PlayerID and p1.DispName is not null and p1.DispName <> '' and binary(p1.DispName) = binary(p2.DispName) and p1.PlayerID > 1010 set p1.DispName = CONCAT(p1.DispName, '(', p1.PlayerID, ')'), p2.DispName = CONCAT(p2.DispName, '(', p2.PlayerID, ')') ");
	call exec_sql_print(v_sqlstr);

	# 删除刚刚建的索引

	call del_idx(db1, 'players', 'player_name_idx');
	call del_idx(db2, 'players', 'player_name_idx');
	call del_idx(db1, 'faction', 'faction_name_idx');
	call del_idx(db2, 'faction', 'faction_name_idx');
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
		set 		v_sqlstr = concat("select '", mytable, "' as 发生冲突的表, '", mycolumn, "' as 发生冲突的字段, t1.", mycolumn, " as 值 from ", db1, ".", mytable, " t1 inner join ", db2, ".", mytable, " t2 on t1.", mycolumn, "= t2.", mycolumn, ";");
		call exec_sql_print(v_sqlstr);
	END IF;
END
$$

#删除机器人
CREATE DEFINER=`root`@`%` PROCEDURE `union_table_delrebolt`(`db1` varchar(64), `mytable` varchar(64),`mycolumn` varchar(64))
BEGIN
	declare v_sqlstr  varchar(1024) default ''; 
	set 		v_sqlstr = concat('delete from ', db1, '.', mytable, ' where ', mycolumn, ' <= 1010');
	call exec_sql_print(v_sqlstr);
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

create procedure del_idx(db varchar(64), IN p_table varchar(200), IN p_idxname VARCHAR(200))  
begin  
	declare v_sqlstr varchar(1024) default ''; 

	set 		@cnt = 0;
	set v_sqlstr = concat("select count(*) into @cnt from information_schema.statistics where table_schema = '", db, "' and table_name = '", p_table, "' and index_name = '", p_idxname, "';");
	call exec_sql(v_sqlstr);

	if @cnt >0 then   
		set v_sqlstr = concat('drop index ', p_idxname, ' on ', db, '.', p_table);
		call exec_sql(v_sqlstr);
	end if;
end
$$

create procedure add_idx(db varchar(64), IN p_table varchar(200), IN p_column varchar(200), IN p_idxname VARCHAR(200))  
begin  
	declare v_sqlstr varchar(1024) default '';
    call del_idx(db, p_table, p_idxname);

	set v_sqlstr = concat("CREATE INDEX ", p_idxname, " ON ", db, ".", p_table, "(", p_column, ");");
	call exec_sql(v_sqlstr);
end
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

use ios_app_cfg;
REPLACE INTO `msgdefprop` VALUES ('0', 'YOU_NEED_REEDIT_YOUR_NAME', '因合区原因，您的昵称与别的玩家发生了冲突，请重新取名', '');
