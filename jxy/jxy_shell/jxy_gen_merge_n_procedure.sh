#  @File Name: jxy_merge
#  @Author: hongkunan
#  @Company: heihuo
#  @brief: 囧西游合区脚本
#  @Created Time: 2014年2月24日 9:23:35

#!/bin/bash

source ./util.sh

merge_gamedb_sql=jxy_merge_gamedb_
merge_logdb_sql=jxy_merge_logdb_
merge_gamedb_procedure=merge_gamedb_
merge_logdb_procedure=merge_logdb_

merge_gift=merge_gift

# 数据库列表
declare -a zones=(
	"ios_app_s033_merge		ios_app_s009_log		囧33区	33	10033"
	"ios_app_s034		ios_app_s010_log		囧34区	34	10034"
	"ios_app_s035		ios_app_s011_log		囧35区	35	10035"
	"ios_app_s036		ios_app_s012_log		囧36区	36	10036"
	"ios_app_s037_merge		ios_app_s013_log		囧37区	37	10037"
	"ios_app_s038_merge		ios_app_s014_log		囧38区	38	10038"
	"ios_app_s039_merge		ios_app_s015_log		囧39区	39	10039"
	"ios_app_s040_merge		ios_app_s016_log		囧40区	40	10040"
)

dst_zone=${zones[i]}
fields=($(echo $dst_zone | tr ' ' ' ' | tr -s '	'))	

dst_gamedb=${fields[0]}
dst_logdb=${fields[1]}
dst_zone_name=${fields[2]}
dst_zoneid=${fields[3]}

for ((i=0;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	echo $zone

	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	field_cnt=${#fields[@]}

	zoneid=${fields[3]}
	merge_gamedb_sql="$merge_gamedb_sql""_""$zoneid"
	merge_logdb_sql="$merge_logdb_sql""_""$zoneid"
	merge_gamedb_procedure="$merge_gamedb_procedure""_""$zoneid"
	merge_logdb_procedure="$merge_logdb_procedure""_""$zoneid"
	merge_gift="$merge_gift""_""$zoneid"
}

merge_gamedb_sql="$merge_gamedb_sql"".sql"
merge_logdb_sql="$merge_logdb_sql"".sql"


echo "1. start configuring sql file"

function generate_gamedb_sql()
{
echo "" > $merge_gamedb_sql

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
DROP PROCEDURE IF EXISTS $merge_gamedb_procedure;

delimiter @@
CREATE PROCEDURE $merge_gamedb_procedure()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	set @g_cur_db = '${dst_gamedb}';
	select database() into @g_cur_db;

	if @g_cur_db <> '${dst_gamedb}' then
		select concat('请在${dst_gamedb}数据库下执行本脚本，当前数据库是', @g_cur_db);
		leave lable_1;
	end if;

	# 稍微检测一下两两合区时会不会碰到合表主键冲突（全部检测要 ${#zones[@]} * ${#zones[@]} / 2 次，太多了）
	SET @v_is_merge_check_ok=1;
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=0;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}
	logdb=${fields[1]}
	zone_name=${fields[2]}
	zoneid=${fields[3]}

	if [ $i -gt 0 ]; then
#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
	call merge_gamedb_check('${dst_gamedb}', '${dst_zone_name}', ${dst_zoneid}, '${gamedb}', '${zone_name}', ${zoneid}, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到${dst_zone_name}与${zone_name}的合区将发生冲突';
	end if;

_EOF_
#输出文件结束------------------------------------------------------------
	fi
}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql

	if @v_is_merge_check_ok = 1 then
		drop table if exists merge.${merge_gift};
		
		# 创建合区奖励表，为各区的玩家都添加一份合区礼包
		create table if not exists merge.${merge_gift} (
		  ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '0:冲值奖励，1：评分奖励',
		  StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		  EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		  PRIMARY KEY (ActivityID,StartPlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

		# 生成出冲突玩家昵称列表
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=0;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
		call merge_conflict_player_name('${gamedb}');
_EOF_
#输出文件结束------------------------------------------------------------
}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql

		# 生成冲突门派名称列表
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=0;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
		call merge_conflict_faction_name('${gamedb}');
_EOF_
#输出文件结束------------------------------------------------------------
}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();
_EOF_
#输出文件结束------------------------------------------------------------

cat << _EOF_ >> $merge_gamedb_sql

		# 在各个区上修改冲突玩家昵称并打印出冲突玩家列表：${dst_zone_name}直接在玩家表上改，其他区拷贝一份玩家表，再在拷贝的表上改名
		call modify_player_name_and_print('${dst_gamedb}', '${dst_zone_name}', 'players', '${dst_zoneid}');
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=1;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}
	logdb=${fields[1]}
	zone_name=${fields[2]}
	zoneid=${fields[3]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
		call cache_player('${gamedb}', '${zone_name}', '${zoneid}');
_EOF_
#输出文件结束------------------------------------------------------------
}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql

		# 在各个区上修改冲突门派名称并打印出冲突门派列表：${dst_zone_name}直接在门派原表上改，其他区拷贝一份门派表，再在拷贝的表上改名
		call modify_faction_name_and_print('${dst_gamedb}', '${dst_zone_name}', 'faction', '${dst_zoneid}');
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=1;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}
	logdb=${fields[1]}
	zone_name=${fields[2]}
	zoneid=${fields[3]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
		call cache_faction('${gamedb}', '${zone_name}', '${zoneid}');
_EOF_
#输出文件结束------------------------------------------------------------
}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql

		# 先分配好合区奖励
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=0;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}
	logdb=${fields[1]}
	zone_name=${fields[2]}
	zoneid=${fields[3]}
	gift_id=${fields[4]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
		call give_merge_gift(${gift_id}, '${gamedb}', '${zone_name}', '${merge_gift}');
_EOF_
#输出文件结束------------------------------------------------------------
}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql

