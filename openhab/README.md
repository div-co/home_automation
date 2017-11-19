# OpenHab2 Setup

### OneWire

~~~
$ ls /sys/bus/w1/devices
~~~

### GPIO

~~~

~~~

### Resources

| Resource | Location | Comment |
|---|---|---|
| GPIO Exports | /sys/class/gpio | |
| openhab.log<br>events.log | /var/log/openhab2 | tail -f /var/log/openhab2/openhab.log -f /var/log/openhab2/events.log |
| Karaf console | ssh -p 8101 openhab@localhost | default pass: habopen |
| Service configuration | /etc/default/openhab2 |  |
| rrd4j databases | /var/lib/openhab2 |  |
| Site configuration | /etc/openhab2 |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
| add-on files | /usr/share/openhab2/addons |  |
| openHAB application | /usr/share/openhab2 |  |
