
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

#define DEBUG true    // activate debugging routings (print trace messages on serial port)

// ESP8266 libraries (~/.arduino15/packages/esp8266)

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// encoder project's include files (~/src/scrolling-display/test/encoder)
#include "debug.h"
#include "RotaryEncoder.h"


#define INIT 0         // number for encoder when button is pressed
#define BUTTON false   // state for when button has been released

// RotaryEncoder object constructor for detecting rotary encoder state
//    D5 = encoder switch (rotary encoder SW)
//    D6 = signal (rotary encoder DT)
//    D7 = signal (rotary encoder CLK)
//    INIT = number for displaycounter when button is pressed
//RotaryEncoder E = RotaryEncoder(D5, D6, D7, INIT);
RotaryEncoder *E;

// version stamp
#define VERSION "0.3.0"
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

    // RotaryEncoder object constructor for detecting rotary encoder state
    //    D5 = encoder switch (rotary encoder SW)
    //    D6 = signal (rotary encoder DT)
    //    D7 = signal (rotary encoder CLK)
    //    INIT = number for displaycounter when button is pressed
    E = new RotaryEncoder(D5, D6, D7, INIT);

}


void loop() {
    static int count = INIT, prv_cnt = INIT;
    static bool state = BUTTON, prv_state = BUTTON;
    static bool direction = true, prv_direct = true;

    // check the status of the rotary encoder
    count = E->check_rotary();        // check rotary encoder for change of state
    state = E->check_switch();        // check button for change of state
    direction = E->get_direction();   // get the direction of last turn

    // if rotary encoder count changes, print debug message
    if (count != prv_cnt) {
        prv_cnt = count;              // update previous count
        INFOD("count = ", count);
    }

    // if rotary encoder button is pressed, re-initialize and print debug message
    if (state == true && state != prv_state) {
        INFOB("state = ", state);
        prv_state = state;            // update previous button state

        count = E->set_init();        // reset button status to initial value
        prv_cnt = count;              // update previous count
        INFOD("count = ", count);
    } else if (state == false && state != prv_state) {
        INFOB("state = ", state);
        prv_state = state;            // update previous button state
    }

    // if rotary encoder direction changes, print debug message
    if (direction != prv_direct) {
        INFOB("direction = ", direction);
        prv_direct = direction;       // update previous direction state
    }

}
