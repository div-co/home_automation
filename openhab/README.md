# OpenHab2 Setup

- [Prerequisites](http://docs.openhab.org/installation/index.html#prerequisites)
- [Manual installation](http://docs.openhab.org/installation/linux.html#manual-installation).

### Java

~~~
$ apt-get install default-jre
~~~

### OneWire

~~~
$ ls /sys/bus/w1/devices
$ cat /sys/bus/w1/devices/28-0014203825ff/w1_slave
~~~

### GPIO


### Resources (custom installation)

| Resource | Location | Comment |
|---|---|---|
| GPIO Exports | /sys/class/gpio | |
| openhab.log<br>events.log | /opt/openhab2/userdata/logs | tail -f /opt/openhab2/userdata/logs/openhab.log -f /opt/openhab2/userdata/logs/events.log |
| Karaf console | ssh -p 8101 openhab@localhost | default pass: habopen |
| Service configuration | /opt/openhab2/openhab2 | Ex: change http/https port<br>OPENHAB_HTTP_PORT=1080<br>OPENHAB_HTTPS_PORT=50443 |
| rrd4j databases | /opt/openhab2/userdata |  |
| Site configuration | /opt/openhab2/conf | sitemap, items, things, etc. |
| Service | /usr/lib/systemd/system/openhab2.service |  |
| add-on files | /usr/share/openhab2/addons |  |
| openHAB application | /opt/openhab2 |  |
