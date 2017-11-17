
# Home automation project

My home automation project includes the following steps

1. Temperature monitoring (onewire) and control
2. Appliances & Lights control
3. Monitoring

### Hardware

- BeagleBone Black (rev. C + IND 4G) [install](https://github.com/div-co/home_automation/tree/master/beaglebone#install--reinstall-beaglebone) | [U-boot (capes)](https://github.com/div-co/home_automation/tree/master/beaglebone#configure-u-boot-cape)
- Z-Wave (Aeon z-stick S2, TKB TZ46)
- [Arduino](https://github.com/div-co/home_automation/tree/master/arduino/readme.md) NANO + Ethernet
- 1-Wire sensors
- ESP8266

### Software

- [Debian](https://github.com/div-co/home_automation/tree/master/debian#os-configuration)
- Oracle Java (for ARM)
- MySQL
- [openHAB](https://github.com/div-co/home_automation/tree/master/openhab), HABmin
- Owserver / OWFS
- SIGAR (provide [System Info](https://github.com/div-co/home_automation/tree/master/debian/systeminfo)rmation)
- [Mosquitto](https://github.com/div-co/home_automation/blob/master/debian/mqtt/README.md.md) (MQTT Broker)
- Openenergy Monitor
