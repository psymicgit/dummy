@echo off

rem �������г��÷���
rem 	%~dp0 =��ǰ�̷�+·������: d:\xxx\xxx
rem
rem �������forѭ���г��÷���
rem 	%%s =��ȫ�ļ�·����  ��: d:\abc.txt�����е�s�����Ǳ����ĸ����%%x
rem 	%%~nxs =�ļ�������չ��  ��: abc.txt
rem 	%%~ns =�ļ���������չ����: abc
rem 	%%~xs =��չ������: txt


set protoc=bin\protoc.exe
set csprotoc=bin\ProtoGen.exe
set out_dir=..
set cur_dir=%~dp0
set temp_dir=�����ļ�

rem echo 0. ���ԭ������
rem del /Q /S %temp_dir%\*

echo ------------------- ����� -------------------
echo 1. ����c++�ļ�
	for /R %%f in (*.proto) do (
		echo ���ڴ���[ %%~nxf ]
        %protoc% %%~nxf --cpp_out=%temp_dir%\
	)
	
echo 2. ������������ļ�
	setlocal EnableDelayedExpansion

	for /R "%temp_dir%" %%f in (*.cc *.h) do (
		set new_file=%%f
		set old_file=%out_dir%\%%~nxf
		
		echo n|comp !old_file! !new_file! >nul 2>&1
		if errorlevel 1 (		
			move /Y !new_file! %out_dir%
		)
	)

echo ------------------- �ͻ��� -------------------

echo 1. ����c#�ļ�
	for /R %%f in (*.proto) do (
		echo ���ڴ���[ %%~nxf ]
		%csprotoc% -i:%%~nxf -o:%temp_dir%\%%~nf.cs
	)
	
echo 2. ������������ļ�
	set client_out_dir=%out_dir%\client

	for /R "%temp_dir%" %%f in (*.cs) do (
		set new_file=%%f
		set old_file=%client_out_dir%\%%~nxf
		
		echo n|comp !old_file! !new_file! >nul 2>&1
		if errorlevel 1 (		
			move /Y !new_file! %client_out_dir%
		)
	)
	
echo ----------- ��� -----------
pause