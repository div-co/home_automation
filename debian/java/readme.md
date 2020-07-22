# How to setup JAVA for BBB

### Apt Installation 
~~~
$ apt-get install default-jre
~~~

### Manual Installation

- Download ejre for ARM from oracle.org
- Copy eJRE or JDK to /opt/java
~~~
$ mkdir /opt/java
$ cd /opt/java
~~~
Extract java archive
~~~
$ tar xfvz .tgz
~~~
Export
~~~
$ export PATH=$PATH:/<java_dir>/jre/bin/
$ export JAVA_HOME=/<java_dir>/jre/
~~~
These two lines can be added to ~/.profile (the users shell profile file) to have it set automatically upon next login.

### openhab2/conf/services/mysql.cfg
~~~
# the database url like 'jdbc:mysql://<host>:<port>/<database>' (without quotes)
url=jdbc:mysql://192.168.50.5:3306/openhab?serverTimezone=Europe/Sofia

# the database user
user=

# the database password
password=

# the reconnection counter
reconnectCnt=1

# the connection timeout (in seconds)
waitTimeout=28800

# Use MySQL Server time to store item values (=false) or use openHAB Server time (=true).
# For new installations, its recommend to set "localtime=true".
# (optional, defaults to false)
#localtime=true
~~~