/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50528
Source Host           : localhost:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50528
File Encoding         : 65001

Date: 2014-05-05 11:38:49
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for singlepram
-- ----------------------------
DROP TABLE IF EXISTS `singlepram`;
CREATE TABLE `singlepram` (
  `MaxPlayerLevel` smallint(5) unsigned NOT NULL,
  `OnlineEncourageKeepDays` smallint(5) unsigned NOT NULL,
  `LoginEncourageKeepDays` smallint(5) unsigned NOT NULL,
  `RefreshEliteNumPerDay` smallint(5) unsigned NOT NULL,
  `GainCropCDGoldPerMin` smallint(5) unsigned NOT NULL,
  `MaxHPGridNum` smallint(5) unsigned NOT NULL,
  `RecruitFavorability` smallint(5) unsigned NOT NULL,
  `RecruitReturnCoinPercentage` smallint(5) unsigned NOT NULL,
  `JuniorLevel` smallint(5) unsigned NOT NULL,
  `TotalYabiaoCnt` int(11) unsigned NOT NULL,
  `TotalJiebiaoCnt` int(11) unsigned NOT NULL,
  `MaxLevelCostGold` int(11) unsigned NOT NULL,
  `JiebiaoViewPlayerCnt` int(11) unsigned NOT NULL,
  `TotalBeJiebiaoCnt` int(11) unsigned NOT NULL,
  `JiebiaoCD` int(11) unsigned NOT NULL,
  `ResetClimbTower` int(11) unsigned NOT NULL DEFAULT '100',
  `ClearJiebiaoCDCostGoldPerMin` int(11) unsigned NOT NULL DEFAULT '10',
  `UpdateRecordDataToSqlInterval` int(11) unsigned NOT NULL DEFAULT '6',
  `PerfectFreeGodweaponGold` int(11) NOT NULL,
  `GoldExperienceAddValue` int(11) NOT NULL,
  `GoldExperienceCostValue` int(11) NOT NULL,
  `FreeFlushBiaoCnt` smallint(6) NOT NULL DEFAULT '0',
  `AppAddress` varchar(512) NOT NULL DEFAULT '',
  `HeroTransFreePercentage` smallint(6) NOT NULL DEFAULT '50' COMMENT '//伙伴传递经验',
  `HeroTransGoldPercentage` smallint(6) NOT NULL DEFAULT '100' COMMENT '//伙伴传递经验',
  `TransGold` smallint(6) NOT NULL DEFAULT '100',
  `CoachExpRate` smallint(5) unsigned NOT NULL DEFAULT '150',
  `ScienceDspContent` varchar(1024) NOT NULL,
  `OpenOprateingActivityLevel` smallint(5) unsigned NOT NULL COMMENT '运营活动开放所需玩家等级',
  `SaveOpHistoryToDBInterval` smallint(6) NOT NULL DEFAULT '60' COMMENT '更新资源消耗历史表的间隔（秒）（一分钟左右为宜）',
  `ReCalcHowManyTopRacer` smallint(6) NOT NULL DEFAULT '0' COMMENT '启动时重新计算多少个竞技场玩家的战力并比对（一般是前100个，如果是0个则表示不重新计算）',
  `IsSaveNewRacerPowerToDB` smallint(6) NOT NULL DEFAULT '0' COMMENT '是否把战力回写到数据库：是1/否0',
  `SavePlayerSqlInterval` int(11) NOT NULL,
  `ApplyFactionMaxNum` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EveryDayAuditMaxNum` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SureOutOrangeAwaken` smallint(5) unsigned NOT NULL DEFAULT '0',
  `FreeChangeIcon` smallint(6) NOT NULL DEFAULT '10',
  `EveryDayRecvFlowers` smallint(6) NOT NULL DEFAULT '10',
  `ExpiresRecvFlower` int(11) NOT NULL DEFAULT '127800',
  `NormalFindBackDoorsTribute` smallint(6) NOT NULL DEFAULT '10',
  `PerfectFindBackDoorsTribute` smallint(6) NOT NULL DEFAULT '0',
  `ExprienceAddInterval` int(11) NOT NULL DEFAULT '10',
  `ChangeFactionIcon` smallint(5) unsigned NOT NULL DEFAULT '100',
  `CloseAreaBoosMulti` smallint(5) unsigned NOT NULL DEFAULT '5',
  `MaxScienceLevel` smallint(5) unsigned NOT NULL DEFAULT '90',
  `JewelryStrengIntervalLevel` smallint(5) unsigned NOT NULL DEFAULT '5',
  `MaxJewelryStrengNum` smallint(5) unsigned NOT NULL DEFAULT '5',
  `JewelryResolveReturn` smallint(5) unsigned NOT NULL DEFAULT '75',
  `JewelryBlueNum` smallint(5) unsigned NOT NULL DEFAULT '1',
  `JewelryPurpleNum` smallint(5) unsigned NOT NULL DEFAULT '2',
  `JewelryRedNum` smallint(5) unsigned NOT NULL DEFAULT '3',
  `JewelryOrangeNum` smallint(5) unsigned NOT NULL DEFAULT '4',
  `CourageHelpTimes` smallint(5) unsigned NOT NULL DEFAULT '15'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of singlepram
-- ----------------------------
INSERT INTO `singlepram` VALUES ('120', '65534', '3', '0', '1', '20', '5', '50', '10', '5', '10', '100', '15', '2', '180', '1', '10', '10', '200', '1000000', '100', '5', '', '55', '85', '100', '150', '客服QQ：4008-500-737       官方网站:http://jxy.737.com', '0', '60', '0', '0', '1800', '5', '15', '5', '1', '10', '172800', '75', '100', '3', '100', '7', '90', '5', '5', '100', '1', '2', '3', '4', '15');
