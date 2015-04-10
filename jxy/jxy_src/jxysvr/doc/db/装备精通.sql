/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50614
Source Host           : localhost:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50614
File Encoding         : 65001

Date: 2013-11-19 20:50:12
*/

SET FOREIGN_KEY_CHECKS=0;

-- ---------------------------------------------
-- 给玩家表添加CourageMedal（勇气勋章数）字段
-- ---------------------------------------------
use ios_app_s001;

DROP PROCEDURE if exists AddCloumn;

CREATE PROCEDURE AddCloumn ()
BEGIN
	if not exists(select column_name from INFORMATION_SCHEMA.columns where table_name = 'players' and column_name = 'CourageMedal' ) then
		ALTER TABLE players ADD CourageMedal int(11) NOT NULL  DEFAULT '0' COMMENT '勇气勋章';
	end if;

	if not exists(select column_name from INFORMATION_SCHEMA.columns where table_name = 'player_consume' and column_name = 'AddMedal' ) then
		ALTER TABLE player_consume ADD AddMedal int(11) NOT NULL  DEFAULT '0' COMMENT '勇气勋章';
		ALTER TABLE player_consume ADD DecMedal int(11) NOT NULL  DEFAULT '0' COMMENT '勇气勋章';
		ALTER TABLE player_consume ADD AfterMedal int(11) NOT NULL  DEFAULT '0' COMMENT '勇气勋章';
	end if;
END;

call AddCloumn();
DROP PROCEDURE if exists AddCloumn;

use ios_app_cfg;

