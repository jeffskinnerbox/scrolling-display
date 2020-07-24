
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    Demonstration program showing the the use of the MD_Parola library, WiFi, and error handling.
    scrolling features.

PHYSICAL DESIGN:
    Hardware
        MAX7219 Dot Matrix Module 4-in-1 Display - https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
        HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E - https://www.amazon.com/gp/product/B010O1G1ES

    Wiring
        Connections for ESP8266 hardware SPI are:
        MAX72XX Pin    ESP8266 Pin    Notes / Comments
        Vcc              3v3           LED matrices seem to work at 3.3V
        GND              GND           GND
        DIN              D7            HSPID or HMOSI
        CS or LD         D8            HSPICS or HCS
        CLK              D5            CLK or HCLK

MONITOR:
    screen /dev/ttyUSB0 9600,cs8
    to terminate Cntr-a :quit

REFERENCE MATERIALS:
    * MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX

SOURCES:
    Code adapted from https://github.com/MajicDesigns/MD_Parola/tree/master/examples/Parola_Scrolling_ESP8266

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <SPI.h>
#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "debug.h"
#include "credentials.h"
#include "MessageStore.h"
#include "WiFiTools.h"


#define ONE_SECOND    1000UL        // milliseconds in one second
#define TWO_SECOND    2000UL        // milliseconds in two second
#define THREE_SECOND  3000UL        // milliseconds in three second
#define ONE_MINUTE    60000UL       // milliseconds in one minute
#define ONE_HOUR      3600000UL     // milliseconds in one hour
#define ONE_DAY       85400000UL    // milliseconds in one day

// display speeds, intensity, etc.
#define MAX_DEVICES   24    // number of dot matrix modules
#define PAUSE_TIME    TWO_SECOND
#define SCROLL_SPEED  50    // lower the number the faster the animation; 0 to run as fast as possible
#define FRAMEDELAY   200    // frame delay value min=10 (fast)  max=200 (slow)
#define INTENSITY      0    // set intensity of the display (0-15)
#define SPACING        0    // distance between the end of one message and the start of the next (0 = off display)

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers are for ESO8266 hardware SPI
//#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW    // values are: PAROLA, GENERIC, ICSTATION, FC16
#define CLK_PIN   D5    // or SCK
#define DATA_PIN  D7    // or MOSI
#define CS_PIN    D8    // or SS

// display text effects
const textPosition_t scrollAlign = PA_CENTER;        // how to align the text (e.g. PA_LEFT)
const textEffect_t scrollEffectIn = PA_SCROLL_UP;    // direction of scrolling (e.g. PA_SCROLL_LEFT)
const textEffect_t scrollEffectOut = PA_DISSOLVE;    // special effects when scrolling (e.g. PA_RANDOM)
const uint8_t intensity = INTENSITY;                 // intensity of matrix LEDs
const uint8_t scrollSpeed = SCROLL_SPEED;            // frame delay value
const uint16_t scrollPause = PAUSE_TIME;             // ms of pause after finished displaying message



//MessageStore Msg = MessageStore(5, 5, 80);
MessageStore Msg = MessageStore();

// Parola object constructor for SPI hardware interface
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Parola object constructor for arbitrary hardware interface
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// WiFiTools constructor
WiFiTools WT = WiFiTools();


//--------------------- Error Message Handler for Display ----------------------

