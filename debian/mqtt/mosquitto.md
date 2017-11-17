Mosquitto (MQTT broker)
=======================

To get the latest Mosquitto version

    wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
    sudo apt-key add mosquitto-repo.gpg.key
    sudo wget http://repo.mosquitto.org/debian/mosquitto-wheezy.list
    apt-get update

Install Mosquitto

    apt-get install mosquitto

Configure mosquitto security and log section

    vi /etc/mosquitto/mosquitto.conf
    
    [mosquitto.conf]
    log_type warning
    password_file /etc/mosquitto/mosquitto.pwd
    log_dest /var/log/mosquitto.log

More on Mosquitto configuration gile at: http://mosquitto.org/man/mosquitto-conf-5.html
