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
    screen /dev/ttyUSB0 9600,cs8
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

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <SPI.h>
#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "debug.h"
#include "WiFiTools.h"
#include "credentials.h"
#include "MessageStore.h"


#define ONE_SECOND    1000UL        // milliseconds in one second
#define TWO_SECOND    2000UL        // milliseconds in two second
#define THREE_SECOND  3000UL        // milliseconds in three second
#define ONE_MINUTE    60000UL       // milliseconds in one minute
#define ONE_HOUR      3600000UL     // milliseconds in one hour
#define ONE_DAY       85400000UL    // milliseconds in one day

/*// display speeds, intensity, etc.*/
/*#define MAX_DEVICES   24    // number of dot matrix modules*/
/*#define PAUSE_TIME    TWO_SECOND*/
/*#define SCROLL_SPEED  50    // lower the number the faster the animation; 0 to run as fast as possible*/
/*#define FRAMEDELAY   200    // frame delay value min=10 (fast)  max=200 (slow)*/
/*#define INTENSITY      0    // set intensity of the display (0-15)*/
/*#define SPACING        0    // distance between the end of one message and the start of the next (0 = off display)*/

// Define the hardware interface and PINs used for wiring
// NOTE: These pin numbers are for ESO8266 hardware SPI
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW    // values are: PAROLA, GENERIC, ICSTATION, FC16
#define CLK_PIN   D5    // or SCK
#define DATA_PIN  D7    // or MOSI
#define CS_PIN    D8    // or SS

// display text effects
const textPosition_t SCROLLALIGN = PA_CENTER;        // how to align the text (e.g. PA_LEFT)
const textEffect_t SCROLLEFFECTIN = PA_SCROLL_UP;    // direction of scrolling (e.g. PA_SCROLL_LEFT)
const textEffect_t SCROLLEFFECTOUT = PA_DISSOLVE;    // special effects when scrolling (e.g. PA_RANDOM)
const uint8_t MAX_DEVICES = 24;     // number of dot matrix modules
const uint8_t SCROLLSPEED = 50;     // lower the number the faster the animation; 0 to run as fast as possible
const uint8_t FRAMEDELAY = 200;     // frame delay value min=10 (fast)  max=200 (slow)
const uint8_t INTENSITY = 0;        // set intensity of the display (0-15)
const uint8_t SPACING = 0;          // distance between the end of one message and the start of the next (0 = off display)
const uint8_t JUSTIFY_SET = 6;      // change the justification
const uint8_t INVERSE_SET = 0;      // set/reset the display to inverse
const uint16_t SCROLLPAUSE = 2000;  // ms of pause after finished displaying message

// Scrolling parameters
//uint8_t frameDelay = FRAMEDELAY;
//textEffect_t scrollEffect = PA_SCROLL_LEFT;

// Parola object constructor for SPI hardware interface
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Parola object constructor for software SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// WiFiTools object constructor
WiFiTools WT = WiFiTools();

// Global message buffers shared by Wifi and Scrolling functions
#define QUEUE_SIZE  5         // number of messages stored in a queue
#define BUF_SIZE    512       // max number of characters in a message
bool newMessageAvailable = false;
int MessageInQueue = 0;
char curMessage[QUEUE_SIZE][BUF_SIZE];
char newMessage[BUF_SIZE];

// MessageStore object constructor for storing the contents of the display
//MessageStore Msg = MessageStore(5, 5, 80);
MessageStore Msg = MessageStore();






//--------------------------- WiFi and Web Stuff ???? --------------------------

// WiFi Server object and parameters
WiFiServer server(80);

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



//--------------------- Error Message Handler for Display ----------------------

