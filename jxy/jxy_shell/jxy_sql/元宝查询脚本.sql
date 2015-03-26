导出2013年12月1日-2014年3月31日的订单表

ios
drop table if exists pay_from_2013_12_to_2014_4;
create table pay_from_2013_12_to_2014_4 as select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB from exchange e,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where 0;

insert into pay_from_2013_12_to_2014_4 select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB/100 from exchange e,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where IsProcess = 1 and e.PlayerID = g.playerid and e.UserID = u.userid and e.UserID <> 33 and e.DateProcess >= '2013-12-01 00:00:00' and e.DateProcess < '2014-04-01 00:00:00' and e.PlayerID not in (select PlayerID from ios_app_usercenter.test_player);

insert into pay_from_2013_12_to_2014_4 select r.CreateTime,r.OrderID,r.ZoneID,u.UserID,u.username,r.PlayerID,g.nickname,r.RMB/100 from myrecharge r,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where r.PlayerID = g.playerid and g.UserID = u.userid and u.UserID <> 33 and r.CreateTime >= '2013-12-01 00:00:00' and r.CreateTime < '2014-04-01 00:00:00' and r.PlayerID not in (select PlayerID from ios_app_usercenter.test_player);

insert into pay_from_2013_12_to_2014_4 select p.createtime,p.orderid,p.zoneid,p.userid,u.username,p.PlayerID,g.nickname,p.rmb from ios_app_usercenter.payed p,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where p.PlayerID = g.playerid and p.userid = u.userid and u.userid <> 33 and p.createtime >= '2013-12-01 00:00:00' and p.createtime < '2014-04-01 00:00:00'  and p.PlayerID not in (select PlayerID from ios_app_usercenter.test_player);

select DateProcess,OrderID,ZoneID,UserID,username,PlayerID,nickname,RMB from pay_from_2013_12_to_2014_4 order by DateProcess asc into outfile '/tmp/pay_from_2013_12_to_2014_4.csv' fields terminated by ',' enclosed by "";



安卓
drop table if exists pay_from_2013_12_to_2014_4;
create table pay_from_2013_12_to_2014_4 as select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB from exchange e,and_app_usercenter.globalplayers g, and_app_usercenter.users u where 0;

insert into pay_from_2013_12_to_2014_4 select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB/100 from exchange e,and_app_usercenter.globalplayers g, and_app_usercenter.users u where IsProcess = 1 and e.PlayerID = g.playerid and e.UserID = u.userid and e.UserID <> 33 and e.DateProcess >= '2013-12-01 00:00:00' and e.DateProcess < '2014-04-01 00:00:00' and e.PlayerID not in (select PlayerID from and_app_usercenter.test_player);

insert into pay_from_2013_12_to_2014_4 select r.CreateTime,r.OrderID,r.ZoneID,u.UserID,u.username,r.PlayerID,g.nickname,r.RMB/100 from myrecharge r,and_app_usercenter.globalplayers g, and_app_usercenter.users u where r.PlayerID = g.playerid and g.UserID = u.userid and u.UserID <> 33 and r.CreateTime >= '2013-12-01 00:00:00' and r.CreateTime < '2014-04-01 00:00:00' and r.PlayerID not in (select PlayerID from and_app_usercenter.test_player);

insert into pay_from_2013_12_to_2014_4 select p.createtime,p.orderid,p.zoneid,p.userid,u.username,p.PlayerID,g.nickname,p.rmb from and_app_usercenter.payed p,and_app_usercenter.globalplayers g, and_app_usercenter.users u where p.PlayerID = g.playerid and p.userid = u.userid and u.userid <> 33 and p.createtime >= '2013-12-01 00:00:00' and p.createtime < '2014-04-01 00:00:00'  and p.PlayerID not in (select PlayerID from and_app_usercenter.test_player);

select DateProcess,OrderID,ZoneID,UserID,username,PlayerID,nickname,RMB from pay_from_2013_12_to_2014_4 order by DateProcess asc into outfile '/tmp/pay_from_2013_12_to_2014_4.csv' fields terminated by ',' enclosed by "";






