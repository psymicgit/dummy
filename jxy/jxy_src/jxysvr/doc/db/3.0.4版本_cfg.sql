/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50613
Source Host           : 127.0.0.1:3306
Source Database       : ios_app_cfg

Target Server Type    : MYSQL
Target Server Version : 50613
File Encoding         : 65001

Date: 2014-02-27 10:19:01
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
INSERT INTO `formationprop2` VALUES ('1', '1', '0', '0', '0');
INSERT INTO `formationprop2` VALUES ('1', '3', '0', '0', '16');
INSERT INTO `formationprop2` VALUES ('1', '4', '1', '0', '0');
INSERT INTO `formationprop2` VALUES ('1', '6', '0', '0', '0');
INSERT INTO `formationprop2` VALUES ('2', '1', '0', '0', '0');
INSERT INTO `formationprop2` VALUES ('2', '3', '1', '0', '0');
INSERT INTO `formationprop2` VALUES ('2', '4', '0', '0', '0');
INSERT INTO `formationprop2` VALUES ('2', '6', '0', '0', '16');
INSERT INTO `formationprop2` VALUES ('3', '1', '0', '0', '0');
INSERT INTO `formationprop2` VALUES ('3', '3', '1', '0', '0');
INSERT INTO `formationprop2` VALUES ('3', '4', '0', '0', '0');
INSERT INTO `formationprop2` VALUES ('3', '6', '0', '0', '16');

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
INSERT INTO `formationunlock` VALUES ('3', '1', '30');

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
  `NeedPlayerLevel` smallint(5) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of formationupgradeprop
-- ----------------------------
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '1', '100', '22000', '5');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '2', '200', '24000', '10');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '3', '300', '27000', '15');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '4', '400', '31000', '20');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '5', '500', '35000', '25');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '6', '600', '39000', '30');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '7', '700', '43000', '35');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '8', '800', '48000', '40');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '9', '900', '54000', '45');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '10', '1000', '60000', '50');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '11', '1100', '66000', '55');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '12', '1200', '72000', '60');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '13', '1300', '79000', '65');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '14', '1400', '87000', '70');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '15', '1500', '95000', '75');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '16', '1600', '103000', '80');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '17', '1700', '111000', '85');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '18', '1800', '120000', '90');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '19', '1900', '130000', '95');
INSERT INTO `formationupgradeprop` VALUES ('1', '白虎阵法', '2', '20', '2000', '0', '100');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '1', '500', '22000', '5');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '2', '1000', '24000', '10');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '3', '1500', '27000', '15');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '4', '2000', '31000', '20');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '5', '2500', '35000', '25');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '6', '3000', '39000', '30');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '7', '3500', '43000', '35');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '8', '4000', '48000', '40');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '9', '4500', '54000', '45');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '10', '5000', '60000', '50');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '11', '5500', '66000', '55');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '12', '6000', '72000', '60');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '13', '6500', '79000', '65');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '14', '7000', '87000', '70');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '15', '7500', '95000', '75');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '16', '8000', '103000', '80');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '17', '8500', '111000', '85');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '18', '9000', '120000', '90');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '19', '9500', '130000', '95');
INSERT INTO `formationupgradeprop` VALUES ('2', '青龙阵法', '1', '20', '10000', '0', '100');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '1', '500', '22000', '5');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '2', '1000', '24000', '10');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '3', '1500', '27000', '15');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '4', '2000', '31000', '20');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '5', '2500', '35000', '25');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '6', '3000', '39000', '30');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '7', '3500', '43000', '35');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '8', '4000', '48000', '40');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '9', '4500', '54000', '45');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '10', '5000', '60000', '50');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '11', '5500', '66000', '55');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '12', '6000', '72000', '60');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '13', '6500', '79000', '65');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '14', '7000', '87000', '70');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '15', '7500', '95000', '75');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '16', '8000', '103000', '80');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '17', '8500', '111000', '85');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '18', '9000', '120000', '90');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '19', '9500', '130000', '95');
INSERT INTO `formationupgradeprop` VALUES ('3', '朱雀阵法', '11', '20', '10000', '0', '100');

