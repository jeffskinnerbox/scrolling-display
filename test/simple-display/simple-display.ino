
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0

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

// ESP8266 libraries
#include <SPI.h>
#include <ESP8266WiFi.h>

// Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// this project's include files
#include "debug.h"
#include "credentials.h"


#define ONE_SECOND    1000UL        // milliseconds in one second
#define TWO_SECOND    2000UL        // milliseconds in two second
#define THREE_SECOND  3000UL        // milliseconds in three second
#define ONE_MINUTE    60000UL       // milliseconds in one minute
#define ONE_HOUR      3600000UL     // milliseconds in one hour
#define ONE_DAY       85400000UL    // milliseconds in one day
#define WIFITIME      10000         // attempt to connect with wifi for 10 seconds then abandon

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

// Scrolling parameters
//uint8_t frameDelay = FRAMEDELAY;
//textEffect_t scrollEffect = PA_SCROLL_LEFT;

// Parola object constructor for SPI hardware interface
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Parola object constructor for arbitrary hardware interface
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// display text effects
const textPosition_t scrollAlign = PA_CENTER;        // how to align the text (e.g. PA_LEFT)
const textEffect_t scrollEffectIn = PA_SCROLL_UP;    // direction of scrolling (e.g. PA_SCROLL_LEFT)
const textEffect_t scrollEffectOut = PA_DISSOLVE;    // special effects when scrolling (e.g. PA_RANDOM)
const uint8_t intensity = INTENSITY;                 // intensity of matrix LEDs
const uint8_t scrollSpeed = SCROLL_SPEED;            // frame delay value
const uint16_t scrollPause = PAUSE_TIME;             // ms of pause after finished displaying message


// credentials for wifi network
const char *ssid = WIFISSID;
const char *password = WIFIPASS;


// Global message buffers shared by Wifi and Scrolling functions
#define QUEUE_SIZE  5         // number of messages stored in a queue
#define BUF_SIZE    512       // max number of characters in a message

char msg[QUEUE_SIZE][BUF_SIZE];



//------------------------------ WiFi Subroutines ------------------------------

// connect to wifi
bool wifiConnect(const char *ssid, const char *password, unsigned long timeout) {
    unsigned long tout;

    // attempt first connect to a WiFi network
    PRINTS("\nAttempting connection to WiFi SSID ", ssid);
    WiFi.begin(ssid, password);

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        PRINT(".");
        if (millis() > tout) {
            PRINTD("\n\rFailed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            return false;
        }
        delay(500);
    }

    PRINTS("\n\rSuccessfully connected to WiFi!  IP address is ", WiFi.localIP());
    PRINTD("WiFi status exit code is ", WiFi.status());

    return true;
}


// terminate the wifi connect
void wifiTerminate() {
    PRINTS("\nDisconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

    PRINT("\n-------------------------------------------------------\n\r");
}


// scan for nearby networks
void wifiScan() {
    PRINT("\nStarting Network Scan\n\r");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    PRINTD("Total number of SSID found: ", numSsid);

    // print the name of each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        PRINTS("   ", WiFi.SSID(thisNet));
    }

    PRINT("Network Scan Completed\n\r");
    PRINT("\n-------------------------------------------------------\n\r");
}



//------------------------ Display Messages Management -------------------------

// initialize all your display messages to null
void clearMsg() {
    for (int i = 0; i < QUEUE_SIZE; i++)
        msg[i][0] = '\0';
}


// return index of next empty message in queue, return -1 if queue is full
int indexMsg() {

    for (int i = 0; i < QUEUE_SIZE; i++)
        if (msg[i][0] == '\0') {
            return i;
        }

    return -1;
}


// return the number of messages in queue
int countMsg() {
    int i = 0;

    for (i = 0; i < QUEUE_SIZE; i++)
        if (msg[i][0] == '\0') {
            return i;
        }

    return QUEUE_SIZE;
}


