
#  @File Name: mysql.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: Mon 23 Dec 2013 06:01:18 AM PST

#!/bin/bash

mysql_dir=mysql-5.6.15
mysql_tar=$mysql_dir.tar.gz

mysqlgroup=mysql
mysqlusr=mysql

rootpwd=123456

echo "detecting whether the mysql is existed"

# 检测是否能成功连上数据库（通过检测是否成功执行指定语句）
mysql << _EOFMYSQL_
    show variables like 'log_%';
_EOFMYSQL_

	is_mysql_installed=$?
	if [ $is_mysql_installed != 0 ]; then
		echo "mysql not exist, start installing ${mysql_dir}..."
	else
		echo "mysql is existed"
		exit
	fi

echo "1. extracting $mysql_tar"
	if [ -d $mysql_dir ] ;then
		echo "ok: Directory ${mysql_dir} Exists!"
	else
		tar zxvf $mysql_tar
	fi

echo "installing dependency utilities.."
	#yum -y install autoconf automake zlib* fiex* libxml*
	#yum -y install ncurses ncurses-devel libmcrypt* libtool-ltdl-devel* 

echo "1. checking mysql user"
	cat /etc/group | grep $mysqlgroup > /dev/null 2>&1
	found_mysql_group=$?

	cat /etc/passwd | grep $mysqlusr > /dev/null 2>&1
	found_mysql_usr=$?

	if [ $found_mysql_group -eq 0 ]; then
		echo "detecting $mysqlgroup group existing"
	else
		echo "not found existing $mysqlgroup group, add $mysqlgroup group"
		groupadd $mysqlgroup
	fi

	if [ $found_mysql_usr -eq 0 ]; then
		echo "detecting $mysqlusr existing"
	else
		echo "not found existing $mysqlgroup usr, add $mysqlgroup usr"
		useradd -r -g $mysqlgroup $mysqlusr -c "create at 2013/12/24"
	fi

exit

echo "2. cmake && make && make install"
	cd $mysql_dir

	# mysql5.5之后需要用cmake支持编译安装
	cmake \
		-DCMAKE_INSTALL_PREFIX=/usr/local/mysql \
		-DMYSQL_DATADIR=/usr/local/mysql/data \
		-DSYSCONFDIR=/etc \
		-DWITH_MYISAM_STORAGE_ENGINE=1 \
		-DWITH_INNOBASE_STORAGE_ENGINE=1 \
		-DWITH_MEMORY_STORAGE_ENGINE=1 \
		-DWITH_READLINE=1 \
		-DMYSQL_UNIX_ADDR=/var/lib/mysql/mysql.sock \
		-DMYSQL_TCP_PORT=3306 \
		-DENABLED_LOCAL_INFILE=1 \
		-DWITH_PARTITION_STORAGE_ENGINE=1 \
		-DEXTRA_CHARSETS=all \
		-DDEFAULT_CHARSET=utf8 \
		-DDEFAULT_COLLATION=utf8_general_ci

	exit
	# ./configure  '--prefix=/usr/local/mysql' '--without-debug' '--with-charset=utf8' '--with-extra-charsets=all' '--enable-assembler' '--with-pthread' '--enable-thread-safe-client' '--with-mysqld-ldflags=-all-static' '--with-client-ldflags=-all-static' '--with-big-tables' '--with-readline' '--with-ssl' '--with-embedded-server' '--enable-local-infile' '--with-plugins=innobase'
	make -j4
	make install

echo "3. configuring $mysql_dir"
	chown -R $mysqlgroup.$mysqlusr /usr/local/mysql

	cd /usr/local/mysql/
	./scripts/mysql_install_db --user=mysql --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data

	cp -r support-files/mysql.server /etc/init.d/mysql
	cp support-files/my-default.cnf /etc/my.cnf

	chkconfig --del mysql
	chkconfig --add mysql
	chkconfig mysql on

echo "4. start mysql service"
	service mysql start
	/usr/local/mysql/bin/mysqladmin -u root password '$rootpwd'

	#/usr/local/mysql/bin/mysql -u root -p -S /tmp/mysql.sock

	#SET PASSWORD FOR 'root'@'localhost' = PASSWORD('123456');

echo "5. final, set path"
	echo export PATH=/usr/local/mysql/bin:$PATH >> /etc/profile
	/etc/profile

echo "complete!"

