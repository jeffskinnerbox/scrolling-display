
// ESP8266 libraries
#include <SPI.h>
#include "ESP8266WiFi.h"

// Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// set to 1 to turn on debug statements to the serial output
// using F() to load strings in flash memory, not RAM
#define DEBUG true
#if DEBUG
    #define PRINT(s, x) { Serial.print(F(s)); Serial.println(x); }
    #define PRINTS(x) Serial.println(F(x))
    #define PRINTX(x) Serial.println(x, HEX)
#else
    #define PRINT(s, x)
    #define PRINTS(x)
    #define PRINTX(x)
#endif

#define ONE_SECOND        1000UL        // milliseconds in one second
#define TWO_SECOND        2000UL        // milliseconds in two second
#define ONE_MINUTE        60000UL       // milliseconds in one minute
#define ONE_HOUR          3600000UL     // milliseconds in one hour
#define ONE_DAY           85400000UL    // milliseconds in one day
#define WIFITIME          10000         // attempt to connect with wifi for 10 seconds then abandon

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers are for ESO8266 hardware SPI
//#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW    // values are: PAROLA, GENERIC, ICSTATION, FC16
//#define MAX_DEVICES 24  // number of dot matrix modules
#define MAX_DEVICES 12  // number of dot matrix modules
#define FRAMEDELAY 50   // default frame delay value min=10 (fast)  max=200 (slow)
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



//------------------------------- WiFi Parameters ------------------------------

// credentials for wifi network
//const char *ssid = "<my-wifi-ssid>";
//const char *pass = "<my-wifi-password>";
const char *ssid = "74LL5";
const char *password = "1CyberPhysicalSystems2";


// Global message buffers shared by Wifi and Scrolling functions
#define QUEUE_SIZE  5         // number of messages stored in a queue
#define BUF_SIZE    512       // max number of characters in a message

int MessageInQueue = 0;
char curMessage[QUEUE_SIZE][BUF_SIZE];
char newMessage[BUF_SIZE];



//------------------------------------------------------------------------------


// handle errors by displaying a code and then restart
void errorHandler(int error) {
    textPosition_t scrollAlign = PA_LEFT;
    uint8_t scrollSpeed = 25;                      // default frame delay value
    uint16_t scrollPause = TWO_SECOND;             // in milliseconds
    textEffect_t scrollEffectIn = PA_SCROLL_LEFT;
    textEffect_t scrollEffectOut = PA_NO_EFFECT;
    int i = 0;
    unsigned long tout;

    sprintf(curMessage[0], "Can't go on without WiFi connection.");
    sprintf(curMessage[1], "Press reset twice to fix.");
    MessageInQueue = 2;

    tout = ONE_MINUTE + millis();                // milliseconds of time given to making connection attempt
    while (millis() < tout) {
        if (P.displayAnimate()) {
            Serial.println("Inside errorHandler");
            P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
            i++;
            if (i == MessageInQueue) i = 0;
            P.displayReset();        // reset the animation back to the beginning
        }
        yield();    // prevent the watchdog timer doing a reboot
    }

    Serial.flush();

    ESP.reset();                     // nothing can be done so restart
}


// connect to wifi
bool wifiConnect(const char *ssid, const char *password, unsigned long timeout) {
    unsigned long tout;

    // attempt first connect to a WiFi network
    Serial.print("\nAttempting connection to WiFi SSID ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        if (millis() > tout) {
            Serial.print("Failed to connect to WiFi!  ");
            Serial.print("WiFi status exit code is ");
            Serial.println(WiFi.status());
            return false;
        }
        delay(500);
    }

    Serial.print("Successfully connected to WiFi!  ");
    Serial.print("IP address is ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi status exit code is ");
    Serial.println(WiFi.status());

    return true;
}


// scan for nearby networks
void scanNetworks() {
    Serial.println("\nStarting Network Scan");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    Serial.print("SSID List:");
    Serial.println(numSsid);

    // print the network number and name for each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        Serial.print("   ");
        Serial.print(thisNet);
        Serial.print(") Network: ");
        Serial.println(WiFi.SSID(thisNet));
    }

    Serial.println("Network Scan Completed");
    Serial.println("\n-------------------------------------------------------");
}


void setup() {
    textPosition_t scrollAlign = PA_LEFT;
    uint8_t scrollSpeed = 25;                      // default frame delay value
    uint16_t scrollPause = TWO_SECOND;             // in milliseconds
    textEffect_t scrollEffectIn = PA_SCROLL_LEFT;
    textEffect_t scrollEffectOut = PA_NO_EFFECT;

    Serial.begin(9600);
    PRINTS("\nInitializing scrolling display.");

    // initialize all your display messages to null
    newMessage[0] = '\0';
    MessageInQueue = 0;
    for (int j = 0; j < QUEUE_SIZE; j++)
            curMessage[0][j] = '\0';

    P.begin();                     // initialize the object data
    P.displayClear();              // clear all the zones in the current display
/*
    P.displaySuspend(false);       // start the current display animation, true = suspend, false = resume
    P.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);
*/

    // scan for wifi access point and print what you find (useful for trouble shouting wifi)
    scanNetworks();                           // scan for wifi access points

    // Connect to and initialise WiFi network
    PRINT("\nConnecting to ", ssid);

    //WiFi.begin(ssid, password);
    if (wifiConnect(ssid, password, WIFITIME)) {  // connect to wifi
    } else {
        Serial.println("Can't go on without WiFi connection.  Press reset twice to fix.");
        errorHandler(1);
    }

    // set up first message as the IP address
    MessageInQueue++;
    sprintf(curMessage[MessageInQueue - 1], "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    PRINT("\ncurMessage[%0] = ", curMessage[MessageInQueue - 1]);

    // 2nd message
    MessageInQueue++;
    sprintf(curMessage[MessageInQueue - 1], "the rain falls mainly on the plane in spain");
    PRINT("\ncurMessage[1] = ", curMessage[MessageInQueue - 1]);

    P.displayText(curMessage[MessageInQueue - 1], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
}


void loop() {
    textPosition_t scrollAlign = PA_LEFT;
    uint8_t scrollSpeed = 25;                      // default frame delay value
    uint16_t scrollPause = TWO_SECOND;             // in milliseconds
    textEffect_t scrollEffectIn = PA_SCROLL_LEFT;
    textEffect_t scrollEffectOut = PA_NO_EFFECT;
    int i = 0;

    Serial.println("Outside the loop");
    if (P.displayAnimate()) {
        Serial.println("Inside the loop");
        P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
        i++;
        if (i == MessageInQueue) i = 0;
        P.displayReset();        // reset the animation back to the beginning
    }
}
