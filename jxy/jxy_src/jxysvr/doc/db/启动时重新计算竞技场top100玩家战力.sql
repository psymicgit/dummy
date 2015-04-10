/*
    服务器启动时重新计算竞技场top100玩家的战力，与数据库表中的旧战力值做比对，如果战力前后不一致，打印战力对比列表，并将最新战力回写到数据库
   （有这个需求是为了检测新服务器上线时战力是否异常，以及以前经常出现的问题：当应用新的战力计算公式时，有玩家反馈竞技场战力更新不及时

    Date: 2013-12-19 09:57:29
*/

-- ----------------------------
-- 添加字段：启动时重新计算多少个竞技场玩家的战力并比对（一般是前100个，如果是0个则表示不重新计算）
-- 添加字段：是否把战力回写到数据库：是1/否0
-- ----------------------------
use ios_app_cfg;

DROP PROCEDURE if exists AddCloumn;
CREATE PROCEDURE AddCloumn ()
BEGIN
	-- 先把原来的字段删掉
	if exists(select column_name from INFORMATION_SCHEMA.columns where table_name = 'singlepram' and column_name = 'ReCalcHowManyTopRacer' ) then
        ALTER TABLE singlepram DROP COLUMN ReCalcHowManyTopRacer;
        ALTER TABLE singlepram DROP COLUMN IsSaveNewRacerPowerToDB;
	end if;

	-- 然后再直接新添加字段
	ALTER TABLE singlepram ADD column ReCalcHowManyTopRacer   smallint(6) NOT NULL DEFAULT '0' COMMENT '启动时重新计算多少个竞技场玩家的战力并比对（一般是前100个，如果是0个则表示不重新计算）';
  	ALTER TABLE singlepram ADD column IsSaveNewRacerPowerToDB smallint(6) NOT NULL DEFAULT '0' COMMENT '是否把战力回写到数据库：是1/否0';
END;

call AddCloumn();
DROP PROCEDURE if exists AddCloumn;