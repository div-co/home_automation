Backup HAB important files
==========================

###Create file backup with the following content

Select Date format and backup file name
~~~
# Date format
# `date +%Y-%m-%d-%H:%M`
# Modify date format as well as file name due to a name lenght limits.
currentDate=`date +%Y-%m-%d`
tar cvzf //<backup_location>/$currentDate.hab.tar.gz /etc/ /root/ /opt/openhab2 /home /usr/src
~~~

