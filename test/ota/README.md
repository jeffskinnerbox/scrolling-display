<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.4.0
-->

![work-in-progress](http://worktrade.eu/img/uc.gif "These materials require additional work and are not ready for general use.")


-----

* [ESP8266 Over The Air (OTA) Programming Without Reset Using Arduino IDE (Mac OSX and Windows)](https://www.youtube.com/watch?v=3aB85PuOQhY&t=20s)
* [ESP8266 Arduino Core: OTA Updates](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)

* [OTA ala Andreas Spiess](https://www.youtube.com/c/AndreasSpiess/search?query=OTA)

* [Putting The Firmware In Your Firmware](https://hackaday.com/2020/09/15/putting-the-firmware-in-your-firmware/)
* [Even More Firmware In Your Firmware](https://hackaday.com/2020/10/01/even-more-firmware-in-your-firmware/)




When working with microcontrollers with wireless capabilities,
a nice add-on features is the ability to update the firmware wirelessly.
OTA (Over the Air) update is the process of uploading firmware to an
ESP module using a WiFi connection rather than a serial port.
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

# Serial over WiFi
I would like to be able to view serial monitor over wifi, but "Serial monitor is not supported on network ports such as <device ip address>"
The Serial.print() sends data directly to the hardware serial pins so you need something that has a physical connection to those pins in order to read the data.
It creates a Telnet port on the ESP chip. You can then connect using a standard Telnet client to access the debugging output.

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

## mDNS Tools
* `avahi-browse` - Browse for mDNS/DNS-SD services using the Avahi daemon
* `avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f8 | uniq`
- this script will scan your network for Arduino OTA services and list IP address when found
(derived from [here][05]).


-----


# Procedure to Use OTA with ESP8266
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
If everything is OK, it will output the dynamic IP address obtained from your router. Note it down.

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

## Step X: Upload New Firmware Over-The-Air (OTA)

http://192.168.1.44:8266




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
[11]:
[12]:
[13]:
[14]:
[15]:
[16]:
[17]:
[18]:
[19]:
[20]:
