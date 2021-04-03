
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:

PHYSICAL DESIGN:
    Hardware

    Wiring
        None required

MONITOR:
    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Cntr-a :quit

TESTING:
    To get the ip & mac addresses
        sudo arp-scan 192.168.1.0/24 | grep Espressif

    To get the hostname
        nslookup 192.168.1.44

USAGE:

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

// found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// found in Arduino libraries (~/Arduino/libraries)

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// this project's include files
#include "secrets.h"


#define PORT 23                 // telnet port is 23
#define MAX_SRV_CLIENTS 3       // maximum number of telnet clients that can connect to this device


WiFiServer server(PORT);                   // creates a server that listens for incoming connections on the specified port
WiFiClient serverClients[MAX_SRV_CLIENTS]; // creates an array of client that can connect to internet ip address



//------------------------------ Helper Routines -------------------------------

void stringToUpper(String &s) {
    for (unsigned int i = 0; i < s.length(); i++)
        s[i] = toupper(s[i]);
}


String MakeHostname(const char *Template) {

    // get esp8266 chip id and create chip id string
    String Id = String(ESP.getChipId(), HEX);
    stringToUpper(Id);

    // create the hostname
    String Result = String(Template) + "-" + Id;

    return Result;

}


void printInfo(void) {

    Serial.print(F(" Connected.\r\n\nESP Board IP address is: "));
    Serial.println(WiFi.localIP());

    Serial.print("ESP Board MAC Address is: ");
    Serial.println(WiFi.macAddress());

    Serial.print("ESP Chip ID is: ");
    Serial.println(ESP.getChipId(), HEX);

    Serial.print("ESP Flash Chip ID is: ");
    Serial.println(ESP.getFlashChipId(), HEX);

}



//------------------------------- WiFi Routines --------------------------------

void ConnectToWiFi(void) {

    uint8_t i = 0;

    //WiFi.hostname("ESP-test");         // set the hostname of the wifi device
    WiFi.mode(WIFI_STA);              // become a devices that connect to wifi access point, aka stations (STA)
    WiFi.begin(WIFISSID, WIFIPASS);   // connect with wifi access point

    Serial.print("Connecting to "); Serial.println(WIFISSID);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);

        if ((++i % 16) == 0) {
            Serial.println(F(" still trying to connect"));
        }
    }

}



//-------------------------------- WiFi Routines -------------------------------

void AdvertiseServices(void) {

    String MyName = MakeHostname(HOSTNAME);

    if (MDNS.begin(MyName.c_str())) {
        Serial.println(F("\r\nmDNS responder started"));
        Serial.print(F("Device hostname is: "));
        Serial.println(MyName.c_str());

        // Add service to MDNS-SD
        MDNS.addService("n8i-mlp", "tcp", PORT);
    } else {
        while(1) {
            Serial.println(F("Error setting up MDNS responder"));
            delay(1000);
        }
    }

}



//------------------------------- Main Routines --------------------------------

void setup(void) {

    Serial.begin(9600);
    Serial.println(F("\r\n\nmDNS Testing Routine"));
    Serial.println(F("-----------------------------"));

    AdvertiseServices();

    ConnectToWiFi();

    printInfo();

    // Start the TCP server
    server.begin();
    server.setNoDelay(true);

    Serial.println("\r\nmDNS responder is ready.");

}


void loop(void) {

    uint8_t i;

    // check if there are any new clients wanting to connect
    if (server.hasClient()) {
        for (i = 0; i < MAX_SRV_CLIENTS; i++) {
            // find free / disconnected spot
            if (!serverClients[i] || !serverClients[i].connected()) {
                if (serverClients[i]) serverClients[i].stop();
                serverClients[i] = server.available();
                Serial.print("New client: "); Serial.println(i);
                break;
            }
        }

        // no free / disconnected clients so reject
        if ( i == MAX_SRV_CLIENTS) {
            WiFiClient serverClient = server.available();
            serverClient.stop();
            Serial.print("Connection rejected.  Your reached maximum of ");
            Serial.println(i);
        }
    }

    // check clients for data, any data that is received is sent out the local serial port
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
        if (serverClients[i] && serverClients[i].connected()) {
            if (serverClients[i].available()) {
                // get data from the telnet client and push it to the serial port
                while (serverClients[i].available()) {
                    Serial.write(serverClients[i].read());
                }
            }
        }
    }

    // check serial port for data, any serial data received is sent to connected telnet clients
    if (Serial.available()) {
        size_t len = Serial.available();
        uint8_t sbuf[len];
        Serial.readBytes(sbuf, len);

        // push UART data to all connected telnet clients
        for (i = 0; i < MAX_SRV_CLIENTS; i++) {
            if (serverClients[i] && serverClients[i].connected()) {
                serverClients[i].write(sbuf, len);
                delay(1);
            }
        }
    }

}

