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

// Arduino libraries (~/src/arduino/libraries)
#include <Arduino.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// encoder project's include files (~/src/scrolling-display/test/encoder)
#include "debug.h"
#include "RotaryEncoder.h"



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create RotaryEncoder
RotaryEncoder::RotaryEncoder(int pinsw, int pindt, int pinclk, int init) {

    PinSW = pinsw;      // rotary encoder switch (rotary encoder SW)
    PinDT = pindt;      // DATA signal (rotary encoder DT)
    PinCLK = pinclk;    // CLOCK signal (rotary encoder CLK)
    Init = init;        // initialization number for displaycounter
    //
    // put rotary encoder current pin state in variables
    PreviousCLK = digitalRead(PinCLK);
    PreviousDATA = digitalRead(PinDT);

    // set the rotary encoder switch pin to use pullup resistors
    pinMode(PinSW, INPUT_PULLUP);
    //pinMode(PinSW, INPUT);
    //pinMode(PinDT, INPUT);
    //pinMode(PinCLK, INPUT);

    // intiailize and print the curent value of encoder counter
    switchpress = HIGH;
    displaycounter = Init;
    INFOD("switchpress = ", switchpress);
    INFOD("displaycounter = ", displaycounter);

    // variable used to debounce rotary encoder
    RotaryTimeOfLastDebounce = 0;
    SwitchTimeOfLastDebounce = 0;
    //DelayOfDebounce = 1UL;         // 1 milliseconds (0.001 seconds)
    DelayOfDebounce = 10UL;        // 10 milliseconds (0.01 seconds)
    //DelayOfDebounce = 100UL;       // 100 milliseconds (0.1 seconds)

    clockwise = true;   // rotary turned clockwise

}


// Destructor to delete RotaryEncoder
RotaryEncoder::~RotaryEncoder() {
}



// ------------------------------ Private Methods ------------------------------



// ------------------------------- Public Methods ------------------------------

// Check if the rotary encoder has moved and update the counter as required.
// Increased counter if moving in clockwise direction and decrease the counter
// if moving in the counter clockwise direction.
void RotaryEncoder::check_rotary() {

    // if enough time has passed, check the rotary encoder
    if ((millis() - RotaryTimeOfLastDebounce) > DelayOfDebounce) {
        if ((PreviousCLK == LOW) && (PreviousDATA == HIGH)) {
            if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                displaycounter++;
                INFOD("A1-displaycounter = ", displaycounter);
            }

            if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                displaycounter--;
                INFOD("A2-displaycounter = ", displaycounter);
            }
        }

        if ((PreviousCLK == HIGH) && (PreviousDATA == LOW)) {
            if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
                displaycounter++;
                INFOD("A3-displaycounter = ", displaycounter);
            }

            if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
                displaycounter--;
                INFOD("A4-displaycounter = ", displaycounter);
            }
        }

        if ((PreviousCLK == HIGH) && (PreviousDATA == HIGH)) {
            if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
                displaycounter++;
                INFOD("A5-displaycounter = ", displaycounter);
            }

            if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
                displaycounter--;
                INFOD("A6-displaycounter = ", displaycounter);
            }
        }

        if ((PreviousCLK == LOW) && (PreviousDATA == LOW)) {
            if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                //displaycounter++;
                INFOD("A7-displaycounter = ", displaycounter);
            }

            if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == HIGH)) {
                displaycounter--;
                INFOD("A8-displaycounter = ", displaycounter);
            }
        }

        // update pin state variables
        PreviousCLK = digitalRead(PinCLK);
        PreviousDATA = digitalRead(PinDT);

        // set the time of last debounce
        RotaryTimeOfLastDebounce = millis();
}


/*    if ((PreviousCLK == LOW) && (PreviousDATA == HIGH)) {*/
        /*if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {*/
            /*displaycounter++;*/
            /*INFOD("A1-displaycounter = ", displaycounter);*/
        /*}*/

        /*if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {*/
            /*displaycounter--;*/
            /*INFOD("A2-displaycounter = ", displaycounter);*/
        /*}*/
    /*}*/

    /*if ((PreviousCLK == HIGH) && (PreviousDATA == LOW)) {*/
        /*if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {*/
            /*displaycounter++;*/
            /*INFOD("A3-displaycounter = ", displaycounter);*/
        /*}*/

        /*if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {*/
            /*displaycounter--;*/
            /*INFOD("A4-displaycounter = ", displaycounter);*/
        /*}*/
    /*}*/

    /*if ((PreviousCLK == HIGH) && (PreviousDATA == HIGH)) {*/
        /*if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {*/
            /*displaycounter++;*/
            /*INFOD("A5-displaycounter = ", displaycounter);*/
        /*}*/

        /*if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {*/
            /*displaycounter--;*/
            /*INFOD("A6-displaycounter = ", displaycounter);*/
        /*}*/
    /*}*/

    /*if ((PreviousCLK == LOW) && (PreviousDATA == LOW)) {*/
        /*if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {*/
            /*//displaycounter++;*/
            /*INFOD("A7-displaycounter = ", displaycounter);*/
        /*}*/

        /*if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == HIGH)) {*/
            /*displaycounter--;*/
            /*INFOD("A8-displaycounter = ", displaycounter);*/
        /*}*/
    /*}*/

}


// check if rotary encoder button is being pressed
void RotaryEncoder::check_switch() {

    // if enough time has passed, check the the button press
    if ((millis() - SwitchTimeOfLastDebounce) > DelayOfDebounce) {
        if ((digitalRead(PinSW) == LOW) && (switchpress == HIGH)) {
            displaycounter = Init;             // reset encoder counter to zero
            switchpress = LOW;                 // switch has been pressed
            INFOD("button pressed: displaycounter = ", displaycounter);
        }

        if ((digitalRead(PinSW) == HIGH) && (switchpress == LOW)) {
            switchpress = HIGH;                 // switch has been released
            INFOD("button released: displaycounter = ", displaycounter);

            // re-set the time of last debounce
            SwitchTimeOfLastDebounce = millis();
        }
    }


/*    if ((digitalRead(PinSW) == LOW) && (switchpress == HIGH)) {*/
        /*displaycounter = Init;             // reset encoder counter to zero*/
        /*switchpress = LOW;                 // switch has been pressed*/
        /*INFOD("button pressed: displaycounter = ", displaycounter);*/
    /*}*/

    /*if ((digitalRead(PinSW) == HIGH) && (switchpress == LOW)) {*/
        /*switchpress = HIGH;                 // switch has been released*/
        /*INFOD("button released: displaycounter = ", displaycounter);*/
    /*}*/

}


