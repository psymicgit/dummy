<!DOCTYPE html>
<html lang="zh-CN">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=7" />

	<title>囧西游各主机执行shell脚本</title>

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
*  @brief: 囧西游各主机执行shell脚本页面
*  @Created Time: 2014年7月31日
*/
	require('jxy_cfg.php');

	$_cmd="";
	
	if(isset($_POST['sql'])){
		$_cmd=$_POST['sql'];
	}

	$_platforms=array("ios", "android");
	if(isset($_POST['platforms'])){
		$_platforms = $_POST['platforms'];
	}
?>

<div class="container">
	<div class="crumb">
		<div class="info_detail srch_mod01" "float:right">
				<ul>
					<li><label><b class="num">本页面功能：</b> </label>在囧西游当前的各个服务器均执行shell脚本</li>
				</ul>
				<ul>
					<li><label><b class="num">提示：</b> </label></li>
				</ul>
		</div>

		<div class="srch_mod01">
			<form action="jxy_host.php" method="post" id="search-form">
				<h2>shell脚本: </h2>
				<textarea style="width:99%" rows="30" name="sql" id="cmd-textarea"><?php echo $_cmd; ?></textarea>

				<p></p>
				<ul><li><div>
			  	<label>ios平台:<input name="platforms[]" type="checkbox" <?php if(in_array("ios",$_platforms)){echo "checked";} ?> value="ios" /></label>
				<label>&nbsp&nbsp&nbsp&nbsp安卓平台:<input name="platforms[]" type="checkbox" <?php if(in_array("android",$_platforms)){echo "checked";} ?> value="android" /></label>
				</div></li></ul>

				<ul class="btn_box_op" name="logdec[]">
					<li>
						<a rel="1"  href="#" >查看服务器进程</a>
						<a rel="2"  href="#" >剩余内存空间</a>
						<a rel="3"  href="#" >查看磁盘资源</a>
						<a rel="4"  href="#" >cpu平均负载</a>
						<a rel="5"  href="#" >查看僵尸进程</a>
						<a rel="6"  href="#" >同步服务器时间</a>
						<a rel="7"  href="#" >查看防火墙</a>
					</li>
				</ul>
				<br/>
				<br/>
				<ul><li class="btn_box"><span class="btn01"><input type="submit" value="开始执行" class="btn_srch" name="submit"></span></li></ul>
			</form>			
		</div>
	</div>

	<script type="text/javascript">
		(function(){
			var cmds = [
				"",
				"/home/tool/myps.sh game",
				"free -mt", 
				"df -hT", 
				"iostat -m", 
				"ps -A -o stat,ppid,pid,cmd | grep -e '^[Zz]' | awk '{print $2}'", 
				"ntpdate time.nist.gov",
				"cat /etc/sysconfig/iptables"
			];　//创建一个数组并赋值
			var form = $("#search-form");

			for(var i=1;i<=cmds.length;i++){ 
				form.find("a[rel='"+i+"']").bind("click", function(){
					var link = $(this);
					var rel = link.attr("rel");

					$("#cmd-textarea").val(cmds[rel]);
				});
			}
		})();
	</script>

<?php
	function each_host_exec_cmd($hosts, $desc, $cmd)
	{
		foreach ($hosts as $ip => $info) {
    		$zone=$info[0];
    		$usr=$info[1];
    		$pwd=$info[2];

    		echo '<font style="color:#0068B7"><pre>';
			echo "$zone   IP:$ip";
			echo "</pre></font>";
    		
    		$connection = ssh2_connect($ip, 65522);
			if ($connection){
				if (ssh2_auth_password($connection, $usr, $pwd)) {
					// echo "连接服务器成功！\n";
				} else {
					echo "<font style=\"color:#f74330\"><pre>连接服务器失败：用户名或密码错误！</pre></font>";
					break;
				}
			}
			else{
			   die("无法连接到服务器...");
			}

			$stream = ssh2_exec($connection, $cmd);
			$stderr_stream = ssh2_fetch_stream($stream, SSH2_STREAM_STDERR);

			stream_set_blocking($stream, true);
			stream_set_blocking($stderr_stream, true);

			$output = stream_get_contents($stream);
			$err 	= stream_get_contents($stderr_stream);

			fclose($stream);
			fclose($stderr_stream);

			echo "<div class=\"crumb\"><pre>$output</pre><font style=\"color:#f74330\"><pre>$err</pre></font></div>";
			
			ob_flush();
			flush();
		}
	}

	if ($_cmd=="") {
		print_hosts($ios_hosts, "ios平台");
		print_hosts($and_hosts, "安卓平台");
	}else{
		if(isset($_POST['platforms'])){
			foreach ($_platforms as $platform){
				if($platform=="ios"){
					print_hosts($ios_hosts, "ios平台");
					each_host_exec_cmd($ios_hosts, "ios平台", $_cmd);
				} else if($platform=="android"){
					print_hosts($and_hosts, "安卓平台");
					each_host_exec_cmd($and_hosts, "安卓平台", $_cmd);
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
</footer>
</div>
</body>
</html>
<div style="margin:10px auto;text-align:center;"><?php echo "当前时间：".date('Y年m月d日 H时i分s秒',time()); ?></div>