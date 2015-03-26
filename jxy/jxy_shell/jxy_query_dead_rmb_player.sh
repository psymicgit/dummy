#  @File Name: .sh
#  @Author: 洪坤安
#  @Company: heihuo
#  @brief: 囧西游：让现在所有的正式服<ios/android/台湾/同步>都一一执行指定的sql脚本文件
#  @Created Time: 2014年3月13日 17:02:10

#!/bin/bash

source ./util.sh

declare err_cnt=0;
v=""

today=`date '+%Y_%m_%d'`

g_mysqldump_no_create_db_no_create_table="--verbose --no-create-db --no-create-info --add-locks --extended-insert --lock-tables --single-transaction --compress --complete-insert --quick --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

# 不建库，只建表和导数据
g_mysqldump_create_table_no_create_db="--no-create-db --single-transaction --compress --complete-insert --opt --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

# 依次链接数据库列表文件中的每一个数据库实例，检测是否成功
function ck_conn_mysql_by_mysql_list()
{
	local zone_list="$1"

	if [ -n "$zone_list" ] && [ ! -f $zone_list ]; then
		echo_err "error: can not found $zone_list file, exiting!"
		exit 1
	fi

	echo_blue "**********************************start connect to $zone_list**********************************************"

	# 依次检测是否能成功连上各个区的数据库
	while read line
	do
		# echo "$line"

		# 分割字符串
		fields=($(echo $line | tr ' ' ' ' | tr -s ' '))
		field_cnt=${#fields[@]}

		# 依次获取区id、游戏库名、游戏库ip、游戏库port、日志库名、日志库ip、日志库port
		zoneid=${fields[0]};
		gamedb=${fields[1]};
		gamedbip=${fields[2]};
		gamedbport=${fields[3]};
		logdb=${fields[4]};
		logdbip=${fields[5]};
		logdbport=${fields[6]};

		gamedbopt="-h$gamedbip -P$gamedbport -uroot -p$dbpwd"
		logdbopt="-h$logdbip -P$logdbport -uroot -p$dbpwd"

		gamedbdesc=$gamedb"     @ [ "$gamedbip" : "$gamedbport" ]"
		logdbdesc=$logdb" @ [ "$logdbip" : "$logdbport" ]"

		# 检测是否能成功连接得上游戏数据库
		ck_conn_ok_by_opt_and_desc "$gamedbdesc" "$gamedbopt"
		conn_mysql_succ=$?
		if [ "$conn_mysql_succ" -ne "0" ]; then
			((err_cnt++))
			exit 1
		fi
	done < $zone_list
	echo_blue "**********************************connect to $zone_list finished*******************************************"
}

# 根据传入的区列表，在每个区都执行一遍数据库语句
function get_player_remain_gold()
{
	# 服务器列表文件
	local zone_list=$1
	local cut_date=$2" 00:00:00"

	echo "**********************************start executing at $zone_list*******************************************"

	local player_gold_login=player_gold_login_${today};

mysql $g_paydbopt $g_usercenter_db << _EOFMYSQL_
		drop table if exists ${player_gold_login};
		create table if not exists ${player_gold_login}(
			PlayerID int(11) unsigned not null comment '角色ID',
			Gold int(11) unsigned not null comment '元宝',
			LastLoginTime datetime not null,
			primary key (PlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;
_EOFMYSQL_

	# 依次在每个游戏数据库执行传入的sql脚本
	while read line
	do
		# 分割字符串
		fields=($(echo $line | tr ' ' ' ' | tr -s ' '))
		field_cnt=${#fields[@]}

		# 依次获取区id、游戏库名、游戏库ip、游戏库port
		zoneid=${fields[0]};
		gamedb=${fields[1]};
		gamedbip=${fields[2]};
		gamedbport=${fields[3]};

		gamedbopt="-h$gamedbip -P$gamedbport -uroot -p$dbpwd"
		gamedb_ip_port="[ "$gamedbip" : "$gamedbport" ]"

		gamedbdesc=$gamedb" @ "$gamedb_ip_port

		echo_blue "-----------------start query gold at $gamedbdesc---------------"
		
mysql $gamedbopt -v $gamedb << _EOFMYSQL_
		drop table if exists ${player_gold_login};
		create table if not exists ${player_gold_login}(
			PlayerID int(11) unsigned not null comment '角色ID',
			Gold int(11) unsigned not null comment '元宝',
			LastLoginTime datetime not null,
			primary key (PlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

		insert ignore into ${player_gold_login} select PlayerID, Gold, LastLoginTime from players where LastLoginTime < '${cut_date}' and PlayerID > 1010 and PlayerID not in (select PlayerID from mysql.test_player);
_EOFMYSQL_

		mysqldump -v $gamedbopt $g_mysqldump_no_create_db_no_create_table $gamedb ${player_gold_login} | mysql $g_paydbopt $g_usercenter_db

mysql $gamedbopt -v $gamedb << _EOFMYSQL_
		drop table ${player_gold_login};
_EOFMYSQL_


		echo_blue "-----------------execute query gold at $gamedbdesc finished---------------"
	done < $zone_list

	echo_ok "**********************************executing at $zone_list finished*******************************************"
}

# 查询充值死亡玩家的剩余元宝
function query_dead_rmb_player_gold()
{
	local dead_date=$1" 00:00:00"

	local rmb_player=rmb_player_${today}
	local player_gold_login=player_gold_login_${today};

mysql $g_paydbopt -vvv $g_pay_db << _EOFMYSQL_
	# 充值人数
	drop table if exists $g_pay_db.${rmb_player};
	create table if not exists $g_pay_db.${rmb_player}(
		PlayerID int(11) unsigned not null comment '角色ID',
		Gold int(11) unsigned not null default 0 comment '元宝',
		LastLoginTime datetime not null default 0,
		primary key (PlayerID)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	# 死亡充值玩家
	insert ignore into ${rmb_player}(PlayerID) select distinct(PlayerID) from ${g_pay_db}.exchange where DateCreate < '${dead_date}' and RMB > 0;
	insert ignore into ${rmb_player}(PlayerID) select distinct(playerid) from ${g_usercenter_db}.payed where createtime < '${dead_date}' and rmb > 1;
	insert ignore into ${rmb_player}(PlayerID) select distinct(PlayerID) from ${g_pay_db}.myrecharge where CreateTime < '${dead_date}';

	select count(*) as '${dead_date}死亡充值玩家' from ${g_pay_db}.${rmb_player} r, ${g_usercenter_db}.globalplayers g where r.PlayerID = g.playerid and g.lastlogintime < '${dead_date}';

	update ${rmb_player} set Gold = 1;
	update ${rmb_player} r, ${g_usercenter_db}.globalplayers g set r.Gold = 0, r.LastLoginTime = g.lastlogintime where r.PlayerID = g.playerid and g.lastlogintime < '${dead_date}';
	delete from ${rmb_player} where Gold = 1;

	# 死亡充值玩家剩余元宝
	update ${rmb_player} r, ${g_usercenter_db}.${player_gold_login} d set r.Gold = d.Gold, r.LastLoginTime=d.LastLoginTime where r.PlayerID = d.PlayerID;
	select sum(Gold) as '${dead_date}死亡充值玩家剩余元宝' from ${rmb_player};
_EOFMYSQL_
}

# 查询未登陆玩家的剩余元宝
function query_dead_player_gold()
{
	local dead_date=$1" 00:00:00"

	local rmb_player=rmb_player_${today}
	local player_gold_login=player_gold_login_${today};

mysql $g_paydbopt -vvv $g_pay_db << _EOFMYSQL_
	# 死亡玩家剩余元宝
	select sum(Gold) as '${dead_date}未登陆玩家剩余元宝' from ${g_usercenter_db}.${player_gold_login} where LastLoginTime < '${dead_date}';
_EOFMYSQL_
}

function query_rmb_player()
{
	local pay_date_beg=$1" 00:00:00"
	local pay_date_end=$2" 00:00:00"

	local dead_date=$3" 00:00:00"

	local rmb_player=rmb_player_${today}
	local player_gold_login=player_gold_login_${today};

mysql $g_paydbopt -vvv $g_pay_db << _EOFMYSQL_
	# 充值人数
	drop table if exists $g_pay_db.${rmb_player};
	create table if not exists $g_pay_db.${rmb_player}(
		PlayerID int(11) unsigned not null comment '角色ID',
		Gold int(11) unsigned not null default 0 comment '元宝',
		LastLoginTime datetime not null default 0,
		primary key (PlayerID)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	insert ignore into ${rmb_player}(PlayerID) select distinct(PlayerID) from ${g_pay_db}.exchange where DateCreate >= '${pay_date_beg}' and DateCreate < '${pay_date_end}' and RMB > 0;
	insert ignore into ${rmb_player}(PlayerID) select distinct(playerid) from ${g_usercenter_db}.payed where createtime >= '${pay_date_beg}' and createtime < '${pay_date_end}' and rmb > 1;
	insert ignore into ${rmb_player}(PlayerID) select distinct(PlayerID) from ${g_pay_db}.myrecharge where CreateTime >= '${pay_date_beg}' and CreateTime < '${pay_date_end}';

	select count(*) as '从${pay_date_beg}到${pay_date_end}的充值人数' from ${g_pay_db}.${rmb_player};

	# 充值总额
	select sum(RMB)/100 into @a from ${g_pay_db}.exchange where DateCreate >= '${pay_date_beg}' and DateCreate < '${pay_date_end}' and RMB > 0;
	select sum(rmb) into @b from ${g_usercenter_db}.payed where createtime >= '${pay_date_beg}' and createtime < '${pay_date_end}' and rmb > 1;

	select @a, @b;

	set @sum_rmb = @a + @b;
	select (@a + @b) * 10 as '从${pay_date_beg}到${pay_date_end}的充值元宝数';

	# 死亡充值玩家
	truncate ${rmb_player};

	insert ignore into ${rmb_player}(PlayerID) select distinct(PlayerID) from ${g_pay_db}.exchange where DateCreate < '${dead_date}' and RMB > 0;
	insert ignore into ${rmb_player}(PlayerID) select distinct(playerid) from ${g_usercenter_db}.payed where createtime < '${dead_date}' and rmb > 1;

	select count(*) as '${dead_date}死亡充值玩家' from ${g_pay_db}.${rmb_player} r, ${g_usercenter_db}.globalplayers g where r.PlayerID = g.playerid and g.lastlogintime < '${dead_date}';

	update ${rmb_player} set Gold = 1;
	update ${rmb_player} r, ${g_usercenter_db}.globalplayers g set r.Gold = 0, r.LastLoginTime = g.lastlogintime where r.PlayerID = g.playerid and g.lastlogintime < '${dead_date}';
	delete from ${rmb_player} where Gold = 1;

	# 死亡充值玩家剩余元宝
	update ${rmb_player} r, ${g_usercenter_db}.${player_gold_login} d set r.Gold = d.Gold, r.LastLoginTime=d.LastLoginTime where r.PlayerID = d.PlayerID;
	select sum(Gold) as '${dead_date}死亡充值玩家剩余元宝' from ${rmb_player};
_EOFMYSQL_
}


function query_ios()
{
	ios_zone_list=zone_list/ios_zone_merge.list

	g_paydbopt="-h118.26.225.102 -P63301 -uroot -p${dbpwd}"
	g_pay_db=ios_app_pay
	g_usercenter_db=ios_app_usercenter

	ck_conn_mysql_by_mysql_list "$ios_zone_list"

	get_player_remain_gold "$ios_zone_list" "2014-04-01"

	query_dead_rmb_player_gold "2014-04-01"
	query_dead_player_gold "2014-04-01"
	#query_rmb_player "2013-11-01" "2014-01-01" "2014-02-01"
}

function query_and()
{
	and_zone_list=zone_list/and_zone.list

	g_paydbopt="-h42.62.60.197 -P63301 -uroot -p${dbpwd}"
	g_pay_db=and_app_pay
	g_usercenter_db=and_app_usercenter

	ck_conn_mysql_by_mysql_list "$and_zone_list"

	get_player_remain_gold "$and_zone_list" "2014-04-01"

	#query_rmb_player "2013-11-01" "2014-01-01" "2014-02-01"

	query_dead_rmb_player_gold "2014-04-01"
	query_dead_player_gold "2014-04-01"
}

# 开始进行配置了

# 2. 其次，获取数据库密码，通过手动输入的方式以防止密码泄露

	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	read -s dbpwd

	if [ "$dbpwd" = "" ]; then
		echo_err "err: you enter nothing, aborted!"
		exit
	fi

# 3. 再次，根据传入的各区数据库列表依次检测是否能成功连上各台服务器的数据库

	# 传入的服务器ip和port列表将如下面所示：
	# --------------------------------------------------------------------------------------------------------
	# ZoneID	    GameDb	        GameDbIP	GameDbPort	    LogDb	        LogDbIP	        LogDbPort
	#  1	    ios_app_s001	118.26.225.102	63316	    ios_app_s001_log	118.26.225.102	63307
	#  2	    ios_app_s002	118.26.225.102	63316	    ios_app_s002_log	118.26.225.102	63307
	#  3	    ios_app_s003	118.26.225.102	63316	    ios_app_s003_log	118.26.225.102	63307
	#  4	    ios_app_s004	118.26.225.102	63316	    ios_app_s004_log	118.26.225.102	63307
	#  5	    ios_app_s005	118.26.225.102	63316	    ios_app_s005_log	118.26.225.102	63307
	#  6	    ios_app_s006	118.26.225.102	63316	    ios_app_s006_log	118.26.225.102	63307
	#  .....
	# --------------------------------------------------------------------------------------------------------

	# 开始检测

# 3. 开始在不同平台的每个区执行sql文件
	query_ios
	#query_and