<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


---

# DESCRIPTION
Test program for ESP8266 NodeMCU + cascaded MAX7219 dot matrix modules to create a scrolling text display.

## The Display
I choose one of the many versions of the [MAX7219 dot matrix module][08]
8x8 dot matrix common cathode LED display modules for my display.
I Choose this because of its modest size
(Module size = length x width x height = 12.8 x 3.2 x 1.3 cm),
low working voltage (5V),
and you can chain 2 or more together to create a [scrolling display][10].
I specific used the MAX7219 Dot Matrix Module 4-in-1 Display
(at [Amazon][06] or much cheaper at [Banggood][07] and where I got mine).
The key tool to drive this display is the [Marco Colli's MajicDesigns MD_Parola][09] library.

# PHYSICAL DESIGN
## Hardware
* Six cascaded [MAX7219 dot matrix module][08] purchased from [Banggood][07].
* [HiLetgo New Version ESP8266 NodeMCU][02] purchased from [Amazon][01].

## Wiring
Connections for ESP8266 hardware SPI are:

|   MAX72XX Pin  | ESP8266 Pin  |   Notes / Comments  |
|:--------------:|:------------:|:-------------------:|
| Vcc            | Vin          | Power               |
| GND            | GND          | Ground              |
| DIN            | D7           | HSPID or HMOSI      |
| CS or LD       | D8           | HSPICS or HCS       |
| CLK            | D5           | CLK or HCLK         |

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

# Build

## Step 1: Add Your Credentials
Using the file `credentials.template` as your starting point,
create a file titled `credentials.h` containing your WiFi network's
SSID and Password.

## Step 2: Load Needed Libraries
The `scrolling-display` program
requires several additional libraries which are not
pre-installed in the Arduino IDE or the `arduino-cli`.
I show below how to do a library install via a`arduino-cli`,
and they will be place in `$HOME/src/arduino/sketechbooks/libraries`

>**NOTE:** You could have install this libary within the Arduino IDE, for example,
>via **Tools** > **Manage Libraries...** > enter "arduino-timer" and install.
>That will work with the Adruino IDE but would **not** have worked with `ardunio-cli`.
>You must use `arduino-cli lib install <library-name>`.

The libraries to be installed are Parola and MAX72xx.
let's first search for them to get the libaries exact names:

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

## Step 3: Compiling the Sketch
Now use the `arduino-cli` and Makefile tools, compile the sketch.

```bash
# compile the sketch
make build

# upload the sketch
make upload
```



[01]:https://www.amazon.com/gp/product/B010O1G1ES
[02]:https://www.miniarduino.com/hiletgo-new-version-esp8266-nodemcu-lua-cp2102-esp-12e-internet-wifi-development-board-open-source-serial-wireless-module-works-great-with-arduino-ide-micropython/
[03]:https://arduinoplusplus.wordpress.com/2017/04/14/parola-a-to-z-adapting-for-different-hardware/

[06]:https://www.amazon.com/WMYCONGCONG-MAX7219-Display-Arduino-Microcontroller/dp/B07FT6MZ7R/Dref=asc_df_B07FT6MZ7R/
[07]:https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
[08]:https://www.openimpulse.com/blog/products-page/product-category/max7219-led-dot-matrix-module/
[09]:https://github.com/MajicDesigns/MD_Parola
[10]:https://www.youtube.com/watch?v=i_8tvPwT6OE
