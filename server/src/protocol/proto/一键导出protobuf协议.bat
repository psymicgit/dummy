@echo off

rem �������г��÷���
rem 	%~dp0 =��ǰ�̷�+·������: d:\xxx\xxx
rem
rem �������forѭ���г��÷���
rem 	%%s =��ȫ�ļ�·����  ��: d:\abc.txt�����е�s�����Ǳ����ĸ����%%x
rem 	%%~nxs =�ļ�������չ��  ��: abc.txt
rem 	%%~ns =�ļ���������չ����: abc

set protoc=protoc.exe
set out_dir=..
set cur_dir=%~dp0
set temp_dir=�����ļ�

rem echo 0. ���ԭ������
rem del /Q /S %out_dir%\*.h
rem del /Q /S %out_dir%\*.cc

echo 1. ����protobuf�ļ�
    for /F %%f in ('dir /b /a-d /s "*.proto"') do (
        echo ���ڴ���[ %%~nxf ]
        %protoc% %%~nxf --cpp_out=%temp_dir%\		
    )
	
echo 2. ������������ļ�
	setlocal EnableDelayedExpansion

	for /F %%f in ('dir /b /a-d /s "%temp_dir%\*"') do (	
		set new_file=%%f
		set old_file=%out_dir%\%%~nxf
		
		echo n|comp !old_file! !new_file! >nul 2>&1
		if errorlevel 1 (
			xcopy /Y !new_file! %out_dir%			
		)
	)
	rem xcopy /D /Y /V %temp_dir%\* %out_dir%\

echo ----------- ��� -----------
pause