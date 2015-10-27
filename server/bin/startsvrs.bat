@echo off

rem call :startcommonsvrs s000
call :startsvrs .

rem pause
exit

:startsvrs
    set svrdir=%1

    echo ------------------开始启动%svrdir%下的服务器-----------------------

  
    echo 启动%svrdir%下的gameserver服务器
    cd gameserver
    start "%svrdir%\gameserver.exe" "%svrdir%\gameserver.exe"
    cd ..

    echo 启动%svrdir%下的gateserver服务器
    cd gateserver
    start "%svrdir%\gateserver.exe" "%svrdir%\gateserver.exe"
    cd ..

    echo 启动%svrdir%下的dbserver服务器
    cd dbserver
    start "%svrdir%\dbserver.exe" "%svrdir%\dbserver.exe"
    cd ..

    echo 启动%svrdir%下的robot机器人
    cd robot
    rem start "%svrdir%\robot.exe" "%svrdir%\robot.exe"
    cd ..

    echo.
goto :eof