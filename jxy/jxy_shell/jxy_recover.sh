
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

# 备份好的sql文件路径
dbsql=$4

# 将被还原到的数据库
db=$5

# 首先检测环境
echo_blue "start recover databases <$db> @ [${mysqlhost}:${mysqlport}] from $dbsql"
	ck_conn_ok_by_opt $mysqlhost $mysqlport "$mysqlopt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit 1
	fi

	if [ ! -f $dbsql ]; then
		echo_err "error: can not found $dbsql, exiting!"
		exit 1
	fi

# 检测数据库已存在，如果存在则中断拷贝
	# 打印出当前数据库列表，将结果输出到log文件
mysql $mysqlopt $v << _EOFMYSQL_ > log
    show databases;
_EOFMYSQL_

	# 检测数据库是否已存在（通过判断数据库列表中是否含有该数据库）
	if find_db=$(grep -i -w "$db" log); then 
		echo_err "error: detecting <$db> is already at [${mysqlhost}:${mysqlport}], aborted!";
		exit 1;
	fi

	#创建数据库
mysql $mysqlopt $v << _EOFMYSQL_ > log
    create database $db default character set utf8 collate utf8_general_ci;
    show databases;
_EOFMYSQL_

	# 检测是否成功创建了数据库（通过判断数据库列表中是否含有该数据库）
	if ! find_db=$(grep -i -w "$db" log); then 
		echo_err "error: create database <$db> failed!";
		exit 1;
	fi
	
	echo_ok "ok: create database <$find_db> successfully!"

	# 开始还原数据库（通过执行sql备份文件）
	echo_warn "recovering <$db> @ [${mysqlhost}:${mysqlport}] <- $dbsql ....."

	set +e

# 打印出当前数据库列表，将结果输出到log文件
	mysql $mysqlopt --force -C $db < $dbsql

	set -e

# 打印出当前数据库列表，将结果输出到log文件
mysql $mysqlopt $v << _EOFMYSQL_ >> log
    show databases;
_EOFMYSQL_

echo_ok "succ: recover databases <${db}> @ [${mysqlhost}:${mysqlport}] from $dbsql Complete!"
