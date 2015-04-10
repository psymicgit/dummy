
hh=/home/server/and_app

for((i = 11; i <= 25;i++))
do 
	zoneid=`printf "%03d" $i`
	cd ${hh}/zone_$zoneid/monitorserver
	./monitorserver
done

