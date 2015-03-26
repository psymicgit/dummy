create table if not exists mysql.rmb_login (	
	PlayerID int(11) unsigned not null comment '角色ID',
	LastLoginTime datetime comment '最后登录时间',
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert ignore into mysql.rmb_login(PlayerID,LastLoginTime) select PlayerID, Time from (select PlayerID, Time from playerloginoff where Login = 1 and Time < "2014-04-01 00:00:00" order by PlayerID asc, Time desc) g group by g.PlayerID;
