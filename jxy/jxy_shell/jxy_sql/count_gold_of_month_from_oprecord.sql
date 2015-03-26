delimiter $$
DROP PROCEDURE if exists QueryGold;
CREATE PROCEDURE QueryGold()
BEGIN
	DECLARE v_done INT DEFAULT 0;
	DECLARE v_tablescheme VARCHAR(128);
	DECLARE oprecord_tablename VARCHAR(128);
	declare v_sqlstr  varchar(1024) default ''; 

	DECLARE v_totaladdgold BIGINT unsigned DEFAULT 0;
	DECLARE v_curaddgold BIGINT unsigned DEFAULT 0;

	DECLARE v_totaldecgold BIGINT unsigned DEFAULT 0;
	DECLARE v_curdecgold BIGINT unsigned DEFAULT 0;

	DECLARE OPRECORD_CURSOR CURSOR for select table_schema, table_name from information_schema.columns where table_schema in (select database()) and table_name like 'oprecord_201402%' group by table_schema, table_name;
	
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
				set 		v_sqlstr = concat('select sum(OpValue) into @g_cur_add_gold from ', oprecord_tablename, ' op where ModuleType=12 and OpTime >= "2014-02-01 00:00:00" and OpTime <= "2014-03-01 00:00:00" and PlayerID in (select PlayerID from exchange.exchange)');
				set 		@sqlstr = v_sqlstr;
				prepare m_str from @sqlstr;
				execute m_str;

				set v_totaladdgold = v_totaladdgold + @g_cur_add_gold;

				set 		v_sqlstr = concat('select sum(OpValue) into @g_cur_dec_gold from ', oprecord_tablename, ' op where ModuleType=13 and OpTime >= "2014-02-01 00:00:00" and OpTime <= "2014-03-01 00:00:00" and PlayerID in (select PlayerID from exchange.exchange)');
				set 		@sqlstr = v_sqlstr;
				prepare m_str from @sqlstr;
				execute m_str;

				set v_totaldecgold = v_totaldecgold + @g_cur_dec_gold;
			end if;
			

		end while;  
	 CLOSE oprecord_cursor ;

	 select v_totaladdgold as totaladdgold, v_totaldecgold as totaldecgold;
END
$$

delimiter ;
call QueryGold();