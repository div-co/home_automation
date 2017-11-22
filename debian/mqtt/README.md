Mosquitto (MQTT broker)
=======================

### Get the latest Mosquitto version
~~~
$ wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
$ sudo apt-key add mosquitto-repo.gpg.key
$ sudo wget http://repo.mosquitto.org/debian/mosquitto-wheezy.list
$ apt-get update
~~~

### Install Mosquitto

~~~
$ apt-get install mosquitto mosquitto-clients
~~~

### Configure mosquitto security and log section
~~~
$ vi /etc/mosquitto/mosquitto.conf
    ...
    [mosquitto.conf]
    log_type warning
    password_file /etc/mosquitto/mosquitto.pwd
    log_dest /var/log/mosquitto.log
~~~

### Add user to password file
~~~
$ mosquitto_passwd -c /etc/mosquitto/passwd hab
~~~

More on Mosquitto configuration file at: http://mosquitto.org/man/mosquitto-conf-5.html