
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

#pragma once                          // compiler to skip subsequent includes of this file

class RotaryEncoder {
  private:
    // microcontroller pins used by the rotary encoder
    int PinSW;                        // rotary encoder switch (rotary encoder SW)
    int PinDT;                        // DATA signal (rotary encoder DT)
    int PinCLK;                       // CLOCK signal (rotary encoder CLK)
    int Init;                         // initialization number for displaycounter when button pressed

    // variable used to debounce rotary encoder and button press
    long RotaryTimeOfLastDebounce;    // variable used to debounce rotary encoder
    long SwitchTimeOfLastDebounce;    // variable used to debounce button
    long DelayOfDebounce;             // to debounce, amount of time to ignore changes

    // storage for previous pins states
    int PreviousDATA;                 // previous DATA signal state
    int PreviousCLK;                  // previous CLOCK signal state

    // current state of the rotary encoder
    bool clockwise;                   // true if rotary turned clockwise
    bool switchpress;                 // if true, switch is being pressed
    int displaycounter;               // store for current counter value

  public:
    // constructors & destructors for the class
    RotaryEncoder(int, int, int, int);
    ~RotaryEncoder(void);

    // public methods for MessageStore
    int check_rotary(void);
    bool check_switch(void);
    bool get_direction(void);
    int set_init(void);
    int set_init(int);
};

