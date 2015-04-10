<?php
/*
*  @Author: 洪坤安
*  @Company: heihuo
*  @brief: 囧西游导出剩余元宝脚本
*  @Created Time: 2014年7月29日
*/

	ini_set('max_execution_time', '60000');

	$ios_except_zones = array(
		254, 255
	);

	$and_except_zones = array(
		255
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
		echo '<font size="3" face="verdana">';
		echo '<table border="1" cellpadding="1" cellspacing="0">';

		$num=mysql_num_rows($result);  //获取行数
		 
		if($num == '0'){
			return;
	    }

		// 显示字段名称
		echo "</b><tr></b>";
		for ($i=0; $i<mysql_num_fields($result); $i++){
			echo '<td "><b>'.
			mysql_field_name($result, $i);
			echo "</b></td></b>";
		}
		echo "</tr></b>";

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		// 循环取出记录
		while ($row=mysql_fetch_row($result)){
			echo "<tr></b>";

			for ($i=0; $i<mysql_num_fields($result); $i++ ){
				echo '<td>';
				echo $row[$i];
				echo '</td>';
			}
			echo "</tr></b>";
		}

		echo "</table></b>";
		echo "</font>";
	}

	function get_create_table_sql($db, $table_name)
	{
$sql="
	use ".$db.";

create table if not exists ".$table_name."(
	gold_date date not null comment '角色ID',
	PlayerID int(11) not null comment '角色ID',
	Gold int(11) not null comment '元宝',
	RMBGold int(11) not null comment '充值元宝',
	primary key (gold_date, PlayerID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;\n\n";
			
		// create index playerid_idx on rmb_player_gold;
		return $sql;
	}

	function get_row_sql($result, $table_name)
	{
		$sql="insert ignore into ".$table_name."(";

		$num_fields=mysql_num_fields($result);
		for($i=0; $i<$num_fields; $i++)
		{
			$sql=$sql."`".mysql_field_name($result, $i)."`";
			if ($i < $num_fields - 1) {
				$sql=$sql.",";
			}
		}
		
		$sql=$sql.") values ";
		
		$num_rows=mysql_num_rows($result);  //获取行数
		if ($num_rows==0) {
			return;
		}

		$cur_row=1;

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		// 循环取出记录
		while ($row=mysql_fetch_row($result)){
			$cur_row++;

			$sql=$sql."(";

			for ($i=0; $i<$num_fields; $i++ ){
				$sql=$sql."'".$row[$i]."'";
				if ($i < $num_fields - 1) {
					$sql=$sql.",";
				}
			}
			
			$sql=$sql.")";

			if($cur_row <= $num_rows){
				$sql=$sql.",";
			}
		}
		
		$sql=$sql.";\n";
		return $sql;
	}

	function take_remain_gold($zoneid, $conn, $file, $table_name)
	{
		// $yesterday=date("Y-m-d",strtotime("-1 day"));
		$today=date("Y-m-d");

		$sql = "select '".$today."' as gold_date, PlayerID, Gold, RMBGold from players where Pay > 0 and Gold > 0";

		// $sql = "select count(*) from players where Gold > 0";

		// 执行sql查询
		$result=mysql_query($sql,$conn);
		$row=mysql_fetch_row($result);

		echo "<br/>区号：$zoneid";
		// echo $sql;

		// 执行sql查询
		$result=mysql_query($sql,$conn);
		$rows_sql=get_row_sql($result, $table_name);

		fwrite($file, $rows_sql);

		// print_query_db($result);

		// 释放资源
		mysql_free_result($result);

		// 关闭连接
		mysql_close($conn);
	}

	function save_gold($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $except_zones, $file_name, $db_name, $table_name, $to_zone)
	{
 		date_default_timezone_set('PRC');

		$except_zones_str = splice_zone_id($except_zones);

		// 执行sql查询
		$conn = mysql_connect($usercenter_ip.":".$usercenter_port, $usr, $pwd);
		if (!$conn)
		{
			die('Could not connect: ' . mysql_error());
		}

		mysql_select_db($usercenter, $conn);

		echo "<br/>---------用户中心:".$usercenter."----------";

		// 执行sql查询
		$result=mysql_query("select * from ".$usercenter.".zonedb where ZoneID in (select distinct(curzoneid) from gamezones where starttime < NOW()) and ZoneID not in (".$except_zones_str.") and ZoneID < ".$to_zone." order by ZoneID asc", $conn);
		// print_query_db($result);

		// 获取查询结果
		// $row=mysql_fetch_row($result);

		$create_table_sql=get_create_table_sql($db_name, $table_name);

		// echo "<br/>".$create_table_sql;

		$file = fopen($file_name, 'w');
		fwrite($file, $create_table_sql);

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		echo "<br/>-------------------";

		// 循环取出记录
		while ($row=mysql_fetch_array($result))
		{
			$zoneid=$row['ZoneID'];
			$gamedb=$row['GameDb'];
			$gamedbip=$row['GameDbIP'];
			$gamedbport=63316;

			$gamedb_conn = mysql_connect($gamedbip.":".$gamedbport, $usr, $pwd);
			if (!$gamedb_conn)
			{
				die('Could not connect: ' . mysql_error());
			}

			mysql_select_db($gamedb, $gamedb_conn);
			take_remain_gold($zoneid, $gamedb_conn, $file, $table_name);
		}

		// 释放资源
		mysql_free_result($result);

		// 关闭连接
		mysql_close($conn);

		fwrite($file, "delete from ".$db_name.".".$table_name." where PlayerID not in (select playerid from ".$usercenter.".globalplayers);");

		$exec_statement="mysql -h".$usercenter_ip." -P".$usercenter_port." -u".$usr." -p".$pwd." ".$db_name." < ".$file_name."";
		// echo "<br/>$exec_statement";
		exec($exec_statement);
	}

	$ios_usercenter_ip="118.26.225.102";
	$and_usercenter_ip="42.62.60.197";

	$usr="root";
	$pwd="Maxnet.1234";

	// chmod(dirname(__FILE__), 0777);

	save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "ios_gold.sql", "goldaudit", "ios_gold", "300");
	save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "and_gold.sql", "goldaudit", "and_gold", "100");
?>