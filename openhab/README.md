# OpenHab2 Customization

## Step 1 [Prerequisites](http://docs.openhab.org/installation/index.html#prerequisites)

### Java

~~~
$ apt-get install default-jre
~~~

### GPIO

- [Configure gpio pins](https://github.com/div-co/home_automation/tree/master/beaglebone#configure-u-boot-cape)

## Step 2 Install And Configure OpenHab2

- [Manual installation](http://docs.openhab.org/installation/linux.html#manual-installation).

~~~
$ rm /opt/openhab2/*.bat
~~~

| Resource | Location | Comment |
|---|---|---|
| GPIO Exports | /sys/class/gpio | |
| openhab.log<br>events.log | /opt/openhab2/userdata/logs | tail -f /opt/openhab2/userdata/logs/openhab.log -f /opt/openhab2/userdata/logs/events.log |
| Karaf console | ssh -p 8101 openhab@localhost | default pass: habopen |
| Service configuration | /opt/openhab2/openhab2 | Ex: change http/https port<br>OPENHAB_HTTP_PORT=1080<br>OPENHAB_HTTPS_PORT=50443 |
| rrd4j databases | /opt/openhab2/userdata |  |
| Site configuration | /opt/openhab2/conf | sitemap, items, things, etc. |
| Service | /lib/systemd/system/openhab2.service |  |
| add-on files | /usr/share/openhab2/addons |  |
| openHAB application | /opt/openhab2 |  |

### OneWire

- Install and configure [owserver](https://github.com/div-co/home_automation/tree/master/debian/owfs)

~~~
$ ls /sys/bus/w1/devices
$ cat /sys/bus/w1/devices/28-0014203825ff/w1_slave
~~~

### Z-Wave

- [Configure](https://github.com/div-co/home_automation/blob/master/openhab/zwave.md)

### Messages brocker (MQTT)

- [Install Mosquitto](https://github.com/div-co/home_automation/tree/master/debian/mqtt)
- Customize Mosquitto Configuration

| Resource | Location | Comment |
|---|---|---|
| Mosquitto Config File | /mnt/mqtt/mosquitto.conf | [Default](https://mosquitto.org/man/mosquitto_passwd-1.html): /etc/mosquitto/mosquitto.conf |
| MQTT Log | /mnt/nfs/logs/mosquitto/mosquitto.log |  |
| Mosquitto Passwd | /mnt/mqtt/passwd | Default: /etc/mosquitto/passwd |

### Troubleshoot

**[ERROR]** [core.karaf.internal.FeatureInstaller] - Failed installing 'openhab-binding-*, openhab-persistence-*, openhab-ui-*, openhab-transformation-map, ... : Resource has no uri

**Fix**

~~~
$ service openhab2 stop
$ rm -rf /opt/openhab2/userdata/cache/*
$ rm -rf /opt/openhab2/userdata/tmp/*
$ rm -rf /opt/openhab2/userdata/kar/*
$ service openhab2 restart
~~~

