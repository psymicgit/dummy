<!DOCTYPE html>
<html lang="zh-CN">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=7" />

	<title>囧西游各区执行sql页面</title>

	<link href="./css/jxy.css" rel="stylesheet" />
	<script type="text/javascript" src="js/jquery-1.6.4.min.js"></script>
	<script type="text/javascript" src="js/t.core.js"></script>
	<script type="text/javascript" src="js/t.pages.js"></script>
</head>
<body>

<?php
/*
*  @Author: 洪坤安
*  @Company: heihuo
*  @brief: 囧西游各区执行sql脚本页面
*  @Created Time: 2014年7月29日
*/
	require('jxy_cfg.php');

	$_sql="";
	if(isset($_POST['sql'])){
		$_sql=$_POST['sql'];
	}

	$_db="dummy";
	if(isset($_POST['db'])){
		$_db=$_POST['db'];
	}

	$_platforms=array("ios", "android");
	if(isset($_POST['platforms'])){
		$_platforms = $_POST['platforms'];
	}

	$_loglevel="3";
	if(isset($_POST['loglevel'])){
		$_loglevel=$_POST['loglevel'];
	}
?>

<div class="container">
	<div class="crumb">
		<div class="info_detail srch_mod01" "float:right">
				<ul>
					<li><label><b class="num">本页面功能：</b> </label>导出元宝记录</li>
				</ul>
		</div>

		<div class="srch_mod01">
			<form action="jxy_query_gold.php" method="post">				
				<p></p>
				<ul><li><div>

				</div></li></ul>
				<div style="float:left;width:90%">
					<div style="float:left;">
						<label><input type="radio" name="db" <?php if($_db=="dummy")echo "checked"; ?>  value="dummy">无效</label>
						<label><input type="radio" name="db" <?php if($_db=="export_add_dec_gold")echo "checked"; ?>  value="export_add_dec_gold">导出：充值留存玩家的元宝获赠和消耗</label>
						<label><input type="radio" name="db" <?php if($_db=="query_add_dec_gold")echo "checked"; ?>  value="query_add_dec_gold">查询：充值留存玩家的元宝获赠和消耗</label>
						<label><input type="radio" name="db" <?php if($_db=="query_pay_gold")echo "checked"; ?> value="query_pay_gold">查询：充值留存玩家的元宝充值</label>
					</div>
				</div>				
				<br/>
				<br/>
				<ul><li class="btn_box"><span class="btn01"><input type="submit" value="开始执行" class="btn_srch" name="submit"></span></li></ul>
			</form>
		</div>
	</div>
