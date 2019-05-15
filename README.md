<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.2
-->

![work-in-progress](http://worktrade.eu/img/uc.gif "These materials require additional work and are not ready for general use.")

---

# DESCRIPTION
Test program for ESP8266 NodeMCU + cascaded MAX7219 dot matrix modules to create a scrolling text display.
Text will be dynamically pushed to the display via REST, MQTT, or Telegram APIs.

# PHYSICAL DESIGN
## Hardware
* Four cascaded [MAX7219 dot matrix module][08] purchased from [Banggood][07].
* [HiLetgo New Version ESP8266 NodeMCU][02] purchased from [Amazon][01].

## Wiring
Connections for ESP8266 hardware SPI are:

| MAX72XX Pin    | ESP8266 Pin  | Notes / Comments |
|:--------------:|:------------:|:---------------------------------:|
| Vcc            | 3v3          | Power (seem to work at 3.3V) |
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

---

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

# Build
```bash
# compile the program
arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 ~/src/scrolling-display

# flash the board
arduino-cli upload --fqbn esp8266:esp8266:nodemcuv2 --port /dev/ttyUSB0 ~/src/scrolling-display
```



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

