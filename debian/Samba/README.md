# Setup a Samba share

### Install SMB server
~~~
$ apt-get install samba
~~~

### Change SMB password for user root
~~~
$ sudo smbpasswd -a root
~~~

### Edit the Samba config file:
~~~
vi /etc/samba/smb.conf
~~~
Add the below lines to smb.conf
~~~
...
[openhab]
comment = openHAB
path = /opt/openhab # Directory to share
public = yes
writable = yes
printable = no
write list = root
...
~~~
### Restart openhab service to apply changes
~~~
$ services openhab restart
~~~
