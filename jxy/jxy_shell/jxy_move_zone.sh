#  @File Name: jxy_merge
#  @Author: hongkunan
#  @Company: heihuo
#  @brief: 囧西游合区脚本
#  @Created Time: 2014年2月24日 9:23:35

#!/bin/bash

source ./util.sh

cur_dir=$(pwd)

# 首先，生成当前日期的文件夹
# now=`date '+%Y-%m-%d_%Hh%Mm%Ss'`
now=`date '+%Y-%m-%d_%Hh%Mm%Ss'`
dump_dir="$cur_dir/merge_$now"
if [ ! -d "$dump_dir" ]; then
	mkdir "$dump_dir"
fi

echo "***********************************************************************************************************"

echo "preparing to connect to ios db, please enter the mysql password:"
read -s dbpwd

if [ "$dbpwd" = "" ]; then
	echo_err "err: you enter nothing, aborted!"
	exit
fi

declare -a zone_1_6_37_40=(
	"118.26.225.102"
	"63306"
	"-S /tmp/mysql_63306.sock -uroot -p$dbpwd"
)

declare -a zone_7_19=(
	"118.26.225.98"
	"63306"
	"-h192.168.2.101 -P63306 -uroot -p$dbpwd"
)

declare -a zone_20_36=(
	"118.2.225.101"
	"63306"
	"-h192.168.2.100 -P63306 -uroot -p$dbpwd"
)


declare -a zone_1_6_37_40_log=(
	"118.26.225.102"
	"63307"
	"-S /tmp/mysql_63307.sock -uroot -p$dbpwd"
)

declare -a zone_7_19_log=(
	"118.26.225.98"
	"63307"
	"-h192.168.2.101 -P63307 -uroot -p$dbpwd"
)

declare -a zone_20_36_log=(
	"118.2.225.101"
	"63307"
	"-h192.168.2.100 -P63307 -uroot -p$dbpwd"
)


ck_conn_ok_by_opt "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}"
conn_mysql_succ=$?
if [ "$conn_mysql_succ" -ne "0" ]; then
	exit
fi

ck_conn_ok_by_opt "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"
conn_mysql_succ=$?
if [ "$conn_mysql_succ" -ne "0" ]; then
	exit
fi

ck_conn_ok_by_opt "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"
conn_mysql_succ=$?
if [ "$conn_mysql_succ" -ne "0" ]; then
	exit
fi

# 备份1, 9, 17, 25, 33区
nohup ./jxy_dump.sh "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}" $dump_dir "ios_app_s001" &
nohup ./jxy_dump.sh "${zone_7_19[0]}"      "${zone_7_19[1]}"      "${zone_7_19[2]}"      $dump_dir "ios_app_s009" &
nohup ./jxy_dump.sh "${zone_7_19[0]}"      "${zone_7_19[1]}"      "${zone_7_19[2]}"      $dump_dir "ios_app_s017" &
nohup ./jxy_dump.sh "${zone_20_36[0]}"     "${zone_20_36[1]}"     "${zone_20_36[2]}"     $dump_dir "ios_app_s025" &
nohup ./jxy_dump.sh "${zone_20_36[0]}"     "${zone_20_36[1]}"     "${zone_20_36[2]}"     $dump_dir "ios_app_s033" &

nohup ./jxy_dump.sh "${zone_1_6_37_40_log[0]}" "${zone_1_6_37_40_log[1]}" "${zone_1_6_37_40_log[2]}" $dump_dir "ios_app_s001_log" &
nohup ./jxy_dump.sh "${zone_7_19_log[0]}"      "${zone_7_19_log[1]}"      "${zone_7_19_log[2]}"      $dump_dir "ios_app_s009_log" &
nohup ./jxy_dump.sh "${zone_7_19_log[0]}"      "${zone_7_19_log[1]}"      "${zone_7_19_log[2]}"      $dump_dir "ios_app_s017_log" &
nohup ./jxy_dump.sh "${zone_20_36_log[0]}"     "${zone_20_36_log[1]}"     "${zone_20_36_log[2]}"     $dump_dir "ios_app_s025_log" &
nohup ./jxy_dump.sh "${zone_20_36_log[0]}"     "${zone_20_36_log[1]}"     "${zone_20_36_log[2]}"     $dump_dir "ios_app_s033_log" &

nohup ./jxy_copy_db.sh "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}"         "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}" $dump_dir "ios_app_s001_merge" "ios_app_s001" &
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}" $dump_dir "ios_app_s009_merge" "ios_app_s009" &
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}" $dump_dir "ios_app_s017_merge" "ios_app_s017" &
nohup ./jxy_copy_db.sh "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s025_merge" "ios_app_s025" &
nohup ./jxy_copy_db.sh "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s033_merge" "ios_app_s033" &

# 移动区

# 7-8
nohup ./jxy_copy_db.sh "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}"     "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}" $dump_dir "ios_app_s007_merge" "ios_app_s007" "--ignore-table=ios_app_s007.battlelog" &
nohup ./jxy_copy_db.sh "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}"     "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}" $dump_dir "ios_app_s008_merge" "ios_app_s008" "--ignore-table=ios_app_s008.battlelog" &

# 20-24
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s020_merge" "ios_app_s020" "--ignore-table=ios_app_s020.battlelog" &
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s021_merge" "ios_app_s021" "--ignore-table=ios_app_s021.battlelog" &
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s022_merge" "ios_app_s022" "--ignore-table=ios_app_s022.battlelog" &
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s023_merge" "ios_app_s023" "--ignore-table=ios_app_s023.battlelog" &
nohup ./jxy_copy_db.sh "${zone_7_19[0]}" "${zone_7_19[1]}" "${zone_7_19[2]}"         "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}" $dump_dir "ios_app_s024_merge" "ios_app_s024" "--ignore-table=ios_app_s024.battlelog" &

# 37-40
nohup ./jxy_copy_db.sh "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"      "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}" $dump_dir "ios_app_s037_merge" "ios_app_s037" "--ignore-table=ios_app_s037.battlelog" &
nohup ./jxy_copy_db.sh "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"      "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}" $dump_dir "ios_app_s038_merge" "ios_app_s038" "--ignore-table=ios_app_s038.battlelog" &
nohup ./jxy_copy_db.sh "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"      "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}" $dump_dir "ios_app_s039_merge" "ios_app_s039" "--ignore-table=ios_app_s039.battlelog" &
nohup ./jxy_copy_db.sh "${zone_20_36[0]}" "${zone_20_36[1]}" "${zone_20_36[2]}"      "${zone_1_6_37_40[0]}" "${zone_1_6_37_40[1]}" "${zone_1_6_37_40[2]}" $dump_dir "ios_app_s040_merge" "ios_app_s040" "--ignore-table=ios_app_s040.battlelog" &

echo "***********************************************************************************************************"