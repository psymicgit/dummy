/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50528
Source Host           : localhost:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50528
File Encoding         : 65001

Date: 2014-03-21 13:41:41
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for awakenresolveprop
-- ----------------------------
DROP TABLE IF EXISTS `awakenresolveprop`;
CREATE TABLE `awakenresolveprop` (
  `AwakenColorKind` smallint(5) unsigned NOT NULL DEFAULT '1',
  `IfResolve` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `AwakenCommonNum` smallint(5) unsigned NOT NULL DEFAULT '5',
  `AwakenCommonCoin` smallint(5) unsigned NOT NULL DEFAULT '2000',
  `AwakenSpecialNum` smallint(5) unsigned NOT NULL DEFAULT '5',
  `AwakenSpecialGold` smallint(5) unsigned NOT NULL DEFAULT '200'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of awakenresolveprop
-- ----------------------------
INSERT INTO `awakenresolveprop` VALUES ('4', '1', '1', '20000', '3', '200');
INSERT INTO `awakenresolveprop` VALUES ('5', '1', '2', '20000', '4', '200');
