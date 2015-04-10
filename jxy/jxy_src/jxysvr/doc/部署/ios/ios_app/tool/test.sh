time=`date '+%Y-%m-%d--%H-%M-%S'`
dirTmp=`date '+%Y-%m-%d'`


for dbpre in $(find /home/server -maxdepth 1 -name "*_*" -type d)
do

	dir=/home/bak/db/$dbpre/$dirTmp/
	#mkdir -p $dir
	print $dbpre

	cd $dir

	for zoneid in $(find /home/server/${dbpre} -maxdepth 1 -name "zone_*" -type d)
	do

		zoneid=`echo ${zoneid}|awk -F"zone_" '{print $2;}'`
		print $zoneid
		
	done

done