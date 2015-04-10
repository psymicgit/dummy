-- MySQL dump 10.13  Distrib 5.5.30, for Win64 (x86)
--
-- Host: 192.168.0.31    Database: interface
-- ------------------------------------------------------
-- Server version	5.5.28-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `exchange_tbs1`
--

DROP TABLE IF EXISTS `exchange_tbs1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exchange_tbs1` (
  `OrderID` varchar(255) NOT NULL COMMENT '订单编号',
  `Source` varchar(45) DEFAULT NULL COMMENT '数据来源',
  `Amount` int(11) DEFAULT NULL COMMENT '金额，单位为分',
  `Partner` varchar(45) DEFAULT NULL COMMENT '游戏编号',
  `PlayerID` int(10) unsigned DEFAULT NULL COMMENT '玩家ID',
  `DateCreate` datetime DEFAULT NULL COMMENT '订单创建日期',
  `DateExchange` datetime DEFAULT NULL COMMENT '从平台商收到该订单数据的时间',
  `DateProcess` datetime DEFAULT NULL COMMENT '数据被处理时间',
  `IsProcess` tinyint(1) DEFAULT '0' COMMENT '订单是否被处理',
  `PlayerLevel` int(11) DEFAULT NULL COMMENT '玩家等级',
  `ZoneID` int(11) DEFAULT NULL COMMENT '区服',
  PRIMARY KEY (`OrderID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `exchange_tbs1`
--

LOCK TABLES `exchange_tbs1` WRITE;
/*!40000 ALTER TABLE `exchange_tbs1` DISABLE KEYS */;
INSERT INTO `exchange_tbs1` VALUES ('1','1',1,'1',1,'2013-03-18 15:23:15',NULL,NULL,0,NULL,NULL);
/*!40000 ALTER TABLE `exchange_tbs1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `global_amount`
--

DROP TABLE IF EXISTS `global_amount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `global_amount` (
  `UserID` int(11) NOT NULL COMMENT '用户ID（全局唯一）',
  `PlayerID` int(11) NOT NULL COMMENT '玩家ID（不同区不同）',
  `TotalAmount` int(11) NOT NULL COMMENT '最该充值记录',
  `ZoneID` int(11) NOT NULL COMMENT '区服ID',
  `LastExchangeTime` datetime NOT NULL COMMENT '上次充值时间',
  `RecentDayAmount` int(11) NOT NULL,
  PRIMARY KEY (`UserID`,`PlayerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPRESSED;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `global_amount`
--

LOCK TABLES `global_amount` WRITE;
/*!40000 ALTER TABLE `global_amount` DISABLE KEYS */;
/*!40000 ALTER TABLE `global_amount` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-04-01 14:25:43
