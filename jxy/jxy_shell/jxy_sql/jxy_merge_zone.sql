DROP PROCEDURE IF EXISTS merge_1_2_3_4_5_6_7_8;

delimiter $$
CREATE DEFINER='root'@'%' PROCEDURE merge_1_2_3_4_5_6_7_8()
lable_1:
BEGIN
	create database if not exists merge;
	drop table if exists merge.merge_encourageplayer;
	drop table if exists merge.conflict_player_name;
	drop table if exists merge.conflict_faction_name;

	SET @v_is_merge_check_ok=1;

	call merge_gamedb_check('merge001', '囧1区', 1, 'merge002', '囧2区', 2, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
		select @v_is_merge_check_ok;
	end if;

	call merge_gamedb_check('merge001', '囧1区', 1, 'merge003', '囧3区', 3, @v_is_merge_check_ok);
	if @v_is_merge_check_ok = 0 then
			select @v_is_merge_check_ok;
	end if;

	call merge_gamedb_check('merge001', '囧1区', 1, 'merge004', '囧4区', 4, @v_is_merge_check_ok);
	call merge_gamedb_check('merge001', '囧1区', 1, 'merge005', '囧5区', 5, @v_is_merge_check_ok);
	call merge_gamedb_check('merge001', '囧1区', 1, 'merge006', '囧6区', 6, @v_is_merge_check_ok);
	call merge_gamedb_check('merge001', '囧1区', 1, 'merge007', '囧7区', 7, @v_is_merge_check_ok);
	call merge_gamedb_check('merge001', '囧1区', 1, 'merge008', '囧8区', 8, @v_is_merge_check_ok);


	if @v_is_merge_check_ok = 1 then

		# 先把这些区的名字汇总起来
		call merge_conflict_player_name('merge001');
		call merge_conflict_player_name('merge002');
		call merge_conflict_player_name('merge003');
		call merge_conflict_player_name('merge004');
		call merge_conflict_player_name('merge005');
		call merge_conflict_player_name('merge006');
		call merge_conflict_player_name('merge007');
		call merge_conflict_player_name('merge008');

		call merge_conflict_faction_name('merge001');
		call merge_conflict_faction_name('merge002');
		call merge_conflict_faction_name('merge003');
		call merge_conflict_faction_name('merge004');
		call merge_conflict_faction_name('merge005');
		call merge_conflict_faction_name('merge006');
		call merge_conflict_faction_name('merge007');
		call merge_conflict_faction_name('merge008');

		# 仅保留冲突的玩家昵称和门派名称
		call erase_unique_player_name();
		call erase_unique_faction_name();

		# 打印出各区的冲突玩家列表（这个列表暂时没什么用，不过以防万一还是打印出来吧）
		call cache_conflict_player_name('merge001', '囧1区', 1);
		call cache_conflict_player_name('merge002', '囧2区', 2);
		call cache_conflict_player_name('merge003', '囧3区', 3);
		call cache_conflict_player_name('merge004', '囧4区', 4);
		call cache_conflict_player_name('merge005', '囧5区', 5);
		call cache_conflict_player_name('merge006', '囧6区', 6);
		call cache_conflict_player_name('merge007', '囧7区', 7);
		call cache_conflict_player_name('merge008', '囧8区', 8);

		# 打印出各区的冲突门派列表
		call cache_conflict_faction_name('merge001', '囧1区', 1);
		call cache_conflict_faction_name('merge002', '囧2区', 2);
		call cache_conflict_faction_name('merge003', '囧3区', 3);
		call cache_conflict_faction_name('merge004', '囧4区', 4);
		call cache_conflict_faction_name('merge005', '囧5区', 5);
		call cache_conflict_faction_name('merge006', '囧6区', 6);
		call cache_conflict_faction_name('merge007', '囧7区', 7);
		call cache_conflict_faction_name('merge008', '囧8区', 8);

		# 先分配好合区奖励
		call give_merge_award(10001, 'merge001', '囧1区');
		call give_merge_award(10002, 'merge002', '囧2区');
		call give_merge_award(10003, 'merge003', '囧3区');
		call give_merge_award(10004, 'merge004', '囧4区');
		call give_merge_award(10005, 'merge005', '囧5区');
		call give_merge_award(10006, 'merge006', '囧6区');
		call give_merge_award(10007, 'merge007', '囧7区');
		call give_merge_award(10008, 'merge008', '囧8区');

		# 正式开始合并
		# 1. 开始合并游戏库
		call merge_gamedb('merge001', '囧1区', 1, 'merge002', '囧2区', 2);
		call merge_gamedb('merge001', '囧1区', 1, 'merge003', '囧3区', 3);
		call merge_gamedb('merge001', '囧1区', 1, 'merge004', '囧4区', 4);
		call merge_gamedb('merge001', '囧1区', 1, 'merge005', '囧5区', 5);
		call merge_gamedb('merge001', '囧1区', 1, 'merge006', '囧6区', 6);
		call merge_gamedb('merge001', '囧1区', 1, 'merge007', '囧7区', 7);
		call merge_gamedb('merge001', '囧1区', 1, 'merge008', '囧8区', 8);

		# 1. 开始合并日志库
		call merge_logdb('merge001_log', '囧1区', 'merge002_log', '囧2区');
		call merge_logdb('merge001_log', '囧1区', 'merge003_log', '囧3区');
		call merge_logdb('merge001_log', '囧1区', 'merge004_log', '囧4区');
		call merge_logdb('merge001_log', '囧1区', 'merge005_log', '囧5区');
		call merge_logdb('merge001_log', '囧1区', 'merge006_log', '囧6区');
		call merge_logdb('merge001_log', '囧1区', 'merge007_log', '囧7区');
		call merge_logdb('merge001_log', '囧1区', 'merge008_log', '囧8区');

		use ios_app_pay;
		insert into activityencouragepro(ZoneID, ActivityID, ActivityType, ActivityName, ActivityDesc, JsonValues, IsNeedCdKey, StartTime, EndTime) values
		(1, 10001, 1, '合区馈赠大礼包', '合区大礼包：元宝20、铜钱100、阅历1', ),

	end if;

end
$$

delimiter ;
call merge_1_2_3_4_5_6_7_8();