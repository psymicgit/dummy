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
read -s dbpwd

if [ "$dbpwd" = "" ]; then
	echo_err "err: you enter nothing, aborted!"
	exit
fi

mysqldump -S /tmp/mysql_63306.sock -uroot -p$dbpwd --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines  --ignore-table=ios_app_s037.battlelog ios_app_s037 > $dump_dir/ios_app_s037.sql
mysqldump -S /tmp/mysql_63306.sock -uroot -p$dbpwd --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines  --ignore-table=ios_app_s038.battlelog ios_app_s038 > $dump_dir/ios_app_s038.sql
mysqldump -S /tmp/mysql_63306.sock -uroot -p$dbpwd --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines  --ignore-table=ios_app_s037.battlelog ios_app_s039 > $dump_dir/ios_app_s039.sql
mysqldump -S /tmp/mysql_63306.sock -uroot -p$dbpwd --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines  --ignore-table=ios_app_s038.battlelog ios_app_s040 > $dump_dir/ios_app_s040.sql

mysqldump -S /tmp/mysql_63307.sock -uroot -p --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines ios_app_s037_log goldrecord_2014 > $dump_dir/ios_app_s037_log.sql
mysqldump -S /tmp/mysql_63307.sock -uroot -p --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines ios_app_s038_log goldrecord_2014 > $dump_dir/ios_app_s038_log.sql
mysqldump -S /tmp/mysql_63307.sock -uroot -p --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines ios_app_s039_log goldrecord_2014 > $dump_dir/ios_app_s039_log.sql
mysqldump -S /tmp/mysql_63307.sock -uroot -p --add-locks --add-drop-table --single-transaction --compress --extended-insert --opt --default-character-set=utf8 --events --hex-blob --skip-comments  --triggers --routines ios_app_s040_log goldrecord_2014 > $dump_dir/ios_app_s040_log.sql
