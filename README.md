<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.1
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----


# DESCRIPTION
Test program for ESP8266 NodeMCU + cascaded MAX7219 dot matrix modules to create a scrolling text display.
Text will be dynamically pushed to the display via REST, MQTT, or Telegram APIs.

Make the display accessible and controlable via:

* MQTT - e.g. data sent to a MQTT broker
* Network Attached - e.g. `echo -n "24,116" | nc -b -w 0 -u 192.168.1.44 1337`
* Web Accessible - e.g. via a browser interface
* Telnet - e.g. `telnet 192.168.1.44 23`
* Telegram -


# PHYSICAL DESIGN
## Hardware
* Four cascaded [MAX7219 dot matrix module][08] purchased from [Banggood][07].
* [HiLetgo New Version ESP8266 NodeMCU][02] purchased from [Amazon][01].

## Wiring
Connections for ESP8266 hardware SPI are:

|   MAX72XX Pin  | ESP8266 Pin  |       Notes / Comments       |
|:--------------:|:------------:|:----------------------------:|
| Vcc            | Vin          | Power  |
| GND            | GND          | Ground |
| DIN            | D7           | HSPID or HMOSI |
| CS or LD       | D8           | HSPICS or HCS |
| CLK            | D5           | CLK or HCLK |

## Software Settings
During the initial setup, it is not unusual for the LED matrix display
is reversed or upside down, or animations are disjointed across the module boundaries.
The [solution to these problems][03] is typically setting the hardware type
variable properly.
In my case, this was:

```c++
// possible values are: PAROLA, GENERIC, ICSTATION, FC16
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
```


------


# The Display
I choose one of the many versions of the [MAX7219 dot matrix module][08]
8x8 dot matrix common cathode LED display modules for my display.
I Choose this because of its modest size
(Module size = length x width x height = 12.8 x 3.2 x 1.3 cm),
low working voltage (5V),
and you can chain 2 or more together to create a [scrolling display][10].
I specific used the MAX7219 Dot Matrix Module 4-in-1 Display
(at [Amazon][06] or much cheaper at [Banggood][07] and where I got mine).

