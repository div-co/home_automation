# Z-Wave

### Z-Wave binding configuration

~~~
$ vi /opt/openhab2/conf/services/zwave.cfg
~~~

### Customize zwave binding

Before proceed with the below steps check if the device exists in [Z-Wave Device Database](http://www.cd-jackson.com/index.php/zwave/zwave-device-database)

1. Uninstall `org.openhab.binding.zwave` binding from Habmin
2. Stop openhab2 service
3. Copy `org.openhab.binding.zwave-2.x.0-SNAPSHOT.jar` to `/opt/openhab2/addons`
4. Modify `org.openhab.binding.zwave-2.x.0-SNAPSHOT.jar`
~~~
$ apt-get install fastjar
$ cd /opt/openhab2/addons
$ fastjar -uf org.openhab.binding.zwave-2.x.0-SNAPSHOT.jar /opt/openhab2/addons/org.openhab.binding.zwave-2.x.0-SNAPSHOT/ESH-INF/thing/tkb_tz06.xml
~~~
5. Start openhab
~~~
$ ssh openhab@localhost -p 8081
openhab>feature:install openhab-transport-serial
~~~

### Nodes
~~~
vi /opt/openhab2/userdata/zwave/nodeXX.xml
~~~