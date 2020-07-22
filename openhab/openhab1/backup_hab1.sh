currentDate=`date +%Y%m%d%H%M`
tar cvzf /mnt/nfs/openhab/$currentDate.HAB.tar.gz /etc/ /root/ /opt/openhab/configurations /opt/openhab/lib /opt/openhab/webapps/images  /opt/java /home /usr/src

#
# Uncomment for testing purposes
#tar cvzf /mnt/nfs/openhab/$currentDate.HAB.tar.gz /usr/src
#
# MySQL 
# databases: 		/var/lib/mysql/
# configuration: 	/etc/mysql/my.cnf
# 
# Java			/opt/java
# 
#
#
#
#



