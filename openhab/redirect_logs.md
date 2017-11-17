Redirect openhab logs
=====================

Edit logback.xml

    <appender name="FILE" class="ch.qos.logback.core.FileAppender">
		<!--
		<file>${openhab.logdir:-logs}/openhab.log</file>
		-->
		<file>/mnt/nfs/openhab/openhab.log</file>
		<encoder>
		<pattern>%d{HH:mm:ss.SSS} %-5level %logger{30}[:%line]- %msg%n%ex{5}</pattern>
		</encoder>
    </appender>