-- ----------------------------
-- Table structure for `luafile`
-- ----------------------------
DROP TABLE IF EXISTS `luafile`;
CREATE TABLE `luafile` (
  `Common1` text NOT NULL,
  `Common2` text NOT NULL,
  `Common3` text NOT NULL,
  `Common4` text NOT NULL,
  `Common5` text NOT NULL,
  `Calculate` text NOT NULL,
  `Activity` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of luafile
-- ----------------------------
INSERT INTO `luafile` VALUES ('', '', '', '', '', 'ECK_ATTACK=1; -- 攻击\r\nECK_DEFENCE=2; -- 防御\r\nEAST_HP=0; --血量\r\nEAST_ATTACK=1; --攻击\r\n\r\n-- 强化铜币\r\nfunction GetStrengthenCoin(StrenthenLevel, DressLevel)\r\n	Coin = 0;\r\n	Type = DressLevel / 10;\r\n	if Type < 1 then\r\n		Coin = StrenthenLevel*StrenthenLevel*50+StrenthenLevel*50+50;\r\n	elseif Type < 3 then\r\n		Coin = StrenthenLevel*StrenthenLevel*60+StrenthenLevel*60+1060;\r\n	elseif Type < 5 then\r\n		Coin = StrenthenLevel*StrenthenLevel*72+StrenthenLevel*72+6272;\r\n	elseif Type < 7 then\r\n		Coin = StrenthenLevel*StrenthenLevel*86+StrenthenLevel*86+17526;\r\n	elseif Type < 8 then\r\n		Coin = StrenthenLevel*StrenthenLevel*130+StrenthenLevel*130+46289;\r\n	elseif Type < 9 then\r\n		Coin = StrenthenLevel*StrenthenLevel*195+StrenthenLevel*195+114933;\r\n	elseif Type < 10 then\r\n		Coin = StrenthenLevel*StrenthenLevel*292+StrenthenLevel*292+272339;\r\n	elseif Type < 11 then\r\n		Coin = StrenthenLevel*StrenthenLevel*438+StrenthenLevel*438+408058;\r\n	else\r\n		Coin = StrenthenLevel*StrenthenLevel*438+StrenthenLevel*438+408058;\r\n	end;\r\n\r\n	return Coin;\r\nend\r\n\r\n-- 俘虏工作生产铜币\r\nfunction GetCaptureWorkCoin(CaptureLevel)\r\n	Coin = (2000 * CaptureLevel) + 5000;\r\n	return Coin;\r\nend\r\n-- 俘虏工作生产器魂\r\nfunction GetCaptureWorkScience(CaptureLevel)\r\n	Science = 16 * CaptureLevel;\r\n	return Science;\r\nend\r\n\r\n-- 排名奖励——铜币\r\nfunction GetRankEncourageCoin(Rank)\r\n	Coin = 0;\r\n	if Rank == 0 then\r\n		Coin = 30000;\r\n	elseif Rank <= 50 then\r\n		Coin = 500000 - 5000 * (Rank-1);\r\n	elseif Rank <= 100 then\r\n		Coin = 250000 - 1000 * (Rank - 50);\r\n    elseif Rank <= 200 then\r\n        Coin = 200000 - 500 * (Rank - 100);\r\n    elseif Rank <= 500 then\r\n		Coin = 150000 - 200 * (Rank - 200);\r\n    elseif Rank <= 1000 then\r\n        Coin = 90000 - 100 * (Rank - 500);\r\n    else\r\n        Coin = 30000;\r\n	end;\r\n\r\n	return Coin;\r\nend\r\n-- 排名奖励——器魂\r\nfunction GetRankEncourageScience(Rank)\r\n	Science = 0;\r\n	if Rank == 0 then\r\n		Science = 300;\r\n	elseif Rank <= 50 then\r\n		Science = 5000 - 50 * (Rank-1);\r\n	elseif Rank <= 100 then\r\n		Science = 2500 - 10 * (Rank - 50);\r\n    elseif Rank <= 200 then\r\n        Science = 2000 - 5 * (Rank - 100);\r\n    elseif Rank <= 500 then\r\n		Science = 1500 - 2 * (Rank - 200);\r\n    elseif Rank <= 1000 then\r\n        Science = 900 - 1 * (Rank - 500);\r\n    else\r\n        Science = 300;\r\n	end;\r\n\r\n	--  return Science;\r\n	return 0;\r\nend\r\n\r\n-- 排名奖励——境界\r\nfunction GetRankEncourageJingJie(Rank)\r\n	JingJie = 0;\r\n	if Rank == 0 then\r\n		JingJie  = 300;\r\n	elseif Rank <= 50 then\r\n		JingJie  = 5000 - 50 * (Rank-1);\r\n	elseif Rank <= 100 then\r\n		JingJie  = 2500 - 10 * (Rank - 50);\r\n    elseif Rank <= 200 then\r\n        JingJie  = 2000 - 5 * (Rank - 100);\r\n    elseif Rank <= 500 then\r\n		JingJie = 1500 - 2 * (Rank - 200);\r\n    elseif Rank <= 1000 then\r\n        JingJie  = 900 - 1 * (Rank - 500);\r\n    else\r\n        JingJie  = 300;\r\n	end;\r\n\r\n	return JingJie ;\r\nend\r\n\r\n\r\n-- 挑战奖励——铜币\r\nfunction GetChallengeEncourageCoin(PlayerLevel, WinFlag)\r\n	Coin = 0;\r\n	if WinFlag == true then\r\n		Coin = PlayerLevel * 100 + 1000\r\n	else\r\n		Coin = (PlayerLevel * 100 + 1000) / 2;\r\n	end;\r\n\r\n	return Coin;\r\nend\r\n-- 挑战奖励——器魂\r\nfunction GetChallengeEncourageScience(PlayerLevel, WinFlag)\r\n	Science = 0;\r\n	if WinFlag == 1 then\r\n		Science = PlayerLevel + 100\r\n	else\r\n		Science = (PlayerLevel + 100) / 2;\r\n	end;\r\n\r\n	-- return Science;\r\n	return 0;\r\nend\r\n-- 挑战奖励——境界\r\nfunction GetChallengeEncourageJingJie(PlayerLevel, WinFlag)\r\n	JingJie = 0;\r\n	if WinFlag == 1 then\r\n		JingJie = 2 * (PlayerLevel * PlayerLevel * 0.005 + PlayerLevel * 0.05 + 10);\r\n	else\r\n		JingJie = PlayerLevel * PlayerLevel * 0.005 + PlayerLevel * 0.05 + 10;\r\n	end;\r\n\r\n	return JingJie;\r\nend\r\n\r\n\r\n-- 摇钱树产出——铜币\r\nfunction GetMoneyTreeProduceCoin(PlayerLevel)\r\n	Coin =  PlayerLevel * 2000 + 5000 ;\r\n	return Coin;\r\nend\r\n\r\n\r\n\r\n-- 属性升级——增加的属性值\r\nfunction GetAttrAddValue(Talent, HeroLevel, AttrType, CareerType)\r\n	AddValue = 0;\r\n    if ECK_DEFENCE == CareerType then\r\n        if AttrType == EAST_ATTACK then\r\n            AddValue = (HeroLevel * Talent * 1.5 + (HeroLevel * Talent * 7.5) / 5 ) * 0.4;\r\n        elseif AttrType == EAST_HP then\r\n            AddValue = ( HeroLevel * Talent * 7.5 + HeroLevel * Talent * 1.5 * 5 ) * 0.6;\r\n        else\r\n            AddValue = 0;\r\n        end;\r\n    elseif ECK_ATTACK == CareerType then\r\n        if EAST_ATTACK == AttrType then\r\n            AddValue = ( HeroLevel * Talent * 1.5 + ( HeroLevel * Talent * 7.5 / 5 )) * 0.6;\r\n        elseif EAST_HP == AttrType then\r\n            AddValue = ( HeroLevel * Talent * 7.5 + HeroLevel * Talent * 1.5 * 5 ) * 0.4;\r\n        else\r\n            AddValue = 0;\r\n		end;\r\n    else\r\n        AddValue = 0;\r\n    end;\r\n\r\n	return AddValue;\r\nend\r\n\r\n-- 属性升级——消耗铜币\r\nfunction GetUpdateAttrCostCoin(Talent, AttrLevel, CoachFlag)\r\n	CostCoinValue = 0;\r\n	CostCoinOriValue = AttrLevel * AttrLevel  + AttrLevel * 400 + 1000;\r\n	if CoachFlag == true then\r\n		CostCoinValue = (CostCoinOriValue * 1 + CostCoinOriValue * 2.5 ) / 4.0;\r\n	else\r\n	    if Talent <= 10 then\r\n            CostCoinValue = CostCoinOriValue * 1;\r\n        elseif Talent <= 20 then\r\n            CostCoinValue = CostCoinOriValue * 1.5;\r\n        elseif Talent <= 30 then\r\n            CostCoinValue = CostCoinOriValue * 2.0;\r\n        elseif Talent <= 40 then\r\n            CostCoinValue = CostCoinOriValue * 2.5;\r\n        else\r\n			dwCostCoinValue = CostCoinOriValue * 3;\r\n		end;\r\n	end;\r\n\r\n	return CostCoinValue;\r\nend\r\n\r\n\r\n\r\nEGCF_MONEYTREE = 1 						--招财\r\nEGCF_RACEBUYNUM = 2 					--竞技场购买挑战次数\r\nEGCF_PLUNDERBUYNUM = 3 				--掠夺购买挑战次数\r\nEGCF_PHYSTRENGTHBUYNUM = 4		--购买体力次数\r\nEGCF_CLIMBTOWERRESET = 5			--爬塔每天可免费重置次数\r\n--EGCF_GENRANDOMGAS=6					--随机聚气\r\n--EGCF_HEROEATJINGYANDAN = 7	--伙伴服用经验丹\r\nEGCF_RESETELITEINSTANCE = 8		--重置精英副本\r\n--EGCF_GENPURPLEGAS = 9				--紫色聚气\r\n--EGCF_AWAKENCALL = 11				--悟道召唤\r\nEGCF_GATHER_SCIENCE_COIN = 12 --铜钱聚魂\r\nEGCF_GATHER_SCIENCE_GOLD = 13 --元宝聚魂\r\nEGCF_BEG_BLUE_IMPRV_LUCK = 14 -- 求仙露改运消费\r\nEGCF_SHADE_CHANGE_MONSTER = 15 --暗部换怪\r\nEGCF_FACTION_OPEN_GOLD = 17 ---开启黄金模式\r\nEGCF_FACTION_CLOSE_MONSTER = 18 --收妖\r\nEGCF_FACTION_CREATE_BY_COIN = 19 --铜钱创建门派\r\nEGCF_FACTION_CREATE_BY_GOLD = 20 --元宝创建门派\r\n\r\nEGCF_PLUNDERDRIVEAWAY = 101		--驱逐\r\nEGCF_GENCOIN = 102 						--聚魂铜钱\r\nEGCF_GENGOLD = 103 						--聚魂元宝\r\nEGCF_GENUPGOLD = 104 					--聚魂元宝随机\r\nEGCF_GENUPGOLDFULL = 105 			--聚魂元宝加满\r\n\r\n\r\n\r\n\r\n\r\n--获得元宝消耗\r\n\r\nfunction GetCostByFunc(byVipLevel, nFunc, nCnt, nIdx )\r\n	if nFunc==EGCF_MONEYTREE then -- 招财\r\n		if byVipLevel < 5 then\r\n			return byVipLevel * 3 + 2\r\n		else\r\n			return ( byVipLevel - 4) * 5 + 15\r\n		end\r\n	elseif nFunc == EGCF_RACEBUYNUM then --竞技场购买挑战次数\r\n		if byVipLevel < 5 then\r\n			return byVipLevel * 3 + 5\r\n		else\r\n			return ( byVipLevel - 4) * 5 + 15\r\n		end\r\n	elseif nFunc == EGCF_PLUNDERBUYNUM then --竞技场挑战次数\r\n		if byVipLevel < 5 then\r\n			return byVipLevel * 3 + 5\r\n		else\r\n			return ( byVipLevel - 4) * 5 + 15\r\n		end\r\n	elseif nFunc == EGCF_PLUNDERDRIVEAWAY then --驱赶\r\n		if nCnt>=0 and nCnt<6 then\r\n			return 10+nCnt*2\r\n		elseif nCnt>=6 and nCnt<10 then\r\n			return (nCnt-5)*5+20\r\n		elseif nCnt>=10 then\r\n			return 50\r\n		end\r\n\r\n  elseif nFunc==EGCF_PHYSTRENGTHBUYNUM then -- 体力\r\n	if byVipLevel < 5 then\r\n		  return byVipLevel * 5 + 20\r\n	elseif byVipLevel <= 10 then\r\n	    return ( byVipLevel - 4 ) * 10 + 40\r\n    end\r\n\r\n	elseif nFunc==EGCF_CLIMBTOWERRESET then --爬塔\r\n		if byVipLevel < 4 then\r\n			return 0\r\n		else\r\n			return (byVipLevel - 2) * nIdx\r\n		end\r\n	elseif nFunc == EGCF_RESETELITEINSTANCE then --精英\r\n		if byVipLevel < 3 then\r\n			if nIdx <= 2 then\r\n				return 50\r\n			elseif nIdx <= 4 then\r\n				return 60\r\n			elseif nIdx == 5 then\r\n				return 70\r\n			elseif nIdx == 6 then\r\n				return 80\r\n			elseif nIdx == 7 then\r\n				return 90\r\n			elseif nIdx == 8 then\r\n				return 100\r\n			else\r\n				return 100\r\n			end\r\n		else\r\n			if nIdx <= 2 then\r\n				return 70+(byVipLevel-3)*20\r\n			elseif nIdx <= 4 then\r\n				return 80+(byVipLevel-3)*20\r\n			elseif nIdx == 5 then\r\n				return 90+(byVipLevel-3)*20\r\n			elseif nIdx == 6 then\r\n				return 100+(byVipLevel-3)*20\r\n			elseif nIdx == 7 then\r\n				return 110+(byVipLevel-3)*20\r\n			elseif nIdx == 8 then\r\n				return 120+(byVipLevel-3)*20\r\n			else\r\n				return 120+(byVipLevel-3)*20\r\n			end\r\n		end\r\n	elseif nFunc==EGCF_GATHER_SCIENCE_COIN then -- 铜钱聚魂消耗的铜钱\r\n		if byVipLevel < 5 then\r\n			return byVipLevel * 300 + 20000\r\n		else\r\n			return ( byVipLevel - 4) * 500 + 150000\r\n		end\r\n	elseif nFunc==EGCF_GATHER_SCIENCE_GOLD then -- 铜钱聚魂消耗的元宝\r\n		if byVipLevel < 5 then\r\n			return byVipLevel * 30 + 200\r\n		else\r\n			return ( byVipLevel - 4) * 500 + 1500\r\n		end\r\n	elseif nFunc == EGCF_BEG_BLUE_IMPRV_LUCK then -- 元宝改运消耗的元宝\r\n		if nIdx >= 1 and nIdx <= 3 then\r\n			return 0\r\n		elseif nIdx >= 4 and nIdx <= 9 then\r\n			return (nIdx-3)*2 + 8\r\n		elseif nIdx >=10 and nIdx <= 15 then\r\n			return (nIdx-2-3) * 5\r\n		elseif nIdx > 15 then\r\n		    return 50	\r\n		else \r\n			return 0\r\n		end	\r\n	elseif nFunc == EGCF_SHADE_CHANGE_MONSTER then --暗部换妖\r\n		if nIdx >= 1 and nIdx <= 2 then\r\n			return 10\r\n		elseif nIdx >= 3 and nIdx <= 4 then\r\n			return 15\r\n		elseif nIdx >=5 and nIdx <= 6 then\r\n			return 20\r\n		elseif nIdx >= 7 and nIdx <= 8 then\r\n		    return 25\r\n		elseif nIdx >= 9 then \r\n				return 30\r\n		else \r\n			return 0\r\n		end	\r\n	elseif nFunc == EGCF_FACTION_OPEN_GOLD then --开启黄金模式\r\n	    return 200\r\n	elseif nFunc == EGCF_FACTION_CLOSE_MONSTER then --收妖\r\n			return 20\r\n	elseif nFunc == EGCF_FACTION_CREATE_BY_COIN then --创建门派消耗铜钱\r\n			return 5000000\r\n	elseif nFunc == EGCF_FACTION_CREATE_BY_GOLD then --创建门派消耗元宝\r\n      return 250\r\n	elseif nFunc==EGCF_GENCOIN then -- 铜钱聚魂消耗的元宝\r\n		return math.floor((nCnt * nCnt *100 + nCnt * 5000 + 40000) / 5000) * 5000;\r\n	elseif nFunc==EGCF_GENGOLD then -- 元宝聚魂消耗的元宝\r\n		if nCnt <= 7 then\r\n			return 20 + nCnt * 5\r\n		elseif nCnt <= 12 then\r\n			return 50 + (nCnt - 7) * 10\r\n		else \r\n			return 100;\r\n		end\r\n	elseif nFunc==EGCF_GENUPGOLD then -- 刷新消耗的铜钱\r\n		if nCnt < 3 then\r\n			return 0\r\n		elseif nCnt < 8 then\r\n			return (nCnt - 2) * 2\r\n		else \r\n			return 10;\r\n		end\r\n	elseif nFunc==EGCF_GENUPGOLDFULL then -- 聚魂一键成长消耗的元宝\r\n		return 100\r\n	else\r\n		return 0\r\n	end\r\nend\r\n\r\n\r\n\r\n-- 功能获取的相关数值\r\nEOF_GATHER_SCIENCE_COIN = 1 --铜钱聚魂\r\nEOF_GATHER_SCIENCE_GOLD = 2 --元宝聚魂\r\nEOF_BLUE = 3 				--蓝气\r\nEOF_PURPLE = 4 				--紫气\r\nfunction GetObtainByFunc(byVipLevel, byLevel, nFunc, nCnt, nIdx)\r\n	if nFunc==EGCF_GATHER_SCIENCE_COIN then\r\n		if byVipLevel < 5 then\r\n			return 2*(byVipLevel+1)\r\n		elseif byVipLevel < 8 then\r\n			return 5*(byVipLevel-4)+10\r\n		else\r\n			return 10*(byVipLevel-8)+30\r\n		end\r\n	elseif nFunc == EGCF_GATHER_SCIENCE_GOLD then\r\n		if byVipLevel < 5 then\r\n			return 2*(byVipLevel+1)\r\n		else\r\n			return 10\r\n		end\r\n	elseif nFunc == EOF_BLUE then\r\n		return 2*(byLevel+1) * nCnt\r\n	elseif nFunc == EOF_PURPLE then\r\n		return 10*(byLevel+1) * nCnt\r\n	end\r\nend\r\n\r\n\r\nfunction GetGenGasNum( byVipLevel, byLevel, byTownIdx, byInstanceIdx)\r\n	if (byLevel < 35 or byTownIdx < 2) or (byTownIdx == 2 and byInstanceIdx < 3)  then\r\n		return 6;\r\n	elseif (byLevel < 40 or byTownIdx < 2) or (byTownIdx == 2 and byInstanceIdx < 3) then\r\n		return 8;\r\n	elseif (byLevel < 45 or byTownIdx < 3) or (byTownIdx == 3 and byInstanceIdx < 4) then\r\n		return 10;\r\n	elseif (byLevel < 50 or byTownIdx < 3) or (byTownIdx == 3 and byInstanceIdx < 9) then\r\n		return 12;\r\n	elseif (byLevel < 55 or byTownIdx < 4) or (byTownIdx == 4 and byInstanceIdx < 5) then\r\n		return 14;\r\n	elseif (byLevel < 60 or byTownIdx < 5) or (byTownIdx == 5 and byInstanceIdx < 1) then\r\n		return 16;\r\n	elseif (byLevel < 65 or byTownIdx < 5) or (byTownIdx == 5 and byInstanceIdx < 6) then\r\n		return 18;\r\n	elseif (byLevel < 70 or byTownIdx < 6) or (byTownIdx == 6 and byInstanceIdx < 2) then\r\n		return 20;\r\n	elseif (byLevel < 75 or byTownIdx < 6) or (byTownIdx == 6 and byInstanceIdx < 7) then\r\n		return 22;\r\n	elseif (byLevel < 80 or byTownIdx < 7) or (byTownIdx == 7 and byInstanceIdx < 3) then\r\n		return 24;\r\n	elseif (byLevel < 85 or byTownIdx < 7) or (byTownIdx == 7 and byInstanceIdx < 8) then\r\n		return 26;\r\n	elseif (byLevel < 90 or byTownIdx < 8) or (byTownIdx == 8 and byInstanceIdx < 4) then\r\n		return 28;\r\n	else \r\n		return 30\r\n	end;\r\nend\r\n\r\n-- 功能聚魂的倍率\r\nfunction GetGatherScienceIncrease(nIdx)\r\n	return nIdx * 3;\r\nend\r\n\r\n\r\n\r\nGSIP_DOWN = 1 			--向下\r\nGSIP_EQUIT= 2 			--相等\r\nGSIP_UP = 3 			--向上\r\n\r\n\r\n-- 功能聚魂的倍率\r\nfunction GetGatherScienceIncreasePer(nIdx, nFunc)\r\n	if nIdx == 1 then\r\n		if nFunc == GSIP_DOWN then\r\n			return 10;\r\n		elseif nFunc == GSIP_EQUIT then\r\n			return 10;\r\n		elseif nFunc == GSIP_UP then\r\n			return 80;\r\n		else \r\n			return 0;\r\n		end \r\n	elseif nIdx == 2 then\r\n		if nFunc == GSIP_DOWN then\r\n			return 20;\r\n		elseif nFunc == GSIP_EQUIT then\r\n			return 20;\r\n		elseif nFunc == GSIP_UP then\r\n			return 60;\r\n		else \r\n			return 0;\r\n		end \r\n	elseif nIdx == 3 then\r\n		if nFunc == GSIP_DOWN then\r\n			return 30;\r\n		elseif nFunc == GSIP_EQUIT then\r\n			return 30;\r\n		elseif nFunc == GSIP_UP then\r\n			return 40;\r\n		else \r\n			return 0;\r\n		end \r\n	elseif nIdx == 4 then\r\n		if nFunc == GSIP_DOWN then\r\n			return 0;\r\n		elseif nFunc == GSIP_EQUIT then\r\n			return 40;\r\n		elseif nFunc == GSIP_UP then\r\n			return 60;\r\n		else \r\n			return 0;\r\n		end \r\n	end \r\nend\r\n\r\n-- 悟道召唤次数\r\nfunction GetAwakenCallNum()\r\n	return 5;\r\nend\r\n\r\n-- 境界蓝气转换紫气比例(多少蓝气拿紫气)\r\nfunction GetBlueNumToPerPurple()\r\n	return 100;\r\nend\r\n-- 每次点蓝气值\r\nfunction GetClickBlueValue()\r\n	return 1000;\r\nend\r\n\r\n-- 获得主城收获元宝\r\nfunction GetMaintownCrownGold(PlayerLevel)\r\n	return 0;\r\nend\r\n-- 获得主城收获铜钱\r\nfunction GetMaintownCrownCoin(PlayerLevel)\r\n	return 2000 * PlayerLevel + 10000;\r\nend\r\n-- 获得主城收获境界点\r\nfunction GetMaintownCrownJingJie(PlayerLevel)\r\n	return 500;\r\nend\r\n\r\n-- 获得征收境界点\r\nfunction GetMaintownLevyJingJie(PlayerLevel)\r\n	return 25;\r\nend\r\n-- 获得主城征收铜钱\r\nfunction GetMaintownLevyCoin(PlayerLevel)\r\n	return 200 * PlayerLevel + 1000;\r\nend\r\n\r\n-- 查找该礼包是否需提示,1需要提示，0不需要\r\nfunction GetBoxGoodsPrompt(BoxGoodsID)\r\n	if BoxGoodsID== 40001 or  BoxGoodsID== 40002 or  BoxGoodsID== 40003 or  BoxGoodsID== 40004 or  BoxGoodsID== 40005 or  BoxGoodsID== 40006 or  BoxGoodsID== 40007 then\r\n		return 1\r\n	else\r\n		return 0\r\n	end\r\nend\r\n\r\n-- 获得生成收集领取奖励次数时间\r\nfunction GetPassIncGenGasNumTime(DayPassSecond)\r\n	TimeSec = 12 * 3600 + 1800;\r\n	if DayPassSecond < TimeSec then\r\n		return TimeSec;\r\n	end\r\n 	TimeSec = 18 * 3600 + 1800;\r\n	if DayPassSecond < TimeSec then\r\n		return TimeSec;\r\n	end\r\nend\r\n\r\n\r\n-- 获取boss战排名奖励\r\nfunction GetBossBTopEncCoin(PlayerLevel, Top)\r\n	Coin = 0;\r\n	if Top == 1 then\r\n		Coin = PlayerLevel*20000+1000000;\r\n	elseif Top == 2 then\r\n		Coin = PlayerLevel*10000+500000;\r\n	elseif Top == 3 then\r\n		Coin = PlayerLevel*5000+250000;\r\n	end;\r\n\r\n	return Coin;\r\nend;\r\n\r\n-- 获取boss战排名奖励\r\nfunction GetBossBTopEncJingJie(PlayerLevel, Top)\r\n	JingJie = 0;\r\n	if Top == 1 then\r\n		JingJie = 2000;\r\n	elseif Top == 2 then\r\n		JingJie = 1000;\r\n	elseif Top == 3 then\r\n		JingJie = 500;\r\n	end;\r\n\r\n	return JingJie;\r\nend;\r\n\r\n-- 获取boss战击杀奖励\r\nfunction GetBossBKillEncCoin(BossLevel)\r\n	Coin = (BossLevel - 20) * 100000 + 1000000;\r\n	if Coin < 0 then\r\n		Coin = 0;\r\n	end;\r\n\r\n	return Coin;\r\nend;\r\n\r\n-- 获取boss战击杀奖励\r\nfunction GetBossBKillEncJingJie(BossLevel)\r\n	return 0;\r\nend;\r\n\r\n-- 获取boss战伤害奖励\r\nfunction GetBossBHurtEncCoin(PlayerLevel, HurtValue)\r\n	Coin = HurtValue / 10;\r\n	MaxCoin = PlayerLevel * 100;\r\n	if Coin > MaxCoin then\r\n		Coin = MaxCoin\r\n	end;\r\n\r\n	if Coin < 1 then\r\n		Coin = 1;\r\n	end;\r\n\r\n	return Coin;\r\nend;\r\n\r\n-- 获取boss战伤害奖励\r\nfunction GetBossBHurtEncJingJie(PlayerLevel, HurtValue)\r\n	JingJie = HurtValue / 1000;\r\n	MaxJingJie = 30;\r\n	if JingJie > MaxJingJie then\r\n		JingJie = MaxJingJie\r\n	end;\r\n\r\n	if JingJie < 1 then\r\n		JingJie = 1;\r\n	end;\r\n\r\n	return JingJie;\r\nend;\r\n\r\n-- 获取boss战英雄奖励\r\nfunction GetBossBHeroHurtEncCoin()\r\n	return 100000;\r\nend;\r\n\r\n-- 获取boss战英雄奖励\r\nfunction GetBossBHeroHurtEncJingJie()\r\n	return 200;\r\nend;\r\n\r\n-- 获取boss战鼓舞概率\r\nfunction GetInspireRate(GoldInspireFlag, IncPowerRate)\r\n	Rate = 0;\r\n	if GoldInspireFlag == 1 then\r\n		Rate = 20;\r\n	else\r\n		--math.randomseed(os.time())\r\n		math.randomseed(tostring(os.time()):reverse():sub(1, 6))\r\n		--temp =  LuaRandomReseed(0, 100);\r\n		if IncPowerRate == 0 then\r\n			if LuaRandomReseed(100) < 50 then\r\n				Rate = 20;\r\n			end;\r\n		elseif IncPowerRate <= 20 then\r\n			if LuaRandomReseed(100) < 40 then\r\n				Rate = 20;\r\n			end;\r\n		elseif IncPowerRate <= 40 then\r\n			if LuaRandomReseed(100) < 30 then\r\n				Rate = 20;\r\n			end;\r\n		elseif IncPowerRate <= 60 then\r\n			if LuaRandomReseed(100) < 20 then\r\n				Rate = 20;\r\n			end;\r\n		elseif IncPowerRate <= 80 then\r\n			if LuaRandomReseed(100) < 10 then\r\n				Rate = 20;\r\n			end;\r\n		else\r\n			Rate = 0;\r\n		end;\r\n	end;\r\n\r\n	return Rate;\r\nend;\r\n\r\n\r\n-- 获取boss战每次阅历鼓舞百分比\r\nfunction GetIncRateByStory()\r\n	return 20;\r\nend;\r\n\r\n-- 获取boss战每次元宝鼓舞百分比\r\nfunction GetIncRateByGold()\r\n	return 20;\r\nend;\r\n\r\n-- 获取随机调整奖励境界\r\nfunction GetRdchallengeEncJingJie(Ret, MyLevel, EnemyLevel)\r\n	if Ret == 0 then\r\n		return (EnemyLevel / MyLevel) * ( MyLevel * MyLevel * 0.01 + MyLevel * 0.1 + 20);\r\n	else\r\n		return (EnemyLevel / MyLevel) * ( MyLevel * MyLevel * 0.01 + MyLevel * 0.1 + 20) / 2;\r\n	end;\r\nend;\r\n\r\n-- 获取随机调整奖励紫气\r\nfunction GetRdchallengeEncPurpleGas(Ret, MyLevel, EnemyLevel)\r\n	if Ret == 0 then\r\n		return (EnemyLevel / MyLevel) * ( MyLevel * MyLevel * 0.01 + MyLevel * 0.1 + 20) * 5;\r\n	else\r\n		return (EnemyLevel / MyLevel) * ( MyLevel * MyLevel * 0.01 + MyLevel * 0.1 + 20) * 5 / 2;\r\n	end;\r\nend;\r\n\r\nfunction GetMeltGold(Science)\r\n	MaxScience = Science;\r\n	MinScience = Science/3;\r\n	Gap = MaxScience - MinScience;\r\n	Ret = 0;\r\n    if  Gap < 1000 then\r\n		Ret = Gap / 20;\r\n	elseif  Gap < 5000 then\r\n		Ret = 1000 / 20 +( Gap - 1000 ) / 50;\r\n	elseif  Gap < 50000 then\r\n		Ret = 1000 / 20 +( 5000 - 1000 ) / 50 + ( Gap - 5000 ) / 100;\r\n	else\r\n		Ret = 1000 / 20 +( 5000 - 1000 ) / 50 + ( 50000 - 5000 ) / 100 + ( Gap - 50000 ) / 200;\r\n	end;\r\n\r\n	if Ret % 1 > 0 then\r\n		Ret = Ret + 1;\r\n	end\r\n	return Ret;\r\n\r\nend;\r\n\r\n-- 根据爬塔层数获取求仙露次数\r\nfunction GetBegBlueMaxTimes(Layer)\r\n    BegblueCnt = 0;\r\n    \r\n    if Layer >= 0 then\r\n        BegblueCnt = math.floor(Layer/10) + 5; \r\n    -- elseif Layer >=55 and Layer <= 100 then\r\n    --    BegblueCnt = math.floor(Layer/5);\r\n    else\r\n        BegblueCnt = 0;\r\n    end;\r\n\r\n	-- test\r\n	-- BegblueCnt = 1000;\r\n    \r\n    return BegblueCnt;\r\nend;\r\n\r\n-- 继续爬塔n层可增加求仙露次数提示\r\nfunction GetClimbTowerPrompt(Layer)\r\n    MoreTowerLayer = 0;\r\n    \r\n    if Layer >= 0 then\r\n        MoreTowerLayer = math.floor(Layer/10 + 1)*10 - Layer; \r\n    -- elseif Layer >=50 and Layer < 100 then\r\n    --     MoreTowerLayer = math.floor(Layer/5 + 1)*5 - Layer; \r\n    else\r\n        MoreTowerLayer = 0;\r\n    end;\r\n    \r\n    return MoreTowerLayer;\r\nend;\r\n\r\n\r\n-- 求仙露每日免费改运次数上限\r\nfunction GetFreeImproveLuckTimes()\r\n    return 3;\r\nend;\r\n\r\n-- 获取求仙露是否忽略倍率\r\nfunction GetBegBlueIsIgnoreXnMultiple()\r\n    return 0;\r\nend;\r\n\r\n-- 每日可免费提升聚魂的次数\r\nfunction GetTotalFreeCommonUpGasNum()\r\n		return 3;\r\nend;\r\n\r\n-- 根据战力获得boss战伤害值\r\nfunction GetBossBHurtValueByPower(Power)\r\n	HurtValue = 0;\r\n    if  Power <= 100000 then\r\n		HurtValue = Power/10;		\r\n	elseif  Power <= 500000 then	\r\n		-- HurtValue = (Power - 100000)/20 + 100000/10;\r\n		HurtValue = (Power - 100000)/20 + 10000;		\r\n	elseif  Power <= 2000000 then\r\n		-- HurtValue = (Power - 500000)/30 + (500000 - 100000)/20 + 100000/10;\r\n		HurtValue = (Power - 500000)/30 + 30000;		\r\n	elseif  Power <= 5000000 then\r\n		-- HurtValue = (Power - 2000000)/40 +  (2000000 - 500000)/30 + (500000 - 100000)/20 + 100000/10;\r\n		HurtValue = (Power - 2000000)/40 +  80000;\r\n		\r\n	else\r\n		-- HurtValue = (Power - 5000000)/50 +  (5000000 - 2000000)/40 + (2000000 - 500000)/30 + (500000 - 100000)/20 + 100000/10;\r\n		HurtValue = (Power - 5000000)/50 + 155000;\r\n	end;\r\n	return HurtValue;\r\nend;\r\n\r\n--免费换怪次数\r\nfunction GetFreeRefreshNum()\r\n		return 10\r\nend\r\n\r\n--筋斗云消耗的元宝\r\nFindMonsterGold = 100\r\nfunction GetGoldByFindMonster()\r\n		return FindMonsterGold\r\nend;\r\n\r\n\r\n--获取酒变质奖励所需铜钱\r\nCoinByBadWine = 10000\r\nfunction GetCoinByBadWine()\r\n		return CoinByBadWine\r\nend\r\n\r\n--捐献一次消耗铜钱\r\nCoinEveryTimeValues = 100000\r\nGoldEveryTimeValues = 20\r\nBaseFundsByCoin = 100\r\nBaseFundsByGold = 500\r\nBaseDoorsTributeByCoin = 100\r\nBaseDoorsTributeByGold = 500\r\n--捐献一次消耗铜钱\r\nfunction GetCostCoinByDonate(FactionLevel)\r\n		return CoinEveryTimeValues\r\nend\r\n\r\n--捐献一次消耗元宝\r\nfunction GetCostGoldByDonate(FactionLevel)\r\n	  return GoldEveryTimeValues\r\nend\r\n\r\n--捐献一次铜钱获取门派资金\r\nfunction GetFundsByDonateCoin(FactionLevel)\r\n		return BaseFundsByCoin\r\nend\r\n\r\n--捐献一次元宝获取门派资金\r\nfunction GetFundsByDonateGold(FactionLevel)\r\n		return BaseFundsByGold\r\nend\r\n\r\n--捐献一次铜钱获取门贡\r\nfunction GetDoorsTributeByDonateCoin(FactionLevel)\r\n		return BaseDoorsTributeByCoin\r\nend\r\n\r\n--捐献一次元宝获取门贡\r\nfunction GetDoorsTributeByDonateGold(FactionLevel)\r\n		return BaseDoorsTributeByGold\r\nend\r\n\r\n--门贡与积分的比例关系\r\nDoorsTributeIntegral = 100\r\nfunction GetDoorsTributeIntegral()\r\n		return DoorsTributeIntegral\r\nend\r\n\r\n--黄金模式的倍数数\r\nGoldModeMulti = 2\r\nfunction GetGoldModeMulti()\r\n		return GoldModeMulti\r\nend\r\n\r\n--加入门派间隔时间\r\nJoinFactionInterval = 14400\r\nfunction GetJoinFactionInterval()\r\n		return JoinFactionInterval\r\nend\r\n\r\n--普通找回门贡消耗的铜钱\r\nfunction GetNormalFindBackDoorsTribute( LeftDoorsTribute )\r\n		return LeftDoorsTribute * 100\r\nend\r\n\r\n--完美找回门贡消耗的元宝\r\nfunction GetPerfectFindBackDoorsTribute( LeftDoorsTribute )\r\n		return LeftDoorsTribute / 100\r\nend\r\n\r\n--可提交经验上限\r\nfunction GetMaxExprience( VipLevel, FactionLevel)\r\n		if FactionLevel < 4 then\r\n				return 0\r\n		else\r\n				return (10000 * (FactionLevel - 3) * (VipLevel / 10 + 1) * (VipLevel / 10 + 1) * (VipLevel / 10 + 1)) / 1000 * 1000\r\n		end\r\nend\r\n\r\n--经验每秒增加基数\r\nfunction GetExprienceAddBase( PlayerLevel, FactionLevel)\r\n		if PlayerLevel < 25 then\r\n				return 0\r\n		elseif FactionLevel < 4 then\r\n				return 0\r\n		else\r\n				return ((PlayerLevel / 10 + 1) * 2 * FactionLevel) / 10\r\n		end\r\nend', 'activityInfo =\r\n{\r\n	-- DspDate娲诲姩鏄剧ず鏃堕棿, StartDate娲诲姩寮�濮嬫椂闂达紝EndDate娲诲姩鎸佺画鍚嶇О锛孨ame娲诲姩鍚嶇О\r\n	{ID=1, DspDate=\"0\", StartDate=\"0\", EndDate=\"0\", Name=\"棣栧厖娲诲姩\"}, \r\n	{ID=2, DspDate=\"2013/3/28 17:23:37\", StartDate=\"2013/11/28 17:23:37\", EndDate=\"2015/11/28 00:00:00\", Name=\"鍐插嚮鍓?0鍚峔\"}, \r\n};\r\n\r\nfunction CkFirstRecharge(player, id)\r\n	if player.CkActivityHaveDoNum(id) > 0 then\r\n		return ACTIVITY_ERR_ACTIVITY_HAVE_FINISH;\r\n	end;\r\n	\r\n	if 0 == player.GetDT_PLAYER_BASE_DATA().dwTotalAmount then\r\n		return ACTIVITY_ERR_ACTIVITY_CON_NOT_MATCH;\r\n	end\r\n	\r\n	player.ActivityEncourage(id, EBKIND_COIN, 1000, 0);\r\n	\r\n	return 0;\r\nend\r\n\r\n\r\nfunction CkRecharge1000(player)\r\n	if nil == player then\r\n		return\r\n	end  \r\nend\r\n\r\n-- 鑾峰彇娲诲姩鎻忚堪淇℃伅\r\nfunction GetActivityInfo(player, id)\r\n	if nil == player then\r\n		return \"\"; \r\n	end;\r\n		\r\n	if 1 == ID then\r\n		 return \"棣栨鍏呭�煎彲鑾锋瀬鍝佷紮浼碶\";\r\n	elseif 2 == ID then\r\n		return \"鍦?鏈?8鏃ュ墠绔炴妧鍦哄墠3鍚嶇殑鐜╁鍙鍙栫绉樺ぇ濂朶\";\r\n	else\r\n		return \"\";\r\n	end;\r\n	return \"\";\r\nend\r\n\r\n-- 鑾峰彇娲诲姩鏄剧ず鏁板瓧淇℃伅\r\nfunction GetActivityDspNum(player, id)\r\n	if nil == player then\r\n		return \"\"; \r\n	end;\r\n		\r\n	if 1 == ID then\r\n		 return 8;\r\n	elseif 2 == ID then\r\n		return 5;\r\n	else\r\n		return 0;\r\n	end;\r\n	return 0;\r\nend\r\n\r\n-- 鑾峰彇娲诲姩鍓嶅線ID\r\nfunction GetActivityForewordID(player, id)\r\n	if nil == player then\r\n		return \"\"; \r\n	end;\r\n		\r\n	if 1 == ID then\r\n		 return 8;\r\n	elseif 2 == ID then\r\n		return 5;\r\n	else\r\n		return 0;\r\n	end;\r\n	return 0;\r\nend\r\n\r\n-- 鑾峰彇娲诲姩鍓嶅線淇℃伅\r\nfunction GetActivityForewordInfo(player, id)\r\n	if nil == player then\r\n		return \"\"; \r\n	end;\r\n		\r\n	if 1 == ID then\r\n		 return \"濂藉惂\";\r\n	elseif 2 == ID then\r\n		return \"鍙互\";\r\n	else\r\n		return \"\";\r\n	end;\r\n	return \"\";\r\nend\r\n\r\n-- 妫�娴嬫椿鍔ㄥ畬鎴愭潯浠舵槸鍚﹁揪鍒帮紝瀹屾垚鍒欑粰濂栧姳\r\nfunction CkFinishActivity(player, id)\r\n	if nil == player then\r\n		return ACTIVITY_ERR_PLAYER_NULL; \r\n	end;\r\n			\r\n	if 1 == ID then\r\n		 CkFirstRecharge(player);\r\n	elseif 2 == ID then\r\n		CkRecharge1000(player);\r\n	else\r\n		return ACTIVITY_ERR_INVALID_ID;\r\n	end;\r\n	\r\n	return ACTIVITY_ERR_INVALID_ID;\r\nend\r\n');

INSERT INTO msgdefprop(msgdefprop.CliRegionType,msgdefprop.MsgKey,msgdefprop.Msg) VALUES(0,"BB_ENCOURAGE_MESSA","求得{x}个笑脸额外获得{y}仙露");

INSERT INTO msgdefprop(msgdefprop.CliRegionType,msgdefprop.MsgKey,msgdefprop.Msg) VALUES(0,"BB_ENCOURAGE_MESSB","再求得{x}个笑脸，即可额外获得{y}仙露");

INSERT INTO msgdefprop(msgdefprop.CliRegionType,msgdefprop.MsgKey,msgdefprop.Msg) VALUES(0,"BB_ENCOURAGE_MESSC","今日可领取笑脸奖励已经领完");

DROP TABLE IF EXISTS `begblueencourageprop`;
CREATE TABLE `begblueencourageprop` (
  `SmileNum` int(11) NOT NULL,
  `Blue` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of begblueencourageprop
-- ----------------------------
INSERT INTO `begblueencourageprop` VALUES ('10', '1000');
INSERT INTO `begblueencourageprop` VALUES ('20', '2000');
INSERT INTO `begblueencourageprop` VALUES ('30', '4000');
INSERT INTO `begblueencourageprop` VALUES ('40', '6000');
INSERT INTO `begblueencourageprop` VALUES ('60', '14000');
INSERT INTO `begblueencourageprop` VALUES ('80', '26000');
INSERT INTO `begblueencourageprop` VALUES ('100', '44000');

ALTER TABLE singlepram ADD COLUMN ExprienceAddInterval smallint(5) unsigned NOT NULL DEFAULT 3;
ALTER TABLE singlepram ADD COLUMN ChangeFactionIcon smallint(5) unsigned NOT NULL DEFAULT 100;
REPLACE INTO `msgdefprop` VALUES ('0', 'YOU_NEED_REEDIT_YOUR_NAME', '因合区原因，您的昵称与别的玩家发生了冲突，请重新取名', '');