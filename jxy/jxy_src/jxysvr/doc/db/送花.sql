/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50529
Source Host           : 127.0.0.1:3306
Source Database       : ios_app_cfg_11_29

Target Server Type    : MYSQL
Target Server Version : 50529
File Encoding         : 65001

Date: 2014-01-13 17:39:28
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `activityencouragepro`
-- ----------------------------
DROP TABLE IF EXISTS `activityencouragepro`;
CREATE TABLE `activityencouragepro` (
  `ActivityType` smallint(6) NOT NULL DEFAULT '1' COMMENT '活动类型',
  `ActivityName` varchar(256) NOT NULL DEFAULT '0' COMMENT '物品ID，',
  `ActivityDesc` varchar(1024) NOT NULL DEFAULT '',
  `JsonValues` varchar(1024) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of activityencouragepro
-- ----------------------------
INSERT INTO `activityencouragepro` VALUES ('1', '充值奖励', '充值奖励', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 20053,\r\n            \"num\": 10\r\n        },\r\n        {\r\n            \"value\": 38002,\r\n            \"num\": 1\r\n        }\r\n,\r\n        {\r\n            \"value\": 30,\r\n            \"num\": 1\r\n        }\r\n,\r\n        {\r\n            \"value\": 37,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 131,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');
INSERT INTO `activityencouragepro` VALUES ('2', '评分奖励', '评分奖励', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');

-- ----------------------------
-- Table structure for `recvflowerencourageprop`
-- ----------------------------
DROP TABLE IF EXISTS `recvflowerencourageprop`;
CREATE TABLE `recvflowerencourageprop` (
  `KindID` int(11) unsigned NOT NULL,
  `jsoncouragedata` varchar(1024) NOT NULL DEFAULT '',
  PRIMARY KEY (`KindID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of recvflowerencourageprop
-- ----------------------------
INSERT INTO `recvflowerencourageprop` VALUES ('1', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');
INSERT INTO `recvflowerencourageprop` VALUES ('9', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');
INSERT INTO `recvflowerencourageprop` VALUES ('99', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');

-- ----------------------------
-- Table structure for `sendflowerencourageprop`
-- ----------------------------
DROP TABLE IF EXISTS `sendflowerencourageprop`;
CREATE TABLE `sendflowerencourageprop` (
  `flowerNum` int(11) unsigned NOT NULL,
  `jsoncouragedata` varchar(1024) NOT NULL DEFAULT '',
  PRIMARY KEY (`flowerNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sendflowerencourageprop
-- ----------------------------
INSERT INTO `sendflowerencourageprop` VALUES ('1', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');
INSERT INTO `sendflowerencourageprop` VALUES ('2', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');
INSERT INTO `sendflowerencourageprop` VALUES ('3', '{\r\n    \"item\": [\r\n        {\r\n            \"value\": 33002,\r\n            \"num\": 1\r\n        },\r\n        {\r\n            \"value\": 38003,\r\n            \"num\": 1\r\n        }\r\n    ],\r\n    \"hero\": 0,\r\n    \"coin\": 0,\r\n    \"gold\": 0,\r\n    \"phystrength\": 0,\r\n    \"science\": 0,\r\n    \"story\": 0,\r\n    \"bluegas\": 0,\r\n    \"pruplegas\": 0,\r\n    \"jingjie\": 0,\r\n    \"doorstribute\": 0,\r\n    \"factionfunds\": 0,\r\n    \"factiontaskintegral\": 0\r\n}');

ALTER TABLE `singlepram`
ADD COLUMN `EveryDayRecvFlowers`  smallint NOT NULL DEFAULT 10 AFTER `SureOutOrangeAwaken`;

ALTER TABLE `singlepram`
ADD COLUMN `ExpiresRecvFlower`  int NOT NULL DEFAULT 127800 AFTER `EveryDayRecvFlowers`;