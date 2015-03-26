<?php
	ini_set('max_execution_time', '60000');

	$ios_zones = array(
		1,   9,   17,  25,
		33,  45,  53,  61,
		69,  77,  85,  93,
		101, 109, 117, 125,
		133, 134, 135, 136,
		137, 138, 139, 140,
		141, 142, 143, 144,
		145, 146, 147
	);
/*
	$and_zones = array(
		1,  9,  17, 25,
		33, 37, 41, 45,
		49, 50, 51, 52,
		53, 54, 55, 56,
		57, 58, 59, 60,
		61, 62, 63, 64,
		65, 66, 67, 68,
		69, 70
	);
*/
	$and_zones = array(
		1, 
		2,  10, 18, 26,
		34, 38, 42, 46,
		50, 51, 52, 53,
		54, 55, 56, 57,
		58, 59, 60, 61,
		62, 63, 64, 65,
		66, 67, 68, 69,
		70, 71
	);
	
	function splice_zone_id($zones)
	{
		$result="";
		foreach ($zones as $zone) {
		    $result = $result.$zone.",";
		}

		$result = rtrim($result, ",");
		return $result;
	}

	function print_query_db($result)
	{
		echo '<font face="verdana">';
		echo '<table border="1" cellpadding="1" cellspacing="0">';

		// 显示字段名称
		echo "</b><tr></b>";
		for ($i=0; $i<mysql_num_fields($result); $i++)
		{
			echo '<td "><b>'.
			mysql_field_name($result, $i);
			echo "</b></td></b>";
		}
		echo "</tr></b>";

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		// 循环取出记录
		while ($row=mysql_fetch_row($result))
		{
			echo "<tr></b>";
			for ($i=0; $i<mysql_num_fields($result); $i++ )
			{
				echo '<td>';
				echo $row[$i];
				echo '</td>';
			}
			echo "</tr></b>";
		}

		echo "</table></b>";
		echo "</font>";
	}

	// 当期充值留存玩家的元宝充值和元宝获赠和元宝剩余
	function query_pay_alive_player($conn, $begin_date, $end_date) 
	{
		// $sql="select sum(rmb) * 10 as sumaddpaygold, sum(gold) as sumaddgold from payed where createtime >= '".$begin_date."' and createtime < '".$end_date."' and playerid in (select playerid from globalplayers where lastlogintime > '".$end_date."') and playerid not in (select PlayerID from mysql.test_player)";

		$sql="select sum(rmb) * 10 as sumaddpaygold, sum(gold) as sumaddgold from payed where createtime >= '".$begin_date."' and createtime < '".$end_date."' and playerid not in (select PlayerID from test_player)";

		// 执行sql查询
		$result=mysql_query($sql,$conn);

		$row=mysql_fetch_row($result);
		$sumaddpaygold=$row[0];
		$sumaddgold=$row[1];

		echo '<table border="1" cellpadding="1" cellspacing="0">';
		echo '<tr><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的元宝充值</td><td>'.$sumaddpaygold.'</td></tr>';
		echo '<tr><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的元宝充值+充值赠送元宝</td><td>'.($sumaddgold).'</td></tr>';
		echo '</table>';
	}

	function query_log($zoneid, $conn) 
	{
		// $sql="select sum(rmb) * 10 as sumaddpaygold, sum(gold) as sumaddgold from payed where createtime >= '".$begin_date."' and createtime < '".$end_date."' and playerid in (select playerid from globalplayers where lastlogintime > '".$end_date."') and playerid not in (select PlayerID from mysql.test_player)";

		$sql="select CurTableName from moduletablename order by ID desc limit 1";

		// 执行sql查询
		$result=mysql_query($sql,$conn);

		$row=mysql_fetch_row($result);
		$logtablename=$row[0];

		# $sql="select count(*) from ".$logtablename."";
		/*
		$sql="select record_1.PlayerID, record_1.cnt as '探索次数', record_2.cnt as '吞噬次数', record_2.devour_cnt as '吞噬战魂个数', record_2.devour_exp as '吞噬战魂获得经验'
from (select PlayerID, count(*) as cnt from ".$logtablename." WHERE OpType = 53 GROUP BY PlayerID) as record_1, (select PlayerID, count(*) as cnt, sum(Param1) as devour_cnt, sum(Param2) as devour_exp from ".$logtablename." WHERE ModuleType = 3 and OpType = 29 GROUP BY PlayerID) as record_2 where record_1.PlayerID = record_2.PlayerID and record_2.devour_cnt > (6 * record_1.cnt)";
		*/
		$sql = "select 
	record_1.PlayerID
	, record_1.cnt as '探索获得战魂数量'
	, record_2.cnt as '吞噬次数'
	, record_2.devour_cnt as '吞噬战魂个数'
	, record_2.devour_exp as '吞噬战魂获得经验'
 from 
		(select soul_val_1.PlayerID, (soul_val_1.val + soul_val_2.val) as cnt from (select PlayerID, count(*) as val from ".$logtablename." WHERE ModuleType = 3 and OpType =53 GROUP BY PlayerID) as soul_val_1
				, (select PlayerID, (10 * count(*) - sum(meterial.cnt)) as val from (SELECT PlayerID, count(*) as cnt from ".$logtablename." WHERE ModuleType = 14 and OpType = 32 GROUP BY PlayerID, OpTime) as meterial GROUP BY PlayerID) as soul_val_2
							where soul_val_1.PlayerID = soul_val_2.PlayerID) as record_1
		, (select PlayerID, count(*) as cnt, sum(Param1) as devour_cnt, sum(Param2) as devour_exp 
					from ".$logtablename." WHERE ModuleType = 3 and OpType = 29 GROUP BY PlayerID) as record_2
where record_1.PlayerID = record_2.PlayerID and record_2.devour_cnt > record_1.cnt; ";

		echo "<br/>区号：$zoneid";
		// echo $sql;

		// 执行sql查询
		$result=mysql_query($sql,$conn);
		print_query_db($result);
	}

	function query_gold($usercenter_mysql, $usr, $pwd, $usercenter, $zones)
	{
		$zones_str = splice_zone_id($zones);
		// echo $zones_str;

		// 执行sql查询
		$conn = mysql_connect($usercenter_mysql, $usr, $pwd);
		if (!$conn)
		{
			die('Could not connect: ' . mysql_error());
		}

		mysql_select_db($usercenter, $conn);

		echo "<br/>---------用户中心:".$usercenter."----------";

		// 执行sql查询
		$result=mysql_query("select * from ".$usercenter.".zonedb where ZoneID in (".$zones_str.") order by ZoneID asc", $conn);
		// print_query_db($result);

		// 获取查询结果
		// $row=mysql_fetch_row($result);

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		echo "<br/>-------------------";

		// 循环取出记录
		while ($row=mysql_fetch_array($result))
		{
			$zoneid=$row['ZoneID'];
			$logdb=$row['LogDb'];
			$logdbip=$row['LogDbIP'];
			$logdbport=63307;

			$logdb_conn = mysql_connect($logdbip.":".$logdbport, $usr, $pwd);
			if (!$logdb_conn)
			{
				die('Could not connect: ' . mysql_error());
			}

			mysql_select_db($logdb, $logdb_conn);
			query_log($zoneid, $logdb_conn);
		}

		// 释放资源
		mysql_free_result($result);

		// 关闭连接
		mysql_close($conn);	  
	}

	$ios_usercenter="118.26.225.102:63301";
	$and_usercenter="42.62.60.197:63301";

	$usr="root";
	$pwd="Maxnet.1234";

	query_gold($ios_usercenter, $usr, $pwd, "ios_app_usercenter", $ios_zones);
	query_gold($and_usercenter, $usr, $pwd, "and_app_usercenter", $and_zones);
?>