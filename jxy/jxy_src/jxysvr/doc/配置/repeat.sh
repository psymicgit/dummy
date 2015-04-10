echo "enter zone id:"
read zone_id
if [ -z "$zone_id" ] ; then
	echo "zone id is empty"
	exit 1;
fi

echo "enter user db ip:"
read user_db_ip
if [ -z "$user_db_ip" ] ; then
	echo "user db ip is empty"
	exit 1;
fi

echo "enter user db port:"
read user_db_port
if [ -z "$user_db_port" ] ; then
	echo "user db port is empty"
	exit 1;
fi

echo "enter pay db ip:"
read pay_db_ip
if [ -z "$pay_db_ip" ] ; then
	echo "pay db ip is empty"
	exit 1;
fi

echo "enter pay db port:"
read pay_db_port
if [ -z "$pay_db_port" ] ; then
	echo "pay db port is empty"
	exit 1;
fi


echo "enter gameserver db ip:"
read gameserver_db_ip
if [ -z "$gameserver_db_ip" ] ; then
	echo "gameserver db ip is empty"
	exit 1;
fi

echo "enter gameserver db port:"
read gameserver_db_port
if [ -z "$gameserver_db_port" ] ; then
	echo "gameserver db port is empty"
	exit 1;
fi

echo "enter record db ip:"
read record_db_ip
if [ -z "$record_db_ip" ] ; then
	echo "record db ip is empty"
	exit 1;
fi

echo "enter record db port:"
read record_db_port
if [ -z "$record_db_port" ] ; then
	echo "record db port is empty"
	exit 1;
fi


echo "enter cfg db ip:"
read cfg_db_ip
if [ -z "$cfg_db_ip" ] ; then
	echo "cfg db ip is empty"
	exit 1;
fi

echo "enter cfg db port:"
read cfg_db_port
if [ -z "$cfg_db_port" ] ; then
	echo "cfg db port is empty"
	exit 1;
fi

sed -i "s/zone_id/$zone_id/g" `grep zone_id -rl /root`

sed -i "s/user_db_ip/$user_db_ip/g" `grep user_db_ip -rl /root`
sed -i "s/user_db_port/$user_db_port/g" `grep user_db_port -rl /root`

sed -i "s/pay_db_ip/$pay_db_ip/g" `grep pay_db_ip -rl /root`
sed -i "s/pay_db_port/$pay_db_port/g" `grep pay_db_port -rl /root`

sed -i "s/gameserver_db_ip/$gameserver_db_ip/g" `grep gameserver_db_ip -rl /root`
sed -i "s/gameserver_db_port/$gameserver_db_port/g" `grep gameserver_db_port -rl /root`


sed -i "s/record_db_ip/$record_db_ip/g" `grep record_db_ip -rl /root`
sed -i "s/record_db_port/$record_db_port/g" `grep record_db_port -rl /root`

sed -i "s/cfg_db_ip/$cfg_db_ip/g" `grep cfg_db_ip -rl /root`
sed -i "s/0000-00-00 00:00:00/2013-11-07 08:00:00/g" `grep 0000-00-00 00:00:00 -rl /root`

sed -i "s/0000-00-00 00:00:00/2013-11-19 08:00:00/g" 'grep "0000-00-00 00:00:00" -rl  /home/server/ios_app/zone_058/'

sed -i "s/0000-00-00 00:00:00/2013-11-19 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_058`
sed -i "s/0000-00-00 00:00:00/2013-11-21 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_059`
sed -i "s/0000-00-00 00:00:00/2013-11-23 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_060`
sed -i "s/0000-00-00 00:00:00/2013-11-25 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_061`
sed -i "s/0000-00-00 00:00:00/2013-11-27 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_062`
sed -i "s/0000-00-00 00:00:00/2013-11-29 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_063`

sed -i "s/0000-00-00 00:00:00/2013-12-01 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_064`
sed -i "s/0000-00-00 00:00:00/2013-12-03 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_065`
sed -i "s/0000-00-00 00:00:00/2013-12-05 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_066`
sed -i "s/0000-00-00 00:00:00/2013-12-07 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_067`
sed -i "s/0000-00-00 00:00:00/2013-12-09 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_068`
sed -i "s/0000-00-00 00:00:00/2013-12-11 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_069`
sed -i "s/0000-00-00 00:00:00/2013-12-13 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_070`


sed -i "s/0000-00-00 00:00:00/2013-12-13 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_070`

