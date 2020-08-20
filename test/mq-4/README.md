<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.4.0
-->

<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>

---

# DESCRIPTION
Test program for ESP8266 NodeMCU + BME280,
a integrated environmental sensing unit from Bosch that combines
sensors for pressure, humidity, and temperature.

The [BME280][01], which will work with the [Adafruit BME280 library][02],
is a low power, small size integrated environmental sensor.
The unit combines individual high linearity, high accuracy sensors for pressure, humidity and temperature.
The BME280 sensor measures atmospheric pressure from
300hPa to 1100hPa (±1 hPa absolute accuracy),
0% to 100% humidity (±3% accuracy)
and -40°C to 85°C temperature (±1.0°C accuracy).
Because pressure changes with altitude, and the pressure measurements are so good,
you can also use it as an altimeter from 0ft to 30Kft (±1 meter accuracy)!

The BME280 module has a two-wire I2C interface for communication.
The default I2C address of the BME280 module is `0x76`
(it can be changed to `0x77` with [this procedure][03]).
The operating voltage of the BME280 module is from 3.3V to 5V.

# PHYSICAL DESIGN

## BME280 Environmental Sensor
The Bosch BME280 sensor (the upgrade to the BMP085/BMP180/BMP183)
is an environmental sensor for reading temperature, barometric pressure and humidity!
It can measuring humidity with ±3% accuracy,
barometric pressure with ±1 hPa absolute accuraccy,
and temperature with ±1.0°C accuracy.

Because pressure changes with altitude, and the pressure measurements are so good,
you can also use it as an altimeter with  ±1 meter or better accuracy!

## MQ-4 Methane Gas Sensor
The MQ-4 Methane Gas Sensor is a gas sensor that detects the presence of methane gas.
Its highly sensitivity to methane and natural gas but also slight sensitivity to alcohol and smoke.
The sensor is simple to wire up, using one analog voltage input pin from your microcontroller.
The MQ-4 detects the concentration of methane gas in the air and ouputs its reading as an analog voltage.
The concentration sensing range of 200 ppm to 10,000 ppm (suitable for gas stove leak detection),
operates at temperatures from -10 to 50°C, and consumes less than 150 mA at 5 V.

The sensor has ajustable sensitivity using the onboard potentiometer.

* [Flammable Gas Sensor - Model：MQ-4](https://cdn.sparkfun.com/datasheets/Sensors/Biometric/MQ-4%20Ver1.3%20-%20Manual.pdf)

* Wemos and MQ-4 gas sensor - http://www.esp8266learning.com/wemos-mq-4-gas-sensor.php
* MQ4 GAS SENSOR – METHANE NATURAL GAS MONITOR WITH MQ3 / MQ4 SENSORS - http://www.geekstips.com/mq4-sensor-natural-gas-methane-arduino/

## Hardware
* [HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E](https://www.amazon.com/gp/product/B010O1G1ES)
* [Diymore BME280 Temperature Humidity Barometric Pressure Sensor Module](https://www.amazon.com/gp/product/B01LETIESU/)
* [MQ-4 Methane and Natural Gas (CNG) Analog Sensor Breakout Board](https://www.amazon.com/ACROBOTIC-Methane-Natural-Breakout-Raspberry/dp/B07CRHWBSJ)

## Wiring
Connections for ESP8266 hardware are as follows:

|         BME280        | NodeMCU Pin |   Notes / Comments    |
|:---------------------:|:-----------:|:---------------------:|
| VIN                   | 3.3V        | Power                 |
| GND                   | GND         | Ground                |
| SCL                   | D1          | I2C clock on NodeMCU  |
| SDA                   | D2          | I2C data on NodeMCU   |


|          MQ-4         | NodeMCU Pin |   Notes / Comments    |
|:---------------------:|:-----------:|:---------------------:|
| VCC                   | 5V          | Power                 |
| GND                   | GND         | Ground                |
| D0                    | not connected | digital output       |
| A0                    | analog pin  | analog pin            |

## NodeMCU ADC Specifications
The NodeMCU has one ADC pin, `A0`, that is accessible, giving it the ability to read analog signals.
The ADC pin has a 10-bit resolution, which means you’ll get values between 0 and 1023
and the ADC voltage range is 0 to 3.3V.

>**NOTE:** If your using the ESP8266 bare chip, the ADC pin input voltage range is 0 to 1V.
>However, the NodeMCU (aka ESP8266 development board) come with an internal voltage divider,
>so the input range is 0 to 3.3V.

# SOFTWARE DESIGN
## Sources
Program code and/or ideas taken from:

* [Create A Simple ESP8266 Weather Station With BME280](https://lastminuteengineers.com/bme280-esp8266-weather-station/)
* [ESP8266 ADC – Read Analog Values with Arduino IDE, MicroPython and Lua](https://randomnerdtutorials.com/esp8266-adc-reading-analog-values-with-nodemcu/)

THe NodeMCU (specifically the ESP8266-12E) have one ADC pin, A0 (aka TOUT), that is easily accessible.
This means that those ESP8266 boards can read analog signals.
The ADC pin has a 10-bit resolution, which means you’ll get values between 0 and 1023.
The ESP8266 ADC pin input voltage range is 0 to 1V if you’re using the bare chip.
However, a development board like the NodeMCU come with an internal voltage divider,
so the input range is 0 to 3.3V.

## Libraries
Arduino IDE or ESP libraries used:

* [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library)
* [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)

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
* [Making a Simple Weather Web Seerver Using ESP8266 and BME280](http://embedded-lab.com/blog/making-simple-weather-web-server-using-esp8266-bme280/)
* For information on barometric pressure measurement, see [RPi - IoT Weather Station](https://www.hackster.io/mjrobot/rpi-iot-weather-station-8c409c)
and [BMP180 Barometric Pressure Sensor Hookup](https://learn.sparkfun.com/tutorials/bmp180-barometric-pressure-sensor-hookup-)



[01]:https://www.bosch-sensortec.com/bst/products/all_products/bme280
[02]:https://github.com/adafruit/Adafruit_BME280_Library
[03]:https://lastminuteengineers.com/bme280-arduino-tutorial/#procedure-to-change-i2c-address