The key tool to drive this display is the [Marco Colli's MajicDesigns MD_Parola][09] library.

# Setting Hostname
By default, each ESP8266 uses the last six hexdecimal digits of its MAC address
prefixed by `ESP_` to make its hostname unique (e.g. `ESP_B19F40`).
You can change this from within your sketch by calling `WiFi.hostname()`
and passing the function a string containing the new hostname.
This call **MUST** occur before you start the connection with `WiFi.begin()`.

# Using .local Hostnames
A hostname is a name assigned to a device on a network to more easily distinguish
between devices instead of using a IP addresses.
Hostnames can be resolved on a LAN without using a custom DNS
or specifying static IP addresses through the router.
To do so, devices connected to the network must be running a Zero Configuration Networking (Zeroconf) service
which then allows any device to be referenced by adding `.local` to its hostname.

On Linux, the Zeroconf service is called Avahi.
Avahi supports service discovery via the mDNS/DNS-SD protocol suite.
This enables you to plug your computer into a network
and instantly be able to view hostnames and services on your LAN,
much like you would with DNS on the Internet.

The big advantage of mDNS is that we don’t need to know the IP address assigned to the ESP8266 to access the HTTP webserver running on it. On top of that, we don’t need to deploy a dedicated server just to do the resolution of names into IPs.

So, we can just define that the ESP8266 will be listening on something like `myesp.local` and we can just access the server in a web browser by typing `http://myesp.local/path` instead of having to know the IP address. Besides this enhancement, the HTTP web server will work the same as it did before.

In order for this example to work, the machine that is accessing the ESP8266 web server also needs to support mDNS. Otherwise, it won’t be able to send the query needed to receive the resolved IP.

```bash
# install avahi suite
sudo apt-get install avahi-daemon avahi-discover avahi-utils libnss-mdns mdns-scan

# check if avahi daemon is running
sudo systemctl status avahi-daemon

# scan for mDNS/DNS-SD services published on the local network
$ mdns-scan
+ googlerpc._googlerpc._tcp.local
+ Chromecast-1aa07ba95aaa5b96c2c6db5ba7b23e35._googlecast._tcp.local
+ 1aa07ba9-5aaa-5b96-c2c6-db5ba7b23e35._googlezone._tcp.local
+ HP LaserJet P2035 @ desktop._printer._tcp.local
+ HP LaserJet P2035 @ desktop._ipps._tcp.local
+ HP LaserJet P2035 @ desktop._ipp._tcp.local
+ Philips Hue - 4A1515._hue._tcp.local
+ Philips hue - 4A1515._hap._tcp.local
Browsing ...

# browse for mDNS/DNS-SD services using the Avahi daemon
avahi-browse --all
```

# Build
```bash
# compile the program
arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 ~/src/scrolling-display

# flash the board
arduino-cli upload --fqbn esp8266:esp8266:nodemcuv2 --port /dev/ttyUSB0 ~/src/scrolling-display
```

### Step 1: Load Needed Libraries - DONE
The scrolling-display
requires several additional libraries which are not
pre-installed in the Arduino IDE or the `arduino-cli`.
I show below how to do a library install via a`arduino-cli`,
and they will be place in `$HOME/src/arduino/sketechbooks/libraries`

>**NOTE:** You could have install this libary within the Arduino IDE, for example,
>via **Tools** > **Manage Libraries...** > enter "arduino-timer" and install.
>That will work with the Adruino IDE but would **not** have worked with command-line tool `ardunio-cli`.
>Instead, you must use `arduino-cli lib install <library-name>`.

The libraries to be installed are Parola and MAX72xx.
let's first search for them to get the libaries exaqct names

```bash
# search for a library with parola
$ arduino-cli lib search parola
Name: "MD_Parola"
  Author: majicDesigns
  Maintainer: marco_c <8136821@gmail.com>
  Sentence: LED matrix text display special effects
  Paragraph: Implemented using the MD_MAX72xx library for hardware control. Provides functions to simplify the implementation of text special effects on the LED matrix.
  Website: https://github.com/MajicDesigns/MD_Parola
  Category: Display
  Architecture: *
  Types: Contributed
  Versions: [2.2.0, 2.5.0, 2.6.1, 2.6.2, 2.6.4, 2.6.5, 2.6.6, 2.7.0, 2.7.1, 2.7.2, 2.7.3, 2.7.4, 3.0.0, 3.0.1, 3.0.2, 3.1.0, 3.1.1, 3.2.0, 3.3.0]

# search for a library with max72xx
$ arduino-cli lib search max72xx
Name: "MD_MAX72XX"
  Author: majicDesigns
  Maintainer: marco_c <8136821@gmail.com>
  Sentence: Implements functions that allow the MAX72xx (eg, MAX7219) to be used for LED matrices (64 individual LEDs)
  Paragraph: Allows the programmer to use the LED matrix as a pixel addressable display.
  Website: https://github.com/MajicDesigns/MD_MAX72XX
  Category: Device Control
  Architecture: *
  Types: Contributed
  Versions: [2.6.0, 2.10.0, 3.0.0, 3.0.1, 3.0.2, 3.1.0, 3.2.0, 3.2.1]
Name: "MD_MAXPanel"
  Author: majicDesigns
  Maintainer: marco_c <8136821@gmail.com>
  Sentence: Implements functions to manage a panel of MAX72xx based LED modules
  Paragraph: Allows the programmer to use the LED matrix panel as a pixel addressable display for graphics and text.
  Website: https://github.com/MajicDesigns/MD_MAXPanel
  Category: Device Control
  Architecture: *
  Types: Contributed
  Versions: [1.0.0, 1.0.1, 1.1.0, 1.1.1, 1.2.1, 1.2.2, 1.2.3]
Name: "MD_Parola"
  Author: majicDesigns
  Maintainer: marco_c <8136821@gmail.com>
  Sentence: LED matrix text display special effects
  Paragraph: Implemented using the MD_MAX72xx library for hardware control. Provides functions to simplify the implementation of text special effects on the LED matrix.
  Website: https://github.com/MajicDesigns/MD_Parola
  Category: Display
  Architecture: *
  Types: Contributed
  Versions: [2.2.0, 2.5.0, 2.6.1, 2.6.2, 2.6.4, 2.6.5, 2.6.6, 2.7.0, 2.7.1, 2.7.2, 2.7.3, 2.7.4, 3.0.0, 3.0.1, 3.0.2, 3.1.0, 3.1.1, 3.2.0, 3.3.0]
```

Now let's install the required libraries.

>**NOTE:** I selected these specific versions of MD_MAX72XX
>and MD_Parola because of compatibility issues.
>Otherwise I got a dependency error when installing MD_Parola.

```bash
# install the desired library
arduino-cli lib install MD_MAX72XX@3.2.0
arduino-cli lib install MD_Parola@3.1.1
#arduino-cli lib install MD_UISwitch

# check if it installed
$ arduino-cli lib list
Name                          Installed Available Location
Adafruit_GFX_Library          1.7.5     -         user
WiFi                          1.2.7     -         user
Time                          1.6       -         user
Timezone                      1.2.4     -         user
Adafruit_STMPE610             1.1.0     -         user
Adafruit_TouchScreen          1.0.4     -         user
Ethernet                      2.0.0     -         user
ArduinoOTA                    1.0.3     -         user
arduino-timer                 2.0.1     -         user
Adafruit_LED_Backpack_Library 1.1.6     -         user
Servo                         1.1.6     -         user
USBHost                       1.0.5     -         user
Adafruit_ILI9341              1.5.4     -         user
MD_Parola                     3.1.1     3.3.0     user
MD_MAX72XX                    3.2.0     3.2.1     user
```

### Step 2: Compiling the Sketch
Now use the `arduino-cli` and Makefile tools, compile the sketch.

```bash
# compile the sketch
make build

# upload the sketch
make upload
```


--------


Important advice on sketch design...

* [ESP32 OTA tutorial with tricks (incl. OTA debugging](https://www.youtube.com/watch?v=1pwqS_NUG7Q)

**You'll need to install TelnetStream**
arduino-cli lib search telnetstream
arduino-cli lib install "TelnetStream"

**telnet address**
telnet 192.168.1.44
telnet test-ota.local

to terminnate telnet - `ctrl+]` then `quit`.

to terminnate screen - `ctrl+a` then `:quit`.


```bash
$ avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f7-9 | uniq | tr ";" "\n"
test-ota.local
192.168.1.44
8266
```

```bash
python2 /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py -d  -i 192.168.1.44 -a 123 -f test-ota.esp8266.esp8266.nodemcuv2.bin
```


# get ip address, hostname, and ota port
```bash
OTAIP = $(avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f8
| uniq)
OTAHOST = $(avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f7
| uniq)
OTAPORT = $(avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f9
| uniq)

# password for ota
PASSWORD = "123"

# location of the espota.py used for ota flashing
ESPOTATOOL =/home/jeff/arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py


$(ESPOTATOOL) -d -i $(OTAIP) -a $(PASSWORD) -f $(PROG).$(PACKAGE).$(ARCH).$(BOARD).bin
```





# Expect Script
The `expect` command, or Expect scripting language,
is a language that talks with your interactive programs or scripts that require user dialog.
`expect` scripting language works by expecting input,
then the `expect` script will send the response without any user interaction.
The primary `expect` command which used for interaction are:

* **spawn** - The spawn command starts a script or a program like the shell, FTP, Telnet, SSH, SCP, and so on.
* **interact** - The interact command allows you to define a predefined user interaction.
* **expect** - The `expect` command waits for input.
* **send** - The send command sends a reply to a script or a program.

`expect` also has a companion utility called `autoexpect`.
You can use it to build an `expect` script automatically,
but no need to use it here because our dialog with `telnet` is simple.

```bash
# install the expect tools
sudo apt-get install expect
```

In our case here,
we want to reach out to the ESP8266 device via `telnet`,
and once connected, send it the character `r`.
That is it.
With that, the ESP8266 should reboot and we can proceed with the OTA of the firmware.

Here is the `expect` script that works for me:

```
#!/usr/bin/expect -f

# grab the arguments off the command-line of this script
set timeout 10               # timeout if you get no dialog
set ip [lindex $argv 0]      # grab commandline first argument
set port [lindex $argv 1]    # grab commandline second argument

# begin your interaction
spawn telnet $ip $port       # startup telnet
expect "'^]'." sleep .1;     # wait for the characters "'^]'." and sleep before sending
send "R\r";                  # send 'R' to request reboot

# terminate the connection
close
```

This script is placed in the file `answerbot`,
made executable (ie. `chmod a+x answerbot`)
and then used with `Makefile` to suport the `update-ota` target.

Sources of inspiration:

* [Expect command and how to automate shell scripts like magic](https://likegeeks.com/expect-command/)
* [Expect examples and tips](https://www.pantz.org/software/expect/expect_examples_and_tips.html)
* [telnet-expect-example](https://github.com/aguther/example-telnet-expect)


--------


# DeBug.cpp / DeBug.h
During the building of this project,
I got a "undefined reference to template method" error.
The problem emerged when I templatized a member function for my DeBug class.
The articles in the **Sources** below contains
[good description of writing a proper template member function][A],
a [description of the source of the error seen][B] during the build process,
and [some ways to work around this issue][C].
This appear to be a know design flaw in some C++ compilers.

A template is not like a function which can be compiled into byte code.
It is just a pattern to generate such a function.
If you put a template on its own into a `*.cpp` file, there is nothing to compile.
Moreover, the explicite instanciation is actually not a template,
but the starting point to make a function out of the template which ends up in the `*.obj` file.

The bottomline is that you cannot separate the compilation
of the function template (i.e. `*.cpp` file) from its declaration (i.e. `*.h`).
Because of a quirk in the C++ compliler implementation (or should I say bug),
the templeted member function is never compiled in the `cpp` file.
As a rsult, the linker doesn't find it when you use it from another piece of code,
and you ghet the "undefined reference to template method" error.
You will not see this in all C++ compiler implementation,
but its there within the Arduino / ESP tool set.

Source:

* [Template member functions](https://blog.feabhas.com/2014/07/template-member-functions/)
* [Why can templates only be implemented in the header file?](https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file)
* [Why can’t I separate the definition of my templates class from its declaration and put it inside a .cpp file?](https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl)
* [“Undefined reference to” template class constructor [duplicate](https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor)
* [C++: “undefined reference to” templated class function](https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function)

[A]:https://blog.feabhas.com/2014/07/template-member-functions
[B]:https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
[C]:https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
[D]:
[E]:






[01]:https://www.amazon.com/gp/product/B010O1G1ES
[02]:https://www.miniarduino.com/hiletgo-new-version-esp8266-nodemcu-lua-cp2102-esp-12e-internet-wifi-development-board-open-source-serial-wireless-module-works-great-with-arduino-ide-micropython/
[03]:https://arduinoplusplus.wordpress.com/2017/04/14/parola-a-to-z-adapting-for-different-hardware/
[04]:
[05]:
[06]:https://www.amazon.com/WMYCONGCONG-MAX7219-Display-Arduino-Microcontroller/dp/B07FT6MZ7R/Dref=asc_df_B07FT6MZ7R/
[07]:https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
[08]:https://www.openimpulse.com/blog/products-page/product-category/max7219-led-dot-matrix-module/
[09]:https://github.com/MajicDesigns/MD_Parola
[10]:https://www.youtube.com/watch?v=i_8tvPwT6OE
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

