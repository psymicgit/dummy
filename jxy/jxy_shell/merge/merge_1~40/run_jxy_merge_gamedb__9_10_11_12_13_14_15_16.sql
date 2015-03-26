
DROP PROCEDURE IF EXISTS merge_gamedb__9_10_11_12_13_14_15_16;

delimiter @@
CREATE PROCEDURE merge_gamedb__9_10_11_12_13_14_15_16()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	set @g_cur_db = 'ios_app_s009';
	select database() into @g_cur_db;

	if @g_cur_db <> 'ios_app_s009' then
		select concat('请在ios_app_s009数据库下执行本脚本，当前数据库是', @g_cur_db);
		leave lable_1;
	end if;

	# 稍微检测一下两两合区时会不会碰到合表主键冲突（全部检测要 8 * 8 / 2 次，太多了）
	SET @v_is_merge_check_ok=1;
	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s010', '囧10区', 10, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧10区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s011', '囧11区', 11, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧11区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s012', '囧12区', 12, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧12区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s013', '囧13区', 13, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧13区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s014', '囧14区', 14, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧14区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s015', '囧15区', 15, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧15区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s009', '囧9区', 9, 'ios_app_s016', '囧16区', 16, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧9区与囧16区的合区将发生冲突';
	end if;


	if @v_is_merge_check_ok = 1 then
		drop table if exists merge.merge_gift_9_10_11_12_13_14_15_16;
		
		# 创建合区奖励表，为各区的玩家都添加一份合区礼包
		create table if not exists merge.merge_gift_9_10_11_12_13_14_15_16 (
		  ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '0:冲值奖励，1：评分奖励',
		  StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		  EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		  PRIMARY KEY (ActivityID,StartPlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

		# 生成出冲突玩家昵称列表
		call merge_conflict_player_name('ios_app_s009');
		call merge_conflict_player_name('ios_app_s010');
		call merge_conflict_player_name('ios_app_s011');
		call merge_conflict_player_name('ios_app_s012');
		call merge_conflict_player_name('ios_app_s013');
		call merge_conflict_player_name('ios_app_s014');
		call merge_conflict_player_name('ios_app_s015');
		call merge_conflict_player_name('ios_app_s016');

		# 生成冲突门派名称列表
		call merge_conflict_faction_name('ios_app_s009');
		call merge_conflict_faction_name('ios_app_s010');
		call merge_conflict_faction_name('ios_app_s011');
		call merge_conflict_faction_name('ios_app_s012');
		call merge_conflict_faction_name('ios_app_s013');
		call merge_conflict_faction_name('ios_app_s014');
		call merge_conflict_faction_name('ios_app_s015');
		call merge_conflict_faction_name('ios_app_s016');

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();

		# 在各个区上修改冲突玩家昵称并打印出冲突玩家列表：囧9区直接在玩家表上改，其他区拷贝一份玩家表，再在拷贝的表上改名
		call modify_player_name_and_print('ios_app_s009', '囧9区', 'players', '9');
		call cache_player('ios_app_s010', '囧10区', '10');
		call cache_player('ios_app_s011', '囧11区', '11');
		call cache_player('ios_app_s012', '囧12区', '12');
		call cache_player('ios_app_s013', '囧13区', '13');
		call cache_player('ios_app_s014', '囧14区', '14');
		call cache_player('ios_app_s015', '囧15区', '15');
		call cache_player('ios_app_s016', '囧16区', '16');

		# 在各个区上修改冲突门派名称并打印出冲突门派列表：囧9区直接在门派原表上改，其他区拷贝一份门派表，再在拷贝的表上改名
		call modify_faction_name_and_print('ios_app_s009', '囧9区', 'faction', '9');
		call cache_faction('ios_app_s010', '囧10区', '10');
		call cache_faction('ios_app_s011', '囧11区', '11');
		call cache_faction('ios_app_s012', '囧12区', '12');
		call cache_faction('ios_app_s013', '囧13区', '13');
		call cache_faction('ios_app_s014', '囧14区', '14');
		call cache_faction('ios_app_s015', '囧15区', '15');
		call cache_faction('ios_app_s016', '囧16区', '16');

		# 先分配好合区奖励
		call give_merge_gift(10009, 'ios_app_s009', '囧9区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10010, 'ios_app_s010', '囧10区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10011, 'ios_app_s011', '囧11区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10012, 'ios_app_s012', '囧12区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10013, 'ios_app_s013', '囧13区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10014, 'ios_app_s014', '囧14区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10015, 'ios_app_s015', '囧15区', 'merge_gift_9_10_11_12_13_14_15_16');
		call give_merge_gift(10016, 'ios_app_s016', '囧16区', 'merge_gift_9_10_11_12_13_14_15_16');

#正式开始合区，注意，这里只有被合到的区数据会受到改动，如2区、3区合到1区，则只有1区数据会增加------------------------------------------------------------
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s010', '囧10区', 10);
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s011', '囧11区', 11);
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s012', '囧12区', 12);
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s013', '囧13区', 13);
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s014', '囧14区', 14);
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s015', '囧15区', 15);
		call merge_gamedb('ios_app_s009', '囧9区', 9, 'ios_app_s016', '囧16区', 16);
	end if;
end
@@

delimiter ;
call merge_gamedb__9_10_11_12_13_14_15_16();
DROP PROCEDURE IF EXISTS merge_gamedb__9_10_11_12_13_14_15_16;
