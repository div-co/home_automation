Configure openhab to run as a daemon
===================================

Copy openhab file to 

    # cd /etc/init.d/
    # git clone https://github.com/div-co/Home_Automation/blob/Home_Automation/debian/daemons/openhab

Edit Java path

    # vi /etc/init.d/openhab

Make the file executable

    # chmod 755 openhab
    
To enable openhab daemon

    # update-rc.d openhab enable    
