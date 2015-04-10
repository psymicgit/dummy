
#for((i = 1;i <= 9;i++));
#do 
#	cp "dealframe01.h" 'dealframe0'$i'.h';
#	sed -i "s/01/0$i/g" '/root/dealframe0'$i'.h'
#	cp "dealframe01.cpp" 'dealframe0'$i'.cpp';
#	sed -i "s/01/0$i/g" '/root/dealframe0'$i'.cpp'
#done
#
#for((i = 10;i <= 60;i++));
#do 
#	cp "dealframe01.h" 'dealframe'$i'.h';
#	sed -i "s/01/$i/g" '/root/dealframe'$i'.h'
#	cp "dealframe01.cpp" 'dealframe'$i'.cpp';
#	sed -i "s/01/$i/g" '/root/dealframe'$i'.cpp'
#done


#for((i = 10;i <= 60;i++));
#do 
#	echo "poDealFrame = new CDealFrame"$i"(&m_oManager);";
#done

DIR="/home/server/ios_app"
echo $DIR


echo "enter zone start id:"
read startid
if [ -z "$startid" ] ; then
	echo "startid is empty"
	exit 1;
fi

echo "enter zone end id:"
read endid
if [ -z "$endid" ] ; then
	echo "endid is empty"
	exit 1;
fi


for((i = $startid; i <= $endid;i++))
do 
	zoneid=`printf "%03d" $i`
	mkdir "zone_$zoneid" 
	cp -rf "zone_xxx/gameserver" "$DIR/zone_$zoneid" 
	cp -rf "zone_xxx/gateserver" "$DIR/zone_$zoneid" 
	cp -rf "zone_xxx/monitorserver" "$DIR/zone_$zoneid" 
	sed -i "s/@xxx@/$zoneid/g" `grep "@xxx@" -rl $DIR/zone_$zoneid`

done