// handle errors by displaying a code and then restart
void errorHandler(int error) {

    int i = 0;
    unsigned long tout;                           // time-out time
    int cycle = 0;
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    switch(error) {
        case 1:
            FATAL("Can't go on without WiFi connection.  Press reset twice to fix.\n\r");
            Msg.clear();
            Msg.addQueue("Can't go on without WiFi connection.");
            Msg.addQueue("Press reset twice to fix.");

            tout = ONE_MINUTE + millis();         // milliseconds of time to display message
            while (millis() < tout) {
                if (P.displayAnimate()) {
                    if (Msg.getQueue(top + cycle)[0] != '\0')
                        P.displayText(Msg.getQueue(top + cycle), SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
                    cycle = (cycle + 1) % size; // prepare index into msg[] for next pass
                }
                yield();                          // prevent the watchdog timer doing a reboot
            }

            // nothing can be done so restart
            FATAL("Nothing can be done.  Doing an automatic restart.\n\r");
            ESP.reset();
            break;
        default:
            // nothing can be done so restart
            ERRORD("Unknown error code in errorHandler: ", error);
            FATAL("Nothing can be done.  Doing an automatic restart.\n\r");
            ESP.reset();
    }
}

/*// handle errors by displaying a code and then restart*/
/*void errorHandler(int error) {*/
    /*textPosition_t scrollAlign = PA_LEFT;*/
    /*uint8_t scrollSpeed = 25;                      // default frame delay value*/
    /*uint16_t scrollPause = TWO_SECOND;             // in milliseconds*/
    /*textEffect_t scrollEffectIn = PA_SCROLL_LEFT;*/
    /*textEffect_t scrollEffectOut = PA_NO_EFFECT;*/
    /*int i = 0;*/
    /*unsigned long tout;*/

    /*sprintf(curMessage[0], "Can't go on without WiFi connection.  Press reset twice to fix.");*/
    /*P.displayText(curMessage[0], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);*/

    /*tout = ONE_MINUTE + millis();                // milliseconds of time given to making connection attempt*/
    /*while (millis() < tout) {*/
        /*if (P.displayAnimate()) {*/
            /*Serial.println("Inside errorHandler");*/
            /*P.displayReset();        // reset the animation back to the beginning*/
        /*}*/
        /*yield();    // prevent the watchdog timer doing a reboot*/
    /*}*/

    /*sprintf(curMessage[0], "Can't go on without WiFi connection.");*/
    /*sprintf(curMessage[1], "Press reset twice to fix.");*/
    /*MessageInQueue = 2;*/

    /*tout = ONE_MINUTE + millis();                // milliseconds of time given to making connection attempt*/
    /*while (millis() < tout) {*/
        /*if (P.displayAnimate()) {*/
            /*INFO("Inside errorHandler");*/
            /*P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);*/
            /*i++;*/
            /*if (i == MessageInQueue) i = 0;*/
            /*P.displayReset();        // reset the animation back to the beginning*/
        /*}*/
        /*yield();    // prevent the watchdog timer doing a reboot*/
    /*}*/

    /*Serial.flush();*/

    /*ESP.reset();                     // nothing can be done so restart*/
/*}*/



//------------------------------------------------------------------------------



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
      INFOS("New Msg: ", newMessage);
    }
  }

  // check scroll direction
  pStart = strstr(szMesg, "/&SD=");
  if (pStart != NULL)
  {
    pStart += 5;  // skip to start of data

    INFOS("Scroll direction: ", *pStart);
    scrollEffect = (*pStart == 'R' ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayReset();
  }

  // check invert
  pStart = strstr(szMesg, "/&I=");
  if (pStart != NULL)
  {
    pStart += 4;  // skip to start of data

    INFOS("Invert mode: ", *pStart);
    P.setInvert(*pStart == '1');
  }

  // check speed
  pStart = strstr(szMesg, "/&SP=");
  if (pStart != NULL)
  {
    pStart += 5;  // skip to start of data

    int16_t speed = atoi(pStart);
    INFOD("Speed: ", P.getSpeed());
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



/*//------------------------------- Main Routines --------------------------------*/

/*void setup() {*/
    /*textPosition_t scrollAlign = PA_LEFT;*/
    /*uint8_t scrollSpeed = 25;                      // default frame delay value*/
    /*uint16_t scrollPause = TWO_SECOND;             // in milliseconds*/
    /*textEffect_t scrollEffectIn = PA_SCROLL_LEFT;*/
    /*textEffect_t scrollEffectOut = PA_NO_EFFECT;*/

    /*Serial.begin(9600);*/
    /*INFO("\n\rInitializing scrolling display ...");*/

    /*// initialize all your display messages to null*/
    /*newMessage[0] = '\0';*/
    /*MessageInQueue = 0;*/
    /*for (int j = 0; j < QUEUE_SIZE; j++)*/
            /*curMessage[0][j] = '\0';*/

    /*P.begin();                     // initialize the object data*/
    /*P.displayClear();              // clear all the zones in the current display*/

    /*P.displaySuspend(false);       // start the current display animation, true = suspend, false = resume*/
    /*P.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);*/

    /*// scan for wifi access point and print what you find - useful for trouble shouting wifi*/
    /*INFO("Initializing WiFi for scanning...\n\r");*/
    /*WT.wifiScan();*/

    /*// Connect to and initialise WiFi network*/
    /*INFOS("Connecting to WIFi SSID ", WIFISSID);*/

    /*//WiFi.begin(ssid, password);*/
    /*if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {  // connect to wifi*/
    /*} else {*/
        /*Serial.println("Can't go on without WiFi connection.  Press reset twice to fix.");*/
        /*errorHandler(1);*/
    /*}*/

    /*// set up first message as the IP address*/
    /*MessageInQueue++;*/
    /*sprintf(curMessage[MessageInQueue - 1], "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);*/
    /*INFOS("curMessage = ", curMessage[MessageInQueue - 1]);*/
    /*P.displayText(curMessage[MessageInQueue - 1], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);*/
/*}*/


/*void loop() {*/
    /*textPosition_t scrollAlign = PA_LEFT;*/
    /*uint8_t scrollSpeed = 25;                      // default frame delay value*/
    /*uint16_t scrollPause = TWO_SECOND;             // in milliseconds*/
    /*textEffect_t scrollEffectIn = PA_SCROLL_LEFT;*/
    /*textEffect_t scrollEffectOut = PA_NO_EFFECT;*/
    /*int i = 0;*/

    /*INFO("Outside the loop");*/
    /*if (P.displayAnimate()) {*/
        /*INFO("Inside the loop");*/
        /*P.displayText(curMessage[i], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);*/
        /*i++;*/
        /*if (i == MessageInQueue) i = 0;*/
        /*P.displayReset();        // reset the animation back to the beginning*/
    /*}*/

/*}*/



//------------------------------ Helper Routines -------------------------------

void loadmsg(void) {

    char string[BUF_SIZE];

    // clear all old messages
    INFO("Populating message queue with messages...\n\r");
    Msg.clearQueue();
    Msg.printQueue();

    // 1st message in simple store is the wifi IP address
    sprintf(string, "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    Msg.addStore(string);
    Msg.printStore();

    // 1st message is gibberish
    Msg.addQueue("#1 message in circular queue!!");
    Msg.printQueue();

    // 2nd message is gibberish
    Msg.addQueue("#2 The rain falls mainly on the plane in Spain");
    Msg.printQueue();

    // 3rd message is gibberish
    Msg.addQueue("#3 What is the weather outside right now?  What about inside?");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 4th message is gibberish
    Msg.addQueue("#4 short message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 5th message is gibberish
    Msg.addQueue("#5 loooooooooooong message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 6th message is gibberish
    Msg.addQueue("#6 Push off the oldest message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    // 7th message is gibberish
    Msg.addQueue("#7 This message should push off the next oldest message");
    Msg.printQueue();
    //delay(2000);
    yield();                                         // prevent the watchdog timer doing a reboot

    INFO("Exiting loadmsg()...\n\r");
    PRINT("\n-------------------------------------------------------\n\r");

}



//------------------------------- Main Routines --------------------------------

void setup() {
    char string[BUF_SIZE];
    unsigned long tout;                           // time-out time
    int cycle = 0;                                // message number being displayed
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    Serial.begin(9600);
    PRINT("\n-------------------------------------------------------\n\r");
    INFO("Entered setup()...\n\r");
    INFO("Initializing scrolling display...\n\r");

    // initialize the display (aka Parola object)
    P.begin();                                           // initialize the display and data object
    P.setIntensity(INTENSITY);                           // set intensity of the display
    P.setTextAlignment(SCROLLALIGN);                     // set the text alignment
    P.setTextEffect(SCROLLEFFECTIN, SCROLLEFFECTOUT);    // special effects when scrolling
    P.setScrollSpacing(SPACING);                         // columns between messages
    P.setSpeed(SCROLLSPEED);                             // frame delay value
    P.setPause(SCROLLPAUSE);                             // ms of pause after finished displaying message
    P.displayClear();                                    // clear the display

    tout = ONE_SECOND + millis();                        // milliseconds of time to display message
    while (millis() < tout) {
        if (P.displayAnimate())
            P.displayText("Stand-By!!", SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
        yield();                                         // prevent the watchdog timer doing a reboot
    }

    // scan for wifi access points
    INFO("Initializing WiFi...\n\r");
    WT.wifiScan();

    // initialize all your display messages to null
    Msg.clear();

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        sprintf(string, "WiFi connected successfully to SSID %s.", WIFISSID);
        Msg.addQueue(string);
        //tout = THREE_SECOND + millis();              // milliseconds of time to display message
        tout = ONE_SECOND + millis();                  // milliseconds of time to display message
        while (millis() < tout) {
            if (P.displayAnimate()) {
                if (Msg.getQueue(top + cycle)[0] != '\0')
                    P.displayText(Msg.getQueue(top + cycle), SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
                cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass
            }
            yield();                                   // prevent the watchdog timer doing a reboot
        }
    } else
        errorHandler(1);

    loadmsg();
}


void loop() {

    static int cycle = 0;                      // message number being displayed
    static int top = Msg.top();
    static int size = Msg.size();

    if (P.displayAnimate()) {
        if (Msg.get(top + cycle)[0] != '\0')
            P.displayText(Msg.get(top + cycle), SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
        cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass
    }

}
