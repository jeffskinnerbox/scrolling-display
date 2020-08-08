
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    Test program for getting a KY-040 Rotary Encoder working with a ESP8266 NodeMCU
    with a simple switch debouncing scheme.

PHYSICAL DESIGN:
    Hardware
        KY-040 Rotary Encoder - https://www.amazon.com/gp/product/B07G71GWX8/
        HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E - https://www.amazon.com/gp/product/B010O1G1ES

    Wiring
        Connect KY-040 Rotary Encoder "+" pin with NodeMCU Vin (5V)
        Connect KY-040 Rotary Encoder GND pin with with NodeMCU GND
        Connect SKY-040 Rotary Encoder SW pin with NodeMCU D5 pin
        Connect DKY-040 Rotary Encoder DT pin with NodeMCU D6 pin
        Connect CKY-040 Rotary Encoder CLK pin with with NodeMCU D7 pin

MONITOR:
    To monitor the clocks activities while operating, power the NodeMCU via
    USB connection to a Linux computer and execut the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter:

        CNTR-a :quit

TESTING:
    xxxxxxxxxx

USAGE:
    xxxxxxxxxx

REFERENCE MATERIALS:
    https://www.youtube.com/watch?v=cYCTMdUi8P0
    https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/
    https://blog.squix.org/2016/05/esp8266-peripherals-ky-040-rotary-encoder.html

SOURCES:
    https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/

CREATED BY:
    Brainy-Bits
------------------------------------------------------------------------------ */

#define DEBUG true    // activate debugging routings that print trace messages on serial port

// ESP8266 libraries (~/.arduino15/packages/esp8266)
//#include <SPI.h>
//#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
//#include <MD_Parola.h>
//#include <MD_MAX72xx.h>

// encoder project's include files (~/src/scrolling-display/test/encoder)
#include "debug.h"
#include "RotaryEncoder.h"


// rotary encoder connections to nodemcu
//const int PINSW = D5;    // rotary encoder switch (rotary encoder SW)
//const int PINDT = D6;    // DATA signal (rotary encoder DT)
//const int PINCLK = D7;    // CLOCK signal (rotary encoder CLK)
//const int INIT = 0;      // initialization number for displaycounter

RotaryEncoder E = RotaryEncoder(D5, D6, D7, 0);

// version stamp
#define VERSION "0.0.2"
#define VER VERSION " - "  __DATE__ " at " __TIME__
const char version[] = VER;



//------------------------------- Main Routines --------------------------------

void setup() {

    // setup serial port so you can print debug output
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect

    PRINT("\n\r");
    INFO("Starting KY-040 Rotary Encoder!");
    INFOS("encoder version = ", version);

/*    // put rotary encoder current pin state in variables*/
    /*PreviousCLK = digitalRead(PINCLK);*/
    /*PreviousDATA = digitalRead(PINDT);*/

/*    // set the rotary encoder switch pin to use pullup resistors*/
    /*pinMode(PINSW, INPUT_PULLUP);*/
    /*//pinMode(PINSW, INPUT);*/
    /*//pinMode(PINDT, INPUT);*/
    /*//pinMode(PINCLK, INPUT);*/

/*    // intiailize and print the curent value of encoder counter*/
    /*switchpress = HIGH;*/
    /*displaycounter = INIT;*/
    /*INFOD("switchpress = ", switchpress);*/
    /*INFOD("displaycounter = ", displaycounter);*/

}


void loop() {

    E.check_rotary();      // check rotary encoder for change of state
/*    if ((millis() - RotaryTimeOfLastDebounce) > DelayOfDebounce) {*/
        /*check_rotary();      // check rotary encoder for change of state*/

        /*// update pin state variables*/
        /*PreviousCLK = digitalRead(PINCLK);*/
        /*PreviousDATA = digitalRead(PINDT);*/

        /*// set the time of last debounce*/
        /*RotaryTimeOfLastDebounce = millis();*/
    /*}*/

    E.check_switch();      // check button for change of state
    /*// check if rotary encoder switch is being pressed*/
    /*if ((millis() - SwitchTimeOfLastDebounce) > DelayOfDebounce) {*/
        /*check_switch();*/

        /*// set the time of last debounce*/
        /*SwitchTimeOfLastDebounce = millis();*/
    /*}*/

}
