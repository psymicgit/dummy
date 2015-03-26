#  @File Name: setup mysql binlog.sh
#  @Author: hongkunan
#  @Company: heihuo
#  @Created Time: 2013/12/4 15:56:54

#!/bin/bash

source ../jxy/util.sh

set +e

set mysqlservice=mysqld

mycnf="/etc/my.cnf"
#now=`date '+%Y-%m-%d_%Hh%Mm%Ss'`
now=`date '+%Y-%m-%d'`

# 主库信息
masterhost=192.168.4.106
masterport=3306
masterusr=root
masterpwd=123456

# 从库信息
slavehost=127.0.0.1
slaveport=3306
slaveusr=root
slavepwd=123456

# 主库授予从库的账号
slavegrantusr=slave
slavegrantpwd=123456

# 主库binlog、dump路径
dump_dir="/home/psy/proj/mysql/masterdump/$now"

# 取ip地址最后一段
serverid=109
expire_logs_days=7
log=log

force_setup_slave=1

# 数据库列表
declare -a dbs=(
    ios_app_cfg
    ios_app_s001
    ios_app_s001_log
    ios_app_s002
    ios_app_s002_log
    ios_app_s003
    ios_app_s003_log
    ios_app_s004
    ios_app_s004_log
)

echo 0. check environment
    # 检测用来备份主库的路径是否存在
    if [ -d $dump_dir ] ;then
        echo_ok "check success: $dump_dir exist!"
    else
        echo "detecting $dump_dir not exist, create $dump_dir directory!"
        mkdir $dump_dir

		if [ ! -d $dump_dir ] ;then
			echo "error: create directory $dump_dir fail, exiting!"
			exit
		fi
    fi

    # 检测my.cnf配置文件是否存在
    if [ -f $mycnf ]; then
        echo_ok "check success: ${mycnf} exist!"
    else
        echo_err "error: ${mycnf} not exist"
        exit
    fi

    # 检测是否能成功连上主库（通过检测是否成功执行指定语句）
    echo "connecting to masterdb $masterusr@[${masterhost}:${masterport}]......"
mysql -h$masterhost -u$masterusr -p$masterpwd << _EOFMYSQL_ > $log
    # 查询log-bin的开启情况，将结果输出到log文件
    show variables like 'log_%';
_EOFMYSQL_

    # 是否成功连接
    if [ "$?" -eq "0" ]; then
        echo_ok "check success: connect to masterdb $masterusr@[${masterhost}:${masterport}] success"
    else
        echo "error: can not connect to masterdb $masterusr@[${masterhost}:${masterport}]" 
        exit
    fi

    # 检测主库是否成功开启logbin（通过在show variables like 'log_%'返回的结果中查找log_bin on字符串来判断）
    logbinon=$(grep -i "log_bin.on" $log)
    if [ "$?" -eq "0" ]; then
        echo_ok "check success: detecting masterdb $masterusr@[${masterhost}:${masterport}]  log-bin = on"
    else
        echo "error: detecting masterdb $masterusr@[${masterhost}:${masterport}] log-bin = off"
        exit
    fi

	# 检测主库是否已开启master，是这样的，如果主库未开启master，则执行show master status;语句将会返回: Empty set (0.00 sec)，从库同理（show slave status;）
mysql -h$masterhost -u$masterusr -p$masterpwd << _EOFMYSQL_ > $log
	show master status\G
_EOFMYSQL_

	master_status=$(grep -i "Empty set*" $log)
    if [ "$?" -ne "0" ] && [ -z "$master_status"]; then
		echo_ok "check success: detecting masterdb $masterusr@[${masterhost}:${masterport}] 's master = on: "
		echo "/-------------------------------------------------------------------------------------------"
		cat $log
		echo "-------------------------------------------------------------------------------------------/"
	else
		cat $log
		echo "error: detecting masterdb $masterusr@[${masterhost}:${masterport}] 's master = off, exit"
		exit
	fi

    # 检测是否能成功连上从库
	echo "connecting to slavedb $slaveusr@[${slavehost}:${slaveport}] ......"
