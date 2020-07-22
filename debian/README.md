# OS configuration

### Connect via Serial or Linux USB Ethernet/RNDIS

~~~
ssh debian@192.168.7.2
passwd
~~~
default Debian username:password is [debian:temppwd]

### Rename default Debian user

~~~
adduser temp
adduser temp sudo
exit

ssh temp@192.168.7.2
mv /home/debian/ /home/new_account_name
usermod --login new_account_name --move-home --home /home/new_account_name new_account_name
exit

ssh new_account_name@192.168.7.2
deluser temp
~~~

### Change the hostname

~~~
nano /etc/hosts
nano /etc/hostname		
~~~

### Configure Network Adapter (eth0)

~~~
nano /etc/network/interfaces
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
nano /etc/resolv.conf
	...
	nameserver 192.168.0.1
	...
~~~

### Create alias `ll`
~~~
echo "alias ll='ls $LS_OPTIONS -l'" >> ~/.profile
echo "alias l='ls $LS_OPTIONS -lA'" >> ~/.profile
	...
source ~/.profile
~~~

### Packages

~~~
apt-get remove --purge x11-common c9-core-installer
sudo apt-get --purge autoremove
apt-get update && apt-get upgrade
apt-get install nfs-common portmap locate
~~~
See also [U-boot configuration (capes)](https://github.com/div-co/home_automation/tree/master/beaglebone#configure-u-boot-cape)

### Mount NFS storages

~~~
mkdir -p /mnt/nfs/{src,logs,backup}
mkdir -p /opt/openhab2
nano /etc/fstab
	...
	NFS_SERVER:/NFS_SHARE/openhab2 /opt/openhab2 nfs _netdev,rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/src /mnt/nfs/src nfs _netdev,rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/logs /mnt/nfs/logs nfs _netdev,rw,sync,hard,intr  0  0
	NFS_SERVER:/NFS_SHARE/backup /mnt/nfs/backup nfs _netdev,rw,sync,hard,intr  0  0
mount -a
~~~

### Setup NTP client

~~~
cat /etc/ntp.conf
~~~	
To see servers you are syncing wit
~~~
vntpq -p
~~~
Change Time zone
~~~ 
cat "Europe/Sofia" > /etc/timezone
~~~
To see or change what timezone your Debian system is configured for
~~~
dpkg-reconfigure tzdata
~~~

### Other packages and tools
- [samba](https://github.com/div-co/home_automation/tree/master/debian/Samba)
- [java](https://github.com/div-co/home_automation/tree/master/debian/java)
- [mysql](https://github.com/div-co/home_automation/blob/master/debian/mysql)
