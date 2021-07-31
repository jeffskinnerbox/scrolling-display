
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/


#define TDEBUG  true       // activate trace message printing for debugging

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)
#include <Arduino.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "DeBug.h"
#include "WiFiHandler.h"

#define BUF 25

extern DeBug DB;        // declare object DB as external, it is instantiated in DeBug.cpp

// ------------------------ Constructors & Destructors -------------------------

// Constructor to create WiFiHandler
WiFiHandler::WiFiHandler(void) {

    ssid = new char[BUF];
    password = new char[BUF];
    timeout = 10000;              // time out for wifi access request

}


// Destructor to delete WiFiHandler
WiFiHandler::~WiFiHandler(void) {

    delete [] ssid;
    delete [] password;

}



//-------------------------------- WiFi Methods --------------------------------

// connect to wifi
bool WiFiHandler::wifiConnect(char *id, char *pass, unsigned long tout) {

    ssid = id;
    password = pass;
    timeout = tout;

    // attempt first connect to a WiFi network
    DEBUGTRACE(INFO, "Attempting connection to WiFi SSID ", ssid);
    WiFi.begin(ssid, password);

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        DEBUGPRINT(".");
        if (millis() > tout) {
            DEBUGTRACE(ERROR, "Failed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            DEBUGTRACE(ERROR, "Timed out after (milliseconds): ", timeout);
            return false;
        }
        delay(500);
    }

    DEBUGPRINT(".\n\r");
    DEBUGTRACE(INFO, "Successfully connected to WiFi!  IP address is ", WiFi.localIP());
    DEBUGTRACE(INFO, "WiFi status exit code is ", WiFi.status());

    return true;
}


// terminate the wifi connect
void WiFiHandler::wifiTerminate() {
    DEBUGTRACE(INFO, "Disconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

    DEBUGTRACE(INFO, "--------------------------------------------------------------------------------");
}


// scan for nearby networks
void WiFiHandler::wifiScan() {
    DEBUGTRACE(INFO, "------- Starting Network Scan --------");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    DEBUGTRACE(INFO, "Total number of SSIDs found: ", numSsid);

    // print the name of each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        DEBUGTRACE(INFO, "   ", WiFi.SSID(thisNet));
    }

    DEBUGTRACE(INFO, "------- Network Scan Completed -------");

}



//--------------------------------- UDP Methods --------------------------------

void WiFiHandler::udpSetPort(unsigned int port) {
    UDPport = port;
}


// start listening for UDP messages on port UDPport
bool WiFiHandler::udpStart() {

    if (udp.begin(UDPport)) {
        DEBUGTRACE(INFO, "Starting UDP for NTP connection.  Using local port ", UDPport);
        return true;
    } else {
        DEBUGTRACE(ERROR, "Failed to start UDP listener.");
        return false;
    }

}


// stop listening for UDP messages on port UDPport
void WiFiHandler::udpStop() {

    DEBUGTRACE(INFO, "Stopping UDP on local port ", UDPport);
    udp.stop();

}


bool WiFiHandler::udpCheck(void) {
    unsigned int rtn;

    rtn = udp.parsePacket();

    if (!rtn)
        DEBUGTRACE(WARN, "No UDP packets are available");

    return rtn;

}


bool WiFiHandler::udpRequest(IPAddress& address, unsigned int port, byte *packetbuffer, unsigned int len) {
    unsigned int rtn1, rtn2, bytes_rtn;

    rtn1 = udp.beginPacket(address, port);      // request IP address and port
    bytes_rtn = udp.write(packetbuffer, len);   // buffer containing the request return
    rtn2 = udp.endPacket();

    if (rtn1 == 0 || rtn2 == 0 || bytes_rtn == 0) {
        DEBUGTRACE(WARN, "UDP request failed");
        DEBUGTRACE(WARN, "trtn1 = ", rtn1);
        DEBUGTRACE(WARN, "trtn2 = ", rtn2);
        DEBUGTRACE(WARN, "tbytes_rtn = ", bytes_rtn);
        return false;
    }

    return true;

}


int WiFiHandler::udpRead(byte *packetbuffer, unsigned int len) {
    int rtn;

    // This function can only be successfully called after WiFiUDP.parsePacket()
    rtn = udp.read(packetbuffer, len);      // read the packet into the buffer

    return rtn;

}

