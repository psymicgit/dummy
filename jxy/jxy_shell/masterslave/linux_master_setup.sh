#  @File Name: setup mysql binlog.sh
#  @Author: hongkunan
#  @Company: 
#  @Created Time: Sun 03 Nov 2013 05:56:54 AM PST

#!/bin/bash

set mysqlservice=mysqld

set masterdir="/root/master"
mycnf="/etc/my.cnf"
masterlog=$masterdir/"mysql_bin_log"

masterusr=root
masterpwd=123456
masterport=3306

slavehost=192.168.4.109
slavegrantusr=slave
slavegrantpwd=123456

echo 0. check the environment
    # 检测主库binlog路径是否存在
    if [ -d masterdir ] ;then
        echo "check success: ${masterdir} exist!"
    else
        echo "check fail: ${masterdir} not exist!"
        exit
    fi
    
    # 检测my.cnf配置文件是否存在
    if [ -f $mycnf ]; then
        echo "check success: ${mycnf} exist!"
    else
        echo "check fail: ${mycnf} not exist"
        exit
    fi

echo 1. configure $mycnf

	pre_logbin=$(grep -i "log-bin=*" $mycnf)
    if [ "$?" -eq "0" ]; then
        echo "check success: binlog = on"
        echo "pre logbin: ${pre_logbin}"
    else
        echo 'detecting binlog = off'
        
        cat << _EOF_ >> $mycnf

            # Binary Logging.
            log-bin=$masterlog
            server-id=1
            binlog-do-db=ios_app_cfg
            binlog-do-db=ios_app_pay
            binlog-do-db=ios_app_user
            binlog-do-db=ios_app_s001
            binlog-do-db=ios_app_s001_log
        _EOF_
    fi

echo 2. grant privileges on masterdb
	# 执行授权语句，并在结束时把查询结果输出到log文件
    mysql -u$masterusr  -p$masterpwd << _EOFMYSQL_ > log
        grant all privileges on *.* to root@"%" identified by '$masterpwd';
        flush privileges;

        grant replication slave on *.* to '$slavegrantusr'@'${slavehost}' identified by '$slavegrantpwd';
        show variables like 'log_%';
    _EOFMYSQL_

	# 是否成功开启logbin（通过在show variables like 'log_%'返回的结果中查找log_bin on字符串来判断）
    logbinon=$(grep -i "log_bin.on" $mycnf)
    if [ "$?" -eq "0" ]; then
        echo "log-bin enable success"	
    else
		echo 'log-bin enable fail, please recheck all the path'
		exit
	fi

	echo $logbinon

echo 3. restart mysql
    service $mysqlservice restart