@echo off

call :startcommonsvrs s000
call :startsvrs s001
call :startsvrs s002
call :startsvrs s003
call :startsvrs s004

call :startgatesvr s001
rem call :startgatesvr s002
rem call :startgatesvr s003
rem call :startgatesvr s004

rem start "" "gmserver\gmserver.exe"
rem start "" "loginserver\loginserver.exe"
rem start "" "monitorserver\monitorserver.exe"

rem pause
exit



:startcommonsvrs
    rem start 参数第一个是标题栏名 第二个是进程路径

    set svrdir=%1

    echo ------------------开始启动%svrdir%下的服务器-----------------------

    echo 启动%svrdir%下的gmserver服务器
    start "%svrdir%\gmserver\gmserver.exe" "%svrdir%\gmserver\gmserver.exe"
    
    echo 启动%svrdir%下的pkserver服务器
    start "%svrdir%\pkserver\pkserver.exe" "%svrdir%\pkserver\pkserver.exe"

    echo.
goto :eof

:startsvrs
    set svrdir=%1

    echo ------------------开始启动%svrdir%下的服务器-----------------------

    echo 启动%svrdir%下的gameserver服务器
    start "%svrdir%\gameserver\1\gameserver.exe" "%svrdir%\gameserver\1\gameserver.exe"

    rem echo 启动%svrdir%下的gateserver服务器
    rem start "%svrdir%\gateserver\gateserver.exe" "%svrdir%\gateserver\gateserver.exe"

    rem echo 启动%svrdir%下的monitorserver服务器
    rem start "%svrdir%\gameserver\monitorserver.exe" "%svrdir%\monitorserver\monitorserver.exe"

    echo.
goto :eof

:startgatesvr
    set svrdir=%1

    echo ------------------开始启动%svrdir%下的网关服务器-----------------------

    echo 启动%svrdir%下的gateserver服务器
    start "%svrdir%\gateserver\gateserver.exe" "%svrdir%\gateserver\gateserver.exe"

    echo.
goto :eof