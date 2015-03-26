create table if not exists `exchange` (
	`OrderID` VARCHAR(255) NOT NULL COMMENT '订单编号',
	`DeviceID` VARCHAR(255) NULL DEFAULT '',
	`UserID` INT(11) NULL DEFAULT NULL,
	`ZoneID` INT(11) NULL DEFAULT NULL COMMENT '区服',
	`Source` VARCHAR(45) NULL DEFAULT NULL COMMENT '数据来源',
	`Amount` INT(11) NULL DEFAULT NULL COMMENT '金额，单位为分',
	`RMB` INT(11) NULL DEFAULT '0',
	`Partner` VARCHAR(45) NULL DEFAULT NULL COMMENT '游戏编号',
	`PlayerID` INT(10) UNSIGNED NULL DEFAULT NULL COMMENT '玩家ID',
	`DateCreate` DATETIME NULL DEFAULT NULL COMMENT '订单创建日期',
	`DateExchange` DATETIME NULL DEFAULT NULL COMMENT '从平台商收到该订单数据的时间',
	`DateProcess` DATETIME NULL DEFAULT NULL COMMENT '数据被处理时间',
	`IsProcess` TINYINT(1) NULL DEFAULT '0' COMMENT '订单是否被处理',
	`PlayerLevel` INT(11) NULL DEFAULT NULL COMMENT '玩家等级',
	`PayBuyEvn` SMALLINT(6) NULL DEFAULT '0',
	PRIMARY KEY (`OrderID`),
	INDEX `IDTime` (`PlayerID`, `DateProcess`, `IsProcess`) USING BTREE,
	INDEX `IDCreateTime` (`PlayerID`, `DateCreate`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;


delimiter $$
DROP PROCEDURE if exists merge_exchange;
CREATE PROCEDURE merge_exchange()
BEGIN
	DECLARE v_done INT DEFAULT 0;
	DECLARE v_tablescheme VARCHAR(128);
	DECLARE v_tablename VARCHAR(128);
	declare v_sqlstr  varchar(1024) default ''; 

	declare pay_cursor cursor for select table_schema, table_name from information_schema.columns where table_schema in (select database()) and table_name like 'exchange_s%' group by table_schema, table_name;
	
	declare continue handler for not found SET v_done=1;

	open pay_cursor;  
		while v_done <> 1 do
			fetch  pay_cursor into v_tablescheme, v_tablename;

			set 		v_sqlstr = concat('insert ignore into exchange(OrderID,DeviceID,UserID,ZoneID,Source,Amount,RMB,Partner,PlayerID,DateCreate,DateExchange,DateProcess,IsProcess,PlayerLevel) select OrderID,DeviceID,UserID,ZoneID,Source,Amount,RMB,Partner,PlayerID,DateCreate,DateExchange,DateProcess,IsProcess,PlayerLevel from ', v_tablename, ' where IsProcess=1 and ZoneID < 255');
			set 		@sqlstr = v_sqlstr;
			prepare m_str from @sqlstr;
			execute m_str;

			set 		v_sqlstr = concat('select count(distinct(PlayerID)) as 充值玩家数, \'', v_tablename, '\' as 表名 from ', v_tablename);
			set 		@sqlstr = v_sqlstr;
			prepare m_str from @sqlstr;
			execute m_str;
		end while;  
	 close pay_cursor ;

	 replace into exchange select distinct(playerid) from ios_app_usercenter.payed;
END
$$

delimiter ;
call merge_exchange();
DROP PROCEDURE if exists merge_exchange;