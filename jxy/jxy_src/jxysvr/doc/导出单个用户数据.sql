	SELECT * FROM player_bags WHERE playerid = 114941  into outfile "/home/player_bags.txt";
	SELECT * FROM player_builds WHERE playerid = 114941  into outfile "/home/player_builds.txt";
	SELECT * FROM player_ext WHERE playerid = 114941  into outfile "/home/player_ext.txt";
	SELECT * FROM player_hero_ext WHERE playerid = 114941  into outfile "/home/player_hero_ext.txt";
	SELECT * FROM player_heros WHERE playerid = 114941  into outfile "/home/player_heros.txt";
	SELECT * FROM player_instances WHERE playerid = 114941  into outfile "/home/player_instances.txt";
	SELECT * FROM player_pvp WHERE playerid = 114941  into outfile "/home/player_pvp.txt";
	SELECT * FROM player_tasks WHERE playerid = 114941  into outfile "/home/player_tasks.txt";
	SELECT * FROM players WHERE playerid = 114941  into outfile "/home/players.txt";
	
	SELECT * FROM player_bags WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_bags.txt";
	SELECT * FROM player_builds WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_builds.txt";
	SELECT * FROM player_ext WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_ext.txt";
	SELECT * FROM player_hero_ext WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_hero_ext.txt";
	SELECT * FROM player_heros WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_heros.txt";
	SELECT * FROM player_instances WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_instances.txt";
	SELECT * FROM player_pvp WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_pvp.txt";
	SELECT * FROM player_tasks WHERE playerid = 114941  into outfile "/home/mysql/tmp/player_tasks.txt";
	SELECT * FROM players WHERE playerid = 114941  into outfile "/home/mysql/tmp/players.txt";
	
	
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_bags.txt' into table player_bags;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_builds.txt' into table player_builds;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_ext.txt' into table player_ext;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_hero_ext.txt' into table player_hero_ext;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_heros.txt' into table player_heros;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_instances.txt' into table player_instances;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_pvp.txt' into table player_pvp;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\player_tasks.txt' into table player_tasks;
	LOAD DATA local INFILE 'F:\\maxnet\\tmp\\players.txt' into table players;
	
delete from player_bags where PlayerID = 114941;
delete from player_builds where PlayerID = 114941;
delete from player_ext where PlayerID = 114941;
delete from player_hero_ext where PlayerID = 114941;
delete from player_heros where PlayerID = 114941;
delete from player_instances where PlayerID = 114941;
delete from player_pvp where PlayerID = 114941;
delete from player_tasks where PlayerID = 114941;
delete from players where PlayerID = 114941;

select * from player_bags where PlayerID = 114941;
select * from player_builds where PlayerID = 114941;
select * from player_ext where PlayerID = 114941;
select * from player_hero_ext where PlayerID = 114941;
select * from player_heros where PlayerID = 114941;
select * from player_instances where PlayerID = 114941;
select * from player_pvp where PlayerID = 114941;
select * from player_tasks where PlayerID = 114941;
select * from players where PlayerID = 114941;
select * from player_consume where PlayerID = 114941;

http://blog.csdn.net/xin_yu_xin/article/details/7574662
	
	
#导出单个用户数据
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_bags --where "playerid = 114941"  > player_bags.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_builds --where "playerid = 114941"  > player_builds.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_ext --where "playerid = 114941"  > player_ext.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_hero_ext --where "playerid = 114941"  > player_hero_ext.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_heros --where "playerid = 114941"  > player_heros.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_instances --where "playerid = 114941"  > player_instances.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_pvp --where "playerid = 114941"  > player_pvp.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 player_tasks --where "playerid = 114941"  > player_tasks.sql;
/home/mysql/mysql_63316/bin/mysqldump -uroot -pMaxnet.1234 -h127.0.0.1 --port=63316  --hex-blob --default-character-set=utf8 -t ios_app_s255 players --where "playerid = 114941"  > players.sql;