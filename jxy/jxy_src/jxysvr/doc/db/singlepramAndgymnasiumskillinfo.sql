/*
Navicat MySQL Data Transfer

Source Server         : 192.168.4.56( wengyanhai)
Source Server Version : 50529
Source Host           : 192.168.4.56:3306
Source Database       : ios_app_cfg_now

Target Server Type    : MYSQL
Target Server Version : 50529
File Encoding         : 65001

Date: 2014-01-02 11:45:44
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `singlepram`
-- ----------------------------
ALTER TABLE singlepram ADD COLUMN ApplyFactionMaxNum smallint(5) unsigned NOT NULL DEFAULT '0';

-- ----------------------------
-- Records of singlepram
-- ----------------------------
UPDATE singlepram set ApplyFactionMaxNum = 5;

-- ----------------------------
-- Table structure for `gymnasiumskillinfo`
-- ----------------------------
DROP TABLE IF EXISTS `gymnasiumskillinfo`;
CREATE TABLE `gymnasiumskillinfo` (
  `SkillKindID` tinyint(5) NOT NULL DEFAULT '0',
  `SkillName` varchar(32) NOT NULL DEFAULT '',
  `UnLockLevelByGymnasium` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DisplayByGymnasium` tinyint(3) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of gymnasiumskillinfo
-- ----------------------------
INSERT INTO `gymnasiumskillinfo` VALUES ('1', '生命加强', '1', '1');
INSERT INTO `gymnasiumskillinfo` VALUES ('2', '攻击加强', '1', '1');
INSERT INTO `gymnasiumskillinfo` VALUES ('4', '格挡加强', '3', '1');
INSERT INTO `gymnasiumskillinfo` VALUES ('3', '破击加强', '5', '3');
INSERT INTO `gymnasiumskillinfo` VALUES ('5', '暴击加强', '3', '1');
INSERT INTO `gymnasiumskillinfo` VALUES ('6', '抗暴加强', '5', '3');