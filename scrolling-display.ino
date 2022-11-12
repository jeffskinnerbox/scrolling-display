
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

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
        GND              GND           Ground
        DIN              D7            HSPID or HMOSI
        CS or LD         D8            HSPICS or HCS
        CLK              D5            CLK or HCLK

MONITOR:
    To monitor the scrolling display's activities on the NodeMCU serial ports while operating,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter: CNTR-a :quit
    To stop the screen scrolling, enter: CNTR-a :

    To monitor using telnet, ececute the following:

        telnet ESP_24F9FB          # using esp8266 hostname ('ESP_' + chip ID)
        telnet 192.168.1.47        # using esp8266 ip address
        telnet scrolling-display.local

    To terminate telnet monitoring, enter: Ctrl-] quit

    To find the device on your LAN:

        sudo arp-scan 192.168.1.200/24 | grep Espressif

TESTING
    For testing, you may want to "see" the messages posted to the display.

    To test the MQTT capabilities, use a public MQTT broker site
    and Mosquitto clients like this:

        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "outTopic-ntp-clock/time"
        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "outTopic-test-scrolling-display/msg"

        mosquitto_pub -h broker.mqtt-dashboard.com -t "inTopic-test-scrolling-display/msg-queue" -m "test message"
        mosquitto_pub -h broker.mqtt-dashboard.com -t "inTopic-test-scrolling-display/command" -m "2"


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
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

// found in Arduino libraries (~/Arduino/libraries)

// found in Arduino libraries (~/src/arduino/libraries)
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
#include <PubSubClient.h>

// this project's include files
#include "DeBug.h"
#include "secrets.h"
#include "OTAHandler.h"
#include "WiFiHandler.h"
#include "MQTTHandler.h"
#include "MessageStore.h"
#include "LinkedList.h"      // TAKE THIS OUT

//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "scrolling-display" " - "  __DATE__ " at " __TIME__
static char version[] = VER;

#define ONE_SECOND    1000UL        // milliseconds in one second
#define TWO_SECOND    2000UL        // milliseconds in two second
#define THREE_SECOND  3000UL        // milliseconds in three second
#define ONE_MINUTE    60000UL       // milliseconds in one minute
#define ONE_HOUR      3600000UL     // milliseconds in one hour
#define ONE_DAY       85400000UL    // milliseconds in one day

// error codes
#define NOWIFI        1             // can't get wifi connection
#define UNKNMQTT      2             // error code meaning don't understand mqtt command request

// Define the hardware interface and PINs used for wiring
// NOTE: These pin numbers are for ESO8266 hardware SPI
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW    // values are: PAROLA, GENERIC, ICSTATION, FC16
#define CLK_PIN   D5    // or SCK
#define DATA_PIN  D7    // or MOSI
#define CS_PIN    D8    // or SS

//-------------------------- Static Scoped Variables ---------------------------

// Update these with values suitable for your wifi and mqtt broker
static char* wifi_ssid = WIFISSID;
static char* wifi_password = WIFIPASS;
static unsigned long wifi_time = WIFITIME;
static char* mqtt_server = MQTTSERVER;
static int mqtt_port = MQTTPORT;

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


extern DeBug DB;         // declare object DB as external, and member of class DeBug
extern OTAHandler OTA;   // declare object OTA as external, and member of class OTAHandler

// Parola object constructor for software SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// wifi and mqtt handlers / object constructor
WiFiHandler WT = WiFiHandler();
MQTTHandler handlerMQTT = MQTTHandler(mqtt_server, mqtt_port);

// MessageStore object constructor for storing the contents of the display
MessageStore Msg = MessageStore(5, 5, 80);



//------------------------------- MQTT Parameters ------------------------------