sed -i "s/0000-00-00 00:00:00/2013-12-15 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_071`
sed -i "s/0000-00-00 00:00:00/2013-12-17 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_072`
sed -i "s/0000-00-00 00:00:00/2013-12-19 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_073`
sed -i "s/0000-00-00 00:00:00/2013-12-21 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_074`
sed -i "s/0000-00-00 00:00:00/2013-12-22 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_075`
sed -i "s/0000-00-00 00:00:00/2013-12-23 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_076`
sed -i "s/0000-00-00 00:00:00/2013-12-25 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_077`
sed -i "s/0000-00-00 00:00:00/2013-12-27 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_078`
sed -i "s/0000-00-00 00:00:00/2013-12-29 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_079`
sed -i "s/0000-00-00 00:00:00/2013-12-31 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_080`
sed -i "s/0000-00-00 00:00:00/2014-01-02 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_081`
sed -i "s/0000-00-00 00:00:00/2014-01-04 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_082`
sed -i "s/0000-00-00 00:00:00/2014-01-06 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_083`
sed -i "s/0000-00-00 00:00:00/2014-01-08 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_084`
sed -i "s/0000-00-00 00:00:00/2014-01-10 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_085`



sed -i "s/0000-00-00 00:00:00/2013-12-04 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/and_app/zone_001`

sed -i "s/@xx@/59/g" `grep "@xx@" -rl /home/server/ios_app/zone_059`

cd 

sed -i "s/@xx@/62/g" `grep "@xx@" -rl /home/server/ios_app/zone_062`
sed -i "s/@xx@/63/g" `grep "@xx@" -rl /home/server/ios_app/zone_063`
sed -i "s/@xx@/64/g" `grep "@xx@" -rl /home/server/ios_app/zone_064`
sed -i "s/@xx@/65/g" `grep "@xx@" -rl /home/server/ios_app/zone_065`
sed -i "s/@xx@/66/g" `grep "@xx@" -rl /home/server/ios_app/zone_066`
sed -i "s/@xx@/67/g" `grep "@xx@" -rl /home/server/ios_app/zone_067`
sed -i "s/@xx@/68/g" `grep "@xx@" -rl /home/server/ios_app/zone_068`
sed -i "s/@xx@/69/g" `grep "@xx@" -rl /home/server/ios_app/zone_069`
sed -i "s/@xx@/70/g" `grep "@xx@" -rl /home/server/ios_app/zone_070`
sed -i "s/@xx@/71/g" `grep "@xx@" -rl /home/server/ios_app/zone_071`
sed -i "s/@xx@/72/g" `grep "@xx@" -rl /home/server/ios_app/zone_072`
sed -i "s/@xx@/73/g" `grep "@xx@" -rl /home/server/ios_app/zone_073`
sed -i "s/@xx@/74/g" `grep "@xx@" -rl /home/server/ios_app/zone_074`
sed -i "s/@xx@/75/g" `grep "@xx@" -rl /home/server/ios_app/zone_075`
sed -i "s/@xx@/76/g" `grep "@xx@" -rl /home/server/ios_app/zone_076`
sed -i "s/@xx@/77/g" `grep "@xx@" -rl /home/server/ios_app/zone_077`
sed -i "s/@xx@/78/g" `grep "@xx@" -rl /home/server/ios_app/zone_078`
sed -i "s/@xx@/79/g" `grep "@xx@" -rl /home/server/ios_app/zone_079`

sed -i "s/@xx@/80/g" `grep "@xx@" -rl /home/server/ios_app/zone_080`
sed -i "s/@xx@/81/g" `grep "@xx@" -rl /home/server/ios_app/zone_081`
sed -i "s/@xx@/82/g" `grep "@xx@" -rl /home/server/ios_app/zone_082`
sed -i "s/@xx@/83/g" `grep "@xx@" -rl /home/server/ios_app/zone_083`
sed -i "s/@xx@/84/g" `grep "@xx@" -rl /home/server/ios_app/zone_084`
sed -i "s/@xx@/02/g" `grep "@xx@" -rl /home/server/and_app/zone_002`

sed -i "s/android_app/and_app/g" `grep "android_app" -rl /home/server/android_app/`

sed -i "s/0000-00-00 00:00:00/2013-12-04 08:00:00/g" 'grep "0000-00-00 00:00:00" -rl  /home/server/and_app/zone_001/'

sed -i "s/@xx@/255/g" `grep "@xx@" -rl /home/server/and_app/zone_255`
sed -i "s/0000-00-00 00:00:00/2013-12-03 08:00:00/g" `grep "0000-00-00 00:00:00" -rl /home/server/ios_app/zone_070`

sed -i "s/@xx@/254/g" `grep "@xx@" -rl /home/server/ios_app/zone_254`






