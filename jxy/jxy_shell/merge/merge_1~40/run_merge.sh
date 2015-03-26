#!/bin/bash

nohup mysql -S /tmp/mysql_63306.sock -uroot -pMaxnet.1234 ios_app_s001 < run_jxy_merge_gamedb__1_2_3_4_5_6_7_8.sql > run_jxy_merge_gamedb__1_2_3_4_5_6_7_8.log &
nohup mysql -h192.168.2.101 -P63306 -uroot -pMaxnet.1234 ios_app_s009 < run_jxy_merge_gamedb__9_10_11_12_13_14_15_16.sql > run_jxy_merge_gamedb__9_10_11_12_13_14_15_16.log &
nohup mysql -h192.168.2.101 -P63306 -uroot -pMaxnet.1234 ios_app_s017 < run_jxy_merge_gamedb__17_18_19_20_21_22_23_24.sql > run_jxy_merge_gamedb__17_18_19_20_21_22_23_24..log &
nohup mysql -h192.168.2.100 -P63306 -uroot -pMaxnet.1234 ios_app_s025 < run_jxy_merge_gamedb__25_26_27_28_29_30_31_32.sql > run_jxy_merge_gamedb__25_26_27_28_29_30_31_32.log &
nohup mysql -h192.168.2.100 -P63306 -uroot -pMaxnet.1234 ios_app_s033 < run_jxy_merge_gamedb__33_34_35_36_37_38_39_40.sql > run_jxy_merge_gamedb__33_34_35_36_37_38_39_40.log &
