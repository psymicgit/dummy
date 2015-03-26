#  @File Name: setup mysql binlog.sh
#  @Author: hongkunan
#  @Company: heihuo
#  @brief: 囧西游合区脚本
#  @Created Time: 2014年2月24日 9:23:35

#!/bin/bash

source ./util.sh

# 将db2合并到db1

db1host=192.168.4.109
db1port=3306

db2host=192.168.4.106
db2port=3306

db1rawopt=" -S /tmp/mysql_3306.sock -uroot"
db2rawopt=" -h${db2host} -P${db2port} -uroot"

echo_blue "connecting to db1 [${db1host}:${db1port}], the option = [mysql $db1rawopt], please enter the password:"
read -s db1pwd

echo_blue "connecting to db2 [${db2host}:${db2port}], the option = [mysql $db2rawopt], please enter the password:"
read -s db2pwd

db1opt=" $db1rawopt -p$db1pwd "
db2opt=" $db2rawopt -p$db2pwd "

# db2合并到db1，但在合并的过程中db2中某些表数据也将发生变化
db1=run_s005
db2=run_s006

db1_desc=正式5区
db2_desc=正式6区

cur_dir=$(pwd)

# 首先，生成当前日期的文件夹
# now=`date '+%Y-%m-%d_%Hh%Mm%Ss'`
now=`date '+%Y-%m-%d'`
dump_dir="$cur_dir/backup_$now"
if [ ! -d "$dump_dir" ]; then
	mkdir "$dump_dir"
fi

# 1. 检测是否能成功连上数据库
echo_blue "***********************************************************************************************************"
	ck_conn_ok_by_opt $db1host $db1port "$db1opt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit 1
	fi

	ck_conn_ok_by_opt $db2host $db2port "$db2opt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit 1
	fi

mysql $db1opt $v << _EOFMYSQL_ > log
	drop database if exists $db1;
	drop database if exists $db2;
_EOFMYSQL_

	./jxy_copy_db.sh $db1host $db1port "$db1opt" $db2host $db2port "$db2opt" $dump_dir $db1 $db1
	./jxy_copy_db.sh $db1host $db1port "$db1opt" $db2host $db2port "$db2opt" $dump_dir $db2 $db2

mysql $db1opt -vvvv << _EOFMYSQL_ > log
	use $db1;
	# call union_databases('${db1}', '${db1_desc}', '${db2}', '${db2_desc}');
_EOFMYSQL_
