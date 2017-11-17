Install SIGAR in order to compile files necessary for openHAB binding to work.

Note. Make sure you have Oracle jdk. If you are using ejre you have to install it first and to point symlinks, PATH and JAVA_HOME to the JDK.

     #export PATH=$PATH:/opt/java/jdk1.8.0_06/bin/
     #export JAVA_HOME=/opt/java/jdk1.8.0_06/

####Install SIGAR

     #cd /usr/src

obtain SIGAR package
  
     # wget http://sourceforge.net/projects/sigar/files/sigar/1.6/hyperic-sigar-1.6.4-src.tar.gz

and extract it to /usr/src/
  
     #tar -zxf hyperic-sigar-1.6.4-src.tar.gz

####Install Ant

     # apt-get install ant

####Compile hyperic-sigar

     # cd hyperic-sigar-1.6.4-src/bindings/java
     # ant

Errors or warnings during compile can be ignored.

Copy 'sigar.jar' and 'libsigar-arm-linux.so' to /openhab/lib/

After you complete the copy you can remove hyperic-sigar from /usr/src/ as well as JDK if you won’t need it.
