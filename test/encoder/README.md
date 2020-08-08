<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>

---

# DESCRIPTION
Test program for getting a KY-040 Rotary Encoder working with a ESP8266 NodeMCU
with a simple switch debouncing scheme.

A [rotary encoder][01] (sometime called a shaft encoder),
is a low tech input device but offers a nice way to adjust settings
without using too much of your valuable GPIO pins.

Here we are specifically using the [KY-040 Rotary Encoder][02].
At first glance you might mistake the rotory encoder for an adjustable resistor, aka potentiometer.
It has a knob which you can turn,
but unlike real potentiometers you can turn round and round without ever getting to a blocking limit.
Also feel certain positions where the encoder will slide into.
It also has a switch which can be closed when pressing the knob vertically.
A rotory encoders advantage is that they can be used relatively easy in a digital system
as opposed to an analog/digital converter (ADC) reading the values from a potentiometer.

There is a surprisingly wide range of [supporting software for rotory encoders][03].
But rotary encoders bit a bouncing in them,
resulting inaccracies and even sometimes sensing a turn one way
when in fact you turned the other way.
Here I'm using some code that try to get the best results possible.

# PHYSICAL DESIGN
## Hardware
* [KY-040 Rotary Encoder](https://www.amazon.com/gp/product/B07G71GWX8/)
* [HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E](https://www.amazon.com/gp/product/B010O1G1ES)

## Wiring
Connections for ESP8266 hardware are as follows:

| KY-040 Rotary Encoder | ESP8266 Pin |   Notes / Comments    |
|:---------------------:|:-----------:|:---------------------:|
| +                     | Vin         | Power                 |
| GND                   | GND         | Ground                |
| SW                    | D5          | rotary encoder switch |
| DT                    | D6          | DATA signal           |
| CLK                   | D7          | CLOCK signal          |

# SOFTWARE DESIGN
## Sources
Program code and/or ideas taken from:

* [ESP8266 Peripherals: KY-040 Rotary Encoder](https://blog.squix.org/2016/05/esp8266-peripherals-ky-040-rotary-encoder.html)

## Libraries
Arduino IDE or ESP libraries used:

* None required

# SERIAL MONITOR
To monitor the programs activities while operating, power the NodeMCU via
USB connection to a Linux computer and execute the following: `screen /dev/ttyUSB0 9600,cs8cls`.
To terminate monitoring, enter: `CNTR-a :quit`.

# TESTING
Turn the knob and press the switch on the KY-040 Rotary Encoder
and you can observer the changing values on the serial monitor.

# USAGE
No commands or parameter are entered.

# SOURCES
* [Best code to use with a KY-040 Rotary Encoder? Let’s find out!](https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/)

# DOCUMENTATION
* [Best code to use with a KY-040 Rotary Encoder? Let’s find out!](https://www.youtube.com/watch?v=cYCTMdUi8P0)
* [Keyes KY-040 Arduino Rotary Encoder User Manual](http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/keyes-ky-040-arduino-rotary-encoder-user-manual/)

## Other
* [KY040 rotary decoder library](https://github.com/dmachard/KY040-rotary)
* [Arduino Esp32 / ESP8266 MULTIPLE Encoder Library](https://github.com/igorantolic/ai-esp32-rotary-encoder)
* [Rotary Encoder: How to use the Keys KY-040 Encoder on the Arduino](https://www.best-microcontroller-projects.com/rotary-encoder.html)
* [Bouncing switches in your Arduino projects? Easy ways to fix it!](https://www.youtube.com/watch?v=dBbR1Oirhz8)



[01]:https://en.wikipedia.org/wiki/Rotary_encoder
[02]:https://blog.squix.org/2016/05/esp8266-peripherals-ky-040-rotary-encoder.html
[03]:https://playground.arduino.cc/Main/RotaryEncoders/

