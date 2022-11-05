<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.4.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----


* [How To Easily Set Up Secure OTA Firmware Updates on ESP32](https://hackaday.com/2021/11/29/how-to-easily-set-up-secure-ota-firmware-updates-on-esp32/)

* [ESP8266 Over The Air (OTA) Programming Without Reset Using Arduino IDE (Mac OSX and Windows)](https://www.youtube.com/watch?v=3aB85PuOQhY&t=20s)
* [ESP8266 Arduino Core: OTA Updates](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)

* [OTA ala Andreas Spiess](https://www.youtube.com/c/AndreasSpiess/search?query=OTA)

* [Putting The Firmware In Your Firmware](https://hackaday.com/2020/09/15/putting-the-firmware-in-your-firmware/)
* [Even More Firmware In Your Firmware](https://hackaday.com/2020/10/01/even-more-firmware-in-your-firmware/)





When working with microcontrollers with wireless capabilities,
a nice add-on features is the ability to update the firmware wirelessly.
Upload OTA (Over the Air) is the process of uploading firmware to an
ESP module using a WiFi connection rather than uploading on a serial port.
Such functionality becomes extremely useful in case of limited or no physical access to the module.
OTA may be done using: Arduino IDE, Web Browser, or HTTP Server.

OTA functionality is extremely useful in case of no physical access to the ESP module.
It helps reduce the amount of time spent for updating each ESP module at the time of maintenance.
Another important feature of OTA is that one central location can send
an update to multiple ESPs sharing same network.
The only disadvantage is that you have to add an extra code for OTA with every sketch you upload,
so that you’re able to use OTA in the next update.

The first firmware upload has to be done over a serial port.
If the OTA routines are correctly implemented in the sketch,
then all subsequent uploads may be done over the air.

* [Tech Note 025 - ESP8266 OTA Sketch Updating](https://www.youtube.com/watch?v=ZVA1pV6WUQg)
* [ESP-01/ESP-07 4Mbyte Memory Upgrade and OTA](https://www.youtube.com/watch?v=xyc1gCjguRU)
* [Internet of Things with ESP8266 #4: Upload Programs Over the Air (OTA)](https://www.youtube.com/watch?v=GoQXOLB50HA)
* [ESP8266 Programming Over The Air (OTA) Using Wi-Fi With Arduino IDE (Mac OSX and Windows)](https://www.youtube.com/watch?v=gFK2EDNpIeM)

# Serial Debugging over WiFi
One of the motivators for me to use OTA is to free myself of bring my device under development
onto my workbench for coding/testing iterations.
With OTA, I can remotely push firmware to my device,
but how does one do the testing without the device physically near me
so I have no access to the serial port for debugging?
The ESP `Serial.print()` command sends data via serial communications and then out the USB.
Instead, I need the equivalent to be sent over TCP/IP via WiFi.
The old workhorse, Telnet, to the rescue!

[Telnet][12] is a simple, light-weight client-server TCP/IP protocol that can be used
to open a serial connection on a remote computer.
Telnet establishes a virtual terminal connection emulator,
using protocols to act like a physical terminal connected to a machine.

>**NOTE:** Telnet is not a secure protocol and is unencrypted.
>By monitoring a user's connection, anyone can access a person's username, password,
>and other private information that is typed over the Telnet session in plaintext.
>[Telnet has limited uses today][12], but remains popular for [testing for open ports][13],
>and because of its light weight, it remains popular for serial communications with embedded systems.

Fortunately, the ESP/Arduino libraries come with a [`telnet` implementation][14].
Combine `telnet` with the traditional `Serial` debugging and you'll have all the bases covered.
Some have built packages to do this,
examples are [here][15] and [here][16]; both are in the Arduino Library Manager.
You may want to build my own to get the greatest flexibility and control the software blot.
I'm choosing to use [TelnetStream][16].

Here are some good sources on this topic:

* [Wireless Serial for ESP8266 using Telnet](https://www.youtube.com/watch?v=j9yW10OcahI)
* [ESP32 OTA tutorial with tricks (incl. OTA debugging](https://www.youtube.com/watch?v=1pwqS_NUG7Q)
* [ESPTelnet](https://github.com/LennartHennigs/ESPTelnet)
* [TelnetStream: Arduino Stream implementation over Telnet for OTA logging and debugging][16]
* [RemoteDebug Library](https://github.com/JoaoLopesF/RemoteDebug#usage)

# Flash Size
OTA is only possible with flash sizes of over 1M bytes
and the flash has to be at least double the size of the program your trying to upload.
So you need ESP chips like ESP-12E and NodeMCU 1.0 (both have 4 Mbyte flash)
but the ESP-01 will not work (unless you [upgrade the flash chip][09]).

# Multicast Domain Name System (mDNS)
The Arduino OTA libraries make use of [Multicast Domain Name System (mDNS)][02]
is one of the features of [Bonjour][03] and the Linux equivalent called [Avahi][04].
mDNS resolves host names to IP addresses within small networks that do not include a local name server.
It is a [zero-configuration service][06],
using essentially the same programming interfaces,
packet formats and operating semantics as the unicast [Domain Name System (DNS)][07].

When an mDNS client needs to resolve a host name,
it sends an IP multicast query message that asks the host having that name to identify itself.
That target machine then multicasts a message that includes its IP address.
All machines in that subnet can then use that information to update their mDNS caches.

On the Internet at large, a DNS name such as `www.apple.com` must be resolved
to a numbered IP address before your computer can connect to the server.
A domain name system (DNS) server normally provides this name resolution service for your computer.
Your Internet service provider or network administrator normally provides a local DNS server for your use.

Conversely, mDNS allows computers on a local network, such as home or small office,
to look up each others' names and addresses without a DNS server.
Each computer knows its own name and responds to requests for that name automatically via IP multicast.

Anywhere that a you could normally use a DNS name such as `www.google.com`,
you could instead enter the Bonjour name of the computer on the local network.
To indicate that the name should be looked up using local multicast instead of a standard DNS query,
all Bonjour host names end with the extension `.local`.
This helps avoid confusion between names that are local computers your LAN, resolved via mDNS,
and globally unique DNS names on the Internet.

## OTA-program SPIFFS on the ESP8266
* [OTA-program SPIFFS on the ESP8266](https://arduino.stackexchange.com/questions/74691/ota-program-spiffs-on-the-esp8266)

## Searching for OTA Devices Using mDNS Tools
[Avahi][04] is a system which facilitates service discovery on a local network via the mDNS/DNS-SD protocol suite.
[`avahi-browse`][17] looks for mDNS/DNS-SD services, using the Avahi daemon,
and can return the hostnames of devices being managed via mDNS.
Since OTA uses mDNS,
this can be very useful when your searching for OTA -enabled device.
For example (derived from [here][05] and [here][18]):

```bash
# dump information about arduino tcp services
$ avahi-browse _arduino._tcp --resolve --parsable --terminate

+;wlan0;IPv4;test-ota;_arduino._tcp;local
+;eth0;IPv4;test-ota;_arduino._tcp;local
=;eth0;IPv4;test-ota;_arduino._tcp;local;test-ota.local;192.168.1.44;8266;"tcp_check=no" "ssh_upload=no" "board="ESP8266_NODEMCU"" "auth_upload=yes"
=;wlan0;IPv4;test-ota;_arduino._tcp;local;test-ota.local;192.168.1.44;8266;"tcp_check=no" "ssh_upload=no" "board="ESP8266_NODEMCU"" "auth_upload=yes"

# scan your network for arduino ota services and list ip address when found
$ avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f8 | uniq

192.168.1.44
```



-------



# Uploading Firmware Over-The-Air (FOTA)
Firmware over the air, FOTA, lets you upload your firmware (the sketch) wirelessly.
It can be useful if your devices are not accessible and you want to update your firmware.

>**NOTE:** A bootloader (also known as a boot program or bootstrap loader)
>is system software that loads into the working memory of a microprocessor after start-up
>and let you upload your firmware by USB or wirelessly, or Serial Peripheral Interface (SPI).
>The bootloader resides in memory on an MCU in an area of ROM or flash memory
>that is protected from getting written over without special tools.
>This whole process of restarting your processor, executing the bootloader,
>and uploading the firmware is called "booting".

We use the `esptool` command line utility to communicate with the
ESP8266 ROM based bootloader in microcontrollers.
This bootloader assumes that the firmware to be loaded is originating from the USB interface,
and the `esptool` negotiates with the ESP8266 bootloader and provides the firmware over the USB.
The `espota.py` Python script does the equivalent, except it does FOTA over the WiFi connection.

Procedure to Use OTA with ESP8266

1. **Upload Basic OTA Firmware Serially -**
Upload the sketch containing OTA firmware (i.e. `BasicOTA.ino`) serially.
It’s a mandatory step, so that you’re able to do the next updates/uploads over-the-air.
2. **Upload New Sketch Over-The-Air -**
Now, you can upload new sketches to the ESP8266 from Arduino IDE over-the-air.

## Install OTA Bootloader
You need to install a OTA bootloader to do you

### Step 1: Create OTA Firmware File
In order to upload firmware to your ESP8266 wirelessly for the first time,
you have to upload the `BasicOTA.ino` sketch example first.
You will find the `BasicOTA.ino` within the Arduino IDE at
**File** > **Examples** > **Arduino OTA** > **BasicOTA.ino** (see video [here][08]),
or take it from the ESP8266 examples here
`~/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino`,
or from Rui Santos ["Random Nerd Tutorials" blog][01],
or best yet, copy my sketch below to a file titled `basic-ota.ino`
within your build directory:

### Step 2: Upload OTA Routine Serially
The factory image in ESP8266 doesn’t have an OTA Upgrade capability. So, you need to load the OTA firmware on the ESP8266 through serial interface first.
It’s a mandatory step to initially update the firmware, so that you’re able to do the next updates/uploads over-the-air.

Using the `Makefile`, execute the following:

```bash
# upload the ota file
make upload
```

### Step X: Serial Monitor
Now, open the Serial Monitor at a baud rate of 9600.
And press the RST button on ESP8266.
If everything is OK, it will output the dynamic IP address obtained from your router.
Note it down.

```bash
# open serial monitor program
$ screen /dev/ttyUSB0 9600,cs8cls

Booting ...
version / creation date = Jan 17 2021 at 21:14:37
ESP8266 chip ID = 24f9fd
ESP8266 MAC address = BC:DD:C2:24:F9:FD

ESP8266 is OTA update ready
initial-ota
192.168.1.44

# hit the esp8266 rest button if you see nothing
```

If you happen not to connected to the serial port on the ESP8266,
but the device is running on your LAN,
you can find the hostname, IP address, and port via this method
(derived from [here][10]):

```bash
# list the hostname, ip address, and port
$ avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f7-9 | uniq | tr ";" "\n"
initial-ota.local
192.168.1.44
8266
```

## Install Program via OTA Bootloader
The OTA bootloader utility

```bash
# enter directory where firmware to be uploaded via ota
cd ~/src/scrolling-display/test/ota/test-ota

# get help for tool
$ python2 ~/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py --help
Usage: espota.py [options]

Transmit image over the air to the esp8266 module with OTA support.

Options:
  -h, --help            show this help message and exit

  Destination:
    -i ESP_IP, --ip=ESP_IP
                        ESP8266 IP Address.
    -I HOST_IP, --host_ip=HOST_IP
                        Host IP Address.
    -p ESP_PORT, --port=ESP_PORT
                        ESP8266 ota Port. Default 8266
    -P HOST_PORT, --host_port=HOST_PORT
                        Host server ota Port. Default random 10000-60000

  Authentication:
    -a AUTH, --auth=AUTH
                        Set authentication password.

  Image:
    -f FILE, --file=FILE
                        Image file.
    -s, --spiffs        Use this option to transmit a SPIFFS image and do not
                        flash the module.

  Output:
    -d, --debug         Show debug output. And override loglevel with debug.
    -r, --progress      Show progress output. Does not work for ArduinoIDE

# using data from previous step, upload via ota
python2 ~/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py -d  -i 192.168.1.44 -a 123 -f test-ota.esp8266.esp8266.nodemcuv2.bin
```


http://192.168.1.44:8266


--------


## Step X: Wireless Serial Monitoring
OTA is great for remotely push firmware to devices,
but how does one do the testing without the device physically near me
so I have no access to the serial port for debugging?
The [old workhorse, Telnet][12], to the rescue!
Telnet establishes a virtual terminal
and communicates to open a serial connection over WiFi.
The ESP/Arduino libraries come with a [`telnet` implementation][14].
Combine `telnet` with the traditional `Serial` debugging and you'll have all the bases covered.
There are Arduino packages making this easy,
and I'm choosing to use [TelnetStream][16].

Here are some good sources on this topic:

* [Wireless Serial for ESP8266 using Telnet](https://www.youtube.com/watch?v=j9yW10OcahI)
* [ESP32 OTA tutorial with tricks (incl. OTA debugging](https://www.youtube.com/watch?v=1pwqS_NUG7Q)
* [ESPTelnet](https://github.com/LennartHennigs/ESPTelnet)
* [TelnetStream: Arduino Stream implementation over Telnet for OTA logging and debugging][16]
* [RemoteDebug Library](https://github.com/JoaoLopesF/RemoteDebug#usage)

# Step X: Install TelnetStream
arduino-cli lib install telnetstream




[01]:https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
[02]:http://www.multicastdns.org/
[03]:https://developer.apple.com/bonjour/
[04]:http://www.avahi.org/
[05]:https://github.com/esp8266/Arduino/issues/3553
[06]:http://www.practicallynetworked.com/sharing/configure_and_use_avahi_and_linux.htm
[07]:http://compnetworking.about.com/od/dns_domainnamesystem/f/dns_servers.htm
[08]:https://www.youtube.com/watch?v=ViDUfrM6-wM
[09]:https://www.youtube.com/watch?v=xyc1gCjguRU
[10]:https://github.com/esp8266/Arduino/issues/3553
[11]:https://www.lifewire.com/what-does-telnet-do-2483642
[12]:https://www.digitalcitizen.life/simple-questions-what-telnet-what-can-it-still-be-used/
[13]:https://support.n4l.co.nz/s/article/How-to-use-Telnet-to-Check-the-Status-of-Ports
[14]:https://github.com/LennartHennigs/ESPTelnet
[15]:https://github.com/JoaoLopesF/RemoteDebug#usage
[16]:https://github.com/jandrassy/TelnetStream
[17]:https://www.systutorials.com/docs/linux/man/1-avahi-browse/
[18]:https://github.com/platformio/platformio-core/issues/463
[19]:
[20]:
