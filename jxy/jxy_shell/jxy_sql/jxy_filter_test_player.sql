create table if not exists mysql.test_player (
	PlayerID int(11) unsigned not null comment '角色ID',
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

delimiter $$
DROP PROCEDURE if exists FilterTestPlayers;
CREATE PROCEDURE FilterTestPlayers()
BEGIN
	DECLARE v_done INT DEFAULT 0;
	DECLARE v_tablescheme VARCHAR(128);
	DECLARE oprecord_tablename VARCHAR(128);
	declare v_sqlstr  varchar(1024) default ''; 

	DECLARE v_totaladdgold BIGINT unsigned DEFAULT 0;
	DECLARE v_curaddgold BIGINT unsigned DEFAULT 0;

	DECLARE v_totaldecgold BIGINT unsigned DEFAULT 0;
	DECLARE v_curdecgold BIGINT unsigned DEFAULT 0;

	DECLARE OPRECORD_CURSOR CURSOR for select table_schema, table_name from information_schema.columns where table_schema in (select database()) and table_name like 'oprecord_20%' group by table_schema, table_name;
	
	declare continue handler for not found SET v_done=1;

	set 		@g_cur_db = '';
	set 		v_sqlstr = concat('select database() into @g_cur_db');

	set 		@sqlstr = v_sqlstr;
	prepare m_str from @sqlstr;
	execute m_str;

	set 		@g_cur_add_gold = 0;
	set 		@g_cur_dec_gold = 0;

	OPEN oprecord_cursor;  
		while v_done <> 1 do
			fetch  oprecord_cursor into v_tablescheme, oprecord_tablename;

			if v_tablescheme = @g_cur_db then
				set 		v_sqlstr = concat('insert ignore into mysql.test_player(PlayerID) select PlayerID from ', oprecord_tablename, ' op where ModuleType=12 and (OpType = 3 or OpType = 14) and OpValue > 5000');
				set 		@sqlstr = v_sqlstr;
				prepare m_str from @sqlstr;
				execute m_str;
			end if;
		end while;  
	 CLOSE oprecord_cursor ;
END
$$

delimiter ;
call FilterTestPlayers();

drop table if exists mysql.test_player;