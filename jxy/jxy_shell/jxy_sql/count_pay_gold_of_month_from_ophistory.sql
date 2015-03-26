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

	select sum(OpTotal) into @g_cur_add_gold from ophistory where ModuleType=12 and OpType =1 and Date >= '2014-02-01 00:00:00' and Date <= '2014-03-01 00:00:00';
	select sum(OpTotal) into @g_cur_dec_gold from ophistory where ModuleType=13 and Date >= '2014-02-01 00:00:00' and Date <= '2014-03-01 00:00:00';

	set v_totaladdgold = @g_cur_add_gold;
	set v_totaldecgold = @g_cur_dec_gold;

	select v_totaladdgold as 充值+充值赠送元宝, v_totaldecgold as 消耗元宝;
END
$$

delimiter ;
call QueryGold();