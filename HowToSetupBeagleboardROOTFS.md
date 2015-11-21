# Table of contents #


# Introduction #
The beagleoncreate program interfaces with 3 sonars and webcam through WiFi connection. The tasks of setting up a working robot lie in:
  * Setting up the wireless connection.
  * Installing the uvc video driver.
  * Compile and install ARToolKitPlus library.
  * Get beagleoncreate from googlecode svn.
This page will guide you through setting up these setups to get a working robot.

# Requirements #
  * Beagleboard-xM Rev. C
  * 4GB microSD card
  * Serial cable
  * [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html)
  * Internet connection
  * (optional) Access to a Linux machine or virtual machine with Linux.

# From Production Copy #
The production copy is what you get on the microSD card that came with your Beagleboard. The 4GB microSD card that came with the Beagleboard should have a label "Beta 4-26". The image file can be found from [circuitco.com](http://circuitco.com/support/index.php?title=BeagleBoard-xM#BeagleBoard-xM_Rev_C_Image) if you don't have the production copy with your microSD card.




When you are ready:
  1. Insert the microSD card with the production copy into the Beagleboard.
  1. Connect the serial cable between the Beagleboard's DB9 connector and your computer's serial port.
  1. Run PuTTY on your computer and connect to the serial port with speed of 115200 baudrate.
  1. Power up the Beagleboard.

Wait a couple minutes and let the Beagleboard start booting up as you can see in the PuTTY terminal window.

To login, type:
```
login as: root
Password: (no password)
```

# Setup WiFi network #
To prepare for the WiFi adapter driver, copy [8192cu.ko](http://beagleoncreate.googlecode.com/svn/doc/8192cu.ko) and [rt3572sta.ko](http://beagleoncreate.googlecode.com/svn/doc/rt3572sta.ko)  and [RT2870STA.dat](http://beagleoncreate.googlecode.com/svn/doc/RT2870STA.dat) under microSD card's rootfs in  `/lib/modules/2.6.32/kernel/drivers/net/wireless/8192cu.ko` and `/lib/modules/2.6.32/kernel/drivers/net/wireless/rt3572sta.ko` and `/etc/Wireless/RT2870STA/RT2870STA.dat`.
You have two options:

1. By giving static IP address to Beagleboard and use [WinSCP](http://winscp.net/eng/download.php) to copy it through Ethernet/wired connection:
> Connect an Ethernet cable between the Beagleboard and a router. Edit `/etc/network/interfaces` :
```
vi /etc/network/interfaces
```
> Append following lines in the `/etc/network/interfaces`:
```
auto usb0
iface usb0 inet static
       address 192.168.1.140
       netmask 255.255.255.0
       network 192.168.1.1
       gateway 192.168.1.1
```
> Also in order to have DNS working create/edit the file `/etc/resolv.conf`.
```
vi /etc/resolv.conf
```
> You should add lines like:
```
nameserver 192.168.1.1
```
> Then reboot:
```
reboot

...
```
> Use [WinSCP](http://winscp.net/eng/download.php) to copy it through Ethernet/wired connection.

2. OR by mounting microSD card on your Linux machine and:
> To copy, run:
```
cp 8192cu.ko /___path/to/microSD/mounting/point___/lib/modules/2.6.32/kernel/drivers/net/wireless/
cp rt3572sta.ko /___path/to/microSD/mounting/point___/lib/modules/2.6.32/kernel/drivers/net/wireless/
cp RT2870STA.dat  /___path/to/microSD/mounting/point___/etc/Wireless/RT2870STA/RT2870STA.dat
```
> After you are done copying the module, unmount and eject the microSD card.

**NOTE: This wifi driver is compiled for linux 2.6.32 kernel. So if your kernel is different, download the source and compile it as per instruction [here](http://yetanotherhackersblog.wordpress.com/2010/11/20/streaming-uvc-webcam-video-over-802-11n-with-a-beagleboard/)**

In the terminal, type in:
```
echo 8192cu >> /etc/modutils/8192cu
echo rt3572sta >> /etc/modutils/rt3572sta
update-modules
reboot

...

```
After the Beagleboard has rebooted, execute:
```
lsmod
```
A list of currently running modules is shown. Make sure `8192cu` and `rt3572sta` is listed.

After the confirmation of the `8192cu` and `rt3572sta` module, remove networkmanager:
```
opkg remove networkmanager --force-removal-of-dependent-packages
```
Without removing network manager, it will force the dhclient not receiving IP address.

Next, edit `/etc/network/interfaces` :
```
vi /etc/network/interfaces
```
Append following lines in the `/etc/network/interfaces`:
```
#auto usb0
iface usb0 inet dhcp

auto wlan0
iface wlan0 inet dhcp
        wpa-driver wext
        wpa-conf /etc/wpa_supplicant.conf

auto wlan1
iface wlan1 inet dhcp
        wpa-driver wext
        wpa-conf /etc/wpa_supplicant.conf

auto ra0
iface ra0 inet dhcp
        wpa-driver wext
        wpa-conf /etc/wpa_supplicant.conf
```
**Notice the `auto usb0` line is commented out.**

Edit `/etc/wpa_supplicant.conf`:
```
vi /etc/wpa_supplicant.conf
```
Append following lines in the `/etc/wpa_supplicant.conf`:
```
network={
        ssid="Create"
        key_mgmt=WPA-PSK
        psk="badbeef123"
        priority=50
        disabled=0
}

network={
        ssid="RedRover"
        key_mgmt=NONE
        priority=20
        disabled=0
}
```
Comment out all other network by adding "#" in front of them.

```
reboot

...

ifconfig
```
Reboot and see if the desired network is connected with the ip address provided.

After you have internet access, run a package list and system update:
```
opkg update
opkg upgrade
```

Install Vim:
```
opkg install vim vim-syntax
```
Copy the [.vimrc](http://beagleoncreate.googlecode.com/svn/doc/.vimrc) under `~/`

# Setup USB-Serial driver #
Execute following lines:
```
opkg install kernel-module-ftdi-sio
reboot

...

lsmod
```
Make sure both `ftdi_sio` and `usbserial` are listed.

# Setup PWM servo driver #
Copy the [servodrive\_1.0-r100.6\_beagleboard.ipk](http://beagleoncreate.googlecode.com/svn/doc/servodrive_1.0-r100.6_beagleboard.ipk) under `~/`
```
cd ~
opkg install servodrive_1.0-r100.6_beagleboard.ipk
```

To auto load `servodrive` module on startup:
```
echo servodrive >> /etc/modutils/servodrive
update-modules
reboot

...

lsmod
```
Make sure `servodrive` is listed

NOTE: This PWM servo driver is compiled for linux 2.6.32 kernel. So if your kernel is different, download the source and refer to http://www.angstrom-distribution.org/building-angstrom for instruction on setting up openembedded toolchain.

# Install USB Video Class driver #
Executing following lines in the terminal:
```
opkg install kernel-module-uvcvideo
echo uvcvideo >> /etc/modutils/uvcvideo
update-modules
reboot

...

lsmod
```
Make sure `uvcvideo` is listed

# Create new user #
To create a new user "beagle", run following lines in the terminal:
```
useradd beagle
passwd beagle
```

To give "beagle" sudo ability, first install sudo:
```
opkg install sudo
```
Then add "beagle" to the sudoer list:
```
vim /etc/sudoers
```
Append following lines into `/etc/sudoers`:
```
root	ALL=(ALL)	ALL
beagle	ALL=(ALL)	ALL
```

Copy the [.bashrc](http://beagleoncreate.googlecode.com/svn/doc/.bashrc) under `/home/beagle/`

Copy the [.vimrc](http://beagleoncreate.googlecode.com/svn/doc/.vimrc) under `/home/beagle/`

**You should log in `beagle` from now on**

# Install ARToolKitPlus library #
The [ARToolKitPlus](https://launchpad.net/artoolkitplus) requires task-native-sdk and cmake package, run following command in the terminal:
```
sudo opkg install task-native-sdk cmake
```

To get gcc to work, run following lines in the terminal:
```
ln -s /lib/libgcc_s.so.1  /lib/libgcc_s.so
ln -s /usr/lib/libstdc++.so.6.0.10  /lib/libstdc++.so
```

Download the [source](https://launchpad.net/artoolkitplus) and place it in the Beagleboard under `~/` then run:
```
cd ~
tar xvjf ARToolKitPlus-2.2.1.tar.bz2
cd ARToolKitPlus-2.2.1/
cmake -G “Unix Makefiles”
make
sudo make install DESTDIR=/
```

# Get code on Beagleboard #
The [beagleoncreate](http://code.google.com/p/beagleoncreate/source/checkout) code is hosted on the googlecode svn repository, run:
```
sudo opkg install subversion
cd ~
svn checkout https://beagleoncreate.googlecode.com/svn/trunk/ beagleoncreate --username yangchuck@gmail.com

...

sudo opkg install opencv opencv-dev libopencv-ml-dev libopencv-objdetect-dev libopencv-legacy-dev gstreamer-dev gst-plugins-base-dev
```

Then try running:
```
cd beagleoncreate/
make
```

Copy these if needed:
  * [artoolkitplus.pc](http://beagleoncreate.googlecode.com/svn/doc/artoolkitplus.pc)
  * [opencv.pc](http://beagleoncreate.googlecode.com/svn/doc/opencv.pc)
  * [gstreamer-0.10.pc](http://beagleoncreate.googlecode.com/svn/doc/gstreamer-0.10.pc)
  * [gstreamer-app-0.10.pc](http://beagleoncreate.googlecode.com/svn/doc/gstreamer-app-0.10.pc)
  * [glib-2.0.pc](http://beagleoncreate.googlecode.com/svn/doc/glib-2.0.pc)
  * [gobject-2.0.pc](http://beagleoncreate.googlecode.com/svn/doc/gobject-2.0.pc)
  * [gthread-2.0.pc](http://beagleoncreate.googlecode.com/svn/doc/gthread-2.0.pc)
  * [gmodule-no-export-2.0.pc](http://beagleoncreate.googlecode.com/svn/doc/gmodule-no-export-2.0.pc)
  * [libxml-2.0.pc](http://beagleoncreate.googlecode.com/svn/doc/libxml-2.0.pc)
  * [gstreamer-base-0.10.pc](http://beagleoncreate.googlecode.com/svn/doc/gstreamer-base-0.10.pc)

into `/usr/lib/pkgconfig`:
```
sudo cp ~/*.pc /usr/lib/pkgconfig/
```
Type `pkg-config --list-all` to double check

After you executed `make` command, it should generate a main file under the folder, type:
```
sudo ./main
```
This will start the beagleoncreate program.


---


After the first copy of image is done. Follow this page to copy file system over:
http://pixhawk.ethz.ch/tutorials/omap/copy_sd_card

**Note: the partition table of the destination sd card should be the same as the source sd card. I am only copying over the 4GB sd card that came with the beagleboard.**

When you are writing from image to sdcard, be sure to **unmount** the partition