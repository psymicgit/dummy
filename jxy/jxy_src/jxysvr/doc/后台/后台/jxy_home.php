<!--
/*
*  @Author: 洪坤安
*  @Company: heihuo
*  @brief: 囧西游后台网站主页
*  @Created Time: 2014年7月30日
*/
-->

<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<title>囧西游服务端后台</title>
<link href="./css/t.frame.css" rel="stylesheet" />
<script type="text/javascript"
	src="./js/jquery-1.6.4.min.js"></script>
<script type="text/javascript"
	src="./js/t.core.js"></script>
<script type="text/javascript"
	src="./js/t.frame.js"></script>
</head>
<body>
	<div class="header" id="j_header">
		<h1 class="logo">
			<a href="">囧西游服务端后台</a>
		</h1>
		<div class="webnav"></div>
		<div class="userinfo">
		<span></span><a href="">刷新</a>
		</div>
	</div>
	<div class="main" id="j_main">
		<div class="main_left" id="j_main_left">
			<div class="left_tab" id="j_left_tab">
				<div class="left_tab_head developer" id="j_left_tab_head"></div>
				<div class="left_tab_body" id="j_left_tab_body">
					<div class="scrollbarv">
						<div class="scrollbarvbtn"></div>
					</div>
					<div class="scrollcen"></div>
				</div>
			</div>
		</div>
		<div class="main_right" id="j_main_right">
			<div class="right_wap">
				<div class="main_tab" id="j_main_tab">
					<div class="tabhead">
						<div class="leftbtn">
							<img src="/images/s.gif" />
						</div>
						<div class="tabwap">
							<ul>
								<li class="current"><a href="javascript:;" hidefocus="true">首 页</a>
								</li>
								<li class="addbtn01"><a href="javascript:;" hidefocus="true">+</a>
								</li>
							</ul>
						</div>
						<div class="rightbtn">
							<img src="/images/s.gif" />
						</div>
						<div class="addbtn02">
							<img src="/images/s.gif" />
						</div>
					</div>
					<div class="tabbody" id="j_tab_body">
						<iframe class="frame" src="#" frameborder="0" scrolling="auto"
							width="100%;"></iframe>
					</div>
				</div>
			</div>
		</div>
	</div>
	<script type="text/javascript">
		var pageCtrl = {
				init :function() {
					pageCtrl.initLayout();

					frameMessage._init([{"name":"元宝查询脚本","allow":true,"children":[{"name":"元宝","allow":true,"children":[{"name":"查询","link":"jxy_query_gold.php","allow":true}]},{"name":"维护","allow":true,"children":[{"name":"操作数据库","link":"jxy_each_zone_exec_sql.php","allow":true}]}]}], "/", "",false);
				},
				initLayout :function() {

					var mainLeft = $("#j_main_left");
					var mainRight = $("#j_main_right");
					
					var leftTab = $("#j_left_tab");
					var leftTabBody = $("#j_left_tab_body");

					var rightTab = $("#j_main_tab");
					var rightTabHead = rightTab.find(".tabhead");
					var rightTabBody = $("#j_tab_body");
					
					var layoutInst = new T.Layout(document.body, {northId:"j_header", eastId : "j_main_left", westId : "j_main_right"});
						layoutInst.on("resize", function(docHeight, mainHeight){
							
							var leftHeight = mainLeft.height();
								leftTab.css({height : leftHeight});
								leftTabBody.css({height : leftHeight});
							
							var rightHeight = mainRight.height();
							var rightBodyHeight = rightHeight - rightTabHead.outerHeight();
								rightTab.css({height : rightHeight});
								rightTabBody.css({height : rightBodyHeight});

								rightTabBody.find("iframe").each(function(k, iframe){
									$(iframe).css("height", rightBodyHeight);
								});
						});
						layoutInst.layout();
				}
			};
			$(document).ready(pageCtrl.init);
	</script>
</body>
</html>