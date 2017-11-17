# OS configuration

### Connect via Serial or Linux USB Ethernet/RNDIS
~~~
$ ssh debian@192.168.7.2
$ passwd
~~~
default Debian username:password is [debian:temppwd]

### Change the hostname
~~~
$ vi /etc/hosts
$ hostname myhab			
~~~

### Configure Network Adapter (eth0)

~~~
$ vi /etc/network/interfaces
	...
	iface eth0 inet static
		address 192.168.0.7
		netmask 255.255.255.0
		network 192.168.0.0
		gateway 192.168.0.1
	...
~~~

### Configure name resolution

~~~
$ vi /etc/resolv.conf
	...
	nameserver 192.168.0.1
	...
~~~

### Create alias `ll`
~~~
$ vi /etc/profile
	...
	alias ll='ls $LS_OPTIONS -l'
	alias l='ls $LS_OPTIONS -lA'
	...
$ source /etc/profile
~~~

### Mount NFS storages

~~~
$ cd /mnt/
$ mkdir /mnt/nfs
$ mkdir logs
$ mkdir backup
$ vi /etc/fstab
	NFS_SERVER:/NFS_SHARE/src /mnt/nfs/src nfs rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/logs /mnt/nfs/logs nfs rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/backup /mnt/nfs/backup nfs rw,sync,hard,intr  0  0
$ mount -a
~~~

### Packages

~~~
$ apt-get update
$ apt-get upgrade
$ apt-get remove c9-core-installer
$ apt-get install nfs-common portmap locate
$ sudo apt-get remove --purge x11-common
$ sudo apt-get --purge autoremove
~~~

### Setup NTP client

~~~
$ cat /etc/ntp.conf
~~~	
To see servers you are syncing wit
~~~
$ vntpq -p
~~~
Change Time zone
~~~ 
$ vi /etc/timezone
	server Europe/Sofia
~~~
To see or change what timezone your Debian system is configured for
~~~
dpkg-reconfigure tzdata
~~~