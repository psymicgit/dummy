@echo off

set usr=root
set pwd=123456

set player=1294064
set sql=sql.log

rem 开始删除玩家

call :delplayer merge1 74266

rem pause
exit

:delplayer
    set database=%1
    set playerid=%2

    echo ----------删除%database%数据库中的%playerid%玩家-------------

	(
		echo delete from %database%.faction where FactionPlayerID ^= %playerid%;
		echo delete from %database%.factionplayer where JoinPlayerID ^= %playerid%;
		echo delete from %database%.myrecharge where PlayerID ^= %playerid%;
		echo delete from %database%.player_bags where PlayerID ^= %playerid%;
		echo delete from %database%.player_builds where PlayerID ^= %playerid%;
		echo delete from %database%.player_cdkey where PlayerID ^= %playerid%;
		echo delete from %database%.player_consume where PlayerID ^= %playerid%;
		echo delete from %database%.player_ext where PlayerID ^= %playerid%;
		echo delete from %database%.player_hero_ext where PlayerID ^= %playerid%;
		echo delete from %database%.player_instances where PlayerID ^= %playerid%;
		echo delete from %database%.player_pvp where PlayerID ^= %playerid%;
		echo delete from %database%.player_recharge where PlayerID ^= %playerid%;
		echo delete from %database%.player_relation where pidA ^= %playerid%;
		echo delete from %database%.player_relation where pidB ^= %playerid%;
		echo delete from %database%.player_tasks where PlayerID ^= %playerid%;
		echo delete from %database%.player_heros where PlayerID ^= %playerid%;
		echo delete from %database%.players where PlayerID ^= %playerid%;
		echo delete from %database%.private_chat_log where sendPid ^= %playerid%;
		echo delete from %database%.private_chat_log where recvPid ^= %playerid%;
		echo delete from %database%.send_flower_log where sendPid ^= %playerid%;
		echo delete from %database%.send_flower_log where recvPid ^= %playerid%;
    )>%sql%
    
	mysql -u%usr% -p%pwd%<%sql%
goto :eof