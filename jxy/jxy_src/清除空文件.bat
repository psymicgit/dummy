@echo off 
for /f "tokens=*" %%a in ('dir /b /ad /s .^|sort /r') do rd "%%a" /q 2>nul

pause