mysql -h$slavehost -u$slaveusr -p$slavepwd << _EOFMYSQL_
_EOFMYSQL_

    # 是否成功连接从库
    if [ "$?" -eq "0" ]; then
        echo_ok "check success: connect to slavedb $slaveusr@[${slavehost}:${slaveport}] success"
    else
        echo "error: can not connect to slavedb $slaveusr@[${slavehost}:${slaveport}]"
        exit
    fi

	# 检测从库是否已经配置成功
mysql -h$slavehost -u$slaveusr -p$slavepwd << _EOFMYSQL_ > $log
	show slave status\G
_EOFMYSQL_

	slave_status=$(grep -i "Empty set*" $log)
    if [ "$?" -ne "0" ] && [ -z "$slave_status" ]; then
        echo_ok "detecting slave = on:"
		echo "/-------------------------------------------------------------------------------------------"
		cat $log
		echo "-------------------------------------------------------------------------------------------/"

		if [ $force_setup_slave -eq 0 ]; then
			echo_ok "Complete & exiting!"
			exit
		else
			echo "force_setup_slave = $force_setup_slave != 0, fore to setup slave, continue......"
		fi
	else
		echo 'detecting slave=off, start configuring....'
	fi

echo 1.configure $mycnf

	# 5.6版本配置主从已经不需要在my.cnf添加master-host之类的字段了，而是通过change master to master-host=...... 语句来设置
	#server-id=$serverid
	#master-host=$masterhost
	#master-port=$masterport
	#master-user=$slaveusr
	#master-password=$slavepwd

	pre_server_id=$(grep -i "server-id*=*" $mycnf)
	if [ "$?" -eq "0" ]; then
		echo_ok "ok: detecting existing $pre_server_id!"
	else
		echo "appending server-id = $serverid"
cat << _EOF_ >> $mycnf
server-id = $serverid
_EOF_
	fi

	pre_expire_logs_day=$(grep -i "expire_logs_days*=*" $mycnf)
	if [ "$?" -eq "0" ]; then
		echo_ok "ok: detecting existing $pre_expire_logs_day!"
	else
		echo "appending expire_logs_days = $expire_logs_days"
cat << _EOF_ >> $mycnf
expire_logs_days = $expire_logs_days
_EOF_
	fi

# Binary Logging.
# 自动跳过的表，有些表没必要做复制
# replicate-ignore-table=ios_app_cfg.xxx
# 自动跳过的错误代码，以防复制出错被中断
# slave-skip-errors = 1032,1062,126,1114,1146,1048,1396 rem 自动跳过的错误代码，以防复制出错被中断

	# 配置需要做从的数据库名
	for db in ${dbs[@]}
	do
		pre_do_db=$(grep -i "replicate-do-db*=*$db$" $mycnf)
		if [ "$?" -eq "0" ]; then
			echo_ok "ok: detecting existing $pre_do_db"
		else
			echo "appending replicate-do-db=$db"
cat << _EOF_ >> $mycnf
replicate-do-db=$db
_EOF_
# 这里_EOF_一定要紧贴最左边，不然会提示错误
		fi
	done

	echo "configure $mycnf done"

echo 2. flush masterdb log
mysql -h$masterhost -u$masterusr  -p$masterpwd << _EOFMYSQL_
    #flush tables with read lock;
	flush logs;
_EOFMYSQL_

