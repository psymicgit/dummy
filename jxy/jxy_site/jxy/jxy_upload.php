<!DOCTYPE HTML>
<html>

<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=7" />

	<title>jQuery+php实现ajax文件上传</title>
	<link rel="stylesheet" type="text/css" href="./css/upload.css" />
	<link href="./css/jxy.css" rel="stylesheet" />

	<style type="text/css">
		.demo p{line-height:32px}
		.btn{position: relative;overflow: hidden;margin-right: 4px;display:inline-block;*display:inline;padding:4px 10px 4px;font-size:14px;line-height:18px;*line-height:20px;color:#fff;text-align:center;vertical-align:middle;cursor:pointer;background-color:#5bb75b;border:1px solid #cccccc;border-color:#e6e6e6 #e6e6e6 #bfbfbf;border-bottom-color:#b3b3b3;-webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px;}
		.btn input {position: absolute;top: 0; right: 0;margin: 0;border: solid transparent;opacity: 0;filter:alpha(opacity=0); cursor: pointer;}
		.progress { position:relative; margin-left:100px; margin-top:-24px; width:200px;padding: 1px; border-radius:3px; display:none}
		.bar {background-color: green; display:block; width:0%; height:20px; border-radius: 3px; }
		.percent { position:absolute; height:20px; display:inline-block; top:3px; left:2%; color:#fff }
		.files{height:22px; line-height:22px; margin:10px 0}
		.delimg{margin-left:20px; color:#090; cursor:pointer}
	</style>

	<script type="text/javascript" src="./js/jquery-1.8.1.min.js"></script>
	<script type="text/javascript" src="./js/jquery-1.8.1.form.js"></script>
</head>

<body>
<?php
/*
*  @Author: 洪坤安
*  @Company: heihuo
*  @brief: 
*  @Created Time: 2014年8月1日
*/
	require('jxy_cfg.php');

	$_platforms=array("ios", "android");
	if(isset($_POST['platforms'])){
		$_platforms = $_POST['platforms'];
	}

	$_up_file="";
	if(isset($_POST['up_file'])){
		$_up_file = $_POST['up_file'];
	}

	$_svr_up_file="";
	if(isset($_POST['svr_up_file_name'])){
		$_svr_up_file = $_POST['svr_up_file_name'];
	}

	$_file_size="";
	if(isset($_POST['file_size'])){
		$_file_size = $_POST['file_size'];
	}	
?>

<div class="container">
	<div class="crumb">
		<div class="info_detail srch_mod01" "float:right">
				<ul>
					<li><label><b class="num">本页面功能：</b> </label>更新囧西游服务器gameserver、gateserver等</li>
				</ul>
				<ul>
					<li><label><b class="num">提示：</b> 上传服务器步骤 1.添加附件 2.开始上传</label></li>
				</ul>
		</div>
		<div id="main">
		   <div class="demo">
		   		<div class="btn">
		            <span>添加附件</span>
		            <input id="fileupload" type="file" name="mypic">
		        </div>
		        <div class="progress">
		    		<span class="bar"></span><span class="percent">0%</span>
				</div>
		        <div class="files"></div>
		   </div>
		</div>
		<div class="srch_mod01">
			<form action="jxy_upload.php" method="post" id="search-form">
				<p></p>
				<ul><li><div>
			  	<label>更新ios平台:<input name="platforms[]" type="checkbox" <?php if(in_array("ios",$_platforms)){echo "checked";} ?> value="ios" /></label>
				<label>&nbsp&nbsp&nbsp&nbsp更新安卓平台:<input name="platforms[]" type="checkbox" <?php if(in_array("android",$_platforms)){echo "checked";} ?> value="android" /></label>
				</div></li></ul>
				<br/>
				<ul><li class="btn_box"><span class="btn01"><input type="submit" value="开始上传" class="btn_srch" name="submit" id="upload_btn"></span></li></ul>
				<input type="hidden" id="up_file" name="up_file" value="">
				<input type="hidden" id="svr_up_file_name" name="svr_up_file_name" value="">
				<input type="hidden" id="file_size" name="file_size" value="">
			</form>
		</div>
	</div>

	<script type="text/javascript">
		$(function () {
			var bar = $('.bar');
			var percent = $('.percent');
			var showimg = $('#showimg');
			var progress = $(".progress");
			var files = $(".files");
			var btn = $(".btn span");
			$("#fileupload").wrap("<form id='myupload' action='handle_upload.php?act=upload' method='post' enctype='multipart/form-data'></form>");
		    $("#fileupload").change(function(){
				$("#myupload").ajaxSubmit({
					dataType:  'json',
					beforeSend: function() {
		        		showimg.empty();
						progress.show();
		        		var percentVal = '0%';
		        		bar.width(percentVal);
		        		percent.html(percentVal);
						btn.html("上传中...");
		    		},
		    		uploadProgress: function(event, position, total, percentComplete) {
		        		var percentVal = percentComplete + '%';
		        		bar.width(percentVal);
		        		percent.html(percentVal);
		    		},
					success: function(data) {
						var echo_size="";
						if (data.size < 1024) {
							echo_size=data.size+"KB";
						} else{
							echo_size=(data.size/1024).toFixed(2)+"MB";
						}

						files.html("<b>"+data.name+"("+echo_size+")</b><span class='delimg' rel='"+data.pic+"'>删除</span>");
						btn.html("添加附件");

						$('#up_file').val(data.name);
						$('#svr_up_file_name').val(data.stamp_name);
						$('#file_size').val(echo_size);

						btn.html("添加附件");
						$('#upload_btn').val('开始上传 <'+data.name+'> 文件');
					},
					error:function(xhr){
						btn.html("上传失败");
						bar.width('0');
						files.html(xhr.responseText);
					}
				});
			});
			
			$(".delimg").live('click',function(){
				var pic = $(this).attr("rel");
				$.post("handle_upload.php?act=delimg",{imagename:pic},function(msg){
					if(msg==1){
						files.html("删除成功.");
						showimg.empty();
						progress.hide();

						$('#up_file').val('');
						$('#upload_btn').val('开始上传');
					}else{
						files.html(msg);
					}
				});
			});
		});
	</script>

<?php
	function each_host_cp_svrs($hosts, $desc)
	{
		foreach ($hosts as $ip => $info) {
    		$zone=$info[0];
    		$usr=$info[1];
    		$pwd=$info[2];

    		$cmd="";
    		if (count($info) >= 4) {
    			$cmd=$info[3];
    		}

    		echo '<font style="color:#0068B7"><pre>';
			echo "$zone   IP:$ip   拷贝脚本:$cmd";
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

			// $stream = ssh2_exec($connection, 'cd /home/server/ios_app \n '.$cmd);
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

	function upload_to_remote_host($host_ip, $host_usr, $host_pwd, $desc, $local_up_file, $remote_upload_path, $cmd, $up_file_name)
	{
		echo '<font style="color:#0068B7"><pre>';
		echo "IP:$host_ip   正在上传:$local_up_file ($desc) -> $remote_upload_path";
		echo "</pre></font>";

		ob_flush();
		flush();

		$connection = ssh2_connect($host_ip, 65522);
		if ($connection){
			if (ssh2_auth_password($connection, $host_usr, $host_pwd)) {
				// echo "连接服务器成功！\n";
			} else {
				echo "<font style=\"color:#f74330\"><pre>连接服务器失败：用户名或密码错误！</pre></font>";
				break;
			}
		}
		else{
		   die("无法连接到服务器...");
		}

		$start_time=time();
		ssh2_scp_send($connection, $local_up_file, $remote_upload_path);
		$end_time=time();

		echo '<font style="color:#4a9705"><pre>';
		echo "上传$local_up_file ($desc) -> $remote_upload_path 成功，耗时：".$end_time-$start_time."s";
		echo "</pre></font>";

		echo '<font style="color:#0068B7"><pre>';
		echo "IP:$host_ip   正在拷贝 $remote_upload_path ($desc) -> 各服务器";
		echo "</pre></font>";

		ob_flush();
		flush();
		
		// 如果上传的是压缩包，则先在第一台服务器那边
		$file_type = strstr($up_file_name, '.');
		if($file_type==".tar.gz" || $file_type==".gz")
		{
			$strArr=explode('.', $up_file_name);
			$file_no_ext=$strArr[0];

			echo '<font style="color:#0068B7"><pre>';
			echo "正在解压 $up_file_name -> $file_no_ext";
			echo "</pre></font>";

			$stream = ssh2_exec($connection, "tar zxvf $remote_upload_path");
			$stderr_stream = ssh2_fetch_stream($stream, SSH2_STREAM_STDERR);

			stream_set_blocking($stream, true);
			stream_set_blocking($stderr_stream, true);

			$output = stream_get_contents($stream);
			$err 	= stream_get_contents($stderr_stream);

			fclose($stream);
			fclose($stderr_stream);

			echo '<font style="color:#4a9705"><pre>';
			echo "解压 $up_file_name -> $file_no_ext 成功";
			echo "</pre></font>";

			$up_file_name=$file_no_ext;

			ob_flush();
			flush();
		}

		$cmd=str_replace("%svr%", $up_file_name, $cmd);

		$stream = ssh2_exec($connection, $cmd);
		$stderr_stream = ssh2_fetch_stream($stream, SSH2_STREAM_STDERR);

		stream_set_blocking($stream, true);
		stream_set_blocking($stderr_stream, true);

		$output = stream_get_contents($stream);
		$err 	= stream_get_contents($stderr_stream);

		fclose($stream);
		fclose($stderr_stream);

		echo "<div class=\"crumb\"><pre>$output</pre><font style=\"color:#f74330\"><pre>$err</pre></font></div>";

		echo '<font style="color:#4a9705"><pre>';
		echo "拷贝 $remote_upload_path ($desc) -> 各服务器 成功";
		echo "</pre></font>";

		ob_flush();
		flush();
	}

	if(isset($_POST['up_file'])){
		if(count($_platforms) > 0)
		{
			// upload_to_remote_host($ios_usercenter_ip, "root", "e-jkas2", "大小: $_file_size", "files/$_svr_up_file", "/home/server/ios_app/$_up_file", "cd /home/server/ios_app \n cat cpfile.sh \n sh cpfile.sh %svr%", $_up_file);
		}

		foreach ($_platforms as $platform){
			if($platform=="ios"){
				print_hosts($ios_hosts, "ios平台");
				each_host_cp_svrs($ios_hosts, "ios平台");
			} else if($platform=="android"){
				print_hosts($and_hosts, "安卓平台");
				each_host_cp_svrs($and_hosts, "安卓平台");
			}
		}

		echo '<font style="color:#0068B7">';
		echo "执行完毕.";
		echo "</font>";
	}
	else{
		print_hosts($ios_hosts, "ios平台");
		print_hosts($and_hosts, "安卓平台");
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