<?php
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
		echo "\n";	
		echo '<div class="data01">';
		echo "\n";
		echo '<table class="t01">';

		$num=mysql_num_rows($result);  //获取行数
		 
		if($num == '0'){
			return;
	    }

		// 显示字段名称
		echo "\n<thead><tr>";
		for ($i=0; $i<mysql_num_fields($result); $i++){
			echo '<th>'.
			mysql_field_name($result, $i);
			echo '</th>';
		}
		echo "</tr></thead>\n<tbody>";

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		// 循环取出记录
		while ($row=mysql_fetch_row($result)){
			echo "\n	<tr>";

			for ($i=0; $i<mysql_num_fields($result); $i++ ){
				echo '<td>';
				echo $row[$i];
				echo '</td>';
			}
			echo "</tr>";
		}

		echo "\n</tbody>\n</table>";
		echo "\n";
		echo "</div>";
		echo '<hr/>';
	}

	function get_create_gold_table_sql($db, $gold_table_name)
	{
		$sql="
			use ".$db.";

			drop table if exists ".$gold_table_name.";
			create table if not exists ".$gold_table_name."(
				CreateTime date NOT NULL,
				PlayerID int(11) unsigned not null comment '角色ID',
				AddGold int(11) not null comment '赠送元宝',
				DecGold int(11) not null comment '消耗元宝',
				primary key (CreateTime,PlayerID)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		return $sql;
	}

	function get_row_sql($result, $table_name, $file)
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

			if($cur_row % 1000 == 0){
				//echo "<br/>当前导出到第".$cur_row."条";
				fwrite($file, $sql);
				$sql="";
				ob_flush();
				flush();
			}
		}
		
		$sql=$sql.";\n";
		fwrite($file, $sql);
	}

	function each_zone_take_add_dec_gold($zoneid, $conn, $gamedb, $file, $table_name, $beg_date, $end_date)
	{
		mysql_select_db($gamedb, $conn);

		//$result=mysql_query("create index playerid_idx on player_consume(PlayerID);",$conn);

		$sql = "select c.CreateTime,c.PlayerID,c.AddGold,c.DecGold from player_consume c,players p where c.PlayerID = p.PlayerID and p.Pay > 0 and (c.AddGold > 0 or c.DecGold > 0) and c.CreateTime >= '".$beg_date."' and c.CreateTime < '".$end_date."'";

		echo "<br/>正在导出->区号：$zoneid ...";
		ob_flush();
		flush();

		// 执行sql查询
		$result=mysql_query($sql,$conn);
		$row=mysql_fetch_row($result);

		// echo $sql;

		// 执行sql查询
		$result=mysql_query($sql,$conn);
//		$rows_sql=get_row_sql($result, $table_name);
		get_row_sql($result, $table_name, $file);

		// echo "<br/>拼接字符串完毕...";
		ob_flush();
		flush();
//		fwrite($file, $rows_sql);

		// 释放资源
		mysql_free_result($result);

		// 关闭连接
		mysql_close($conn);

		ob_flush();
		flush();
	}

	function print_zone_list($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $except_zones, $file_name, $db_name, $table_name, $to_zone)
	{
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
		print_query_db($result);
	}

	function get_add_dec_gold_table_name($old_gold_table, $beg_date, $end_date)
	{
		$name=$old_gold_table."_from_".$beg_date."_to_".$end_date;
		$name=str_replace('-', '_', $name);
		return $name;
	}

	function save_gold($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $except_zones, $file_name, $db_name, $table_name, $to_zone, $beg_date, $end_date)
	{
		$table_name = get_add_dec_gold_table_name($table_name, $beg_date, $end_date);
		ob_flush();
		flush();

		$except_zones_str = splice_zone_id($except_zones);

		// 执行sql查询
		$conn = mysql_connect($usercenter_ip.":".$usercenter_port, $usr, $pwd);
		if (!$conn){
			die('Could not connect: ' . mysql_error());
		}

		mysql_select_db($usercenter, $conn);

		echo "<br/><font style=\"color:#0068B7\">---------用户中心:".$usercenter."----------</font>";
		echo "<br/><font style=\"color:#0068B7\">---------正在导出从".$beg_date."到".$end_date."的充值玩家获得元宝和充值玩家消耗元宝----------</font>";

		ob_flush();
		flush();

		// 执行sql查询
		$result=mysql_query("select * from ".$usercenter.".zonedb where ZoneID in (select distinct(curzoneid) from gamezones where starttime < NOW()) and ZoneID not in (".$except_zones_str.") and ZoneID < ".$to_zone." order by ZoneID asc", $conn);
		// print_query_db($result);

		// 获取查询结果
		// $row=mysql_fetch_row($result);

		$create_table_sql=get_create_gold_table_sql($db_name, $table_name);

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
			$gamedbport=63306;

			$gamedb_conn = mysql_connect($gamedbip.":".$gamedbport, $usr, $pwd);
			if (!$gamedb_conn)
			{
				die('Could not connect: ' . mysql_error());
			}

			each_zone_take_add_dec_gold($zoneid, $gamedb_conn, $gamedb, $file, $table_name, $beg_date, $end_date);
		}

		// 释放资源
		mysql_free_result($result);

		// 关闭连接
		mysql_close($conn);

		fwrite($file, "delete from ".$db_name.".".$table_name." where PlayerID not in (select playerid from ".$usercenter.".globalplayers);");

		$exec_statement="mysql -h".$usercenter_ip." -P".$usercenter_port." -u".$usr." -p".$pwd." ".$db_name." < ".$file_name."";
		exec($exec_statement);		
	}

	function query_add_dec_gold($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $except_zones, $to_zone, $audit_db_name, $gold_table_name, $beg_date, $end_date)
	{
		$gold_table_name = get_add_dec_gold_table_name($gold_table_name, $beg_date, $end_date);

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
		$result=mysql_query("select sum(AddGold) as '从".$beg_date."到".$end_date."的充值玩家获得元宝', sum(DecGold) as '从".$beg_date."到".$end_date."的充值玩家消耗元宝' from ".$audit_db_name.".".$gold_table_name." where PlayerID in (select distinct(playerid) from ".$usercenter.".payed where createtime < '".$end_date."' and rmb > 1);", $conn);

		print_query_db($result);
	}

	function query_pay_gold($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $beg_date, $end_date)
	{
		// 执行sql查询
		$conn = mysql_connect($usercenter_ip.":".$usercenter_port, $usr, $pwd);
		if (!$conn)
		{
			die('Could not connect: ' . mysql_error());
		}

		mysql_select_db($usercenter, $conn);

		echo "<br/>---------用户中心:".$usercenter."----------";

		// 执行sql查询
		$result=mysql_query("select sum(rmb) * 10 as '从".$beg_date."到".$end_date."的充值元宝' from ".$usercenter.".payed where createtime >= '".$beg_date."' and createtime < '".$end_date."' and rmb > 1", $conn);
		print_query_db($result);
	}

	function query_remain_gold($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $audit_db_name, $gold_table_name, $beg_date, $end_date)
	{
		// 执行sql查询
		$conn = mysql_connect($usercenter_ip.":".$usercenter_port, $usr, $pwd);
		if (!$conn){
			die('Could not connect: ' . mysql_error());
		}

		mysql_select_db($usercenter, $conn);

		echo "<br/>---------用户中心:".$usercenter."----------";

		// 执行sql查询
		$result=mysql_query("select sum(Gold) as '".$beg_date."至".$end_date."当期充值留存玩家的剩余元宝' from ".$audit_db_name.".".$gold_table_name." where gold_date='".$end_date."' and PlayerID in (select playerid from ".$usercenter.".payed p where p.createtime >= '".$beg_date."' and p.createtime < '".$end_date."' and p.rmb > 1)", $conn);
		print_query_db($result);
	}

	$usr="root";
	$pwd="Maxnet.1234";

	if ($_db=="dummy") {
		print_zone_list($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "ios_gold.sql", "goldaudit", "ios_add_dec_gold", "300");
		print_zone_list($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "and_gold.sql", "goldaudit", "and_add_dec_gold", "100");		
	}else if($_db=="export_add_dec_gold"){ 
		echo '<font style="color:#0068B7">';
		echo "开始导出元宝获赠和消耗数据......";
		echo "</font>";

		//save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "ios_add_dec_gold.sql", "goldaudit", "ios_add_dec_gold", "300", "2014-07-01", "2014-07-14");
		//save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "and_add_dec_gold.sql", "goldaudit", "and_add_dec_gold", "100", "2014-07-01", "2014-07-14");

		save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "ios_add_dec_gold.sql", "goldaudit", "ios_add_dec_gold", "300", "2014-07-01", "2014-10-01");
		save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "and_add_dec_gold.sql", "goldaudit", "and_add_dec_gold", "100", "2014-07-01", "2014-10-01");

		//save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "ios_add_dec_gold.sql", "goldaudit", "ios_add_dec_gold", "300", "2014-10-01", "2014-10-13");
		//save_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "and_add_dec_gold.sql", "goldaudit", "and_add_dec_gold", "100", "2014-10-01", "2014-10-13");

		echo '<font style="color:#0068B7">';
		echo "<br/>执行完毕.";
		echo "</font>";
	}else if($_db=="query_add_dec_gold"){ 
		query_add_dec_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "300", "goldaudit", "ios_add_dec_gold", "2014-07-01", "2014-07-14");
		query_add_dec_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "100", "goldaudit", "and_add_dec_gold", "2014-07-01", "2014-07-14");

		query_add_dec_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "300", "goldaudit", "ios_add_dec_gold", "2014-07-01", "2014-10-01");
		query_add_dec_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "100", "goldaudit", "and_add_dec_gold", "2014-07-01", "2014-10-01");

		query_add_dec_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_except_zones, "300", "goldaudit", "ios_add_dec_gold", "2014-10-01", "2014-10-13");
		query_add_dec_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_except_zones, "100", "goldaudit", "and_add_dec_gold", "2014-10-01", "2014-10-13");

		query_pay_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", "2014-07-01", "2014-07-14");
		query_pay_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", "2014-07-01", "2014-07-14");

		query_pay_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", "2014-07-01", "2014-10-01");
		query_pay_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", "2014-07-01", "2014-10-01");

		query_pay_gold($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", "2014-10-01", "2014-10-13");
		query_pay_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", "2014-10-01", "2014-10-13");

		query_remain_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", "goldaudit", "ios_gold", "2014-07-01", "2014-10-01");
		query_remain_gold($ios_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", "goldaudit", "and_gold", "2014-07-01", "2014-10-01");

		echo '<br/><font style="color:#0068B7">';
		echo "执行完毕.";
		echo "</font>";
	}else if($_db=="query_pay_gold"){ 


		echo '<br/><font style="color:#0068B7">';
		echo "执行完毕.";
		echo "</font>";
	}
?>

<footer style="margin-top: 80px;">
	<hr>
    <p style="float:left;">Copyright &copy; 2014 Power by <a href="" target="_blank" style="color:#999999">黑火信息科技有限公司</a></p>
</footer>
</div>
</body>
</html>
<div style="margin:10px auto;text-align:center;"><?php echo "当前时间：".date('Y年m月d日 H时i分s秒',time()); ?></div>