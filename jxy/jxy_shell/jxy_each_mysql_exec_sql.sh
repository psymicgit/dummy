#  @File Name: jxy_each_mysql_exec_sql.sh
#  @Author: 洪坤安
#  @Company: heihuo
#  @brief: 囧西游：让现在所有的正式服<ios/android/台湾/同步>都一一执行指定的sql脚本文件
#  @Created Time: 2014年3月13日 17:02:10

#!/bin/bash

source ./util.sh

declare err_cnt=0;
declare exec_log="exec_sql.log"
v=""

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

# 根据传入的区列表，在每个区都执行一遍数据库语句
function each_mysql_exec_sql()
{
	# 服务器列表文件
	local zone_list=$1

	# 执行的sql文件路径
	local gamedbsql=$2

	echo "**********************************start executing $gamedbsql to $zone_list**********************************************" >> $exec_log

	if [ -n "$zone_list" ] && [ ! -f $zone_list ]; then
		echo_warn "error: can not found $zone_list file, exiting!"
		return 1
	fi

	if [ ! -n "$zone_list" ]; then
		echo_warn "warning: can not found $gamedbsql file!"
		return
	fi

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

		local gamedbdesc=$gamedb" @ "$gamedb_ip_port
		echo_blue "-----------------start executing $gamedbsql at $gamedbdesc---------------"
		mysql $gamedbopt -vvv < $gamedbsql >> $exec_log
		echo_blue "-----------------execute $gamedbsql at $gamedbdesc finished---------------" >> $exec_log
	done < $zone_list

	echo "**********************************executing $gamedbsql to $zone_list finished*******************************************" >> $exec_log
}

# 开始进行配置了
# ios平台
ios_mysql_list="$1"

# android平台
and_mysql_list="$2"

# 台湾平台
tw_mysql_list="$3"

# 各个mysql实例将要执行的脚本文件
sql_file=$4

# 1. 首先，检测必要的环境
	if [ ! -f $ios_mysql_list ] && [ ! -f $and_mysql_list ] && [ ! -f $tw_mysql_list ]; then
		echo_err "err: detecting $ios_mysql_list or $and_mysql_list or $tw_mysql_list not exist, aborted!"
		exit
	fi

	if [ ! -f $sql_file ] && [ ! -f $logdb_sql_file ]; then
		echo_err "err: detecting $sql_file or $logdb_sql_file not exist, aborted!"
		exit
	fi

# 2. 其次，获取数据库密码，通过手动输入的方式以防止密码泄露
	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	# read -s dbpwd

	dbpwd=Maxnet.1234
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
	ck_conn_mysql_by_mysql_list "$ios_mysql_list"
	ck_conn_mysql_by_mysql_list "$and_mysql_list"
	ck_conn_mysql_by_mysql_list "$tw_mysql_list"

	# 发现无法连接则退出
	if [ $err_cnt -gt 0 ]; then
		echo_err "error: detecting there are $err_cnt errors, aborted!"
		exit 1
	fi

	echo "" > $exec_log

# 3. 开始在不同平台的每个区执行sql文件
	if [ -f "$ios_mysql_list" ]; then
		each_mysql_exec_sql "$ios_mysql_list"  "$sql_file"
	fi

	if [ -f "$and_mysql_list" ]; then
		each_mysql_exec_sql "$and_mysql_list"  "$sql_file"
	fi
	
	if [ -f "$tw_mysql_list" ]; then
		each_mysql_exec_sql "$tw_mysql_list"   "$sql_file"
	fi

	exit