#  @File Name: jxy_merge
#  @Author: hongkunan
#  @Company: heihuo
#  @brief: 囧西游合区脚本
#  @Created Time: 2014年2月24日 9:23:35

#!/bin/bash

source ./util.sh

give_gift_sql=jxy_generate_gift.sql

# 数据库列表
declare -a zone_gifts=(
"1	10001	757	75700"
"2	10002	759	75900"
"3	10003	760	76000"
"4	10004	762	76200"
"5	10005	764	76400"
"6	10006	766	76600"
"7	10007	768	76800"
"8	10008	770	77000"
"9	10009	772	77200"
"10	10010	774	77400"
"11	10011	776	77600"
"12	10012	778	77800"
"13	10013	780	78000"
"14	10014	782	78200"
"15	10015	784	78400"
"16	10016	786	78600"
"17	10017	787	78700"
"18	10018	790	79000"
"19	10019	793	79300"
"20	10020	794	79400"
"21	10021	796	79600"
"22	10022	798	79800"
"23	10023	800	80000"
"24	10024	802	80200"
"25	10025	804	80400"
"26	10026	806	80600"
"27	10027	808	80800"
"28	10028	810	81000"
"29	10029	812	81200"
"30	10030	814	81400"
"31	10031	816	81600"
"32	10032	818	81800"
"33	10033	820	82000"
"34	10034	822	82200"
"35	10035	824	82400"
"36	10036	826	82600"
"37	10037	828	82800"
"38	10038	830	83000"
"39	10039	832	83200"
"40	10040	834	83400"
)

echo "" > $give_gift_sql

for ((i=0;i<${#zone_gifts[@]};i++))
{
	gift=${zone_gifts[i]}
	fields=($(echo $gift | tr ' ' ' ' | tr -s '	'))
	
	zoneid=${fields[0]}
	giftid=${fields[1]}
	gold=${fields[2]}
	coin=${fields[3]}

	merge_zoneid=$(($zoneid - 1))
	merge_zoneid=$(($merge_zoneid / 8))
	merge_zoneid=$(($merge_zoneid * 8))
	merge_zoneid=$(($merge_zoneid + 1))

#开始输出文件------------------------------------------------------------
cat << _EOF_ >> $give_gift_sql
replace into activityencouragepro(ZoneID,ActivityID,ActivityType,ActivityName,ActivityDesc,JsonValues,IsNeedCdKey,StartTime,EndTime) 
	values (${merge_zoneid}, ${giftid}, 1, 
	"合区礼包",
	"欢迎您来到合区之后的大家庭，小囧在此特别赠送合区礼包，里面有${gold}元宝和${coin}铜钱，并附送20点体力，祝您游戏愉快",
	'{
		"item": [{"value": 0,"num": 1,"percent": 100}],
		"hero": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"coin": {
			"value": ${coin},
			"num": 1,
			"percent": 100
		},
		"gold": {
			"value": ${gold},
			"num": 1,
			"percent": 100
		},
		"phystrength": {
			"value": 20,
			"num": 1,
			"percent": 100
		},
		"science": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"story": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"bluegas": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"pruplegas": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"jingjie": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"doorstribute": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"factionfunds": {
			"value": 0,
			"num": 1,
			"percent": 100
		},
		"factiontaskintegral": {
			"value": 0,
			"num": 1,
			"percent": 100
		}
	}',
	0, "0000-00-00 00:00:00", "2015-01-01 00:00:00");
_EOF_
#输出文件结束------------------------------------------------------------
}
