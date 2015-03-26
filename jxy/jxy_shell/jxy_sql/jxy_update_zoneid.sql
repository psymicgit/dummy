

select database() into @zone;
set @zoneid_str=substring(@zone, -3);
select @zoneid_str+0 into @zoneid;
select @zoneid;

SELECT count(*) from players where OriZoneID = 0;
update players set OriZoneID = @zoneid where OriZoneID = 0;
SELECT count(*) from players where OriZoneID = 0;

SELECT count(*) from goldrecord_2014 where ZoneID = 0;
update goldrecord_2014 set ZoneID = @zoneid where ZoneID = 0;
SELECT count(*) from goldrecord_2014 where ZoneID = 0;