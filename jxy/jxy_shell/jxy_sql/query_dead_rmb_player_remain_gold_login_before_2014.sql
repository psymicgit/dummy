create table if not exists mysql.player_gold_login_before_2014(
	PlayerID int(11) unsigned not null comment '角色ID',
	Gold int(11) unsigned not null comment '元宝',
	LastLoginTime datetime not null,
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert ignore into mysql.player_gold_login_before_2014 select PlayerID, Gold, LastLoginTime from players where LastLoginTime < '2014-01-01 00:00:00';
