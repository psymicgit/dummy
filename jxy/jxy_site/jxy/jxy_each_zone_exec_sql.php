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

	$_db="master";
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
					<li><label><b class="num">本页面功能：</b> </label>在囧西游当前的各个区均执行sql语句</li>
				</ul>
				<ul>
					<li><label><b class="num">提示：</b> </label>查询日志库时，$logtable代表最新的日志表，如: select count(*) from $logtable;             </li>
				</ul>
		</div>

		<div class="srch_mod01">
			<form action="jxy_each_zone_exec_sql.php" method="post">				
				<h2>sql语句: </h2>
				<textarea style="width:99%" rows="30" name="sql"><?php echo $_sql; ?></textarea>

				<p></p>
				<ul><li><div>
			  	<label>ios平台:<input name="platforms[]" type="checkbox" <?php if(in_array("ios",$_platforms)){echo "checked";} ?> value="ios" /></label>
				<label>&nbsp&nbsp&nbsp&nbsp安卓平台:<input name="platforms[]" type="checkbox" <?php if(in_array("android",$_platforms)){echo "checked";} ?> value="android" /></label>
				</div></li></ul>
				<div style="float:left;width:90%">
					<div style="float:right;">
						<label><input type="radio" name="db" <?php if($_db=="master")echo "checked"; ?>  value="master">游戏主库</label>
						<label><input type="radio" name="db" <?php if($_db=="slave")echo "checked"; ?> value="slave">游戏从库</label>
						<label><input type="radio" name="db" <?php if($_db=="log")echo "checked"; ?> value="log">日志库</label>
					</div>					
				</div>
				<ul><li><div>
					<label>日志级别：</label>
						<div class="selewap1">
							<div class="selewap2">
								<select class="selebox" name="loglevel">
									<option <?php if("3"==$_loglevel){echo "selected";} ?> value="3">输出全部日志</option>
									<!-- <option <?php if("2"==$_loglevel){echo "selected";} ?> value="2">高（日志未排版）</option> -->
									<option <?php if("1"==$_loglevel){echo "selected";} ?> value="1">高（日志未排版）</option>
									<option <?php if("0"==$_loglevel){echo "selected";} ?> value="0">低（仅显示查询结果，不打印sql语句）</option>
								</select>
							</div>
						</div>
				</div></li></ul>
				<br/>
				<br/>
				<ul><li class="btn_box"><span class="btn01"><input type="submit" value="开始执行" class="btn_srch" name="submit"></span></li></ul>
			</form>
		</div>
	</div>


	<!--
	<div class="data01">
		<table class="t01">
			<thead><tr><th>ZoneID</th><th>GameDb</th><th>GameDbIP</th><th>GameDbPort</th><th>LogDb</th><th>LogDbIP</th><th>LogDbPort</th></tr></thead>
			<tbody>
				<tr><td>1</td><td>ios_app_s001</td><td>118.26.225.102</td><td>63316</td><td>ios_app_s001_log</td><td>118.26.225.102</td><td>63307</td></tr>
				<tr><td>9</td><td>ios_app_s009</td><td>118.26.225.98</td><td>63316</td><td>ios_app_s009_log</td><td>118.26.225.98</td><td>63307</td></tr>
				<tr><td>17</td><td>ios_app_s017</td><td>118.26.225.98</td><td>63316</td><td>ios_app_s017_log</td><td>118.26.225.98</td><td>63307</td></tr>
			</tbody>
		</table>
	</div>
	-->
