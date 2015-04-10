hh=/home/server/and_app

cd ${hh}/zone_000/monitorserver
./monitorserver

cd ${hh}/zone_255/monitorserver
./monitorserver

for((i = 1; i <= 10;i++))
do 
	zoneid=`printf "%03d" $i`
	cd ${hh}/zone_$zoneid/monitorserver
	./monitorserver
done