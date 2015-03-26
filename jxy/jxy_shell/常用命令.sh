--------------------------------------------------------------------------------------------------------------------------
mysql:
1. 查询状态
	查询数据库结构
	show databases;
	show tables;
	describe 表名;

	查看主从库状态
	show master status;
	show slave status;

	查看sock文件和数据库文件路径
	show variables like 'socket%';
	show variables like 'datadir';

	查看版本号
	select version();
	status;

	查看变量
	show global variables;
	mysqladmin variables -p(shell环境下)

	查看存储过程
	show procedure status;
	show triggers from `gvg_s002`;
	show events from `gvg_s002`;

	查看存储过程或函数的创建代码
	show create procedure xxx;
	show create function xxx;

	查看my.cnf位置
	ps -ef | grep mysql

	查看进程列表
	show processlist;

	sql语句分析
	explain SELECT * FROM xxx;

	显示当前已经被打开的表列表
	SHOW OPEN TABLES

	查看当前状态
	show status

	查看最大连接数
	show variables like 'max_connections';

2. 打印mysql默认参数
	mysqld --print-defaults

3. 命令行输出格式
	show slave status\G

4. 赋予权限
	grant all privileges on *.* to root@localhost identified by '123456';
	grant shutdown on *.* to root@localhost identified by '123456';

5. 设置默认配置文件
	mysqld_safe --defaults-file=/data/mysql/my.cnf

6. 设置密码
	mysqladmin --socket=/data/mysql/m1/mysqld.sock -u root password 'password'
	mysqladmin -u root -p 123456

7. 登录，连接数据库
	 mysql -S /data/mysql/m1/mysqld.sock -uroot -ppassword -e "show databes;"

8. 使用mysql_multi 管理

	启动/关闭全部实例		
	mysqld_multi --defaults-file=/data/mysql/my_multi.cnf --log=/tmp/mysql_multi.log  start
	mysqld_multi --defaults-file=/data/mysql/my_multi.cnf stop
	
	N 为1、2、3、4...	
	mysqld_multi --defaults-file=/data/mysql/my_multi.cnf start 1
	mysqld_multi --defaults-file=/data/mysql/my_multi.cnf start 1-4
	mysqld_multi --defaults-file=/data/mysql/my_multi.cnf stop N

9. 查看版本号
	mysql -V

10.备份
	SELECT * into outfile xxx from xxx.xxxtable
	load data INFILE xxx ignore into table xxx.xxxtable;

