<?php
/*
*  @Author: 洪坤安
*  @Created Time: 2014年8月1日
*/

	ini_set('max_execution_time', '60000');
 	date_default_timezone_set('PRC');
	ob_implicit_flush(true);

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