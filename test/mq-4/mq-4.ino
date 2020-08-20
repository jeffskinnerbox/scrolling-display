
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0

DESCRIPTION:

PHYSICAL DESIGN:
    Hardware

    Wiring

MONITOR:
    To monitor the clocks activities while operating, power the NodeMCU via
    USB connection to a Linux computer and execut the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter:

        CNTR-a :quit

USAGE:
    Place the IP address of the device (printed in debug message)
    in a browser to see the web server output of the device.

REFERENCE MATERIALS:
    Dew-point Calculation - http://irtfweb.ifa.hawaii.edu/~tcs3/tcs3/Misc/Dewpoint_Calculation_Humidity_Sensor_E.pdf
    Dew-point Calculation - https://en.wikipedia.org/wiki/Dew_point

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------ */

#define DEBUG true    // activate debugging routings (print trace messages on serial port)

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// bme280 project's include files (current directory)
#include "debug.h"
#include "WiFiTools.h"
#include "credentials.h"


#define SEALEVELPRESSURE_HPA (1013.25)   // average sea-level pressure = 1013.25 mbar/101.325 kPa/29.921 inHg/760.00 mmHg
#define BME_I2C 0x76                     // I2C address of bme280
#define BUF_SIZE 80                      // string buffer size used for sprintf

Adafruit_BME280 bme;

// create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);

// WiFiTools object constructor
WiFiTools WT = WiFiTools();

// version stamp
#define VERSION "0.3.0"
#define VER VERSION " - "  __DATE__ " at " __TIME__
const char version[] = VER;


const int analogInPin = A0;     // nodemcu analog Pin ADC0 = A0


//------------------------------ Helper Routines -------------------------------
// NOTE: To convert hPa to millibars, bar = 100 pascal
// 1 millibar is 1/1000th of a bar, 1 hPa is 100 pascal, therefore 1 hPa = 1 millibar

// convert hPa to inches of mercury
float hPa_to_inches(float p) {
    return p / 33.863886666667;
}


// convert celsius to fahrenheit
float C_to_F(float t) {
    return t * 9 / 5 + 32;
}


// convert meters to feet
float meter_to_feet(float a) {
    return a * 3.28084;
}


/*
// calculate approximate dew point in Celsius
// the dew point is the temperature to which air must be cooled to become saturated with water vapor.
float dew_point_C(float t, float h) {
    float dp;

    // accurate to within about ±1 °C as long as the relative humidity is above 50%
    if (h > 50)
        dp = t - (100 - h) / 5;

    return dp;
}


// calculate approximate dew point in Fahrenheit
// the dew point is the temperature to which air must be cooled to become saturated with water vapor.
float dew_point_F(float t, float h) {
    float dp;

    // accurate to within about ±1 °C as long as the relative humidity is above 50%
    if (h > 50)
        dp = t - 9 / 25 * (100 - h);

    return dp;
}
*/



//----------------------------- Webserver Routines -----------------------------

String PostData(float temperature,float humidity,float pressure,float altitude) {
    String ptr = "<!DOCTYPE html> <html>\n";

    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>ESP8266 Weather Station</title>\n";
    ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
    ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div id=\"webpage\">\n";
    ptr += "<h1>ESP8266 Weather Station</h1>\n";
    ptr += "<p><strong>Temperature: </strong>";
    ptr += temperature;
    ptr += " &deg;C</p>";
    ptr += "<p><strong>Humidity: </strong>";
    ptr += humidity;
    ptr += " %</p>";
    ptr += "<p><strong>Pressure: </strong>";
    ptr += pressure;
    ptr += " hPa</p>";
    ptr += "<p><strong>Altitude: </strong>";
    ptr += altitude;
    ptr += " meters</p>";
    ptr += "</div>\n";
    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;

}


// Send HTTP status 200 (Ok) and send some text to the browser/client
void handle_Root() {

    char string[BUF_SIZE];
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure() / 100.0F;
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

    // print temperature
    sprintf(string, "temperature = %.2f °C / %.2f °F",
            temperature, C_to_F(temperature));
    INFOS("", string);

    // print relative humidity
    sprintf(string, "relative humidity = %.2f %%", humidity);
    INFOS("", string);

    // print air pressure
    sprintf(string, "pressure = %.2f % hPa (aka millibars) / %.2f inches of mercury",
            pressure, hPa_to_inches(pressure));
    INFOS("", string);

    // print estimated altitude above sea level
    sprintf(string, "approx. altitude above sea level = %.2f meters / %.2f feet",
            altitude, meter_to_feet(altitude));
    INFOS("", string);

    // send HTTP status 200 (Ok) and send some text to the browser/client
    server.send(200, "text/html", PostData(temperature, humidity, pressure, altitude));
    INFO("Posted data on webserver.");

}


 // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
void handle_NotFound(){

    server.send(404, "text/plain", "404: Not found");
    ERROR("404 Error on Webserver.");

}




//--------------------- Error Message Handler for Display ----------------------

// handle errors by printing code and then take action (e.g. restart)
void errorHandler(int error) {

    switch(error) {
        case 1:
            ERRORD("Error code in errorHandler: ", error);
            FATAL("Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.reset();
            break;
        default:
            // nothing can be done so restart
            ERRORD("Unknown error code in errorHandler: ", error);
            FATAL("Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.reset();
    }

}



//------------------------------- Main Routines --------------------------------

void setup() {

    // setup serial port to print debug output
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect

    PRINT("\n--------------------------------------------------------------------------------");
    INFO("Starting Bosch BME280 Pressure - Humidity - Temp Sensor Test!");
    INFOS("bme280 version = ", version);
    INFOS("bme280 MAC address = ", WiFi.macAddress());

    // start reading the bme280 sensor
    if (!bme.begin(BME_I2C)) {
        ERROR("Could not find a valid BME280 sensor, check wiring!");
    }

    // attempt to connect and initialize WiFi network
    if (!WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME))
        errorHandler(1);

    server.on("/", HTTP_GET, handle_Root);  // call function when a client requests URI "/"
    server.onNotFound(handle_NotFound);     // call when requests an unknown URI (i.e. something other than the above)

    // start the web server
    server.begin();
    INFO("HTTP server started");

}


void loop() {
    const float MAXVOLT = 3.3;  // maximum voltage possible on pot tap
    const int MAXADC = 1024;    // maximum ADC value for maximum voltage on pot tap
    int sensorValue;            // ADC value read from the analog pin
    float voltage;              // voltage reading of pot tap

    // listen for HTTP requests from clients
    server.handleClient();

    // ADC read of analog pin (integer proportional to max)
    sensorValue = analogRead(analogInPin);

    // voltage value of analog pin
    voltage = MAXVOLT * (float)sensorValue / (float)MAXADC;
    INFOD("voltage = ", voltage);

}