#正式开始合区，注意，这里只有被合到的区数据会受到改动，如2区、3区合到1区，则只有1区数据会增加------------------------------------------------------------
_EOF_
#输出文件结束------------------------------------------------------------

for ((i=1;i<${#zones[@]};i++))
{
	zone=${zones[i]}
	fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
	
	gamedb=${fields[0]}
	logdb=${fields[1]}
	zone_name=${fields[2]}
	zoneid=${fields[3]}
	gift_id=${fields[4]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
		call merge_gamedb('${dst_gamedb}', '${dst_zone_name}', ${dst_zoneid}, '${gamedb}', '${zone_name}', ${zoneid});
_EOF_
#输出文件结束------------------------------------------------------------
}


#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_gamedb_sql
	end if;
end
@@

delimiter ;
call $merge_gamedb_procedure();
DROP PROCEDURE IF EXISTS $merge_gamedb_procedure;
_EOF_
#输出文件结束------------------------------------------------------------	
}

function generate_logdb_sql()
{
echo "" > $merge_logdb_sql

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_logdb_sql
DROP PROCEDURE IF EXISTS $merge_logdb_procedure;

delimiter @@
CREATE PROCEDURE $merge_logdb_procedure()
lable_1:
BEGIN
	set @g_cur_db = '';
	select database() into @g_cur_db;

	if @g_cur_db = '${dst_logdb}' then
_EOF_

#输出文件结束------------------------------------------------------------
	for ((i=1;i<${#zones[@]};i++))
	{
		zone=${zones[i]}
		fields=($(echo $zone | tr ' ' ' ' | tr -s '	'))
		
		gamedb=${fields[0]}
		logdb=${fields[1]}
		zone_name=${fields[2]}
		zoneid=${fields[3]}
		gift_id=${fields[4]}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_logdb_sql
		call merge_logdb('${dst_logdb}', '${dst_zone_name}', '${logdb}', '${zone_name}');
_EOF_
#输出文件结束------------------------------------------------------------
	}

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $merge_logdb_sql

		call split_oprecord('${dst_logdb}', '${dst_zone_name}');
	else
		select concat('请在${dst_logdb}数据库下执行本脚本，当前数据库是', @g_cur_db);
	end if;
end
@@

delimiter ;

call $merge_logdb_procedure();
DROP PROCEDURE IF EXISTS $merge_logdb_procedure;
_EOF_
#输出文件结束------------------------------------------------------------	
}

generate_gamedb_sql
