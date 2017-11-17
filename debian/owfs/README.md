# Install OWFS

### OWSERVER

~~~
$ apt-get install owserver
$ apt-get install ow-shell
$ vi /etc/owfs.conf
~~~
owfs.conf
~~~
	...
	server: server = 192.168.7.2:53131
	server: w1
	...
~~~
Read owserver records
~~~
$ owdir
~~~
Incase of Owserver port issue change Cloud9 port    	
~~~   	
$ vi /opt/cloud9/cloud9.socket
~~~