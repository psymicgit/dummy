#  @File Name: jxy_push_sql
#  @Author: 洪坤安
#  @Company: heihuo
#  @brief: 囧西游：让现在所有的正式服<ios/android/台湾/同步>都一一执行指定的sql脚本文件
#  @Created Time: 2014年3月13日 17:02:10

#!/bin/bash

source ./util.sh

declare err_cnt=0;
declare exec_log="exec_sql.log"
v=-vvv

# 依次链接数据库列表文件中的每一个数据库实例，检测是否成功
function ck_conn_mysql_by_mysql_list()
{
	local zone_list=$1

	echo_blue "**********************************start connect to $zone_list**********************************************"

	if [ ! -f $zone_list ]; then
		echo_err "error: can not found $zone_list file, exiting!"
		exit 1
	fi

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

		# 检测是否能成功连接得上日志数据库
		ck_conn_ok_by_opt_and_desc "$logdbdesc" "$gamedbopt"
		conn_mysql_succ=$?
		if [ "$conn_mysql_succ" -ne "0" ]; then
			((err_cnt++))
			exit 1
		fi
	done < $zone_list
	echo_blue "**********************************connect to $zone_list finished*******************************************"
}

# 依次链接数据库列表文件中的每一个数据库实例，并检测是否含有对应的数据库，以免出现意外
function ck_db_exist_by_zone_list()
{
	local zone_list=$1

	echo_blue "**********************************start connect to $zone_list**********************************************"

	if [ ! -f $zone_list ]; then
		echo_err "error: can not found $zone_list file, exiting!"
		exit 1
	fi

	# 依次检测是否能成功连上各个区的数据库，并检测该实例是否含有对应区的数据库
	while read line
	do
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

		mysql $gamedbopt -e "show databases;" > log
		# 检测数据库列表中是否含有该游戏数据库
		if ! find_db=$(grep -i -w "$gamedb" log); then 
			echo_err "error: could not find database <$gamedbdesc>!";
			((err_cnt++))
		else
			echo_ok "ok: detecting <$gamedbdesc> existing."
		fi

		mysql $logdbopt -e "show databases;" > log
		# 检测数据库列表中是否含有该日志数据库
		if ! find_db=$(grep -i -w "$logdb" log); then 
			echo_err "error: could not find database <$logdbdesc>!";
			((err_cnt++))
		else
			echo_ok "ok: detecting <$logdbdesc> existing."
		fi

	done < $zone_list
	echo_blue "**********************************connect to $zone_list finished*******************************************"
}

# 重新生成mysql实例列表
function regenerate_mysql_list()
{
	ios_conn_opt=$1
	and_conn_opt=$2
	tw_conn_opt=$3

	ios_mysql_list=$4
	and_mysql_list=$5
	tw_mysql_list=$6

	mysql $ios_conn_opt -e "use ios_app_usercenter;select * from zonedb group by GameDbIP,GameDbPort order by ZoneID asc;" > $ios_mysql_list
	mysql $and_conn_opt -e "use and_app_usercenter;select * from zonedb group by GameDbIP,GameDbPort order by ZoneID asc;" > $and_mysql_list
	mysql $tw_conn_opt  -e "use tw_app_usercenter;select * from zonedb group by GameDbIP,GameDbPort order by ZoneID asc;"  > $tw_mysql_list

	# 删除第一行中的字段名称
	sed -i '1d' $ios_mysql_list
	sed -i '1d' $and_mysql_list
	sed -i '1d' $tw_mysql_list

	# 把修复错误的从库port
	sed -i 's/63306/63307/g' $ios_mysql_list
	sed -i 's/63306/63307/g' $and_mysql_list
	sed -i 's/63306/63307/g' $tw_mysql_list

	# 把从库的port换为主库的port
	sed -i 's/63316/63306/g' $ios_mysql_list
	sed -i 's/63316/63306/g' $and_mysql_list
	sed -i 's/63316/63306/g' $tw_mysql_list

	sort -n $ios_mysql_list | uniq
	sort -n $and_mysql_list | uniq
	sort -n $tw_mysql_list | uniq
}

