
hh=/home/server/ios_app

for((i = 6; i <= 20;i++))
do 
	zoneid=`printf "%03d" $i`
	cd ${hh}/zone_$zoneid/monitorserver
	./monitorserver
done



