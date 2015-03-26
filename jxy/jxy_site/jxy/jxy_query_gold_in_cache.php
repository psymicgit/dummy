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

	// 当期充值留存玩家的元宝充值
	function query_sum_pay_of_rmb_player($conn, $begin_date, $end_date) 
	{
		$sql="select sum(RMB) * 10 as sumaddpaygold from pay_list_2014 where DateProcess >= '".$begin_date."' and DateProcess < '".$end_date."'";

		// 执行sql查询
		$result=mysql_query($sql,$conn);

		$row=mysql_fetch_row($result);
		$sumaddpaygold=$row[0];

		echo '<table border="1" cellpadding="1" cellspacing="0">';
		echo '<tr><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的元宝充值</td><td>'.$sumaddpaygold.'</td></tr>';
		echo '</table>';
	}

	// 当期充值留存玩家的元宝充值
	function query_add_dec_gold_of_rmb_player($conn, $begin_date, $end_date) 
	{
		mysql_query("drop table if EXISTS tmp_rmb_player_cache",$conn);
		mysql_query("create table tmp_rmb_player_cache as select distinct(PlayerID) from pay_list_2014 where DateProcess >= '".$begin_date."' and DateProcess < '".$end_date."'");
		mysql_query("CREATE INDEX playerid_idx ON tmp_rmb_player_cache(PlayerID)",$conn);

		$sql="select sum(AddGold) as sumaddgold, sum(DecGold) as sumdecgold from rmb_player_dec_gold_2014_07_18 where CreateTime >= '".$begin_date."' and CreateTime < '".$end_date."' and PlayerID in (SELECT PlayerID from tmp_rmb_player_cache)";

		$result=mysql_query($sql,$conn);
		// echo $sql;

		mysql_query("drop table if EXISTS tmp_rmb_player_cache",$conn);

		// 执行sql查询
		$row=mysql_fetch_row($result);
		$sumaddgold=$row[0];
		$sumdecgold=$row[1];

		echo '<table border="1" cellpadding="1" cellspacing="0">';
		echo '<tr><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的元宝获赠</td><td>'.$sumaddgold.'</td><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的元宝消耗</td><td>'.$sumdecgold.'</td></tr>';
		//echo '<tr><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的元宝消耗</td><td>'.$sumdecgold.'</td></tr>';
		echo '</table>';
	}

	// 当期充值留存玩家元宝剩余
	function query_remain_gold_of_rmb_player($conn, $begin_date, $end_date) 
	{
		$sql="select sum(Gold) as sumremaingold from roll_back_player_gold_2014_07_18 where PlayerID in (select distinct(PlayerID) from pay_list_2014 where DateProcess >= '".$begin_date."' and DateProcess < '".$end_date."')";
		$result=mysql_query($sql,$conn);

		//echo $sql;

		// 执行sql查询
		$row=mysql_fetch_row($result);
		$sumremaingold=$row[0];

		echo '<table border="1" cellpadding="1" cellspacing="0">';
		echo '<tr><td>从'.$begin_date.'到'.$end_date.'当期留存玩家的剩余元宝</td><td>'.$sumremaingold.'</td></tr>';
		echo '</table>';
	}

	function query_gold($usercenter_mysql, $usr, $pwd, $usercenter, $paydb, $zones)
	{
		$zones_str = splice_zone_id($zones);
		// echo $zones_str;

		// 执行sql查询
		$conn = mysql_connect($usercenter_mysql, $usr, $pwd);
		if (!$conn)
		{
			die('Could not connect: ' . mysql_error());
		}

		mysql_select_db($paydb, $conn);

		echo "<br>---------用户中心:".$usercenter."----------";
		echo "<br>---------pay库:".$paydb."----------";

		query_remain_gold_of_rmb_player($conn, "2014-01-01", "2014-04-01");
		query_remain_gold_of_rmb_player($conn, "2014-02-01", "2014-05-01");
		query_remain_gold_of_rmb_player($conn, "2014-03-01", "2014-06-01");
		query_remain_gold_of_rmb_player($conn, "2014-04-01", "2014-07-01");

		echo "<br>---------元宝充值----------";
		query_sum_pay_of_rmb_player($conn, "2014-01-01", "2014-04-01");
		query_sum_pay_of_rmb_player($conn, "2014-02-01", "2014-05-01");
		query_sum_pay_of_rmb_player($conn, "2014-03-01", "2014-06-01");
		query_sum_pay_of_rmb_player($conn, "2014-04-01", "2014-07-01");

		query_sum_pay_of_rmb_player($conn, "2014-05-01", "2014-07-01");
		query_sum_pay_of_rmb_player($conn, "2014-06-01", "2014-07-01");

		echo "<br>---------元宝获赠和消耗----------";
		query_add_dec_gold_of_rmb_player($conn, "2014-01-01", "2014-04-01");
		query_add_dec_gold_of_rmb_player($conn, "2014-02-01", "2014-05-01");
		query_add_dec_gold_of_rmb_player($conn, "2014-03-01", "2014-06-01");
		query_add_dec_gold_of_rmb_player($conn, "2014-04-01", "2014-07-01");

		query_add_dec_gold_of_rmb_player($conn, "2014-05-01", "2014-07-01");
		query_add_dec_gold_of_rmb_player($conn, "2014-06-01", "2014-07-01");
	}

	$ios_usercenter="118.26.225.102:63301";
	$and_usercenter="42.62.60.197:63301";

	$usr="root";
	$pwd="Maxnet.1234";

	query_gold($ios_usercenter, $usr, $pwd, "ios_app_usercenter", "ios_app_pay", $ios_zones);
	query_gold($and_usercenter, $usr, $pwd, "and_app_usercenter", "and_app_pay", $and_zones);
?>