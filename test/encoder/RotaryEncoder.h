
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    This object calls implements two data circular queue, aka ring buffer, for holding the messages that will appear on the scrolling display.
    is a linear data structure in which the operations are performed based on FIFO (First In First Out) principle and the last position is connected back to the first position to make a circle. It is also called "Ring Buffer".

REFERENCE MATERIALS:
    * https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
    * https://stackoverflow.com/questions/1403150/how-do-you-dynamically-allocate-a-matrix

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

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
    int DelayOfDebounce;              // to debounce, amount of time to ignore changes

    // storage for previous pins states
    int PreviousDATA;                 // previous DATA signal state
    int PreviousCLK;                  // previous CLOCK signal state

    // current state of the rotary encoder
    bool clockwise;                   // true if rotary turned clockwise
    bool switchpress;                 // if true, switch is being pressed
    int displaycounter;               // store for current counter value
    //int switchpress;                  // if LOW, switch is being pressed

  public:
    // constructors & destructors for the class
    RotaryEncoder(int, int, int, int);
    ~RotaryEncoder(void);

    // public methods for MessageStore
    int check_rotary(void);
    bool check_switch(void);
};

