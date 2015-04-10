/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50528
Source Host           : localhost:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50528
File Encoding         : 65001

Date: 2014-03-28 21:58:17
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for courageactivitytimeprop
-- ----------------------------
DROP TABLE IF EXISTS `courageactivitytimeprop`;
CREATE TABLE `courageactivitytimeprop` (
  `ActivityKind` smallint(6) unsigned NOT NULL DEFAULT '1',
  `StartTime` char(5) NOT NULL DEFAULT '0',
  `KeepTime` int(11) unsigned NOT NULL DEFAULT '0',
  `MaxGroupNum` int(11) NOT NULL DEFAULT '16',
  PRIMARY KEY (`ActivityKind`,`StartTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of courageactivitytimeprop
-- ----------------------------
INSERT INTO `courageactivitytimeprop` VALUES ('1', '0840', '62900', '16');
