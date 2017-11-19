# Install OWFS

### OWSERVER

~~~
$ apt-get install owserver owdir ow-shell
$ vi /etc/owfs.conf
~~~
owfs.conf
~~~
	...
	server: server = 192.168.7.2:53131
	server: w1
	...
~~~
In case of Owserver port issue change Cloud9 port    	
~~~   	
$ vi /opt/cloud9/cloud9.socket
~~~

# Read owserver records
~~~
$ owdir
~~~

### DS18B20
~~~
$ ls -al /sys/bus/w1/devices/
~~~