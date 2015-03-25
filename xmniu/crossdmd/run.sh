#!/bin/sh

killall crossdmd
killall crossdmd_robot

#bin/crossdmd [监听端口](端口默认是843，可以不填)
bin/crossdmd &
bin/crossdmd &
bin/crossdmd 10023 &
bin/crossdmd 10024 &
bin/crossdmd 10025 &
bin/crossdmd 10026 &
bin/crossdmd 10027 &
bin/crossdmd 10028 &
bin/crossdmd 10029 &
bin/crossdmd 10030 &

for i in $`seq 1`; do 
	#bin/crossdmd_robot 机器人数量 是否打印全部日志[是:1/否:0] 服务器1地址 服务器1端口 ...[服务器8地址 服务器8端口]... 

	#bin/crossdmd_robot 1000 1 s0.9.game2.com.cn 843 s0.9.game2.com.cn 844; 
	#bin/crossdmd_robot 200 1 s0.9.game2.com.cn 843; 

	bin/crossdmd_robot 5000 0 127.0.0.1 10023 127.0.0.1 10024 127.0.0.1 10025 127.0.0.1 10026 127.0.0.1 10027 127.0.0.1 10028 127.0.0.1 10029 127.0.0.1 10030; 
	bin/crossdmd_robot 5000 0 127.0.0.1 10023 127.0.0.1 10024; 
	bin/crossdmd_robot 5000 0 127.0.0.1 10023; 
done;

exit
