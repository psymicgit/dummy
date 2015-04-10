. /home/tool/getdays.sh
todaytmp=`date '+%y_%m_%d'`
today=`GetDays $todaytmp`
for dbpre in $(find /home/server -maxdepth 1 -name "*_*" -type d)
do
        echo ${dbpre}		
	find ${dbpre} -name "*1970_01_01.log" -exec rm -rf {} \;
        for logfile in $(find ${dbpre} -name "*.log") 
	do
                datetime=`echo $logfile|awk 'BEGIN{FS="log/"}{ print $2; }'|awk 'BEGIN{ FS="_20"}{ print $2; }'|awk 'BEGIN{ FS=".log"} { print $1; }'`
                fileday=`GetDays $datetime`
                passday=`expr $today - $fileday`
                if [ $passday -gt 3 ]
                then
                        rm -rf $logfile
                fi
	done
done

