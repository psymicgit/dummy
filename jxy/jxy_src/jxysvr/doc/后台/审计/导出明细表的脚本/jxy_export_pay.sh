#  @File Name: .sh
#  @Company: heihuo
#  @brief: 囧西游：导出订单表
#  @Created Time: 2014年7月14日

#!/bin/bash

source ./util.sh

declare err_cnt=0;
v=""

today=`date '+%Y_%m_%d'`

g_localhostdbopt="-uroot -p123456"

g_mysqldump_no_create_db_no_create_table="--no-create-db --no-create-info --add-locks --extended-insert --lock-tables --single-transaction --compress --complete-insert --quick --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

# 不建库，只建表和导数据
g_mysqldump_create_table_no_create_db="--no-create-db --single-transaction --compress --complete-insert --opt --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

function export_pay_list()
{
	local exportfile=$1
	local begin_pay_date=$2
	local end_pay_date=$3

	local pay_list=pay_list_2014

mysql $g_paydbopt $g_pay_db << _EOFMYSQL_
	drop table if exists ${pay_list};
	create table ${pay_list} as select e.DateProcess,e.OrderID,e.ZoneID,e.UserID,u.username,e.PlayerID,g.nickname,e.RMB from exchange e,$g_usercenter_db.globalplayers g, $g_usercenter_db.users u where 0;

	insert into ${pay_list} select p.createtime,p.orderid,p.zoneid,p.userid,u.username,p.PlayerID,g.nickname,p.rmb from $g_usercenter_db.payed p,$g_usercenter_db.globalplayers g, $g_usercenter_db.users u where p.PlayerID = g.playerid and p.userid = u.userid and p.createtime >= '${begin_pay_date}' and p.createtime < '${end_pay_date}'  and p.rmb > 1;
_EOFMYSQL_

mysql $g_localhostdbopt mysql << _EOFMYSQL_
	drop table if exists ${pay_list};
_EOFMYSQL_

	mysqldump -vvv $g_paydbopt $g_mysqldump_create_table_no_create_db $g_pay_db ${pay_list} | mysql $g_localhostdbopt mysql

	rm -f ${exportfile}

mysql $g_localhostdbopt -vvv mysql << _EOFMYSQL_
	select DateProcess,OrderID,ZoneID,UserID,username,PlayerID,nickname,RMB from ${pay_list} order by DateProcess asc into outfile '/mnt/hgfs/proj/dummy/jxy/${exportfile}' fields terminated by '	' enclosed by '"' lines terminated by '\n';
_EOFMYSQL_

	sed '1 i"date"	"orderid"	"zoneid"	"userid"	"username" "playerid" "nickname" "rmb"' -i ${exportfile}

	for (( i = 0; i < 2; i++ )); do
		echo "" >> ${exportfile}
	done

	echo "" >> ${exportfile}

	echo "《2014年起截止至${today}充值订单明细表》" >> ${exportfile}

mysql $g_localhostdbopt mysql << _EOFMYSQL_ >> ${exportfile}
	select count(*) as 记录数 from ${pay_list};
_EOFMYSQL_
}

function query_ios()
{
	g_paydbopt="-h118.26.225.102 -P63301 -uroot -p${dbpwd}"
	g_pay_db=ios_app_pay
	g_usercenter_db=ios_app_usercenter

	export_pay_list "ios_pay_at_${today}.log" "2014-01-01" "2014-07-01" 2> log
}

function query_and()
{
	g_paydbopt="-h42.62.60.197 -P63301 -uroot -p${dbpwd}"
	g_pay_db=and_app_pay
	g_usercenter_db=and_app_usercenter

	export_pay_list "and_pay_at_${today}.log" "2014-01-01" "2014-07-01" 2> log
}

# 开始进行配置了

# 2. 其次，获取数据库密码，通过手动输入的方式以防止密码泄露

	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	dbpwd=
	#read -s dbpwd

	if [ "$dbpwd" = "" ]; then
		echo_err "err: you enter nothing, aborted!"
		exit
	fi

# 3. 再次，根据传入的各区数据库列表依次检测是否能成功连上各台服务器的数据库

	# 传入的服务器ip和port列表将如下面所示：
	# --------------------------------------------------------------------------------------------------------
	# ZoneID	    GameDb	        GameDbIP	GameDbPort	    LogDb	        LogDbIP	        LogDbPort
	#  1	    ios_app_s001	118.26.225.102	63316	    ios_app_s001_log	118.26.225.102	63307
	#  2	    ios_app_s002	118.26.225.102	63316	    ios_app_s002_log	118.26.225.102	63307
	#  3	    ios_app_s003	118.26.225.102	63316	    ios_app_s003_log	118.26.225.102	63307
	#  4	    ios_app_s004	118.26.225.102	63316	    ios_app_s004_log	118.26.225.102	63307
	#  5	    ios_app_s005	118.26.225.102	63316	    ios_app_s005_log	118.26.225.102	63307
	#  6	    ios_app_s006	118.26.225.102	63316	    ios_app_s006_log	118.26.225.102	63307
	#  .....
	# --------------------------------------------------------------------------------------------------------

	# 开始检测

# 3. 开始在不同平台的每个区执行sql文件
	query_ios
	query_and