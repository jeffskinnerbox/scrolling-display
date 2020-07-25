
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/


// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)
#include <Arduino.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "debug.h"
#include "WiFiTools.h"

#define BUF 25

// ------------------------ Constructors & Destructors -------------------------

// Constructor to create WiFiTools
WiFiTools::WiFiTools(void) {

    ssid = new char[BUF];
    password = new char[BUF];
    timeout = 10000;              // time out for wifi access request

}


// Destructor to delete WiFiTools
WiFiTools::~WiFiTools(void) {

    delete [] ssid;
    delete [] password;

}



//-------------------------------- WiFi Methods --------------------------------

// connect to wifi
bool WiFiTools::wifiConnect(char *id, char *pass, unsigned long tout) {

    ssid = id;
    password = pass;
    timeout = tout;

    // attempt first connect to a WiFi network
    INFOS("Attempting connection to WiFi SSID ", ssid);
    WiFi.begin(ssid, password);

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        PRINT(".");
        if (millis() > tout) {
            PRINT("\n\r");
            ERRORD("Failed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            return false;
        }
        delay(500);
    }
    PRINT("\n\r");

    INFOS("Successfully connected to WiFi!  IP address is ", WiFi.localIP());
    INFOD("WiFi status exit code is ", WiFi.status());

    return true;
}


// terminate the wifi connect
void WiFiTools::wifiTerminate() {
    INFOS("\nDisconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

    PRINT("\n-------------------------------------------------------\n\r");
}


// scan for nearby networks
void WiFiTools::wifiScan() {
    INFO("Starting Network Scan\n\r");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    INFOD("Total number of SSIDs found: ", numSsid);

    // print the name of each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        INFOS("   ", WiFi.SSID(thisNet));
    }

    INFO("Network Scan Completed\n\r");
    PRINT("\n-------------------------------------------------------\n\r");
}

