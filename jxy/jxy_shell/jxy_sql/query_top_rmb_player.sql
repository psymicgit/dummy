create table if not exists rmb_player (	
	PlayerID int(11) unsigned not null comment '角色ID',
	ZoneID smallint unsigned not null comment '区',
	UserName varchar(64) binary NOT NULL DEFAULT '' COMMENT '账号',
	DispName varchar(64) binary NOT NULL DEFAULT '' COMMENT '角色',
	RMB int(11) unsigned not null comment '人民币',
	Gold int(11) not null comment '元宝',
	LastLoginTime datetime comment '最后登录时间',
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

replace into rmb_player(PlayerID,ZoneID,UserName,DispName,RMB,Gold,LastLoginTime) select PlayerID,OriZoneID,UserName,DispName,Pay,Gold,LastLoginTime from players where Pay > 0 and PlayerID not in (select PlayerID from mysql.test_player);

drop table if exists sumaddgold;
create table sumaddgold as select PlayerID, sum(Gold) as Gold from (select PlayerID, OpRMBGold + OpRMBGoldGift + OpGmGold + OpGameGold as Gold from goldrecord_2014 where ModuleType = 12 and OpTime > "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID;

drop table if exists sumdecgold;
create table sumdecgold as select PlayerID, sum(Gold) as Gold from (select PlayerID, OpRMBGold + OpRMBGoldGift + OpGmGold + OpGameGold as Gold from goldrecord_2014 where ModuleType = 13 and OpTime > "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID;

update rmb_player p, sumaddgold s set p.Gold = p.Gold - s.Gold where p.PlayerID = s.PlayerID;
update rmb_player p, sumdecgold s set p.Gold = p.Gold + s.Gold where p.PlayerID = s.PlayerID;

update rmb_player set Gold=0 where Gold < 0;

drop table if exists sumaddgold;
drop table if exists sumdecgold;

create table if not exists mysql.rmb_player (	
	PlayerID int(11) unsigned not null comment '角色ID',
	ZoneID smallint unsigned not null comment '区',
	UserName varchar(64) binary NOT NULL DEFAULT '' COMMENT '账号',
	DispName varchar(64) binary NOT NULL DEFAULT '' COMMENT '角色',
	RMB int(11) unsigned not null comment '人民币',
	Gold int(11) not null comment '元宝',
	LastLoginTime datetime comment '最后登录时间',
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

replace into mysql.rmb_player(PlayerID,ZoneID,UserName,DispName,RMB,Gold,LastLoginTime) select PlayerID,ZoneID,UserName,DispName,RMB,Gold,LastLoginTime from rmb_player order by Gold desc limit 1000;
drop table rmb_player;