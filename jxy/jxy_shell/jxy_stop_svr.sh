
#  @File Name: jxy_stop_svr.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: 2013-12-25 13:49:27

#!/bin/bash

gamesvr=gameserver
gatesvr=gateserver
gmsvr=gmserver
monitorsvr=monitorserver
loginsvr=loginserver

declare -a svr_process_list=(
    $gamesvr
    $gatesvr
    $gmsvr
    $monitorsvr
    $loginsvr
)

echo "1. kill current running server"
    for svr_process in ${svr_process_list[@]}
    do
        # echo killing $svr_process
		killall -9 $svr_process
    done

