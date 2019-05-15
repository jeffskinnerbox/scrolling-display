/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.2

DESCRIPTION:
    Demonstration program showing the the use of the MD_Parola library display
    scrolling features.

    User can enter text through a web browser and this will display as a
    scrolling message on the display. Some parameters for the text can also
    be controlled from the web page.

    The IP address for the ESP8266 is displayed on the scrolling display
    after startup initialisation and connected to the WiFi network.

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
    screen /dev/ttyUSB0 57600,cs8
    to terminate Cntr-a :quit

TESTING:
    xxxxxxxxxx

USAGE:
    xxxxxxxxxx

REFERENCE MATERIALS:
    * MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX

SOURCES:
    Code adapted from https://github.com/MajicDesigns/MD_Parola/tree/master/examples/Parola_Scrolling_ESP8266

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/


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
#define MAX_DEVICES 24  // number of dot matrix modules
#define FRAMEDELAY 50   // default frame delay value min=10 (fast)  max=200 (slow)
#define CLK_PIN   D5    // or SCK
#define DATA_PIN  D7    // or MOSI
#define CS_PIN    D8    // or SS

// Scrolling parameters
uint8_t frameDelay = FRAMEDELAY;
textEffect_t scrollEffect = PA_SCROLL_LEFT;

// Parola object constructor for SPI hardware interface
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Parola object constructor for arbitrary hardware interface
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);



//------------------------------- WiFi Parameters ------------------------------

// credentials for wifi network
//const char *ssid = "<my-wifi-ssid>";
//const char *pass = "<my-wifi-password>";
const char *ssid = "74LL5";
//const char *password = "1CyberPhysicalSystems2";
const char *password = "???";


// WiFi Server object and parameters
WiFiServer server(80);

// Global message buffers shared by Wifi and Scrolling functions
#define QUEUE_SIZE  5         // number of messages stored in a queue
#define BUF_SIZE    512       // max number of characters in a message
bool newMessageAvailable = false;
int MessageInQueue = 0;
char curMessage[QUEUE_SIZE][BUF_SIZE];
char newMessage[BUF_SIZE];

