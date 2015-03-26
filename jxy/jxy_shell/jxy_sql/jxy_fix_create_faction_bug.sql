drop PROCEDURE if exists `sp_CreateFaction`;

delimiter $$
CREATE PROCEDURE `sp_CreateFaction`(IN `v_ZoneID` int, IN `v_PlayerID` int,IN `v_FactionName` varchar(255),IN `v_FactionAnnouncement` varchar(255))
lable:
BEGIN
		DECLARE v_FactionID int UNSIGNED DEFAULT 0;
		DECLARE v_MaxFactionID  int UNSIGNED DEFAULT 0;
		declare v_ErrCode int default 0;  
		declare v_ErrStr varchar(120) default 'Success!';

		declare exit handler for sqlexception
		BEGIN
		 		set v_ErrCode = 99;
		 		set v_FactionID = 0;
		 		select v_ErrCode as errcode, v_FactionID as factionid;
		 		rollback;
		END;

		
		start transaction; 

		IF  EXISTS(select 1 from faction where FactionName = v_FactionName ) THEN
		BEGIN
					set v_ErrCode = 5;
					set v_FactionID = 0;
					select v_ErrCode as errcode, v_FactionID as factionid;
					rollback;
					leave lable;
		END;
		END IF;

		IF  EXISTS(select 1 from factionplayer where JoinPlayerID = v_PlayerID and JoinState = 0) THEN
		BEGIN
					set v_ErrCode = 2;
					set v_FactionID = 0;
					select v_ErrCode as errcode, v_FactionID as factionid;
					rollback;
					leave lable;
		END;
		END IF;


		
		select MAX(FactionID % 1000000) into v_MaxFactionID from faction;

		IF v_MaxFactionID is NULL THEN
			BEGIN
					set v_MaxFactionID = 0;
			END;
		END IF;

		
		set 	  v_MaxFactionID = v_MaxFactionID % 1000000;
		set 		v_FactionID = (v_ZoneID * 1000000) + v_MaxFactionID + 1;


		
		DELETE  from factionplayer where JoinPlayerID = v_PlayerID; 

		INSERT into faction(FactionID, FactionPlayerID, FactionName, FactionRank, FactionLevel, FactionNotice, FactionAnnouncement, FactionPlayerDataLst, FactionLogDataLst, FactionCreateTime, 
		RareItemRoomLevel, GymnasiumLevel, FactionFunds, FactionTaskSchedule, ShadeLevel, HotSpringLevel, ColdPrisonLevel, FactionTowerLevel, CreateFactionPlayerID, FactionFundsSum, UpdateTime) 
		VALUES(v_FactionID, v_PlayerID, v_FactionName, 0, 1, '', v_FactionAnnouncement, '', '', now(), 0, 0, 0, '', 0, 0, 0, 0, v_PlayerID, 0, NOW());

		INSERT into factionplayer(FactionID, JoinPlayerID, JobType, UnLoginBattleLog) VALUES (v_FactionID, v_PlayerID, 1, '');

		select v_ErrCode as errcode, v_FactionID as factionid;

		commit;   

END
$$
