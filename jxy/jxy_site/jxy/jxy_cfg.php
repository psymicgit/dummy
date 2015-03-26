<?php
/*
*  @Author: 洪坤安
*  @Company: heihuo
*  @brief: 囧西游后台通用配置
*  @Created Time: 2014年8月1日
*/

	ini_set('max_execution_time', '60000');
 	date_default_timezone_set('PRC');
	ob_implicit_flush(true);

	// ios主机列表
	$ios_hosts = array(
	    "118.26.225.98" => array("01~24区", "root", "o_39m#V", "cd /home/server/ios_app \n cat cp01-255.sh \n sh cp01-255.sh"),
	    "118.26.225.101" => array("25~44区", "root", "!QAZ234%TGB", "cd /home/server/ios_app \n cat cp20-36.sh \n sh cp20-36.sh"),
	    "118.26.225.100" => array("45~60区", "root", "@WSX345^YHN", "cd /home/server/ios_app \n cat cp.sh \n sh cp.sh"),
	    "118.26.225.99" => array("61~76区", "root", "!QAZ234%TGB", "cd /home/server/ios_app \n cat cp58-70.sh \n sh cp58-70.sh"),
	    "118.26.225.104" => array("77~92区", "root", "@WSX345^YHN", "cd /home/server/ios_app \n cat cp71-85.sh \n sh cp71-85.sh"),
	    "118.26.225.105" => array("93-100,155-162区", "root", "#EDC456&UJM", "cd /home/server/ios_app \n cat cp.sh \n sh cp.sh"),
	    "42.62.60.200" => array("101~116区", "root", "#EDC456&UJM", "cd /home/server/ios_app \n cat cp101-115.sh \n sh cp101-115.sh"),
	    "42.62.60.196" => array("117~132区", "root", "F7szqtxVYq", "cd /home/server/ios_app \n cat cp116-128.sh \n sh cp116-128.sh"),
	    "42.62.47.197" => array("133~143区", "root", "9NaM),ivyxCGir^", "cd /home/server/ios_app \n cat cp129-143.sh \n sh cp129-143.sh"),
	    "42.62.60.201" => array("144~150区", "root", "8g69ZXH7SC!BQqHOxsLD8TIa!wgfo1!M", "cd /home/server/ios_app \n cat cp144-150.sh \n sh cp144-150.sh"),
	);

	// 安卓主机列表
	$and_hosts = array(
	    "42.62.60.197" => array("01~16区", "root", "#EDC456&UJM", "cd /home/server/and_app \n cat cp01-10.sh \n sh cp01-10.sh"),
	    "42.62.60.199" => array("17~24区", "root", "@WSX345^YHN", "cd /home/server/and_app \n cat cp11-25.sh \n sh cp11-25.sh"),
	    "42.62.60.202" => array("25~40区", "root", "#EDC456&UJM", "cd /home/server/and_app \n cat cp26-40.sh \n sh cp26-40.sh"),
	    "42.62.47.199" => array("41~54区", "root", "(1CtaX/vLC%P/^s", "cd /home/server/and_app \n cat cp41-55.sh \n sh cp41-55.sh"),
	    "42.62.60.203" => array("55~70区", "root", "gPnkyrrl!HLQVBmakVlh!tY3UrdE4JqI", "cd /home/server/and_app \n cat cp56-70.sh \n sh cp56-70.sh"),
	);

	$ios_usercenter_ip="118.26.225.102";
	$and_usercenter_ip="118.26.225.102";

	function print_hosts($hosts, $desc)
	{
		echo '<font style="color:#0068B7">';
		echo "<br/>---------  $desc  ----------";
		echo "</font>";

		echo "\n";
		echo '<div class="data01">';
		echo "\n";
		echo '<table class="t01">';

		// 显示字段名称
		echo "\n<thead><tr>";
		echo '<th>区范围</th><th>IP</th>';
		echo "</tr></thead>\n<tbody>";

		foreach ($hosts as $ip => $info) {
			echo "\n	<tr>";
    		echo "<td>".$info[0]."</td>";
    		echo "<td>$ip</td>";
    		echo "</tr>";
		}

		echo "\n</tbody>\n</table>";
		echo "\n";
		echo "</div>";
		echo '<hr/>';

		ob_flush();
		flush();
	}
?>