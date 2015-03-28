@echo off
taskkill /f /im gameserver.exe
taskkill /f /im gateserver.exe
taskkill /f /im robot.exe

taskkill /f /im msbuild.exe
taskkill /f /im vcpkgsrv.exe
taskkill /f /im link.exe
taskkill /f /im mspdbsrv.exe
taskkill /f /im vsaenv10.exe
taskkill /f /im vsmsvr10.exe
taskkill /f /im cl.exe
exit