导出所有订单
ios

drop table if exists pay_2013_2014_4;
create table pay_2013_2014_4 as select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB,"255.255.255.255" as IP from exchange e,ios_app_usercenter.globalplayers g,ios_app_usercenter.users u where 0;

insert into pay_2013_2014_4 select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB/100,'' as IP from exchange e,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where IsProcess = 1 and e.PlayerID = g.playerid and e.UserID = u.userid and e.UserID <> 33 and e.PlayerID not in (select PlayerID from ios_app_usercenter.test_player);

insert into pay_2013_2014_4 select r.CreateTime,r.OrderID,r.ZoneID,u.UserID,u.username,r.PlayerID,g.nickname,r.RMB/100,'' as IP from myrecharge r,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where r.PlayerID = g.playerid and g.UserID = u.userid and u.UserID <> 33 and r.PlayerID not in (select PlayerID from ios_app_usercenter.test_player);

insert into pay_2013_2014_4 select p.createtime,p.orderid,p.zoneid,p.userid,u.username,p.PlayerID,g.nickname,p.rmb,p.createip from ios_app_usercenter.payed p,ios_app_usercenter.globalplayers g, ios_app_usercenter.users u where p.PlayerID = g.playerid and p.userid = u.userid and u.userid <> 33 and p.PlayerID not in (select PlayerID from ios_app_usercenter.test_player);

select DateProcess,OrderID,ZoneID,UserID,username,PlayerID,nickname,RMB,IP from pay_2013_2014_4 order by DateProcess asc into outfile '/tmp/ios_pay_2013_2014.csv' fields terminated by ',' enclosed by "";


安卓
drop table if exists pay_2013_2014_4;
create table pay_2013_2014_4 as select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB,"255.255.255.255" as IP from exchange e,and_app_usercenter.globalplayers g,and_app_usercenter.users u where 0;

insert into pay_2013_2014_4 select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB/100,'' as IP from exchange e,and_app_usercenter.globalplayers g, and_app_usercenter.users u where IsProcess = 1 and e.PlayerID = g.playerid and e.UserID = u.userid and e.UserID <> 33 and e.PlayerID not in (select PlayerID from and_app_usercenter.test_player);

insert into pay_2013_2014_4 select r.CreateTime,r.OrderID,r.ZoneID,u.UserID,u.username,r.PlayerID,g.nickname,r.RMB/100,'' as IP from myrecharge r,and_app_usercenter.globalplayers g, and_app_usercenter.users u where r.PlayerID = g.playerid and g.UserID = u.userid and u.UserID <> 33 and r.PlayerID not in (select PlayerID from and_app_usercenter.test_player);

insert into pay_2013_2014_4 select p.createtime,p.orderid,p.zoneid,p.userid,u.username,p.PlayerID,g.nickname,p.rmb,p.createip from and_app_usercenter.payed p,and_app_usercenter.globalplayers g, and_app_usercenter.users u where p.PlayerID = g.playerid and p.userid = u.userid and u.userid <> 33 and p.PlayerID not in (select PlayerID from and_app_usercenter.test_player);

select DateProcess,OrderID,ZoneID,UserID,username,PlayerID,nickname,RMB,IP from pay_2013_2014_4 order by DateProcess asc into outfile '/tmp/and_pay_2013_2014.csv' fields terminated by ',' enclosed by "";






查询3值总额
ios

select sum(RMB) / 100 into @a from exchange where IsProcess = 1 and DateCreate >= '2014-01-01 00:00:00' and DateCreate < '2014-03-01 00:00:00';
select sum(RMB) / 100 into @b from myrecharge where CreateTime >= '2014-01-01 00:00:00' and CreateTime < '2014-03-01 00:00:00';
select sum(rmb) into @c from and_app_usercenter.payed where createtime >= '2014-01-01 00:00:00' and createtime < '2014-03-01 00:00:00';
set @totalrmb=@a+@b+@c;

select @a, @b, @c, @totalrmb as 总充值总额;