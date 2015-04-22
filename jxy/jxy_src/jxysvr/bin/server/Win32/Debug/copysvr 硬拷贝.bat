@echo off

call :copyconfigs backup\s000 .
call :copyconfigs backup\s001 .
call :copyconfigs backup\s002 .
call :copyconfigs backup\s003 .
call :copyconfigs backup\s004 .

call :copysvr backup\s001 s001
call :copysvr backup\s002 s002
call :copysvr backup\s003 s003
call :copysvr backup\s004 s004
call :copycommonsvr backup\s000 s000
rem call :copysvr backup\s002 s002
rem call :copysvr backup\s003 s003
rem call :copysvr backup\s004 s004

rem xcopy /Y ".\*.bat" "all\"

rem pause
exit

:copysvr
    set configdir=%1
    set to=%2

    echo ------------------开始复制%to%，采用%configdir%下的配置-----------------------

    if not exist "%to%" (
        mkdir %to%
    )

    del /Q "%to%\*"

    xcopy /e /T /Y ".\gameserver\*.exe" "%to%\gameserver\"
    xcopy /e /T /Y ".\gameserver\*.lib" "%to%\gameserver\"

    xcopy /e /T /Y ".\gateserver\*.exe" "%to%\gateserver\"
    xcopy /e /T /Y ".\gateserver\*.lib" "%to%\gateserver\"

    xcopy /e /T /Y ".\monitorserver\*.exe" "%to%\monitorserver\"
    xcopy /e /T /Y ".\monitorserver\*.lib" "%to%\monitorserver\"

    call :copyconfigs %configdir% %to%
goto :eof

:copycommonsvr
    set configdir=%1
    set to=%2

    echo ------------------开始复制%to%，采用%configdir%下的配置-----------------------

    if not exist "%to%" (
        mkdir %to%
    )

    del /Q "%to%\*"

    xcopy /e /Y ".\gameserver\1\*.exe" ".\gameserver\2\"
    xcopy /e /Y ".\gameserver\1\*.lib" ".\gameserver\2\"
    xcopy /e /Y ".\gameserver\1\*.exe" ".\pkserver\"
    xcopy /e /Y ".\gameserver\1\*.lib" ".\pkserver\"
    move /Y ".\pkserver\gameserver.exe" ".\pkserver\pkserver.exe"

    xcopy /e /Y ".\pkserver\*.exe" "%to%\pkserver\"
    xcopy /e /Y ".\pkserver\*.lib" "%to%\pkserver\"

    xcopy /e /Y ".\gmserver\*.exe" "%to%\gmserver\"
    xcopy /e /Y ".\gmserver\*.lib" "%to%\gmserver\"

    call :copyconfigs %configdir% %to%
goto :eof

:copyconfigs
    set configdir=%1
    set to=%2

    echo ------复制%configdir%下的配置到%to%文件夹下------

    xcopy /e /Y "%configdir%\*.xml" "%to%\"
goto :eof