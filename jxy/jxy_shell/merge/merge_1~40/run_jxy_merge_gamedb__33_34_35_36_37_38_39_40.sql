
DROP PROCEDURE IF EXISTS merge_gamedb__33_34_35_36_37_38_39_40;

delimiter @@
CREATE PROCEDURE merge_gamedb__33_34_35_36_37_38_39_40()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	set @g_cur_db = 'ios_app_s033';
	select database() into @g_cur_db;

	if @g_cur_db <> 'ios_app_s033' then
		select concat('请在ios_app_s033数据库下执行本脚本，当前数据库是', @g_cur_db);
		leave lable_1;
	end if;

	# 稍微检测一下两两合区时会不会碰到合表主键冲突（全部检测要 8 * 8 / 2 次，太多了）
	SET @v_is_merge_check_ok=1;
	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s034', '囧34区', 34, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧34区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s035', '囧35区', 35, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧35区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s036', '囧36区', 36, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧36区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s037_merge', '囧37区', 37, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧37区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s038_merge', '囧38区', 38, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧38区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s039_merge', '囧39区', 39, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧39区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s033', '囧33区', 33, 'ios_app_s040_merge', '囧40区', 40, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧33区与囧40区的合区将发生冲突';
	end if;


	if @v_is_merge_check_ok = 1 then
		drop table if exists merge.merge_gift_33_34_35_36_37_38_39_40;
		
		# 创建合区奖励表，为各区的玩家都添加一份合区礼包
		create table if not exists merge.merge_gift_33_34_35_36_37_38_39_40 (
		  ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '0:冲值奖励，1：评分奖励',
		  StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		  EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		  PRIMARY KEY (ActivityID,StartPlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

		# 生成出冲突玩家昵称列表
		call merge_conflict_player_name('ios_app_s033');
		call merge_conflict_player_name('ios_app_s034');
		call merge_conflict_player_name('ios_app_s035');
		call merge_conflict_player_name('ios_app_s036');
		call merge_conflict_player_name('ios_app_s037_merge');
		call merge_conflict_player_name('ios_app_s038_merge');
		call merge_conflict_player_name('ios_app_s039_merge');
		call merge_conflict_player_name('ios_app_s040_merge');

		# 生成冲突门派名称列表
		call merge_conflict_faction_name('ios_app_s033');
		call merge_conflict_faction_name('ios_app_s034');
		call merge_conflict_faction_name('ios_app_s035');
		call merge_conflict_faction_name('ios_app_s036');
		call merge_conflict_faction_name('ios_app_s037_merge');
		call merge_conflict_faction_name('ios_app_s038_merge');
		call merge_conflict_faction_name('ios_app_s039_merge');
		call merge_conflict_faction_name('ios_app_s040_merge');

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();

		# 在各个区上修改冲突玩家昵称并打印出冲突玩家列表：囧33区直接在玩家表上改，其他区拷贝一份玩家表，再在拷贝的表上改名
		call modify_player_name_and_print('ios_app_s033', '囧33区', 'players', '33');
		call cache_player('ios_app_s034', '囧34区', '34');
		call cache_player('ios_app_s035', '囧35区', '35');
		call cache_player('ios_app_s036', '囧36区', '36');
		call cache_player('ios_app_s037_merge', '囧37区', '37');
		call cache_player('ios_app_s038_merge', '囧38区', '38');
		call cache_player('ios_app_s039_merge', '囧39区', '39');
		call cache_player('ios_app_s040_merge', '囧40区', '40');

		# 在各个区上修改冲突门派名称并打印出冲突门派列表：囧33区直接在门派原表上改，其他区拷贝一份门派表，再在拷贝的表上改名
		call modify_faction_name_and_print('ios_app_s033', '囧33区', 'faction', '33');
		call cache_faction('ios_app_s034', '囧34区', '34');
		call cache_faction('ios_app_s035', '囧35区', '35');
		call cache_faction('ios_app_s036', '囧36区', '36');
		call cache_faction('ios_app_s037_merge', '囧37区', '37');
		call cache_faction('ios_app_s038_merge', '囧38区', '38');
		call cache_faction('ios_app_s039_merge', '囧39区', '39');
		call cache_faction('ios_app_s040_merge', '囧40区', '40');

		# 先分配好合区奖励
		call give_merge_gift(10033, 'ios_app_s033', '囧33区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10034, 'ios_app_s034', '囧34区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10035, 'ios_app_s035', '囧35区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10036, 'ios_app_s036', '囧36区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10037, 'ios_app_s037_merge', '囧37区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10038, 'ios_app_s038_merge', '囧38区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10039, 'ios_app_s039_merge', '囧39区', 'merge_gift_33_34_35_36_37_38_39_40');
		call give_merge_gift(10040, 'ios_app_s040_merge', '囧40区', 'merge_gift_33_34_35_36_37_38_39_40');

#正式开始合区，注意，这里只有被合到的区数据会受到改动，如2区、3区合到1区，则只有1区数据会增加------------------------------------------------------------
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s034', '囧34区', 34);
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s035', '囧35区', 35);
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s036', '囧36区', 36);
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s037_merge', '囧37区', 37);
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s038_merge', '囧38区', 38);
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s039_merge', '囧39区', 39);
		call merge_gamedb('ios_app_s033', '囧33区', 33, 'ios_app_s040_merge', '囧40区', 40);
	end if;
end
@@

delimiter ;
call merge_gamedb__33_34_35_36_37_38_39_40();
DROP PROCEDURE IF EXISTS merge_gamedb__33_34_35_36_37_38_39_40;
