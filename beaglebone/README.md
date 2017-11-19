# Install / Reinstall Beaglebone

### Reinstall Beaglebone Black with Debian

1. Download the [latest image](http://beagleboard.org/latest-images)
2. Disconnect the Ethernet cable.
3. Plug out power supply (incl. USB)
4. Insert SD card with the image (i.e Debian)
5. Hold down S1 and plug power supply. Keep S1 pressed till leds start blinking.
6. Image deployment may take 25-45 min. All leds will light up when it’s ready BBB_4_User_LEDs
7. Remove power supply and eject SD card

If the boar doesn’t start after the above procedure check the image version if it’s compatible with your board and repeat the above steps.

# Configure U-boot (cape)

### Check pinmux assignment
~~~
$ cat /sys/kernel/debug/pinctrl/44e10800.pinmux/pinmux-pins
~~~
Make sure you will not use a pin which is allocated, i.e.<br>`pin 32 (PIN32): 481d8000.mmc (GPIO UNCLAIMED) function pinmux_emmc_pins group pinmux_emmc_pins`

### Build dtbo files with dtc
~~~
$ dtc -W no-unit_address_vs_reg -@ -I dts -O dtb -o /lib/firmware/W1-00A0.dtbo W1-00A0.dts
$ dtc -W no-unit_address_vs_reg -@ -I dts -O dtb -o /lib/firmware/GPIO-00A0.dtbo GPIO-00A0.dts
~~~


### Configure kernel boot options (U-boot)

The below configuration will enable 2 custom capes and will disable HDMI + Audio, Wireless, ADC.
~~~
$ vi /boot/uEnv.txt
	###Additional custom capes
	uboot_overlay_addr4=/lib/firmware/W1-00A0.dtbo
	uboot_overlay_addr5=/lib/firmware/GPIO-00A0.dtbo
	###Disable auto loading of virtual capes (emmc/video/wireless/adc)
	disable_uboot_overlay_video=1
	disable_uboot_overlay_audio=1
	disable_uboot_overlay_wireless=1
	disable_uboot_overlay_adc=1
~~~

### Configure cape manager

~~~
vi /etc/default/capemgr
	# Options to pass to capemgr
	CAPE=W1-00A0,GPIO-00A0
~~~

### Restart BBB

~~~
$ reboot
~~~

### Check cape/pinmux configuration again
~~~
$ dmesg -c
$ cat /sys/kernel/debug/pinctrl/44e10800.pinmux/pinmux-pins
~~~

### Expansion Voltages

| VDD | Description |
|---|---|
| VDD_3V3B | rail is supplied by the LDO on the BeagleBone Black and is the primary power rail for expansion boards. If the power requirement for the capes exceeds the current rating, then locally generated voltage rail can be used. It is recommended that this rail be used to power any buffers or level translators that may be used. |
| VDD_5V | is the main power supply from the DC input jack. This voltage is not present when the board is powered via USB. The amount of current supplied by this rail is dependent upon the amount of current available. Based on the board design, this rail is limited to 1A per pin from the main board. |
| SYS_5V | rail is the main rail for the regulators on the main board. When powered from a DC supply or USB, this rail will be 5V. The available current from this rail depends on the current available from the USB and DC external supplies. |
| Expansion boards PS | It is possible to provide 5V to the main board from an expansion board. By supplying a 5V signal into the VDD_5V rail, the main board can be supplied. This voltage must not exceed 5V. You should not supply any voltage into any other pin of the expansion connectors. Based on the board design, this rail is limited to 1A per pin to the BeagleBone Black. |

To prevent damage the board:
- Do not apply any voltages to any I/O pins when the board is not powered on.
- Do not drive any external signals into the I/O pins until after the VDD_3V3B rail is up.
- Do not apply any voltages that are generated from external sources.
- If voltages are generated from the VDD_5V signal, those supplies must not become active until after the VDD_3V3B rail is up.
- If you are applying signals from other boards into the expansion headers, make sure you power the board up after you power up the BeagleBone Black or make the connections after power is applied on both boards.

Powering the processor via its I/O pins can cause damage to the processor.

### Hignligts

- Power 5V, 3.3V , VDD_ADC(1.8V)
- 3.3V I/O on all signals
- McASP0, SPI1, I2C, GPIO(69 max), LCD, GPMC, MMC1, MMC2, 7 AIN(1.8V MAX), 4 Timers, 4 Serial Ports, CAN0, EHRPWM(0,2),XDMA Interrupt, Power button, 
- Expansion Board ID (Up to 4 can be stacked)

### Boot modes

|| Mode | Description |
|---|---|---|
| 1 | eMMC Boot | This is the default boot mode and will allow for the fastest boot time and will enable the board to boot out of the box using the pre-flashed OS image without having to purchase an microSD card or an microSD card writer. |
| 2 | SD Boot | This mode will boot from the microSD slot. This mode can be used to override what is on the eMMC device and can be used to program the eMMC when used in the manufacturing process or for field updates. |
| 3 | Serial Boot | This mode will use the serial port to allow downloading of the software direct. A separate USB to serial cable is required to use this port. |
| 4 | USB Boot | This mode supports booting over the USB port. |

The board is equipped with a single microSD connector to act as the secondary boot source for the board and, if selected as such, can be the primary boot source. The connector will support larger capacity microSD cards.

### Permanent change the default boot to microSD

If you always want to boot from your microSD card you can invalidate the boot partition on the onboard eMMC. Then the AM335x processor will always fail back to the second boot option in the [default order](https://github.com/div-co/Home_Automation/tree/Home_Automation/Beaglebone#boot-modes) which is the microSD card. This can be done easily by booting your favourite Linux distro on the BeagleBone Black

:warning: The below command invalidate the boot partition on eMMC. :warning:
~~~
$ sudo dd if=/dev/zero of=/dev/mmcblk1 bs=1024 count=1024
~~~
