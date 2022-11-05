
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:

PHYSICAL DESIGN:
    Hardware

    Wiring

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

USAGE:
    xxxxxxxxxx

REFERENCE MATERIALS:

SOURCES:
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

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// this project's include files
#include "DeBug.h"


//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "debug" " - "  __DATE__ " at " __TIME__
static char version[] = VER;

#define DATASIZE 50         // max characters in Node data

extern DeBug DB;         // declare object DB as external, and member of class DeBug
//extern OTAHandler OTA;   // declare object OTA as external, and member of class OTAHandler


//-------------------------- Static Scoped Variables ---------------------------



//------------------------------- MQTT Parameters ------------------------------


//--------------------------- WiFi and Web Stuff ???? --------------------------




//--------------------- Error Message Handler for Display ----------------------




//-------------------------------- MQTT Routines -------------------------------




//------------------------------ Helper Routines -------------------------------

// test cases for debugging code
void testcase_1(void) {

    DEBUGTRACE(INFO, "----------------------------- Entering testcase_1() -----------------------------");

    DEBUGTRACE(HEADING, "just a test stub for the \"location\" feature");
    DEBUGLOCATION();                 // just a test stub for the "location" feature

    DEBUGTRACE(HEADING, "provide information about debug status flags");
    DEBUGSTATUS();                   // provide information about debug status flags

    DEBUGTRACE(HEADING, "provide some useful information about the microprocessor");
    DEBUGINFO();                     // provide some useful information about the microprocessor

    DEBUGTRACE(INFO, "----------------------------- Exiting testcase_1() ------------------------------");

}


// test cases for debugging code
void testcase_2(void) {
    struct Node {              // A node to create a linked list
        char data[DATASIZE];
        Node *next;
    };                         // NOTE: you must have the ';' in structures
    Node *head;                // head pointer for linked list
    float pi = 3.1415926536;

    DEBUGTRACE(INFO, "----------------------------- Entering testcase_2() -----------------------------");

    // create and assign data to first node in linked list
    Node *node_1 = new Node;
    head = node_1;
    head->next = NULL;
    snprintf(node_1->data, DATASIZE, "%s", "1st data node inside linked list");

    // create and assign data to second node in linked list
    Node *node_2 = new Node;
    head->next = node_2;
    node_2->next = NULL;
    snprintf(node_2->data, DATASIZE, "%s", "2nd data node inside linked list");

    // for expressions like Serial.println(char* str) and Serial.print(char* str); Serial.println(char* str)
    DEBUGTRACE(HEADING, "String Expressions: e.g. DEBUGTRACE(INFO, \"\\tJust a single string message\");");
    DEBUGTRACE(INFO, "Just a single string message");

    //DEBUGTRACE(INFO, "\nSimple String Expressions Using F\(\) macro:");
    //DEBUGTRACE(INFO, F("\tYou can also put a simple string in flash memory instead of SRAM via the F\(\) macro"));
    //DEBUGTRACE(INFO, (reinterpret_cast<__FlashStringHelper *>(PSTR("\tYou can also put a simple string in flash memory instead of SRAM via the F macro"))));

    // for expressions like Serial.print("i = "); Serial.println(i)
    DEBUGTRACE(HEADING, "Variable Expressions: e.g. DEBUGTRACE(INFO, pi);");
    //DEBUGTRACE(INFO, pi);

    // for expressions like Serial.print(char *str); Serial.print(var, format)
    DEBUGTRACE(HEADING, "String and Variable Expressions: e.g. DEBUGTRACE(INFO, \"ESP8266 Chip ID = \", ESP.getChipId());");
    DEBUGTRACE(INFO, "ESP8266 Chip ID = ", ESP.getChipId());
    DEBUGTRACE(INFO, "head->data = ", head->data);
    DEBUGTRACE(INFO, "Version string for this program = ", version);
    DEBUGTRACE(INFO, "Should be the same: node_1->data = ", node_1->data);
    DEBUGTRACE(INFO, "node_2->data = ", node_2->data);
    DEBUGTRACE(INFO, "Should be the same: head->next->data = ", head->next->data);

    //for expressions like Serial.print(char* str); Serial.println(int i, HEX);
    DEBUGTRACE(HEADING, "String and Variable Expressions: e.g. DEBUGTRACE(INFO, \"ESP8266 Chip ID (HEX) = %X\n\r\", ESP.getChipId(), HEX);");
    DEBUGTRACE(INFO, "ESP8266 Chip ID (HEX) = ", ESP.getChipId(), HEX);
    DEBUGTRACE(INFO, "ESP8266 Chip ID (HEX) = %X\n\r", ESP.getChipId(), HEX);
    DEBUGTRACE(INFO, "ESP8266 Chip ID (HEX) = %X\n\r", ESP.getChipId());

    DEBUGTRACE(HEADING, "Formating String and Variable Expressions:");
    //DEBUGTRACE(INFO, "\tESP8266 Chip ID (HEX) = %X", ESP.getChipId());

    //DEBUGTRACE(INFO, ESP.getChipId(), "\tESP8266 Chip ID (HEX) = %X");
    //DEBUGTRACE(INFO, "\thead->next = ", head->next);
    //DEBUGTRACE(INFO, "\tnode_2->next = ", node_2->next);
    //DEBUGTRACE(INFO, "\thead->next->next = ", head->next->next);

    // for expressions whenyou have a variable number of arguments
    // https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
    // https://www.geeksforgeeks.org/variable-length-argument-c/
    // https://en.cppreference.com/w/cpp/utility/variadic
    DEBUGTRACE(HEADING, "Formating String and Variable Number of Arguments:");
    //DEBUGTRACE(INFO, "\tA = %X, Z = %d pi = %d\n\r", A, Z, pi);

    DEBUGTRACE(INFO, "----------------------------- Exiting testcase_2() ------------------------------");

}




