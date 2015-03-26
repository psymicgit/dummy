@echo off
set protoc=protoc.exe
rem set protoc=..\..\..\3rd\protobuf\protoc-2.5.0-win32\protoc.exe
set gen_dir=..
set cur_dir=%~dp0

echo 0. 清除原有数据

rem del /Q /S *.h
rem del /Q /S *.cpp
rem del /Q /S *.xsd
del /Q /S %gen_dir%\*.h
del /Q /S %gen_dir%\*.cpp

rem 下面是批处理中的一些符号的说明
rem %~dp0 =当前盘符+路径，如: d:\xxx\xxx
rem 同时，在批处理的for循环中
rem %%s   =完全文件路径，  如: d:\abc.txt，其中的s可以是别的字母，如%%x
rem %%~nxs=文件名带扩展，  如: abc.txt
rem %%~ns =文件名不带扩展，如: abc

echo 1. 生成protobuf文件
    for /F %%x in ('dir /b /a-d /s "*.proto"') do (
        echo 正在处理[ %%~nxx ]
        %protoc% %%~nxx --cpp_out=%gen_dir%/
    )

pause