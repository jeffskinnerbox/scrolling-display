
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.4.0

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#define TDEBUG true    // activate debugging routings (print trace messages on serial port)

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)
#include <Arduino.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "DeBug.h"
#include "WiFiTools.h"

#define BUF 25

//--------------------------- Global Scoped Variables --------------------------

extern DeBug DB;        // declare object DB as external, it is instantiated in DeBug.cpp


// ------------------------ Constructors & Destructors -------------------------

// Constructor to create WiFiTools
WiFiTools::WiFiTools(void) {

    ssid = new char[BUF];
    password = new char[BUF];
    timeout = 10000UL;              // time out for wifi access request

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

    // set wifi mode: WIFI_AP, WIFI_STA, WIFI_AP_STA, WIFI_OFF
    if (WiFi.mode(WIFI_STA)) {
        DEBUGTRACE(INFO, "WiFi set to Station (STA) Mode");
    } else {
        DEBUGTRACE(ERROR, "Could not set WiFi to Station (STA) Mode!");
    }

    // attempt first connect to a WiFi network
    DEBUGTRACE(INFO, "Attempting connection to WiFi with SSID ", ssid);
    WiFi.begin(ssid, password);

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        DEBUGPRINT(".");
        if (millis() > tout) {
            DEBUGPRINT(".\n\r");
            DEBUGTRACE(ERROR, "Failed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            return false;
        }
        delay(500);
    }

    DEBUGPRINT(".\n\r");
    DEBUGTRACE(INFO, "Successfully connected to WiFi!  IP address is ", WiFi.localIP());

    return true;

}


// terminate the wifi connect
void WiFiTools::wifiTerminate() {

    DEBUGTRACE(INFO, "Disconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

}


/*
// start the mDNS responder service
bool WiFiTools::wifiMDNS(char *name) {

    if (MDNS.begin(name)) {              // Start the mDNS responder for 'name'.local
        DEBUGTRACE(INFO, "mDNS responder started for ", name);
        return true;
    } else {
        DEBUGTRACE(ERROR, "Error setting up mDNS responder for ", name);
        return false;
    }

}
*/


// wifi diagnostic information
void WiFiTools::wifiDiag() {

    DEBUGTRACE(INFO, "Printing WiFi diagnostic information:");
    WiFi.printDiag(Serial);

}


// scan for nearby networks
void WiFiTools::wifiScan() {

    DEBUGTRACE(INFO, "Starting Network Scan");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    DEBUGTRACE(INFO, "Total number of SSIDs found: ", numSsid);

    // print the name of each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        DEBUGTRACE(INFO, "   ", WiFi.SSID(thisNet));
    }

    DEBUGTRACE(INFO, "Network Scan Completed");

}

