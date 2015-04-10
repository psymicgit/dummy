#  @File Name: jxy_each_zone_exec_sql.sh
#  @Author: 洪坤安
#  @Company: heihuo
#  @brief: 囧西游合区脚本
#  @Created Time: 2014年4月7日 17:02:10

#!/bin/bash

source ./util.sh

g_err_cnt=0

now=`date '+%Y_%m_%d_%H_%M_%S'`
today=`date '+%Y_%m_%d'`
hour_now=`date '+%Y_%m_%d_%H'`

g_backup_dir=""
g_check_merge_dir=""
g_merge_dir=""

g_merge_how_many_zone_to_1=8

# mysqldump的导出参数：含义分别是
# 不建库不建表，只导数据
g_mysqldump_no_create_db_no_create_table="--verbose --no-create-db --no-create-info --add-locks --extended-insert --lock-tables --single-transaction --compress --complete-insert --quick --default-character-set=utf8 --hex-blob --skip-comments --insert-ignore --no-autocommit"

# 不建库，只建表和导数据
g_mysqldump_create_table_no_create_db="--no-create-db --single-transaction --compress --complete-insert --opt --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

# 备份：建库、建表，导数据和存储过程、事件、触发器
g_mysqldump_backup="--add-drop-database --add-drop-table --add-drop-trigger --events --triggers --routines --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --hex-blob --skip-comments"

# 备份游戏库时所备份的表（不含players、faction、goldrecord表）
g_tables="factionplayer player_bags player_builds player_cdkey player_consume player_ext player_hero_ext player_heros player_instances player_pvp player_recharge player_relation player_tasks private_chat_log send_flower_log gvggift"

# 获取与游戏库的连接选项
function get_zone_conn_opt()
{
	local zone_idx=$1

	local gamedb_ip=${g_gamedb_ip_array[zone_idx]}
	local gamedb_sock=${g_gamedb_sock_array[zone_idx]}
	local zone_conn_opt=""

	if [ "$gamedb_sock" = "null" ]; then
		zone_conn_opt="-h$gamedb_ip -P63306 -uroot -p$dbpwd"
	else
		# dbpwd=123456
		zone_conn_opt="-S $gamedb_sock -uroot -p$dbpwd"
	fi

	echo "${zone_conn_opt}"
}

# 获取与配置库的连接选项
function get_paydb_conn_opt()
{
	local gamedb_ip=$1
	local gamedb_sock=$2

	local paydb_conn_opt=""

	if [ "$gamedb_sock" = "null" ]; then
		paydb_conn_opt="-h$gamedb_ip -P63301 -uroot -p$dbpwd"
	else
		# dbpwd=123456
		paydb_conn_opt="-S $gamedb_sock -uroot -p$dbpwd"
	fi

	echo "${paydb_conn_opt}"
}

function get_zone_list_from_file()
{
	local zone_list_file=$1

	g_zone_id_array=($(awk '{print $1}' $zone_list_file))
	g_zone_name_array=($(awk '{print $2}' $zone_list_file))
	g_gift_id_array=($(awk '{print $3}' $zone_list_file))
	g_gamedb_array=($(awk '{print $4}' $zone_list_file))
	g_gamedb_ip_array=($(awk '{print $5}' $zone_list_file))
	g_gamedb_sock_array=($(awk '{print $6}' $zone_list_file))

	local line_cnt=`wc -l $zone_list_file| awk '{print $1}'`
	g_zone_cnt=$(($line_cnt-1))
}

