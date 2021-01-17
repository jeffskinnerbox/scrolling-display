
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:
    the first firmware upload has to be done over a serial port. If the OTA routines are correctly implemented in the sketch, then all subsequent uploads may be done over the air.

    I would like to be able to view serial monitor over wifi, but "Serial monitor is not supported on network ports such as <device ip address>"
    The Serial.print() sends data directly to the hardware serial pins so you need something that has a physical connection to those pins in order to read the data.
    It creates a Telnet port on the ESP chip. You can then connect using a standard Telnet client to access the debugging output.

PHYSICAL DESIGN:
    Just ESP8266, nothe else required

MONITOR:
    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Cntr-a :quit

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

// Arduino libraries (~/src/arduino/libraries)
#include <WiFiUdp.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// basic-ota project's include files (~/src/scrolling-display/test/ota)
#include "credentials.h"

// variables for blinking an LED with Millis (don't use delay)
#define LED D0                          // ESP8266 Pin to which onboard LED is connected
int ledState = LOW;                     // current LED state
unsigned long previousTime = 0;         // stores last time LED was updated
const unsigned long blink_rate = 1000;  // interval at which to blink LED (milliseconds)

// version stamp
#define VER  __DATE__ " at " __TIME__
const char version[] = VER;




//------------------------------ Helper Routines -------------------------------

void telnetPrint() {
}


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

    //Serial.print("Successfully connected to access point ");
    //Serial.println(WiFi.SSID());
    //Serial.print("IP address is ");
    //Serial.println(WiFi.localIP());
    //Serial.print("WiFi setup exit status code is ");
    //Serial.println(WiFi.status());

}


void setupOTA() {

    // if not set, port defaults to 8266
    ArduinoOTA.setPort(8266);

    // if not set, hostname defaults to esp8266-<ChipID-in-Hex>
    ArduinoOTA.setHostname("test-ota");

    // if not set, no authentication required by default
    // ArduinoOTA.setPassword((const char *)"123");

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

    ArduinoOTA.onEnd([]() { Serial.println("\nUpdating ended"); });

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

    // provide status information concerning ota
    //Serial.println("\n\rCompleted OTA provisioning");
    //Serial.print("Hostname = ");
    //Serial.println(ArduinoOTA.getHostname());

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


//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(9600);
    while (!Serial) {}          // wait for serial port to connect

    Serial.println("\n\n\rBooting ...");
    Serial.printf("version / creation date = %s\n\r", version);
    Serial.printf("ESP8266 chip ID = %x\n\r", ESP.getChipId());

    setupWiFi();                // connect to wifi network

    setupOTA();                 // make esp8266 ready for ota support

    getFlashInfo();             // get a sense of memory to support OTA (maybe)

    pinMode(LED, OUTPUT);       // set led pin as output so you can blink it

    Serial.println("\n\rESP8266 is OTA update ready");
    Serial.println(ArduinoOTA.getHostname());
    Serial.println(WiFi.localIP());

}


void loop() {
    static unsigned int i = 0;
    unsigned long currentTime = millis();

    ArduinoOTA.handle();                  // OTA handler, look for OTA update request

    //loop to blink without delay
    if (currentTime - previousTime >= blink_rate) {
        previousTime = currentTime;       // save the last time you blinked the LED
        ledState = not(ledState);         // if the LED is off turn it on and vice-versa
        digitalWrite(LED, ledState);      // set the LED with the ledState of the variable

        //Serial.printf("I'm in loop() waiting for work to do ... %u\n\r", ++i);
    }

}
