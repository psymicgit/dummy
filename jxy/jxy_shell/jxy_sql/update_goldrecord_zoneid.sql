select count(1) as 无区的玩家数 from goldrecord_2014 where ZoneID = 0;
select count(1) as 有区的玩家数 from goldrecord_2014 where ZoneID <> 0;

select count(1) as 匹配的玩家数 from goldrecord_2014 g, usercenter.player_zone u where g.PlayerID = u.playerid;
update goldrecord_2014 g, usercenter.player_zone u set g.ZoneID = u.zoneid where g.PlayerID = u.playerid;

select 1 as 成功更新goldrecord_2014中的记录;

select count(1) as 无区的玩家数 from goldrecord_2014 where ZoneID = 0;
select count(1) as 有区的玩家数 from goldrecord_2014 where ZoneID <> 0;