delimiter $$
DROP PROCEDURE if exists merge_start_end_player_id;
CREATE PROCEDURE merge_start_end_player_id()
BEGIN
	DECLARE v_done INT DEFAULT 0;

	select count(*) as 合并前的记录数 from activityencourageplayer;

	update activityencourageplayer p1 inner join activityencourageplayer p2 on p1.ActivityID = p2.ActivityID and p2.StartPlayerID = p1.EndPlayerID + 1 and p2.StartPlayerID = p2.EndPlayerID set p1.EndPlayerID = p2.EndPlayerID, p2.EndPlayerID = 0;
	while exists(select p1.ActivityID from activityencourageplayer p1 inner join activityencourageplayer p2 on p1.ActivityID = p2.ActivityID and p2.StartPlayerID = p1.EndPlayerID + 1 limit 1) do
		update activityencourageplayer p1 inner join activityencourageplayer p2 on p1.ActivityID = p2.ActivityID and p2.StartPlayerID = p1.EndPlayerID + 1 and p2.EndPlayerID = 0 set p1.EndPlayerID = p2.StartPlayerID;
		delete p2 from activityencourageplayer p1 inner join activityencourageplayer p2 on p1.ActivityID = p2.ActivityID and p2.EndPlayerID = 0 and p2.StartPlayerID = p1.EndPlayerID;
	end while;
	delete from activityencourageplayer where EndPlayerID = 0;

	select count(*) as 合并后的记录数 from activityencourageplayer;
END
$$

delimiter ;
call merge_start_end_player_id();