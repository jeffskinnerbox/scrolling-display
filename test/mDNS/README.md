<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----


# DESCRIPTION

## ESP8266 mDNS
In networking world it is difficult to remember IP address of each website and computer.
To solve this problem, the Domain Name System (DNS) is used to make human understandable names.
In the world of microcontrollers like the ESP8266, when using ESP as web server, for example,
its equally difficult to remember IP address of ESP8266, particularly when using DHCP.
Many ESP8266 application doesn’t have display interface
and they are not easy to access to know its IP address.
To overcome this problem Multicast DNS (mDNS) is used.

mDNS provides the ability to perform DNS-like operations on the local network
in the absence of any conventional Unicast DNS server.
In addition, Multicast DNS designates a portion of the DNS namespace to be free for local use,
without the need to pay any annual fee,
and without the need to set up delegations or
otherwise configure a conventional DNS server to answer for those names.

mDNS, or multicast DNS, is a service that helps you find your Ethernet devices
on the network without knowing their IP address.
It is a standard protocol widely implemented for discovery of devices on your network.
mDNS won’t let you find devices any where on the Internet,
but mDNS can help if your sensor and computer are on the same local network.


# PHYSICAL DESIGN


## Hardware

## Wiring

# SERIAL MONITOR
To monitor the programs activities while operating, power the NodeMCU via
USB connection to a Linux computer and execute the following: `screen /dev/ttyUSB0 9600,cs8`.
To terminate monitoring, enter: `CNTR-a :quit`.

# TESTING
Point a browser at the IP address shown on the serial monitor.
The NodeMCU should serve up a web page showing temperature, humidity, pressure and altitude from BME280.

# USAGE
No commands or parameter are entered.

# DOCUMENTATION

