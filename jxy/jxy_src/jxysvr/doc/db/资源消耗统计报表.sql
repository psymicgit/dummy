/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50614
Source Host           : localhost:3306
Source Database       : ios_app_s001_log

Target Server Type    : MYSQL
Target Server Version : 50614
File Encoding         : 65001

Date: 2013-12-10 09:57:29
*/

use ios_app_s001_log;

-- OpType大约算了一下，有276种
-- ----------------------------
-- Table structure for 每天玩家消费统计报表
-- ----------------------------
-- MaxHour` int(10) unsigned NOT NULL comment	     '当天操作最密集的3个小时段'
DROP TABLE IF EXISTS `ophistory`;
CREATE TABLE `ophistory` (
  `Date` datetime NOT NULL,
  `ModuleType` smallint(5) unsigned NOT NULL comment '资源类型',
  `OpType` smallint(5) unsigned NOT NULL comment     '资源操作类型',
  `OpTotal` bigint(20) unsigned NOT NULL comment	 '当天总的资源操作数额',
  `OpCnt` int(10) unsigned NOT NULL comment	         '玩家当天在该类资源上的总操作次数',
  `MaxOpVal` int(10) unsigned NOT NULL comment	     '玩家当天单笔最高操作，比如最高的一次得到了xxx元宝，消耗了xxx元宝',
  `MinOpVal` int(10) unsigned NOT NULL comment	     '玩家当天单笔最低操作，比如最少的一次得到了xxx元宝，消耗了xxx元宝',
  `vip0` bigint(20) unsigned NOT NULL default '0' comment 'vip0表示不是vip玩家',
  `vip1` bigint(20) unsigned NOT NULL default '0',
  `vip2` bigint(20) unsigned NOT NULL default '0',
  `vip3` bigint(20) unsigned NOT NULL default '0',
  `vip4` bigint(20) unsigned NOT NULL default '0',
  `vip5` bigint(20) unsigned NOT NULL default '0',
  `vip6` bigint(20) unsigned NOT NULL default '0',
  `vip7` bigint(20) unsigned NOT NULL default '0',
  `vip8` bigint(20) unsigned NOT NULL default '0',
  `vip9` bigint(20) unsigned NOT NULL default '0',
  `vip10` bigint(20) unsigned NOT NULL default '0',
  `vip11` bigint(20) unsigned NOT NULL default '0',
  `vip12` bigint(20) unsigned NOT NULL default '0' comment 'vip12级玩家消费汇总',
  `vip12+` bigint(20) unsigned NOT NULL default '0' comment 'vip12级以上玩家消费汇总',
  `level_1_9` bigint(20) unsigned NOT NULL default '0' comment '第一个十级区间的玩家消费汇总',
  `level_10` bigint(20) unsigned NOT NULL default '0',
  `level_20` bigint(20) unsigned NOT NULL default '0',
  `level_30` bigint(20) unsigned NOT NULL default '0',
  `level_40` bigint(20) unsigned NOT NULL default '0',
  `level_50` bigint(20) unsigned NOT NULL default '0',
  `level_60` bigint(20) unsigned NOT NULL default '0',
  `level_70` bigint(20) unsigned NOT NULL default '0',
  `level_80` bigint(20) unsigned NOT NULL default '0',
  `level_90` bigint(20) unsigned NOT NULL default '0',
  `level_100` bigint(20) unsigned NOT NULL default '0',
  `level_110` bigint(20) unsigned NOT NULL default '0',
  `level_120+` bigint(20) unsigned NOT NULL  default '0' comment '120级及120级以上的玩家消费汇总',
  PRIMARY KEY (`Date`,`ModuleType`,`OpType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- 设定更新资源消耗历史表的时间间隔
-- ----------------------------
use ios_app_cfg;

DROP PROCEDURE if exists AddCloumn;
CREATE PROCEDURE AddCloumn ()
BEGIN
	-- 先把原来的那个字段删掉
	if exists(select column_name from INFORMATION_SCHEMA.columns where table_name = 'singlepram' and column_name = 'SaveOpHistoryToDBInterval' ) then
		ALTER TABLE singlepram DROP COLUMN SaveOpHistoryToDBInterval; 
	end if;

	-- 然后再直接新添加字段
	ALTER TABLE singlepram ADD column SaveOpHistoryToDBInterval smallint(6) NOT NULL DEFAULT '60' COMMENT '更新资源消耗历史表的间隔（秒）（一分钟左右为宜）';
	update singlepram set SaveOpHistoryToDBInterval = 60; -- 时间设置成60秒
END;

call AddCloumn();
DROP PROCEDURE if exists AddCloumn;