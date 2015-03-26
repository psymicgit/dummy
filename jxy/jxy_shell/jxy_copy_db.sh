#  @File Name: setup mysql binlog.sh
#  @Author: hongkunan
#  @Company: heihuo
#  @brief: 囧西游合区脚本
#  @Created Time: 2014年2月24日 9:23:35

#!/bin/bash

source ./util.sh

# 将db2拷贝到

dstdbhost=$1
dstdbport=$2
dstdbopt="$3"

srcdbhost=$4
srcdbport=$5
srcdbopt="$6"

# db2合并到db1，但在合并的过程中db2中某些表数据也将发生变化
dump_dir=$7

dstdb=$8
srcdb=$9

extra_dump_opt=${10}

echo_blue "start copying ${srcdb}@[${srcdbhost}:${srcdbport}] -> ${dstdb}@[${dstdbhost}:${dstdbport}]"

# 1. 检测是否能成功连上数据库
echo "1. detect whether connecting ${dstdbhost}:${dstdbport} and ${srcdbhost}:${srcdbport} success"
	ck_conn_ok_by_opt $srcdbhost $srcdbport "$srcdbopt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit
	fi

	ck_conn_ok_by_opt $dstdbhost $dstdbport "$dstdbopt"
	conn_mysql_succ=$?
	if [ "$conn_mysql_succ" -ne "0" ]; then
		exit
	fi

	if [ ! -d "$dump_dir" ]; then
		mkdir "$dump_dir"
	fi

	if [ "$srcdbhost" = "$dstdbhost" ] && [ $srcdbport -eq $dstdbport ] && [ "$dstdb" = "$srcdb" ]; then
		echo_err "err: detecting ${srcdb}@[${srcdbhost}:${srcdbport}] = ${dstdb}@[${dstdbhost}:${dstdbport}], skip copying, aborted!"
		exit 1
	fi

	# 检测数据库已存在，如果存在则中断拷贝
	# 打印出当前数据库列表，将结果输出到log文件
mysql $dstdbopt << _EOFMYSQL_ > log
    show databases;
_EOFMYSQL_

	# 检测数据库是否已存在（通过判断数据库列表中是否含有该数据库）
	if find_db=$(grep -i -w "$dstdb" log); then
		echo_err "error: detecting <$dstdb> is already at [${dstdbhost}:${dstdbport}], aborted!";
		exit 0;
	fi

# 2. 备份srcdb
echo "2. backup srcdb[${srcdbhost}:${srcdbport}] databases"
	echo "The backuped db will be stored in $dump_dir directory"
	./jxy_dump.sh $srcdbhost $srcdbport "$srcdbopt" $dump_dir $srcdb $extra_dump_opt

# 3. 将db2拷贝到db1所在的主机
echo "3. start copying ${srcdb}@[${srcdbhost}:${srcdbport}] -> ${dstdb}@[${dstdbhost}:${dstdbport}]"
	srcdbsql=${dump_dir}"/"${srcdb}".sql"
	./jxy_recover.sh $dstdbhost $dstdbport "$dstdbopt" $srcdbsql $dstdb

# 4. 结束
if [ $? -eq 0 ]; then
	echo_ok "ok: copying ${srcdb}@[${srcdbhost}:${srcdbport}] -> ${dstdb}@[${dstdbhost}:${dstdbport}] successfully!"
	echo_ok "all finished!"
else
	echo_err "aborted!"
fi
