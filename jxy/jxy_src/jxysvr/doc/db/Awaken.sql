SET FOREIGN_KEY_CHECKS=0;

ALTER TABLE singlepram ADD COLUMN SureOutOrangeAwaken smallint(5) unsigned NOT NULL DEFAULT '0';

UPDATE singlepram set SureOutOrangeAwaken = 5;