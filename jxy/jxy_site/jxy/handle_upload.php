<?php
	ini_set('max_execution_time', '60000');
	date_default_timezone_set('PRC');

$action = $_GET['act'];
if($action=='delimg'){
	$filename = $_POST['imagename'];
	if(!empty($filename)){
		unlink('files/'.$filename);
		echo '1';
	}else{
		echo '删除失败.';
	}
}else if($action=='upload'){
	$picname = $_FILES['mypic']['name'];
	$picsize = $_FILES['mypic']['size'];
	$pics="";
	if ($picname != "") {
		if ($picsize > 1024*1024*1024) {
			echo '图片大小不能超过1M';
			exit;
		}
		$type = strstr($picname, '.');
		$rand = rand(100, 999);
		// $pics = date("YmdHis") . $rand . $type;
		$pics = date("Y_m_d_H_i_s_") . $picname;

		//上传路径
		$pic_path = "files/". $pics;
		move_uploaded_file($_FILES['mypic']['tmp_name'], $pic_path);
	}
	$size = round($picsize/1024,2);
	$arr = array(
		'name'=>$picname,
		'stamp_name'=>$pics,
		'pic'=>$pics,
		'size'=>$size
	);
	echo json_encode($arr);
}
?>