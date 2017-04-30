@echo off
taskkill /f /im gameserver.exe
taskkill /f /im gateserver.exe
taskkill /f /im robot.exe
taskkill /f /im dbserver.exe

rem taskkill /f /im msbuild.exe
rem taskkill /f /im vcpkgsrv.exe
rem taskkill /f /im link.exe
rem taskkill /f /im mspdbsrv.exe
rem taskkill /f /im vsaenv10.exe
rem taskkill /f /im vsmsvr10.exe
rem taskkill /f /im cl.exe