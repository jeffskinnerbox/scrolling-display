
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

// ESP8266 libraries (~/.arduino15/packages/esp8266)
//#include <SPI.h>
//#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
//#include <MD_Parola.h>
//#include <MD_MAX72xx.h>

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "debug.h"


// rotary encoder connections to nodemcu
const int PinSW =  D5;    // rotary encoder switch (rotary encoder SW)
const int PinDT =  D6;    // DATA signal (rotary encoder DT)
const int PinCLK = D7;    // CLOCK signal (rotary encoder CLK)

// variable to debounce rotary encoder
long RotaryTimeOfLastDebounce = 0;
long SwitchTimeOfLastDebounce = 0;
int DelayOfDebounce = 1UL;         // 1 milliseconds (0.001 seconds)
//int DelayOfDebounce = 10UL;        // 10 milliseconds (0.01 seconds)
//int DelayOfDebounce = 100UL;       // 100 milliseconds (0.1 seconds)

// store previous pins states
int PreviousCLK;
int PreviousDATA;

int displaycounter;      // store current counter value
const int Init = 0;      // initialization number displaycounter

int switchpress;         // if LOW, switch is being pressed

// version stamp
#define VERSION "0.0.2"
#define VER VERSION " - "  __DATE__ " at " __TIME__
const char version[] = VER;



//------------------------------ Helper Routines -------------------------------

// Check if the rotary encoder has moved and update the counter as required.
// Increased counter if moving in clockwise direction and decrease the counter
// if moving in the counter clockwise direction.
void check_rotary() {

    if ((PreviousCLK == LOW) && (PreviousDATA == HIGH)) {
        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
            displaycounter++;
            INFOD("displaycounter = ", displaycounter);
        }

        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
            displaycounter--;
            INFOD("displaycounter = ", displaycounter);
        }
    }

    if ((PreviousCLK == HIGH) && (PreviousDATA == LOW)) {
        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
            displaycounter++;
            INFOD("displaycounter = ", displaycounter);
        }

        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
            displaycounter--;
            INFOD("displaycounter = ", displaycounter);
        }
    }

    if ((PreviousCLK == HIGH) && (PreviousDATA == HIGH)) {
        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
            displaycounter++;
            INFOD("displaycounter = ", displaycounter);
        }

        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
            displaycounter--;
            INFOD("displaycounter = ", displaycounter);
        }
    }

    if ((PreviousCLK == LOW) && (PreviousDATA == LOW)) {
        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
            displaycounter++;
            INFOD("displaycounter = ", displaycounter);
        }

        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == HIGH)) {
            displaycounter--;
            INFOD("displaycounter = ", displaycounter);
        }
    }

}


// check if rotary encoder switch is being pressed
void check_switch() {

    if ((digitalRead(PinSW) == LOW) && (switchpress == HIGH)) {
        displaycounter = Init;             // reset encoder counter to zero
        switchpress = LOW;                 // switch has been pressed, now reset
        INFOD("button pressed: displaycounter = ", displaycounter);
    }

    if ((digitalRead(PinSW) == HIGH) && (switchpress == LOW)) {
        switchpress = HIGH;                 // switch has been released
        INFOD("button released: displaycounter = ", displaycounter);
    }

}



//------------------------------- Main Routines --------------------------------

void setup() {

    // setup serial port so you can print debug output
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect

    PRINT("\n\r");
    INFO("Starting KY-040 Rotary Encoder!");
    INFOS("encoder version = ", version);

    // put current pin state in variables
    PreviousCLK = digitalRead(PinCLK);
    PreviousDATA = digitalRead(PinDT);

    // set the switch pin to use pullup resistors
    pinMode(PinSW, INPUT_PULLUP);
    //pinMode(PinSW, INPUT);
    //pinMode(PinDT, INPUT);
    //pinMode(PinCLK, INPUT);

    // intiailize and print the curent value of encoder counter
    switchpress = HIGH;
    displaycounter = Init;
    INFOD("switchpress = ", switchpress);
    INFOD("displaycounter = ", displaycounter);

}


void loop() {

    // if enough time has passed, check the rotary encoder
    if ((millis() - RotaryTimeOfLastDebounce) > DelayOfDebounce) {
        check_rotary();      // check rotary encoder for change of state

        // update pin state variables
        PreviousCLK = digitalRead(PinCLK);
        PreviousDATA = digitalRead(PinDT);

        // set the time of last debounce
        RotaryTimeOfLastDebounce = millis();
    }

    // check if rotary encoder switch is being pressed
    if ((millis() - SwitchTimeOfLastDebounce) > DelayOfDebounce) {
        check_switch();

        // set the time of last debounce
        SwitchTimeOfLastDebounce = millis();
    }

}