const char WebResponse[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";

char WebPage[] =
"<!DOCTYPE html>" \
"<html>" \
"<head>" \
"<title>Scrolling Display Test Page</title>" \

"<script>" \
"strLine = \"\";" \

"function SendData()" \
"{" \
"  nocache = \"/&nocache=\" + Math.random() * 1000000;" \
"  var request = new XMLHttpRequest();" \
"  strLine = \"&MSG=\" + document.getElementById(\"data_form\").Message.value;" \
"  strLine = strLine + \"/&SD=\" + document.getElementById(\"data_form\").ScrollType.value;" \
"  strLine = strLine + \"/&I=\" + document.getElementById(\"data_form\").Invert.value;" \
"  strLine = strLine + \"/&SP=\" + document.getElementById(\"data_form\").Speed.value;" \
"  request.open(\"GET\", strLine + nocache, false);" \
"  request.send(null);" \
"}" \
"</script>" \
"</head>" \

"<body>" \
"<p><b>MD_Parola set message</b></p>" \

"<form id=\"data_form\" name=\"frmText\">" \
"<label>Message:<br><input type=\"text\" name=\"Message\" maxlength=\"255\"></label>" \
"<br><br>" \
"<input type = \"radio\" name = \"Invert\" value = \"0\" checked> Normal" \
"<input type = \"radio\" name = \"Invert\" value = \"1\"> Inverse" \
"<br>" \
"<input type = \"radio\" name = \"ScrollType\" value = \"L\" checked> Left Scroll" \
"<input type = \"radio\" name = \"ScrollType\" value = \"R\"> Right Scroll" \
"<br><br>" \
"<label>Speed:<br>Fast<input type=\"range\" name=\"Speed\"min=\"10\" max=\"200\">Slow"\
"<br>" \
"</form>" \
"<br>" \
"<input type=\"submit\" value=\"Send Data\" onclick=\"SendData()\">" \
"</body>" \
"</html>";

char *err2Str(wl_status_t code) {
    switch (code) {
    case WL_IDLE_STATUS:    return("IDLE");           break; // WiFi is in process of changing between statuses
    case WL_NO_SSID_AVAIL:  return("NO_SSID_AVAIL");  break; // case configured SSID cannot be reached
    case WL_CONNECTED:      return("CONNECTED");      break; // successful connection is established
    case WL_CONNECT_FAILED: return("CONNECT_FAILED"); break; // password is incorrect
    case WL_DISCONNECTED:   return("CONNECT_FAILED"); break; // module is not configured in station mode
    default: return("??");
    }
}

uint8_t htoi(char c) {
    c = toupper(c);
    if ((c >= '0') && (c <= '9')) return(c - '0');
    if ((c >= 'A') && (c <= 'F')) return(c - 'A' + 0xa);
    return(0);
}


//-----------------------------------------------------------------------------r


// handle errors by displaying a code and then restart
void errorHandler(int error) {
    textPosition_t scrollAlign = PA_LEFT;
    uint8_t scrollSpeed = 25;                      // default frame delay value
    uint16_t scrollPause = TWO_SECOND;             // in milliseconds
    textEffect_t scrollEffectIn = PA_SCROLL_LEFT;
    textEffect_t scrollEffectOut = PA_NO_EFFECT;
    int i = 0;
    unsigned long tout;

/*
    sprintf(curMessage[0], "Can't go on without WiFi connection.  Press reset twice to fix.");
    P.displayText(curMessage[0], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);

    tout = ONE_MINUTE + millis();                // milliseconds of time given to making connection attempt
    while (millis() < tout) {
        if (P.displayAnimate()) {
            Serial.println("Inside errorHandler");
            P.displayReset();        // reset the animation back to the beginning
        }
        yield();    // prevent the watchdog timer doing a reboot
    }
*/

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


// terminate the wifi connect
void wifiTerminate() {
    Serial.print("\nDisconnecting from WiFi with SSID ");
    Serial.println(WiFi.SSID());

    WiFi.disconnect();

    Serial.println("\n-------------------------------------------------------");
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


/*
// start listening for UDP messages on port localPort
void startUDP() {
    if (udp.begin(localPort))
        Serial.print("\nStarting UDP for NTP connection.  ");
    else
        Serial.print("Failed to start UDP listener.  ");

    Serial.print("Using local port ");
    Serial.println(udp.localPort());
}


// stop listening for UDP messages on port localPort
void stopUDP() {
    Serial.print("Stopping UDP on local port ");
    Serial.println(udp.localPort());
    Serial.print("\n");

    udp.stop();
}
*/


//------------------------------------------------------------------------------


/*
void getData(char *szMesg, uint8_t len)
// Message may contain data for:
// New text (/&MSG=)
// Scroll direction (/&SD=)
// Invert (/&I=)
// Speed (/&SP=)
{
  char *pStart, *pEnd;      // pointer to start and end of text

  // check text message
  pStart = strstr(szMesg, "/&MSG=");
  if (pStart != NULL)
  {
    char *psz = newMessage;

    pStart += 6;  // skip to start of data
    pEnd = strstr(pStart, "/&");

    if (pEnd != NULL)
    {
      while (pStart != pEnd)
      {
        if ((*pStart == '%') && isdigit(*(pStart + 1)))
        {
          // replace %xx hex code with the ASCII character
          char c = 0;
          pStart++;
          c += (htoi(*pStart++) << 4);
          c += htoi(*pStart++);
          *psz++ = c;
        }
        else
          *psz++ = *pStart++;
      }

      *psz = '\0'; // terminate the string
      newMessageAvailable = (strlen(newMessage) != 0);
      PRINT("\nNew Msg: ", newMessage);
    }
  }

  // check scroll direction
  pStart = strstr(szMesg, "/&SD=");
  if (pStart != NULL)
  {
    pStart += 5;  // skip to start of data

    PRINT("\nScroll direction: ", *pStart);
    scrollEffect = (*pStart == 'R' ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayReset();
  }

  // check invert
  pStart = strstr(szMesg, "/&I=");
  if (pStart != NULL)
  {
    pStart += 4;  // skip to start of data

    PRINT("\nInvert mode: ", *pStart);
    P.setInvert(*pStart == '1');
  }

  // check speed
  pStart = strstr(szMesg, "/&SP=");
  if (pStart != NULL)
  {
    pStart += 5;  // skip to start of data

    int16_t speed = atoi(pStart);
    PRINT("\nSpeed: ", P.getSpeed());
    P.setSpeed(speed);
    frameDelay = speed;
  }
}
*/


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
    PRINTS("\nS_IDLE");
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
    PRINT("\nNew client @ ", szTxt);
#endif

    timeStart = millis();
    state = S_READ;
  }
  break;

  case S_READ: // get the first line of data
    PRINTS("\nS_READ ");

    while (client.available())
    {
      char c = client.read();

      if ((c == '\r') || (c == '\n'))
      {
        szBuf[idxBuf] = '\0';
        client.flush();
        PRINT("\nRecv: ", szBuf);
        state = S_EXTRACT;
      }
      else
        szBuf[idxBuf++] = (char)c;
    }
    if (millis() - timeStart > 1000)
    {
      PRINTS("\nWait timeout");
      state = S_DISCONN;
    }
    break;

  case S_EXTRACT: // extract data
    PRINTS("\nS_EXTRACT");
    // Extract the string from the message if there is one
    getData(szBuf, BUF_SIZE);
    state = S_RESPONSE;
    break;

  case S_RESPONSE: // send the response to the client
    PRINTS("\nS_RESPONSE");
    // Return the response to the client (web page)
    client.print(WebResponse);
    client.print(WebPage);
    state = S_DISCONN;
    break;

  case S_DISCONN: // disconnect client
    PRINTS("\nS_DISCONN");
    client.flush();
    client.stop();
    state = S_IDLE;
    break;

  default:  state = S_IDLE;
  }
}
*/


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
    PRINT("\ncurMessage = ", curMessage[MessageInQueue - 1]);
    P.displayText(curMessage[MessageInQueue - 1], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
}


void loop() {
    textPosition_t scrollAlign = PA_LEFT;
    uint8_t scrollSpeed = 25;                      // default frame delay value
    uint16_t scrollPause = TWO_SECOND;             // in milliseconds
    textEffect_t scrollEffectIn = PA_SCROLL_LEFT;
    textEffect_t scrollEffectOut = PA_NO_EFFECT;
    int i = 0;

/*
    handleWiFi();
*/

    Serial.println("Outside the loop");
    if (P.displayAnimate()) {
        Serial.println("Inside the loop");
        P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);
        i++;
        if (i == MessageInQueue) i = 0;
        P.displayReset();        // reset the animation back to the beginning
    }

/*
    Serial.println("Outside the loop");
    if (P.displayAnimate()) {
        Serial.println("Inside the loop");
        if (newMessageAvailable) {
            strcpy(curMessage[MessageInQueue - 1], newMessage);
            newMessageAvailable = false;
        }
        P.displayReset();        // reset the animation back to the beginning
    }
*/
}
