
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.3.0

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

REFERENCE MATERIALS:

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

#define ONE_SECOND    1000UL        // milliseconds in one second
#define TWO_SECOND    2000UL        // milliseconds in two second
#define THREE_SECOND  3000UL        // milliseconds in three second
#define ONE_MINUTE    60000UL       // milliseconds in one minute

#define SEALEVELPRESSURE_HPA (1013.25)
#define BME_I2C 0x76
#define BUF_SIZE 80

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

// Put your SSID & Password
const char *ssid = WIFISSID;
const char *pass = WIFIPASS;

// create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);

// WiFiTools object constructor
WiFiTools WT = WiFiTools();

// version stamp
#define VERSION "0.3.0"
#define VER VERSION " - "  __DATE__ " at " __TIME__
const char version[] = VER;


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


// calculate approximate dew point in Celsius
// the dew point is the temperature to which air must be cooled to become saturated with water vapor.
float dew_point_C(float t, float h) {
    return t - (100 - h) / 5;            // approximation
}


// calculate approximate dew point in Fahrenheit
// the dew point is the temperature to which air must be cooled to become saturated with water vapor.
float dew_point_F(float t, float h) {
    return t - 9 / 25 * (100 - h);       // approximation
}



//-------------------------------- WiFi Routines -------------------------------
//----------------------------- Webserver Routines -----------------------------

String SendHTML(float temperature,float humidity,float pressure,float altitude) {
    String ptr = "<!DOCTYPE html> <html>\n";

    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr +="<title>ESP8266 Weather Station</title>\n";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
    ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
    ptr +="</style>\n";
    ptr +="</head>\n";
    ptr +="<body>\n";
    ptr +="<div id=\"webpage\">\n";
    ptr +="<h1>ESP8266 Weather Station</h1>\n";
    ptr +="<p><strong>Temperature: </strong>";
    ptr +=temperature;
    ptr +=" &deg;C</p>";
    ptr +="<p><strong>Humidity: </strong>";
    ptr +=humidity;
    ptr +=" %</p>";
    ptr +="<p><strong>Pressure: </strong>";
    ptr +=pressure;
    ptr +=" hPa</p>";
    ptr +="<p><strong>Altitude: </strong>";
    ptr +=altitude;
    ptr +=" meters</p>";
    ptr +="</div>\n";
    ptr +="</body>\n";
    ptr +="</html>\n";

    return ptr;

}


void handle_OnConnect() {
    char string[BUF_SIZE];
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F;
    altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

    // print temperature
    sprintf(string, "temperature = %.2f °C  /  %.2f °F",
            temperature, C_to_F(temperature));
    INFOS("", string);

    // print humidity
    sprintf(string, "humidity = %.2f %%", humidity);
    INFOS("", string);

    // print estimated dew point
    sprintf(string, "approx. dew point =  %.2f °C  /  %.2f °F",
            dew_point_C(temperature, humidity), dew_point_F(C_to_F(temperature), humidity));
    INFOS("", string);

    // print air pressure
    sprintf(string, "pressure = %.2f % hPa (aka millibars)  /  %.2f inches of mercury",
            pressure, hPa_to_inches(pressure));
    INFOS("", string);

    // print estimated altitude above sea level
    sprintf(string, "approx. altitude above sea level = %.2f meters  /  %.2f feet",
            altitude, meter_to_feet(altitude));
    INFOS("", string);

    // send HTTP status 200 (Ok) and send some text to the browser/client
    server.send(200, "text/html", SendHTML(temperature, humidity, pressure, altitude));

}


void handle_NotFound(){
    server.send(404, "text/plain", "Not found");

    ERROR("404 Error on Webserver.");
}




//--------------------- Error Message Handler for Display ----------------------

// handle errors by displaying a code and then taking action (e.g. restart)
void errorHandler(int error) {

    int i = 0;
    unsigned long tout;                           // time-out time

    switch(error) {
        case 1:
            FATAL("Can't go on without WiFi connection.  Press reset twice to fix.");
            tout = ONE_MINUTE + millis();         // milliseconds of time to display message
            while (millis() < tout) {
                yield();                          // prevent the watchdog timer doing a reboot
            }

            // nothing can be done so restart
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
    char string[BUF_SIZE];
    unsigned long tout;                           // time-out time

    // setup serial port to print debug output
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect
    Serial.println('\n');

    PRINT("--------------------------------------------------------------------------------");
    INFO("Starting Bosch BME280 Pressure - Humidity - Temp Sensor Test!");
    INFOS("bme280 version = ", version);
    INFOS("bme280 MAC address = ", WiFi.macAddress());

    // start reading the bme280 sensor
    if (!bme.begin(BME_I2C)) {
        ERROR("Could not find a valid BME280 sensor, check wiring!");
    }

/*    // report the wifi access point you connect with*/
    /*Serial.print("Connecting to ");*/
    /*Serial.print(ssid);*/

    /*//connect to your local wi-fi network*/
    /*WiFi.begin(ssid, pass);*/

    /*//connect nodemcu wifi your wifi access point*/
    /*while (WiFi.status() != WL_CONNECTED) {*/
        /*delay(1000);*/
        /*Serial.print(".");*/
    /*}*/
    /*Serial.println("\n");*/

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        while (millis() < tout) {
            tout = ONE_SECOND + millis();              // milliseconds of time to display message
            yield();                                   // prevent the watchdog timer doing a reboot
        }
    } else
        errorHandler(1);


    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);

    // start the web server
    server.begin();
    INFO("HTTP server started");

/*    // report the ip address of your web server*/
    /*Serial.print("Connected to WiFi with IP Address: ");*/
    /*Serial.println(WiFi.localIP());*/
    /*Serial.println("\n");*/
}


void loop() {

    server.handleClient();

}

