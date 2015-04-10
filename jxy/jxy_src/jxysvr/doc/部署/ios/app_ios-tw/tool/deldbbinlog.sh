mysql -uroot -pMaxnet.1234 -S /tmp/mysql_63301.sock  -e 'PURGE MASTER LOGS BEFORE DATE_SUB( NOW(),INTERVAL 1 DAY)'
mysql -uroot -pMaxnet.1234 -S /tmp/mysql_63306.sock  -e 'PURGE MASTER LOGS BEFORE DATE_SUB( NOW(),INTERVAL 1 DAY)'
#mysql -uroot -pMaxnet.1234 -S /tmp/mysql_63311.sock  -e 'PURGE MASTER LOGS BEFORE DATE_SUB( NOW(),INTERVAL 1 DAY)'
mysql -uroot -pMaxnet.1234 -S /tmp/mysql_63316.sock  -e 'PURGE MASTER LOGS BEFORE DATE_SUB( NOW(),INTERVAL 1 DAY)'
