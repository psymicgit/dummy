
#  @File Name: jxy_start_svr.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: 2013-12-25 19:49:27

#!/bin/bash

source ./util.sh

# 连接的数据库信息
mysqlhost=$1
mysqlport=$2
mysqlopt="$3"

dump_dir=$4
db=$5

extra_opt=$6

echo_blue "start export zone $dbs @ [${mysqlhost}:${mysqlport}] to $dump_dir"
echo "detect whether connecting $mysqlhost success"

# 1. 检测是否能成功连上数据库

	ck_conn_ok_by_opt $mysqlhost $mysqlport "$mysqlopt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit 1
	fi

# 2. 开始备份数据库
echo "start dump databases [${mysqlhost}:${mysqlport}] to $dump_dir"
	# 检测数据库已存在，如果存在则中断拷贝
	# 打印出当前数据库列表，将结果输出到log文件
mysql $mysqlopt << _EOFMYSQL_ > log
    show databases;
_EOFMYSQL_

	# 检测数据库是否已存在（通过判断数据库列表中是否含有该数据库）
	if ! find_db=$(grep -i -w "$db" log); then 
		echo_err "error: could not found <$db> at [${mysqlhost}:${mysqlport}], aborted!";
		exit 1;
	fi

	echo_warn "dumping $db -> $dump_dir/$db.sql ....."
	time mydumper -S /tmp/mysql_63306.sock -u root -p Maxnet.1234 -v 2 -C -t 24 -B ios_app_s001 --less-locking --outputdir=/home3/bak/2014-04-04/

	if [ ! -f "$dump_dir"/$db.sql ]; then
		echo_err "error: dump $db -> "$dump_dir"/$db.sql fail, exiting!"
		exit
	fi
	echo_ok "ok: dump $db to "$dump_dir"/$db.sql successfully!"

echo_ok "succ: dump databases ${db} Complete!"
