
DROP PROCEDURE IF EXISTS merge_gamedb__17_18_19_20_21_22_23_24;

delimiter @@
CREATE PROCEDURE merge_gamedb__17_18_19_20_21_22_23_24()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	set @g_cur_db = 'ios_app_s017_merge';
	select database() into @g_cur_db;

	if @g_cur_db <> 'ios_app_s017_merge' then
		select concat('请在ios_app_s017_merge数据库下执行本脚本，当前数据库是', @g_cur_db);
		leave lable_1;
	end if;

	# 稍微检测一下两两合区时会不会碰到合表主键冲突（全部检测要 8 * 8 / 2 次，太多了）
	SET @v_is_merge_check_ok=1;
	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s018', '囧18区', 18, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧18区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s019', '囧19区', 19, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧19区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s020_merge', '囧20区', 20, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧20区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s021_merge', '囧21区', 21, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧21区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s022_merge', '囧22区', 22, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧22区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s023_merge', '囧23区', 23, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧23区的合区将发生冲突';
	end if;

	call merge_gamedb_check('ios_app_s017_merge', '囧17区', 17, 'ios_app_s024_merge', '囧24区', 24, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok as '检测到囧17区与囧24区的合区将发生冲突';
	end if;


	if @v_is_merge_check_ok = 1 then
		drop table if exists merge.merge_gift_17_18_19_20_21_22_23_24;
		
		# 创建合区奖励表，为各区的玩家都添加一份合区礼包
		create table if not exists merge.merge_gift_17_18_19_20_21_22_23_24 (
		  ActivityID smallint(6) NOT NULL DEFAULT '0' COMMENT '0:冲值奖励，1：评分奖励',
		  StartPlayerID int(11) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
		  EndPlayerID int(10) unsigned NOT NULL DEFAULT '0',
		  PRIMARY KEY (ActivityID,StartPlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

		# 生成出冲突玩家昵称列表
		call merge_conflict_player_name('ios_app_s017_merge');
		call merge_conflict_player_name('ios_app_s018');
		call merge_conflict_player_name('ios_app_s019');
		call merge_conflict_player_name('ios_app_s020_merge');
		call merge_conflict_player_name('ios_app_s021_merge');
		call merge_conflict_player_name('ios_app_s022_merge');
		call merge_conflict_player_name('ios_app_s023_merge');
		call merge_conflict_player_name('ios_app_s024_merge');

		# 生成冲突门派名称列表
		call merge_conflict_faction_name('ios_app_s017_merge');
		call merge_conflict_faction_name('ios_app_s018');
		call merge_conflict_faction_name('ios_app_s019');
		call merge_conflict_faction_name('ios_app_s020_merge');
		call merge_conflict_faction_name('ios_app_s021_merge');
		call merge_conflict_faction_name('ios_app_s022_merge');
		call merge_conflict_faction_name('ios_app_s023_merge');
		call merge_conflict_faction_name('ios_app_s024_merge');

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();

		# 在各个区上修改冲突玩家昵称并打印出冲突玩家列表：囧17区直接在玩家表上改，其他区拷贝一份玩家表，再在拷贝的表上改名
		call modify_player_name_and_print('ios_app_s017_merge', '囧17区', 'players', '17');
		call cache_player('ios_app_s018', '囧18区', '18');
		call cache_player('ios_app_s019', '囧19区', '19');
		call cache_player('ios_app_s020_merge', '囧20区', '20');
		call cache_player('ios_app_s021_merge', '囧21区', '21');
		call cache_player('ios_app_s022_merge', '囧22区', '22');
		call cache_player('ios_app_s023_merge', '囧23区', '23');
		call cache_player('ios_app_s024_merge', '囧24区', '24');

		# 在各个区上修改冲突门派名称并打印出冲突门派列表：囧17区直接在门派原表上改，其他区拷贝一份门派表，再在拷贝的表上改名
		call modify_faction_name_and_print('ios_app_s017_merge', '囧17区', 'faction', '17');
		call cache_faction('ios_app_s018', '囧18区', '18');
		call cache_faction('ios_app_s019', '囧19区', '19');
		call cache_faction('ios_app_s020_merge', '囧20区', '20');
		call cache_faction('ios_app_s021_merge', '囧21区', '21');
		call cache_faction('ios_app_s022_merge', '囧22区', '22');
		call cache_faction('ios_app_s023_merge', '囧23区', '23');
		call cache_faction('ios_app_s024_merge', '囧24区', '24');

		# 先分配好合区奖励
		call give_merge_gift(10017, 'ios_app_s017_merge', '囧17区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10018, 'ios_app_s018', '囧18区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10019, 'ios_app_s019', '囧19区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10020, 'ios_app_s020_merge', '囧20区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10021, 'ios_app_s021_merge', '囧21区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10022, 'ios_app_s022_merge', '囧22区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10023, 'ios_app_s023_merge', '囧23区', 'merge_gift_17_18_19_20_21_22_23_24');
		call give_merge_gift(10024, 'ios_app_s024_merge', '囧24区', 'merge_gift_17_18_19_20_21_22_23_24');

#正式开始合区，注意，这里只有被合到的区数据会受到改动，如2区、3区合到1区，则只有1区数据会增加------------------------------------------------------------
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s018', '囧18区', 18);
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s019', '囧19区', 19);
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s020_merge', '囧20区', 20);
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s021_merge', '囧21区', 21);
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s022_merge', '囧22区', 22);
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s023_merge', '囧23区', 23);
		call merge_gamedb('ios_app_s017_merge', '囧17区', 17, 'ios_app_s024_merge', '囧24区', 24);
	end if;
end
@@

delimiter ;
call merge_gamedb__17_18_19_20_21_22_23_24();
DROP PROCEDURE IF EXISTS merge_gamedb__17_18_19_20_21_22_23_24;