11.删除binlog
	[见: http://www.cnblogs.com/martinzhang/p/3454358.html]

	查看mysql bin-log日志，除了这个以外的，其它都可以使用删除。 
	show binary logs;

	查看binlog
	mysqlbinlog --start-datetime="2014-3-30 00:00:00" --stop-datetime="2014-3-30 03:00:00" --database=ios_app_s017 /home/mysql/data_63306/mysql-bin.004858


	删除mysql bin-log日志，删除binlog.000005之前的，不包括binlog.000058
	purge binary logs to 'binlog.000058';
	purge master logs before '2014-03-06 22:46:26';




12. 检测表数据
	见[http://database.51cto.com/art/201108/286708.htm]
	[http://outofmemory.cn/code-snippet/1386/usage-mysqlcheck-rebuild-mysql-database-method]
	mysqlcheck -u root -p --all-databases

13. 查看mysql配置
	mysql_config

14. mysqldum

15. 索引
	添加索引
	CREATE INDEX xxx_idx ON xxx_db.xxx_table(xxx_column);

	删除索引
	drop index xxx_idx on xxx_db.xxx_table;

16. 修复数据库
	修复myisam表 [http://blog.51yip.com/mysql/1457.html]
		myisamchk -of ios_app_s057_log/oprecord_20140401000352.MYI
		myisamchk -r ios_app_s057_log/oprecord_20140401000352.MYI
		myisamchk ios_app_s057_log/oprecord_20140401000352.MYI

17. 设定binlog_format
	见[http://codejm.iteye.com/blog/1540046]
	
	查看binlog_format
	show variables like '%binlog%';

18. MySQL死锁导致无法查询解决

	先查看当前正在执行的事务
	SELECT * FROM INFORMATION_SCHEMA.INNODB_TRX;  

	然后查看当前时间select sysdate()，看哪些事务执行得比较久。根据这个事物的线程ID（trx_mysql_thread_id）
	执行命令：
		kill 线程ID  
	解除死锁事务。

19. 查看缓冲区大小
	MySQL导出的SQL语句在导入时有可能会非常非常慢，在处理百万级数据的时候，可能导入要花几小时。在导出时合理使用几个参数，可以大大加快导入的速度。

　　-e 使用包括几个VALUES列表的多行INSERT语法;
　　--max_allowed_packet=XXX 客户端/服务器之间通信的缓存区的最大大小;
　　--net_buffer_length=XXX TCP/IP和套接字通信缓冲区大小,创建长度达net_buffer_length的行。
　　注意：max_allowed_packet 和 net_buffer_length 不能比目标数据库的设定数值大，否则可能出错。

　　首先确定目标数据库的参数值
　　show variables like 'max_allowed_packet';
　　show variables like 'net_buffer_length';

	根据参数值书写 mysqldump 命令，如：
　　mysqldump -uroot -p123 21andy -e --max_allowed_packet=16777216 --net_buffer_length=16384 > 21andy.sql
　　OK，现在速度就很快了，主要注意的是导入和导出端的 max_allowed_packet 和 net_buffer_length 这2个参数值设定，弄大点就OK了
　　其实，最快的方法，是直接COPY数据库目录，不过记得先停止 MySQL 服务。

--------------------------------------------------------------------------------------------------------------------------
shell:
1. 查看文件
	查看文件开头
	head -n 1000 xxx.log > log

	查看文件末尾
	tail -n 1000 xxx.log > log

	cat log

	more log


	Linux统计文件行数
		语法：wc [选项] 文件…
		说明：该命令统计给定文件中的字节数、字数、行数。如果没有给出文件名，则从标准输入读取。wc同时也给出所有指定文件的总统计数。字是由空格字符区分开的最大字符串。
		该命令各选项含义如下：
		　　- c 统计字节数。
		　　- l 统计行数。
		　　- w 统计字数。
	
	wc -l xxx.txt

	shell中将文件行数存入变量
	local line_cnt=`wc -l xxxxx| awk '{print $1}'`

2 查看相应端口是否已经被监听
	netstat -tunlp | grep xxxx

3. 查看哪些端口被打开  netstat -anp

	关闭端口号:
		iptables -A INPUT -p tcp --drop 端口号 -j DROP
		iptables -A OUTPUT -p tcp --dport 端口号 -j DROP

	打开端口号：
		iptables -A INPUT -ptcp --dport  端口号 -j ACCEPT

	开放端口
		1.  在/etc/sysconfig/iptables里添加

			vi /etc/sysconfig/iptables

		添加一条配置规则，如要想开放8080的端口，如下所示：

			-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 8080 –j ACCEPT

		2. 重启iptables

			/etc/init.d/iptables restart

		3. 看下状态

			/etc/init.d/iptables status

		4.关闭防火墙

		（1） 重启后永久性生效：
		开启：chkconfig iptables on 

		　　关闭：chkconfig iptables off
		　　（2） 即时生效，重启后失效（即重启后防火墙自动开启）：
		　　开启：service iptables start
		　　关闭：service iptables stop

4. 赋予权限
	chown -R mysql. xxxx

5. 查看资源
	查看当前最耗资源进程
	top -n 100

	[查看后输入P按cpu排序]
	[查看后输入M按内存排序]

	查看系统近期负载
	uptime

	查看各用户cpu
	w

	查看剩余内存
	free -mt		[m = MB][t = total]

	查看cpu
	lscpu

	查看磁盘资源情况
	df -hT			[h = 人类可读][T = 文件系统类别]

	查看磁盘容量情况
	fdisk -l		[List  the  partition  tables  for  the  specified devices and then exit.  If no devices are given, those mentioned in /proc/partitions (if that exists) are used.]

	测试磁盘速率
	hdparm -t /dev/sda

	怎么知道电脑有多少个CPU核心呢？
	查看CPU信息
	cat /proc/cpuinfo

	查看内存信息
	cat /proc/meminfo
	
	直接返回CPU的总核心数
	grep -c 'model name' /proc/cpuinfo

	[http://os.51cto.com/art/201005/200741.htm]
	CPU平均负载，硬盘活动
	iostat -m   [m=MB]


	iostat -m -d -x

6.查看进程
	ps -aux | grep mysql
	ps -ef | grep mysql

7.查看当前目录
	pwd

8.查找文件
	locate my.cnf 有的可能不支持
	which mysqld
	whereis mysqld

	查看文件类型，使用这个shell内建命令经常会有意想不到的发现
	type -a xxxx

9. 查看时间
	date '+%Y-%m-%d_%Hh%Mm%Ss'

10. 查看系统状态
    
	查看计算机什么时候启动的，以及有谁登录过
	who

	当前用户
	whoami

	查看所有用户
	vi /etc/passwd

	查看用户登陆信息
	w

	查看当前登陆用户
	who

	最近一个月用户登陆情况
	last -Fn 10    [F = 完整时间格式][n = 显示最后几条]

	检查某特定用户上次登录的时间，并格式化输出上次登录日志/var/log/lastlog的内容
	lastlog

11. 用户权限相关
	建用户
	adduser phpq //新建phpq用户
	passwd phpq  //给phpq用户设置密码

	建工作组
	groupadd rungroup

	新建用户同时增加工作组
	useradd -g rungroup runusr

	删除用户
	userdel petergroup

	删除工作组
	groupdel peteruser

	强制删除该用户的主目录和主目录下的所有文件和子目录
	usermod –G petergroup peteruser

12. 文件操作
	创建文件
	touch xxx.txt

	创建软链接
	ln -sf source-file soft-link   (s表示软连接,f表示，若有同名文件在，则将它覆盖过去)

13. 控制命令
	清屏
	clear

	查看历史命令
	history 100 | less
	history 100 | more

	搜素最近一条历史命令
	ctrl + R

	历史命令文件
	vim ~/.bash_history

14. 压缩和解压
	gzip压缩
	gzip xxxx.txt

	gzip解压
	gzip -d xxxx.gzip

	tar压缩
	tar -zcvf xxxx.tar.gz xxxx.sql

	tar解压
	tar -zxvf xxxx.tar.gz

15. 查看文件大小
	du * -h 读*| sort -nr       [h = 人类可读][* = ./* 代表当前目录下所有文件]

	[见http://www.educity.cn/wenda/419739.html]
	比如查找当前目录下大于 10M 的文件
	find . -type f -size +10240k -exec du -h {} \;

	查看当前目录的大小
	du -sh .

16. 字符串命令
	sed命令行格式为：(见 http://www.iyism.com/blog/post/239/ )
		sed [-nefri] ‘command’ 输入文本

		常用选项：
		-n∶使用安静(silent)模式。在一般 sed 的用法中，所有来自 STDIN的资料一般都会被列出到萤幕上。但如果加上 -n 参数后，则只有经过sed 特殊处理的那一行(或者动作)才会被列出来。
		-e∶直接在指令列模式上进行 sed 的动作编辑；
		-f∶直接将 sed 的动作写在一个档案内， -f filename 则可以执行 filename 内的sed 动作；
		-r∶sed 的动作支援的是延伸型正规表示法的语法。(预设是基础正规表示法语法)
		-i∶直接修改读取的档案内容，而不是由萤幕输出。

		常用命令：
		a ∶新增， a 的后面可以接字串，而这些字串会在新的一行出现(目前的下一行)～
		c ∶取代， c 的后面可以接字串，这些字串可以取代 n1,n2 之间的行！
		d ∶删除，因为是删除啊，所以 d 后面通常不接任何咚咚；
		i ∶插入， i 的后面可以接字串，而这些字串会在新的一行出现(目前的上一行)；
		p ∶列印，亦即将某个选择的资料印出。通常 p 会与参数 sed -n 一起运作～
		s ∶取代，可以直接进行取代的工作哩！通常这个 s 的动作可以搭配正规表示法！例如 1,20s/old/new/g 就是啦！

	删除空行
	sed "/^\s*$/d" xxx.log

	删除文件中含有指定字符串的行
	sed -i '/^INSERT INTO/d' xxx.log

	在每行的头添加字符，比如"xxxx"，命令如下：

	sed 's/^/xxxx&/g' test.file

	在每行的行尾添加字符，比如“TAIL”，命令如下：

	sed 's/$/&xxxx/g' test.file

	把每行开头的  "(数字,"  替换成  "(NULL,"
	sed -i 's/^([0-9][0-9]*,/(NULL,/g' xxx.sql

	删除文件中含有指定字符串的行
	sed -i '/^INSERT INTO/d' xxx.log
	sed '1d' xxx.log	#删除第一行 
	sed '1d' xxx.log	#删除第一行 
	sed '$d' xxx.log	#删除最后一行
	sed '1,2d' xxx.log  #删除第一行到第二行
	sed '2,$d' xxx.log  #删除第二行到最后一行

	
	     删除某行
    sed '1d' ab              #删除第一行 
    sed '$d' ab              #删除最后一行
    sed '1,2d' ab           #删除第一行到第二行
    sed '2,$d' ab           #删除第二行到最后一行

　　显示某行
.   sed -n '1p' ab          #显示第一行 
    sed -n '$p' ab          #显示最后一行
    sed -n '1,2p' ab        #显示第一行到第二行
    sed -n '2,$p' ab        #显示第二行到最后一行

　　使用模式进行查询
    sed -n '/ruby/p' ab		 #查询包括关键字ruby所在所有行
    sed -n '/\$/p' ab        #查询包括关键字$所在所有行，使用反斜线\屏蔽特殊含义

    使用sed在指定行插入
	见http://blog.163.com/strive_only/blog/static/89380168201310148550782/

17. 远程数据同步
	见 http://www.linuxany.com/archives/226.html

	rsync -avz --password-file=/etc/work.pass /home2/bak/db/ios_app/ work@192.168.2.103::102dbbackup > /tmp/rsyncd.log 2>&1 [-a=--archive=文档mode][-v=--verbose=更加可视][-z=--compress=压缩]

18. 结束进程
	killall xxx

	kill -s 9 1827						[1827 = pid]
	其中-s 9 制定了传递给进程的信号是9

17. 复杂命令
	awk
	

	rsync
	dd
	tr

18. 查看内核版本命令
	cat /proc/version

19. vim
	显示行号
	:set nu

	譬如要统计hello这个词的个数，可以用下面的命令：
	:%s/hello/&/g

20. awk
	在文件中查找xxxx所在的行
	awk '/xxxx/' 文件名

	分割字符串，如将aaa_xxx_cccc，分割成aaa cccc
	awk -F"_xxx_" '{print $2;}'

	显示文本myfile中所有超过80个字符的行号，在这里，用$0表示整个记录（行），同时，内置变量NR不使用标志符'$'。 
	awk 'length($0)>80 {print NR}' xxxx

	awk 'length($0)>80 {print $0}' xxxx  [打印出超过80个字符的行]

21. 查看文件创建时间
	stat xxxx

22. securecrt
	解决SecureCRT中文显示乱码

	远程linux机器。修改环境变量LANG。例如在~/.bash_profile里面添加
	export LANG=zh_CN.UTF8  
	重新登录之后生效

	现在查看一下当前设置：

	查看一下当前设置
	locale 

	本地windows机器。修改SecureCRT的设置。找到“选项”->“会话选项”->“外观”：
	字符编码设置为utf-8。
    字体设置，选择中文字体，例如新宋体。

23. ccache
	设置ccache缓存大小
	ccache -M 50G

	设置ccache保存路径，存入~/.bash_profile文件
	export USE_CCACHE=1
	默认cache存储在~/.ccache. 可以使用下列命令来设置路径
	export CCACHE_DIR=xxxxxx

	查看ccache配置
	ccache -s

	给ccache添加链接
	cp ccache /usr/local/bin/
	ln -s ccache /usr/local/bin/gcc
	ln -s ccache /usr/local/bin/g++
	ln -s ccache /usr/local/bin/cc
	ln -s ccache /usr/local/bin/c++

23. 关机、重启
	重启命令：
	1、reboot
	2、shutdown -r now 立刻重启(root用户使用)
	3、shutdown -r 10 过10分钟自动重启(root用户使用) 
	4、shutdown -r 20:35 在时间为20:35时候重启(root用户使用)
	如果是通过shutdown命令设置重启的话，可以用shutdown -c命令取消重启

	关机命令：
	1、halt   立刻关机
	2、poweroff  立刻关机
	3、shutdown -h now 立刻关机(root用户使用)
	4、shutdown -h 10 10分钟后自动关机
	如果是通过shutdown命令设置关机的话，可以用shutdown -c命令取消重启

24. 查看目录下文件
	ll -t 按时间排序

25. tee 重定向文件并打印
	见[http://blog.csdn.net/love_gaohz/article/details/8100808]
	
	重定向到文件并打印到屏幕
	echo 'Debug: this is a debug infor.' | tee _log

	如果想以追加的方式输出到文件（ echo 'Debug: this is a debug infor.' >> _log 的效果），则需要在调用 tee 时加上 -a选项：
	echo 'Debug: this is a debug infor.' | tee -a _log

26. 杀死僵尸进程
	[http://www.cnblogs.com/sbaicl/archive/2012/09/16/2687201.html]
	解决方法：
	　　ps -A -o stat,ppid,pid,cmd |grep -e "^[Zz]" //先查看具体进程：
	　　kill -9 pid号　　　　　　　　　				//杀死z进程（这些动作是比较危险的，希望在真正的服务器上面慎用！！！）
	　　

	假若你的z进程比较多，可以编写个小小的脚本，下面是参与网上的
	ps -A -o stat,ppid,pid,cmd | grep -e '^[Zz]' | awk '{print $2}' | xargs kill -9

27. 查找文件
	find ./ -name *.log


	用rm递归递归删除子目录下所有.o后缀文件  
	find . -name "*.o"  | xargs rm -f

28. 同步服务器时间
	ntpdate time.nist.gov

29. 查找函数
	最后写上点man的小用法，一般只需直接man+所查命令，但有相同命令时，man出来的结果很可能不是想要的，可以先用man -f 查询，然后用man+数字+命令名查询。程序函数一般可以用man 2+函数名查到，如直接man open出来的并不是open()函数的帮助，发现有俩open，第二个是找开文件，这时就可用man 2 open了

	man page里面函数后面的括号加数字什么意思？
		RT，例如 ptrace(2) 后面的(2)是什么意思？

		------解决方案--------------------
		转自网上
		（1）所有的手册页都属于一个特定的领域，用一个字符来表示。
		 
		Linux下最通用的领域及其名称及说明如下：
		领域 名称 说明

		1 用户命令， 可由任何人启动的。
		2 系统调用， 即由内核提供的函数。
		3 例程， 即库函数。
		4 设备， 即/dev目录下的特殊文件。
		5 文件格式描述， 例如/etc/passwd。
		6 游戏， 不用解释啦！
		7 杂项， 例如宏命令包、惯例等。
		8 系统管理员工具， 只能由root启动。
		9 其他（Linux特定的）， 用来存放内核例行程序的文档。

		n 新文档， 可能要移到更适合的领域。
		o 老文档， 可能会在一段期限内保留。
		l 本地文档， 与本特定系统有关的。

		例如：man ls就会有LS(1)

		见[http://www.myexception.cn/linux-unix/227399.html]

30. crontab
	最简单最直接的方法：直接输入crontab -e 命令，在后面再追加一个命令即可

	显示用户的Crontab文件的内容 
	crontabl –l

	见http://www.cnblogs.com/xiaoluo501395377/archive/2013/04/06/3002602.html

31. nohup不输入回车
	nohup sleep 10 >/dev/null 2>&1 &
	把sleep 10替换成想要的命令

--------------------------------------------------------------------------------------------------------------------------
囧西游脚本:

1. 拷贝数据库
	./jxy_copy_db.sh "192.168.4.106" 3306 "192" "192.168.4.106" 3306 root 123456 dump ios_app_s004_copy ios_app_s004

2. 各区执行sql文件
	./jxy_each_zone_exec_sql.sh zone_list/ios_mysql.list zone_list/and_mysql.list zone_list/tw_mysql.list gamedb.sql logdb.sql

	执行完之后
	sed -i "/^\s*$/d" exec_sql.log 
	sed -i '/^and_app/d' exec_sql.log 
	sed -i '/^ios_app/d' exec_sql.log 
	sed -i '/^--/d' exec_sql.log 
	sed -i '/^sum/d' exec_sql.log 
	sed -i '/^Database/d' exec_sql.log 
	sed -i '/^NULL/d' exec_sql.log 

--------------------------------------------------------------------------------------------------------------------------
服务器run
1. 连接数据库的语句
	mysql -uroot -S /tmp/mysql_63316.sock -p

	执行一段sql语句
	mysql -uroot -S /tmp/mysql_63316.sock -p -vvv -e "" > merge_s003_s004

	合区
	use run_s003;
	call union_databases('run_s003', '正式3区', 'run_s004', '正式4区');

	mysql -S /tmp/mysql_63316.sock -uroot -p -vvv -e "use run_s003;call union_databases('run_s003', '正式3区', 'run_s004', '正式4区');" > merge_s003_s004.log

2. 数据库备份
	[mysqldump]
	备份服务器
		time mysqldump -uroot -p -S /tmp/mysql_63306.sock --single-transaction --opt --extended-insert --hex-blob --routines --triggers --events --default-character-set=utf8 --skip-comments --compress --ignore-table=ios_app_s001.battlelog --ignore-table=ios_app_s001.battlelog2 --ignore-table=ios_app_s001.goldrecord_2014 ios_app_s001 > ios_app_s001.sql

	备份服务器结构
		上面那句+[-d]

	还原数据库
		mysql -S /tmp/mysql_63306.sock -uroot -p ios_app_s001 < ios_app_s001.sql

	主库备份
		全部库备份
		mysqldump -S /tmp/mysql_63306.sock -uroot -p --hex-blob --default-character-set=utf8 --compress --master-data=1 --extended-insert --opt --single-transaction --skip-comments --all--database > ios_app_63306.sql

		几个库备份
		mysqldump -S /tmp/mysql_63306.sock -uroot -p --hex-blob --default-character-set=utf8 --compress --master-data=1 --extended-insert --opt --single-transaction --skip-comments --databases ios_app_s007 ios_app_s008 ios_app_s009 ios_app_s010 ios_app_s011 ios_app_s012 ios_app_s013 ios_app_s014 ios_app_s015 ios_app_s016 ios_app_s017 ios_app_s018 ios_app_s019 > ios_app_63306.sql

	主从
	见[http://www.blogjava.net/dongbule/archive/2010/08/22/329602.html]

	[mydumper]
		查看用法
		mydumper --help

		备份
		time mydumper -S /tmp/mysql_63306.sock -u root -p xxx  -v 3 -C -c -t 24 -B ios_app_s001 --less-locking --outputdir=/home3/bak/2014-04-04/ios_app_s001
		[-v, --verbose               Verbosity of output, 0 = silent, 1 = errors, 2 = warnings, 3 = info, default 2]
		[-C, --compress-protocol     Use compression on the MySQL connection]
		[-c, --compress              Compress output files]
		[-t, --threads               Number of threads to use, default 4]
		[-B, --database              Database to dump]
		[--less-locking              Minimize locking time on InnoDB tables.]
		[-o, --outputdir             Directory to output files to]
		[-r, --rows                  Try to split tables into chunks of this many rows. This option turns off --chunk-filesize]
		[-T, --tables-list			 导出的表列表，用逗号隔开，如players,faction]

		还原
		time myloader -S /tmp/mysql_63307.sock -u root -p xxxx -v 2 -C -t 24 -B xxxx_db -d xxx_dir/

3. 文件传输
	secureCrt
	上传
	在目录下输入rz命令, SecureCRT会弹出文件选择对话框，在查找范围中找到你要上传的文件，按Add按钮。然后OK就可以把文件上传到linux上了。

	下载
	输入sz filename，下载的文件存放在你设定的默认下载目录下.

	--拷贝文件到远程机器
	pscp e:\proj\masterslave\linux_slave_setup.sh root@192.168.4.59:/root/mysqlslave/

	--拷贝文件到本地机器
	scp -P 65522 root@192.168.2.102:/home/server/ios_app/ios_app_s002.sql E:\jxy\running

4. 安装mydumper
	1. 安装依赖库
		yum install cmake glib2-devel zlib zlib-devel pcre-devel -y			还有个[gcc-c++]一般不必再装了
	2. 解压
		tar zxvf mydumper-0.6.1.tar.gz
	3. 进入文件夹
		cmake .
		make && make install

	如果出现<错误：hash.h：没有那个文件或目录>，则拷贝hash.h到/usr/local/include/下

5. 给服务器里面的内网服赋予权限
	GRANT SELECT, INDEX, SHOW DATABASES, CREATE TEMPORARY TABLES, LOCK TABLES, EXECUTE, SHOW VIEW, CREATE ROUTINE, ALTER ROUTINE, EVENT, TRIGGER ON *.* TO 'root'@'192.168.2.102' identified by 'xxxx';
	grant all privileges on *.* to 'root'@'218.5.83.234' identified by 'xxxx';

6. nginx
	启动
	sbin/nginx -c conf/nginx.conf

	重启
	sbin/nginx -s reload

	MySQL
	/usr/bin/mysqld_safe
	/usr/libexec/mysqld

7. 远程连接服务器
	ssh -p 65522 root@192.168.2.102