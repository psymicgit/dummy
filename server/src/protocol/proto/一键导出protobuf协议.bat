@echo off

rem 批处理中常用符号
rem 	%~dp0 =当前盘符+路径，如: d:\xxx\xxx
rem
rem 批处理的for循环中常用符号
rem 	%%s =完全文件路径，  如: d:\abc.txt，其中的s可以是别的字母，如%%x
rem 	%%~nxs =文件名带扩展，  如: abc.txt
rem 	%%~ns =文件名不带扩展，如: abc
rem 	%%~xs =扩展名，如: txt


set protoc=bin\protoc.exe
set csprotoc=bin\ProtoGen.exe
set out_dir=..
set cur_dir=%~dp0
set temp_dir=无用文件

rem echo 0. 清除原有数据
rem del /Q /S %temp_dir%\*

echo ------------------- 服务端 -------------------
echo 1. 生成c++文件
	for /R %%f in (*.proto) do (
		echo 正在处理[ %%~nxf ]
        %protoc% %%~nxf --cpp_out=%temp_dir%\
	)
	
echo 2. 拷贝变更过的文件
	setlocal EnableDelayedExpansion

	for /R "%temp_dir%" %%f in (*.cc *.h) do (
		set new_file=%%f
		set old_file=%out_dir%\%%~nxf
		
		echo n|comp !old_file! !new_file! >nul 2>&1
		if errorlevel 1 (		
			move /Y !new_file! %out_dir%
		)
	)

echo ------------------- 客户端 -------------------

echo 1. 生成c#文件
	for /R %%f in (*.proto) do (
		echo 正在处理[ %%~nxf ]
		%csprotoc% -i:%%~nxf -o:%temp_dir%\%%~nf.cs
	)
	
echo 2. 拷贝变更过的文件
	set client_out_dir=%out_dir%\client

	for /R "%temp_dir%" %%f in (*.cs) do (
		set new_file=%%f
		set old_file=%client_out_dir%\%%~nxf
		
		echo n|comp !old_file! !new_file! >nul 2>&1
		if errorlevel 1 (		
			move /Y !new_file! %client_out_dir%
		)
	)
	
echo ----------- 完成 -----------
pause