
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
#include "WiFiHandler.h"

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
        PRT(".");
        if (millis() > tout) {
            ERRORD("Failed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            return false;
        }
        delay(500);
    }

    PRT(".\n\r");
    INFOS("Successfully connected to WiFi!  IP address is ", WiFi.localIP());
    INFOD("WiFi status exit code is ", WiFi.status());

    return true;
}


// terminate the wifi connect
void WiFiTools::wifiTerminate() {
    INFOS("Disconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

    PRINT("--------------------------------------------------------------------------------");
}


// scan for nearby networks
void WiFiTools::wifiScan() {
    INFO("Starting Network Scan");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    INFOD("Total number of SSIDs found: ", numSsid);

    // print the name of each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        INFOS("   ", WiFi.SSID(thisNet));
    }

    INFO("Network Scan Completed");
    PRINT("--------------------------------------------------------------------------------");
}


/*
void handleWiFi(void)
{
  static enum { S_IDLE, S_WAIT_CONN, S_READ, S_EXTRACT, S_RESPONSE, S_DISCONN } state = S_IDLE;
  static char szBuf[1024];
  static uint16_t idxBuf = 0;
  static WiFiClient client;
  static uint32_t timeStart;

  switch (state)
  {
  case S_IDLE:   // initialise
    INFO("S_IDLE");
    idxBuf = 0;
    state = S_WAIT_CONN;
    break;

  case S_WAIT_CONN:   // waiting for connection
  {
    client = server.available();
    if (!client) break;
    if (!client.connected()) break;

#if DEBUG
    char szTxt[20];
    sprintf(szTxt, "%03d.%03d.%03d.%03d", client.remoteIP()[0], client.remoteIP()[1], client.remoteIP()[2], client.remoteIP()[3]);
    INFOS("New client @ ", szTxt);
#endif

    timeStart = millis();
    state = S_READ;
  }
  break;

  case S_READ: // get the first line of data
    INFO("S_READ ");

    while (client.available())
    {
      char c = client.read();

      if ((c == '\r') || (c == '\n'))
      {
        szBuf[idxBuf] = '\0';
        client.flush();
        INFOS("Recv: ", szBuf);
        state = S_EXTRACT;
      }
      else
        szBuf[idxBuf++] = (char)c;
    }
    if (millis() - timeStart > 1000)
    {
      INFO("Wait timeout");
      state = S_DISCONN;
    }
    break;

  case S_EXTRACT: // extract data
    INFO("S_EXTRACT");
    // Extract the string from the message if there is one
    getData(szBuf, BUF_SIZE);
    state = S_RESPONSE;
    break;

  case S_RESPONSE: // send the response to the client
    INFO("S_RESPONSE");
    // Return the response to the client (web page)
    client.print(WebResponse);
    client.print(WebPage);
    state = S_DISCONN;
    break;

  case S_DISCONN: // disconnect client
    INFO("S_DISCONN");
    client.flush();
    client.stop();
    state = S_IDLE;
    break;

  default:  state = S_IDLE;
  }
}
*/
