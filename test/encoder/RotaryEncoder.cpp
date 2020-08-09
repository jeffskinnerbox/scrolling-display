
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.3.0

DESCRIPTION:
    Test program for getting a KY-040 Rotary Encoder working with a ESP8266 NodeMCU
    with a simple software switch debouncing scheme.

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

REFERENCE MATERIALS:
    https://www.youtube.com/watch?v=cYCTMdUi8P0
    https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/
    https://blog.squix.org/2016/05/esp8266-peripherals-ky-040-rotary-encoder.html

SOURCES:
    https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/

CREATED BY:
    Brainy-Bits and modified by jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------ */

#define DEBUG false    // activate debugging routings (print trace messages on serial port)

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

    // initialize pin assignment to rotary encoder
    PinSW = pinsw;                 // rotary encoder switch (rotary encoder SW)
    PinDT = pindt;                 // DATA signal (rotary encoder DT)
    PinCLK = pinclk;               // CLOCK signal (rotary encoder CLK)
    Init = init;                   // initialization number for displaycounter

    // intiailize the state of the rotary encoder
    switchpress = false;           // if true, switch is being pressed
    displaycounter = Init;         // initialize the encoder turn count
    clockwise = true;              // rotary turned clockwise

    // initialize variable used to debounce rotary encoder
    RotaryTimeOfLastDebounce = 0;
    SwitchTimeOfLastDebounce = 0;
    DelayOfDebounce = 10UL;        // 10 milliseconds (0.01 seconds)

    // initialize rotary encoder current pin state
    PreviousCLK = digitalRead(PinCLK);
    PreviousDATA = digitalRead(PinDT);

    // set the rotary encoder switch pin to use pullup resistors
    pinMode(PinSW, INPUT);
    pinMode(PinSW, INPUT_PULLUP);    // pulled HIGH to the logic voltage (5V or 3.3V for most cases)
    pinMode(PinCLK, INPUT);
    pinMode(PinCLK, INPUT_PULLUP);
    pinMode(PinDT, INPUT);
    pinMode(PinDT, INPUT_PULLUP);

    INFOD("DelayOfDebounce = ", DelayOfDebounce);
    INFOD("Init = ", Init);
    INFOB("clockwise = ", clockwise);
    INFOB("switchpress = ", switchpress);
    INFOD("displaycounter = ", displaycounter);
    PRINT("--------------------------------------------------------------------------------\n\r");

}


// Destructor to delete RotaryEncoder
RotaryEncoder::~RotaryEncoder() {
}



// ------------------------------ Private Methods ------------------------------



// ------------------------------- Public Methods ------------------------------

// Check if the rotary encoder has moved and update the counter as required.
// Increased counter if moving in clockwise direction and decrease the counter
// if moving in the counter clockwise direction.
int RotaryEncoder::check_rotary() {

    // if enough time has passed, check the rotary encoder
    if ((millis() - RotaryTimeOfLastDebounce) > DelayOfDebounce) {
/*        if ((PreviousCLK == LOW) && (PreviousDATA == HIGH)) {*/
            //if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                //clockwise = true;              // rotary turned clockwise
                //displaycounter++;
                //INFOD("A1-displaycounter = ", displaycounter);
            //}

            //if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                //clockwise = false;              // rotary turned counter-clockwise
                //displaycounter--;
                //INFOD("A2-displaycounter = ", displaycounter);
            //}
        //}

        //if ((PreviousCLK == HIGH) && (PreviousDATA == LOW)) {
            //if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
                //clockwise = true;              // rotary turned clockwise
                //displaycounter++;
                //INFOD("A3-displaycounter = ", displaycounter);
            //}

            //if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
                //clockwise = false;              // rotary turned counter-clockwise
                //displaycounter--;
                //INFOD("A4-displaycounter = ", displaycounter);
            //}
        //}

        //if ((PreviousCLK == HIGH) && (PreviousDATA == HIGH)) {
            //if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
                //clockwise = true;              // rotary turned clockwise
                //displaycounter++;
                //INFOD("A5-displaycounter = ", displaycounter);
            //}

            //if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
                //clockwise = false;              // rotary turned counter-clockwise
                //displaycounter--;
                //INFOD("A6-displaycounter = ", displaycounter);
            //}
        //}

        //if ((PreviousCLK == LOW) && (PreviousDATA == LOW)) {
            //if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                // I have found commenting out lines can give better results
                //clockwise = true;              // rotary turned clockwise
                //displaycounter++;
                //INFOD("A7-displaycounter = ", displaycounter);
            //}

            //if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == HIGH)) {
                //clockwise = false;              // rotary turned counter-clockwise
                //displaycounter--;
                //INFOD("A8-displaycounter = ", displaycounter);
            //}
        /*}*/

        if ((PreviousCLK == LOW) && (PreviousDATA == HIGH)) {
            if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                clockwise = true;              // rotary turned clockwise
                displaycounter++;
                INFOD("A1-displaycounter = ", displaycounter);
            } else if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                clockwise = false;              // rotary turned counter-clockwise
                displaycounter--;
                INFOD("A2-displaycounter = ", displaycounter);
            }
        } else if ((PreviousCLK == HIGH) && (PreviousDATA == LOW)) {
            if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
                clockwise = true;              // rotary turned clockwise
                displaycounter++;
                INFOD("A3-displaycounter = ", displaycounter);
            } else if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
                clockwise = false;              // rotary turned counter-clockwise
                displaycounter--;
                INFOD("A4-displaycounter = ", displaycounter);
            }
        } else if ((PreviousCLK == HIGH) && (PreviousDATA == HIGH)) {
            if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
                clockwise = true;              // rotary turned clockwise
                displaycounter++;
                INFOD("A5-displaycounter = ", displaycounter);
            } else if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
                clockwise = false;              // rotary turned counter-clockwise
                displaycounter--;
                INFOD("A6-displaycounter = ", displaycounter);
            }
        } else if ((PreviousCLK == LOW) && (PreviousDATA == LOW)) {
            if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
                // I have found commenting out lines can give better results
                //clockwise = true;              // rotary turned clockwise
                //displaycounter++;
                INFOD("A7-displaycounter = ", displaycounter);
            } else if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == HIGH)) {
                clockwise = false;              // rotary turned counter-clockwise
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

    return displaycounter;

}


// check if rotary encoder button is being pressed
bool RotaryEncoder::check_switch() {

    // if enough time has passed, check the the button press
    if ((millis() - SwitchTimeOfLastDebounce) > DelayOfDebounce) {
        if ((digitalRead(PinSW) == LOW) && (switchpress == false)) {
            switchpress = true;                // button has been pressed
            INFOD("button pressed: displaycounter = ", displaycounter);
        }

        if ((digitalRead(PinSW) == HIGH) && (switchpress == true)) {
            switchpress = false;                 // button has been released
            INFOD("button released: displaycounter = ", displaycounter);

            // re-set the time of last debounce
            SwitchTimeOfLastDebounce = millis();
        }
    }

    return switchpress;

}


// return the direction the rotary encoder was last turned
bool RotaryEncoder::get_direction(void) {
    return clockwise;
}


// reset rotary encoder counter to initial value
int RotaryEncoder::set_init(void) {
    displaycounter = Init;

    return displaycounter;
}


// set rotary encoder initial value user specified value
int RotaryEncoder::set_init(int init) {
    Init = init;
    displaycounter = Init;

    return displaycounter;
}

