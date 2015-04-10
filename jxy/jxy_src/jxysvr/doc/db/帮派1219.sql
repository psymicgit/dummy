ALTER TABLE `faction`
ADD COLUMN `UpdateTime`  datetime NOT NULL DEFAULT '2013-12-19 00:00:00' AFTER `FactionFundsSum`;

drop procedure if exists sp_AddRecharge;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_AddRecharge`(IN v_OrderID varchar(42),IN v_PlayerID int, IN v_Gold int, IN v_RMB int, IN v_SandBoxFlag int, v_Comment varchar(256))
    SQL SECURITY INVOKER
lable_1:
BEGIN   
		declare v_ErrCode int default 0;   
		declare v_ErrStr varchar(128) default  'Success!';  

		declare exit handler for sqlexception
		BEGIN
				set v_ErrCode = 3;
				set v_ErrStr = 'mysql error!';
				select v_ErrCode as ErrCode, v_ErrStr as ErrStr;
				rollback;
		END;

		set v_ErrCode = 0;
		set v_ErrStr = 'OrderID is OK!';
		#开启事物
		start transaction; 



		-- 检测订单是否存在
		IF  EXISTS ( select 1 from `player_recharge` where OrderID = v_OrderID )THEN
					set v_ErrCode = 2;
					set v_ErrStr = 'OrderID is Processed!';
					select v_ErrCode as ErrCode, v_ErrStr as ErrStr;
					rollback;
					leave lable_1; 
		END IF;
		# 检测玩家是否存在
		IF NOT EXISTS ( select 1 from `players` where PlayerID = v_PlayerID )THEN
					set v_ErrCode = 13;
					set v_ErrStr = 'PlayerID is Not Exit!';
					select v_ErrCode as ErrCode, v_ErrStr as ErrStr;
					rollback;
					leave lable_1; 
		END IF;

		# 直接更新玩家的数据，因为玩家的数据更新不与充值在同一个事物中。有可能存在失败的风险，保证玩家的元宝数在这次事物提交中成功
		#UPDATE players set Pay = Pay + v_Gold, Gold = Gold + v_Gold, TotalRMB = TotalRMB + v_RMB WHERE PlayerID = v_PlayerID;
		# 在订单表中插入订单记录。
		INSERT INTO player_recharge(OrderID, PlayerID, Gold, RMB, SandBoxFlag, Comment, CreateTime) VALUES (v_OrderID, v_PlayerID, v_Gold, v_RMB, v_SandBoxFlag, v_Comment, now());
		#结束事物
		select v_ErrCode as ErrCode, v_ErrStr as ErrStr;
		commit; 
END