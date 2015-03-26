DROP PROCEDURE IF EXISTS `get_conflict`;
DROP PROCEDURE IF EXISTS `add_idx`;

DELIMITER $$
CREATE DEFINER=`root`@`%` PROCEDURE `get_conflict`(IN  db1 varchar(64), IN db1_name varchar(64), IN db2 varchar(64), IN db2_name varchar(64))
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

	# 首先在db2的门派和玩家的名称字段上建立索引（这个对性能非常重要）
	call add_idx(db1, 'players', 'DispName', 'player_name_idx');
	call add_idx(db2, 'players', 'DispName', 'player_name_idx');
	call add_idx(db1, 'faction', 'FactionName', 'faction_name_idx');
	call add_idx(db2, 'faction', 'FactionName', 'faction_name_idx');


	set @mycnt = 0;

	drop table if exists merge_s005.`conflict_player`;
	create table merge_s005.`conflict_player` as  
		select (@mycnt := @mycnt + 1) as rowid, p1.PlayerID, p1.DispName, substring_index(p1.DispName,'#',1) as RawDispName, 0 as isrepeat from merge_s005.players p1;

	insert into merge_s005.conflict_player select (@mycnt := @mycnt + 1) as rowid, p1.PlayerID, p1.DispName, substring_index(p1.DispName,'#',1) as RawDispName, 0 as isrepeat from merge_s006.players p1;

	call add_idx('merge_s005', 'conflict_player', 'RawDispName', 'DispNameIdx');

	update conflict_player c1 inner join conflict_player c2 on c1.RawDispName = c2.RawDispName and c1.PlayerID <> c2.PlayerID and c1.RawDispName is not null and c1.RawDispName <> '' and binary(c1.RawDispName) = binary(c2.RawDispName) set c1.isrepeat = 1 , c2.isrepeat = 1;
	delete from conflict_player where isrepeat = 0;

	set @mycnt = 0;
	update conflict_player set rowid = (@mycnt := @mycnt + 1) order by RawDispName;

	update conflict_player c1 inner join (select min(rowid) as rowid, PlayerID, DispName from `conflict_player` c2 group by DispName) c2 on c1.DispName = c2.DispName set c1.rowid = c1.rowid - c2.rowid;
	update conflict_player set  RawDispName = concat(RawDispName, '#');
	update conflict_player set  RawDispName = substring_index(RawDispName,'#',1);

	select * from merge_s005.`conflict_player`;

	drop table if exists merge_s005.`conflict_player`;
END
$$

create procedure add_idx(db varchar(64), IN p_table varchar(200), IN p_column varchar(200), IN p_idxname VARCHAR(200))  
begin  
	declare v_sqlstr varchar(1024) default '';
    
	set 		@cnt = 0;
	set v_sqlstr = concat("select count(*) into @cnt from information_schema.statistics where table_schema = '", db, "' and table_name = '", p_table, "' and index_name = '", p_idxname, "';");
	call exec_sql(v_sqlstr);

	if @cnt = 0 then   
		set v_sqlstr = concat("CREATE INDEX ", p_idxname, " ON ", db, ".", p_table, "(", p_column, ");");
		call exec_sql(v_sqlstr);
	end if;
end
$$

delimiter ;


call get_conflict('merge_s005', '囧5区', 'merge_s006', '囧6区');