
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.5.0

DESCRIPTION:
    I would like to be able to view serial monitor over WiFi, but serial monitor
    is not supported on IP network ports.  The Serial.print() sends data directly
    to the hardware serial pins so you need something that has a physical
    connection to those pins in order to read the data.

    This program creates a Telnet port on the ESP chip. You can then connect
    using a standard Telnet client to access the debugging output.

    This program test the OTA capabilities of the ESP8266. I show you how to
    turn on OTA updates temporaraly (up to 15 seconds after boot) and make use
    of telnet to serial trace messages for debugging.

    The first firmware upload has to be done over a serial port. If the OTA
    routines are correctly implemented in the sketch, then all subsequent
    uploads may be done over the air.

PHYSICAL DESIGN:
    Just ESP8266, nothing else required

MONITOR:
    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Ctrl-a :quit

    telnet test-ota.local
    to terminate Ctrl-] quit

TESTING:
    NA

USAGE:
    NA

REFERENCE MATERIALS:
    https://lastminuteengineers.com/esp8266-ota-updates-arduino-ide/
    https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/

SOURCES:
    Code adapted from https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
    https://tttapa.github.io/ESP8266/Chap13%20-%20OTA.html
    ~/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// Arduino libraries (~/Arduino/libraries)
#include <TelnetStream.h>

// Arduino libraries (~/src/arduino/libraries)
#include <WiFiUdp.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// test-ota project's include files (~/src/scrolling-display/test/ota/test-ota)
#include "credentials.h"

// variables for blinking an LED
#define LED D0               // ESP8266 Pin to which onboard LED is connected
#define STDBLKRATE 1000      // interval at which to blink LED (milliseconds)
#define OTABLKRATE 250       // interval at which to blink LED (milliseconds)

// variables for OTA management
#define MAXTIME 15000UL      // max time to wait for ota handler to engage
bool ota_flag = true;        // if true, engage the ota handler

// version stamp
#define VER OTAHOSTNAME " " __DATE__ " at " __TIME__
const char version[] = VER;



//------------------------------ Helper Routines -------------------------------

void setupWiFi() {

    Serial.print("ESP8266 MAC address = ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFISSID, WIFIPASS);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WIFi Connection failed!  Rebooting...");
        delay(2000);
        ESP.restart();
    }

}


void getFlashInfo() {

    Serial.println("\n\rInformation concerning flash memory chip");

    Serial.printf("Chip ID: %x hex\n\r", ESP.getFlashChipId());
    Serial.printf("Chip Real Size (from chip): %d bits\n\r", ESP.getFlashChipRealSize());
    Serial.printf("Chip Size (what compiler set): %d bits\n\r", ESP.getFlashChipSize());
    Serial.printf("Chip Speed: %d Hz\n\r", ESP.getFlashChipSpeed());
    Serial.printf("Chip Mode: %d\n\r", ESP.getFlashChipMode());
    Serial.printf("Free Heap Memory: %d bytes\n\r", ESP.getFreeHeap());
    Serial.printf("Heap Fragmentation: %d%%\n\r", ESP.getHeapFragmentation());  // 0% is clean, more than ~50% is not harmless

}


void blinkLED(unsigned long rate) {
    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();

    // blink LED to signal that you are OTA update ready
    if (currentTime - previousTime >= rate) {
        previousTime = currentTime;            // save the last time you blinked the LED
        digitalWrite(LED, !digitalRead(LED));  // toggle led state
    }

}


//-------------------------------- OTA Routines --------------------------------

void TelnetStreamHandler() {

    switch (TelnetStream.read()) {
        case 'R':   // reboot the esp8266
            Serial.println("\n\rRebooting ...");
            TelnetStream.println("\n\rRebooting ...");
            TelnetStream.flush();
            TelnetStream.stop();
            delay(100);
            ESP.reset();
            break;
        case 'C':   // drop telnet connection to esp8266
            Serial.println("\n\rDropping telnet connection ... bye bye");
            TelnetStream.println("\n\rDropping telnet connection ... bye bye");
            TelnetStream.flush();
            TelnetStream.stop();
            break;
    }

}


void setupOTA() {

    ArduinoOTA.setPort(OTAPORT);                       // if not set, port defaults to 8266 (3232 for esp32)
    ArduinoOTA.setHostname(OTAHOSTNAME);               // if not set, hostname defaults to esp8266-<ChipID-in-Hex>
    ArduinoOTA.setPassword((const char *)OTAPASSWORD); // if not set, no authentication required by default

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else {                      // U_SPIFFS
            type = "filesystem";
            // NOTE: if updating SPIFFS, this would be the place to unmount SPIFFS using SPIFFS.end()
        }

        Serial.println("Started updating " + type);
    });

    ArduinoOTA.onEnd([]() { Serial.println("\n\rUpdating ended"); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();

}

//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(9600);
    while (!Serial) {}          // wait for serial port to connect

    Serial.println("\n\n\rBooting ...");
    Serial.printf("Version = %s\n\r", version);
    Serial.printf("ESP8266 chip ID = %x\n\r", ESP.getChipId());

    setupWiFi();                // connect to wifi network

    setupOTA();                 // make esp8266 ready for ota support

    getFlashInfo();             // get a sense of memory to support OTA (maybe)

    pinMode(LED, OUTPUT);       // set led pin as output so you can blink it

    Serial.println("\n\rESP8266 OTA update is enabled.  ota_flag = " + String(ota_flag));
    Serial.println("hostname = " + ArduinoOTA.getHostname() + ".local");
    Serial.print("ip address = "); Serial.println(WiFi.localIP());
    Serial.println("OTA port = " + String(OTAPORT));

    TelnetStream.begin();

    Serial.println("\n\rTelnetStream enabled");
    TelnetStream.println("TelnetStream enabled");

    Serial.println("ESP8266 OTA update is enabled.  ota_flag = " + String(ota_flag));
    TelnetStream.println("ESP8266 OTA update is enabled.  ota_flag = " + String(ota_flag));

}


void loop() {
    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();
    static unsigned long elapsedTime, i = 0;

    TelnetStreamHandler();

    //TelnetStream.printf("ota_flag = %d\n\r", ota_flag);

    // if ota is being requested, activate the handler
    if (ota_flag) {
        while (currentTime - previousTime < MAXTIME) {
            ArduinoOTA.handle();     // OTA handler, look for OTA update request
            currentTime = millis();
            blinkLED(OTABLKRATE);    // blink led at ota update enabled rate
            delay(10);
        }
        ota_flag = false;
        Serial.println("\n\rESP8266 OTA update is disabled.  ota_flag = " + String(ota_flag));
        TelnetStream.println("\n\rESP8266 OTA update is disabled.  ota_flag = " + String(ota_flag));
    }

    blinkLED(STDBLKRATE);            // blink led at standard rate (ota update disabled)

    if (currentTime - previousTime < MAXTIME) {
        currentTime = millis();
    } else {
        ++i;
        previousTime = millis();
        Serial.printf("I'm in loop() waiting for work to do ... %u\n\r", i);
        TelnetStream.printf("I'm in loop() waiting for work to do ... %u\n\r", i);
    }

}
