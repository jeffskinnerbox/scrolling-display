
//
// Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
// Version:      0.1.0
//
// simple scrolling display with wifi and error handling
//


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
const uint16_t scrollPause = TWO_SECOND;             // ms of pause after finished displaying message


// credentials for wifi network
const char *ssid = WIFISSID;
const char *password = WIFIPASS;


// Global message buffers shared by Wifi and Scrolling functions
#define QUEUE_SIZE  5         // number of messages stored in a queue
#define BUF_SIZE    512       // max number of characters in a message

int MessageInQueue = 0;
char curMessage[QUEUE_SIZE][BUF_SIZE];



//--------------------- Error Message Handler for Display ----------------------

// handle errors by displaying a code and then restart
void errorHandler(int error) {
    int i = 0;
    unsigned long tout;

    tout = ONE_MINUTE + millis();                // milliseconds of time given to making connection attempt
    while (millis() < tout) {
        if (P.displayAnimate()) {
            PRINT("Inside errorHandler\n\r");
            P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
            i++;
            if (i == MessageInQueue) i = 0;
            P.displayReset();        // reset the animation back to the beginning
        }
        yield();    // prevent the watchdog timer doing a reboot
    }

    PRINT("\nDoing an automatic restart\n\r");
    ESP.reset();                     // nothing can be done so restart
}



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


// scan for nearby networks
void scanNetworks() {
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



//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(9600);
    PRINT("\n\n\rInitializing scrolling display.\n\r");

    // initialize all your display messages to null
    MessageInQueue = 0;
    for (int j = 0; j < QUEUE_SIZE; j++)
            curMessage[0][j] = '\0';

    P.begin();                                           // initialize the display and data object
    P.setIntensity(intensity);                           // set intensity of the display
    P.setTextAlignment(scrollAlign);                     // set the text alignment
    P.setTextEffect(scrollEffectIn, scrollEffectOut);    // special effects when scrolling
    P.setScrollSpacing(SPACING);                         // columns between messages
    P.setSpeed(scrollSpeed);                             // frame delay value
    P.setPause(scrollPause);                             // ms of pause after finished displaying message
    P.displayClear();                                    // clear the display

    scanNetworks();               // scan for wifi access points (useful for trouble shouting wifi)

    // Connect to and initialise WiFi network
    if (wifiConnect(ssid, password, WIFITIME)) {         // connect to wifi
    } else {
        PRINT("\n\nCan't go on without WiFi connection.  Press reset twice to fix.\n\r");
        sprintf(curMessage[0], "Can't go on without WiFi connection.");
        sprintf(curMessage[1], "Press reset twice to fix.");
        MessageInQueue = 2;
        errorHandler(1);
    }

    // set up first message as the IP address
    MessageInQueue++;
    sprintf(curMessage[MessageInQueue - 1], "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    PRINTS("\ncurMessage[0] = ", curMessage[MessageInQueue - 1]);

    // 2nd message
    MessageInQueue++;
    sprintf(curMessage[MessageInQueue - 1], "the rain falls mainly on the plane in spain");
    PRINTS("curMessage[1] = ", curMessage[MessageInQueue - 1]);

    P.displayText(curMessage[MessageInQueue - 1], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
}


void loop() {

    int i = 0;

    //PRINT("Outside the loop\n\r");
    if (P.displayAnimate()) {
        //PRINT("Inside the loop\n\r");
        P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
        i++;
        if (i == MessageInQueue) i = 0;
        P.displayReset();        // reset the animation back to the beginning
    }
}