-- ----------------------------
-- Table structure for masterbreakprop
-- ----------------------------
DROP TABLE IF EXISTS `masterbreakprop`;
CREATE TABLE `masterbreakprop` (
  `EquipKind` smallint(6) unsigned NOT NULL COMMENT '装备类别（头盔1、战袍2、武器3、战靴4）',
  `Rank` smallint(6) unsigned NOT NULL COMMENT '装备精通级别（未激活0、初1、中2、高3、极4）',
  `BreakMedal` int(11) unsigned DEFAULT '0' COMMENT '突破到下一级所需勇气勋章',
  `BreakLevel` int(6) unsigned DEFAULT NULL COMMENT '突破所需精通等级',
  `BreakPlayerLevel` smallint(6) NOT NULL DEFAULT '0' COMMENT '突破所需玩家等级',
  PRIMARY KEY (`EquipKind`,`Rank`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of masterbreakprop
-- ----------------------------
INSERT INTO `masterbreakprop` VALUES ('1', '0', '100', '0', '30');
INSERT INTO `masterbreakprop` VALUES ('1', '1', '200', '15', '40');
INSERT INTO `masterbreakprop` VALUES ('1', '2', '400', '20', '60');
INSERT INTO `masterbreakprop` VALUES ('1', '3', '800', '25', '80');
INSERT INTO `masterbreakprop` VALUES ('1', '4', '0', '30', '0');
INSERT INTO `masterbreakprop` VALUES ('2', '0', '100', '0', '30');
INSERT INTO `masterbreakprop` VALUES ('2', '1', '200', '15', '40');
INSERT INTO `masterbreakprop` VALUES ('2', '2', '400', '20', '60');
INSERT INTO `masterbreakprop` VALUES ('2', '3', '800', '25', '80');
INSERT INTO `masterbreakprop` VALUES ('2', '4', '0', '30', '0');
INSERT INTO `masterbreakprop` VALUES ('3', '0', '100', '0', '30');
INSERT INTO `masterbreakprop` VALUES ('3', '1', '200', '15', '40');
INSERT INTO `masterbreakprop` VALUES ('3', '2', '400', '20', '60');
INSERT INTO `masterbreakprop` VALUES ('3', '3', '800', '25', '80');
INSERT INTO `masterbreakprop` VALUES ('3', '4', '0', '30', '0');
INSERT INTO `masterbreakprop` VALUES ('4', '0', '100', '0', '30');
INSERT INTO `masterbreakprop` VALUES ('4', '1', '200', '15', '40');
INSERT INTO `masterbreakprop` VALUES ('4', '2', '400', '20', '60');
INSERT INTO `masterbreakprop` VALUES ('4', '3', '800', '25', '80');
INSERT INTO `masterbreakprop` VALUES ('4', '4', '0', '30', '0');

-- ----------------------------
-- Table structure for mastercheatprop
-- ----------------------------
DROP TABLE IF EXISTS `mastercheatprop`;
CREATE TABLE `mastercheatprop` (
  `CheatTimes` tinyint(4) NOT NULL COMMENT '培养精通之书的出千次数',
  `GoldCost` smallint(6) NOT NULL DEFAULT '0' COMMENT '出千所耗费的元宝数',
  PRIMARY KEY (`CheatTimes`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mastercheatprop
-- ----------------------------
INSERT INTO `mastercheatprop` VALUES ('1', '0');
INSERT INTO `mastercheatprop` VALUES ('2', '10');
INSERT INTO `mastercheatprop` VALUES ('3', '10');
INSERT INTO `mastercheatprop` VALUES ('4', '15');
INSERT INTO `mastercheatprop` VALUES ('5', '15');
INSERT INTO `mastercheatprop` VALUES ('6', '20');
INSERT INTO `mastercheatprop` VALUES ('7', '20');
INSERT INTO `mastercheatprop` VALUES ('8', '25');
INSERT INTO `mastercheatprop` VALUES ('9', '25');
INSERT INTO `mastercheatprop` VALUES ('10', '30');
INSERT INTO `mastercheatprop` VALUES ('11', '40');
INSERT INTO `mastercheatprop` VALUES ('12', '50');

-- ----------------------------
-- Table structure for masterdefprop
-- ----------------------------
DROP TABLE IF EXISTS `masterdefprop`;
CREATE TABLE `masterdefprop` (
  `BigBookExp` smallint(6) NOT NULL COMMENT '大经验书加多少经验',
  `SmallBookExp` smallint(6) NOT NULL COMMENT '小经验书加多少经验',
  `MasterCultivateMedalCost` smallint(6) NOT NULL COMMENT '培养精通之书的勋章消耗',
  `MasterCheatPlayerLevel` smallint(6) NOT NULL COMMENT '玩家达到几级才能出千'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of masterdefprop
-- ----------------------------
INSERT INTO `masterdefprop` VALUES ('100', '10', '50', '40');

-- ----------------------------
-- Table structure for masterexpprop
-- ----------------------------
DROP TABLE IF EXISTS `masterexpprop`;
CREATE TABLE `masterexpprop` (
  `EquipKind` smallint(6) unsigned NOT NULL COMMENT '装备类别（武器1、护甲2、鞋子3、头盔4）',
  `Rank` smallint(6) unsigned NOT NULL COMMENT '装备精通阶级（未激活0、初1、中2、高3、极4）',
  `Level` int(11) unsigned NOT NULL COMMENT '精通等级',
  `Exp` int(11) unsigned DEFAULT NULL COMMENT '本级经验条长度',
  `Promote` smallint(6) unsigned DEFAULT NULL COMMENT '本级的属性加成，值20则表示属性加成+20%',
  PRIMARY KEY (`EquipKind`,`Rank`,`Level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of masterexpprop
-- ----------------------------
INSERT INTO `masterexpprop` VALUES ('1', '1', '1', '220', '5');
INSERT INTO `masterexpprop` VALUES ('1', '1', '2', '240', '6');
INSERT INTO `masterexpprop` VALUES ('1', '1', '3', '260', '7');
INSERT INTO `masterexpprop` VALUES ('1', '1', '4', '280', '8');
INSERT INTO `masterexpprop` VALUES ('1', '1', '5', '300', '9');
INSERT INTO `masterexpprop` VALUES ('1', '1', '6', '320', '10');
INSERT INTO `masterexpprop` VALUES ('1', '1', '7', '340', '11');
INSERT INTO `masterexpprop` VALUES ('1', '1', '8', '360', '12');
INSERT INTO `masterexpprop` VALUES ('1', '1', '9', '380', '13');
INSERT INTO `masterexpprop` VALUES ('1', '1', '10', '400', '14');
INSERT INTO `masterexpprop` VALUES ('1', '1', '11', '420', '15');
INSERT INTO `masterexpprop` VALUES ('1', '1', '12', '440', '16');
INSERT INTO `masterexpprop` VALUES ('1', '1', '13', '460', '17');
INSERT INTO `masterexpprop` VALUES ('1', '1', '14', '480', '18');
INSERT INTO `masterexpprop` VALUES ('1', '1', '15', '500', '19');
INSERT INTO `masterexpprop` VALUES ('1', '2', '1', '680', '29');
INSERT INTO `masterexpprop` VALUES ('1', '2', '2', '760', '31');
INSERT INTO `masterexpprop` VALUES ('1', '2', '3', '840', '33');
INSERT INTO `masterexpprop` VALUES ('1', '2', '4', '920', '35');
INSERT INTO `masterexpprop` VALUES ('1', '2', '5', '1000', '37');
INSERT INTO `masterexpprop` VALUES ('1', '2', '6', '1080', '39');
INSERT INTO `masterexpprop` VALUES ('1', '2', '7', '1160', '41');
INSERT INTO `masterexpprop` VALUES ('1', '2', '8', '1240', '43');
INSERT INTO `masterexpprop` VALUES ('1', '2', '9', '1320', '45');
INSERT INTO `masterexpprop` VALUES ('1', '2', '10', '1400', '47');
INSERT INTO `masterexpprop` VALUES ('1', '2', '11', '1480', '49');
INSERT INTO `masterexpprop` VALUES ('1', '2', '12', '1560', '51');
INSERT INTO `masterexpprop` VALUES ('1', '2', '13', '1640', '53');
INSERT INTO `masterexpprop` VALUES ('1', '2', '14', '1720', '55');
INSERT INTO `masterexpprop` VALUES ('1', '2', '15', '1800', '57');
INSERT INTO `masterexpprop` VALUES ('1', '2', '16', '1880', '59');
INSERT INTO `masterexpprop` VALUES ('1', '2', '17', '1960', '61');
INSERT INTO `masterexpprop` VALUES ('1', '2', '18', '2040', '63');
INSERT INTO `masterexpprop` VALUES ('1', '2', '19', '2120', '65');
INSERT INTO `masterexpprop` VALUES ('1', '2', '20', '2200', '67');
INSERT INTO `masterexpprop` VALUES ('1', '3', '1', '2580', '82');
INSERT INTO `masterexpprop` VALUES ('1', '3', '2', '2760', '85');
INSERT INTO `masterexpprop` VALUES ('1', '3', '3', '2940', '88');
INSERT INTO `masterexpprop` VALUES ('1', '3', '4', '3120', '91');
INSERT INTO `masterexpprop` VALUES ('1', '3', '5', '3300', '94');
INSERT INTO `masterexpprop` VALUES ('1', '3', '6', '3480', '97');
INSERT INTO `masterexpprop` VALUES ('1', '3', '7', '3660', '100');
INSERT INTO `masterexpprop` VALUES ('1', '3', '8', '3840', '103');
INSERT INTO `masterexpprop` VALUES ('1', '3', '9', '4020', '106');
INSERT INTO `masterexpprop` VALUES ('1', '3', '10', '4200', '109');
INSERT INTO `masterexpprop` VALUES ('1', '3', '11', '4380', '112');
INSERT INTO `masterexpprop` VALUES ('1', '3', '12', '4560', '115');
INSERT INTO `masterexpprop` VALUES ('1', '3', '13', '4740', '118');
INSERT INTO `masterexpprop` VALUES ('1', '3', '14', '4920', '121');
INSERT INTO `masterexpprop` VALUES ('1', '3', '15', '5100', '124');
INSERT INTO `masterexpprop` VALUES ('1', '3', '16', '5280', '127');
INSERT INTO `masterexpprop` VALUES ('1', '3', '17', '5460', '130');
INSERT INTO `masterexpprop` VALUES ('1', '3', '18', '5640', '133');
INSERT INTO `masterexpprop` VALUES ('1', '3', '19', '5820', '136');
INSERT INTO `masterexpprop` VALUES ('1', '3', '20', '6000', '139');
INSERT INTO `masterexpprop` VALUES ('1', '3', '21', '6180', '142');
INSERT INTO `masterexpprop` VALUES ('1', '3', '22', '6360', '145');
INSERT INTO `masterexpprop` VALUES ('1', '3', '23', '6540', '148');
INSERT INTO `masterexpprop` VALUES ('1', '3', '24', '6720', '151');
INSERT INTO `masterexpprop` VALUES ('1', '3', '25', '6900', '154');
INSERT INTO `masterexpprop` VALUES ('1', '4', '1', '8480', '174');
INSERT INTO `masterexpprop` VALUES ('1', '4', '2', '8960', '178');
INSERT INTO `masterexpprop` VALUES ('1', '4', '3', '9440', '182');
INSERT INTO `masterexpprop` VALUES ('1', '4', '4', '9920', '186');
INSERT INTO `masterexpprop` VALUES ('1', '4', '5', '10400', '190');
INSERT INTO `masterexpprop` VALUES ('1', '4', '6', '10880', '194');
INSERT INTO `masterexpprop` VALUES ('1', '4', '7', '11360', '198');
INSERT INTO `masterexpprop` VALUES ('1', '4', '8', '11840', '202');
INSERT INTO `masterexpprop` VALUES ('1', '4', '9', '12320', '206');
INSERT INTO `masterexpprop` VALUES ('1', '4', '10', '12800', '210');
INSERT INTO `masterexpprop` VALUES ('1', '4', '11', '13280', '214');
INSERT INTO `masterexpprop` VALUES ('1', '4', '12', '13760', '218');
INSERT INTO `masterexpprop` VALUES ('1', '4', '13', '14240', '222');
INSERT INTO `masterexpprop` VALUES ('1', '4', '14', '14720', '226');
INSERT INTO `masterexpprop` VALUES ('1', '4', '15', '15200', '230');
INSERT INTO `masterexpprop` VALUES ('1', '4', '16', '15680', '234');
INSERT INTO `masterexpprop` VALUES ('1', '4', '17', '16160', '238');
INSERT INTO `masterexpprop` VALUES ('1', '4', '18', '16640', '242');
INSERT INTO `masterexpprop` VALUES ('1', '4', '19', '17120', '246');
INSERT INTO `masterexpprop` VALUES ('1', '4', '20', '17600', '250');
INSERT INTO `masterexpprop` VALUES ('1', '4', '21', '18080', '254');
INSERT INTO `masterexpprop` VALUES ('1', '4', '22', '18560', '258');
INSERT INTO `masterexpprop` VALUES ('1', '4', '23', '19040', '262');
INSERT INTO `masterexpprop` VALUES ('1', '4', '24', '19520', '266');
INSERT INTO `masterexpprop` VALUES ('1', '4', '25', '20000', '270');
INSERT INTO `masterexpprop` VALUES ('1', '4', '26', '20480', '274');
INSERT INTO `masterexpprop` VALUES ('1', '4', '27', '20960', '278');
INSERT INTO `masterexpprop` VALUES ('1', '4', '28', '21440', '282');
INSERT INTO `masterexpprop` VALUES ('1', '4', '29', '21920', '286');
INSERT INTO `masterexpprop` VALUES ('1', '4', '30', '22400', '290');
INSERT INTO `masterexpprop` VALUES ('2', '1', '1', '220', '5');
INSERT INTO `masterexpprop` VALUES ('2', '1', '2', '240', '6');
INSERT INTO `masterexpprop` VALUES ('2', '1', '3', '260', '7');
INSERT INTO `masterexpprop` VALUES ('2', '1', '4', '280', '8');
INSERT INTO `masterexpprop` VALUES ('2', '1', '5', '300', '9');
INSERT INTO `masterexpprop` VALUES ('2', '1', '6', '320', '10');
INSERT INTO `masterexpprop` VALUES ('2', '1', '7', '340', '11');
INSERT INTO `masterexpprop` VALUES ('2', '1', '8', '360', '12');
INSERT INTO `masterexpprop` VALUES ('2', '1', '9', '380', '13');
INSERT INTO `masterexpprop` VALUES ('2', '1', '10', '400', '14');
INSERT INTO `masterexpprop` VALUES ('2', '1', '11', '420', '15');
INSERT INTO `masterexpprop` VALUES ('2', '1', '12', '440', '16');
INSERT INTO `masterexpprop` VALUES ('2', '1', '13', '460', '17');
INSERT INTO `masterexpprop` VALUES ('2', '1', '14', '480', '18');
INSERT INTO `masterexpprop` VALUES ('2', '1', '15', '500', '19');
INSERT INTO `masterexpprop` VALUES ('2', '2', '1', '680', '29');
INSERT INTO `masterexpprop` VALUES ('2', '2', '2', '760', '31');
INSERT INTO `masterexpprop` VALUES ('2', '2', '3', '840', '33');
INSERT INTO `masterexpprop` VALUES ('2', '2', '4', '920', '35');
INSERT INTO `masterexpprop` VALUES ('2', '2', '5', '1000', '37');
INSERT INTO `masterexpprop` VALUES ('2', '2', '6', '1080', '39');
INSERT INTO `masterexpprop` VALUES ('2', '2', '7', '1160', '41');
INSERT INTO `masterexpprop` VALUES ('2', '2', '8', '1240', '43');
INSERT INTO `masterexpprop` VALUES ('2', '2', '9', '1320', '45');
INSERT INTO `masterexpprop` VALUES ('2', '2', '10', '1400', '47');
INSERT INTO `masterexpprop` VALUES ('2', '2', '11', '1480', '49');
INSERT INTO `masterexpprop` VALUES ('2', '2', '12', '1560', '51');
INSERT INTO `masterexpprop` VALUES ('2', '2', '13', '1640', '53');
INSERT INTO `masterexpprop` VALUES ('2', '2', '14', '1720', '55');
INSERT INTO `masterexpprop` VALUES ('2', '2', '15', '1800', '57');
INSERT INTO `masterexpprop` VALUES ('2', '2', '16', '1880', '59');
INSERT INTO `masterexpprop` VALUES ('2', '2', '17', '1960', '61');
INSERT INTO `masterexpprop` VALUES ('2', '2', '18', '2040', '63');
INSERT INTO `masterexpprop` VALUES ('2', '2', '19', '2120', '65');
INSERT INTO `masterexpprop` VALUES ('2', '2', '20', '2200', '67');
INSERT INTO `masterexpprop` VALUES ('2', '3', '1', '2580', '82');
INSERT INTO `masterexpprop` VALUES ('2', '3', '2', '2760', '85');
INSERT INTO `masterexpprop` VALUES ('2', '3', '3', '2940', '88');
INSERT INTO `masterexpprop` VALUES ('2', '3', '4', '3120', '91');
INSERT INTO `masterexpprop` VALUES ('2', '3', '5', '3300', '94');
INSERT INTO `masterexpprop` VALUES ('2', '3', '6', '3480', '97');
INSERT INTO `masterexpprop` VALUES ('2', '3', '7', '3660', '100');
INSERT INTO `masterexpprop` VALUES ('2', '3', '8', '3840', '103');
INSERT INTO `masterexpprop` VALUES ('2', '3', '9', '4020', '106');
INSERT INTO `masterexpprop` VALUES ('2', '3', '10', '4200', '109');
INSERT INTO `masterexpprop` VALUES ('2', '3', '11', '4380', '112');
INSERT INTO `masterexpprop` VALUES ('2', '3', '12', '4560', '115');
INSERT INTO `masterexpprop` VALUES ('2', '3', '13', '4740', '118');
INSERT INTO `masterexpprop` VALUES ('2', '3', '14', '4920', '121');
INSERT INTO `masterexpprop` VALUES ('2', '3', '15', '5100', '124');
INSERT INTO `masterexpprop` VALUES ('2', '3', '16', '5280', '127');
INSERT INTO `masterexpprop` VALUES ('2', '3', '17', '5460', '130');
INSERT INTO `masterexpprop` VALUES ('2', '3', '18', '5640', '133');
INSERT INTO `masterexpprop` VALUES ('2', '3', '19', '5820', '136');
INSERT INTO `masterexpprop` VALUES ('2', '3', '20', '6000', '139');
INSERT INTO `masterexpprop` VALUES ('2', '3', '21', '6180', '142');
INSERT INTO `masterexpprop` VALUES ('2', '3', '22', '6360', '145');
INSERT INTO `masterexpprop` VALUES ('2', '3', '23', '6540', '148');
INSERT INTO `masterexpprop` VALUES ('2', '3', '24', '6720', '151');
INSERT INTO `masterexpprop` VALUES ('2', '3', '25', '6900', '154');
INSERT INTO `masterexpprop` VALUES ('2', '4', '1', '8480', '174');
INSERT INTO `masterexpprop` VALUES ('2', '4', '2', '8960', '178');
INSERT INTO `masterexpprop` VALUES ('2', '4', '3', '9440', '182');
INSERT INTO `masterexpprop` VALUES ('2', '4', '4', '9920', '186');
INSERT INTO `masterexpprop` VALUES ('2', '4', '5', '10400', '190');
INSERT INTO `masterexpprop` VALUES ('2', '4', '6', '10880', '194');
INSERT INTO `masterexpprop` VALUES ('2', '4', '7', '11360', '198');
INSERT INTO `masterexpprop` VALUES ('2', '4', '8', '11840', '202');
INSERT INTO `masterexpprop` VALUES ('2', '4', '9', '12320', '206');
INSERT INTO `masterexpprop` VALUES ('2', '4', '10', '12800', '210');
INSERT INTO `masterexpprop` VALUES ('2', '4', '11', '13280', '214');
INSERT INTO `masterexpprop` VALUES ('2', '4', '12', '13760', '218');
INSERT INTO `masterexpprop` VALUES ('2', '4', '13', '14240', '222');
INSERT INTO `masterexpprop` VALUES ('2', '4', '14', '14720', '226');
INSERT INTO `masterexpprop` VALUES ('2', '4', '15', '15200', '230');
INSERT INTO `masterexpprop` VALUES ('2', '4', '16', '15680', '234');
INSERT INTO `masterexpprop` VALUES ('2', '4', '17', '16160', '238');
INSERT INTO `masterexpprop` VALUES ('2', '4', '18', '16640', '242');
INSERT INTO `masterexpprop` VALUES ('2', '4', '19', '17120', '246');
INSERT INTO `masterexpprop` VALUES ('2', '4', '20', '17600', '250');
INSERT INTO `masterexpprop` VALUES ('2', '4', '21', '18080', '254');
INSERT INTO `masterexpprop` VALUES ('2', '4', '22', '18560', '258');
INSERT INTO `masterexpprop` VALUES ('2', '4', '23', '19040', '262');
INSERT INTO `masterexpprop` VALUES ('2', '4', '24', '19520', '266');
INSERT INTO `masterexpprop` VALUES ('2', '4', '25', '20000', '270');
INSERT INTO `masterexpprop` VALUES ('2', '4', '26', '20480', '274');
INSERT INTO `masterexpprop` VALUES ('2', '4', '27', '20960', '278');
INSERT INTO `masterexpprop` VALUES ('2', '4', '28', '21440', '282');
INSERT INTO `masterexpprop` VALUES ('2', '4', '29', '21920', '286');
INSERT INTO `masterexpprop` VALUES ('2', '4', '30', '22400', '290');
INSERT INTO `masterexpprop` VALUES ('3', '1', '1', '220', '5');
INSERT INTO `masterexpprop` VALUES ('3', '1', '2', '240', '6');
INSERT INTO `masterexpprop` VALUES ('3', '1', '3', '260', '7');
INSERT INTO `masterexpprop` VALUES ('3', '1', '4', '280', '8');
INSERT INTO `masterexpprop` VALUES ('3', '1', '5', '300', '9');
INSERT INTO `masterexpprop` VALUES ('3', '1', '6', '320', '10');
INSERT INTO `masterexpprop` VALUES ('3', '1', '7', '340', '11');
INSERT INTO `masterexpprop` VALUES ('3', '1', '8', '360', '12');
INSERT INTO `masterexpprop` VALUES ('3', '1', '9', '380', '13');
INSERT INTO `masterexpprop` VALUES ('3', '1', '10', '400', '14');
INSERT INTO `masterexpprop` VALUES ('3', '1', '11', '420', '15');
INSERT INTO `masterexpprop` VALUES ('3', '1', '12', '440', '16');
INSERT INTO `masterexpprop` VALUES ('3', '1', '13', '460', '17');
INSERT INTO `masterexpprop` VALUES ('3', '1', '14', '480', '18');
INSERT INTO `masterexpprop` VALUES ('3', '1', '15', '500', '19');
INSERT INTO `masterexpprop` VALUES ('3', '2', '1', '680', '29');
INSERT INTO `masterexpprop` VALUES ('3', '2', '2', '760', '31');
INSERT INTO `masterexpprop` VALUES ('3', '2', '3', '840', '33');
INSERT INTO `masterexpprop` VALUES ('3', '2', '4', '920', '35');
INSERT INTO `masterexpprop` VALUES ('3', '2', '5', '1000', '37');
INSERT INTO `masterexpprop` VALUES ('3', '2', '6', '1080', '39');
INSERT INTO `masterexpprop` VALUES ('3', '2', '7', '1160', '41');
INSERT INTO `masterexpprop` VALUES ('3', '2', '8', '1240', '43');
INSERT INTO `masterexpprop` VALUES ('3', '2', '9', '1320', '45');
INSERT INTO `masterexpprop` VALUES ('3', '2', '10', '1400', '47');
INSERT INTO `masterexpprop` VALUES ('3', '2', '11', '1480', '49');
INSERT INTO `masterexpprop` VALUES ('3', '2', '12', '1560', '51');
INSERT INTO `masterexpprop` VALUES ('3', '2', '13', '1640', '53');
INSERT INTO `masterexpprop` VALUES ('3', '2', '14', '1720', '55');
INSERT INTO `masterexpprop` VALUES ('3', '2', '15', '1800', '57');
INSERT INTO `masterexpprop` VALUES ('3', '2', '16', '1880', '59');
INSERT INTO `masterexpprop` VALUES ('3', '2', '17', '1960', '61');
INSERT INTO `masterexpprop` VALUES ('3', '2', '18', '2040', '63');
INSERT INTO `masterexpprop` VALUES ('3', '2', '19', '2120', '65');
INSERT INTO `masterexpprop` VALUES ('3', '2', '20', '2200', '67');
INSERT INTO `masterexpprop` VALUES ('3', '3', '1', '2580', '82');
INSERT INTO `masterexpprop` VALUES ('3', '3', '2', '2760', '85');
INSERT INTO `masterexpprop` VALUES ('3', '3', '3', '2940', '88');
INSERT INTO `masterexpprop` VALUES ('3', '3', '4', '3120', '91');
INSERT INTO `masterexpprop` VALUES ('3', '3', '5', '3300', '94');
INSERT INTO `masterexpprop` VALUES ('3', '3', '6', '3480', '97');
INSERT INTO `masterexpprop` VALUES ('3', '3', '7', '3660', '100');
INSERT INTO `masterexpprop` VALUES ('3', '3', '8', '3840', '103');
INSERT INTO `masterexpprop` VALUES ('3', '3', '9', '4020', '106');
INSERT INTO `masterexpprop` VALUES ('3', '3', '10', '4200', '109');
INSERT INTO `masterexpprop` VALUES ('3', '3', '11', '4380', '112');
INSERT INTO `masterexpprop` VALUES ('3', '3', '12', '4560', '115');
INSERT INTO `masterexpprop` VALUES ('3', '3', '13', '4740', '118');
INSERT INTO `masterexpprop` VALUES ('3', '3', '14', '4920', '121');
INSERT INTO `masterexpprop` VALUES ('3', '3', '15', '5100', '124');
INSERT INTO `masterexpprop` VALUES ('3', '3', '16', '5280', '127');
INSERT INTO `masterexpprop` VALUES ('3', '3', '17', '5460', '130');
INSERT INTO `masterexpprop` VALUES ('3', '3', '18', '5640', '133');
INSERT INTO `masterexpprop` VALUES ('3', '3', '19', '5820', '136');
INSERT INTO `masterexpprop` VALUES ('3', '3', '20', '6000', '139');
INSERT INTO `masterexpprop` VALUES ('3', '3', '21', '6180', '142');
INSERT INTO `masterexpprop` VALUES ('3', '3', '22', '6360', '145');
INSERT INTO `masterexpprop` VALUES ('3', '3', '23', '6540', '148');
INSERT INTO `masterexpprop` VALUES ('3', '3', '24', '6720', '151');
INSERT INTO `masterexpprop` VALUES ('3', '3', '25', '6900', '154');
INSERT INTO `masterexpprop` VALUES ('3', '4', '1', '8480', '174');
INSERT INTO `masterexpprop` VALUES ('3', '4', '2', '8960', '178');
INSERT INTO `masterexpprop` VALUES ('3', '4', '3', '9440', '182');
INSERT INTO `masterexpprop` VALUES ('3', '4', '4', '9920', '186');
INSERT INTO `masterexpprop` VALUES ('3', '4', '5', '10400', '190');
INSERT INTO `masterexpprop` VALUES ('3', '4', '6', '10880', '194');
INSERT INTO `masterexpprop` VALUES ('3', '4', '7', '11360', '198');
INSERT INTO `masterexpprop` VALUES ('3', '4', '8', '11840', '202');
INSERT INTO `masterexpprop` VALUES ('3', '4', '9', '12320', '206');
INSERT INTO `masterexpprop` VALUES ('3', '4', '10', '12800', '210');
INSERT INTO `masterexpprop` VALUES ('3', '4', '11', '13280', '214');
INSERT INTO `masterexpprop` VALUES ('3', '4', '12', '13760', '218');
INSERT INTO `masterexpprop` VALUES ('3', '4', '13', '14240', '222');
INSERT INTO `masterexpprop` VALUES ('3', '4', '14', '14720', '226');
INSERT INTO `masterexpprop` VALUES ('3', '4', '15', '15200', '230');
INSERT INTO `masterexpprop` VALUES ('3', '4', '16', '15680', '234');
INSERT INTO `masterexpprop` VALUES ('3', '4', '17', '16160', '238');
INSERT INTO `masterexpprop` VALUES ('3', '4', '18', '16640', '242');
INSERT INTO `masterexpprop` VALUES ('3', '4', '19', '17120', '246');
INSERT INTO `masterexpprop` VALUES ('3', '4', '20', '17600', '250');
INSERT INTO `masterexpprop` VALUES ('3', '4', '21', '18080', '254');
INSERT INTO `masterexpprop` VALUES ('3', '4', '22', '18560', '258');
INSERT INTO `masterexpprop` VALUES ('3', '4', '23', '19040', '262');
INSERT INTO `masterexpprop` VALUES ('3', '4', '24', '19520', '266');
INSERT INTO `masterexpprop` VALUES ('3', '4', '25', '20000', '270');
INSERT INTO `masterexpprop` VALUES ('3', '4', '26', '20480', '274');
INSERT INTO `masterexpprop` VALUES ('3', '4', '27', '20960', '278');
INSERT INTO `masterexpprop` VALUES ('3', '4', '28', '21440', '282');
INSERT INTO `masterexpprop` VALUES ('3', '4', '29', '21920', '286');
INSERT INTO `masterexpprop` VALUES ('3', '4', '30', '22400', '290');
INSERT INTO `masterexpprop` VALUES ('4', '1', '1', '220', '5');
INSERT INTO `masterexpprop` VALUES ('4', '1', '2', '240', '6');
INSERT INTO `masterexpprop` VALUES ('4', '1', '3', '260', '7');
INSERT INTO `masterexpprop` VALUES ('4', '1', '4', '280', '8');
INSERT INTO `masterexpprop` VALUES ('4', '1', '5', '300', '9');
INSERT INTO `masterexpprop` VALUES ('4', '1', '6', '320', '10');
INSERT INTO `masterexpprop` VALUES ('4', '1', '7', '340', '11');
INSERT INTO `masterexpprop` VALUES ('4', '1', '8', '360', '12');
INSERT INTO `masterexpprop` VALUES ('4', '1', '9', '380', '13');
INSERT INTO `masterexpprop` VALUES ('4', '1', '10', '400', '14');
INSERT INTO `masterexpprop` VALUES ('4', '1', '11', '420', '15');
INSERT INTO `masterexpprop` VALUES ('4', '1', '12', '440', '16');
INSERT INTO `masterexpprop` VALUES ('4', '1', '13', '460', '17');
INSERT INTO `masterexpprop` VALUES ('4', '1', '14', '480', '18');
INSERT INTO `masterexpprop` VALUES ('4', '1', '15', '500', '19');
INSERT INTO `masterexpprop` VALUES ('4', '2', '1', '680', '29');
INSERT INTO `masterexpprop` VALUES ('4', '2', '2', '760', '31');
INSERT INTO `masterexpprop` VALUES ('4', '2', '3', '840', '33');
INSERT INTO `masterexpprop` VALUES ('4', '2', '4', '920', '35');
INSERT INTO `masterexpprop` VALUES ('4', '2', '5', '1000', '37');
INSERT INTO `masterexpprop` VALUES ('4', '2', '6', '1080', '39');
INSERT INTO `masterexpprop` VALUES ('4', '2', '7', '1160', '41');
INSERT INTO `masterexpprop` VALUES ('4', '2', '8', '1240', '43');
INSERT INTO `masterexpprop` VALUES ('4', '2', '9', '1320', '45');
INSERT INTO `masterexpprop` VALUES ('4', '2', '10', '1400', '47');
INSERT INTO `masterexpprop` VALUES ('4', '2', '11', '1480', '49');
INSERT INTO `masterexpprop` VALUES ('4', '2', '12', '1560', '51');
INSERT INTO `masterexpprop` VALUES ('4', '2', '13', '1640', '53');
INSERT INTO `masterexpprop` VALUES ('4', '2', '14', '1720', '55');
INSERT INTO `masterexpprop` VALUES ('4', '2', '15', '1800', '57');
INSERT INTO `masterexpprop` VALUES ('4', '2', '16', '1880', '59');
INSERT INTO `masterexpprop` VALUES ('4', '2', '17', '1960', '61');
INSERT INTO `masterexpprop` VALUES ('4', '2', '18', '2040', '63');
INSERT INTO `masterexpprop` VALUES ('4', '2', '19', '2120', '65');
INSERT INTO `masterexpprop` VALUES ('4', '2', '20', '2200', '67');
INSERT INTO `masterexpprop` VALUES ('4', '3', '1', '2580', '82');
INSERT INTO `masterexpprop` VALUES ('4', '3', '2', '2760', '85');
INSERT INTO `masterexpprop` VALUES ('4', '3', '3', '2940', '88');
INSERT INTO `masterexpprop` VALUES ('4', '3', '4', '3120', '91');
INSERT INTO `masterexpprop` VALUES ('4', '3', '5', '3300', '94');
INSERT INTO `masterexpprop` VALUES ('4', '3', '6', '3480', '97');
INSERT INTO `masterexpprop` VALUES ('4', '3', '7', '3660', '100');
INSERT INTO `masterexpprop` VALUES ('4', '3', '8', '3840', '103');
INSERT INTO `masterexpprop` VALUES ('4', '3', '9', '4020', '106');
INSERT INTO `masterexpprop` VALUES ('4', '3', '10', '4200', '109');
INSERT INTO `masterexpprop` VALUES ('4', '3', '11', '4380', '112');
INSERT INTO `masterexpprop` VALUES ('4', '3', '12', '4560', '115');
INSERT INTO `masterexpprop` VALUES ('4', '3', '13', '4740', '118');
INSERT INTO `masterexpprop` VALUES ('4', '3', '14', '4920', '121');
INSERT INTO `masterexpprop` VALUES ('4', '3', '15', '5100', '124');
INSERT INTO `masterexpprop` VALUES ('4', '3', '16', '5280', '127');
INSERT INTO `masterexpprop` VALUES ('4', '3', '17', '5460', '130');
INSERT INTO `masterexpprop` VALUES ('4', '3', '18', '5640', '133');
INSERT INTO `masterexpprop` VALUES ('4', '3', '19', '5820', '136');
INSERT INTO `masterexpprop` VALUES ('4', '3', '20', '6000', '139');
INSERT INTO `masterexpprop` VALUES ('4', '3', '21', '6180', '142');
INSERT INTO `masterexpprop` VALUES ('4', '3', '22', '6360', '145');
INSERT INTO `masterexpprop` VALUES ('4', '3', '23', '6540', '148');
INSERT INTO `masterexpprop` VALUES ('4', '3', '24', '6720', '151');
INSERT INTO `masterexpprop` VALUES ('4', '3', '25', '6900', '154');
INSERT INTO `masterexpprop` VALUES ('4', '4', '1', '8480', '174');
INSERT INTO `masterexpprop` VALUES ('4', '4', '2', '8960', '178');
INSERT INTO `masterexpprop` VALUES ('4', '4', '3', '9440', '182');
INSERT INTO `masterexpprop` VALUES ('4', '4', '4', '9920', '186');
INSERT INTO `masterexpprop` VALUES ('4', '4', '5', '10400', '190');
INSERT INTO `masterexpprop` VALUES ('4', '4', '6', '10880', '194');
INSERT INTO `masterexpprop` VALUES ('4', '4', '7', '11360', '198');
INSERT INTO `masterexpprop` VALUES ('4', '4', '8', '11840', '202');
INSERT INTO `masterexpprop` VALUES ('4', '4', '9', '12320', '206');
INSERT INTO `masterexpprop` VALUES ('4', '4', '10', '12800', '210');
INSERT INTO `masterexpprop` VALUES ('4', '4', '11', '13280', '214');
INSERT INTO `masterexpprop` VALUES ('4', '4', '12', '13760', '218');
INSERT INTO `masterexpprop` VALUES ('4', '4', '13', '14240', '222');
INSERT INTO `masterexpprop` VALUES ('4', '4', '14', '14720', '226');
INSERT INTO `masterexpprop` VALUES ('4', '4', '15', '15200', '230');
INSERT INTO `masterexpprop` VALUES ('4', '4', '16', '15680', '234');
INSERT INTO `masterexpprop` VALUES ('4', '4', '17', '16160', '238');
INSERT INTO `masterexpprop` VALUES ('4', '4', '18', '16640', '242');
INSERT INTO `masterexpprop` VALUES ('4', '4', '19', '17120', '246');
INSERT INTO `masterexpprop` VALUES ('4', '4', '20', '17600', '250');
INSERT INTO `masterexpprop` VALUES ('4', '4', '21', '18080', '254');
INSERT INTO `masterexpprop` VALUES ('4', '4', '22', '18560', '258');
INSERT INTO `masterexpprop` VALUES ('4', '4', '23', '19040', '262');
INSERT INTO `masterexpprop` VALUES ('4', '4', '24', '19520', '266');
INSERT INTO `masterexpprop` VALUES ('4', '4', '25', '20000', '270');
INSERT INTO `masterexpprop` VALUES ('4', '4', '26', '20480', '274');
INSERT INTO `masterexpprop` VALUES ('4', '4', '27', '20960', '278');
INSERT INTO `masterexpprop` VALUES ('4', '4', '28', '21440', '282');
INSERT INTO `masterexpprop` VALUES ('4', '4', '29', '21920', '286');
INSERT INTO `masterexpprop` VALUES ('4', '4', '30', '22400', '290');

-- ----------------------------
-- Table structure for masterextraexpprop
-- ----------------------------
DROP TABLE IF EXISTS `masterextraexpprop`;
CREATE TABLE `masterextraexpprop` (
  `BigExpBookCnt` tinyint(4) NOT NULL DEFAULT '0' COMMENT '装备精通大经验书数量',
  `ExtraExp` int(11) NOT NULL DEFAULT '0' COMMENT '额外获得的经验',
  PRIMARY KEY (`BigExpBookCnt`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of masterextraexpprop
-- ----------------------------
INSERT INTO `masterextraexpprop` VALUES ('1', '0');
INSERT INTO `masterextraexpprop` VALUES ('2', '0');
INSERT INTO `masterextraexpprop` VALUES ('3', '0');
INSERT INTO `masterextraexpprop` VALUES ('4', '0');
INSERT INTO `masterextraexpprop` VALUES ('5', '50');
INSERT INTO `masterextraexpprop` VALUES ('6', '70');
INSERT INTO `masterextraexpprop` VALUES ('7', '100');
INSERT INTO `masterextraexpprop` VALUES ('8', '140');
INSERT INTO `masterextraexpprop` VALUES ('9', '190');
INSERT INTO `masterextraexpprop` VALUES ('10', '250');
INSERT INTO `masterextraexpprop` VALUES ('11', '320');
INSERT INTO `masterextraexpprop` VALUES ('12', '400');

-- ----------------------------
-- Table structure for masterrateprop
-- ----------------------------
DROP TABLE IF EXISTS `masterrateprop`;
CREATE TABLE `masterrateprop` (
  `EquipKind` tinyint(4) NOT NULL DEFAULT '0' COMMENT '精通类别（头盔1、战袍2、武器3、战靴4）',
  `BigExpBookCnt` tinyint(4) NOT NULL DEFAULT '0' COMMENT '大精通经验书数量',
  `Rate` tinyint(4) NOT NULL DEFAULT '0' COMMENT '概率，值10表示10%',
  `Exp` int(11) NOT NULL DEFAULT '0' COMMENT '加多少经验',
  PRIMARY KEY (`EquipKind`,`BigExpBookCnt`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of masterrateprop
-- ----------------------------
INSERT INTO `masterrateprop` VALUES ('1', '0', '30', '30');
INSERT INTO `masterrateprop` VALUES ('1', '1', '60', '120');
INSERT INTO `masterrateprop` VALUES ('1', '2', '10', '210');
INSERT INTO `masterrateprop` VALUES ('1', '3', '0', '300');
INSERT INTO `masterrateprop` VALUES ('2', '0', '30', '30');
INSERT INTO `masterrateprop` VALUES ('2', '1', '60', '120');
INSERT INTO `masterrateprop` VALUES ('2', '2', '10', '210');
INSERT INTO `masterrateprop` VALUES ('2', '3', '0', '300');
INSERT INTO `masterrateprop` VALUES ('3', '0', '30', '30');
INSERT INTO `masterrateprop` VALUES ('3', '1', '60', '120');
INSERT INTO `masterrateprop` VALUES ('3', '2', '10', '210');
INSERT INTO `masterrateprop` VALUES ('3', '3', '0', '300');
INSERT INTO `masterrateprop` VALUES ('4', '0', '30', '30');
INSERT INTO `masterrateprop` VALUES ('4', '1', '60', '120');
INSERT INTO `masterrateprop` VALUES ('4', '2', '10', '210');
INSERT INTO `masterrateprop` VALUES ('4', '3', '0', '300');