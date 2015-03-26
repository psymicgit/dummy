
DROP PROCEDURE IF EXISTS merge_gamedb__25_26_27_28_29_30_31_32;

delimiter @@
CREATE PROCEDURE merge_gamedb__25_26_27_28_29_30_31_32()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	set @g_cur_db = 'ios_app_s025_merge';
	select database() into @g_cur_db;

	if @g_cur_db <> 'ios_app_s025_merge' then
		select concat('请在ios_app_s025_merge数据库下执行本脚本，当前数据库是', @g_cur_db);
		leave lable_1;
	end if;

	# 稍微检测一下两两合区时会不会碰到合表主键冲突（全部检测要 8 * 8 / 2 次，太多了）
	SET @v_is_merge_check_ok=1;
	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s026', '囧26区', 26, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧26区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s027', '囧27区', 27, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧27区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s028', '囧28区', 28, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧28区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s029', '囧29区', 29, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧29区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s030', '囧30区', 30, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧30区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s031', '囧31区', 31, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧31区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s025_merge', '囧25区', 25, 'ios_app_s032', '囧32区', 32, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧25区与囧32区的合区将发生冲突';
	end if;


	if @v_is_merge_check_ok = 1 then
		drop table if exists merge.merge_gift_25_26_27_28_29_30_31_32;
		
		# 创建合区奖励表，为各区的玩家都添加一份合区礼包
		create table if not exists merge.merge_gift_25_26_27_28_29_30_31_32 (
		  ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '0:冲值奖励，1：评分奖励',
		  StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		  EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		  PRIMARY KEY (ActivityID,StartPlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

		# 生成出冲突玩家昵称列表
		call merge_conflict_player_name('ios_app_s025_merge');
		call merge_conflict_player_name('ios_app_s026');
		call merge_conflict_player_name('ios_app_s027');
		call merge_conflict_player_name('ios_app_s028');
		call merge_conflict_player_name('ios_app_s029');
		call merge_conflict_player_name('ios_app_s030');
		call merge_conflict_player_name('ios_app_s031');
		call merge_conflict_player_name('ios_app_s032');

		# 生成冲突门派名称列表
		call merge_conflict_faction_name('ios_app_s025_merge');
		call merge_conflict_faction_name('ios_app_s026');
		call merge_conflict_faction_name('ios_app_s027');
		call merge_conflict_faction_name('ios_app_s028');
		call merge_conflict_faction_name('ios_app_s029');
		call merge_conflict_faction_name('ios_app_s030');
		call merge_conflict_faction_name('ios_app_s031');
		call merge_conflict_faction_name('ios_app_s032');

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();

		# 在各个区上修改冲突玩家昵称并打印出冲突玩家列表：囧25区直接在玩家表上改，其他区拷贝一份玩家表，再在拷贝的表上改名
		call modify_player_name_and_print('ios_app_s025_merge', '囧25区', 'players', '25');
		call cache_player('ios_app_s026', '囧26区', '26');
		call cache_player('ios_app_s027', '囧27区', '27');
		call cache_player('ios_app_s028', '囧28区', '28');
		call cache_player('ios_app_s029', '囧29区', '29');
		call cache_player('ios_app_s030', '囧30区', '30');
		call cache_player('ios_app_s031', '囧31区', '31');
		call cache_player('ios_app_s032', '囧32区', '32');

		# 在各个区上修改冲突门派名称并打印出冲突门派列表：囧25区直接在门派原表上改，其他区拷贝一份门派表，再在拷贝的表上改名
		call modify_faction_name_and_print('ios_app_s025_merge', '囧25区', 'faction', '25');
		call cache_faction('ios_app_s026', '囧26区', '26');
		call cache_faction('ios_app_s027', '囧27区', '27');
		call cache_faction('ios_app_s028', '囧28区', '28');
		call cache_faction('ios_app_s029', '囧29区', '29');
		call cache_faction('ios_app_s030', '囧30区', '30');
		call cache_faction('ios_app_s031', '囧31区', '31');
		call cache_faction('ios_app_s032', '囧32区', '32');

		# 先分配好合区奖励
		call give_merge_gift(10025, 'ios_app_s025_merge', '囧25区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10026, 'ios_app_s026', '囧26区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10027, 'ios_app_s027', '囧27区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10028, 'ios_app_s028', '囧28区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10029, 'ios_app_s029', '囧29区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10030, 'ios_app_s030', '囧30区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10031, 'ios_app_s031', '囧31区', 'merge_gift_25_26_27_28_29_30_31_32');
		call give_merge_gift(10032, 'ios_app_s032', '囧32区', 'merge_gift_25_26_27_28_29_30_31_32');

#正式开始合区，注意，这里只有被合到的区数据会受到改动，如2区、3区合到1区，则只有1区数据会增加------------------------------------------------------------
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s026', '囧26区', 26);
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s027', '囧27区', 27);
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s028', '囧28区', 28);
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s029', '囧29区', 29);
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s030', '囧30区', 30);
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s031', '囧31区', 31);
		call merge_gamedb('ios_app_s025_merge', '囧25区', 25, 'ios_app_s032', '囧32区', 32);
	end if;
end
@@

delimiter ;
call merge_gamedb__25_26_27_28_29_30_31_32();
DROP PROCEDURE IF EXISTS merge_gamedb__25_26_27_28_29_30_31_32;
