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
$ vi /etc/hostname		
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
$ vi ~/.profile
	...
	alias ll='ls $LS_OPTIONS -l'
	alias l='ls $LS_OPTIONS -lA'
	...
$ source ~/.profile
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
See also [U-boot configuration (capes)](https://github.com/div-co/home_automation/tree/master/beaglebone#configure-u-boot-cape)

### Mount NFS storages

~~~
$ mkdir -p /mnt/nfs/src
$ mkdir -p /mnt/nfs/logs
$ mkdir -p /mnt/nfs/backup
$ mkdir backup
$ vi /etc/fstab
	...
	NFS_SERVER:/NFS_SHARE/src /mnt/nfs/src nfs _netdev,rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/logs /mnt/nfs/logs nfs _netdev,rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/backup /mnt/nfs/backup nfs _netdev,rw,sync,hard,intr  0  0
$ mount -a
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
	Europe/Sofia
~~~
To see or change what timezone your Debian system is configured for
~~~
dpkg-reconfigure tzdata
~~~

### Other packages and tools
- [samba](https://github.com/div-co/home_automation/tree/master/debian/Samba)
- [java](https://github.com/div-co/home_automation/tree/master/debian/java)
- [mysql](https://github.com/div-co/home_automation/blob/master/debian/mysql)
