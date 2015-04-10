/*
Navicat SQLite Data Transfer

Source Server         : maxnetcfgdb
Source Server Version : 30706
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30706
File Encoding         : 65001

Date: 2012-11-20 09:31:57
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for "main"."awakenattrkind"
-- ----------------------------
DROP TABLE "main"."awakenattrkind";
CREATE TABLE "awakenattrkind"(
  Kind smallint unsigned NOT NULL,
  Name varchar(32) NOT NULL,
  Introduction varchar unsigned NOT NULL,
  PRIMARY KEY(Kind));

-- ----------------------------
-- Records of awakenattrkind
-- ----------------------------
INSERT INTO "main"."awakenattrkind" VALUES (1, 'HP', 'HP');
INSERT INTO "main"."awakenattrkind" VALUES (2, '攻击力', '攻击力');
INSERT INTO "main"."awakenattrkind" VALUES (3, '命中率', '命中率');
INSERT INTO "main"."awakenattrkind" VALUES (6, '抗暴率', '抗暴率');
INSERT INTO "main"."awakenattrkind" VALUES (5, '暴击率', '暴击率');
INSERT INTO "main"."awakenattrkind" VALUES (4, '闪避率', '闪避率');
INSERT INTO "main"."awakenattrkind" VALUES (7, '增伤', '增加伤害率');
INSERT INTO "main"."awakenattrkind" VALUES (8, '减伤', '减少伤害率');

-- ----------------------------
-- Table structure for "main"."awakenbaseprop"
-- ----------------------------
DROP TABLE "main"."awakenbaseprop";
CREATE TABLE "awakenbaseprop" (
"AwakenStudyExp"  int NOT NULL,
"MaxStudyLevel"  smallint NOT NULL,
"StoreClipGridNum"  smallint NOT NULL,
"StoreStudyGridNum"  smallint NOT NULL,
PRIMARY KEY ("AwakenStudyExp" ASC)
);

-- ----------------------------
-- Records of awakenbaseprop
-- ----------------------------
INSERT INTO "main"."awakenbaseprop" VALUES (1000, 10, 16, 6);

-- ----------------------------
-- Table structure for "main"."awakenherostudyopenlevel"
-- ----------------------------
DROP TABLE "main"."awakenherostudyopenlevel";
CREATE TABLE "awakenherostudyopenlevel" (
"HeroLevel"  smallint NOT NULL,
"OpenStudyNum"  smallint NOT NULL,
PRIMARY KEY ("HeroLevel")
);

-- ----------------------------
-- Records of awakenherostudyopenlevel
-- ----------------------------
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (25, 1);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (30, 2);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (35, 3);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (40, 4);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (45, 5);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (50, 6);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (55, 7);
INSERT INTO "main"."awakenherostudyopenlevel" VALUES (60, 8);

-- ----------------------------
-- Table structure for "main"."awakenplevelrop"
-- ----------------------------
DROP TABLE "main"."awakenplevelrop";
CREATE TABLE "awakenplevelrop" (
"AwakenLevel"  smallint NOT NULL,
"NeedCoin"  INTEGER NOT NULL DEFAULT 0,
"NextLevelRate"  smallint NOT NULL,
"BuleStudyChipRate"  smallint NOT NULL,
"PurpleStudyChipRate"  smallint NOT NULL,
"RedStudyChipRate"  smallint NOT NULL,
"YellowStudyChipRate"  smallint NOT NULL,
"StudyExpRate"  smallint NOT NULL,
PRIMARY KEY ("AwakenLevel" ASC)
);

-- ----------------------------
-- Records of awakenplevelrop
-- ----------------------------
INSERT INTO "main"."awakenplevelrop" VALUES (1, 10, 80, 40, 30, 10, 10, 10);
INSERT INTO "main"."awakenplevelrop" VALUES (2, 10, 80, 40, 30, 10, 10, 10);
INSERT INTO "main"."awakenplevelrop" VALUES (3, 10, 80, 40, 30, 10, 10, 10);
INSERT INTO "main"."awakenplevelrop" VALUES (4, 10, 80, 40, 30, 10, 10, 10);
INSERT INTO "main"."awakenplevelrop" VALUES (5, 10, 80, 40, 30, 10, 10, 10);

-- ----------------------------
-- Table structure for "main"."awakenstudycolorkind"
-- ----------------------------
DROP TABLE "main"."awakenstudycolorkind";
CREATE TABLE "awakenstudycolorkind"(
  Kind smallint unsigned NOT NULL,
  Name varchar(32) NOT NULL,
  Introduction varchar unsigned NOT NULL,
  PRIMARY KEY(Kind));

-- ----------------------------
-- Records of awakenstudycolorkind
-- ----------------------------
INSERT INTO "main"."awakenstudycolorkind" VALUES (1, '蓝色心得', '蓝色心得');
INSERT INTO "main"."awakenstudycolorkind" VALUES (2, '紫色心得', '紫色心得');
INSERT INTO "main"."awakenstudycolorkind" VALUES (3, '红色心得', '红色心得');
INSERT INTO "main"."awakenstudycolorkind" VALUES (4, '黄色心得', '黄色心得');

-- ----------------------------
-- Table structure for "main"."awakenstudycolorprop"
-- ----------------------------
DROP TABLE "main"."awakenstudycolorprop";
CREATE TABLE "awakenstudycolorprop" (
"ColorKind"  smallint NOT NULL,
"NeedClipNum"  smallint NOT NULL,
"Clip2Exp"  int NOT NULL,
"UpgradeStudyExpConst"  smallint NOT NULL,
PRIMARY KEY ("ColorKind" ASC)
);

-- ----------------------------
-- Records of awakenstudycolorprop
-- ----------------------------
INSERT INTO "main"."awakenstudycolorprop" VALUES (1, 10, 100, 100);
INSERT INTO "main"."awakenstudycolorprop" VALUES (2, 10, 100, 100);
INSERT INTO "main"."awakenstudycolorprop" VALUES (3, 10, 100, 100);
INSERT INTO "main"."awakenstudycolorprop" VALUES (4, 10, 100, 100);

-- ----------------------------
-- Table structure for "main"."awakenstudykind"
-- ----------------------------
DROP TABLE "main"."awakenstudykind";
CREATE TABLE "awakenstudykind"(
  Kind smallint unsigned NOT NULL,
  Name varchar(32) NOT NULL,
  Introduction varchar unsigned NOT NULL,
  PRIMARY KEY(Kind));

-- ----------------------------
-- Records of awakenstudykind
-- ----------------------------
INSERT INTO "main"."awakenstudykind" VALUES (1, '天心得', '天/地/风/雷/火/水/山/泽');
INSERT INTO "main"."awakenstudykind" VALUES (2, '地心得', '地心得');
INSERT INTO "main"."awakenstudykind" VALUES (3, '风心得', '风心得');
INSERT INTO "main"."awakenstudykind" VALUES (8, '泽心得', '泽心得');
INSERT INTO "main"."awakenstudykind" VALUES (4, '雷心得', '雷心得');
INSERT INTO "main"."awakenstudykind" VALUES (5, '火心得', '火心得');
INSERT INTO "main"."awakenstudykind" VALUES (6, '水心得', '水心得');
INSERT INTO "main"."awakenstudykind" VALUES (7, '山心得', '山心得');

-- ----------------------------
-- Table structure for "main"."awakenstudyprop"
-- ----------------------------
DROP TABLE "main"."awakenstudyprop";
CREATE TABLE "awakenstudyprop" (
"StudyKind"  smallint NOT NULL,
"ColorKind"  smallint DEFAULT 1,
"AffectAttr"  smallint NOT NULL,
"AffectAttrBaseValue"  int NOT NULL,
"AffectAttrIncValue"  int NOT NULL,
PRIMARY KEY ("StudyKind" ASC, "ColorKind" ASC)
);

-- ----------------------------
-- Records of awakenstudyprop
-- ----------------------------
INSERT INTO "main"."awakenstudyprop" VALUES (1, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (2, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (3, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (4, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (5, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (6, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (7, 1, 1, 100, 20);
INSERT INTO "main"."awakenstudyprop" VALUES (8, 1, 1, 100, 20);

-- ----------------------------
-- Table structure for "main"."bagprop"
-- ----------------------------
DROP TABLE "main"."bagprop";
CREATE TABLE "bagprop" (
"MaxOpenNum"  smallint NOT NULL,
"InitOpenNum"  smallint NOT NULL,
"BaseOpenCoin"  INTEGER NOT NULL,
"OpenCoinIncrease"  INTEGER NOT NULL,
"BaseOpenGold"  INTEGER NOT NULL,
"OpenGoldIncrease"  INTEGER NOT NULL
);

-- ----------------------------
-- Records of bagprop
-- ----------------------------
INSERT INTO "main"."bagprop" VALUES (50, 10, 50, 10, 5, 2);

-- ----------------------------
-- Table structure for "main"."battleattributekind"
-- ----------------------------
DROP TABLE "main"."battleattributekind";
CREATE TABLE "battleattributekind" (
"KindID"  INTEGER NOT NULL,
"Name"  CHAR NOT NULL,
PRIMARY KEY ("KindID")
);

-- ----------------------------
-- Records of battleattributekind
-- ----------------------------
INSERT INTO "main"."battleattributekind" VALUES (1, 'HP');
INSERT INTO "main"."battleattributekind" VALUES (2, '攻击力');
INSERT INTO "main"."battleattributekind" VALUES (3, '命中率');
INSERT INTO "main"."battleattributekind" VALUES (4, '闪避率');
INSERT INTO "main"."battleattributekind" VALUES (5, '暴击率');
INSERT INTO "main"."battleattributekind" VALUES (6, '抗暴率');
INSERT INTO "main"."battleattributekind" VALUES (7, '增伤率(增加的伤害百分比)');
INSERT INTO "main"."battleattributekind" VALUES (8, '减伤率(减少的伤害百分比)');

-- ----------------------------
-- Table structure for "main"."buildsortprop"
-- ----------------------------
DROP TABLE "main"."buildsortprop";
CREATE TABLE "buildsortprop" (
"Kind"  smallint NOT NULL,
"Sort"  smallint NOT NULL,
"Introduction"  varchar NOT NULL,
PRIMARY KEY ("Kind" ASC),
CONSTRAINT "Sort" UNIQUE ("Sort")
);

-- ----------------------------
-- Records of buildsortprop
-- ----------------------------
INSERT INTO "main"."buildsortprop" VALUES (1, 1, '主城');
INSERT INTO "main"."buildsortprop" VALUES (6, 6, '竞技场');
INSERT INTO "main"."buildsortprop" VALUES (5, 5, '招募馆');
INSERT INTO "main"."buildsortprop" VALUES (4, 4, '铁匠铺');
INSERT INTO "main"."buildsortprop" VALUES (3, 3, '科技馆');
INSERT INTO "main"."buildsortprop" VALUES (2, 2, '民房');
INSERT INTO "main"."buildsortprop" VALUES (7, 7, 7);
INSERT INTO "main"."buildsortprop" VALUES (8, 8, 8);
INSERT INTO "main"."buildsortprop" VALUES (9, 9, 9);
INSERT INTO "main"."buildsortprop" VALUES (10, 10, 10);
INSERT INTO "main"."buildsortprop" VALUES (11, 11, 11);

-- ----------------------------
-- Table structure for "main"."cdprop"
-- ----------------------------
DROP TABLE "main"."cdprop";
CREATE TABLE "cdprop" (
"KindID"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"Level"  smallint NOT NULL,
"CD"  INTEGER NOT NULL,
PRIMARY KEY ("KindID" ASC)
);

-- ----------------------------
-- Records of cdprop
-- ----------------------------
INSERT INTO "main"."cdprop" VALUES (1, '强化', 1, 600);

-- ----------------------------
-- Table structure for "main"."drugprop"
-- ----------------------------
DROP TABLE "main"."drugprop";
CREATE TABLE "drugprop" (
"Level"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"CanUseNum"  smallint NOT NULL,
"BaseTalent"  smallint NOT NULL,
"DecTalent"  smallint NOT NULL,
"DrugItemKindID"  smallint NOT NULL,
PRIMARY KEY ("Level" ASC)
);

-- ----------------------------
-- Records of drugprop
-- ----------------------------
INSERT INTO "main"."drugprop" VALUES (1, '1级丹药使用提升', 2, 10, 5, 20001);
INSERT INTO "main"."drugprop" VALUES (2, '2级丹药使用提升', 3, 15, 5, 20002);
INSERT INTO "main"."drugprop" VALUES (3, '3级丹药使用提升', 4, 20, 5, 20003);
INSERT INTO "main"."drugprop" VALUES (4, '4级丹药使用提升', 5, 25, 5, 20004);
INSERT INTO "main"."drugprop" VALUES (5, '5级丹药使用提升', 6, 30, 5, 20005);
INSERT INTO "main"."drugprop" VALUES (6, '6级丹药使用提升', 7, 35, 5, 20006);

-- ----------------------------
-- Table structure for "main"."equipcolor"
-- ----------------------------
DROP TABLE "main"."equipcolor";
CREATE TABLE "equipcolor" (
"Color"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
PRIMARY KEY ("Color")
);

-- ----------------------------
-- Records of equipcolor
-- ----------------------------
INSERT INTO "main"."equipcolor" VALUES (1, '蓝色装备');
INSERT INTO "main"."equipcolor" VALUES (2, '紫色装备');
INSERT INTO "main"."equipcolor" VALUES (3, '红色装备');
INSERT INTO "main"."equipcolor" VALUES (4, '橙色装备');

-- ----------------------------
-- Table structure for "main"."equipposkind"
-- ----------------------------
DROP TABLE "main"."equipposkind";
CREATE TABLE "equipposkind" (
"KindID"   NOT NULL,
"Name"   NOT NULL,
PRIMARY KEY ("KindID")
);

-- ----------------------------
-- Records of equipposkind
-- ----------------------------
INSERT INTO "main"."equipposkind" VALUES (1, '头盔');
INSERT INTO "main"."equipposkind" VALUES (2, '战袍');
INSERT INTO "main"."equipposkind" VALUES (3, '武器');
INSERT INTO "main"."equipposkind" VALUES (4, '战靴');

-- ----------------------------
-- Table structure for "main"."equipprop"
-- ----------------------------
DROP TABLE "main"."equipprop";
CREATE TABLE "equipprop" (
"KindID"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"Introduction"  varchar(255) NOT NULL,
"PosKindID"  smallint NOT NULL,
"EquipColor"  smallint NOT NULL DEFAULT 1,
"DressLevel"  smallint NOT NULL DEFAULT 1,
"BuyCoin"  INTEGER NOT NULL,
"BuyGold"  INTEGER NOT NULL,
"RecoverPrice"  INTEGER NOT NULL,
"HP"  INTEGER NOT NULL DEFAULT 0,
"Attack"  INTEGER NOT NULL DEFAULT 0,
"StrengthenHPPerLvl"  INTEGER NOT NULL DEFAULT 0,
"StrengthenAttackPerLvl"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("KindID" ASC)
);

-- ----------------------------
-- Records of equipprop
-- ----------------------------
INSERT INTO "main"."equipprop" VALUES (1, '启蒙之剑', null, 3, 1, 1, 1, 0, 3000, 0, 100, 0, 10);
INSERT INTO "main"."equipprop" VALUES (2, '启蒙之仗', null, 3, 1, 1, 1, 0, 3000, 0, 100, 0, 10);
INSERT INTO "main"."equipprop" VALUES (3, '启蒙之弓', null, 3, 1, 1, 1, 0, 3000, 0, 100, 0, 10);
INSERT INTO "main"."equipprop" VALUES (4, '启蒙之盔', null, 1, 1, 1, 1, 0, 3000, 600, 40, 60, 4);
INSERT INTO "main"."equipprop" VALUES (5, '启蒙之甲', null, 2, 1, 1, 1, 0, 3000, 1000, 0, 100, 0);
INSERT INTO "main"."equipprop" VALUES (6, '启蒙之靴', null, 4, 1, 1, 1, 0, 3000, 400, 60, 40, 6);
INSERT INTO "main"."equipprop" VALUES (7, '封魔之剑', null, 3, 2, 20, 1, 0, 8000, 0, 300, 0, 15);
INSERT INTO "main"."equipprop" VALUES (8, '封魔之仗', null, 3, 2, 20, 1, 0, 8000, 0, 300, 0, 15);
INSERT INTO "main"."equipprop" VALUES (9, '封魔之弓', null, 3, 2, 20, 1, 0, 8000, 0, 300, 0, 15);
INSERT INTO "main"."equipprop" VALUES (10, '封魔之盔', null, 1, 2, 20, 1, 0, 8000, 1800, 120, 90, 6);
INSERT INTO "main"."equipprop" VALUES (11, '封魔之甲', null, 2, 2, 20, 1, 0, 8000, 3000, 0, 150, 0);
INSERT INTO "main"."equipprop" VALUES (12, '封魔之靴', null, 4, 2, 20, 1, 0, 8000, 1200, 180, 60, 9);
INSERT INTO "main"."equipprop" VALUES (13, '御龙神剑', null, 3, 3, 40, 1, 0, 15000, 0, 600, 0, 25);
INSERT INTO "main"."equipprop" VALUES (14, '御龙神仗', null, 3, 3, 40, 1, 0, 15000, 0, 600, 0, 25);
INSERT INTO "main"."equipprop" VALUES (15, '御龙神弓', null, 3, 3, 40, 1, 0, 15000, 0, 600, 0, 25);
INSERT INTO "main"."equipprop" VALUES (16, '御龙神盔', null, 1, 3, 40, 1, 0, 15000, 3600, 240, 150, 10);
INSERT INTO "main"."equipprop" VALUES (17, '御龙神甲', null, 2, 3, 40, 1, 0, 15000, 6000, 0, 250, 0);
INSERT INTO "main"."equipprop" VALUES (18, '御龙神靴', null, 4, 3, 40, 1, 0, 15000, 2400, 360, 100, 15);
INSERT INTO "main"."equipprop" VALUES (19, '无双圣剑', null, 3, 4, 60, 1, 0, 30000, 0, 1000, 0, 40);
INSERT INTO "main"."equipprop" VALUES (20, '无双圣仗', null, 3, 4, 60, 1, 0, 30000, 0, 1000, 0, 40);
INSERT INTO "main"."equipprop" VALUES (21, '无双圣弓', null, 3, 4, 60, 1, 0, 30000, 0, 1000, 0, 40);
INSERT INTO "main"."equipprop" VALUES (22, '无双圣盔', null, 1, 4, 60, 1, 0, 30000, 6000, 400, 240, 16);
INSERT INTO "main"."equipprop" VALUES (23, '无双圣甲', null, 2, 4, 60, 1, 0, 30000, 10000, 0, 400, 0);
INSERT INTO "main"."equipprop" VALUES (24, '无双圣靴', null, 4, 4, 60, 1, 0, 30000, 4000, 600, 160, 24);

-- ----------------------------
-- Table structure for "main"."formationprop"
-- ----------------------------
DROP TABLE "main"."formationprop";
CREATE TABLE "formationprop" (
"FormationIdx"  INTEGER NOT NULL,
"UnlockLevel"  INTEGER NOT NULL
);

-- ----------------------------
-- Records of formationprop
-- ----------------------------
INSERT INTO "main"."formationprop" VALUES (1, 1);
INSERT INTO "main"."formationprop" VALUES (2, 5);
INSERT INTO "main"."formationprop" VALUES (3, 20);
INSERT INTO "main"."formationprop" VALUES (4, 30);
INSERT INTO "main"."formationprop" VALUES (5, 40);

-- ----------------------------
-- Table structure for "main"."godweaponubaseprop"
-- ----------------------------
DROP TABLE "main"."godweaponubaseprop";
CREATE TABLE "godweaponubaseprop" (
"UpgradeQualityItemID"  smallint NOT NULL,
"GodWeaponRecoverRate"  smallint NOT NULL,
"QualityUpdateNum"  smallint NOT NULL DEFAULT 10
);

-- ----------------------------
-- Records of godweaponubaseprop
-- ----------------------------
INSERT INTO "main"."godweaponubaseprop" VALUES (30007, 90, 10);

-- ----------------------------
-- Table structure for "main"."godweaponupgradeprop"
-- ----------------------------
DROP TABLE "main"."godweaponupgradeprop";
CREATE TABLE "godweaponupgradeprop" (
"QualityLevel"  smallint NOT NULL,
"QualityName"  varchar(32) NOT NULL,
"RequirePlayerLevel"  smallint NOT NULL,
"UpgradeQualityItemNum"  smallint NOT NULL DEFAULT 0,
"BaseUpgradeScience"  INTEGER NOT NULL,
"IncUpgradeScience"  INTEGER NOT NULL,
"BaseAttack"  int NOT NULL,
"BaseHP"  int NOT NULL,
"IncAttack"  int NOT NULL,
"IncHP"  int NOT NULL,
"QualityUpgradeEncAttrKind"  smallint NOT NULL DEFAULT NULL,
"UpgradeQualityEncAttrValue"  smallint NOT NULL DEFAULT 1,
PRIMARY KEY ("QualityLevel" ASC)
);

-- ----------------------------
-- Records of godweaponupgradeprop
-- ----------------------------
INSERT INTO "main"."godweaponupgradeprop" VALUES (1, '凡品', 1, 0, 50, 40, 150, 1500, 14, 140, 3, 1);
INSERT INTO "main"."godweaponupgradeprop" VALUES (2, '灵品', 20, 10, 500, 60, 250, 2500, 36, 360, 4, 1);
INSERT INTO "main"."godweaponupgradeprop" VALUES (3, '仙品', 30, 20, 1500, 80, 470, 4700, 60, 600, 5, 1);
INSERT INTO "main"."godweaponupgradeprop" VALUES (4, '神品', 40, 30, 3500, 100, 820, 8200, 100, 1000, 6, 1);
INSERT INTO "main"."godweaponupgradeprop" VALUES (5, '圣品', 50, 40, 7000, 120, 1400, 14000, 150, 1500, 7, 1);
INSERT INTO "main"."godweaponupgradeprop" VALUES (6, '无品', 51, 50, 8000, 150, 1800, 18000, 180, 1800, 8, 1);

-- ----------------------------
-- Table structure for "main"."goodsmainkind"
-- ----------------------------
DROP TABLE "main"."goodsmainkind";
CREATE TABLE "goodsmainkind" (
"Kind"  smallint NOT NULL,
"Name"  char(32) NOT NULL,
"Introduction"  varchar(255) NOT NULL,
PRIMARY KEY ("Kind")
);

-- ----------------------------
-- Records of goodsmainkind
-- ----------------------------
INSERT INTO "main"."goodsmainkind" VALUES (1, '加体力', '增加玩家体力');
INSERT INTO "main"."goodsmainkind" VALUES (2, '强化', '不需金钱可强化装备');
INSERT INTO "main"."goodsmainkind" VALUES (3, '经验双倍', '在此过程中打怪经验双倍');
INSERT INTO "main"."goodsmainkind" VALUES (4, '加经验', '增加武将经验');
INSERT INTO "main"."goodsmainkind" VALUES (5, '材料', '某些操作必须的材料');
INSERT INTO "main"."goodsmainkind" VALUES (6, '神秘礼包', '随机获得1个道具');
INSERT INTO "main"."goodsmainkind" VALUES (7, '金币', '给玩家金币');
INSERT INTO "main"."goodsmainkind" VALUES (8, '宝箱', '打开后给予玩家宝箱内的道具');
INSERT INTO "main"."goodsmainkind" VALUES (9, '丹药', '使用后增加修为');
INSERT INTO "main"."goodsmainkind" VALUES (10, '制作卷', '各种材料的制作卷
');
INSERT INTO "main"."goodsmainkind" VALUES (99, '其他', '其他');
INSERT INTO "main"."goodsmainkind" VALUES (11, '器魂', '使用后增加器魂点');
INSERT INTO "main"."goodsmainkind" VALUES (12, '内部宝箱', '给该物品时系统直接给玩家使用');

-- ----------------------------
-- Table structure for "main"."goodsprop"
-- ----------------------------
DROP TABLE "main"."goodsprop";
CREATE TABLE "goodsprop" (
"KindID"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"Introduction"  varchar(255) NOT NULL,
"MainKindID"  smallint NOT NULL,
"UseLevel"  smallint NOT NULL DEFAULT 1,
"BuyCoin"  INTEGER NOT NULL,
"BuyGold"  INTEGER NOT NULL,
"RecoverPrice"  smallint NOT NULL,
"UseTimesPerDay"  smallint NOT NULL,
"UseCD"  smallint NOT NULL,
"Value"  smallint NOT NULL,
PRIMARY KEY ("KindID" ASC)
);

-- ----------------------------
-- Records of goodsprop
-- ----------------------------
INSERT INTO "main"."goodsprop" VALUES (20001, '一品修为丹', null, 9, 0, 10000, 0, 1000, 0, 0, 1);
INSERT INTO "main"."goodsprop" VALUES (20002, '二品修为丹', null, 9, 0, 30000, 0, 3000, 0, 0, 2);
INSERT INTO "main"."goodsprop" VALUES (20003, '三品修为丹', null, 9, 0, 100000, 0, 10000, 0, 0, 3);
INSERT INTO "main"."goodsprop" VALUES (20004, '四品修为丹', null, 9, 0, 1, 0, 50000, 0, 0, 4);
INSERT INTO "main"."goodsprop" VALUES (20005, '五品修为丹', null, 9, 0, 1, 0, 100000, 0, 0, 5);
INSERT INTO "main"."goodsprop" VALUES (20006, '六品修为丹', null, 9, 0, 1, 0, 200000, 0, 0, 6);
INSERT INTO "main"."goodsprop" VALUES (20028, '犀角', '20级武器合成材料', 5, 0, 1, 0, 20, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20029, '水妖牙', '20级武器合成材料', 5, 0, 1, 0, 20, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20030, '赤蚌珠', '20级武器合成材料', 5, 0, 1, 0, 20, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20031, '蟹甲', '20级防具合成材料', 5, 0, 1, 0, 20, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20032, '龟板', '20级防具合成材料', 5, 0, 1, 0, 20, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20033, '暴熊皮', '20级防具合成材料', 5, 0, 1, 0, 20, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20034, '驱邪珠', '40级武器合成材料', 5, 0, 1, 0, 30, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20035, '云栈石', '40级武器合成材料', 5, 0, 1, 0, 30, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20036, '天辰钢', '40级武器合成材料', 5, 0, 1, 0, 30, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20037, '月幽绒', '40级防具合成材料（绒布）', 5, 0, 1, 0, 30, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20038, '雪绒丝', '40级防具合成材料（丝卷）', 5, 0, 1, 0, 30, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20039, '黑煞珠', '40级防具合成材料', 5, 0, 1, 0, 30, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20040, '白玉骨', '60级武器合成材料（骨头一枚）', 5, 0, 1, 0, 50, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20041, '火赤金', '60级武器合成材料（被火烧亮的金矿）', 5, 0, 1, 0, 50, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20042, '逐日石', '60级武器合成材料（在高温中烫得通红）', 5, 0, 1, 0, 50, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20043, '鬼哭草', '60级防具合成材料（色调暗沉的草）', 5, 0, 1, 0, 50, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20044, '仙灵花', '60级防具合成材料', 5, 0, 1, 0, 50, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20045, '积雷岩', '60级防具合成材料（隐隐发出电光）', 5, 0, 1, 0, 50, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20046, '金福禄袋', '使用后获得100元宝', 2, 0, 1, 0, 0, 0, 0, 100);
INSERT INTO "main"."goodsprop" VALUES (20047, '小铜钱袋', '使用后获得5000铜币', 7, 0, 1, 0, 0, 0, 0, 5000);
INSERT INTO "main"."goodsprop" VALUES (20048, '中铜钱袋', '使用后获得50000铜币', 7, 0, 1, 0, 0, 0, 0, 50000);
INSERT INTO "main"."goodsprop" VALUES (20049, '超大铜钱袋', '使用后获得500000铜币', 7, 0, 1, 0, 0, 0, 0, 500000);
INSERT INTO "main"."goodsprop" VALUES (20050, '吉祥铜钱箱', '使用后获得1000000铜币', 7, 0, 1, 0, 0, 0, 0, 1000000);
INSERT INTO "main"."goodsprop" VALUES (20051, '如意铜钱箱', '使用后获得2000000铜币', 7, 0, 1, 0, 0, 0, 0, 2000000);
INSERT INTO "main"."goodsprop" VALUES (20052, '大铜钱袋', '使用后获得100000铜币', 7, 0, 1, 0, 0, 0, 0, 100000);
INSERT INTO "main"."goodsprop" VALUES (20053, '小铜钱箱', '使用后获得10000铜币', 7, 0, 1, 0, 0, 0, 0, 10000);
INSERT INTO "main"."goodsprop" VALUES (20054, '金身傀儡', '可在敌人刺杀成功时，顶替我方将领迷惑敌人，金身破灭一次，下一次所需的数量加1', 99, 0, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20055, null, '获得一次刺杀低于你5~10等级的玩家一次机会', 99, 0, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20056, null, '获得一次刺杀低于你5~15等级的玩家一次机会', 99, 0, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20057, null, '获得一次刺杀低于你5~20等级的玩家一次机会', 99, 0, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20058, '跨级掠夺符', '能对任意等级的玩家进行斗法', 99, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (20059, '双倍经验符+1', '使用后双倍经验场次数+1', 3, 0, 1, 0, 0, 0, 0, 1);
INSERT INTO "main"."goodsprop" VALUES (20060, '双倍经验符+5', '使用后双倍经验场次数+5', 3, 0, 1, 0, 0, 0, 0, 5);
INSERT INTO "main"."goodsprop" VALUES (20061, '双倍经验符+10', '使用后双倍经验场次数+10', 3, 0, 1, 0, 0, 0, 0, 10);
INSERT INTO "main"."goodsprop" VALUES (20062, null, '使用后增加刺杀次数', 99, 0, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (30001, '测试体力+10', null, 1, 0, 1, 0, 0, 0, 0, 10);
INSERT INTO "main"."goodsprop" VALUES (30002, '测试体力+1000', null, 1, 0, 1, 0, 0, 0, 0, 1000);
INSERT INTO "main"."goodsprop" VALUES (30003, '测试经验+100', null, 4, 0, 1, 0, 0, 0, 0, 100);
INSERT INTO "main"."goodsprop" VALUES (30004, '测试经验+1000', null, 4, 0, 1, 0, 0, 0, 0, 1000);
INSERT INTO "main"."goodsprop" VALUES (30005, '双倍经验1次', null, 3, 0, 1, 0, 0, 0, 0, 1);
INSERT INTO "main"."goodsprop" VALUES (30006, '双倍经验5次', null, 3, 0, 1, 0, 0, 0, 0, 5);
INSERT INTO "main"."goodsprop" VALUES (30007, '进阶符', '神器进阶时所需的道具', 99, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (40004, '四品修为丹制作卷', null, 10, 0, 1, 0, 8000, 0, 0, 20004);
INSERT INTO "main"."goodsprop" VALUES (40005, '五品修为丹制作卷', null, 10, 0, 1, 0, 12000, 0, 0, 20005);
INSERT INTO "main"."goodsprop" VALUES (40006, '六品修为丹制作卷', null, 10, 0, 1, 0, 24000, 0, 0, 20006);
INSERT INTO "main"."goodsprop" VALUES (40007, '封魔之剑制作卷', null, 10, 0, 1, 0, 5000, 0, 0, 7);
INSERT INTO "main"."goodsprop" VALUES (40008, '封魔之仗制作卷', null, 10, 0, 1, 0, 5000, 0, 0, 8);
INSERT INTO "main"."goodsprop" VALUES (40009, '封魔之弓制作卷', null, 10, 0, 1, 0, 5000, 0, 0, 9);
INSERT INTO "main"."goodsprop" VALUES (40010, '封魔之盔制作卷', null, 10, 0, 1, 0, 5000, 0, 0, 10);
INSERT INTO "main"."goodsprop" VALUES (40011, '封魔之甲制作卷', null, 10, 0, 1, 0, 5000, 0, 0, 11);
INSERT INTO "main"."goodsprop" VALUES (40012, '封魔之靴制作卷', null, 10, 0, 1, 0, 5000, 0, 0, 12);
INSERT INTO "main"."goodsprop" VALUES (40013, '御龙神剑制作卷', null, 10, 0, 1, 0, 10000, 0, 0, 13);
INSERT INTO "main"."goodsprop" VALUES (40014, '御龙神仗制作卷', null, 10, 0, 1, 0, 10000, 0, 0, 14);
INSERT INTO "main"."goodsprop" VALUES (40015, '御龙神弓制作卷', null, 10, 0, 1, 0, 10000, 0, 0, 15);
INSERT INTO "main"."goodsprop" VALUES (40016, '御龙神盔制作卷', null, 10, 0, 1, 0, 10000, 0, 0, 16);
INSERT INTO "main"."goodsprop" VALUES (40017, '御龙神甲制作卷', null, 10, 0, 1, 0, 10000, 0, 0, 17);
INSERT INTO "main"."goodsprop" VALUES (40018, '御龙神靴制作卷', null, 10, 0, 1, 0, 10000, 0, 0, 18);
INSERT INTO "main"."goodsprop" VALUES (40019, '无双圣剑制作卷', null, 10, 0, 1, 0, 20000, 0, 0, 19);
INSERT INTO "main"."goodsprop" VALUES (40020, '无双圣仗制作卷', null, 10, 0, 1, 0, 20000, 0, 0, 20);
INSERT INTO "main"."goodsprop" VALUES (40021, '无双圣弓制作卷', null, 10, 0, 1, 0, 20000, 0, 0, 21);
INSERT INTO "main"."goodsprop" VALUES (40022, '无双圣盔制作卷', null, 10, 0, 1, 0, 20000, 0, 0, 22);
INSERT INTO "main"."goodsprop" VALUES (40023, '无双圣甲制作卷', null, 10, 0, 1, 0, 20000, 0, 0, 23);
INSERT INTO "main"."goodsprop" VALUES (40024, '无双圣靴制作卷', null, 10, 0, 1, 0, 20000, 0, 0, 24);
INSERT INTO "main"."goodsprop" VALUES (50001, '启蒙宝箱', null, 6, 0, 1, 0, 5000, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51000, '10级成长礼包', null, 8, 10, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51001, '20级成长礼包', null, 8, 20, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51002, '30级成长礼包', null, 8, 30, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51003, '40级成长礼包', null, 8, 40, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51004, '50级成长礼包', null, 8, 50, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51005, '新手大礼包', null, 8, 1, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (51006, '60级成长礼包', null, 8, 60, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (52001, '普通技能卡', '普通技能卡', 99, 1, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (52002, '稀有技能卡', '稀有技能卡', 99, 1, 1, 0, 0, 0, 0, 0);
INSERT INTO "main"."goodsprop" VALUES (54001, '器魂点100', '器魂点100', 11, 1, 1, 0, 0, 0, 0, 0);

-- ----------------------------
-- Table structure for "main"."heroprop"
-- ----------------------------
DROP TABLE "main"."heroprop";
CREATE TABLE "heroprop" (
"KindID"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"Talent"  smallint NOT NULL DEFAULT 1,
"HitRate"  smallint NOT NULL DEFAULT 0,
"DodgeRate"  smallint NOT NULL DEFAULT 0,
"CritRate"  smallint NOT NULL DEFAULT 0,
"DeCritRate"  smallint NOT NULL DEFAULT 0,
"VisitCoin"  INTEGER NOT NULL DEFAULT 100,
"VisitNum"  smallint NOT NULL,
PRIMARY KEY ("KindID" ASC)
);

-- ----------------------------
-- Records of heroprop
-- ----------------------------
INSERT INTO "main"."heroprop" VALUES (1, '战士', 200, 100, 20, 30, 10, 0, 0);
INSERT INTO "main"."heroprop" VALUES (2, '法师', 200, 100, 20, 30, 10, 0, 0);
INSERT INTO "main"."heroprop" VALUES (3, '弓手', 200, 100, 20, 30, 10, 0, 0);
INSERT INTO "main"."heroprop" VALUES (101, '熊魁', 150, 0, 0, 0, 0, 500, 1);
INSERT INTO "main"."heroprop" VALUES (102, '酒剑仙', 150, 0, 0, 0, 0, 1000, 1);
INSERT INTO "main"."heroprop" VALUES (103, '镇元子', 150, 0, 0, 0, 0, 5000, 1);
INSERT INTO "main"."heroprop" VALUES (104, '多吉', 150, 0, 0, 0, 0, 10000, 2);
INSERT INTO "main"."heroprop" VALUES (105, '冰漓', 200, 0, 0, 0, 0, 15000, 3);
INSERT INTO "main"."heroprop" VALUES (106, '玉面书生', 200, 0, 0, 0, 0, 25000, 4);
INSERT INTO "main"."heroprop" VALUES (107, '敖广', 200, 0, 0, 0, 0, 50000, 5);
INSERT INTO "main"."heroprop" VALUES (108, '嫦娥', 200, 0, 0, 0, 0, 100000, 6);
INSERT INTO "main"."heroprop" VALUES (109, '李靖', 300, 0, 0, 0, 0, 200000, 7);
INSERT INTO "main"."heroprop" VALUES (110, '哪吒', 300, 0, 0, 0, 0, 300000, 8);
INSERT INTO "main"."heroprop" VALUES (111, '红孩儿', 300, 0, 0, 0, 0, 500000, 9);
INSERT INTO "main"."heroprop" VALUES (112, '牛魔王', 300, 0, 0, 0, 0, 1000000, 10);
INSERT INTO "main"."heroprop" VALUES (113, '慕容雪', 150, 0, 0, 0, 0, 2000, 0);
INSERT INTO "main"."heroprop" VALUES (4, '女战士', 200, 100, 20, 30, 10, 0, 0);
INSERT INTO "main"."heroprop" VALUES (5, '女法师', 200, 100, 20, 30, 10, 0, 0);
INSERT INTO "main"."heroprop" VALUES (6, '女弓手', 200, 100, 20, 30, 10, 0, 0);

-- ----------------------------
-- Table structure for "main"."heroupgradeprop"
-- ----------------------------
DROP TABLE "main"."heroupgradeprop";
CREATE TABLE "heroupgradeprop" (
"Level"  smallint NOT NULL,
"Experience"  int NOT NULL,
PRIMARY KEY ("Level" ASC)
);

-- ----------------------------
-- Records of heroupgradeprop
-- ----------------------------
INSERT INTO "main"."heroupgradeprop" VALUES (1, 0);
INSERT INTO "main"."heroupgradeprop" VALUES (2, 700);
INSERT INTO "main"."heroupgradeprop" VALUES (3, 1900);
INSERT INTO "main"."heroupgradeprop" VALUES (4, 4300);
INSERT INTO "main"."heroupgradeprop" VALUES (5, 8500);
INSERT INTO "main"."heroupgradeprop" VALUES (6, 15100);
INSERT INTO "main"."heroupgradeprop" VALUES (7, 24700);
INSERT INTO "main"."heroupgradeprop" VALUES (8, 37900);
INSERT INTO "main"."heroupgradeprop" VALUES (9, 55300);
INSERT INTO "main"."heroupgradeprop" VALUES (10, 77500);
INSERT INTO "main"."heroupgradeprop" VALUES (11, 105100);
INSERT INTO "main"."heroupgradeprop" VALUES (12, 138700);
INSERT INTO "main"."heroupgradeprop" VALUES (13, 178900);
INSERT INTO "main"."heroupgradeprop" VALUES (14, 226300);
INSERT INTO "main"."heroupgradeprop" VALUES (15, 281500);
INSERT INTO "main"."heroupgradeprop" VALUES (16, 345100);
INSERT INTO "main"."heroupgradeprop" VALUES (17, 417700);
INSERT INTO "main"."heroupgradeprop" VALUES (18, 499900);
INSERT INTO "main"."heroupgradeprop" VALUES (19, 592300);
INSERT INTO "main"."heroupgradeprop" VALUES (20, 695500);
INSERT INTO "main"."heroupgradeprop" VALUES (21, 810100);
INSERT INTO "main"."heroupgradeprop" VALUES (22, 936700);
INSERT INTO "main"."heroupgradeprop" VALUES (23, 1075900);
INSERT INTO "main"."heroupgradeprop" VALUES (24, 1228300);
INSERT INTO "main"."heroupgradeprop" VALUES (25, 1394500);
INSERT INTO "main"."heroupgradeprop" VALUES (26, 1575100);
INSERT INTO "main"."heroupgradeprop" VALUES (27, 1770700);
INSERT INTO "main"."heroupgradeprop" VALUES (28, 1981900);
INSERT INTO "main"."heroupgradeprop" VALUES (29, 2209300);
INSERT INTO "main"."heroupgradeprop" VALUES (30, 2453500);
INSERT INTO "main"."heroupgradeprop" VALUES (31, 2715100);
INSERT INTO "main"."heroupgradeprop" VALUES (32, 2994700);
INSERT INTO "main"."heroupgradeprop" VALUES (33, 3292900);
INSERT INTO "main"."heroupgradeprop" VALUES (34, 3610300);
INSERT INTO "main"."heroupgradeprop" VALUES (35, 3947500);
INSERT INTO "main"."heroupgradeprop" VALUES (36, 4305100);
INSERT INTO "main"."heroupgradeprop" VALUES (37, 4683700);
INSERT INTO "main"."heroupgradeprop" VALUES (38, 5083900);
INSERT INTO "main"."heroupgradeprop" VALUES (39, 5506300);
INSERT INTO "main"."heroupgradeprop" VALUES (40, 5951500);
INSERT INTO "main"."heroupgradeprop" VALUES (41, 6420100);
INSERT INTO "main"."heroupgradeprop" VALUES (42, 6912700);
INSERT INTO "main"."heroupgradeprop" VALUES (43, 7429900);
INSERT INTO "main"."heroupgradeprop" VALUES (44, 7972300);
INSERT INTO "main"."heroupgradeprop" VALUES (45, 8540500);
INSERT INTO "main"."heroupgradeprop" VALUES (46, 9135100);
INSERT INTO "main"."heroupgradeprop" VALUES (47, 9756700);
INSERT INTO "main"."heroupgradeprop" VALUES (48, 10405900);
INSERT INTO "main"."heroupgradeprop" VALUES (49, 11083300);
INSERT INTO "main"."heroupgradeprop" VALUES (50, 11789500);
INSERT INTO "main"."heroupgradeprop" VALUES (51, 12525100);
INSERT INTO "main"."heroupgradeprop" VALUES (52, 13290700);
INSERT INTO "main"."heroupgradeprop" VALUES (53, 14086900);
INSERT INTO "main"."heroupgradeprop" VALUES (54, 14914300);
INSERT INTO "main"."heroupgradeprop" VALUES (55, 15773500);
INSERT INTO "main"."heroupgradeprop" VALUES (56, 16665100);
INSERT INTO "main"."heroupgradeprop" VALUES (57, 17589700);
INSERT INTO "main"."heroupgradeprop" VALUES (58, 18547900);
INSERT INTO "main"."heroupgradeprop" VALUES (59, 19540300);
INSERT INTO "main"."heroupgradeprop" VALUES (60, 20567500);

-- ----------------------------
-- Table structure for "main"."itemcompose"
-- ----------------------------
DROP TABLE "main"."itemcompose";
CREATE TABLE "itemcompose" (
"KindID"  smallint NOT NULL,
"SubKindID"  smallint NOT NULL,
"Num"  smallint NOT NULL,
"Rate"  smallint NOT NULL DEFAULT 100,
PRIMARY KEY ("KindID" ASC, "SubKindID" ASC, "Num" ASC)
);

-- ----------------------------
-- Records of itemcompose
-- ----------------------------
INSERT INTO "main"."itemcompose" VALUES (40004, 20001, 8, 0);
INSERT INTO "main"."itemcompose" VALUES (40004, 20002, 4, 0);
INSERT INTO "main"."itemcompose" VALUES (40004, 20003, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (40005, 20001, 20, 0);
INSERT INTO "main"."itemcompose" VALUES (40005, 20002, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40005, 20003, 5, 0);
INSERT INTO "main"."itemcompose" VALUES (40006, 20001, 48, 0);
INSERT INTO "main"."itemcompose" VALUES (40006, 20002, 24, 0);
INSERT INTO "main"."itemcompose" VALUES (40006, 20003, 12, 0);
INSERT INTO "main"."itemcompose" VALUES (40007, 20028, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40007, 20029, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40007, 20030, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40008, 20028, 12, 0);
INSERT INTO "main"."itemcompose" VALUES (40008, 20029, 15, 0);
INSERT INTO "main"."itemcompose" VALUES (40008, 20030, 15, 0);
INSERT INTO "main"."itemcompose" VALUES (40009, 20028, 12, 0);
INSERT INTO "main"."itemcompose" VALUES (40009, 20029, 15, 0);
INSERT INTO "main"."itemcompose" VALUES (40009, 20030, 15, 0);
INSERT INTO "main"."itemcompose" VALUES (40010, 20031, 12, 0);
INSERT INTO "main"."itemcompose" VALUES (40010, 20032, 15, 0);
INSERT INTO "main"."itemcompose" VALUES (40010, 20033, 15, 0);
INSERT INTO "main"."itemcompose" VALUES (40011, 20031, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40011, 20032, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40011, 20033, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40012, 20031, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40012, 20033, 10, 0);
INSERT INTO "main"."itemcompose" VALUES (40012, 20033, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40013, 20034, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40013, 20035, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40013, 20036, 30, 0);
INSERT INTO "main"."itemcompose" VALUES (40014, 20034, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40014, 20035, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40014, 20036, 30, 0);
INSERT INTO "main"."itemcompose" VALUES (40015, 20034, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40015, 20035, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40015, 20036, 30, 0);
INSERT INTO "main"."itemcompose" VALUES (40016, 20037, 30, 0);
INSERT INTO "main"."itemcompose" VALUES (40016, 20038, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40016, 20039, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40017, 20037, 30, 0);
INSERT INTO "main"."itemcompose" VALUES (40017, 20038, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40017, 20039, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40018, 20037, 30, 0);
INSERT INTO "main"."itemcompose" VALUES (40018, 20038, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40018, 20039, 25, 0);
INSERT INTO "main"."itemcompose" VALUES (40019, 20040, 40, 0);
INSERT INTO "main"."itemcompose" VALUES (40019, 20041, 40, 0);
INSERT INTO "main"."itemcompose" VALUES (40019, 20042, 50, 0);
INSERT INTO "main"."itemcompose" VALUES (40020, 20040, 40, 0);
INSERT INTO "main"."itemcompose" VALUES (40020, 20041, 40, 0);
INSERT INTO "main"."itemcompose" VALUES (40020, 20042, 50, 0);
INSERT INTO "main"."itemcompose" VALUES (40021, 20040, 40, 0);
INSERT INTO "main"."itemcompose" VALUES (40021, 20041, 40, 0);
INSERT INTO "main"."itemcompose" VALUES (40021, 20042, 50, 0);
INSERT INTO "main"."itemcompose" VALUES (40022, 20043, 45, 0);
INSERT INTO "main"."itemcompose" VALUES (40022, 20044, 45, 0);
INSERT INTO "main"."itemcompose" VALUES (40022, 20045, 55, 0);
INSERT INTO "main"."itemcompose" VALUES (40023, 20043, 45, 0);
INSERT INTO "main"."itemcompose" VALUES (40023, 20044, 45, 0);
INSERT INTO "main"."itemcompose" VALUES (40023, 20045, 55, 0);
INSERT INTO "main"."itemcompose" VALUES (40024, 20043, 45, 0);
INSERT INTO "main"."itemcompose" VALUES (40024, 20044, 45, 0);
INSERT INTO "main"."itemcompose" VALUES (40024, 20045, 55, 0);
INSERT INTO "main"."itemcompose" VALUES (50001, 1, 1, 13);
INSERT INTO "main"."itemcompose" VALUES (50001, 2, 1, 13);
INSERT INTO "main"."itemcompose" VALUES (50001, 3, 1, 13);
INSERT INTO "main"."itemcompose" VALUES (50001, 4, 1, 13);
INSERT INTO "main"."itemcompose" VALUES (50001, 5, 1, 13);
INSERT INTO "main"."itemcompose" VALUES (50001, 6, 1, 13);
INSERT INTO "main"."itemcompose" VALUES (50001, 20001, 1, 12);
INSERT INTO "main"."itemcompose" VALUES (50001, 20002, 1, 10);
INSERT INTO "main"."itemcompose" VALUES (51000, 51001, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51000, 20047, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (51000, 1, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51000, 4, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51000, 5, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51000, 6, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51001, 51002, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51001, 20048, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51001, 20001, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (51002, 51003, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51002, 20052, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51002, 20002, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (51003, 51004, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51003, 20049, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51003, 20003, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (51004, 51006, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51004, 20050, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51004, 20004, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (51005, 51000, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51005, 20001, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (51005, 20047, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51005, 20046, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51006, 20051, 1, 0);
INSERT INTO "main"."itemcompose" VALUES (51006, 20005, 2, 0);
INSERT INTO "main"."itemcompose" VALUES (40008, 2, 1, 100);
INSERT INTO "main"."itemcompose" VALUES (40009, 3, 1, 0);

-- ----------------------------
-- Table structure for "main"."itemkind"
-- ----------------------------
DROP TABLE "main"."itemkind";
CREATE TABLE "itemkind"(
  Kind smallint unsigned NOT NULL,
  Name varchar(32) NOT NULL,
  Introduction varchar unsigned NOT NULL,
  PRIMARY KEY(Kind));

-- ----------------------------
-- Records of itemkind
-- ----------------------------
INSERT INTO "main"."itemkind" VALUES (1, '装备', '操作方法:装备，效果：增加武将攻击属性');
INSERT INTO "main"."itemkind" VALUES (3, '武将', '操作方法:使用，效果：赋予玩家武将');
INSERT INTO "main"."itemkind" VALUES (4, '强化符', '操作方法:使用，效果：不需用货币也可强化');
INSERT INTO "main"."itemkind" VALUES (2, '材料', '操作方法:使用，效果：赋予玩家道具');

-- ----------------------------
-- Table structure for "main"."loginencourage"
-- ----------------------------
DROP TABLE "main"."loginencourage";
CREATE TABLE "loginencourage" (
"KindID"  smallint NOT NULL,
"Num"  smallint NOT NULL,
"Rate"  smallint NOT NULL,
PRIMARY KEY ("KindID" ASC, "Num" ASC)
);

-- ----------------------------
-- Records of loginencourage
-- ----------------------------
INSERT INTO "main"."loginencourage" VALUES (20053, 1, 25);
INSERT INTO "main"."loginencourage" VALUES (20048, 1, 10);
INSERT INTO "main"."loginencourage" VALUES (20059, 1, 10);
INSERT INTO "main"."loginencourage" VALUES (20058, 1, 5);
INSERT INTO "main"."loginencourage" VALUES (30007, 1, 5);
INSERT INTO "main"."loginencourage" VALUES (50001, 1, 25);
INSERT INTO "main"."loginencourage" VALUES (54001, 1, 10);
INSERT INTO "main"."loginencourage" VALUES (30001, 1, 10);

-- ----------------------------
-- Table structure for "main"."maintowncrownprop"
-- ----------------------------
DROP TABLE "main"."maintowncrownprop";
CREATE TABLE "maintowncrownprop" (
"PlayerLevel"  smallint NOT NULL,
"CrownCoin"  INTEGER NOT NULL,
"CrownGold"  INTEGER NOT NULL,
"LevyCoin"  INTEGER NOT NULL,
PRIMARY KEY ("PlayerLevel" ASC)
);

-- ----------------------------
-- Records of maintowncrownprop
-- ----------------------------
INSERT INTO "main"."maintowncrownprop" VALUES (1, 10000, 1, 1000);
INSERT INTO "main"."maintowncrownprop" VALUES (60, 200000, 100, 50000);

-- ----------------------------
-- Table structure for "main"."maintownlevyprop"
-- ----------------------------
DROP TABLE "main"."maintownlevyprop";
CREATE TABLE "maintownlevyprop" (
"LevyNumPerDay"  smallint NOT NULL,
"LevyCD"   NOT NULL
);

-- ----------------------------
-- Records of maintownlevyprop
-- ----------------------------
INSERT INTO "main"."maintownlevyprop" VALUES (15, 300);

-- ----------------------------
-- Table structure for "main"."moneytreeprop"
-- ----------------------------
DROP TABLE "main"."moneytreeprop";
CREATE TABLE "moneytreeprop" (
"RockNumPerDay"  smallint NOT NULL,
"BaseGold"  smallint NOT NULL,
"IncGold"  smallint NOT NULL,
"MaxGold"  smallint NOT NULL,
"IncCoinPerMoodRate"  smallint NOT NULL DEFAULT 20,
"MaxMoodValue"  smallint NOT NULL,
"GetMoodRate"  smallint NOT NULL,
"MoodKeepTime"  smallint NOT NULL,
PRIMARY KEY ("RockNumPerDay" ASC)
);

-- ----------------------------
-- Records of moneytreeprop
-- ----------------------------
INSERT INTO "main"."moneytreeprop" VALUES (10, 1, 1, 10, 10, 5, 20, 300);

-- ----------------------------
-- Table structure for "main"."monsterprop"
-- ----------------------------
DROP TABLE "main"."monsterprop";
CREATE TABLE "monsterprop" (
"KindID"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"Type"  smallint NOT NULL DEFAULT 1,
"Level"  smallint NOT NULL DEFAULT 1,
"HitRate"  smallint NOT NULL DEFAULT 0,
"DodgeRate"  smallint NOT NULL DEFAULT 0,
"CritRate"  smallint NOT NULL DEFAULT 0,
"DeCritRate"  smallint NOT NULL DEFAULT 0,
"AttackSkillID1"  smallint NOT NULL DEFAULT 0,
"AttackSkillLevel1"  smallint NOT NULL DEFAULT 1,
"AttackSkillID2"  smallint NOT NULL DEFAULT 0,
"AttackSkillLevel2"  smallint NOT NULL DEFAULT 0,
"DefenseSkillID"  smallint NOT NULL DEFAULT 0,
"DefenseSkillLevel"  smallint NOT NULL DEFAULT 0,
PRIMARY KEY ("KindID" ASC)
);

-- ----------------------------
-- Records of monsterprop
-- ----------------------------
INSERT INTO "main"."monsterprop" VALUES (1, '如意童子', 1, 1, 100, 20, 30, 10, 1001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (2, '如意童女', 1, 2, 100, 20, 30, 10, 1002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (3, '太云仙童', 1, 3, 100, 20, 30, 10, 1003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (4, '火麒麟', 2, 3, 102, 22, 32, 12, 1004, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (5, '巡捕甲', 1, 4, 100, 20, 30, 10, 10001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (6, '巡捕乙', 1, 5, 100, 20, 30, 10, 10002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (7, '捕头', 1, 6, 100, 20, 30, 10, 10003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (8, '厉无命', 2, 6, 105, 25, 35, 15, 10004, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (9, '蜘蛛', 1, 7, 100, 20, 30, 10, 20001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (10, '蝎子', 1, 8, 100, 20, 30, 10, 20002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (11, '甲虫', 1, 9, 100, 20, 30, 10, 20003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (12, '慕容雪', 2, 9, 108, 28, 38, 18, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (13, '蜘蛛', 1, 10, 100, 20, 30, 10, 1, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (14, '蝎子', 1, 11, 101, 21, 30, 10, 2, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (15, '甲虫', 1, 12, 102, 22, 30, 10, 3, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (16, '毒蟾', 2, 12, 111, 31, 41, 21, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (17, '王府侍卫甲', 1, 13, 103, 23, 30, 10, 1001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (18, '王府侍卫乙', 1, 14, 104, 24, 30, 10, 1002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (19, '王府食客', 1, 15, 105, 25, 30, 10, 1003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (20, '并肩王', 2, 15, 114, 34, 44, 24, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (21, '蝎子', 1, 16, 106, 26, 30, 10, 10001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (22, '山猪', 1, 17, 107, 27, 30, 10, 10002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (23, '龅牙武士', 1, 18, 108, 28, 30, 10, 10003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (24, '熊魁', 2, 18, 117, 37, 47, 27, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (25, '蜘蛛', 1, 19, 109, 29, 30, 10, 20001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (26, '蝙蝠', 1, 20, 110, 30, 30, 10, 20002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (27, '蝙蝠', 1, 19, 109, 29, 30, 10, 20003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (29, '龅牙武士', 1, 20, 110, 30, 30, 10, 20004, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (30, '鬼虎', 2, 20, 119, 39, 49, 29, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (31, '甲虫', 1, 21, 111, 31, 31, 11, 2, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (32, '蜘蛛', 1, 22, 112, 32, 32, 12, 3, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (33, '蜘蛛', 1, 21, 111, 31, 31, 11, 4, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (34, '蝎子', 1, 22, 112, 32, 32, 12, 1001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (36, '酒剑仙', 2, 22, 121, 41, 51, 31, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (37, '蝎子', 1, 23, 113, 33, 33, 13, 1003, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (38, '小蛇', 1, 24, 114, 34, 34, 14, 1004, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (39, '小蛇', 1, 23, 113, 33, 33, 13, 10001, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (41, '山猪', 1, 24, 114, 34, 34, 14, 10002, 1, 0, 0, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (42, '红眼魔熊', 2, 24, 123, 43, 53, 33, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (43, '蜘蛛', 1, 25, 115, 35, 35, 15, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (44, '小蛇', 1, 26, 116, 36, 36, 16, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (45, '小蛇', 1, 25, 115, 35, 35, 15, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (47, '龅牙武士', 1, 26, 116, 36, 36, 16, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (48, '金鼻鼠精', 2, 26, 125, 45, 55, 35, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (49, '蝎子', 1, 27, 117, 37, 37, 17, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (50, '龅牙武士', 1, 28, 118, 38, 38, 18, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (51, '龅牙武士', 1, 27, 117, 37, 37, 17, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (53, '熊兵', 1, 28, 118, 38, 38, 18, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (54, '混世魔王', 2, 28, 127, 47, 57, 37, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (55, '蜘蛛', 1, 29, 119, 39, 39, 19, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (56, '人参果', 1, 30, 120, 40, 40, 20, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (57, '人参果', 1, 29, 119, 39, 39, 19, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (58, '扫地小道', 1, 30, 120, 40, 40, 20, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (60, '如意真君', 2, 30, 129, 49, 59, 39, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (61, '扫地小道', 1, 31, 121, 41, 41, 21, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (62, '清风', 1, 32, 122, 42, 42, 22, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (63, '清风', 1, 31, 121, 41, 41, 21, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (64, '明月', 1, 32, 122, 42, 42, 22, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (65, '镇元大仙', 2, 32, 131, 51, 61, 41, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (66, '灯笼怪', 1, 33, 123, 43, 43, 23, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (67, '宝箱怪', 1, 34, 124, 44, 44, 24, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (68, '宝箱怪', 1, 33, 123, 43, 43, 23, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (69, '熊兵', 1, 34, 124, 44, 44, 24, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (70, '角犀大王', 2, 34, 133, 53, 63, 43, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (71, '蝎子', 1, 35, 125, 45, 45, 25, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (72, '小蛇', 1, 36, 126, 46, 46, 26, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (73, '小蛇', 1, 35, 125, 45, 45, 25, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (74, '蛇精', 1, 36, 126, 46, 46, 26, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (75, '灵蛇女王', 2, 36, 135, 55, 65, 45, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (76, '蜘蛛', 1, 37, 127, 47, 47, 27, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (77, '甲虫', 1, 38, 128, 48, 48, 28, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (78, '甲虫', 1, 37, 127, 47, 47, 27, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (79, '蝎子', 1, 38, 128, 48, 48, 28, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (80, '蝎子', 1, 37, 127, 47, 47, 27, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (81, '百眼魔君', 2, 37, 136, 56, 66, 46, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (82, '小蛇', 1, 38, 128, 48, 48, 28, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (83, '龅牙武士', 1, 39, 129, 49, 49, 29, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (84, '龅牙武士', 1, 38, 128, 48, 48, 28, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (85, '熊兵', 1, 39, 129, 49, 49, 29, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (86, '熊兵', 1, 38, 128, 48, 48, 28, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (87, '银角大王', 2, 38, 137, 57, 67, 47, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (88, '蝙蝠', 1, 39, 129, 49, 49, 29, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (89, '龅牙武士', 1, 40, 130, 50, 50, 30, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (90, '龅牙武士', 1, 39, 129, 49, 49, 29, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (91, '蛇精', 1, 40, 130, 50, 50, 30, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (92, '蛇精', 1, 39, 129, 49, 49, 29, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (93, '金角大王', 2, 39, 138, 58, 67, 48, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (94, '村姑', 1, 40, 130, 50, 50, 30, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (95, '村妇', 1, 41, 131, 51, 51, 31, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (96, '村妇', 1, 40, 130, 50, 50, 30, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (97, '老翁', 1, 41, 131, 51, 51, 31, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (98, '老翁', 1, 40, 130, 50, 50, 30, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (99, '冰漓', 2, 40, 139, 59, 69, 49, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (100, '蝙蝠', 1, 41, 131, 51, 51, 31, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (101, '白骨精', 1, 42, 132, 52, 52, 32, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (102, '白骨精', 1, 41, 131, 51, 51, 31, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (103, '骷髅侍女', 1, 42, 132, 52, 52, 32, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (104, '骷髅侍女', 1, 41, 131, 51, 51, 31, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (105, '白骨妖姬', 2, 41, 140, 60, 70, 50, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (106, '盗老三', 1, 42, 132, 52, 52, 32, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (107, '盗老七', 1, 43, 133, 53, 53, 33, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (108, '盗老七', 1, 42, 132, 52, 52, 32, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (109, '盗十一', 1, 43, 133, 53, 53, 33, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (110, '盗十一', 1, 42, 132, 52, 52, 32, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (111, '阿里巴巴', 2, 42, 141, 61, 71, 51, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (112, '傲来侍卫甲', 1, 43, 133, 53, 53, 33, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (113, '傲来侍卫乙', 1, 44, 134, 54, 54, 34, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (114, '傲来侍卫乙', 1, 43, 133, 53, 53, 33, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (115, '傲来将军', 1, 44, 134, 54, 54, 34, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (116, '傲来将军', 1, 43, 133, 53, 53, 33, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (117, '傲来国王', 2, 43, 142, 62, 72, 52, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (118, '蜘蛛', 1, 44, 134, 54, 54, 34, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (119, '蝎子', 1, 45, 135, 55, 55, 35, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (120, '蝎子', 1, 44, 134, 54, 54, 34, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (121, '小蛇', 1, 45, 135, 55, 55, 35, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (122, '小蛇', 1, 44, 134, 54, 54, 34, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (123, '残暴鼠妖', 2, 44, 143, 63, 73, 53, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (124, '蝙蝠', 1, 45, 135, 55, 55, 35, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (125, '顽皮猴', 1, 46, 136, 56, 56, 36, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (126, '顽皮猴', 1, 45, 135, 55, 55, 35, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (127, '老猴头', 1, 46, 136, 56, 56, 36, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (128, '老猴头', 1, 45, 135, 55, 55, 35, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (129, '猴神医', 2, 45, 144, 64, 74, 54, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (130, '水妖甲', 1, 46, 136, 56, 56, 36, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (131, '水妖乙', 1, 47, 137, 57, 57, 37, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (132, '水妖乙', 1, 46, 136, 56, 56, 36, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (133, '蚌精', 1, 47, 137, 57, 57, 37, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (134, '蚌精', 1, 46, 136, 56, 56, 36, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (135, '千年鱼精', 2, 46, 145, 65, 75, 55, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (136, '虾兵', 1, 47, 137, 57, 57, 37, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (137, '蟹将', 1, 48, 138, 58, 58, 38, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (138, '蟹将', 1, 47, 137, 57, 57, 37, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (139, '龟丞相', 1, 48, 138, 58, 58, 38, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (140, '龟丞相', 1, 47, 137, 57, 57, 37, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (141, '敖广', 2, 47, 146, 66, 76, 56, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (142, '僧侣1', 1, 48, 138, 58, 58, 38, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (143, '僧侣2', 1, 49, 139, 59, 59, 39, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (144, '僧侣2', 1, 48, 138, 58, 58, 38, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (145, '僧侣3', 1, 49, 139, 59, 59, 39, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (146, '僧侣3', 1, 48, 138, 58, 58, 38, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (147, '玄石神僧', 2, 48, 147, 67, 77, 57, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (148, '精英熊罴怪', 3, 20, 119, 39, 49, 29, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (149, '精英千年鱼精', 3, 22, 121, 41, 51, 31, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (150, '精英敖广', 3, 24, 123, 43, 53, 33, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (151, '精英混世魔王', 3, 26, 125, 45, 55, 35, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (152, '精英猴神医', 3, 28, 127, 47, 57, 37, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (153, '精英符道仙', 3, 30, 129, 49, 59, 39, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (154, '精英玄石神僧', 3, 32, 131, 51, 61, 41, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (155, '精英高太公', 3, 34, 133, 53, 63, 43, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (156, '精英八戒', 3, 36, 135, 55, 65, 45, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (157, '精英聚云仙人', 3, 37, 136, 56, 66, 46, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (158, '精英吴刚', 3, 38, 137, 57, 67, 47, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (159, '精英嫦娥', 3, 39, 138, 58, 68, 48, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (160, '精英增长天王', 3, 40, 139, 59, 69, 49, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (161, '精英王母娘娘', 3, 41, 140, 60, 70, 50, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (162, '精英沙悟净', 3, 42, 141, 61, 71, 51, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (163, '精英清风', 3, 43, 142, 62, 72, 52, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (164, '精英镇元子', 3, 44, 143, 63, 73, 53, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (165, '精英滑头老翁', 3, 45, 144, 64, 74, 54, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (166, '精英白骨夫人', 3, 46, 145, 65, 75, 55, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (167, '精英银角大王', 3, 47, 146, 66, 76, 56, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (168, '精英金角大王', 3, 48, 147, 67, 77, 57, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (169, '精英阿里巴巴', 3, 49, 148, 68, 78, 58, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (170, '精英宝象国王', 3, 50, 149, 69, 79, 59, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (171, '精英黄袍怪', 3, 51, 150, 70, 80, 60, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (172, '精英金鼻白鼠', 3, 52, 151, 71, 81, 61, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (173, '精英托塔天王', 3, 53, 152, 72, 82, 62, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (174, '精英如意真君', 3, 54, 153, 73, 83, 63, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (175, '精英春娇将军', 3, 55, 154, 74, 84, 64, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (176, '精英西梁女王', 3, 56, 155, 75, 85, 65, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (177, '精英红孩儿', 3, 57, 156, 76, 86, 66, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (178, '精英夸父', 3, 58, 157, 77, 87, 67, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (179, '精英铁扇公主', 3, 59, 158, 78, 88, 68, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (180, '精英牛魔王', 3, 60, 159, 79, 89, 69, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (181, '善财童女', 1, 49, 139, 59, 59, 39, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (182, '善财童子', 1, 50, 140, 60, 60, 40, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (183, '善财童子', 1, 49, 139, 59, 59, 39, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (184, '推云童子', 1, 50, 140, 60, 60, 40, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (185, '推云童子', 1, 49, 139, 59, 59, 39, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (186, '穷奇', 2, 49, 148, 68, 78, 58, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (187, '天兵1', 1, 50, 140, 60, 60, 40, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (188, '天兵2', 1, 51, 141, 61, 61, 41, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (189, '天兵2', 1, 50, 140, 60, 60, 40, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (190, '天兵3', 1, 51, 141, 61, 61, 41, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (191, '天兵3', 1, 50, 140, 60, 60, 40, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (192, '吴刚', 2, 50, 149, 69, 79, 59, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (193, '天兵1', 1, 51, 141, 61, 61, 41, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (194, '天兵2', 1, 52, 142, 62, 62, 42, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (195, '天兵2', 1, 51, 141, 61, 61, 41, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (196, '天兵3', 1, 52, 142, 62, 62, 42, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (197, '天兵3', 1, 51, 141, 61, 61, 41, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (198, '嫦娥', 2, 51, 150, 70, 80, 60, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (199, '天兵1', 1, 52, 142, 62, 62, 42, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (200, '天兵2', 1, 53, 143, 63, 63, 43, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (201, '天兵2', 1, 52, 142, 62, 62, 42, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (202, '天兵3', 1, 53, 143, 63, 63, 43, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (203, '天兵3', 1, 52, 142, 62, 62, 42, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (204, '毗琉璃', 2, 52, 151, 71, 81, 61, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (205, '天兵1', 1, 53, 143, 63, 63, 43, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (206, '天兵2', 1, 54, 144, 64, 64, 44, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (207, '天兵2', 1, 53, 143, 63, 63, 43, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (208, '天兵3', 1, 54, 144, 64, 64, 44, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (209, '天兵3', 1, 53, 143, 63, 63, 43, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (210, '李靖', 2, 53, 152, 72, 82, 62, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (211, '天兵1', 1, 54, 144, 64, 64, 44, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (212, '天兵2', 1, 55, 145, 65, 65, 45, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (213, '天兵2', 1, 54, 144, 64, 64, 44, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (214, '天兵3', 1, 55, 145, 65, 65, 45, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (215, '天兵3', 1, 54, 144, 64, 64, 44, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (216, '西王母', 2, 54, 153, 73, 83, 63, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (217, '熔岩武士1', 1, 55, 145, 65, 65, 45, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (218, '熔岩武士2', 1, 56, 146, 66, 66, 46, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (219, '熔岩武士2', 1, 55, 145, 65, 65, 45, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (220, '熔岩武士3', 1, 56, 146, 66, 66, 46, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (221, '熔岩武士3', 1, 55, 145, 65, 65, 45, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (222, '夸父', 2, 55, 154, 74, 84, 64, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (223, '幼年麒麟', 1, 56, 146, 66, 66, 46, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (224, '火焰兽', 1, 57, 147, 67, 67, 47, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (225, '火焰兽', 1, 56, 146, 66, 66, 46, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (226, '火麒麟', 1, 57, 147, 67, 67, 47, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (227, '火麒麟', 1, 56, 146, 66, 66, 46, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (228, '梼杌', 2, 56, 155, 75, 85, 65, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (229, '沙虫', 1, 57, 147, 67, 67, 47, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (230, '弯刀沙妖', 1, 58, 148, 68, 68, 48, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (231, '弯刀沙妖', 1, 57, 147, 67, 67, 47, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (232, '牛小兵斧头', 1, 58, 148, 68, 68, 48, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (233, '牛小兵斧头', 1, 57, 147, 67, 67, 47, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (234, '悟净心魔', 2, 57, 156, 76, 86, 66, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (235, '犀牛精', 1, 58, 148, 68, 68, 48, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (236, '牛小兵', 1, 59, 149, 69, 69, 49, 10002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (237, '牛小兵', 1, 58, 148, 68, 68, 48, 10003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (238, '牛小兵斧头', 1, 59, 149, 69, 69, 49, 10004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (239, '牛小兵斧头', 1, 58, 148, 68, 68, 48, 20001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (240, '红孩儿', 2, 58, 157, 77, 87, 67, 20002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (241, '侍女玉儿', 1, 59, 149, 69, 69, 49, 20003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (242, '侍女翠儿', 1, 60, 150, 70, 70, 50, 20004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (243, '侍女翠儿', 1, 59, 149, 69, 69, 49, 1, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (244, '侍女香儿', 1, 60, 150, 70, 70, 50, 2, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (245, '侍女香儿', 1, 59, 149, 69, 69, 49, 3, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (246, '铁扇公主', 2, 59, 158, 78, 88, 68, 4, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (247, '积雷甲牛', 1, 60, 150, 70, 70, 50, 1001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (248, '魔王亲卫', 1, 61, 151, 71, 71, 51, 1002, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (249, '魔王亲卫', 1, 60, 150, 70, 70, 50, 1003, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (250, '玉面狐狸', 1, 61, 151, 71, 71, 51, 1004, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (251, '玉面狐狸', 1, 60, 150, 70, 70, 50, 10001, 1, 0, 1, 0, 0);
INSERT INTO "main"."monsterprop" VALUES (252, '牛魔王', 2, 60, 159, 79, 89, 69, 10002, 1, 0, 1, 0, 0);

-- ----------------------------
-- Table structure for "main"."monstertype"
-- ----------------------------
DROP TABLE "main"."monstertype";
CREATE TABLE "monstertype" (
"Type"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
PRIMARY KEY ("Type")
);

-- ----------------------------
-- Records of monstertype
-- ----------------------------
INSERT INTO "main"."monstertype" VALUES (1, '普通怪物');
INSERT INTO "main"."monstertype" VALUES (2, 'BOSS怪物');
INSERT INTO "main"."monstertype" VALUES (3, '精英怪物');

-- ----------------------------
-- Table structure for "main"."monsterupgradeprop"
-- ----------------------------
DROP TABLE "main"."monsterupgradeprop";
CREATE TABLE "monsterupgradeprop" (
"Level"  smallint NOT NULL,
"Type"  smallint NOT NULL DEFAULT 1,
"HP"  INTEGER NOT NULL,
"Attack"  INTEGER NOT NULL,
"EncourageExp"  INTEGER NOT NULL DEFAULT 0,
"EncourageCoin"  INTEGER NOT NULL DEFAULT 0,
"EncourageScience"  INTEGER NOT NULL DEFAULT 100,
PRIMARY KEY ("Level" ASC, "Type" ASC)
);

-- ----------------------------
-- Records of monsterupgradeprop
-- ----------------------------
INSERT INTO "main"."monsterupgradeprop" VALUES (1, 2, 60, 4, 100, 100, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (1, 1, 30, 4, 60, 50, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (2, 1, 60, 8, 100, 60, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (2, 2, 120, 8, 140, 120, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (3, 1, 90, 12, 140, 80, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (3, 2, 180, 12, 180, 160, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (4, 1, 420, 16, 180, 100, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (4, 2, 840, 16, 220, 200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (5, 1, 450, 20, 220, 110, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (5, 2, 900, 20, 260, 220, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (6, 2, 960, 25, 300, 260, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (6, 1, 480, 25, 260, 130, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (7, 1, 660, 92, 300, 150, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (7, 2, 1320, 92, 340, 300, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (8, 1, 720, 100, 340, 160, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (8, 2, 1440, 100, 380, 320, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (9, 1, 750, 104, 380, 180, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (9, 2, 1500, 104, 420, 360, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (10, 1, 900, 133, 420, 200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (10, 2, 1800, 133, 460, 400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (11, 1, 1240, 103, 460, 210, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (11, 2, 2480, 103, 500, 420, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (12, 1, 1280, 106, 500, 230, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (12, 2, 2560, 106, 540, 460, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (13, 1, 2200, 172, 540, 250, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (13, 2, 4400, 172, 580, 500, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (14, 1, 2280, 178, 580, 260, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (14, 2, 4560, 178, 620, 520, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (15, 1, 2360, 184, 620, 280, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (15, 2, 4720, 184, 660, 560, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (16, 2, 8000, 312, 700, 600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (16, 1, 4000, 312, 660, 300, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (17, 1, 4200, 328, 700, 310, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (17, 2, 8400, 328, 740, 620, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (18, 1, 6000, 468, 740, 330, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (18, 2, 12000, 468, 780, 660, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (19, 1, 8000, 625, 780, 350, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (19, 2, 16000, 625, 820, 700, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (20, 3, 100000, 2000, 1380, 2400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (20, 1, 10000, 781, 820, 360, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (20, 2, 20000, 781, 860, 720, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (21, 1, 13200, 660, 860, 380, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (21, 2, 26400, 660, 900, 760, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (22, 1, 13800, 690, 900, 400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (22, 2, 27600, 690, 940, 800, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (22, 3, 100000, 2000, 1500, 2600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (23, 1, 14250, 712, 940, 410, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (23, 2, 28500, 712, 980, 820, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (24, 1, 14850, 742, 980, 430, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (24, 2, 29700, 742, 1020, 860, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (24, 3, 100000, 2000, 1620, 2800, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (25, 1, 28200, 1410, 1020, 450, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (25, 2, 56400, 1410, 1060, 900, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (26, 1, 29200, 1460, 1060, 460, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (26, 2, 58400, 1460, 1100, 920, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (26, 3, 100000, 2000, 1940, 3000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (27, 2, 60000, 1500, 1140, 960, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (27, 1, 30000, 1500, 1100, 480, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (28, 3, 100000, 2000, 1860, 3200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (28, 1, 31000, 1550, 1140, 500, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (28, 2, 62000, 1550, 1180, 1000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (29, 1, 33250, 1662, 1180, 510, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (29, 2, 66500, 1662, 1220, 1020, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (30, 1, 42875, 2144, 1220, 530, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (30, 2, 85750, 2144, 1260, 1060, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (30, 3, 100000, 2000, 1980, 3400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (31, 2, 106044, 1841, 1300, 1100, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (31, 1, 53022, 1841, 1260, 550, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (32, 3, 100000, 2000, 2100, 3600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (32, 1, 54600, 1896, 1300, 560, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (32, 2, 109200, 1896, 1340, 1120, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (33, 1, 56172, 1950, 1340, 580, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (33, 2, 112344, 1950, 1380, 1160, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (34, 1, 57750, 2005, 1380, 600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (34, 2, 115500, 2005, 1420, 1200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (34, 3, 100000, 2000, 2340, 4000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (35, 2, 118644, 2060, 1460, 1220, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (35, 1, 59322, 2060, 1420, 610, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (36, 1, 60900, 2114, 1460, 630, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (36, 2, 121800, 2114, 1500, 1260, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (36, 3, 100000, 2000, 2400, 4100, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (37, 1, 62472, 2169, 1500, 650, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (37, 2, 124944, 2169, 1540, 1300, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (37, 3, 100000, 2000, 2460, 4200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (38, 3, 100000, 2000, 2520, 4300, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (38, 1, 64050, 2224, 1540, 660, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (38, 2, 128100, 2224, 1580, 1320, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (39, 3, 100000, 2000, 2580, 4400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (39, 1, 65622, 2278, 1580, 680, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (39, 2, 131244, 2278, 1620, 1360, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (40, 1, 76200, 2646, 1620, 700, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (40, 2, 152400, 2646, 1660, 1400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (40, 3, 100000, 2000, 2640, 4500, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (41, 3, 100000, 2000, 2700, 4600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (41, 1, 90734, 2314, 1660, 710, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (41, 2, 181468, 2314, 1700, 1420, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (42, 3, 100000, 2000, 2760, 4700, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (42, 2, 185150, 2362, 1740, 1460, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (42, 1, 92575, 2362, 1700, 730, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (43, 1, 116375, 2968, 1740, 750, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (43, 2, 232750, 2968, 1780, 1500, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (43, 3, 100000, 2000, 2820, 4800, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (44, 1, 119000, 3036, 1780, 760, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (44, 2, 238000, 3036, 1820, 1520, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (44, 3, 100000, 2000, 2880, 4900, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (45, 1, 121625, 3102, 1820, 780, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (45, 2, 243250, 3102, 1860, 1560, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (45, 3, 100000, 2000, 2940, 5000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (46, 1, 124250, 3170, 1860, 800, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (46, 2, 248500, 3170, 1900, 1600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (46, 3, 100000, 2000, 3000, 5100, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (47, 1, 126875, 3236, 1900, 810, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (47, 2, 253750, 3236, 1940, 1620, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (47, 3, 100000, 2000, 3060, 5200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (48, 1, 129500, 3304, 1940, 830, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (48, 3, 100000, 2000, 3120, 5300, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (48, 2, 25900, 3304, 1980, 1660, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (49, 1, 132125, 3370, 1980, 850, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (49, 2, 264250, 3370, 2020, 1700, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (49, 3, 100000, 2000, 3180, 5400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (50, 1, 134750, 3438, 2020, 860, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (50, 2, 269500, 3438, 2060, 1720, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (50, 3, 100000, 2000, 3240, 5500, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (51, 1, 167200, 3266, 2060, 880, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (51, 2, 334400, 3266, 2100, 1760, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (51, 3, 100000, 2000, 3300, 5600, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (52, 1, 170400, 3328, 2100, 900, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (52, 2, 340800, 3328, 2140, 1800, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (52, 3, 100000, 2000, 3360, 5700, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (53, 1, 173600, 3390, 2140, 910, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (53, 2, 347200, 3390, 2180, 1820, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (53, 3, 100000, 2000, 3420, 5800, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (54, 2, 353600, 3453, 2220, 1860, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (54, 3, 100000, 2000, 3480, 5900, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (54, 1, 176800, 3453, 2180, 930, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (55, 2, 360000, 3516, 2260, 1900, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (55, 3, 100000, 2000, 3540, 6000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (55, 1, 180000, 3516, 2220, 950, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (56, 1, 183200, 3578, 2260, 960, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (56, 2, 366400, 3578, 2300, 1920, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (56, 3, 100000, 2000, 3600, 6100, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (57, 3, 100000, 2000, 3660, 6200, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (57, 1, 186400, 3640, 2300, 980, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (57, 2, 372800, 3640, 2340, 1960, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (58, 3, 100000, 2000, 3720, 6300, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (58, 1, 189600, 3703, 2340, 1000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (58, 2, 379200, 3703, 2380, 2000, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (59, 1, 192800, 3766, 2380, 1010, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (59, 2, 385600, 3766, 2420, 2020, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (59, 3, 100000, 2000, 3780, 6400, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (60, 1, 196000, 3828, 2420, 1030, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (60, 2, 392000, 3828, 2460, 2060, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (60, 3, 100000, 2000, 3840, 6500, 100);
INSERT INTO "main"."monsterupgradeprop" VALUES (61, 1, 200000, 3900, 2460, 1050, 100);

-- ----------------------------
-- Table structure for "main"."onhook"
-- ----------------------------
DROP TABLE "main"."onhook";
CREATE TABLE "onhook" (
"PassSecondPerInstance"  smallint NOT NULL,
"NoCDGoldPerInstance"  smallint NOT NULL
);

-- ----------------------------
-- Records of onhook
-- ----------------------------
INSERT INTO "main"."onhook" VALUES (10, 5);

-- ----------------------------
-- Table structure for "main"."onlineencourage"
-- ----------------------------
DROP TABLE "main"."onlineencourage";
CREATE TABLE "onlineencourage" (
"Idx"  INTEGER NOT NULL,
"Coin"  INTEGER NOT NULL DEFAULT 0,
"Gold"  INTEGER NOT NULL DEFAULT 0,
"Science"  INTEGER NOT NULL,
"Experience"  INTEGER NOT NULL DEFAULT 0,
"PhyStrength"  smallint NOT NULL DEFAULT 0,
"ItemKindID1"  smallint NOT NULL,
"ItemPileNum1"  smallint NOT NULL,
"ItemKindID2"  smallint NOT NULL,
"ItemPileNum2"  smallint NOT NULL,
"ItemKindID3"  smallint NOT NULL,
"ItemPileNum3"  smallint NOT NULL,
"RecvCD"  smallint NOT NULL,
PRIMARY KEY ("Idx" ASC)
);

-- ----------------------------
-- Records of onlineencourage
-- ----------------------------
INSERT INTO "main"."onlineencourage" VALUES (1, 0, 0, 0, 0, 20, 20047, 1, 0, 0, 0, 0, 300);
INSERT INTO "main"."onlineencourage" VALUES (2, 0, 0, 0, 0, 20, 20047, 1, 0, 0, 0, 0, 600);
INSERT INTO "main"."onlineencourage" VALUES (3, 0, 0, 0, 0, 30, 20053, 1, 0, 0, 0, 0, 1800);
INSERT INTO "main"."onlineencourage" VALUES (4, 0, 0, 0, 0, 30, 20053, 1, 0, 0, 0, 0, 3600);

-- ----------------------------
-- Table structure for "main"."phystrengthprop"
-- ----------------------------
DROP TABLE "main"."phystrengthprop";
CREATE TABLE "phystrengthprop" (
"CommonPhyStrength"  smallint NOT NULL,
"ElitePhyStrength"  smallint NOT NULL,
"PhyStrengthRefreshCD"  smallint NOT NULL DEFAULT 300,
"BuyPhyStrength"  smallint NOT NULL,
"BuyMaxNumPerDay"  smallint NOT NULL,
"BuyBaseGold"  INTEGER NOT NULL,
"BuyIncGold"  INTEGER NOT NULL,
"BuyMaxGold"  INTEGER NOT NULL
);

-- ----------------------------
-- Records of phystrengthprop
-- ----------------------------
INSERT INTO "main"."phystrengthprop" VALUES (5, 0, 300, 5, 10, 5, 1, 15);

-- ----------------------------
-- Table structure for "main"."playerupgradeprop"
-- ----------------------------
DROP TABLE "main"."playerupgradeprop";
CREATE TABLE "playerupgradeprop" (
"Level"  smallint NOT NULL,
"MaxPhyStrength"  smallint NOT NULL,
"PhyStrengthRefreshCD"  smallint NOT NULL,
"PhyStrengthRefreshValue"  smallint NOT NULL,
PRIMARY KEY ("Level" ASC)
);

-- ----------------------------
-- Records of playerupgradeprop
-- ----------------------------
INSERT INTO "main"."playerupgradeprop" VALUES (1, 200, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (2, 250, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (3, 300, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (4, 350, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (5, 400, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (6, 450, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (7, 500, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (8, 550, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (9, 600, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (10, 600, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (11, 600, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (12, 600, 300, 1);
INSERT INTO "main"."playerupgradeprop" VALUES (13, 600, 300, 1);

-- ----------------------------
-- Table structure for "main"."plunderbaseprop"
-- ----------------------------
DROP TABLE "main"."plunderbaseprop";
CREATE TABLE "plunderbaseprop" (
"PlunderOpenLevel"  smallint NOT NULL,
"PlunderLowerLevelItemID"  smallint NOT NULL,
"PlunderFreeTimesPerDay"  smallint NOT NULL,
"PlunderMaxTimesPerDay"  smallint NOT NULL,
"PlunderBaseGold"  smallint NOT NULL,
"PlunderIncGold"  smallint NOT NULL,
"PlunderMaxGold"  smallint NOT NULL,
"CaptureWorkSecond"  INTEGER NOT NULL DEFAULT 24,
"WorkCoinPerSec"  INTEGER NOT NULL,
"WorkSciencePerSec"  INTEGER NOT NULL,
"BleedPerGoldClrMin"  smallint NOT NULL DEFAULT 5,
"MaxCaptureNum"  smallint NOT NULL,
"CaptureKeepSecond"  INTEGER NOT NULL,
"DriveAwayBaseGold"  INTEGER NOT NULL DEFAULT 10,
"DriveAwayIncGold"  INTEGER NOT NULL DEFAULT 2,
PRIMARY KEY ("PlunderOpenLevel" ASC)
);

-- ----------------------------
-- Records of plunderbaseprop
-- ----------------------------
INSERT INTO "main"."plunderbaseprop" VALUES (1, 20058, 10, 15, 10, 5, 30, 300, 10, 2, 5, 4, 2400, 10, 2);

-- ----------------------------
-- Table structure for "main"."plundergridunlockprop"
-- ----------------------------
DROP TABLE "main"."plundergridunlockprop";
CREATE TABLE "plundergridunlockprop" (
"GridIdx"  smallint NOT NULL,
"UnlockLevel"  smallint NOT NULL,
PRIMARY KEY ("GridIdx")
);

-- ----------------------------
-- Records of plundergridunlockprop
-- ----------------------------
INSERT INTO "main"."plundergridunlockprop" VALUES (1, 1);
INSERT INTO "main"."plundergridunlockprop" VALUES (4, 41);
INSERT INTO "main"."plundergridunlockprop" VALUES (2, 21);
INSERT INTO "main"."plundergridunlockprop" VALUES (3, 31);

-- ----------------------------
-- Table structure for "main"."racebase"
-- ----------------------------
DROP TABLE "main"."racebase";
CREATE TABLE "racebase" (
"MaxChallengeNumPerDay"  INTEGER NOT NULL,
"FreeChallengeNumPerDay"  INTEGER NOT NULL,
"ChallengeFailedCD"  INTEGER NOT NULL,
"ClrCDGoldPerMinute"  INTEGER NOT NULL,
"ChallengeBaseGold"  INTEGER NOT NULL,
"ChallengeIncGold"  INTEGER NOT NULL,
"ChallengeMaxGold"  INTEGER NOT NULL,
"ChallengeNum"  INTEGER NOT NULL,
"RankShowNum"  INTEGER NOT NULL,
"LimitRank"  INTEGER NOT NULL
);

-- ----------------------------
-- Records of racebase
-- ----------------------------
INSERT INTO "main"."racebase" VALUES (15, 10, 3, 1, 10, 5, 30, 5, 25, 50);

-- ----------------------------
-- Table structure for "main"."racechallengesec"
-- ----------------------------
DROP TABLE "main"."racechallengesec";
CREATE TABLE "racechallengesec" (
"Min"  INTEGER NOT NULL,
"Max"  INTEGER NOT NULL,
"Interval"  INTEGER NOT NULL,
PRIMARY KEY ("Min")
);

-- ----------------------------
-- Records of racechallengesec
-- ----------------------------
INSERT INTO "main"."racechallengesec" VALUES (1, 100, 1);
INSERT INTO "main"."racechallengesec" VALUES (101, 200, 2);
INSERT INTO "main"."racechallengesec" VALUES (201, 500, 3);
INSERT INTO "main"."racechallengesec" VALUES (501, 1000, 5);
INSERT INTO "main"."racechallengesec" VALUES (1001, 2000, 10);
INSERT INTO "main"."racechallengesec" VALUES (2001, 5000, 50);
INSERT INTO "main"."racechallengesec" VALUES (5001, 0, 100);

-- ----------------------------
-- Table structure for "main"."scienceattrprop"
-- ----------------------------
DROP TABLE "main"."scienceattrprop";
CREATE TABLE "scienceattrprop" (
"AttrKindID"  smallint NOT NULL,
"AttrKindName"  varchar(32) NOT NULL,
"AttrLevel"  smallint NOT NULL,
"AttrValue"  INTEGER NOT NULL,
"UpgradeSciencePoint"  INTEGER NOT NULL,
PRIMARY KEY ("AttrKindID" ASC, "AttrLevel" ASC)
);

-- ----------------------------
-- Records of scienceattrprop
-- ----------------------------
INSERT INTO "main"."scienceattrprop" VALUES (1, 'HP', 1, 10, 10);
INSERT INTO "main"."scienceattrprop" VALUES (1, 'HP', 2, 15, 15);
INSERT INTO "main"."scienceattrprop" VALUES (1, 'HP', 3, 20, 20);
INSERT INTO "main"."scienceattrprop" VALUES (2, 'Attack', 1, 5, 10);
INSERT INTO "main"."scienceattrprop" VALUES (2, 'Attack', 3, 7, 15);
INSERT INTO "main"."scienceattrprop" VALUES (2, 'Attack', 2, 6, 20);

-- ----------------------------
-- Table structure for "main"."shopprop"
-- ----------------------------
DROP TABLE "main"."shopprop";
CREATE TABLE "shopprop" (
"TabID"  smallint NOT NULL,
"KindID"  smallint NOT NULL,
"PileNum"   NOT NULL DEFAULT 1,
PRIMARY KEY ("TabID" ASC, "KindID" ASC, "PileNum" ASC)
);

-- ----------------------------
-- Records of shopprop
-- ----------------------------
INSERT INTO "main"."shopprop" VALUES (1, 30001, 1);
INSERT INTO "main"."shopprop" VALUES (1, 30002, 1);
INSERT INTO "main"."shopprop" VALUES (1, 30003, 1);
INSERT INTO "main"."shopprop" VALUES (1, 30004, 1);
INSERT INTO "main"."shopprop" VALUES (1, 30005, 1);
INSERT INTO "main"."shopprop" VALUES (1, 30006, 1);
INSERT INTO "main"."shopprop" VALUES (1, 20046, 1);
INSERT INTO "main"."shopprop" VALUES (1, 20050, 1);
INSERT INTO "main"."shopprop" VALUES (1, 20001, 1);
INSERT INTO "main"."shopprop" VALUES (1, 20002, 1);
INSERT INTO "main"."shopprop" VALUES (1, 50001, 1);
INSERT INTO "main"."shopprop" VALUES (1, 51000, 1);
INSERT INTO "main"."shopprop" VALUES (1, 51005, 1);
INSERT INTO "main"."shopprop" VALUES (1, 40019, 1);
INSERT INTO "main"."shopprop" VALUES (1, 1, 1);
INSERT INTO "main"."shopprop" VALUES (1, 5, 1);
INSERT INTO "main"."shopprop" VALUES (1, 4, 1);
INSERT INTO "main"."shopprop" VALUES (1, 6, 1);
INSERT INTO "main"."shopprop" VALUES (1, 10, 1);
INSERT INTO "main"."shopprop" VALUES (1, 12, 1);
INSERT INTO "main"."shopprop" VALUES (6, 20058, 1);
INSERT INTO "main"."shopprop" VALUES (6, 30007, 1);
INSERT INTO "main"."shopprop" VALUES (1, 52002, 5);
INSERT INTO "main"."shopprop" VALUES (1, 52001, 5);

-- ----------------------------
-- Table structure for "main"."shoptab"
-- ----------------------------
DROP TABLE "main"."shoptab";
CREATE TABLE "shoptab" (
"TabID"  smallint NOT NULL,
"Name"  char(32) NOT NULL,
PRIMARY KEY ("TabID")
);

-- ----------------------------
-- Records of shoptab
-- ----------------------------
INSERT INTO "main"."shoptab" VALUES (1, '热卖');
INSERT INTO "main"."shoptab" VALUES (2, '装备');
INSERT INTO "main"."shoptab" VALUES (3, '物品');
INSERT INTO "main"."shoptab" VALUES (4, '技能卡片');
INSERT INTO "main"."shoptab" VALUES (5, '其它');
INSERT INTO "main"."shoptab" VALUES (6, '指定叠加数购买');

-- ----------------------------
-- Table structure for "main"."singlepram"
-- ----------------------------
DROP TABLE "main"."singlepram";
CREATE TABLE "singlepram" (
"MaxPlayerLevel"  smallint NOT NULL,
"OnlineEncourageKeepDays"  smallint NOT NULL DEFAULT 3,
"LoginEncourageKeepDays"  smallint NOT NULL DEFAULT 3
);

-- ----------------------------
-- Records of singlepram
-- ----------------------------
INSERT INTO "main"."singlepram" VALUES (60, 3, 3);

-- ----------------------------
-- Table structure for "main"."skillbaseprop"
-- ----------------------------
DROP TABLE "main"."skillbaseprop";
CREATE TABLE "skillbaseprop" (
"NormalUpgradeItemID"  smallint NOT NULL DEFAULT 5000,
"RareUpgradeItemID"  smallint NOT NULL,
"IncTalentPerNormalTrain"  smallint NOT NULL,
"IncTalentPerRareTrain"  smallint NOT NULL DEFAULT 10
);

-- ----------------------------
-- Records of skillbaseprop
-- ----------------------------
INSERT INTO "main"."skillbaseprop" VALUES (52001, 52002, 5, 10);

-- ----------------------------
-- Table structure for "main"."skillebuffkind"
-- ----------------------------
DROP TABLE "main"."skillebuffkind";
CREATE TABLE "skillebuffkind"(
  Kind smallint unsigned NOT NULL,
  Name varchar(32) NOT NULL,
  Introduction varchar unsigned NOT NULL,
  PRIMARY KEY(Kind));

-- ----------------------------
-- Records of skillebuffkind
-- ----------------------------
INSERT INTO "main"."skillebuffkind" VALUES (1, '静止', '暂定拥有该状态者本回合内攻击，属于负面状态');
INSERT INTO "main"."skillebuffkind" VALUES (2, '沉默', '暂定拥有该状态者本回合内使用技能，属于负面状态');
INSERT INTO "main"."skillebuffkind" VALUES (3, '中毒', '使得拥有该状态者本回合内掉固定值血量，属于负面状态');
INSERT INTO "main"."skillebuffkind" VALUES (4, '残废', '使得拥有该状态者本回合内攻击力按百分比降低，属于负面状态');
INSERT INTO "main"."skillebuffkind" VALUES (14, '治疗', '使得拥有该状态者本回合内恢复固定值血量，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (11, '嗜血', '使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (12, '狂暴', '使得拥有该状态者本回合内暴击率提升，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (13, '敏捷', '使得拥有该状态者本回合内闪避率提升，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (21, '停顿', '暂定拥有该状态者本回合内攻击，属于中立状态');
INSERT INTO "main"."skillebuffkind" VALUES (15, '吸血', '使得拥有该状态者攻击时将对对方伤害的百分比转换成自身的血量，属于增益状');
INSERT INTO "main"."skillebuffkind" VALUES (16, '减伤', '使得拥有该状态者受击命中时减少伤害值，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (17, '反击', '使得拥有该状态者受击命中时反击对方，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (18, '反弹', '使得拥有该状态者受击命中时经部分伤害反弹给对方，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (19, '格挡', '使得拥有该状态者受击命中时减少所受伤害百分比，属于增益状态');
INSERT INTO "main"."skillebuffkind" VALUES (5, '破甲', '使得拥有该状态者本回合内受到伤害加成，属于负面状态');

-- ----------------------------
-- Table structure for "main"."skilleffectkind"
-- ----------------------------
DROP TABLE "main"."skilleffectkind";
CREATE TABLE "skilleffectkind"(
  Kind smallint unsigned NOT NULL,
  Name varchar(32) NOT NULL,
  Introduction varchar unsigned NOT NULL,
  PRIMARY KEY(Kind));

-- ----------------------------
-- Records of skilleffectkind
-- ----------------------------
INSERT INTO "main"."skilleffectkind" VALUES (1, '伤害', '伤害，攻击时触发');

-- ----------------------------
-- Table structure for "main"."skillprop"
-- ----------------------------
DROP TABLE "main"."skillprop";
CREATE TABLE "skillprop" (
"ID"  smallint NOT NULL,
"Name"  varchar(32) NOT NULL,
"Introduction"  varchar(255) NOT NULL,
"SkillLevel"  smallint NOT NULL DEFAULT 0,
"Color"  smallint NOT NULL,
"SkillGrade"  smallint NOT NULL DEFAULT 1,
"UpgradeCardNum"  smallint NOT NULL,
"StudyLevel"  smallint NOT NULL DEFAULT 1,
"PreSkillID1"  smallint NOT NULL DEFAULT 101,
"PreSkillLevel1"  smallint NOT NULL DEFAULT 1,
"PreSkillID2"  smallint NOT NULL DEFAULT 0,
"PreSkillLevel2"  smallint NOT NULL DEFAULT 0,
"TriggerRate"  smallint NOT NULL DEFAULT 20,
"EffectKindID"  smallint NOT NULL,
"EffectHurtRate"  smallint NOT NULL,
"EffectParam"  smallint NOT NULL,
"BuffKindID1"  smallint NOT NULL DEFAULT 0,
"BuffRound1"  smallint NOT NULL DEFAULT 0,
"BuffParam1"  smallint NOT NULL DEFAULT 0,
"BuffRate1"  smallint NOT NULL DEFAULT 100,
"BuffKindID2"  smallint NOT NULL DEFAULT 0,
"BuffRound2"  smallint NOT NULL DEFAULT 0,
"BuffParam2"  smallint NOT NULL DEFAULT 0,
"BuffRate2"  smallint NOT NULL DEFAULT 100,
"BuffKindID3"  smallint NOT NULL DEFAULT 0,
"BuffRound3"  smallint NOT NULL DEFAULT 0,
"BuffParam3"  smallint NOT NULL DEFAULT 0,
"BuffRate3"  smallint NOT NULL DEFAULT 100,
PRIMARY KEY ("ID" ASC, "SkillLevel" ASC)
);

-- ----------------------------
-- Records of skillprop
-- ----------------------------
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 1, 1, 1, 1, 5, 0, 0, 0, 0, 33, 1, 115, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 2, 1, 1, 2, 8, 0, 0, 0, 0, 33, 1, 120, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 3, 1, 1, 3, 11, 0, 0, 0, 0, 33, 1, 125, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 4, 1, 1, 4, 14, 0, 0, 0, 0, 33, 1, 130, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 5, 1, 1, 5, 17, 0, 0, 0, 0, 33, 1, 135, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 6, 1, 1, 6, 20, 0, 0, 0, 0, 33, 1, 140, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 7, 1, 1, 7, 23, 0, 0, 0, 0, 33, 1, 145, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 8, 1, 1, 8, 26, 0, 0, 0, 0, 33, 1, 150, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 9, 1, 1, 9, 29, 0, 0, 0, 0, 33, 1, 155, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1, '极冰锥', '极冰锥', 10, 1, 1, 10, 32, 0, 0, 0, 0, 33, 1, 160, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 1, 1, 1, 1, 8, 0, 0, 0, 0, 33, 1, 115, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 2, 1, 1, 2, 11, 0, 0, 0, 0, 33, 1, 120, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 3, 1, 1, 3, 14, 0, 0, 0, 0, 33, 1, 125, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 4, 1, 1, 4, 17, 0, 0, 0, 0, 33, 1, 130, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 5, 1, 1, 5, 20, 0, 0, 0, 0, 33, 1, 135, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 6, 1, 1, 6, 23, 0, 0, 0, 0, 33, 1, 140, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 7, 1, 1, 7, 26, 0, 0, 0, 0, 33, 1, 145, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 8, 1, 1, 8, 29, 0, 0, 0, 0, 33, 1, 150, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 9, 1, 1, 9, 32, 0, 0, 0, 0, 33, 1, 155, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (2, '炽焰陨', '炽焰陨', 10, 1, 1, 10, 35, 0, 0, 0, 0, 33, 1, 160, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 1, 1, 1, 1, 12, 0, 0, 0, 0, 33, 1, 115, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 2, 1, 1, 2, 15, 0, 0, 0, 0, 33, 1, 120, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 3, 1, 1, 3, 18, 0, 0, 0, 0, 33, 1, 125, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 4, 1, 1, 4, 21, 0, 0, 0, 0, 33, 1, 130, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 5, 1, 1, 5, 24, 0, 0, 0, 0, 33, 1, 135, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 6, 1, 1, 6, 27, 0, 0, 0, 0, 33, 1, 140, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 7, 1, 1, 7, 30, 0, 0, 0, 0, 33, 1, 145, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 8, 1, 1, 8, 33, 0, 0, 0, 0, 33, 1, 150, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 9, 1, 1, 9, 36, 0, 0, 0, 0, 33, 1, 155, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (3, '天雷轰', '天雷轰', 10, 1, 1, 10, 39, 0, 0, 0, 0, 33, 1, 160, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 1, 1, 1, 1, 15, 0, 0, 0, 0, 33, 1, 115, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 2, 1, 1, 2, 18, 0, 0, 0, 0, 33, 1, 120, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 3, 1, 1, 3, 21, 0, 0, 0, 0, 33, 1, 125, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 4, 1, 1, 4, 24, 0, 0, 0, 0, 33, 1, 130, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 5, 1, 1, 5, 27, 0, 0, 0, 0, 33, 1, 135, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 6, 1, 1, 6, 30, 0, 0, 0, 0, 33, 1, 140, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 7, 1, 1, 7, 33, 0, 0, 0, 0, 33, 1, 145, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 8, 1, 1, 8, 36, 0, 0, 0, 0, 33, 1, 150, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 9, 1, 1, 9, 39, 0, 0, 0, 0, 33, 1, 155, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (4, '风焱斩', '风焱斩', 10, 1, 1, 10, 42, 0, 0, 0, 0, 33, 1, 160, 0, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 1, 1, 2, 1, 18, 0, 0, 0, 0, 33, 1, 125, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 2, 1, 2, 2, 21, 0, 0, 0, 0, 33, 1, 130, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 3, 1, 2, 3, 24, 0, 0, 0, 0, 33, 1, 135, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 4, 1, 2, 4, 27, 0, 0, 0, 0, 33, 1, 140, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 5, 1, 2, 5, 30, 0, 0, 0, 0, 33, 1, 145, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 6, 1, 2, 6, 33, 0, 0, 0, 0, 33, 1, 150, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 7, 1, 2, 7, 36, 0, 0, 0, 0, 33, 1, 155, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 8, 1, 2, 8, 39, 0, 0, 0, 0, 33, 1, 160, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 9, 1, 2, 9, 42, 0, 0, 0, 0, 33, 1, 165, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1001, '风卷尘生', '风卷尘生', 10, 1, 2, 10, 45, 0, 0, 0, 0, 33, 1, 170, 0, 2, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 1, 1, 2, 1, 21, 0, 0, 0, 0, 33, 1, 125, 150, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 2, 1, 2, 2, 24, 0, 0, 0, 0, 33, 1, 130, 200, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 3, 1, 2, 3, 27, 0, 0, 0, 0, 33, 1, 135, 250, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 4, 1, 2, 4, 30, 0, 0, 0, 0, 33, 1, 140, 300, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 5, 1, 2, 5, 33, 0, 0, 0, 0, 33, 1, 145, 350, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 6, 1, 2, 6, 36, 0, 0, 0, 0, 33, 1, 150, 400, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 7, 1, 2, 7, 39, 0, 0, 0, 0, 33, 1, 155, 450, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 8, 1, 2, 8, 42, 0, 0, 0, 0, 33, 1, 160, 500, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 9, 1, 2, 9, 45, 0, 0, 0, 0, 33, 1, 165, 550, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1002, '九黎之光', '九黎之光', 10, 1, 2, 10, 48, 0, 0, 0, 0, 33, 1, 170, 600, 0, 0, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 1, 1, 2, 1, 24, 0, 0, 0, 0, 33, 1, 125, 0, 3, 2, 100, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 2, 1, 2, 2, 27, 0, 0, 0, 0, 33, 1, 130, 0, 3, 2, 140, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 3, 1, 2, 3, 30, 0, 0, 0, 0, 33, 1, 135, 0, 3, 2, 180, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 4, 1, 2, 4, 33, 0, 0, 0, 0, 33, 1, 140, 0, 3, 2, 220, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 5, 1, 2, 5, 36, 0, 0, 0, 0, 33, 1, 145, 0, 3, 2, 260, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 6, 1, 2, 6, 39, 0, 0, 0, 0, 33, 1, 150, 0, 3, 2, 300, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 7, 1, 2, 7, 42, 0, 0, 0, 0, 33, 1, 155, 0, 3, 2, 340, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 8, 1, 2, 8, 45, 0, 0, 0, 0, 33, 1, 160, 0, 3, 2, 380, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 9, 1, 2, 9, 48, 0, 0, 0, 0, 33, 1, 165, 0, 3, 2, 420, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1003, '地狱魔眼', '地狱魔眼', 10, 1, 2, 10, 51, 0, 0, 0, 0, 33, 1, 170, 0, 3, 2, 460, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 1, 1, 2, 1, 27, 0, 0, 0, 0, 33, 1, 125, 0, 4, 2, 10, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 2, 1, 2, 2, 30, 0, 0, 0, 0, 33, 1, 130, 0, 4, 2, 12, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 3, 1, 2, 3, 33, 0, 0, 0, 0, 33, 1, 135, 0, 4, 2, 14, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 4, 1, 2, 4, 36, 0, 0, 0, 0, 33, 1, 140, 0, 4, 2, 16, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 5, 1, 2, 5, 39, 0, 0, 0, 0, 33, 1, 145, 0, 4, 2, 18, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 6, 1, 2, 6, 42, 0, 0, 0, 0, 33, 1, 150, 0, 4, 2, 20, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 7, 1, 2, 7, 45, 0, 0, 0, 0, 33, 1, 155, 0, 4, 2, 22, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 8, 1, 2, 8, 48, 0, 0, 0, 0, 33, 1, 160, 0, 4, 2, 24, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 9, 1, 2, 9, 51, 0, 0, 0, 0, 33, 1, 165, 0, 4, 2, 26, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (1004, '断魂刀法', '断魂刀法', 10, 1, 2, 10, 54, 0, 0, 0, 0, 33, 1, 170, 0, 4, 2, 28, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 1, 1, 1, 1, 30, 0, 0, 0, 0, 33, 1, 140, 0, 12, 2, 10, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 2, 1, 1, 2, 33, 0, 0, 0, 0, 33, 1, 150, 0, 12, 2, 15, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 3, 1, 1, 3, 36, 0, 0, 0, 0, 33, 1, 160, 0, 12, 2, 20, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 4, 1, 1, 4, 39, 0, 0, 0, 0, 33, 1, 170, 0, 12, 2, 25, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 5, 1, 1, 5, 42, 0, 0, 0, 0, 33, 1, 180, 0, 12, 2, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 6, 1, 1, 6, 45, 0, 0, 0, 0, 33, 1, 190, 0, 12, 2, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 7, 1, 1, 7, 48, 0, 0, 0, 0, 33, 1, 200, 0, 12, 2, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 8, 1, 1, 8, 51, 0, 0, 0, 0, 33, 1, 210, 0, 12, 2, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 9, 1, 1, 9, 54, 0, 0, 0, 0, 33, 1, 220, 0, 12, 2, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10001, '无极冰风舞', '无极冰风舞', 10, 1, 1, 10, 57, 0, 0, 0, 0, 33, 1, 230, 0, 12, 2, 100, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 1, 1, 1, 1, 33, 0, 0, 0, 0, 33, 1, 140, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 2, 1, 1, 2, 36, 0, 0, 0, 0, 33, 1, 150, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 3, 1, 1, 3, 39, 0, 0, 0, 0, 33, 1, 160, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 4, 1, 1, 4, 42, 0, 0, 0, 0, 33, 1, 170, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 5, 1, 1, 5, 45, 0, 0, 0, 0, 33, 1, 180, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 6, 1, 1, 6, 48, 0, 0, 0, 0, 33, 1, 190, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 7, 1, 1, 7, 51, 0, 0, 0, 0, 33, 1, 200, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 8, 1, 1, 8, 54, 0, 0, 0, 0, 33, 1, 210, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 9, 1, 1, 9, 57, 0, 0, 0, 0, 33, 1, 220, 0, 1, 1, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10002, '真焱光龙咒', '真焱光龙咒', 10, 1, 1, 10, 60, 0, 0, 0, 0, 33, 1, 230, 0, 1, 2, 0, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 1, 1, 1, 1, 33, 0, 0, 0, 0, 33, 1, 140, 0, 15, 1, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 2, 1, 1, 2, 36, 0, 0, 0, 0, 33, 1, 150, 0, 15, 1, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 3, 1, 1, 3, 39, 0, 0, 0, 0, 33, 1, 160, 0, 15, 1, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 4, 1, 1, 4, 42, 0, 0, 0, 0, 33, 1, 170, 0, 15, 1, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 5, 1, 1, 5, 45, 0, 0, 0, 0, 33, 1, 180, 0, 15, 1, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 6, 1, 1, 6, 48, 0, 0, 0, 0, 33, 1, 190, 0, 15, 1, 55, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 7, 1, 1, 7, 51, 0, 0, 0, 0, 33, 1, 200, 0, 15, 1, 60, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 8, 1, 1, 8, 54, 0, 0, 0, 0, 33, 1, 210, 0, 15, 1, 65, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 9, 1, 1, 9, 57, 0, 0, 0, 0, 33, 1, 220, 0, 15, 1, 70, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10003, '炼狱魔雷阵', '炼狱魔雷阵', 10, 1, 1, 10, 60, 0, 0, 0, 0, 33, 1, 230, 0, 15, 1, 100, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 1, 1, 1, 1, 33, 0, 0, 0, 0, 33, 1, 140, 0, 5, 2, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 2, 1, 1, 2, 36, 0, 0, 0, 0, 33, 1, 150, 0, 5, 2, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 3, 1, 1, 3, 39, 0, 0, 0, 0, 33, 1, 160, 0, 5, 2, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 4, 1, 1, 4, 42, 0, 0, 0, 0, 33, 1, 170, 0, 5, 2, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 5, 1, 1, 5, 45, 0, 0, 0, 0, 33, 1, 180, 0, 5, 2, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 6, 1, 1, 6, 48, 0, 0, 0, 0, 33, 1, 190, 0, 5, 2, 55, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 7, 1, 1, 7, 51, 0, 0, 0, 0, 33, 1, 200, 0, 5, 2, 60, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 8, 1, 1, 8, 54, 0, 0, 0, 0, 33, 1, 210, 0, 5, 2, 65, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 9, 1, 1, 9, 57, 0, 0, 0, 0, 33, 1, 220, 0, 5, 2, 70, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (10004, '九幽玄冥斩', '九幽玄冥斩', 10, 1, 1, 10, 60, 0, 0, 0, 0, 33, 1, 230, 0, 5, 2, 100, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 1, 1, 1, 1, 9, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 2, 1, 1, 2, 12, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 3, 1, 1, 3, 15, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 4, 1, 1, 4, 18, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 5, 1, 1, 5, 21, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 6, 1, 1, 6, 24, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 55, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 7, 1, 1, 7, 27, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 60, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 8, 1, 1, 8, 30, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 65, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 9, 1, 1, 9, 33, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 70, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20001, '火之护盾', '火之护盾', 10, 1, 1, 10, 36, 0, 0, 0, 0, 33, 1, 0, 0, 16, 3, 75, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 1, 1, 1, 1, 13, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 2, 1, 1, 2, 16, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 3, 1, 1, 3, 19, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 4, 1, 1, 4, 22, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 5, 1, 1, 5, 25, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 6, 1, 1, 6, 28, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 55, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 7, 1, 1, 7, 31, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 60, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 8, 1, 1, 8, 34, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 65, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 9, 1, 1, 9, 37, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 70, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20002, '反弹之盾', '反弹之盾', 10, 1, 1, 10, 40, 0, 0, 0, 0, 33, 1, 0, 0, 18, 3, 75, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 1, 1, 1, 1, 16, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 20, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 2, 1, 1, 2, 19, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 25, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 3, 1, 1, 3, 22, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 4, 1, 1, 4, 25, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 5, 1, 1, 5, 28, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 6, 1, 1, 6, 31, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 7, 1, 1, 7, 34, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 8, 1, 1, 8, 37, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 55, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 9, 1, 1, 9, 40, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 60, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20003, '神域格挡', '神域格挡', 10, 1, 1, 10, 43, 0, 0, 0, 0, 33, 1, 0, 0, 19, 3, 65, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 1, 1, 1, 1, 20, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 30, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 2, 1, 1, 2, 23, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 35, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 3, 1, 1, 3, 26, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 40, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 4, 1, 1, 4, 29, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 45, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 5, 1, 1, 5, 32, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 50, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 6, 1, 1, 6, 35, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 55, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 7, 1, 1, 7, 38, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 60, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 8, 1, 1, 8, 41, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 65, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 9, 1, 1, 9, 44, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 70, 100, 0, 0, 0, 100, 0, 0, 0, 100);
INSERT INTO "main"."skillprop" VALUES (20004, '绝地反击', '绝地反击', 10, 1, 1, 10, 47, 0, 0, 0, 0, 33, 1, 0, 0, 17, 3, 75, 100, 0, 0, 0, 100, 0, 0, 0, 100);