// handle errors by displaying a code and then restart
void errorHandler(int error) {

    int i = 0;
    unsigned long tout;                           // time-out time
    int cycle = 0;
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    switch(error) {
        case 1:
            FATAL("Can't go on without WiFi connection.  Press reset twice to fix.\n\r");
            Msg.clear();
            Msg.addQueue("Can't go on without WiFi connection.");
            Msg.addQueue("Press reset twice to fix.");

            tout = ONE_MINUTE + millis();         // milliseconds of time to display message
            while (millis() < tout) {
                if (P.displayAnimate()) {
                    if (Msg.getQueue(top + cycle)[0] != '\0')
                        P.displayText(Msg.getQueue(top + cycle), scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
                    cycle = (cycle + 1) % size; // prepare index into msg[] for next pass
                }
                yield();                          // prevent the watchdog timer doing a reboot
            }

            // nothing can be done so restart
            FATAL("Nothing can be done.  Doing an automatic restart.\n\r");
            ESP.reset();
            break;
        default:
            // nothing can be done so restart
            ERRORD("Unknown error code in errorHandler: ", error);
            FATAL("Nothing can be done.  Doing an automatic restart.\n\r");
            ESP.reset();
    }
}



//------------------------------ Helper Routines -------------------------------

void loadmsg(void) {

    char string[BUF_SIZE];

    // clear all old messages
    INFO("Populating message queue with messages...\n\r");
    Msg.clearQueue();
    Msg.printQueue();

    // 1st message in simple store is the wifi IP address
    sprintf(string, "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    Msg.addStore(string);
    Msg.printStore();

    // 1st message is gibberish
    Msg.addQueue("#1 message in circular queue!!");
    Msg.printQueue();

    // 2nd message is gibberish
    Msg.addQueue("#2 The rain falls mainly on the plane in Spain");
    Msg.printQueue();

    // 3rd message is gibberish
    Msg.addQueue("#3 What is the weather outside right now?  What about inside?");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 4th message is gibberish
    Msg.addQueue("#4 short message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 5th message is gibberish
    Msg.addQueue("#5 loooooooooooong message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 6th message is gibberish
    Msg.addQueue("#6 Push off the oldest message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 7th message is gibberish
    Msg.addQueue("#7 This message should push off the next oldest message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    INFO("Exiting setup()...\n\r");
    PRINT("\n-------------------------------------------------------\n\r");

}



//------------------------------- Main Routines --------------------------------

void setup() {
    char string[BUF_SIZE];
    unsigned long tout;                           // time-out time
    int cycle = 0;                                // message number being displayed
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    Serial.begin(9600);
    PRINT("\n-------------------------------------------------------\n\r");
    INFO("Entered setup()...\n\r");
    INFO("Initializing scrolling display...\n\r");

    // initialize the display
    P.begin();                                           // initialize the display and data object
    P.setIntensity(intensity);                           // set intensity of the display
    P.setTextAlignment(scrollAlign);                     // set the text alignment
    P.setTextEffect(scrollEffectIn, scrollEffectOut);    // special effects when scrolling
    P.setScrollSpacing(SPACING);                         // columns between messages
    P.setSpeed(scrollSpeed);                             // frame delay value
    P.setPause(scrollPause);                             // ms of pause after finished displaying message
    P.displayClear();                                    // clear the display

    tout = ONE_SECOND + millis();                        // milliseconds of time to display message
    while (millis() < tout) {
        if (P.displayAnimate())
            P.displayText("Stand-By!!", scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
        yield();                                         // prevent the watchdog timer doing a reboot
    }

    // scan for wifi access points
    INFO("Initializing WiFi...\n\r");
    WT.wifiScan();

    // initialize all your display messages to null
    Msg.clear();

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        sprintf(string, "WiFi connected successfully to SSID %s.", WIFISSID);
        Msg.addQueue(string);
        //tout = THREE_SECOND + millis();              // milliseconds of time to display message
        tout = ONE_SECOND + millis();                  // milliseconds of time to display message
        while (millis() < tout) {
            if (P.displayAnimate()) {
                if (Msg.getQueue(top + cycle)[0] != '\0')
                    P.displayText(Msg.getQueue(top + cycle), scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
                cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass
            }
            yield();                                   // prevent the watchdog timer doing a reboot
        }
    } else
        errorHandler(1);

    loadmsg();
}


void loop() {

    static int cycle = 0;                      // message number being displayed
    static int top = Msg.top();
    static int size = Msg.size();

    if (P.displayAnimate()) {
        if (Msg.get(top + cycle)[0] != '\0')
            P.displayText(Msg.get(top + cycle), scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
        cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass
    }

}
