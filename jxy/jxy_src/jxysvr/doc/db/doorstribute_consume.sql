SET FOREIGN_KEY_CHECKS=0;

ALTER TABLE player_consume ADD COLUMN AddDoorsTribute bigint(20) unsigned NOT NULL DEFAULT '0';
ALTER TABLE player_consume ADD COLUMN DecDoorsTribute bigint(20) unsigned NOT NULL DEFAULT '0';
ALTER TABLE player_consume ADD COLUMN AfterDoorsTribute bigint(20) unsigned NOT NULL DEFAULT '0';

