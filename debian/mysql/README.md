# Install MySQL

    #apt-get update
    #apt-get install mysql-client mysql-server

### Create openhab database
~~~
$ mysql -p
$ mysql> CREATE DATABASE openhabpersist;
$ mysql> GRANT ALL ON openhab.* TO 'SQL_USER'@'localhost';
$ /etc/init.d/mysql restart
~~~
Check connectivity
~~~
$ mysql> show databases;
~~~

### Backup database
~~~   
dbbackup.sh
------------

currentDate=`date +%Y%m%d%H%M`
mysqldump --opt -u root -p openhab > /<backup_location>/$currentDate.openhab.sql
~~~

### Restore tables
~~~
mysql> CREATE DATABASE tempdatabase;

# mysql -p tempdatabase < /mnt/nfs/mysql/<backupName>.sql
    
mysql> show databases;
mysql> use openhab;
mysql> select * from Items;
mysql> select * from tempdatabase.Items;

##### Use INSERT IGNORE to avoid duplicate entry

mysql> INSERT INTO openhab.Item# (Time, Value) SELECT Time, Value FROM tempdatabase.Item#; 
mysql> drop database tempdatabase;
~~~