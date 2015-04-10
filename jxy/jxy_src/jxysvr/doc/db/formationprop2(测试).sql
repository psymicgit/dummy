/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50613
Source Host           : 127.0.0.1:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50613
File Encoding         : 65001

Date: 2014-02-12 22:07:45
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `formationprop2`
-- ----------------------------
DROP TABLE IF EXISTS `formationprop2`;
CREATE TABLE `formationprop2` (
  `FormationKindID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OpenIdx` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CoachIdxFlag` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VisableLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UnlockLevel` smallint(5) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of formationprop2
-- ----------------------------
INSERT INTO `formationprop2` VALUES ('1', '1', '0', '1', '0');
INSERT INTO `formationprop2` VALUES ('1', '3', '1', '1', '0');
INSERT INTO `formationprop2` VALUES ('1', '4', '0', '1', '25');
INSERT INTO `formationprop2` VALUES ('1', '6', '0', '25', '30');
INSERT INTO `formationprop2` VALUES ('2', '1', '0', '1', '1');
INSERT INTO `formationprop2` VALUES ('2', '3', '1', '1', '1');
INSERT INTO `formationprop2` VALUES ('2', '4', '0', '1', '25');
INSERT INTO `formationprop2` VALUES ('2', '6', '0', '25', '30');
INSERT INTO `formationprop2` VALUES ('3', '1', '0', '1', '1');
INSERT INTO `formationprop2` VALUES ('3', '3', '1', '1', '1');
INSERT INTO `formationprop2` VALUES ('3', '4', '0', '1', '25');
INSERT INTO `formationprop2` VALUES ('3', '6', '0', '25', '30');
INSERT INTO `formationprop2` VALUES ('4', '1', '0', '1', '1');
INSERT INTO `formationprop2` VALUES ('4', '3', '1', '1', '1');
INSERT INTO `formationprop2` VALUES ('4', '4', '0', '1', '25');
INSERT INTO `formationprop2` VALUES ('4', '6', '0', '25', '30');

-- ----------------------------
-- Table structure for `formationunlock`
-- ----------------------------
DROP TABLE IF EXISTS `formationunlock`;
CREATE TABLE `formationunlock` (
  `FormationID` smallint(5) NOT NULL,
  `VisibleLevel` smallint(5) NOT NULL,
  `UnlockLevel` smallint(5) NOT NULL,
  PRIMARY KEY (`FormationID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of formationunlock
-- ----------------------------
INSERT INTO `formationunlock` VALUES ('1', '1', '1');
INSERT INTO `formationunlock` VALUES ('2', '1', '1');
INSERT INTO `formationunlock` VALUES ('3', '1', '35');
INSERT INTO `formationunlock` VALUES ('4', '35', '40');

-- ----------------------------
-- Table structure for `formationupgradeprop`
-- ----------------------------
DROP TABLE IF EXISTS `formationupgradeprop`;
CREATE TABLE `formationupgradeprop` (
  `FormationID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AttrKindName` varchar(30) NOT NULL DEFAULT '',
  `AttrKindID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AttrLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AttrKindValue` int(12) unsigned NOT NULL DEFAULT '0',
  `UpgradeSciencePoint` int(12) unsigned NOT NULL DEFAULT '0',
  `NeedPlayerLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`FormationID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of formationupgradeprop
-- ----------------------------
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵', '2', '0', '100', '1000', '30');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵', '1', '0', '100', '1000', '1');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵', '3', '0', '100', '1000', '35');
INSERT INTO `formationupgradeprop` VALUES ('4', '玄武阵', '4', '0', '100', '1000', '40');

-- ----------------------------
-- Table structure for `huntingcollectiverewardprop2`
-- ----------------------------
DROP TABLE IF EXISTS `huntingcollectiverewardprop2`;
CREATE TABLE `huntingcollectiverewardprop2` (
  `ScheduleLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NeedShadeLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaxIntegral` int(12) unsigned NOT NULL DEFAULT '0',
  `RewardType` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewardValue` int(12) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ScheduleLevel`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of huntingcollectiverewardprop2
-- ----------------------------
INSERT INTO `huntingcollectiverewardprop2` VALUES ('1', '1', '100', '11', '15000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('2', '1', '150', '11', '24000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('3', '1', '200', '11', '36000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('4', '2', '250', '11', '48000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('5', '3', '300', '11', '63000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('6', '4', '350', '11', '78000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('7', '5', '400', '11', '96000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('8', '6', '450', '11', '114000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('9', '7', '500', '11', '135000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('10', '8', '550', '11', '156000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('11', '9', '600', '11', '180000');
INSERT INTO `huntingcollectiverewardprop2` VALUES ('12', '10', '650', '11', '204000');

ALTER TABLE singlepram ADD COLUMN FreeChangeIcon smallint(5) unsigned NOT NULL DEFAULT 0;
ALTER TABLE singlepram ADD COLUMN NormalFindBackDoorsTribute smallint(5) unsigned NOT NULL DEFAULT 0;
ALTER TABLE singlepram ADD COLUMN PerfectFindBackDoorsTribute smallint(5) unsigned NOT NULL DEFAULT 0;
