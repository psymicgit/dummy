@echo off

rem call :startcommonsvrs s000
call :startsvrs .

rem pause
exit

:startsvrs
    set svrdir=%1

    echo ------------------��ʼ����%svrdir%�µķ�����-----------------------

  
    echo ����%svrdir%�µ�gameserver������
    cd gameserver
    start "%svrdir%\gameserver.exe" "%svrdir%\gameserver.exe"
    cd ..

    echo ����%svrdir%�µ�gateserver������
    cd gateserver
    start "%svrdir%\gateserver.exe" "%svrdir%\gateserver.exe"
    cd ..

    echo ����%svrdir%�µ�dbserver������
    cd dbserver
    start "%svrdir%\dbserver.exe" "%svrdir%\dbserver.exe"
    cd ..

    echo ����%svrdir%�µ�robot������
    cd robot
    rem start "%svrdir%\robot.exe" "%svrdir%\robot.exe"
    cd ..

    echo.
goto :eof