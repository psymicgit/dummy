
hh=/home/server/and_app

for((i = 1; i <= 10;i++))
do 
	zoneid=`printf "%03d" $i`
	cp  -f   gameserver $hh/zone_$zoneid/gameserver/1
	cp  -f   gameserver $hh/zone_$zoneid/gameserver/2
	cp  -f   gateserver $hh/zone_$zoneid/gateserver/
	cp  -f   monitorserver $hh/zone_$zoneid/monitorserver/
done







