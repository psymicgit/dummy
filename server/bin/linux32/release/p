for pid in $(ps -ef|grep $1|grep -v grep | awk '{ print $2 }')
do
	ls -l /proc/$pid/exe;
done


