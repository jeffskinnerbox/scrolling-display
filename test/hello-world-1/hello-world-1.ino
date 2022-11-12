
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      1.0.0

DESCRIPTION:
    Demonstration program showing the the use of the MD_Parola library display
    scrolling features.

    Simplest program that does - Hello World!

MONITOR:
    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Cntr-a :quit

TESTING:
    Just upload and observe.

USAGE:
    Just upload and observe.

SOURCES:
    Derived from https://github.com/MajicDesigns/MD_Parola/tree/master/examples/Parola_HelloWorld

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

#define DEBUG true    // activate debugging routings (print trace messages on serial port)

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <SPI.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// encoder project's include files (~/src/scrolling-display/test/hello-world-1)
#include "debug.h"


// hardware coordinate mapping
// four common types of matrix modules on the market: PAROLA, GENERIC, ICSTATION, FC16
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW

// number of LED dot matrix modules in the chain
#define MAX_DEVICES 24  // number of modules connected

// NOTE: These pin numbers are for ESO8266 hardware SPI
#define DATA_PIN  D7    // or MOSI
#define CLK_PIN   D5    // or SCK
#define CS_PIN    D8    // or SS

// SPI connection type
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);  // software SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                     // hardware SPI

// version stamp
#define VERSION "1.0.0"
#define VER VERSION " - "  __DATE__ " at " __TIME__
const char version[] = VER;


//------------------------------------------------------------------------------

void setup() {
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect

    PRINT("--------------------------------------------------------------------------------");
    INFO("Starting hello-world-1!");
    INFOS("hello-world-1 version = ", version);
    //INFOS("ESP8266 MAC address = ", WiFi.macAddress());
    INFOD("ESP8266 chip ID = ", ESP.getChipId());

    P.begin();
    P.displayText("Hello World 1!", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);

    Serial.println("Exit setup()");
}


void loop() {
    P.displayAnimate();
}