//------------------------------- Main Routines --------------------------------

void setup() {

    // always start Serial first so it can be used by DeBug
    Serial.begin(9600);
    while (!Serial) {}      // wait for serial port to connect
    DEBUGSETUP();           // for proper operation, should be right after 'Serial.begin'

    DEBUGTRACE(INFO, "------------------------------- Entering setup() -------------------------------");
    testcase_1();

/*    // scan for wifi access points*/
    /*WT.wifiScan();*/

    /*// attempt to connect and initialise WiFi network*/
    /*if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi*/
        /*snprintf(string, BUF_SIZE, "WiFi connected successfully to SSID %s.", WIFISSID);*/
        /*Msg.addQueue(string);*/
        /*//tout = THREE_SECOND + millis();              // milliseconds of time to display message*/
        /*tout = ONE_SECOND + millis();                  // milliseconds of time to display message*/
        /*while (millis() < tout) {*/
            /*if (P.displayAnimate()) {*/
                /*if (Msg.getQueue(top + cycle)[0] != '\0')*/
                    /*P.displayText(Msg.getQueue(top + cycle), SCROLLALIGN, SCROLLSPEED, SCROLLPAUSE, SCROLLEFFECTIN, SCROLLEFFECTOUT);*/
                /*cycle = (cycle + 1) % size;            // prepare index into msg[] for next pass*/
            /*}*/
            /*yield();                                   // prevent the watchdog timer doing a reboot*/
        /*}*/
    /*} else*/
        /*errorHandler(NOWIFI, "Can't go on without WiFi connection. Press reset to try again.");*/

    /*OTA.setupOTA();            // should always follow 'DEBUGSETUP()' and after wifi is up*/

    /*handlerMQTT.reconnect();   // connect to mqtt broker*/
    /*handlerMQTT.setServer(mqtt_server, mqtt_port); // set your mqtt broker to be used*/
    /*handlerMQTT.subscribe(MSGQUEUETOPIC);  // subscribe to a topic of interest*/
    /*handlerMQTT.setCallback(SubscriptionCallback); // set the callback for subscribe topic*/

    /*loadmsg();                 // load the messages to be displayed*/

    //LinkedList LL = LinkedList();    // TAKE THIS OUT
    testcase_2();

    DEBUGTRACE(INFO, "------------------ Exiting setup() And Now Going To loop() ---------------------");

}


void loop() {

    DEBUGLOOP();
    //OTA.loopOTA();

}
