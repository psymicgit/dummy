/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50528
Source Host           : localhost:3306
Source Database       : ios_app_s001

Target Server Type    : MYSQL
Target Server Version : 50528
File Encoding         : 65001

Date: 2014-04-26 17:05:22
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for battlelog2
-- ----------------------------
DROP TABLE IF EXISTS `battlelog2`;
CREATE TABLE `battlelog2` (
  `DataIndex` int(11) NOT NULL,
  `PlayerID1` int(11) NOT NULL,
  `PlayerID2` int(11) NOT NULL,
  `BattleData` blob NOT NULL,
  `CreateTime` datetime NOT NULL,
  `Version` int(11) NOT NULL,
  PRIMARY KEY (`DataIndex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
