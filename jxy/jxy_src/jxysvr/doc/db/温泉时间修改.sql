/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50613
Source Host           : 127.0.0.1:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50613
File Encoding         : 65001

Date: 2014-01-22 17:06:09
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `wineprop`
-- ----------------------------
DROP TABLE IF EXISTS `wineprop`;
CREATE TABLE `wineprop` (
  `WineID` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `CostType` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `CostValue` int(12) unsigned NOT NULL DEFAULT '0',
  `WineCDValue` smallint(12) unsigned NOT NULL DEFAULT '0',
  `LeftDrinkNum` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `RewardID1` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `RewardValue1` int(12) unsigned NOT NULL DEFAULT '0',
  `RewardID2` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `RewardValue2` int(12) unsigned NOT NULL DEFAULT '0',
  `NeedVipLevel` tinyint(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`WineID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of wineprop
-- ----------------------------
INSERT INTO `wineprop` VALUES ('1', '2', '100000', '36', '5', '4', '5', '8', '300', '0');
INSERT INTO `wineprop` VALUES ('2', '3', '50', '36', '5', '4', '10', '8', '1500', '0');
INSERT INTO `wineprop` VALUES ('3', '3', '100', '36', '5', '4', '20', '8', '4000', '2');
