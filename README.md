
![work-in-progress](http://worktrade.eu/img/uc.gif "These materials require additional work and are not ready for general use.")

---

# DESCRIPTION
Test program for ESP8266 NodeMCU + 7-segment display + backpack from Adafruit.
NodeMCU will drive the display as a clock synchronized, via NTP, with a NIST time server.

# PHYSICAL DESIGN
## Hardware
* Adafruit 1.2" 4-Digit 7-Segment Display w/I2C Backpack - https://www.adafruit.com/product/1270
* HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E - https://www.amazon.com/gp/product/B010O1G1ES

## Wiring
* Connect display's backpack I2C clock pin "C" (SCL) with NodeMCU D1 pin
* Connect display's backpack I2C data pin "D" (SDA) pin with NodeMCU D2 pin
* Connect display's backpack GND pin "-" with with NodeMCU GND
* Connect display's backpack VCC pin "+" with NodeMCU Vin (5V)
* Connect display's backpack pin "IO" with with NodeMCU 3.3V

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



[01]:
[02]:
[03]:
[04]:
[05]:
[06]:https://www.amazon.com/WMYCONGCONG-MAX7219-Display-Arduino-Microcontroller/dp/B07FT6MZ7R/Dref=asc_df_B07FT6MZ7R/
[07]:https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
[08]:https://www.openimpulse.com/blog/products-page/product-category/max7219-led-dot-matrix-module/
[09]:https://github.com/MajicDesigns/MD_Parola
[10]:https://www.youtube.com/watch?v=i_8tvPwT6OE

