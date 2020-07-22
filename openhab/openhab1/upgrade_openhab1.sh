#!/bin/sh
OH_ROOT=/opt/openhab
OH_BAK=/mnt/nfs/openhab/openhab-bak
SOURCES=/mnt/nfs/openhab
ADDONS="org.openhab.action.mail*.jar \
		org.openhab.binding.configadmin*.jar \
		org.openhab.binding.exec*.jar \
		org.openhab.binding.gpio*.jar \
		org.openhab.binding.http*.jar \
		org.openhab.binding.lgtv*.jar\
		org.openhab.binding.mqtt*.jar \
		org.openhab.binding.networkhealth*.jar \
		org.openhab.binding.ntp*.jar \
		org.openhab.binding.onewire*.jar \
		org.openhab.binding.openenergymonitor*.jar \
		org.openhab.binding.owserver*.jar \
		org.openhab.binding.serial*.jar \
		org.openhab.binding.systeminfo*.jar \
		org.openhab.binding.tcp*.jar \
		org.openhab.binding.zwave*.jar \
		org.openhab.io.gpio*.jar \
		org.openhab.io.habmin*.jar \
		org.openhab.io.multimedia.tts.freetts*.jar \
		org.openhab.binding.lgtv*.jar \
		org.openhab.persistence.mysql*.jar" 

############################## DO NOT CHANGE ANYTHING AFTER HERE #####################################

echo Removing previous distribution sources
rm -rf addons.zip
rm -rf runtime.zip

mv $SOURCES/*addons*.zip $SOURCES/addons.zip
mv $SOURCES/*runtime*.zip $SOURCES/runtime.zip
			
echo Removing old backup $OH_BAK
rm -rf $OH_BAK/

echo Stopping the service...
service openhab stop

echo Copy $OH_ROOT to $OH_BAK
mv $OH_ROOT/ $OH_BAK
mkdir $OH_ROOT

echo Unziping runtime.zip to $OH_ROOT
unzip -o $SOURCES/runtime.zip -d $OH_ROOT/ 

mkdir $OH_ROOT/logs

echo Copy Sigar /lib, Configurations, scripts
cp -Rf $OH_BAK/lib/ $OH_ROOT/
cp -Rf $OH_BAK/configurations/ $OH_ROOT/
cp -Rf $OH_BAK/webapps/habmin/ $OH_ROOT/webapps/
cp -Rf $OH_BAK/webapps/images/ $OH_ROOT/webapps/
cp $OH_BAK/addons/*habmin* $OH_ROOT/addons/

echo Unzip addons
unzip -o $SOURCES/addons.zip $ADDONS -d $OH_ROOT/addons
chmod 777 $OH_ROOT/*.sh
chmod -R 777 $OH_ROOT/configurations

#echo Starting the service...
#service openhab start

echo Rebooting the server
reboot
