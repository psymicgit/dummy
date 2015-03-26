select PlayerID, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 group by PlayerID order by OpTimeDesc limit 1;

select from players where Pay > 0 and PlayerID not in (select PlayerID from mysql.test_player) and LastLoginTime >= "2014-01-01 00:00:00";


select ID, PlayerID, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 where OpTime < "2014-04-01 00:00:00";



select PlayerID, OpTimem Gold from (select PlayerID, OpTime, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 where OpTime < "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID, g.OpTime;

select * from players p, where p.PlayerID in (select PlayerID, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold) goldrecord_2014 g 

select ID, PlayerID, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 where OpTime < "2014-04-01 00:00:00";


select PlayerID, OpTime, Gold from (select PlayerID, OpTime, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 where OpTime < "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID;


select p.* from rmb_player p, (select PlayerID, OpTime, Gold from (select PlayerID, OpTime, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 where OpTime < "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID) g where p.PlayerID = g.PlayerID and p.Gold <> g.Gold;

# update rmb_player p, (select PlayerID, OpTime, Gold from (select PlayerID, OpTime, AfterRMBGold + AfterRMBGoldGift + AfterGmGold + AfterGameGold as Gold from goldrecord_2014 where OpTime < "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID) g set p.Gold=g.Gold where p.PlayerID = g.PlayerID;




create table if not exists rmb_player (	
	PlayerID int(11) unsigned not null comment '角色ID',
	UserName varchar(64) binary NOT NULL DEFAULT '' COMMENT '账号',
	DispName varchar(64) binary NOT NULL DEFAULT '' COMMENT '角色',
	RMB int(11) unsigned not null comment '人民币',
	Gold int(11) unsigned not null comment '元宝',
	LastLoginTime datetime comment '最后登录时间',
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

replace into rmb_player(PlayerID,UserName,DispName,RMB,Gold,LastLoginTime) select PlayerID, UserName,DispName,Pay,Gold,LastLoginTime from players where Pay > 0 and PlayerID not in (select PlayerID from mysql.test_player);

drop table if exists sumaddgold;
create table sumaddgold as select PlayerID, sum(Gold) as Gold from (select PlayerID, OpRMBGold + OpRMBGoldGift + OpGmGold + OpGameGold as Gold from goldrecord_2014 where ModuleType = 12 and OpTime > "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID;

drop table if exists sumdecgold;
create table sumdecgold as select PlayerID, sum(Gold) as Gold from (select PlayerID, OpRMBGold + OpRMBGoldGift + OpGmGold + OpGameGold as Gold from goldrecord_2014 where ModuleType = 13 and OpTime > "2014-04-01 00:00:00" order by PlayerID asc, OpTime desc) as g group by g.PlayerID;

update rmb_player p, sumaddgold s set p.Gold = p.Gold - s.Gold where p.PlayerID = s.PlayerID;
update rmb_player p, sumdecgold s set p.Gold = p.Gold + s.Gold where p.PlayerID = s.PlayerID;

drop table if exists sumaddgold;
drop table if exists sumdecgold;

create table if not exists mysql.rmb_player (
	PlayerID int(11) unsigned not null comment '角色ID',
	UserName varchar(64) binary NOT NULL DEFAULT '' COMMENT '账号',
	DispName varchar(64) binary NOT NULL DEFAULT '' COMMENT '角色',
	RMB int(11) unsigned not null comment '人民币',
	Gold int(11) unsigned not null comment '元宝',
	LastLoginTime datetime comment '最后登录时间',
	primary key (PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

replace into mysql.rmb_player(PlayerID,UserName,DispName,RMB,Gold,LastLoginTime) select PlayerID,UserName,DispName,RMB,Gold,LastLoginTime from rmb_player order by Gold desc limit 1000;