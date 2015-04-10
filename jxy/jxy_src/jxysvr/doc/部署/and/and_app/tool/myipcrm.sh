for i in $(ipcs -m | awk '{if($5 = 9188)  print $2; }') 
do 
	ipcrm -m $i; 
done

for i in $(ipcs -m | awk '{if($5 = 32)  print $2; }')
do
        ipcrm -m $i;
done

find /usr/maxnet -name "KeepAlive" -exec rm -rf {} \;
find /usr/maxnet -name "pipeinfo" -exec rm -rf {} \;

