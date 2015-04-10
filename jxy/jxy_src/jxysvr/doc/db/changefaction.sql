SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `singlepram`
-- ----------------------------
ALTER TABLE faction ADD COLUMN TodayPassNum smallint(5) unsigned NOT NULL DEFAULT '0';

ALTER TABLE faction ADD COLUMN `LastAuditTime` datetime NOT NULL DEFAULT '2013-12-19 00:00:00';