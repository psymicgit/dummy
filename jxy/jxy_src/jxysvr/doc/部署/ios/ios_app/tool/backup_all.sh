time=`date '+%Y-%m-%d--%H-%M-%S'`
dirTmp=`date '+%Y-%m-%d'`


for dbpre in $(find /home/server -maxdepth 1 -name "*_*" -type d)
do

	dbpre=`echo ${dbpre}|awk -F"/home/server/" '{print $2;}'`
	dir=/home/bak/db/$dbpre/$dirTmp/
	mkdir -p $dir

	cd $dir

	/home/mysql/mysql_63301/bin/mysqldump -ubackup  -pbackup -h127.0.0.1 --port=63301 --hex-blob --default-character-set=utf8 ${dbpre}_config   > ${dbpre}_config_${time}.sql
	tar -zcvf - ${dbpre}_config_${time}.sql | openssl des3 -salt -k zzb@009 | dd of=${dbpre}_config_${time}.tar.gz.des3
	#if [ $? -eq 0 ]; then
		rm -rf ${dbpre}_config_${time}.sql
	#fi

	/home/mysql/mysql_63301/bin/mysqldump -ubackup  -pbackup -h127.0.0.1 --port=63301	--hex-blob --default-character-set=utf8 ${dbpre}_pay   > ${dbpre}_pay_${time}.sql
	tar -zcvf - ${dbpre}_pay_${time}.sql | openssl des3 -salt -k zzb@009 | dd of=${dbpre}_pay_${time}.tar.gz.des3
	#if [ $? -eq 0 ]; then
		rm -rf ${dbpre}_pay_${time}.sql
	#fi
	
	/home/mysql/mysql_63301/bin/mysqldump -ubackup  -pbackup -h127.0.0.1 --port=63301 --hex-blob --default-character-set=utf8 ${dbpre}_user --ignore-table=${dbpre}_user.loginlog  > ${dbpre}_user_${time}.sql
	tar -zcvf - ${dbpre}_user_${time}.sql | openssl des3 -salt -k zzb@009 | dd of=${dbpre}_user_${time}.tar.gz.des3
	#if [ $? -eq 0 ]; then
		rm -rf ${dbpre}_user_${time}.sql
	#fi


	for zoneid in $(find /home/server/${dbpre} -maxdepth 1 -name "zone_*" -type d)
	do

		zoneid=`echo ${zoneid}|awk -F"zone_" '{print $2;}'`
		/home/mysql/mysql_63306/bin/mysqldump -ubackup -pbackup -pbackup -h127.0.0.1 --port=63306 --hex-blob --default-character-set=utf8 ${dbpre}_s${zoneid} --ignore-table=${dbpre}_s${zoneid}.battlelog > ${dbpre}_s${zoneid}_${time}.sql
		tar -zcvf - ${dbpre}_s${zoneid}_${time}.sql | openssl des3 -salt -k zzb@009 | dd of=${dbpre}_s${zoneid}_${time}.tar.gz.des3
		#if [ $? -eq 0 ]; then
			rm -rf ${dbpre}_s${zoneid}_${time}.sql
		#fi

		/home/mysql/mysql_63307/bin/mysqldump -ubackup -pbackup -pbackup -h127.0.0.1 --port=63307 --hex-blob --default-character-set=utf8 ${dbpre}_s${zoneid}_log > ${dbpre}_s${zoneid}_log_${time}.sql
		tar -zcvf - ${dbpre}_s${zoneid}_log_${time}.sql | openssl des3 -salt -k zzb@009 | dd of=${dbpre}_s${zoneid}_log_${time}.tar.gz.des3
		#if [ $? -eq 0 ]; then
			rm -rf ${dbpre}_s${zoneid}_log_${time}.sql
		#fi

	done

done
