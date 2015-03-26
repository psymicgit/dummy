
DROP PROCEDURE IF EXISTS merge_gamedb__1_2_3_4_5_6_7_8;

delimiter @@
CREATE PROCEDURE merge_gamedb__1_2_3_4_5_6_7_8()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	set @g_cur_db = 'ios_app_s001';
	select database() into @g_cur_db;

	if @g_cur_db <> 'ios_app_s001' then
		select concat('请在ios_app_s001数据库下执行本脚本，当前数据库是', @g_cur_db);
		leave lable_1;
	end if;

	# 稍微检测一下两两合区时会不会碰到合表主键冲突（全部检测要 8 * 8 / 2 次，太多了）
	SET @v_is_merge_check_ok=1;
	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s002', '囧2区', 2, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧2区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s003', '囧3区', 3, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧3区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s004', '囧4区', 4, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧4区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s005', '囧5区', 5, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧5区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s006', '囧6区', 6, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧6区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s007_merge', '囧7区', 7, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧7区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s001', '囧1区', 1, 'ios_app_s008_merge', '囧8区', 8, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧1区与囧8区的合区将发生冲突';
	end if;


	if @v_is_merge_check_ok = 1 then
		drop table if exists merge.merge_gift_1_2_3_4_5_6_7_8;
		
		# 创建合区奖励表，为各区的玩家都添加一份合区礼包
		create table if not exists merge.merge_gift_1_2_3_4_5_6_7_8 (
		  ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '0:冲值奖励，1：评分奖励',
		  StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		  EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		  PRIMARY KEY (ActivityID,StartPlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

		# 生成出冲突玩家昵称列表
		call merge_conflict_player_name('ios_app_s001');
		call merge_conflict_player_name('ios_app_s002');
		call merge_conflict_player_name('ios_app_s003');
		call merge_conflict_player_name('ios_app_s004');
		call merge_conflict_player_name('ios_app_s005');
		call merge_conflict_player_name('ios_app_s006');
		call merge_conflict_player_name('ios_app_s007_merge');
		call merge_conflict_player_name('ios_app_s008_merge');

		# 生成冲突门派名称列表
		call merge_conflict_faction_name('ios_app_s001');
		call merge_conflict_faction_name('ios_app_s002');
		call merge_conflict_faction_name('ios_app_s003');
		call merge_conflict_faction_name('ios_app_s004');
		call merge_conflict_faction_name('ios_app_s005');
		call merge_conflict_faction_name('ios_app_s006');
		call merge_conflict_faction_name('ios_app_s007_merge');
		call merge_conflict_faction_name('ios_app_s008_merge');

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();

		# 在各个区上修改冲突玩家昵称并打印出冲突玩家列表：囧1区直接在玩家表上改，其他区拷贝一份玩家表，再在拷贝的表上改名
		call modify_player_name_and_print('ios_app_s001', '囧1区', 'players', '1');
		call cache_player('ios_app_s002', '囧2区', '2');
		call cache_player('ios_app_s003', '囧3区', '3');
		call cache_player('ios_app_s004', '囧4区', '4');
		call cache_player('ios_app_s005', '囧5区', '5');
		call cache_player('ios_app_s006', '囧6区', '6');
		call cache_player('ios_app_s007_merge', '囧7区', '7');
		call cache_player('ios_app_s008_merge', '囧8区', '8');

		# 在各个区上修改冲突门派名称并打印出冲突门派列表：囧1区直接在门派原表上改，其他区拷贝一份门派表，再在拷贝的表上改名
		call modify_faction_name_and_print('ios_app_s001', '囧1区', 'faction', '1');
		call cache_faction('ios_app_s002', '囧2区', '2');
		call cache_faction('ios_app_s003', '囧3区', '3');
		call cache_faction('ios_app_s004', '囧4区', '4');
		call cache_faction('ios_app_s005', '囧5区', '5');
		call cache_faction('ios_app_s006', '囧6区', '6');
		call cache_faction('ios_app_s007_merge', '囧7区', '7');
		call cache_faction('ios_app_s008_merge', '囧8区', '8');

		# 先分配好合区奖励
		call give_merge_gift(10001, 'ios_app_s001', '囧1区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10002, 'ios_app_s002', '囧2区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10003, 'ios_app_s003', '囧3区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10004, 'ios_app_s004', '囧4区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10005, 'ios_app_s005', '囧5区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10006, 'ios_app_s006', '囧6区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10007, 'ios_app_s007_merge', '囧7区', 'merge_gift_1_2_3_4_5_6_7_8');
		call give_merge_gift(10008, 'ios_app_s008_merge', '囧8区', 'merge_gift_1_2_3_4_5_6_7_8');

#正式开始合区，注意，这里只有被合到的区数据会受到改动，如2区、3区合到1区，则只有1区数据会增加------------------------------------------------------------
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s002', '囧2区', 2);
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s003', '囧3区', 3);
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s004', '囧4区', 4);
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s005', '囧5区', 5);
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s006', '囧6区', 6);
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s007_merge', '囧7区', 7);
		call merge_gamedb('ios_app_s001', '囧1区', 1, 'ios_app_s008_merge', '囧8区', 8);
	end if;
end
@@

delimiter ;
call merge_gamedb__1_2_3_4_5_6_7_8();
DROP PROCEDURE IF EXISTS merge_gamedb__1_2_3_4_5_6_7_8;