// MQTT message buffer for publishing and subscription
static const int MSGSIZE = 80;                 // size of mqtt message buffer
static char msg[MSGSIZE];                      // buffer to hold mqtt messages


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
void errorHandler(int error_code, char *msg) {

    int i = 0;
    unsigned long tout;                           // time-out time
    int cycle = 0;
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    DEBUGTRACE(INFO, "In errorHandsler() and error_code = %d", error_code);

    switch(error_code) {
        case NOWIFI:
            DEBUGTRACE(ERROR, msg);
            DEBUGTRACE(FATAL, "An automated restart will be requested.");
            handlerMQTT.publish(ERRORTOPIC, msg, false);
            Msg.clear();
            Msg.addQueue("Can't go on without WiFi connection.");
            Msg.addQueue("Press reset twice to fix.");

            tout = ONE_MINUTE + millis();         // milliseconds of time to display message
            while (millis() < tout) {
                if (P.displayAnimate()) {
                    if (Msg.getQueue(top + cycle)[0] != '\0')
                        P.displayText(Msg.getQueue(top + cycle), SCROLLALIGN, SCROLLSPEED,
                                SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
                    cycle = (cycle + 1) % size; // prepare index into msg[] for next pass
                }
                yield();                        // prevent the watchdog timer from doing a reboot
            }

            // nothing can be done so restart
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.reset();                     // nothing can be done so restart
            break;
        default:
            // nothing can be done so restart
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: ", error_code);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            handlerMQTT.publish(ERRORTOPIC, "Unknown error code in errorHandler", false);
            Serial.flush();                  // make sure serial messages are posted
            ESP.reset();                     // nothing can be done so restart
    }
}



//-------------------------------- MQTT Routines -------------------------------

//void SubscriptionCallback(char* topic, byte* payload, unsigned int length) {
void SubscriptionCallback(char* topic, unsigned char* payload, unsigned int length) {

    DEBUGPRINT("\r\n");
    DEBUGTRACE(INFO, "Message arrived on topic = ", topic);

    snprintf(msg, length + 1, "%s", payload);
    DEBUGTRACE(INFO, "MQTT message = ", msg);

    /*switch((char)payload[0]) {*/
        /*case '1':          // toggle the nodemcu red LED to blinking or off*/
            /*if (blinkLED) {*/
                /*blinkLED = false;*/
                /*nodemcuLED = HIGH;*/
                /*digitalWrite(LED, nodemcuLED);   // make sure nodmcu red led is 'off'*/
            /*} else {*/
                /*blinkLED = true;*/
            /*}*/
            /*break;*/
        /*case '2':          // write to topic how much the clock has drifted from the ntp time server*/
            /*TimeDriftCheck();*/
            /*break;*/
        /*case '3':          // forced time refresh*/
            /*TimeRefresh(NULL);*/
            /*break;*/
        /*default:*/
            /*errorHandler(UNKNMQTT, "MQTT message unknown.  No action taken.");*/
    /*}*/
}


/*bool MQTTPublishMsg(char *topic, char* payload) {*/

    /*// if not connect to mqtt broker, then reconnect*/
    /*if (!handlerMQTT.connected()) {*/
        /*handlerMQTT.reconnect();*/
    /*}*/

    /*// publish message*/
    /*handlerMQTT.publish(topic, payload, true);*/

/*    // if not connect to mqtt broker, then reconnect*/
    /*if (!handlerMQTT.connected()) {*/
        /*handlerMQTT.reconnect();*/
    /*}*/

    /*// format message for sending*/
    /*if (status24hour)*/
        /*snprintf(msg, MSGSIZE, "%02d:%02d:%02d", displayHours, displayMinutes, displaySeconds);*/
    /*else*/
        /*snprintf(msg, MSGSIZE, "%02d:%02d:%02d%s", displayHours, displayMinutes, displaySeconds, displayAMPM);*/

    /*// publish message*/
    /*handlerMQTT.publish(TIMETOPIC, msg, true);*/

/*}*/




//------------------------------ Helper Routines -------------------------------

void loadmsg(void) {

    char string[BUF_SIZE];

    DEBUGTRACE(INFO, "------------------------------ Entering loadmsg() ------------------------------");

    DEBUGTRACE(INFO, "Populating message queue with messages...");

    // clear all old messages
    Msg.clear();

    // 1st message in simple store is the wifi IP address
    snprintf(string, BUF_SIZE, "IP Address is %03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    Msg.addStore(string);
    //Msg.printStore();

    // 1st message is gibberish
    Msg.addQueue("#1 message in circular queue!!");
    //Msg.printQueue();

    // 2nd message is gibberish
    Msg.addQueue("#2 The rain falls mainly on the plane in Spain");
    //Msg.printQueue();

    // 3rd message is gibberish
    Msg.addQueue("#3 What is the weather outside right now?  What about inside?");
    //Msg.printQueue();
    //yield();                                         // prevent the watchdog timer doing a reboot

    // 4th message is gibberish
    Msg.addQueue("#4 short message");
    //Msg.printQueue();
    //yield();                                         // prevent the watchdog timer doing a reboot

    // 5th message is gibberish
    Msg.addQueue("#5 loooooooooooong message");
    //Msg.printQueue();
    //yield();                                         // prevent the watchdog timer doing a reboot

    // 6th message is gibberish
    Msg.addQueue("#6 Push off the oldest message");
    //Msg.printQueue();
    //yield();                                         // prevent the watchdog timer doing a reboot

    // 7th message is gibberish
    Msg.addQueue("#7 This message should push off the next oldest message");
    //Msg.printQueue();
    //yield();                                         // prevent the watchdog timer doing a reboot

    DEBUGTRACE(INFO, "------------------------------- Exiting loadmsg() ------------------------------");

}



//------------------------------- Main Routines --------------------------------

void setup() {
    char string[BUF_SIZE];
    unsigned long tout;                                  // time-out time
    int cycle = 0;                                       // message number being displayed
    int top = Msg.topQueue();
    int size = Msg.sizeQueue();

    // always start Serial first so it can be used by DeBug
    Serial.begin(9600);
    while (!Serial) {}                                   // wait for serial port to connect

    DEBUGSETUP();                                        // should be right after 'Serial.begin'
    DEBUGTRACE(INFO, "--------------------------------- In setup() -----------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);
    DEBUGLOCATION();                                     // just a test stub for the "location" feature
    DEBUGSTATUS();                                       // provide information about debug status flags
    DEBUGINFO();                                         // provide some useful information about the microprocessor

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

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        snprintf(string, BUF_SIZE, "WiFi connected successfully to SSID %s.", WIFISSID);
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
        errorHandler(NOWIFI, "Can't go on without WiFi connection. Press reset to try again.");

    OTA.setupOTA();            // should always follow 'DEBUGSETUP()' and after wifi is up

    handlerMQTT.reconnect();   // connect to mqtt broker
    handlerMQTT.setServer(mqtt_server, mqtt_port); // set your mqtt broker to be used
    handlerMQTT.subscribe(MSGQUEUETOPIC);  // subscribe to a topic of interest
    handlerMQTT.setCallback(SubscriptionCallback); // set the callback for subscribe topic

    loadmsg();                 // load the messages to be displayed

    LinkedList LL = LinkedList();    // TAKE THIS OUT
    LL.testcases();                  // TAKE THIS OUT

    DEBUGTRACE(INFO, "------------------------------- Entering loop() --------------------------------");

}


void loop() {
    char string[BUF_SIZE];
    static int cycle = 0;                      // message number being displayed
    static int top = Msg.top();
    static int size = Msg.size();

    DEBUGLOOP();
    OTA.loopOTA();

    if (P.displayAnimate()) {
        if (Msg.get(top + cycle)[0] != '\0')
            P.displayText(Msg.get(top + cycle), SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);
            snprintf(string, BUF_SIZE, "Posting to display message[%d] = %s", cycle, Msg.get(top + cycle));
            DEBUGTRACE(INFO, string);
            handlerMQTT.publish(MSGDISTOPIC, Msg.get(top + cycle), false);
        cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass
    }

}