# 重新生成区列表
function regenerate_zone_list()
{
	ios_conn_opt=$1
	and_conn_opt=$2
	tw_conn_opt=$3

	ios_zone_list=$4
	and_zone_list=$5
	tw_zone_list=$6

	mysql $ios_conn_opt -e "use ios_app_usercenter;select * from zonedb order by ZoneID asc;" > $ios_zone_list
	mysql $and_conn_opt -e "use and_app_usercenter;select * from zonedb order by ZoneID asc;" > $and_zone_list
	mysql $tw_conn_opt -e "use tw_app_usercenter;select * from zonedb order by ZoneID asc;"  > $tw_zone_list

	# 删除第一行中的字段名称
	sed -i '1d' $ios_zone_list
	sed -i '1d' $and_zone_list
	sed -i '1d' $tw_zone_list

	sed -i 's/63306/63307/g' $ios_mysql_list
	sed -i 's/63306/63307/g' $and_mysql_list
	sed -i 's/63306/63307/g' $tw_mysql_list

	sed -i 's/63316/63306/g' $ios_zone_list
	sed -i 's/63316/63306/g' $and_zone_list
	sed -i 's/63316/63306/g' $tw_zone_list
}

# 开始进行配置了
# ios平台
zonedb_dir=zone_list

ios_ip=118.26.225.102
ios_port=63301
ios_mysql_list=$zonedb_dir/ios_mysql.list
ios_zone_list=$zonedb_dir/ios_zone.list

# android平台
and_ip=42.62.60.197
and_port=63301
and_mysql_list=$zonedb_dir/and_mysql.list
and_zone_list=$zonedb_dir/and_zone.list

# 台湾平台
tw_ip=210.59.247.145
tw_port=63311
tw_mysql_list=$zonedb_dir/tw_mysql.list
tw_zone_list=$zonedb_dir/tw_zone.list

is_check_conn=0

# 1. 首先，获取各个区所属的数据库名称和ip、端口列表，这个列表将按数据库实例来分类，而不是按区来区分
	# 先获取数据库密码，通过手动输入的方式以防止密码泄露
	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	read -s dbpwd

	if [ "$dbpwd" = "" ]; then
		echo_err "err: you enter nothing, aborted!"
		exit
	fi

	if [ ! -d "$zonedb_dir" ]; then
		mkdir "$zonedb_dir"
	fi

	ios_conn_opt="-h$ios_ip -P$ios_port -uroot -p$dbpwd"
	and_conn_opt="-h$and_ip -P$and_port -uroot -p$dbpwd"
	tw_conn_opt="-h$tw_ip  -P$tw_port  -uroot -p$dbpwd"

	# 首先通过select * from zonedb group by xxxxxx获取ios、安卓平台的服务器ip和端口列表，过滤掉处于同一台服务器的冗余记录
	# select * from zonedb的结果将如下面所示
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

	# 重新生成各个区所属的mysql实例列表
	regenerate_mysql_list "$ios_conn_opt" "$and_conn_opt" "$tw_conn_opt" "$ios_mysql_list" "$and_mysql_list" "$tw_mysql_list"

# 2. 其次，依次检测是否能成功连上各台服务器的数据库
	ck_conn_mysql_by_mysql_list "$ios_mysql_list"
	ck_conn_mysql_by_mysql_list "$and_mysql_list"
	ck_conn_mysql_by_mysql_list "$tw_mysql_list"

# 3. 然后，获取各个区的数据库名称和ip、端口列表，这个列表按区来归类，而不是按数据库实例来归类
	set +e

	# 重新生成各个区对应的数据库ip和端口列表
	regenerate_zone_list "$ios_conn_opt" "$and_conn_opt" "$tw_conn_opt" "$ios_zone_list" "$and_zone_list" "$tw_zone_list"

	ck_db_exist_by_zone_list "$ios_zone_list"
	ck_db_exist_by_zone_list "$and_zone_list"
	ck_db_exist_by_zone_list "$tw_zone_list"

	set -e

	if [ $err_cnt -gt 0 ]; then
		echo_err "error: detecting there are $err_cnt errors, aborted!"
		exit 1
	fi

	exit