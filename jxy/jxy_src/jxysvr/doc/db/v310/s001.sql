ALTER TABLE player_consume ADD BuyPhyStrengthNum BIGINT(20) DEFAULT 0;
ALTER TABLE player_bags ADD `Jewelry` blob NOT NULL AFTER `Goods`;