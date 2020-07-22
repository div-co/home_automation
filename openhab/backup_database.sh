# MySQL Backup 
#  
# mysqldump parameters available at : http://webcheatsheet.com/sql/mysql_backup_restore.php
#
currentDate=`date +%Y%m%d%H%M`
mysqldump --opt -u root -p openhab > /mnt/nfs/mysql/$currentDate.openhab.sql
