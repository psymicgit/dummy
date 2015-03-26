#  @File Name: .sh
#  @Author: 洪坤安
#  @Company: heihuo
#  @brief: 囧西游：导出元宝获赠和消耗表
#  @Created Time: 2014年3月13日 17:02:10

#!/bin/bash

source ./util.sh

declare err_cnt=0;
v=""

today=`date '+%Y_%m_%d'`

g_localhostdbopt="-uroot -p123456"

g_mysqldump_no_create_db_no_create_table="--no-create-db --no-create-info --add-locks --extended-insert --lock-tables --single-transaction --compress --complete-insert --quick --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit --insert-ignore"

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
function create_player_add_dec_gold()
{
	# 服务器列表文件
	local zone_list=$1
	local pay_date_beg=$2
	local pay_date_end=$3

	echo "**********************************start executing at $zone_list*******************************************"

	local rmb_player_dec_gold=rmb_player_dec_gold_${today};

mysql $g_paydbopt $g_pay_db << _EOFMYSQL_
		drop table if exists ${rmb_player_dec_gold};
		create table if not exists ${rmb_player_dec_gold}(
			CreateTime date NOT NULL,
			PlayerID int(11) unsigned not null comment '角色ID',
			AddGold int(11) not null comment '赠送元宝',
			DecGold int(11) not null comment '消耗元宝',
			primary key (CreateTime,PlayerID)
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

		echo_blue "-----------------start create ${rmb_player_dec_gold} at $gamedbdesc---------------"
		
mysql $gamedbopt $gamedb << _EOFMYSQL_
		drop table if exists ${rmb_player_dec_gold};
		create table if not exists ${rmb_player_dec_gold}(
			CreateTime date NOT NULL,
			PlayerID int(11) unsigned not null comment '角色ID',
			AddGold int(11) not null comment '赠送元宝',
			DecGold int(11) not null comment '消耗元宝',
			primary key (CreateTime,PlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

		insert ignore into ${rmb_player_dec_gold}(CreateTime, PlayerID, AddGold, DecGold) select c.CreateTime,c.PlayerID,c.AddGold,c.DecGold from player_consume c,players p where c.PlayerID = p.PlayerID and p.Pay > 0 and (c.AddGold > 0 or c.DecGold > 0) and c.CreateTime >= '${pay_date_beg}' and c.CreateTime < '${pay_date_end}';
_EOFMYSQL_

		mysqldump $gamedbopt $g_mysqldump_no_create_db_no_create_table $gamedb ${rmb_player_dec_gold} | mysql $g_paydbopt $g_pay_db

mysql $gamedbopt $gamedb << _EOFMYSQL_
		drop table ${rmb_player_dec_gold};
_EOFMYSQL_

		echo_blue "-----------------end ${rmb_player_dec_gold} at $gamedbdesc finished---------------"
	done < $zone_list

	echo_ok "**********************************executing at $zone_list finished*******************************************"
}

function create_rmb_player_add_dec_gold()
{
	local zone_list=$1
	local pay_date_beg=$2
	local pay_date_end=$3

	create_player_add_dec_gold ${zone_list} ${pay_date_beg} ${pay_date_end}

	local rmb_player_dec_gold=rmb_player_dec_gold_${today}
	local rmb_player=rmb_player_${today}

mysql $g_paydbopt -vvv $g_pay_db << _EOFMYSQL_
	delete from ${rmb_player_dec_gold} where PlayerID not in (select PlayerID from ${rmb_player});

	select sum(AddGold) as '从${pay_date_beg}到${pay_date_end}的充值玩家获得元宝', sum(DecGold) as '从${pay_date_beg}到${pay_date_end}的充值玩家消耗元宝' from ${rmb_player_dec_gold} where PlayerID in (select PlayerID from ${rmb_player});
_EOFMYSQL_
}

function export_add_dec_gold()
{
	local exportfile=$1
	local rmb_player_dec_gold=rmb_player_dec_gold_${today}

	rm -f ${exportfile}

mysql $g_localhostdbopt -vvv mysql << _EOFMYSQL_
	drop table if exists ${rmb_player_dec_gold};
_EOFMYSQL_

	mysqldump -vvv $g_paydbopt $g_mysqldump_create_table_no_create_db $g_pay_db ${rmb_player_dec_gold} | mysql $g_localhostdbopt mysql

mysql $g_localhostdbopt -vvv mysql << _EOFMYSQL_
	select CreateTime, PlayerID, AddGold, DecGold from ${rmb_player_dec_gold} order by CreateTime asc into outfile '/mnt/hgfs/proj/dummy/jxy/${exportfile}' fields terminated by '	' enclosed by '"' lines terminated by '\n';
_EOFMYSQL_

	sed '1 i"date"	"playerid"	"addgold"	"decgold"' -i ${exportfile}

	for (( i = 0; i < 2; i++ )); do
		echo "" >> ${exportfile}
	done

	echo "" >> ${exportfile}
	echo "《2014年起至${today}各充值玩家每天的获赠元宝数和消耗元宝数明细表》" >> ${exportfile}

mysql $g_localhostdbopt mysql << _EOFMYSQL_ >> ${exportfile}
	select count(*) as 记录数 from ${rmb_player_dec_gold};
_EOFMYSQL_
}

function query_ios()
{
	ios_zone_list=zone_list/ios_zone_merge_slave.list
	#ios_zone_list=zone_list/ios_zone_1.list

	g_paydbopt="-h118.26.225.102 -P63301 -uroot -p${dbpwd}"
	g_pay_db=ios_app_pay
	g_usercenter_db=ios_app_usercenter

	ck_conn_mysql_by_mysql_list "$ios_zone_list" &> log

	create_rmb_player_add_dec_gold "${ios_zone_list}" "2014-07-01" "2014-07-14" &> log
	export_add_dec_gold "ios_add_dec_gold_at_${today}.log" 2> log
}

function query_and()
{
	and_zone_list=zone_list/and_zone_merge_slave.list

	g_paydbopt="-h42.62.60.197 -P63301 -uroot -p${dbpwd}"
	g_pay_db=and_app_pay
	g_usercenter_db=and_app_usercenter

	ck_conn_mysql_by_mysql_list "$and_zone_list" &> log

	create_rmb_player_add_dec_gold "${and_zone_list}" "2014-07-01" "2014-07-14" &> log
	export_add_dec_gold "and_add_dec_gold_at_${today}.log" 2> log
}

# 开始进行配置了

# 2. 其次，获取数据库密码，通过手动输入的方式以防止密码泄露

	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	dbpwd=Maxnet.1234
	#read -s dbpwd

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

	query_ios | tee ios_add_dec_gold_${today}.log
	query_and | tee and_add_dec_gold_${today}.log