
#  @File Name: jxy_start_svr.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: 2013-12-25 13:49:27

#!/bin/bash

bin_dir=.

gamesvr=gameserver
gatesvr=gateserver
gmsvr=gmserver
monitorsvr=monitorserver
loginsvr=loginserver

gamesvr_dir=$bin_dir/$gamesvr/1/
gatesvr_dir=$bin_dir/$gatesvr/
gmsvr_dir=$bin_dir/$gmsvr/
monitorsvr_dir=$bin_dir/$monitorsvr/
loginsvr_dir=$bin_dir/$loginsvr/

gamesvr_bin=$gamesvr_dir/$gamesvr
gatesvr_bin=$gatesvr_dir/$gatesvr
gmsvr_bin=$gmsvr_dir/$gmsvr
monitorsvr_bin=$monitorsvr_dir/$monitorsvr
loginsvr_bin=$loginsvr_dir/$loginsvr

declare -a svrs=(
    $gamesvr_bin
    $gatesvr_bin
    $gmsvr_bin
    $monitorsvr_bin
    $loginsvr_bin
)

echo "chmod +x to all svrs"
	# cp $bin_dir/$gamesvr/$gamesvr $gamesvr_bin
	all_executable=0
	for svr in ${svrs[@]}
    do
        if [ ! -e $svr ]; then
			echo "$svr is not executable, exiting"	
			exit
        fi

		echo chmod +x $svr
		chmod +x $svr
    done

echo "chmod all success"

echo "1. kill current running server"
    for svr_process in ${svr_process_list[@]}
    do
        # echo killing $svr_process
		killall -9 $svr_process
    done

echo "2. start run servers"
	echo launching gmserver
		cd $gmsvr_dir
		nohup ./$gmsvr &
		cd ../

	echo launching monitorserver
		cd $monitorsvr_dir
		nohup ./$monitorsvr &
		cd ../

exit
