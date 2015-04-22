@echo off

set cur_dir=%cd%

rem call :copyconfigs backup\s000 .
rem call :copyconfigs backup\s001 .
rem call :copyconfigs backup\s002 .
rem call :copyconfigs backup\s003 .
rem call :copyconfigs backup\s004 .

rem echo d | xcopy /e /Y "gateserver" "running_gateserver" /D
rem echo d | xcopy /e /Y "gameserver" "running_gameserver" /D
rem echo d | xcopy /e /Y "gmserver" "running_gmserver" /D

echo d | xcopy /e "gateserver\*.exe" "running_gateserver" /Y /D /EXCLUDE:exclude.txt
echo d | xcopy /e "gameserver\*.exe" "running_gameserver" /Y /D /EXCLUDE:exclude.txt
echo d | xcopy /e "gmserver\*.exe" "running_gmserver" /Y /D /EXCLUDE:exclude.txt

call :cleansvr s000
call :cleansvr s001
call :cleansvr s002
call :cleansvr s003
call :cleansvr s004

call :copysvr backup\s001 s001
call :copysvr backup\s002 s002
call :copysvr backup\s003 s003
call :copysvr backup\s004 s004
call :copycommonsvr backup\s000 s000

pause

exit

:cleansvr
    set to=%1
    rd /s /q "%to%\"
goto :eof

:copysvr
    set configdir=%1
    set to=%2

    echo ------------------开始复制%to%，采用%configdir%下的配置-----------------------

    echo 删除%to%
    rd /s /q "%to%\"
    rem ping -n 1 127.0.0.1 > nul 

    if not exist "%to%" (
        mkdir %to%
    )   

    call :copyconfigs %configdir% %to%

    echo ------------------创建硬链接-----------------------    

	mklink /H "%to%\gameserver\1\gameserver.exe" "%cur_dir%\running_gameserver\1\gameserver.exe" 
    mklink /H "%to%\gateserver\gateserver.exe" "%cur_dir%\running_gateserver\gateserver.exe"
    mklink /H "%to%\monitorserver\monitorserver.exe" "%cur_dir%\monitorserver\monitorserver.exe"

    rem xcopy /Y /D ".\gameserver\1\gameserver.exe" "%to%\gameserver\1\"
    rem xcopy /Y /D ".\gateserver\gateserver.exe" "%to%\gateserver\"
    rem xcopy /Y /D ".\monitorserver\monitorserver.exe" "%to%\monitorserver\"
goto :eof

:copycommonsvr
    set configdir=%1
    set to=%2

    echo ------------------开始复制%to%，采用%configdir%下的配置-----------------------

    if not exist "%to%" (
        mkdir %to%
    )

    rem xcopy /e /Y ".\gameserver\1\*.exe" ".\gameserver\2\"
    rem xcopy /e /Y ".\gameserver\1\*.lib" ".\gameserver\2\"

    call :copyconfigs %configdir% %to%

    mklink /H "%to%\pkserver\pkserver.exe" "%cur_dir%\running_gameserver\1\gameserver.exe"
    mklink /H "%to%\gmserver\gmserver.exe" "%cur_dir%\running_gmserver\gmserver.exe"
goto :eof

:copyconfigs
    set configdir=%1
    set to=%2

    echo ------复制%configdir%下的配置到%to%文件夹下------

    xcopy /e /Y "%configdir%\*.xml" "%to%\"
goto :eof