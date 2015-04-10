

#删除过期的备份玩家数据
. /home/tool/getdays.sh
todaytmp=`date '+%Y%m%d%H%M%S'`
today=`GetDays2 $todaytmp`
for zonepre in $(find /home/server -maxdepth 1 -name "*_*" -type d)
do
	echo ${zonepre}		
	for dirpre in $(find ${zonepre} -maxdepth 4 -name "SavePlayerDataSql") 
	do
		echo ${dirpre}		
		for logpath in $(find ${dirpre} -maxdepth 3 -name "SavePlayerDataSql") 
		do
			for logdir in `ls $logpath`; do
				dirday=`GetDays3 $logdir`	
				passday=`expr $today - $dirday`
				if [ $passday -gt 1 ] 
				then
					rm -rf $logpath/$logdir
					echo "del $logpath/$logdir OK"
				fi
			done
		done
	done
done

