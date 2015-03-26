#!/bin/bash

# 将db2合并到db1
cur_dir=$(pwd)

# 首先，生成当前日期的文件夹
# now=`date '+%Y-%m-%d_%Hh%Mm%Ss'`
now=`date '+%Y-%m-%d'`
dump_dir="$cur_dir/backup_$now"
if [ ! -d "$dump_dir" ]; then
	mkdir "$dump_dir"
fi

# 1. 检测是否能成功连上数据库
echo "***********************************************************************************************************"

echo "preparing to connect to ios db, please enter the mysql password:"
# read -s dbpwd

dbpwd=Maxnet.1234
if [ "$dbpwd" = "" ]; then
	echo_err "err: you enter nothing, aborted!"
	exit
fi

function copy_45_48_to_localhost()
{
	declare -a localhost_gamedb=(
		"192.168.4.106"
		"3306"
		"-h192.168.4.106 -uroot -p123456"
	)

	declare -a zone_41_53_gamedb=(
		"118.26.225.100"
		"63316"
		"-h118.26.225.100 -P63316 -uroot -p$dbpwd"
	)

	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_41_53_gamedb[0]}" "${zone_41_53_gamedb[1]}" "${zone_41_53_gamedb[2]}" $dump_dir "ios_app_s041" "ios_app_s041" ""
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_41_53_gamedb[0]}" "${zone_41_53_gamedb[1]}" "${zone_41_53_gamedb[2]}" $dump_dir "ios_app_s045" "ios_app_s045" ""
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_41_53_gamedb[0]}" "${zone_41_53_gamedb[1]}" "${zone_41_53_gamedb[2]}" $dump_dir "ios_app_s049" "ios_app_s049" ""
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_41_53_gamedb[0]}" "${zone_41_53_gamedb[1]}" "${zone_41_53_gamedb[2]}" $dump_dir "ios_app_s053" "ios_app_s053" ""
}

function copy_81_88_to_localhost()
{
	declare -a localhost_gamedb=(
		"192.168.4.106"
		"3306"
		"-h192.168.4.106 -uroot -p123456"
	)

	declare -a zone_81_85_gamedb=(
		"118.26.225.104"
		"63316"
		"-h118.26.225.104 -P63316 -uroot -p$dbpwd"
	)

	declare -a zone_86_88_gamedb=(
		"118.26.225.105"
		"63316"
		"-h118.26.225.105 -P63316 -uroot -p$dbpwd"
	)

	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_81_85_gamedb[0]}" "${zone_81_85_gamedb[1]}" "${zone_81_85_gamedb[2]}" $dump_dir "gvg_s000" "ios_app_s081" "--ignore-table=ios_app_s081.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_81_85_gamedb[0]}" "${zone_81_85_gamedb[1]}" "${zone_81_85_gamedb[2]}" $dump_dir "gvg_s081" "ios_app_s081" "--ignore-table=ios_app_s081.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_81_85_gamedb[0]}" "${zone_81_85_gamedb[1]}" "${zone_81_85_gamedb[2]}" $dump_dir "gvg_s082" "ios_app_s082" "--ignore-table=ios_app_s082.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_81_85_gamedb[0]}" "${zone_81_85_gamedb[1]}" "${zone_81_85_gamedb[2]}" $dump_dir "gvg_s083" "ios_app_s083" "--ignore-table=ios_app_s083.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_81_85_gamedb[0]}" "${zone_81_85_gamedb[1]}" "${zone_81_85_gamedb[2]}" $dump_dir "gvg_s084" "ios_app_s084" "--ignore-table=ios_app_s084.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_81_85_gamedb[0]}" "${zone_81_85_gamedb[1]}" "${zone_81_85_gamedb[2]}" $dump_dir "gvg_s085" "ios_app_s085" "--ignore-table=ios_app_s085.battlelog"


	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_86_88_gamedb[0]}" "${zone_86_88_gamedb[1]}" "${zone_86_88_gamedb[2]}" $dump_dir "gvg_s086" "ios_app_s086" "--ignore-table=ios_app_s086.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_86_88_gamedb[0]}" "${zone_86_88_gamedb[1]}" "${zone_86_88_gamedb[2]}" $dump_dir "gvg_s087" "ios_app_s087" "--ignore-table=ios_app_s087.battlelog"
	./jxy_copy_db.sh "${localhost_gamedb[0]}" "${localhost_gamedb[1]}" "${localhost_gamedb[2]}" "${zone_86_88_gamedb[0]}" "${zone_86_88_gamedb[1]}" "${zone_86_88_gamedb[2]}" $dump_dir "gvg_s088" "ios_app_s088" "--ignore-table=ios_app_s088.battlelog"
}

copy_81_88_to_localhost