function get_pay_db_from_file()
{
	local pay_db_list=$1

	local pay_db_array=($(awk '{print $1}' $pay_db_list))
	local cfg_db_array=($(awk '{print $2}' $pay_db_list))	
	local usercenter_db_array=($(awk '{print $3}' $pay_db_list))
	local pay_db_ip_array=($(awk '{print $4}' $pay_db_list))
	local pay_db_sock_array=($(awk '{print $5}' $pay_db_list))

	pay_db_cnt=${#pay_db_array[@]}

	if [ $pay_db_cnt -ne 2 ]; then
		echo_err "error: $pay_db_list invalid, pay_db_cnt[$pay_db_cnt != 2] !"
		exit
	fi

	g_pay_db=${pay_db_array[1]}
	g_pay_db_name=${pay_db_array[0]}
	g_cfg_db=${cfg_db_array[1]}
	g_cfg_db_name=${cfg_db_array[0]}
	g_usercenter_db=${usercenter_db_array[1]}
	g_usercenter_db_name=${usercenter_db_array[0]}
	g_pay_db_ip=${pay_db_ip_array[1]}

	local pay_db_sock=${pay_db_sock_array[1]}
	g_pay_db_mysqlopt=`get_paydb_conn_opt $g_pay_db_ip $pay_db_sock`
}

# 清空日志中的颜色标记: [32m、[34m、[35m、[0m
function clean_log()
{
	local log_file="$1"

	sed -i 's/^..3.m//g' "${log_file}"
	sed -i 's/..0m$//g' "${log_file}"
}

# 依次连接合区列表中的每一个数据库，检测是否连接成功
function ck_db_by_list()
{
	local zone_list_file="$1"

	if [ ! -f $zone_list_file ]; then
		echo_err "error: can not found $zone_list file, exiting!"
		exit 1
	fi

	echo_blue "**********************************start connect to $zone_list_file**********************************************"

	local base_dir=${zone_list_file%%/*}
	local show_database_log=${base_dir}/show_database_${now}.log

	get_zone_list_from_file $zone_list_file
	for ((i=1;i<=$g_zone_cnt;i++))
	{
		local zone_name=${g_zone_name_array[i]}
		local gamedb=${g_gamedb_array[i]}
		local gamedb_ip=${g_gamedb_ip_array[i]}

		local zone_conn_opt=`get_zone_conn_opt $i`

		ck_conn_ok_by_opt_and_desc "$zone_name" "$zone_conn_opt"
		conn_mysql_succ=$?
		if [ "$conn_mysql_succ" -ne "0" ]; then
			exit 1
		fi

		mysql $zone_conn_opt -e "show databases;" > ${show_database_log}

		# 检测数据库已存在
		if ! find_db=$(grep -i -w "$gamedb" ${show_database_log}); then
			echo_err "error: could not found <$gamedb> at [$zone_name] @ <$gamedb_ip>, aborted!";
			exit 1;
		fi

		echo_ok "ok: detecting [$zone_name : $gamedb] @ <$gamedb_ip> existing."
		echo ""
	}
	echo_blue "**********************************connect to $zone_list_file finished*******************************************"

	if [ $((($g_zone_cnt)%$g_merge_how_many_zone_to_1)) -ne 0  ]; then
		echo_err "err: trying to merge $g_zone_cnt zone by $g_merge_how_many_zone_to_1, aborted!"
		exit
	fi

	rm -f ${show_database_log}
}

function ck_pay_db_by_list()
{
	local pay_db_list="$1"

	if [ ! -f $pay_db_list ]; then
		echo_err "error: can not found $pay_db_list file, exiting!"
		exit 1
	fi

	echo_blue "**********************************start connect to $pay_db_list**********************************************"
	get_pay_db_from_file $pay_db_list

	ck_conn_ok_by_opt_and_desc "${g_pay_db_name},${g_cfg_db_name},${g_usercenter_db_name}" "$g_pay_db_mysqlopt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit 1
	fi

	local base_dir=${pay_db_list%%/*}
	local show_database_log=${base_dir}/show_database_${now}.log

	mysql $g_pay_db_mysqlopt -e "show databases;" > ${show_database_log}

	# 检测pay数据库是否存在
	if ! find_db=$(grep -i -w "$g_pay_db" ${show_database_log}); then
		echo_err "error: could not found <$g_pay_db> at [${g_pay_db_name}] @ <$g_pay_db_ip>, aborted!";
		exit 1;
	fi

	echo_ok "ok: detecting [${g_pay_db_name} : $g_pay_db] @ <$g_pay_db_ip> existing."

	# 检测配置库是否存在
	if ! find_db=$(grep -i -w "$g_cfg_db" ${show_database_log}); then
		echo_err "error: could not found <$g_cfg_db> at [${g_cfg_db_name}] @ <$g_pay_db_ip>, aborted!";
		exit 1;
	fi

	echo_ok "ok: detecting [${g_usercenter_db_name} : $g_usercenter_db] @ <$g_pay_db_ip> existing."

	# 检测用户中心数据库是否存在
	if ! find_db=$(grep -i -w "$g_usercenter_db" ${show_database_log}); then
		echo_err "error: could not found <$g_usercenter_db> at [${g_usercenter_db_name}] @ <$g_pay_db_ip>, aborted!";
		exit 1;
	fi

	echo_ok "ok: detecting [${g_usercenter_db_name} : $g_usercenter_db] @ <$g_pay_db_ip> existing."
	echo ""	

	echo_blue "**********************************connect to $pay_db_list finished*******************************************"
	rm -f ${show_database_log}
}

# 把区名称连结起来，形如：[1区,2区,3区,4区][5区,6区,7区,8区]
function tie_zone_name()
{
	local start_zone_idx=$1
	local end_zone_idx=$2

	local zone_name_list=""
	local i=1
	for ((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		zone_name=${g_zone_name_array[i]}

		if [ $((i%$g_merge_how_many_zone_to_1)) -eq 1 ]; then
			zone_name_list="$zone_name_list""["
		fi

		if [ $((i%$g_merge_how_many_zone_to_1)) -eq 0 ]; then
			zone_name_list="$zone_name_list""$zone_name""]"
		else
			zone_name_list="$zone_name_list""$zone_name"","
		fi
	}

	echo "${zone_name_list}"
}

# 把区id连结起来
function tie_zone_id()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local joint="$3"
	
	local zone_id_list=""
	local i=1

	local start_zone_id=${g_zone_id_array[start_zone_idx]}
	local end_zone_id=${g_zone_id_array[end_zone_idx]}

	zone_id_list="$start_zone_id"${joint}"$end_zone_id"
	echo "${zone_id_list}"
}

function get_copy_players_table()
{
	local zone_idx=$1
	local zone_id=${g_zone_id_array[zone_idx]}
	local copy_players_table=at_${now}_zone_${zone_id}_players
	echo "${copy_players_table}"
}

function get_copy_faction_table()
{
	local zone_idx=$1
	local zone_id=${g_zone_id_array[zone_idx]}	
	local copy_faction_table=at_${now}_zone_${zone_id}_faction
	echo "${copy_faction_table}"
}

function get_tiny_gamedb()
{
	local zone_idx=$1
	local gamedb=${g_gamedb_array[zone_idx]}
	local tiny_gamedb=at_${now}_tiny_${gamedb}
	echo "${tiny_gamedb}"
}

# 拷贝玩家players的基本信息到另一个表
function copy_players_info()
{
	local zone_idx=$1

	local zone_name=${g_zone_name_array[zone_idx]}
	local gamedb=${g_gamedb_array[zone_idx]}

	local copy_players=`get_copy_players_table ${zone_idx}`
	echo_warn "$zone_name : copying players info"

	local zone_conn_opt=`get_zone_conn_opt $zone_idx`

mysql $zone_conn_opt $gamedb << _EOFMYSQL_
	drop table if exists ${copy_players};
	create table if not exists ${copy_players} select PlayerID,OriZoneID,UserName,DispName,Pay,VipLevel,Level,Gold,Coin,Science,Story,JingJie,BlueGas,PurpleGas,StudyExp,BagOpenNum,Firstattack,Power,MainHeroKindID,Rank,AllOnlineTime,TotalRMB,FlowerSum,LastLoginTime,LastLogoffTime,TodayFirstLoginTime,CreateTime,UpdateTime from players where PlayerID > 1010;
_EOFMYSQL_
}

# 拷贝玩家players表和门派faction表
function copy_players_faction()
{
	local zone_idx=$1

	local zone_name=${g_zone_name_array[zone_idx]}
	local gamedb=${g_gamedb_array[zone_idx]}

	local copy_players=`get_copy_players_table ${zone_idx}`
	local copy_faction=`get_copy_faction_table ${zone_idx}`
	
	echo_warn "[$zone_name]: copying players and faction"

	local zone_conn_opt=`get_zone_conn_opt $zone_idx`

mysql $zone_conn_opt $gamedb << _EOFMYSQL_
	drop table if exists ${copy_players};
	drop table if exists ${copy_faction};

	create table if not exists ${copy_players} select * from players where PlayerID > 1010;
	create table if not exists ${copy_faction} select * from faction;
_EOFMYSQL_
}

# 将指定区的玩家拷贝表移动到目标区所在的check_merge_2014_xx_xx数据库
function move_players_to_merge_check_db()
{
	# 从将玩家拷贝表从src区移动到dst区
	local dst_zone_idx=$1
	local src_zone_idx=$2
	local check_merge_db=$3		# 专门用来存储检测数据的数据库

	if [ $dst_zone_idx -gt $src_zone_idx ]; then
		echo_err "dst_zone_idx > src_zone_idx: $dst_zone_idx > $src_zone_idx"
		return
	fi

	local dst_zone_id=${g_zone_id_array[dst_zone_idx]}
	local dst_zone_name=${g_zone_name_array[dst_zone_idx]}
	local dst_gamedb=${g_gamedb_array[dst_zone_idx]}

	local src_zone_id=${g_zone_id_array[src_zone_idx]}
	local src_zone_name=${g_zone_name_array[src_zone_idx]}
	local src_gamedb=${g_gamedb_array[src_zone_idx]}

	local dst_zone_conn_opt=`get_zone_conn_opt $dst_zone_idx`
	local src_zone_conn_opt=`get_zone_conn_opt $src_zone_idx`

	local copy_players_table=`get_copy_players_table ${src_zone_idx}`

	echo_warn "[${src_zone_name} -> ${check_merge_db}] : moving players ..."
	mysqldump $src_zone_conn_opt $g_mysqldump_create_table_no_create_db $src_gamedb ${copy_players_table} | mysql $dst_zone_conn_opt $check_merge_db

	# 移动成功后把原来的玩家拷贝表删掉
mysql $src_zone_conn_opt $src_gamedb << _EOFMYSQL_
	drop table ${copy_players_table};
_EOFMYSQL_
}

# 将指定区的玩家拷贝表和门派拷贝表移动到目标区所在的merge_2014_xx_xx数据库
function move_players_faction_to_mergedb()
{
	# 从将玩家拷贝表从src区移动到dst区
	local dst_zone_idx=$1
	local src_zone_idx=$2
	local merge_db=$3		# 专门用来存储检测数据的数据库

	if [ $dst_zone_idx -gt $src_zone_idx ]; then
		echo_err "dst_zone_idx > src_zone_idx: $dst_zone_idx > $src_zone_idx"
		return
	fi

	local dst_zone_id=${g_zone_id_array[dst_zone_idx]}
	local dst_zone_name=${g_zone_name_array[dst_zone_idx]}
	local dst_gamedb=${g_gamedb_array[dst_zone_idx]}

	local src_zone_id=${g_zone_id_array[src_zone_idx]}
	local src_zone_name=${g_zone_name_array[src_zone_idx]}
	local src_gamedb=${g_gamedb_array[src_zone_idx]}

	local dst_zone_conn_opt=`get_zone_conn_opt $dst_zone_idx`
	local src_zone_conn_opt=`get_zone_conn_opt $src_zone_idx`

	local copy_players_table=`get_copy_players_table ${src_zone_idx}`
	local copy_faction_table=`get_copy_faction_table ${src_zone_idx}`

	echo_warn "[${src_zone_name} -> ${merge_db}] : moving players and faction ..."

	mysqldump $src_zone_conn_opt $g_mysqldump_create_table_no_create_db $src_gamedb $copy_players_table $copy_faction_table | mysql $dst_zone_conn_opt $merge_db

	# 为玩家和门派拷贝表的id和名称字段加上索引
mysql $dst_zone_conn_opt $merge_db << _EOFMYSQL_
	create index player_id_idx on ${copy_players_table}(PlayerID);
	create index faction_id_idx on ${copy_faction_table}(FactionID);

	create index player_name_idx on ${copy_players_table}(DispName);
	create index faction_name_idx on ${copy_faction_table}(FactionName);
_EOFMYSQL_

	# 移动成功后把原来的玩家拷贝表和门派拷贝表删掉
mysql $src_zone_conn_opt $src_gamedb << _EOFMYSQL_
	drop table ${copy_players_table};
	drop table ${copy_faction_table};
_EOFMYSQL_
}

#################start:合区前检测#################


# 将指定区的玩家ID和门派id各自收集起来存入表中，用于后续的各区id重复检测
function collect_playerid_factionid_by_zone()
{
	local zone_idx=$1

	local zone_id=${g_zone_id_array[zone_idx]}
	local zone_name=${g_zone_name_array[zone_idx]}
	local gamedb=${g_gamedb_array[zone_idx]}

	local collect_players_id=at_${now}_collect_players_id_${zone_id}
	local collect_faction_id=at_${now}_collect_faction_id_${zone_id}

	local zone_conn_opt=`get_zone_conn_opt $zone_idx`

	echo_warn "[$zone_name] : collecting playerid and factionid ..."

mysql $zone_conn_opt $gamedb << _EOFMYSQL_
	drop table if exists ${collect_players_id};
	drop table if exists ${collect_faction_id};

	create table if not exists ${collect_players_id} (
		PlayerID int(11) unsigned not null comment '角色ID',
		primary key (PlayerID)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	create table if not exists ${collect_faction_id} (
		FactionID int(11) unsigned not null comment '门派ID',
		primary key (FactionID)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	insert into ${collect_players_id}(PlayerID) select PlayerID from players;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_bags;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_builds;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_cdkey;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_consume;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_ext;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_heros;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_hero_ext;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_instances;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_pvp;
	# insert ignore into ${collect_players_id} select distinct PlayerID from player_tasks;
	# insert ignore into ${collect_players_id} select distinct JoinPlayerID from factionplayer;
	# insert ignore into ${collect_players_id} select distinct FactionPlayerID from faction;

	insert into ${collect_faction_id}(FactionID) select FactionID from faction;
	# insert ignore into ${collect_faction_id} select distinct FactionID from factionplayer;
_EOFMYSQL_
}

# 将指定区中收集的玩家id表和门派id表移动到目标区所在的check_merge_年_月_日数据库
function move_collect_playerid_factionid()
{
	local dst_zone_idx=$1
	local src_zone_idx=$2
	local check_merge_db=$3		# 专门用来存储检测数据的数据库

	if [ $dst_zone_idx -gt $src_zone_idx ]; then
		echo_err "dst_zone_idx > src_zone_idx: $dst_zone_idx > $src_zone_idx"
		return
	fi

	local dst_zone_id=${g_zone_id_array[dst_zone_idx]}
	local dst_zone_name=${g_zone_name_array[dst_zone_idx]}
	local dst_gamedb=${g_gamedb_array[dst_zone_idx]}

	local src_zone_id=${g_zone_id_array[src_zone_idx]}
	local src_zone_name=${g_zone_name_array[src_zone_idx]}
	local src_gamedb=${g_gamedb_array[src_zone_idx]}

	local dst_zone_conn_opt=`get_zone_conn_opt $dst_zone_idx`
	local src_zone_conn_opt=`get_zone_conn_opt $src_zone_idx`

	local collect_players_id=at_${now}_collect_players_id_${src_zone_id}
	local collect_faction_id=at_${now}_collect_faction_id_${src_zone_id}

	echo_warn "[$src_zone_name -> $check_merge_db] : moving collect playersid and factionid ..."
	mysqldump $src_zone_conn_opt $g_mysqldump_create_table_no_create_db $src_gamedb ${collect_players_id} ${collect_faction_id} | mysql $dst_zone_conn_opt $check_merge_db

mysql $src_zone_conn_opt $src_gamedb << _EOFMYSQL_
	drop table if exists ${collect_players_id};
	drop table if exists ${collect_faction_id};
_EOFMYSQL_
}

# 在数据库中检测玩家id是否发生冲突，并将冲突结果存入一个表
function check_conflict_playerid_factionid()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local check_merge_db=$3
	local check_merge_err_file="$4"

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	local all_players_id=at_${now}_all_players_id_${zone_id_list}
	local all_faction_id=at_${now}_all_faction_id_${zone_id_list}

	local conflict_players=at_${now}_conflict_players_${zone_id_list}
	local conflict_players_id=at_${now}_conflict_players_id_${zone_id_list}
	local conflict_faction_id=at_${now}_conflict_faction_id_${zone_id_list}

	echo_warn "${zone_name_list} : checking conflict players and faction ..."

	local dst_zone_conn_opt=`get_zone_conn_opt $start_zone_idx`

mysql $dst_zone_conn_opt $check_merge_db << _EOFMYSQL_
	drop table if exists ${all_players_id};
	drop table if exists ${all_faction_id};
	drop table if exists ${conflict_players_id};
	drop table if exists ${conflict_faction_id};
	drop table if exists ${conflict_players};
_EOFMYSQL_

	echo_warn "${zone_name_list} : making conflict players and faction table ..."
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		local zone_id=${g_zone_id_array[i]}

		local collect_players_id=at_${now}_collect_players_id_${zone_id}
		local collect_faction_id=at_${now}_collect_faction_id_${zone_id}

		local copy_players_table=`get_copy_players_table ${i}`
		local copy_faction_table=`get_copy_faction_table ${i}`

mysql $dst_zone_conn_opt $check_merge_db << _EOFMYSQL_
		update ${copy_players_table} set OriZoneID = ${zone_id};

		create table if not exists ${conflict_players} as select * from ${copy_players_table} where 0;
		insert into ${conflict_players} select * from ${copy_players_table};

		create table if not exists ${all_players_id} as select * from ${collect_players_id} where 0;
		insert into ${all_players_id} select * from ${collect_players_id};

		create table if not exists ${all_faction_id} as select * from ${collect_faction_id} where 0;
		insert into ${all_faction_id} select * from ${collect_faction_id};

		drop table ${collect_players_id};
		drop table ${collect_faction_id};
		drop table ${copy_players_table};
_EOFMYSQL_
	}

	echo_warn "${zone_name_list} : erasing not conflict players and faction ..."

# 删除不重复的玩家，仅保留冲突玩家
mysql $dst_zone_conn_opt $check_merge_db << _EOFMYSQL_
	create table if not exists ${conflict_players_id} as select PlayerID from ${all_players_id} group by PlayerID having count(PlayerID) > 1;
	create table if not exists ${conflict_faction_id} as select FactionID from ${all_faction_id} group by FactionID having count(FactionID) > 1;	

	create index player_id_idx on ${conflict_players}(PlayerID);
	create index player_id_idx on ${conflict_players_id}(PlayerID);

	update ${conflict_players} set Firstattack = 0;
	update ${conflict_players} c inner join ${conflict_players_id} d on c.PlayerID = d.PlayerID set c.Firstattack = 1;
	delete from ${conflict_players} where Firstattack = 0;
	
	drop table if exists ${all_players_id};
	drop table if exists ${all_faction_id};
	drop table if exists ${conflict_players_id};
_EOFMYSQL_

# 输出冲突玩家和门派列表到文件中
mysql $dst_zone_conn_opt $check_merge_db << _EOFMYSQL_ >> ${check_merge_err_file}
	select PlayerID,DispName,OriZoneID,Level from ${conflict_players} order by PlayerID asc limit 100;
	select FactionID from ${conflict_faction_id};
_EOFMYSQL_

	echo_ok "${zone_name_list} : checking conflict players and faction finished"
}

# 给指定区建立一个微型的拷贝（对该区的每个表都只导出前1000条数据）
function copy_tiny_zone()
{
	local zone_idx=$1

	local zone_conn_opt=`get_zone_conn_opt $zone_idx`

	local zone_name=${g_zone_name_array[zone_idx]}
	local gamedb=${g_gamedb_array[zone_idx]}
	local tiny_gamedb=`get_tiny_gamedb $zone_idx`

	echo_warn "[${zone_name}] : creating tiny ${gamedb} ..."

mysql $zone_conn_opt << _EOFMYSQL_
	create database if not exists ${tiny_gamedb};
_EOFMYSQL_

	mysqldump $zone_conn_opt $g_mysqldump_create_table_no_create_db $gamedb $g_tables players faction goldrecord_2014 --where " true limit 500" --ignore-table=${gamedb}.battlelog | mysql $zone_conn_opt ${tiny_gamedb}

	echo_ok "[${zone_name}] : create tiny ${gamedb} ok"
}

function drop_tiny_zone()
{
	local zone_idx=$1

	local zone_conn_opt=`get_zone_conn_opt $zone_idx`
	local zone_name=${g_zone_name_array[zone_idx]}
	local gamedb=${g_gamedb_array[zone_idx]}

	if [ "${gamedb:0:22}" = "at_${now}" ]; then
		echo_warn "[${zone_name}] : droping database ${gamedb} ..."

mysql $zone_conn_opt << _EOFMYSQL_
	drop database if exists ${gamedb};
_EOFMYSQL_

		echo_ok "[${zone_name}] : drop database ${gamedb} ok ..."
	else 
		echo_err "drop database ${gamedb} failed, [${gamedb:0:22}] not start with [at_${now}] ..."
	fi
}

# 给用户中心库、配置库和pay库各建立一个微型的拷贝，该拷贝仅包含几个需要用到的表
function copy_tiny_paydb()
{
	local tiny_pay_db=at_${now}_tiny_${g_pay_db}
	local tiny_cfg_db=at_${now}_tiny_${g_cfg_db}
	local tiny_usercenter_db=at_${now}_tiny_${g_usercenter_db}

mysql $g_pay_db_mysqlopt << _EOFMYSQL_
	create database if not exists ${tiny_pay_db};
	create database if not exists ${tiny_cfg_db};
	create database if not exists ${tiny_usercenter_db};
_EOFMYSQL_

	echo_warn "[${g_pay_db_name}] : creating tiny ${g_pay_db} ..."
	mysqldump $g_pay_db_mysqlopt $g_mysqldump_create_table_no_create_db $g_pay_db activityencourageplayer activityencouragepro | mysql $g_pay_db_mysqlopt ${tiny_pay_db}

	echo_warn "[${g_cfg_db_name}] : creating tiny ${g_cfg_db} ..."
	mysqldump $g_pay_db_mysqlopt $g_mysqldump_create_table_no_create_db $g_cfg_db closedarea | mysql $g_pay_db_mysqlopt ${tiny_cfg_db}

	echo_warn "[${g_usercenter_db_name}] : creating tiny ${g_usercenter_db} ..."
	mysqldump $g_pay_db_mysqlopt $g_mysqldump_create_table_no_create_db $g_usercenter_db gamezones | mysql $g_pay_db_mysqlopt ${tiny_usercenter_db}

	echo_ok "[${g_pay_db_name},${g_cfg_db_name},${g_usercenter_db_name}] : creating tiny ${g_pay_db},${g_cfg_db},${g_usercenter_db} finished"
}

function drop_tiny_paydb()
{
	if [ "${g_pay_db:0:22}" = "at_${now}" ]; then

		echo_warn "[${g_pay_db_name},${g_cfg_db_name},${g_usercenter_db_name}] : droping ${g_pay_db},${g_cfg_db},${g_usercenter_db} ..."

mysql $g_pay_db_mysqlopt << _EOFMYSQL_
		create database if not exists ${g_pay_db};
		create database if not exists ${g_cfg_db};
		create database if not exists ${g_usercenter_db};
_EOFMYSQL_

		echo_ok "[${g_pay_db_name},${g_cfg_db_name},${g_usercenter_db_name}] : drop ${g_pay_db},${g_cfg_db},${g_usercenter_db} ok"
	else 
		echo_err "drop database ${g_pay_db} failed, [${g_pay_db:0:22}] not start with [at_${now}] ..."
	fi
}

function rename_tiny_zone()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	
	local i=1
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		local tiny_gamedb=`get_tiny_gamedb $i`
		g_gamedb_array[i]=${tiny_gamedb}
	}
}

function reinit_env()
{
	local try_merge_dir="$1"

	g_pay_db=at_${now}_tiny_${g_pay_db}
	g_cfg_db=at_${now}_tiny_${g_cfg_db}
	g_usercenter_db=at_${now}_tiny_${g_usercenter_db}

	g_merge_dir="${try_merge_dir}"
}

function try_merge_n_to_1()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local try_merge_dir=$3

	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`	
	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`

	echo_warn "**********************${zone_name_list} : start try merge***********************"

	local i=1
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		copy_tiny_zone $i &
	}

	wait
	rename_tiny_zone $start_zone_idx $end_zone_idx

	local try_merge_db=try_merge_${today}
	merge_n_to_1 $start_zone_idx $end_zone_idx ${try_merge_db}

	for((i=$(($start_zone_idx+1));i<=$end_zone_idx;i++))
	{
		drop_tiny_zone $i &
	}

	wait
	echo_warn "**********************${zone_name_list} : try merge finished***********************"
}

function try_merge()
{
	local zone_name_list=`tie_zone_name 1 $g_zone_cnt`

	local try_merge_dir=${g_check_merge_dir}/try_merge
	if [ ! -d "$try_merge_dir" ]; then
		mkdir "$try_merge_dir"
	fi

	echo_blue "**********************************${zone_name_list} : start try merge**********************************************"

	copy_tiny_paydb "${try_merge_dir}"
	reinit_env "${try_merge_dir}"

	jxy_try_merge_log=${try_merge_dir}/jxy_try_merge_${all_zone_id_list}_at_${now}.log

	local i=0
	for((i=1;i<$g_zone_cnt;i=$(($i+$g_merge_how_many_zone_to_1)) ))
	{
		try_merge_n_to_1 $i $(($i+$g_merge_how_many_zone_to_1-1)) "${try_merge_dir}" | tee ${jxy_try_merge_log}
	}

	# drop_tiny_paydb

	echo_blue "**********************************${zone_name_list} : try merge finished**********************************************"
}

function check_conflicts()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local check_merge_err_file="$3"

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	echo_warn "**********************${zone_name_list} : start check conflict id***********************"
	local dst_gamedb=${g_gamedb_array[start_zone_idx]}
	local dst_zone_conn_opt=`get_zone_conn_opt $start_zone_idx`

	local check_merge_db=merge_check_${today}

	# 给各个区的玩家表制造一份拷贝，并将该区中以玩家id做主键的表中的玩家ID集中插入一个表
	local i=1
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		copy_players_info $i
		collect_playerid_factionid_by_zone $i
	}

mysql $dst_zone_conn_opt << _EOFMYSQL_
	create database if not exists ${check_merge_db};
_EOFMYSQL_

	# 将各个区的玩家拷贝表、玩家id收集表和门派id收集表 集中移动到第一个区所在的check_merge_2014_xx_xx数据库
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		move_players_to_merge_check_db $start_zone_idx $i $check_merge_db
		move_collect_playerid_factionid $start_zone_idx $i $check_merge_db
	}

	# 检测出冲突玩家
	check_conflict_playerid_factionid $start_zone_idx $end_zone_idx $check_merge_db "${check_merge_err_file}"
	echo_ok "**********************${zone_name_list} : check conflict id finished********************"	
}

function check_merge_n_to_1()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local check_merge_err_file="$3"

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	echo_warn "**********************${zone_name_list} : start check merge***********************"
	check_conflicts $start_zone_idx $end_zone_idx "${check_merge_err_file}"
	echo_ok "**********************${zone_name_list} : check merge finished********************"
}

function check_merge()
{
	local zone_name_list=`tie_zone_name 1 $g_zone_cnt`
	local zone_id_list=`tie_zone_id 1 $g_zone_cnt "_"`

	echo_blue "**********************************${zone_name_list} : start check merge**********************************************"

	g_check_merge_dir=${base_dir}/check_merge_${zone_id_list}_at_${now}

	if [ ! -d "$g_check_merge_dir" ]; then
		mkdir "$g_check_merge_dir"
	fi

	local check_merge_err_file=${g_check_merge_dir}/check_merge_err_${zone_id_list}.err

	local i=0
	for((i=1;i<$g_zone_cnt;i=$(($i+$g_merge_how_many_zone_to_1)) ))
	{
		check_merge_n_to_1 $i $(($i+$g_merge_how_many_zone_to_1-1)) "${check_merge_err_file}"
	}

	echo_blue "**********************************${zone_name_list} : check merge finished*******************************************"

	# 根据错误日志文件是否包含PlayerID和FactionID关键字来判断是否存在冲突玩家id或者门派id
	if find_err=$(grep -i -w "PlayerID" ${check_merge_err_file}); then 
		echo_err "error: find conflict player id in ${zone_name_list}, err_file = ${check_merge_err_file}, aborted!"
		exit 1;
	fi

	if find_err=$(grep -i -w "FactionID" ${check_merge_err_file}); then 
		echo_err "error: find conflict faction id in ${zone_name_list}, err_file = ${check_merge_err_file}, aborted!"
		exit 1;
	fi

	try_merge
}

#################end:合区前检测#################

#################start:合区#################

# 修正各区的玩家昵称和门派名称，使合区后不会重复
# 玩家昵称和门派名称重复时的处理方式为：
#	玩家昵称 = 玩家昵称#区号
#	门派名称 = 门派名称#区号
function fix_conflict_name()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local merge_db=$3		# 专门创建用来合区的数据库
	local merge_n_to_1_dir="$4"

	local dst_zone_conn_opt=`get_zone_conn_opt $start_zone_idx`

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	local all_players_name=at_${now}_all_players_name_${zone_id_list}
	local all_faction_name=at_${now}_all_faction_name_${zone_id_list}

	local conflict_players_name=at_${now}_conflict_players_name_${zone_id_list}
	local conflict_faction_name=at_${now}_conflict_faction_name_${zone_id_list}

	local fix_player_name_log=${merge_n_to_1_dir}/fix_player_name.log
	local fix_faction_name_log=${merge_n_to_1_dir}/fix_faction_name.log

	echo_warn "$zone_name_list : fixing conflict players name and faction name at [${merge_db}] ..."

mysql $dst_zone_conn_opt $merge_db << _EOFMYSQL_
	drop table if exists ${all_players_name};
	drop table if exists ${all_faction_name};

	create table ${all_players_name}(
		DispName varchar(64) binary NOT NULL DEFAULT '' COMMENT '玩家昵称'
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	create table ${all_faction_name}(
		FactionName varchar(64) binary NOT NULL DEFAULT '' COMMENT '门派名称'
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
_EOFMYSQL_
	
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		local copy_players_table=`get_copy_players_table ${i}`
		local copy_faction_table=`get_copy_faction_table ${i}`

mysql $dst_zone_conn_opt $merge_db << _EOFMYSQL_
		insert into ${all_players_name} select distinct(DispName) from ${copy_players_table} where DispName <> '' and PlayerID > 1010;
		insert into ${all_faction_name} select FactionName from ${copy_faction_table};
_EOFMYSQL_

	
	}

# 创建冲突玩家昵称表和冲突门派名称表
mysql $dst_zone_conn_opt $merge_db << _EOFMYSQL_
		drop table if exists ${conflict_players_name};
		drop table if exists ${conflict_faction_name};

		create table if not exists ${conflict_players_name} as select DispName	  from ${all_players_name} group by DispName having count(DispName) > 1;
		create table if not exists ${conflict_faction_name} as select FactionName from ${all_faction_name} group by FactionName having count(FactionName) > 1;

		create index player_name_idx on ${conflict_players_name}(DispName);
		create index faction_name_idx on ${conflict_faction_name}(FactionName);
_EOFMYSQL_

	# 开始修正玩家表和门派表：修正冲突玩家昵称和冲突门派名称并设置好改名标志
	for((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		local zone_id=${g_zone_id_array[i]}
		local zone_name=${g_zone_name_array[i]}
		local copy_players_table=`get_copy_players_table ${i}`
		local copy_faction_table=`get_copy_faction_table ${i}`

mysql $dst_zone_conn_opt -v $merge_db << _EOFMYSQL_
		update ${copy_players_table} p inner join ${conflict_players_name} c on p.DispName = c.DispName and p.DispName <> '' and binary(p.DispName) = binary(c.DispName) set p.DispName = CONCAT(p.DispName, '#${zone_id}'), p.ExtData = 3;
		update ${copy_faction_table} f inner join ${conflict_faction_name} c on f.FactionName = c.FactionName and binary(f.FactionName) = binary(c.FactionName) set f.FactionName = CONCAT(f.FactionName, '#${zone_id}'), f.Ext = 1;

		select count(*) as '${zone_name}冲突玩家数' from ${copy_players_table} where ExtData = 3;
		select count(*) as '${zone_name}冲突门派数' from ${copy_faction_table} where Ext = 1;
_EOFMYSQL_



mysql $dst_zone_conn_opt -vvv $merge_db << _EOFMYSQL_ >> ${fix_player_name_log}
		select count(*) as '${zone_name}冲突玩家数' from ${copy_players_table} where ExtData = 3;
		set @rowid=0;
		select @rowid:=@rowid+1 as '行数',PlayerID as '角色ID',DispName as '玩家昵称',VipLevel as 'VIP等级',Level as '角色等级' from ${copy_players_table} where ExtData = 3;
_EOFMYSQL_
mysql $dst_zone_conn_opt -vvv $merge_db << _EOFMYSQL_ >> ${fix_faction_name_log}
		select count(*) as '${zone_name}冲突门派数' from ${copy_faction_table} where Ext = 1;
		set @rowid=0;
		select @rowid:=@rowid+1 as '行数',FactionID as '门派ID',FactionName as '门派名称' from ${copy_faction_table} where Ext = 1;
_EOFMYSQL_
	}

# 删除玩家昵称表和门派名称表
mysql $dst_zone_conn_opt $merge_db << _EOFMYSQL_
		drop table if exists ${all_players_name};
		drop table if exists ${all_faction_name};
_EOFMYSQL_

	echo_ok "$zone_name_list : fix conflict players name and faction name at [${merge_db}] finished"
}

# 将指定区移动到目标区
function move_zone()
{
	local dst_zone_idx=$1
	local src_zone_idx=$2
	local merge_db=$3
	local merge_n_to_1_dir="$4"

	if [ $dst_zone_idx -gt $src_zone_idx ]; then
		echo_err "dst_zone_idx > src_zone_idx: $dst_zone_idx > $src_zone_idx"
		return
	fi

	local dst_zone_id=${g_zone_id_array[dst_zone_idx]}
	local dst_zone_name=${g_zone_name_array[dst_zone_idx]}
	local dst_gamedb=${g_gamedb_array[dst_zone_idx]}

	local src_zone_id=${g_zone_id_array[src_zone_idx]}
	local src_zone_name=${g_zone_name_array[src_zone_idx]}
	local src_gamedb=${g_gamedb_array[src_zone_idx]}

	local dst_zone_conn_opt=`get_zone_conn_opt $dst_zone_idx`
	local src_zone_conn_opt=`get_zone_conn_opt $src_zone_idx`

	local src_zone_sql=${src_gamedb}.sql
	local src_goldrecord_2014_sql=${src_gamedb}.goldrecord_2014.sql

	echo_warn "[$src_zone_name -> $dst_zone_name] : start moving..."
	echo_warn "[$src_zone_name -> $dst_zone_name] : moving [$src_zone_name : $src_gamedb] -> [$dst_zone_name : $dst_gamedb] ..."

	# 导出各个游戏表以及元宝日志表（不含players和factions表）（不导出表结构）
	time mysqldump $src_zone_conn_opt ${g_mysqldump_no_create_db_no_create_table} $src_gamedb $g_tables | mysql $dst_zone_conn_opt $dst_gamedb

	time mysqldump $src_zone_conn_opt ${g_mysqldump_no_create_db_no_create_table} $src_gamedb goldrecord_2014 > ${merge_n_to_1_dir}/${src_goldrecord_2014_sql}

	# 将导出的元宝日志文件中（文件名如:ios_app_s043.goldrecord_2014.sql）每一个insert语句中的<(日志id,> 替换成 <(NULL,>（日志id是自动增长字段，用NULL以使导入元宝日志表时日志id继续自增）
	echo_warn "[$src_zone_name]: replacing '(id,' --> '(NULL,' in ${src_goldrecord_2014_sql}"
	sed -i 's/([0-9][0-9]*,/(NULL,/g' "${merge_n_to_1_dir}/${src_goldrecord_2014_sql}"

	# 开始导入到目标区
	echo_warn "[$src_zone_name -> $dst_zone_name] : importing [${src_goldrecord_2014_sql}] into ${dst_zone_name} ..."
	time mysql $dst_zone_conn_opt $dst_gamedb < ${merge_n_to_1_dir}/${src_goldrecord_2014_sql}

	# 从merge库中取出players和factions表移动到该区
	import_player_faction_from_mergedb ${dst_zone_idx} ${src_zone_idx} ${merge_db} "${merge_n_to_1_dir}"

	#（压缩成.sql.gz格式）
	echo_warn "[$src_zone_name -> $dst_zone_name] : gzip [${src_zone_sql}],[${src_goldrecord_2014_sql}] ..."
	time gzip "${merge_n_to_1_dir}/${src_goldrecord_2014_sql}" &

	echo_ok "[$src_zone_name -> $dst_zone_name] : move finished ..."
}

# 制造合区礼包
# 合区奖励 =（1000-开服时间）元宝+（1000-开服时间）*100铜钱
function make_merge_gift()
{
	local start_zone_idx=$1
	local end_zone_idx=$2

	local merge_n_to_1_dir="$3"

	local start_zone_id=${g_zone_id_array[start_zone_idx]}
	local start_gift_id=${g_gift_id_array[start_zone_idx]}

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	local merge_gift_players=merge_gift_players_${zone_id_list}

	local gift_log=${merge_n_to_1_dir}/merge_gift_${zone_id_list}.log
	local gift_sql=${merge_n_to_1_dir}/merge_gift_${zone_id_list}.sql

	echo_warn "$zone_name_list: making merge gift ..."
	local n=$g_merge_how_many_zone_to_1

	local dot_zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx ","`

# 先到用户中心根据各区开服时间表计算出各区对应的元宝和铜钱奖励
mysql $g_pay_db_mysqlopt $g_usercenter_db << _EOFMYSQL_ > $gift_log
	select zoneid - (zoneid - 1) mod $n as '区号', ${start_gift_id} as '礼包id', 1000 - TO_DAYS(NOW()) + TO_DAYS(starttime) as '元宝', (1000 - TO_DAYS(NOW()) + TO_DAYS(starttime)) * 100 as '铜钱' from gamezones where zoneid = ${start_zone_id};
_EOFMYSQL_

	local gift_zone_id_array=($(awk '{print $1}' $gift_log))
	local gift_id_array=($(awk '{print $2}' $gift_log))
	local gift_gold_array=($(awk '{print $3}' $gift_log))
	local gift_coin_array=($(awk '{print $4}' $gift_log))

	local gift_cnt=${#gift_id_array[@]}
	gift_cnt=$(($gift_cnt-1))

	echo $gift_cnt

	echo "" > $gift_sql

	local i=1
	for ((i=1;i<=$gift_cnt;i++))
	{
		local gift_zone_id=${gift_zone_id_array[i]}
		local gift_id=${gift_id_array[i]}
		local gift_gold=${gift_gold_array[i]}
		local gift_coin=${gift_coin_array[i]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $gift_sql
replace into activityencouragepro(ZoneID,ActivityID,ActivityType,ActivityName,ActivityDesc,JsonValues,IsNeedCdKey,StartTime,EndTime) 
	values (${gift_zone_id}, ${gift_id}, 1, 
	"合区礼包",	"欢迎您来到合区之后的大家庭，小囧在此特别赠送合区礼包，里面有${gift_gold}元宝和${gift_coin}铜钱，并附送20点体力，祝您游戏愉快",
	'{
		"item": [{"value": 0,"num": 1,"percent": 100}],
		"hero": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"coin": {
			"value": ${gift_coin},
			"num": 1,
			"percent": 100
		},
		"gold": {
			"value": ${gift_gold},
			"num": 1,
			"percent": 100
		},
		"phystrength": {
			"value": 20,
			"num": 1,
			"percent": 100
		},
		"science": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"story": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"bluegas": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"pruplegas": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"jingjie": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"doorstribute": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"factionfunds": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"factiontaskintegral": {
			"value": 0,
			"num": 1,
			"percent": 100
		}
	}',
	0, "0000-00-00 00:00:00", "2015-01-01 00:00:00");
_EOF_
#输出文件结束------------------------------------------------------------
	}

	# 插入合区礼包表
	mysql $g_pay_db_mysqlopt $g_pay_db < $gift_sql
	echo_ok "$zone_name_list: make merge gift ok"
}

# 给各区玩家都分配一个合区礼包
function give_merge_gift()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local merge_db=$3
	local merge_n_to_1_dir="$4"

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	local merge_gift_players_table=at_${now}_merge_gift_players_${zone_id_list}

	local dst_zone_id=${g_zone_id_array[start_zone_idx]}
	local dst_gamedb=${g_gamedb_array[start_zone_idx]}
	local dst_gift_id=${g_gift_id_array[start_zone_idx]}
	local dst_zone_conn_opt=`get_zone_conn_opt $start_zone_idx`

	echo_warn "$zone_name_list: giving merge gift to players ..."
	
	# 创建合区礼包分配表
mysql $dst_zone_conn_opt $merge_db << _EOFMYSQL_
	drop table if exists ${merge_gift_players_table};
	
	# 创建合区奖励表，为各区的玩家都添加一份合区礼包
	create table if not exists ${merge_gift_players_table} (
		ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '活动ID',
		StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		PRIMARY KEY (ActivityID,StartPlayerID)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
_EOFMYSQL_

	local copy_players_table=`get_copy_players_table ${start_zone_idx}`

	# 给合区之后处于一个大区的玩家分配同一个合区礼包
mysql $dst_zone_conn_opt -v $merge_db << _EOFMYSQL_
		insert into ${merge_gift_players_table} select ${dst_gift_id}, 0, MAX(PlayerID) from ${copy_players_table} where PlayerID > 1010;
		select 1 as "成功给予${zone_name_list}的玩家${dst_gift_id}礼包";
_EOFMYSQL_

	# 将合区礼包分配表合并到pay库中的活动奖励表
	echo_warn "[$merge_db -> $g_pay_db] : moving [${merge_gift_players_table}] ..."
	time mysqldump $dst_zone_conn_opt $g_mysqldump_create_table_no_create_db $merge_db ${merge_gift_players_table} | mysql $g_pay_db_mysqlopt $g_pay_db

mysql $g_pay_db_mysqlopt -v $g_pay_db << _EOFMYSQL_
	select count(*) as 分配礼包前activityencourageplayer表的记录数 from activityencourageplayer;
	replace into activityencourageplayer(ActivityID,StartPlayerID,EndPlayerID) select ActivityID,StartPlayerID,EndPlayerID from ${merge_gift_players_table};
	select count(*) as 分配礼包后activityencourageplayer表的记录数 from activityencourageplayer;
	drop table ${merge_gift_players_table};
_EOFMYSQL_

	echo_ok "$zone_name_list : give merge gift to players ok"
}

# 强化合区后boss战活动中的boss，免得被玩家虐
function set_boss_more_powerful
{
	local zone_idx=$1

	local zone_id=${g_zone_id_array[zone_idx]}
	local zone_name=${g_zone_name_array[zone_idx]}

	echo_warn "[$zone_name] : setting the boss of boss activity more powerful  ..."
mysql $g_pay_db_mysqlopt -v $g_cfg_db << _EOFMYSQL_
	select count(*) as 强化boss前closedarea表的记录数 from closedarea;
	select * from closedarea;
	insert ignore into closedarea(ZoneID) values (${zone_id});
	select count(*) as 强化boss后closedarea表的记录数 from closedarea;
	select * from closedarea;
_EOFMYSQL_
}

# 将玩家拷贝表和门派拷贝表从合区库移动到目的区
function import_player_faction_from_mergedb()
{
	local dst_zone_idx=$1
	local src_zone_idx=$2
	local merge_db=$3
	local merge_n_to_1_dir="$4"

	if [ $dst_zone_idx -gt $src_zone_idx ]; then
		echo_err "dst_zone_idx > src_zone_idx: $dst_zone_idx > $src_zone_idx"
		return
	fi

	local dst_zone_id=${g_zone_id_array[dst_zone_idx]}
	local dst_zone_name=${g_zone_name_array[dst_zone_idx]}
	local dst_gamedb=${g_gamedb_array[dst_zone_idx]}

	local src_zone_id=${g_zone_id_array[src_zone_idx]}
	local src_zone_name=${g_zone_name_array[src_zone_idx]}
	local src_gamedb=${g_gamedb_array[src_zone_idx]}

	local dst_zone_conn_opt=`get_zone_conn_opt $dst_zone_idx`
	local src_zone_conn_opt=`get_zone_conn_opt $src_zone_idx`

	# 从merge库中导出players和factions表（不导出表结构、不drop表）
	local copy_players_table=`get_copy_players_table ${src_zone_idx}`
	local copy_faction_table=`get_copy_faction_table ${src_zone_idx}`

	local src_fixed_name_players_faction_sql=${merge_n_to_1_dir}/${src_gamedb}.players.faction.sql

	echo_warn "[$src_zone_name -> $dst_zone_name] : moving [${copy_players_table}] and [${copy_faction_table}]> ..."
	echo_warn "[$src_zone_name -> $dst_zone_name] : exporting copy players and faction -> [${src_fixed_name_players_faction_sql}] ..."

	time mysqldump $dst_zone_conn_opt $g_mysqldump_no_create_db_no_create_table $merge_db $copy_players_table $copy_faction_table > "$src_fixed_name_players_faction_sql"
	wait

	sed -i "s/${copy_players_table}/players/g" "${src_fixed_name_players_faction_sql}"
	sed -i "s/${copy_faction_table}/faction/g" "${src_fixed_name_players_faction_sql}"

	# 开始导入到目标区
	if [ $dst_zone_idx -eq $src_zone_idx ]; then
mysql $dst_zone_conn_opt $dst_gamedb << _EOFMYSQL_
		delete from players where PlayerID > 1010;
		truncate faction;
_EOFMYSQL_
	fi

	echo_warn "[$src_zone_name -> $dst_zone_name] : importing players and factions from [${src_fixed_name_players_faction_sql}] ..."
	time mysql $dst_zone_conn_opt $dst_gamedb < "$src_fixed_name_players_faction_sql"
	echo_ok "[$src_zone_name -> $dst_zone_name] : importing players and factions from [${src_fixed_name_players_faction_sql}] ok"

	rm -f ${src_fixed_name_players_faction_sql}
}

function merge_n_to_1()
{
	local start_zone_idx=$1
	local end_zone_idx=$2
	local merge_db=$3

	local zone_name_list=`tie_zone_name $start_zone_idx $end_zone_idx`
	local zone_id_list=`tie_zone_id $start_zone_idx $end_zone_idx "_"`

	local merge_n_to_1_dir=${g_merge_dir}/merge_${zone_id_list}

	if [ ! -d "$merge_n_to_1_dir" ]; then
		mkdir "$merge_n_to_1_dir"
	fi
	
	local dst_zone_conn_opt=`get_zone_conn_opt $start_zone_idx`

	echo_warn "**********************start merge $zone_name_list***********************"
	local i=1
	for ((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		copy_players_faction $i
	}

mysql $dst_zone_conn_opt << _EOFMYSQL_
	create database if not exists ${merge_db};
_EOFMYSQL_

	# 将各个区的玩家拷贝表和门派拷贝表集中移动到第一个区所在的merge_年_月_日数据库
	for ((i=$start_zone_idx;i<=$end_zone_idx;i++))
	{
		move_players_faction_to_mergedb $start_zone_idx $i $merge_db
	}

	# 检测出冲突玩家并打印出列表
	fix_conflict_name $start_zone_idx $end_zone_idx $merge_db "${merge_n_to_1_dir}"

	echo_blue "----------------------------------fix_conflict_name End----------------------------------------------"
#------------到这一步开始第1个区的表开始变动
	# 先修正第一个区的玩家和门派表名称
	import_player_faction_from_mergedb ${start_zone_idx} ${start_zone_idx} ${merge_db} "${merge_n_to_1_dir}"
echo_blue "----------------------------------import_player_faction_from_mergedb End----------------------------------------------"
	# 将各个区的数据移动到第一个区
	for ((i=$(($start_zone_idx+1));i<=$end_zone_idx;i++))
	{
		move_zone $start_zone_idx $i $merge_db "$merge_n_to_1_dir"
	}

	make_merge_gift $start_zone_idx $end_zone_idx "$merge_n_to_1_dir"
	give_merge_gift $start_zone_idx $end_zone_idx $merge_db "$merge_n_to_1_dir"

	set_boss_more_powerful $start_zone_idx

	echo_warn "**********************merge $zone_name_list finished********************"
}

function start_merge_zone()
{
	local zone_name_list=`tie_zone_name 1 $g_zone_cnt`
	local zone_id_list=`tie_zone_id 1 $g_zone_cnt "_"`

	echo_blue "**********************************start merge $zone_name_list**********************************************"

	# 获取合区列表所在的文件夹（之后的所有合区日志和文件都将放在该文件夹下）
	local merge_db=merge_${today}

	local i=0
	for ((i=1;i<$g_zone_cnt;i=$(($i+$g_merge_how_many_zone_to_1)) ))
	{
		merge_n_to_1 $i $(($i+$g_merge_how_many_zone_to_1-1)) ${merge_db}
	}

	echo_blue "**********************************merge $zone_name_list finished*******************************************"
}

#################end:合区#################

#################start:备份#################
# 检测今天是否已备份过，如果没有则强制退出
function check_backup()
{
	local zone_name_list=`tie_zone_name 1 $g_zone_cnt`
	local zone_id_list=`tie_zone_id 1 $g_zone_cnt "_"`

	local base_dir=${g_merge_zone_list%%/*}
	local backup_dir=${base_dir}/backup_${zone_id_list}_at_${today}

	if ! ls ${backup_dir}* >/dev/null 2>&1; then
		echo_err "warning: can not find backup of ${zone_name_list}, start backing up!"
		time backup_zones
	fi
}

# 备份指定区的游戏库（不备份战斗录像）
function backup_zone()
{
	local zone_idx=$1

	local zone_id=${g_zone_id_array[zone_idx]}
	local zone_name=${g_zone_name_array[zone_idx]}
	local gamedb=${g_gamedb_array[zone_idx]}

	local dump_sql=$g_backup_dir/${gamedb}.sql

	echo_warn "**********************[$zone_name] : start backup $gamedb -> $dump_sql********************"

	local zone_conn_opt=`get_zone_conn_opt $zone_idx`
	time mysqldump $zone_conn_opt -v $g_mysqldump_backup $gamedb > "$dump_sql"

	if [ ! -f "$dump_sql" ]; then
		echo_err "error: dump $gamedb -> $dump_sql fail, exiting!"
		exit
	fi
	echo_ok "[$zone_name] : ok: dump $gamedb -> $dump_sql successfully!"

	echo_ok "**********************[$zone_name] : backup $gamedb -> $dump_sql finished********************"
}

# 在合区前先备份好被合入的区
function backup_zones()
{
	local zone_name_list=`tie_zone_name 1 $g_zone_cnt`
	local zone_id_list=`tie_zone_id 1 $g_zone_cnt "_"`

	echo_blue "**********************************$zone_name_list : start backup**********************************************"

	local base_dir=${g_merge_zone_list%%/*}
	g_backup_dir=${base_dir}/backup_${zone_id_list}_at_${now}

	if [ ! -d "$g_backup_dir" ]; then
		mkdir "$g_backup_dir"
	fi

	local i=0
	for ((i=1;i<$g_zone_cnt;i=$(($i+$g_merge_how_many_zone_to_1)) ))
	{
		backup_zone $i &
	}

	wait
	echo_blue "**********************************$zone_name_list : backup finished*******************************************"
}
#################end:备份#################

###############################################################主入口##########################################################################

# 合区列表
g_merge_zone_list=$1
g_pay_db_list=$2

# 执行哪种操作：
# 1. backup -> 合区前的备份，仅备份被合入的区，如：1~8区合入1区，则仅备份1区
# 2. check  -> 合区前检测，检测各区间是否存在重复的玩家id，如果有则打印出来
# 3. merge_年_月_日_小时 -> 开始合区：为尽量避免参数传入失误，故将当前时间传入（精确到小时），如:merge_2014_04_10_12
method=$3

# 1. 检测合区列表是否存在
	if [ ! $g_merge_zone_list ] || [ ! -n "$g_merge_zone_list" ] || [ ! -f $g_merge_zone_list ]; then
		echo_err "err: detecting zone list file <$g_merge_zone_list> not exist, aborted!"
		exit
	fi

	if [ ! $g_pay_db_list ] || [ ! -n "$g_pay_db_list" ] || [ ! -f $g_pay_db_list ]; then
		echo_err "err: detecting pay db list file <$g_pay_db_list> not exist, aborted!"
		exit
	fi

# 2. 获取数据库密码，通过手动输入的方式以防止密码泄露
	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	read -s dbpwd

	if [ "$dbpwd" = "" ]; then
		echo_err "err: you enter nothing, aborted!"
		exit
	fi

# 3. 根据传入的合区列表依次检测是否能成功连上各区的数据库

	# 传入的服务器ip和port列表将如下面所示：
	# --------------------------------------------------------------------------------------------------------
	# 区号		区名		合区礼包		游戏库名			数据库ip			本地socket位置
	# 57		囧57区		10057			ios_app_s057		192.168.2.99		/tmp/mysql_63306.sock	
	# 58		囧58区		10058			ios_app_s058		192.168.2.98		null
	# 59		囧59区		10059			ios_app_s059		192.168.2.98		null
	# 60		囧60区		10060			ios_app_s060		192.168.2.98		null
	# --------------------------------------------------------------------------------------------------------

	# 开始检测
	ck_db_by_list "$g_merge_zone_list"
	ck_pay_db_by_list "$g_pay_db_list"

	# 发现无法连接则退出
	if [ $g_err_cnt -gt 0 ]; then
		echo_err "error: detecting there are $g_err_cnt errors, aborted!"
		exit 1
	fi

	all_zone_id_list=`tie_zone_id 1 $g_zone_cnt "_"`
	base_dir=${g_merge_zone_list%%/*}

# 3. 开始合区
	if [ $method = "backup" ]; then
		time backup_zones
	elif [ $method = "check" ]; then
		time check_merge
	else
		if [ $method = "merge_${hour_now}" ]; then
			check_backup

			g_merge_dir=${base_dir}/merge_${all_zone_id_list}_at_${now}
			if [ ! -d "$g_merge_dir" ]; then
				mkdir "$g_merge_dir"
			fi

			all_zone_name_list=`tie_zone_name 1 $g_zone_cnt`

			jxy_merge_log=${g_merge_dir}/jxy_merge_${all_zone_id_list}_at_${now}.log

			# check_merge >> ${jxy_merge_log}
			time start_merge_zone ${g_merge_dir} | tee "${jxy_merge_log}"
			wait

			clean_log "${jxy_merge_log}"
		else
			echo_err "err: unrecognized method = $method, aborted!"
			exit
		fi
	fi

	# 等待所有的线程执行完毕
	wait