
#  @File Name: jxy_start_svr.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: 2013-12-25 19:49:27

#!/bin/bash

#告诉bash一但有任何一个语句返回非真的值，则退出bash
set -o errexit
set -o pipefail
set -o nounset

v="-v"

function get_now()
{
	v_now=`date '+%Y_%m_%d %H:%M:%S'`
}

# 绿色
function echo_ok()
{
	local msg=$1
	get_now
	echo -e "\\033[32m[$v_now] $msg\\033[0m"
}

# 红色
function echo_err()
{
	local msg=$1
	get_now
	echo -e "\\033[31m[$v_now] $msg\\033[0m"
}

# 黄色
function echo_warn()
{
	local msg=$1
	get_now
	echo -e "\\033[35m[$v_now] $msg\\033[0m"
}

# 蓝色
function echo_blue()
{
	local msg=$1
	get_now
	echo -e "\\033[34m[$v_now] $msg\\033[0m"
}

# 紫色
function echo_purple()
{
	local msg=$1
	get_now
	echo -e "\\033[35m[$v_now] $msg\\033[0m"
}

# 青绿
function echo_bluegreen()
{
	local msg=$1
	get_now
	echo -e "\\033[36m[$v_now] $msg\\033[0m"
}

# 检测是否能成功连接上数据库
function ck_conn_ok()
{
	local host=$1
	local port=$2
	local usr=$3
	local pwd=$4

mysql -h$host -P$port -u$usr -p$pwd << _EOFMYSQL_
_EOFMYSQL_

	local conn_mysql_succ=$?

	if [ "$conn_mysql_succ" -eq "0" ]; then
		echo_ok "ok: connect to db [${host}:${port}] success"
	else
		echo_err "error: can not connect to db [${host}:${port}], exiting!"
		return 1
	fi

	return 0
}

# 检测是否能成功连接上数据库
function ck_conn_ok_by_opt()
{
	local host=$1
	local port=$2
	local opt=$3

	# echo_warn "connecting to [${host}:${port}] now, please enter the pwd:"
mysql ${opt} << _EOFMYSQL_
_EOFMYSQL_

	local conn_mysql_succ=$?

	if [ "$conn_mysql_succ" -eq "0" ]; then
		echo_ok "ok: connect to db [${host}:${port}] success"
	else
		echo_err "error: can not connect to db [${host}:${port}], exiting!"
		return 1
	fi

	return 0
}

# 检测是否能成功连接上数据库
function ck_conn_ok_by_opt_and_desc()
{
	local dbdesc=$1
	local opt=$2

	echo_blue "connecting to $dbdesc:"
mysql ${opt} << _EOFMYSQL_
_EOFMYSQL_

	local conn_mysql_succ=$?

	if [ "$conn_mysql_succ" -eq "0" ]; then
		echo_ok "ok: connect to $dbdesc success"
	else
		echo_err "error: can not connect to $dbdesc, exiting!"
		return 1
	fi

	return 0
}

