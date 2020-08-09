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
## Hardware
* [HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E](https://www.amazon.com/gp/product/B010O1G1ES)
* [Diymore BME280 Temperature Humidity Barometric Pressure Sensor Module](https://www.amazon.com/gp/product/B01LETIESU/)

## Wiring
Connections for ESP8266 hardware are as follows:

|         BME280        | NodeMCU Pin |   Notes / Comments    |
|:---------------------:|:-----------:|:---------------------:|
| VIN                   | 3.3V        | Power                 |
| GND                   | GND         | Ground                |
| SCL                   | D1          | I2C clock on NodeMCU  |
| SDA                   | D2          | I2C data on NodeMCU   |

# SOFTWARE DESIGN
## Sources
Program code and/or ideas taken from:

* [Create A Simple ESP8266 Weather Station With BME280](https://lastminuteengineers.com/bme280-esp8266-weather-station/)

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

