
<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0
-->

<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>

----

# DESCRIPTION
This is a test program for ESP8266 NodeMCU + cascaded MAX7219 dot matrix modules
that creates a scrolling text display.
The program exercises the MD_MAX72XX library by:

* Uses the graphics functions of the MD_MAX72XX library, animate a pair of eyes on 2 matrix modules
* Eyes are coordinated to work together
* Eyes are created to fill all available modules

# PHYSICAL DESIGN
## Hardware
* Four cascaded [MAX7219 dot matrix module][08] purchased from [Banggood][07].
* [HiLetgo New Version ESP8266 NodeMCU][02] purchased from [Amazon][01].

## Wiring
Connections for ESP8266 hardware SPI are:

|   MAX72XX Pin  | ESP8266 Pin  |     Notes / Comments     |
|:--------------:|:------------:|:------------------------:|
| Vcc            | Vin          | Power                    |
| GND            | GND          | Ground                   |
| DIN            | D7           | HSPID or HMOSI           |
| CS or LD       | D8           | HSPICS or HCS            |
| CLK            | D5           | CLK or HCLK              |

# MONITOR
To monitor the clocks activities while operating, power the NodeMCU via
USB connection to a Linux computer and execute the following:

```bash
# start monitoring via USB
screen /dev/ttyUSB0 9600,cs8

# to terminate monitoring, enter
CNTR-a :quit
```

# USAGE
## Test Program
Plug & play.

## Makefile
To build the executable

```bash
# build the executable
make build

# to flash the device
make upload
```

Makefile syntax:

* **=** - set the variable
* **?=** - set the variable only if it's not set/doesn't have a value
* **:=** - set variable as the output an executed shell script
* **+=** - is used for appending more text to variables

# SOURCES
The code for this test routing was taken from [MD_MAX72xx_Eyes][].

The Makefile file was created with the help of these website:

* [Efficient Arduino Programming with Arduino CLI and Visual Studio Code][03]
* [Getting Started with arduino-cli][04]

# DOCUMENTATION
* [MD_MAX72xx LED Matrix Arduino Library][06]
* [Parola for Arduino][05]



[01]:https://www.amazon.com/gp/product/B010O1G1ES
[02]:https://www.miniarduino.com/hiletgo-new-version-esp8266-nodemcu-lua-cp2102-esp-12e-internet-wifi-development-board-open-source-serial-wireless-module-works-great-with-arduino-ide-micropython/
[03]:https://learn.sparkfun.com/tutorials/efficient-arduino-programming-with-arduino-cli-and-visual-studio-code/all
[04]:https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
[05]:https://majicdesigns.github.io/MD_Parola/
[06]:https://majicdesigns.github.io/MD_MAX72XX/
[07]:https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
[08]:https://www.openimpulse.com/blog/products-page/product-category/max7219-led-dot-matrix-module/
[09]:https://github.com/MajicDesigns/MD_MAX72XX/tree/master/examples/MD_MAX72xx_Eyes
