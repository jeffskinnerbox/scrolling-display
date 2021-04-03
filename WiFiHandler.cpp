
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0

DESCRIPTION:
    mDNS Service
    mDNS, or multicast DNS, is a service that helps you find your WiFi/Ethernet devices
    on the network without knowing their IP address. It is a standard protocol widely
    implemented for local device discovery. mDNS won’t let you find devices any where
    on the Internet, but mDNS can help if your sensor and computer are on the same local network.

    https://www.megunolink.com/documentation/connecting/mdns-browser/

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

#define TDEBUG  false       // activate trace message printing for debugging

// found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino libraries (~/Arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// this project's include files
#include "DeBug.h"
#include "WiFiHandler.h"

#define BUF 25

// for trace messages/debugging, construct object DB as class DeBug
extern DeBug DB;

// ------------------------ Constructors & Destructors -------------------------

// Constructor to create WiFiTools
WiFiTools::WiFiTools(void) {

    ssid = new char[BUF];
    password = new char[BUF];
    hostname = new char[BUF];
    timeout = 10000;              // time out for wifi access request

}


// Constructor to create WiFiTools
WiFiTools::WiFiTools(char *name, char *id, char *pass, unsigned long tout) {

    ssid = new char[BUF];
    password = new char[BUF];
    hostname = new char[BUF];
    timeout = 10000;              // time out for wifi access request

    wifiConnect(name, id, pass, tout);

}


// Destructor to delete WiFiTools
WiFiTools::~WiFiTools(void) {

    delete [] ssid;
    delete [] password;
    delete [] hostname;

}



//-------------------------------- WiFi Methods --------------------------------

// connect to wifi
bool WiFiTools::wifiConnect(char *name, char *id, char *pass, unsigned long tout) {

    ssid = id;
    password = pass;
    hostname = name;
    timeout = tout;
    bool rtn = true;

    // set the wifi hostname
    hostname = name;
    WiFi.hostname(hostname);
    DEBUGTRACE(INFO, "hostname = ", hostname);

    // attempt first connect to a WiFi network
    DEBUGTRACE(INFO, "Attempting connection to WiFi SSID ", ssid);
    WiFi.begin(ssid, password);

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        DEBUGPRINT(".");
        if (millis() > tout) {
            DEBUGTRACE(ERROR, "Failed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            return false;
        }
        delay(500);
    }
    DEBUGPRINT(".\n\r");

    // print ip address and status code
    DEBUGTRACE(INFO, "Successfully connected to WiFi!  IP address is ", WiFi.localIP());
    DEBUGTRACE(INFO, "WiFi status exit code is ", WiFi.status());  // code 3 = connected

/*    // if wifi connected to access point, then start mDNS*/
    //if (WiFi.status() == WL_CONNECTED) {
        //if (MDNS.begin(hostname)) {
            //DEBUGTRACE(INFO, "mDNS responder started!");
            //return true;
        //} else {
            //DEBUGTRACE(ERROR, "Error setting up mDNS responder!");
            //return false;
        //}
    //} else {
        //DEBUGTRACE(ERROR, "Cannot setup mDNS since not connected to WiFi!");
        //return false;
    /*}*/

    return rtn;
}


// terminate the wifi connect
void WiFiTools::wifiTerminate(void) {
    DEBUGTRACE(INFO, "Disconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

    DEBUGTRACE(INFO, "--------------------------------------------------------------------------------");
}


/*// set wifi hostname*/
//bool WiFiTools::wifiHostname(char *name) {

    //hostname = name;
    //DEBUGTRACE(INFO, "hostname = ", hostname);

    //if (WiFi.status() == WL_CONNECTED) {     // if wifi connected to access point, then start mDNS
        //if (MDNS.begin(hostname)) {
            //DEBUGTRACE(INFO, "mDNS responder started!");
            //return true;
        //} else {
            //DEBUGTRACE(ERROR, "Error setting up mDNS responder!");
            //return false;
        //}
    //} else {
        //DEBUGTRACE(ERROR, "Cannot setup mDNS since not connected to WiFi!");
        //return false;
    //}

/*}*/


// scan for nearby networks
void WiFiTools::wifiScan(void) {
    DEBUGTRACE(INFO, "Starting Network Scan");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen
    DEBUGTRACE(INFO, "Total number of SSIDs found: ", numSsid);

    // print the name of each network found
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        DEBUGTRACE(INFO, "   ", WiFi.SSID(thisNet));
    }

    DEBUGTRACE(INFO, "Network Scan Completed");
    DEBUGTRACE(INFO, "--------------------------------------------------------------------------------");
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
