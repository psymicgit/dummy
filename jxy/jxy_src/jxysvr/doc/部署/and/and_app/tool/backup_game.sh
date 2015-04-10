time=`date '+%Y-%m-%d--%H-%M-%S'`
dirTmp=`date '+%Y-%m-%d'`


for dbpre in $(find /home/server -maxdepth 1 -name "*_*" -type d)
do

	dbpre=`echo ${dbpre}|awk -F"/home/server/" '{print $2;}'`
	dir=/home2/bak/db/$dbpre/$dirTmp/playerdata
	
	mkdir -p $dir
	cd $dir
	
	echo $dir;	

	for zoneid in $(find /home/server/${dbpre} -maxdepth 1 -name "zone_*" -type d)
	do

		zoneid=`echo ${zoneid}|awk -F"zone_" '{print $2;}'`
		/home/mysql/mysql_63316/bin/mysqldump -ubackup -pbackup -h127.0.0.1 --port=63316 --dump-slave=1  --single-transaction --hex-blob --default-character-set=utf8 ${dbpre}_s${zoneid}  --ignore-table=${dbpre}_s${zoneid}.battlelog > ${dbpre}_s${zoneid}_${time}.sql
		tar -zcvf ${dbpre}_s${zoneid}_${time}.tar.gz ${dbpre}_s${zoneid}_${time}.sql
		rm -rf ${dbpre}_s${zoneid}_${time}.sql
		
	done

done

sleep 30

rsync -avz --progress --password-file=/etc/work.pass /home2/bak/db/ios_app/ work@192.168.2.103::102dbbackup > /tmp/rsyncd.log 2>&1