<?php
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

	function get_mysql_option_by_loglevel($loglevel)
	{
		$option="";
		switch ($loglevel) {
		case 3:
			$option="-vvv";
			break;

		case 2:
			$option="-vv";
			break;

		case 1:
			$option="-v";
			break;
		
		default:
			# code...
			break;
		}

		return $option;
	}

	function each_zone_exec_sql($usercenter_ip, $usercenter_port, $usr, $pwd, $usercenter, $zones, $db_type, $sql, $sql_file, $loglevel)
	{
		$zones_str = splice_zone_id($zones);

		// 执行sql查询
		$conn = mysql_connect($usercenter_ip.":".$usercenter_port, $usr, $pwd);
		if (!$conn)
		{
			die('Could not connect: ' . mysql_error() .$usercenter_ip.":".$usercenter_port.$usr.$pwd);
		}

		mysql_select_db($usercenter, $conn);
		
		echo '<font style="color:#0068B7">';
		echo "<br/>---------用户中心:".$usercenter."----------";
		echo "</font>";

		// echo "<br/>---------用户中心:".$usercenter."----------";	

		// 执行sql查询
		// $result=mysql_query("select * from ".$usercenter.".zonedb where ZoneID in (".$zones_str.") order by ZoneID asc", $conn);
		$result=mysql_query("select * from ".$usercenter.".zonedb where ZoneID in (select distinct(curzoneid) from gamezones) order by ZoneID asc", $conn);
		print_query_db($result);

		// 定位到第一条记录
		mysql_data_seek($result, 0);

		// 循环取出记录
		while ($row=mysql_fetch_array($result))
		{
			$zoneid=$row['ZoneID'];
			$gamedb=$row['GameDb'];
			$gamedbip=$row['GameDbIP'];
			$logdb=$row['LogDb'];
			$logdbip=$row['LogDbIP'];

			$gamedbport=63306;

			$db="";
			$dbip="";
			$dbport="";
			$dbdesc="";

			switch ($db_type){
			case "master":
				$db=$gamedb;
				$dbip=$gamedbip;
				$dbport=63306;
				$dbdesc="游戏主库";
				break;

			case "slave":
				$db=$gamedb;
				$dbip=$gamedbip;
				$dbport=63316;
				$dbdesc="游戏从库";
				break;

			case "log":
				$db=$logdb;
				$dbip=$logdbip;
				$dbport=63307;
				$dbdesc="日志库";
				break;
			}

			echo '<font style="color:#0068B7"><pre>';
			echo "区号：$zoneid     $dbdesc:$db  端口:$dbport  IP:$dbip";
			echo "</pre></font>";

			$db_conn = mysql_connect($dbip.":".$dbport, $usr, $pwd);
			if(!$db_conn)
			{
				die("Could not connect: ".mysql_error());
				// continue;
			}

			mysql_select_db($db, $db_conn);

			if($db_type=="log")
			{
				$select_newest_logtable_sql="select CurTableName from moduletablename order by ID desc limit 5";

				$table_names_result=mysql_query($select_newest_logtable_sql,$db_conn);

				// 定位到第一条记录
				mysql_data_seek($table_names_result, 0);

				// 循环取出记录
				while ($tablename_row=mysql_fetch_row($table_names_result))
				{
					$newest_logtablename=$tablename_row[0];
					$replaced_sql=str_replace("\$logtable", $newest_logtablename, $sql);

					$file = fopen($sql_file, 'w');
					fwrite($file, $replaced_sql);
					fclose($file);

					$log_option=get_mysql_option_by_loglevel($loglevel);
					$command="mysql $log_option -h$dbip -P$dbport -u$usr -p$pwd $db < $sql_file 2>&1";
					$output="";
					exec($command, $output, $ret);
					// echo "<br/>$sql";
					
					unset($output[0]); 
					$output_str = implode('<br/>', $output);
					
					if(strstr($output_str,"Empty set")){
						echo "<div class=\"crumb\"><pre></pre></div>";
					}else{
						if($ret > 0){
							echo "<div class=\"crumb\"><font style=\"color:#f74330\"><pre>$output_str</pre></font></div>";
						}
						else {
							// echo "<div class=\"crumb\"><font style=\"color:#4a9705\"><pre>$output_str</pre></font></div>";
							echo "<div class=\"crumb\"><pre>$output_str</pre></div>";
						}

						ob_flush();
						flush();
					}
				}
			}else {
				$log_option=get_mysql_option_by_loglevel($loglevel);
				$command="mysql $log_option -h$dbip -P$dbport -u$usr -p$pwd $db < $sql_file 2>&1";
				$output="";
				exec($command, $output, $ret);
				// echo "<br/>$sql";
				
				unset($output[0]); 
				$output_str = implode('<br/>', $output);
				
				if($ret > 0){
					echo "<div class=\"crumb\"><font style=\"color:#f74330\"><pre>$output_str</pre></font></div>";
				}
				else {
					// echo "<div class=\"crumb\"><font style=\"color:#4a9705\"><pre>$output_str</pre></font></div>";
					echo "<div class=\"crumb\"><pre>$output_str</pre></div>";
				}

				ob_flush();
				flush();
			}
		}

		// 释放资源
		mysql_free_result($result);

		// 关闭连接
		mysql_close($conn);
	}

	$usr="root";
	$pwd="Maxnet.1234";

	if ($_sql=="") {
		
	}else{
		// $sql_file=date('y_m_d_h_i_s',time()).".sql";
		$sql_file=date('y_m_d',time()).".sql";

		$file = fopen($sql_file, 'w');
		fwrite($file, $_sql);
		fclose($file);

		if(isset($_POST['platforms'])){
			foreach ($_platforms as $platform){
				if($platform=="ios"){
					each_zone_exec_sql($ios_usercenter_ip, 63301, $usr, $pwd, "ios_app_usercenter", $ios_zones, $_db, $_sql, $sql_file, $_loglevel);
				} else if($platform=="android"){
					each_zone_exec_sql($and_usercenter_ip, 63301, $usr, $pwd, "and_app_usercenter", $and_zones, $_db, $_sql, $sql_file, $_loglevel);
				}
			}
		}

		echo '<font style="color:#0068B7">';
		echo "执行完毕.";
		echo "</font>";
	}
?>

<footer style="margin-top: 80px;">
	<hr>
    <p style="float:left;">Copyright &copy; 2014 Power by <a href="" target="_blank" style="color:#999999">黑火信息科技有限公司</a></p>
    <!--<p style="float:right"><?php echo "当前时间：".date('Y年m月d日 H时i分s秒',time()); ?></p>-->
</footer>
</div>
</body>
</html>
<div style="margin:10px auto;text-align:center;"><?php echo "当前时间：".date('Y年m月d日 H时i分s秒',time()); ?></div>