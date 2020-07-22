# Install OWFS

### OWSERVER

~~~
$ apt-get install owserver owdir ow-shell
$ vi /etc/owfs.conf
~~~
owfs.conf
~~~
	! server: server = localhost:4304
	#mountpoint = /mnt/1wire
	allow_other
	http: port = 2121
	ftp: port = 2120
	#server: port = 127.0.0.1:4304
	server: w1
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