echo 3. start backup masterdb
    # 备份各个数据库到指定目录
    for db in ${dbs[@]}
    do
		dump_file=$dump_dir/$db.sql

        echo backingup $db -> $dump_file ......

		# 可逐行抽取，也可缓存整个表内容然后一次dump，但对于大表则比较消耗内存
		# --quick采用row-by-row模式，也可使用—opt(包含--quick)；--skip-quick则使用缓存机制
		# --dump-slave：生成change master语句
		# --apply-slave-statements: 指定—dump-slave时，在change master to前后添加stop slave/start slave
		# --include-master-host-port：对生成的change master添加master_host/master_port选项
		# --master-data(1|2)：可用于创建slave，该选项会输出change master to 并包含binlog的具体位置，在导入slave后就不必再手工配置change master to信息；如果值为2则以注释的方式生成；
		#					  此选项会自动禁止—lock-tables同时激活—lock-all-tables(除非指定了—single-transaction)；
		#					  需要reload权限，并且启用了二进制日志
		# --lock-tables/-l：对于每个即将dump的db，提前锁定其下所有表；对MyISAM采用read local，InnoDB最好采用--single-transaction
		# --single-transaction：只用于事务表，不必锁定要导出的表；将事务隔离级别设置为repeatable read，在dump数据前执行start transaction

        mysqldump -h$masterhost -u$masterusr -p$masterpwd --lock-tables --single-transaction --skip-quick --master-data=1 --include-master-host-port --flush-logs --database $db >$dump_file
		# --default-character-set=utf8 --set-charset=utf8  
		#--dump-slave=1 --apply-slave-statements 

        # 检查是否备份成功（通过是否存在对应的dump文件来判断）
        if [ -f $dump_file ]; then
            echo_ok "success: backup $db -> $dump_file success!"
        else
            echo_err "error: backup $db -> $dump_file fail!"
			echo_err "exiting: unlocking masterdb [${masterhost}:${masterport}] ....."

mysql -h$masterhost -u$masterusr  -p$masterpwd << _EOFMYSQL_
    #unlock tables;
_EOFMYSQL_

			echo_err "back up is failed, please recheck the configuration"
            exit
        fi
    done

echo 4. unlock masterdb
mysql -h$masterhost -u$masterusr  -p$masterpwd << _EOFMYSQL_
    # unlock tables;
_EOFMYSQL_
	echo "[${masterhost}:${masterport}] is unlocked"

echo 5. recovering from masterdb
mysql -h$slavehost -u$slaveusr -p$slavepwd << _EOFMYSQL_
	stop slave;

	# 清除所有的slave同步信息
	reset slave all;

	change master to master_host='${masterhost}', master_user='${slavegrantusr}', master_password='${slavegrantpwd}';#, master_log_file='${masterlog}', master_log_pos=${dump_pos};
_EOFMYSQL_

	# 从指定dump目录还原各个数据库
    for db in ${dbs[@]}
    do
		dump_file=$dump_dir/$db.sql

        echo "recoving from $db <- $dump_file"
		# mysqldump -h$masterhost -u$masterusr -p$masterpwd --database $db <$dump_file
        mysql -h$slavehost -u$slaveusr -p$slavepwd <$dump_file

        # 检查是否还原备份成功
        if [ "$?" -eq "0"  ]; then
            echo_ok "recover form $db <- $dump_file success"
        else
            echo_err "recover form $db <- $dump_file fail"
            exit
        fi
    done

echo 6. restart mysql
	service $mysqlservice restart

echo 7. start slave

mysql -h$slavehost -u$slaveusr -p$slavepwd << _EOFMYSQL_
	# 启动从库连接
	start slave; 
_EOFMYSQL_

# 最后， 查看从库状态
mysql -h$slavehost -u$slaveusr -p$slavepwd << _EOFMYSQL_ > $log
	show slave status\G
_EOFMYSQL_

	slave_status=$(grep -i "Empty set*" $log)
    if [ "$?" -ne "0" ] && [ -z "$slave_status" ]; then
        echo_ok "detecting slave = on:"
		echo "/-------------------------------------------------------------------------------------------"
		cat $log
		echo "-------------------------------------------------------------------------------------------/"
		echo_ok "All successfule! Complete & exiting!"
		exit
    else
		cat $log
        echo_err 'error: detecting slave = off, fail to configure slave, please recheck the configuration.'
	fi
	
