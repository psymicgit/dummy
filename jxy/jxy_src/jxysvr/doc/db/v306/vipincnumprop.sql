/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50528
Source Host           : localhost:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50528
File Encoding         : 65001

Date: 2014-04-15 20:34:20
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for vipincnumprop
-- ----------------------------
DROP TABLE IF EXISTS `vipincnumprop`;
CREATE TABLE `vipincnumprop` (
  `FuncKind` smallint(5) unsigned NOT NULL,
  `VipLevel` smallint(5) unsigned NOT NULL,
  `IncNum` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`FuncKind`,`VipLevel`,`IncNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of vipincnumprop
-- ----------------------------
INSERT INTO `vipincnumprop` VALUES ('1', '0', '5');
INSERT INTO `vipincnumprop` VALUES ('1', '1', '10');
INSERT INTO `vipincnumprop` VALUES ('1', '2', '15');
INSERT INTO `vipincnumprop` VALUES ('1', '3', '20');
INSERT INTO `vipincnumprop` VALUES ('1', '4', '30');
INSERT INTO `vipincnumprop` VALUES ('1', '5', '40');
INSERT INTO `vipincnumprop` VALUES ('1', '6', '50');
INSERT INTO `vipincnumprop` VALUES ('1', '7', '60');
INSERT INTO `vipincnumprop` VALUES ('1', '8', '80');
INSERT INTO `vipincnumprop` VALUES ('1', '9', '100');
INSERT INTO `vipincnumprop` VALUES ('1', '10', '120');
INSERT INTO `vipincnumprop` VALUES ('2', '0', '12');
INSERT INTO `vipincnumprop` VALUES ('2', '1', '14');
INSERT INTO `vipincnumprop` VALUES ('2', '2', '16');
INSERT INTO `vipincnumprop` VALUES ('2', '3', '18');
INSERT INTO `vipincnumprop` VALUES ('2', '4', '20');
INSERT INTO `vipincnumprop` VALUES ('2', '5', '25');
INSERT INTO `vipincnumprop` VALUES ('2', '6', '30');
INSERT INTO `vipincnumprop` VALUES ('2', '7', '35');
INSERT INTO `vipincnumprop` VALUES ('2', '8', '40');
INSERT INTO `vipincnumprop` VALUES ('2', '9', '50');
INSERT INTO `vipincnumprop` VALUES ('2', '10', '60');
INSERT INTO `vipincnumprop` VALUES ('3', '0', '12');
INSERT INTO `vipincnumprop` VALUES ('3', '1', '14');
INSERT INTO `vipincnumprop` VALUES ('3', '2', '16');
INSERT INTO `vipincnumprop` VALUES ('3', '3', '18');
INSERT INTO `vipincnumprop` VALUES ('3', '4', '20');
INSERT INTO `vipincnumprop` VALUES ('3', '5', '25');
INSERT INTO `vipincnumprop` VALUES ('3', '6', '30');
INSERT INTO `vipincnumprop` VALUES ('3', '7', '35');
INSERT INTO `vipincnumprop` VALUES ('3', '8', '40');
INSERT INTO `vipincnumprop` VALUES ('3', '9', '50');
INSERT INTO `vipincnumprop` VALUES ('3', '10', '60');
INSERT INTO `vipincnumprop` VALUES ('4', '0', '2');
INSERT INTO `vipincnumprop` VALUES ('4', '1', '4');
INSERT INTO `vipincnumprop` VALUES ('4', '2', '6');
INSERT INTO `vipincnumprop` VALUES ('4', '3', '8');
INSERT INTO `vipincnumprop` VALUES ('4', '4', '10');
INSERT INTO `vipincnumprop` VALUES ('4', '5', '15');
INSERT INTO `vipincnumprop` VALUES ('4', '6', '20');
INSERT INTO `vipincnumprop` VALUES ('4', '7', '25');
INSERT INTO `vipincnumprop` VALUES ('4', '8', '30');
INSERT INTO `vipincnumprop` VALUES ('4', '9', '35');
INSERT INTO `vipincnumprop` VALUES ('4', '10', '40');
INSERT INTO `vipincnumprop` VALUES ('5', '0', '1');
INSERT INTO `vipincnumprop` VALUES ('5', '1', '1');
INSERT INTO `vipincnumprop` VALUES ('5', '2', '1');
INSERT INTO `vipincnumprop` VALUES ('5', '3', '1');
INSERT INTO `vipincnumprop` VALUES ('5', '4', '2');
INSERT INTO `vipincnumprop` VALUES ('5', '5', '3');
INSERT INTO `vipincnumprop` VALUES ('5', '6', '4');
INSERT INTO `vipincnumprop` VALUES ('5', '7', '5');
INSERT INTO `vipincnumprop` VALUES ('5', '8', '6');
INSERT INTO `vipincnumprop` VALUES ('5', '9', '7');
INSERT INTO `vipincnumprop` VALUES ('5', '10', '8');
INSERT INTO `vipincnumprop` VALUES ('7', '0', '0');
INSERT INTO `vipincnumprop` VALUES ('7', '1', '0');
INSERT INTO `vipincnumprop` VALUES ('7', '2', '0');
INSERT INTO `vipincnumprop` VALUES ('7', '3', '5');
INSERT INTO `vipincnumprop` VALUES ('7', '4', '10');
INSERT INTO `vipincnumprop` VALUES ('7', '5', '15');
INSERT INTO `vipincnumprop` VALUES ('7', '6', '20');
INSERT INTO `vipincnumprop` VALUES ('7', '7', '30');
INSERT INTO `vipincnumprop` VALUES ('7', '8', '40');
INSERT INTO `vipincnumprop` VALUES ('7', '9', '50');
INSERT INTO `vipincnumprop` VALUES ('7', '10', '60');
INSERT INTO `vipincnumprop` VALUES ('8', '0', '0');
INSERT INTO `vipincnumprop` VALUES ('8', '1', '1');
INSERT INTO `vipincnumprop` VALUES ('8', '2', '1');
INSERT INTO `vipincnumprop` VALUES ('8', '3', '2');
INSERT INTO `vipincnumprop` VALUES ('8', '4', '3');
INSERT INTO `vipincnumprop` VALUES ('8', '5', '4');
INSERT INTO `vipincnumprop` VALUES ('8', '6', '5');
INSERT INTO `vipincnumprop` VALUES ('8', '7', '6');
INSERT INTO `vipincnumprop` VALUES ('8', '8', '7');
INSERT INTO `vipincnumprop` VALUES ('8', '9', '8');
INSERT INTO `vipincnumprop` VALUES ('8', '10', '9');
INSERT INTO `vipincnumprop` VALUES ('11', '0', '0');
INSERT INTO `vipincnumprop` VALUES ('11', '1', '0');
INSERT INTO `vipincnumprop` VALUES ('11', '2', '0');
INSERT INTO `vipincnumprop` VALUES ('11', '3', '5');
INSERT INTO `vipincnumprop` VALUES ('11', '4', '10');
INSERT INTO `vipincnumprop` VALUES ('11', '5', '15');
INSERT INTO `vipincnumprop` VALUES ('11', '6', '20');
INSERT INTO `vipincnumprop` VALUES ('11', '7', '30');
INSERT INTO `vipincnumprop` VALUES ('11', '8', '40');
INSERT INTO `vipincnumprop` VALUES ('11', '9', '50');
INSERT INTO `vipincnumprop` VALUES ('11', '10', '60');
INSERT INTO `vipincnumprop` VALUES ('16', '0', '0');
INSERT INTO `vipincnumprop` VALUES ('16', '1', '0');
INSERT INTO `vipincnumprop` VALUES ('16', '2', '0');
INSERT INTO `vipincnumprop` VALUES ('16', '3', '1');
INSERT INTO `vipincnumprop` VALUES ('16', '4', '2');
INSERT INTO `vipincnumprop` VALUES ('16', '5', '3');
INSERT INTO `vipincnumprop` VALUES ('16', '6', '4');
INSERT INTO `vipincnumprop` VALUES ('16', '7', '5');
INSERT INTO `vipincnumprop` VALUES ('16', '8', '6');
INSERT INTO `vipincnumprop` VALUES ('16', '9', '7');
INSERT INTO `vipincnumprop` VALUES ('16', '10', '8');
