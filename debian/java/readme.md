How to setup JAVA for BBB
=========================

Download ejre for ARM from oracle.org

Copy eJRE or JDK to /opt/java

	# mkdir /opt/java
	# cd /opt/java

Extract java archive
	
	# tar xfvz .tgz

Export

	# export PATH=$PATH:/<java_dir>/jre/bin/
	# export JAVA_HOME=/<java_dir>/jre/

These two lines can be added to ~/.profile (the users shell profile file) to have it set automatically upon next login.
