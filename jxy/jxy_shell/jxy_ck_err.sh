#  @File Name: jxy_each_zone_exec_sql.sh
#  @Author: 洪坤安
#  @Company: heihuo
#  @brief: 囧西游：让现在所有的正式服<ios/android/台湾/同步>都一一执行指定的sql脚本文件
#  @Created Time: 2014年3月13日 17:02:10

#!/bin/bash

source ./util.sh

ck_err_sql=jxy_ck_err.sql
ck_err_log=jxy_ck_err.log

cat << _EOF_ > $ck_err_sql
	show slave status\G
_EOF_

time ./jxy_each_mysql_exec_sql.sh "zone_list/ios_mysql_slave.list" "zone_list/and_mysql_slave.list" '' ${ck_err_sql} ''