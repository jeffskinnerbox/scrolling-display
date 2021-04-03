
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0

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
    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Cntr-a :quit

TESTING:
    xxxxxxxxxx

USAGE:
    xxxxxxxxxx

REFERENCE MATERIALS:
    * MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
    https://lastminuteengineers.com/esp8266-ota-updates-arduino-ide/
    https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/

SOURCES:
    Code adapted from https://github.com/MajicDesigns/MD_Parola/tree/master/examples/Parola_Scrolling_ESP8266
    Code adapted from https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
    https://tttapa.github.io/ESP8266/Chap13%20-%20OTA.html
    ~/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

#define TDEBUG  true       // activate trace message printing for debugging

// found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <SPI.h>
#include <ESP8266WiFi.h>

// found in Arduino libraries (~/Arduino/libraries)

// found in Arduino libraries (~/src/arduino/libraries)
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// this project's include files
#include "Ota.h"
#include "DeBug.h"
#include "secrets.h"
#include "WiFiHandler.h"
#include "MessageStore.h"

#define ONE_SECOND    1000UL        // milliseconds in one second
#define TWO_SECOND    2000UL        // milliseconds in two second
#define THREE_SECOND  3000UL        // milliseconds in three second
#define ONE_MINUTE    60000UL       // milliseconds in one minute
#define ONE_HOUR      3600000UL     // milliseconds in one hour
#define ONE_DAY       85400000UL    // milliseconds in one day

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


extern DeBug DB;        // declare object DB as external, and member of class DeBug
extern Ota OTA;         // declare object OTA as external, and member of class Ota

// Parola object constructor for software SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// WiFiTools object constructor
WiFiTools WT = WiFiTools();

// MessageStore object constructor for storing the contents of the display
//MessageStore Msg = MessageStore(5, 5, 80);
MessageStore Msg = MessageStore();

// version stamp
#define VER "scrolling-display" " - "  __DATE__ " at " __TIME__
char version[] = VER;



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
    default:
        DEBUGTRACE(WARN, "Unknown code passed to *err2Str(): ", code);
        return("??");
    }
}

uint8_t htoi(char c) {
    c = toupper(c);
    if ((c >= '0') && (c <= '9')) return(c - '0');
    if ((c >= 'A') && (c <= 'F')) return(c - 'A' + 0xa);
    return(0);
}



//--------------------- Error Message Handler for Display ----------------------

// handle errors by displaying a code and then taking action (e.g. restart)
void errorHandler(int error) {

    int i = 0;
    unsigned long tout;                           // time-out time
    int cycle = 0;
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    switch(error) {
        case 1:
            DEBUGTRACE(FATAL, "Can't go on without WiFi connection.  Press reset twice to fix.");
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
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.reset();
            break;
        default:
            // nothing can be done so restart
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: ", error);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.reset();
    }
}



//------------------------------ Helper Routines -------------------------------

void loadmsg(void) {

    char string[BUF_SIZE];


    DEBUGTRACE(INFO, "\r\nEntering loadmsg() for scrolling display");

    // clear all old messages
    DEBUGTRACE(INFO, "Populating message queue with messages...");
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

    DEBUGTRACE(INFO, "Exiting loadmsg() for scrolling display\r\n");

}



//------------------------------- Main Routines --------------------------------

void setup() {
    char string[BUF_SIZE];
    unsigned long tout;                       // time-out time
    int cycle = 0;                            // message number being displayed
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    // always start Serial first so it can be used by DeBug
    Serial.begin(9600);
    while (!Serial) {}                        // wait for serial port to connect

    DEBUGSETUP();
    DEBUGPRINT("\n\r");
    OTA.setupOTA();

    DEBUGTRACE(INFO, "\r\n\n--------------------------------------------------------------------------------");
    DEBUGTRACE(INFO, "Entering setup() for scrolling display");

    DEBUGTRACE(INFO, "\tApplication Version = ", version);

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
    WT.wifiScan();

    // initialize all your display messages to null
    Msg.clear();

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(HOSTNAME, WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
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

    // load the messages to be displayed
    loadmsg();

    // provide some useful information about the microprocessor
    DEBUGINFO();

    DEBUGTRACE(INFO, "Exiting setup() for scrolling display");
    DEBUGTRACE(INFO, "--------------------------------------------------------------------------------\r\n\n");

}


void loop() {

    static int cycle = 0;                      // message number being displayed
    static int top = Msg.top();
    static int size = Msg.size();

    DEBUGLOOP();
    OTA.loopOTA();

    if (P.displayAnimate()) {
        if (Msg.get(top + cycle)[0] != '\0')
            P.displayText(Msg.get(top + cycle), SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
            DEBUGTRACE(INFO, "Posting to display message = ", Msg.get(top + cycle));
        cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass
    }

}
