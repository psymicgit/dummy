/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50614
Source Host           : localhost:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50614
File Encoding         : 65001

Date: 2013-12-03 10:27:22
*/

-- 活动聊天频道的消息定义

use ios_app_cfg;

REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_REPORT', '本次击杀【火麒麟】活动中，', '');
REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_BOSS_BE_KILLED', '世界BOSS【火麒麟】已被玩家【{玩家名字}】击杀，获得奖励:铜钱{x}.', '');
REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_RANK_ENCOURAGE', '第{xx}名【{玩家名字}】,奖励铜钱{M},境界点{N}', '');
REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_START', 'BOSS战已开始，请各位少侠火速前往麒麟洞镇压火麒麟', '');

use ios_app_pay;

-- ----------------------------
-- 系统公告聊天频道
-- ----------------------------
DROP TABLE IF EXISTS `syschat`;
CREATE TABLE `syschat` (
  `NoticeID` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '聊天公告Id',
  `ZoneID` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '区Id',
  `NoticeContent` varchar(1024) NOT NULL COMMENT '系统聊天公告的内容',
  `ExpiresTime` datetime NOT NULL COMMENT '过期时间',
  PRIMARY KEY (`NoticeID`)
) ENGINE=InnoDB AUTO_INCREMENT=78 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of syschat
-- ----------------------------
