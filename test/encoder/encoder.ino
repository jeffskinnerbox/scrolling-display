/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION
    Test program for getting a KY-040 Rotary Encoder working with a ESP8266 NodeMCU
    with a simple switch debouncing scheme.

PHYSICAL DESIGN
    Hardware
        KY-040 Rotary Encoder - https://www.amazon.com/gp/product/B07G71GWX8/
        HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E - https://www.amazon.com/gp/product/B010O1G1ES

    Wiring
        Connect KY-040 Rotary Encoder "+" pin with NodeMCU Vin (5V)
        Connect KY-040 Rotary Encoder GND pin with with NodeMCU GND
        Connect SKY-040 Rotary Encoder SW pin with NodeMCU D5 pin
        Connect DKY-040 Rotary Encoder DT pin with NodeMCU D6 pin
        Connect CKY-040 Rotary Encoder CLK pin with with NodeMCU D7 pin

MONITOR
    To monitor the clocks activities while operating, power the NodeMCU via
    USB connection to a Linux computer and execut the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter:

        CNTR-a :quit

SOURCES
    https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/

DOCUMENTATION
    https://www.youtube.com/watch?v=cYCTMdUi8P0
    https://www.brainy-bits.com/arduino-rotary-encoder-ky-040/
    https://blog.squix.org/2016/05/esp8266-peripherals-ky-040-rotary-encoder.html

CREATED BY
    Brainy-Bits
------------------------------------------------------------------------------ */


// rotary encoder connections to nodemcu
const int PinSW =  D5;    // rotary encoder switch (rotary encoder SW)
const int PinDT =  D6;    // DATA signal (rotary encoder DT)
const int PinCLK = D7;    // CLOCK signal (rotary encoder CLK)

// variable to debounce rotary encoder
long TimeOfLastDebounce = 0;
int DelayOfDebounce = 0.01;

// store previous pins states
int PreviousCLK;
int PreviousDATA;

int displaycounter;      // store current counter value
const int Init = 0;      // initialization number displaycounter

int switchpress;         // if LOW, switch is being pressed



//------------------------------------------------------------------------------



// Check if the rotary encoder has moved and update the counter as required.
// Increased counter if moving in clockwise direction and decrease the counter
// if moving in the counter clockwise direction.
void check_rotary() {
    if ((PreviousCLK == LOW) && (PreviousDATA == HIGH)) {
        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
            displaycounter++;
            Serial.println(displaycounter);
        }

        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
            displaycounter--;
            Serial.println(displaycounter);
        }
    }

    if ((PreviousCLK == HIGH) && (PreviousDATA == LOW)) {
        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
            displaycounter++;
            Serial.println(displaycounter);
        }

        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
            displaycounter--;
            Serial.println(displaycounter);
        }
    }

    if ((PreviousCLK == HIGH) && (PreviousDATA == HIGH)) {
        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == HIGH)) {
            displaycounter++;
            Serial.println(displaycounter);
        }

        if ((digitalRead(PinCLK) == LOW) && (digitalRead(PinDT) == LOW)) {
            displaycounter--;
            Serial.println(displaycounter);
        }
    }

    if ((PreviousCLK == LOW) && (PreviousDATA == LOW)) {
        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == LOW)) {
            displaycounter++;
            Serial.println(displaycounter);
        }

        if ((digitalRead(PinCLK) == HIGH) && (digitalRead(PinDT) == HIGH)) {
            displaycounter--;
            Serial.println(displaycounter);
        }
    }
}



//------------------------------------------------------------------------------



void setup() {

    // setup serial port to print debug output
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect

    Serial.println("\n\rStarting KY-040 Rotary Encoder!");

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
    Serial.println(displaycounter);

}


void loop() {
    // if enough time has passed, check the rotary encoder
    if ((millis() - TimeOfLastDebounce) > DelayOfDebounce) {
        check_rotary();      // check rotary encoder for change of state

        // update pin state variables
        PreviousCLK = digitalRead(PinCLK);
        PreviousDATA = digitalRead(PinDT);

        // set the time of last debounce
        TimeOfLastDebounce = millis();
    }

    // check if rotary encoder switch is being pressed
    if ((digitalRead(PinSW) == LOW) && (switchpress == HIGH)) {
        displaycounter = Init;             // reset encoder counter to zero
        switchpress = LOW;                 // switch is being pressed
        Serial.print("button pressed: ");
        Serial.println(displaycounter);
    }
    if ((digitalRead(PinSW) == HIGH) && (switchpress == LOW)) {
        switchpress = HIGH;                 // switch is no longer pressed
        Serial.println(displaycounter);
    }
}