// put a message on the bottom of the msg queue
bool putMsg(char *str) {
    int index;

    index = indexMsg();

    if (index < 0) {
        PRINT("\nFailed to add message to queue. Queue is full.\n\r");
        return false;
    } else {
        PRINT("\nSuccessfully adding message to queue.\n\r");
        sprintf(msg[index], str);
        return true;
    }
}


// print all the messages
void printMsg() {
    int index;

    index = countMsg();

    if (index == 0) {
        PRINT("\nMessage queue is empty.\n\r");
    } else {
        PRINTD("Messages in msg queue: ", countMsg());
        for (int i = 0; i < index; i++)
            PRINTS("\t", msg[i]);
    }
}



//--------------------- Error Message Handler for Display ----------------------

// handle errors by displaying a code and then restart
void errorHandler(int error) {
    int i = 0;
    unsigned long tout;                           // time-out time
    uint8_t cycle = 0;                            // message number being displayed

    switch(error) {
        case 1:
            PRINT("\n\nCan't go on without WiFi connection.  Press reset twice to fix.\n\r");
            clearMsg();
            putMsg("Can't go on without WiFi connection.");
            putMsg("Press reset twice to fix.");

            tout = ONE_MINUTE + millis();         // milliseconds of time to display message
            while (millis() < tout) {
                if (P.displayAnimate()) {
                    if (msg[cycle][0] != '\0')
                        P.displayText(msg[cycle], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);

                    cycle = (cycle + 1) % ARRAY_SIZE(msg); // prepare index into msg[] for next pass
                }
                yield();                          // prevent the watchdog timer doing a reboot
            }

            PRINT("\nNothing can be done.  Doing an automatic restart\n\r");
            ESP.reset();                          // nothing can be done so restart
            break;
        default:                                  // nothing can be done so restart
            PRINTD("Unknown error code in errorHandler: ", error);
            PRINT("\nNothing can be done.  Doing an automatic restart\n\r");
            ESP.reset();
    }
}



//------------------------------- Main Routines --------------------------------

void setup() {
    char string[BUF_SIZE];
    unsigned long tout;                           // time-out time
    uint8_t cycle = 0;                            // message number being displayed

    Serial.begin(9600);
    PRINT("\n\n\rInitializing scrolling display...\n\r");

    // initialize all your display messages to null
    clearMsg();

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
    PRINT("\n\n\rInitializing WiFi...\n\r");
    wifiScan();

    // attempt to connect and initialise WiFi network
    if (wifiConnect(ssid, password, WIFITIME)) {         // connect to wifi
        clearMsg();
        sprintf(string, "WiFi connected successfully to SSID %s.", ssid);
        putMsg(string);
        //tout = THREE_SECOND + millis();                  // milliseconds of time to display message
        tout = ONE_SECOND + millis();                  // milliseconds of time to display message
        while (millis() < tout) {
            if (P.displayAnimate()) {
                if (msg[cycle][0] != '\0')
                    P.displayText(msg[cycle], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
                 cycle = (cycle + 1) % ARRAY_SIZE(msg);  // prepare index into msg[] for next pass
            }
            yield();                                     // prevent the watchdog timer doing a reboot
        }
    } else
        errorHandler(1);

    // clear all old messages
    PRINT("\n\n\rPopulating message queue with messages...\n\r");
    clearMsg();
    printMsg();

    // 1st message is the wifi IP address
    sprintf(string, "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    putMsg(string);
    printMsg();

    // 2nd message is gibberish
    putMsg("The rain falls mainly on the plane in Spain");
    printMsg();

    // 3rd message is gibberish
    putMsg("What is the weather outside right now?  What about inside?");
    printMsg();

    // 4th message is gibberish
    putMsg("short message");
    printMsg();

    // 5th message is gibberish
    putMsg("loooooooooooong message");
    printMsg();

    // 6th message is gibberish
    putMsg("this message should fail to load");
    printMsg();
}


void loop() {

    static uint8_t cycle = 0;                            // message number being displayed

    if (P.displayAnimate()) {
        if (msg[cycle][0] != '\0')
            P.displayText(msg[cycle], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
        cycle = (cycle + 1) % ARRAY_SIZE(msg);    // prepare index into msg[] for next pass
    }

}
