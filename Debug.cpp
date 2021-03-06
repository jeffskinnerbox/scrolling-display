
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0

DESCRIPTION:

PHYSICAL DESIGN:
    Just ESP8266, nothing else required

MONITOR:
    Make sure SERIAL = true and TELNET = true

    sudo arp-scan 192.168.1.200/24
    sudo arp-scan 192.168.1.200/24 | grep Espressif
    nslookup 192.168.1.44

    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Ctrl-a :quit

    telnet scrolling-display.local
    telnet 192.168.1.44
    telnet ESP_24F9FD.fios-router.home.
    telnet ESP_24F9FD
    to terminate Ctrl-] quit

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

// found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <Arduino.h>
#include <ESP8266WiFi.h>

// found in Arduino libraries (~/Arduino/libraries)
#include <TelnetStream.h>

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// for trace messages/debugging, found in this directory
#include "Debug.h"

#define COLS      30        // max characters in labels
#define ROWS       5        // number of labels (see list in constructor below)



// ------------------------ Constructors & Destructors -------------------------

// default constructors for the class
DeBug::DeBug(void) {

    serial = true;      // flag to turn on/off serial trace messages
    telnet = false;     // flag to turn on/off telnet trace messages
    preamble = false;   // flag to turn on/off preamble for trace messages
    cols = COLS;        // max characters in labels
    rows = ROWS;        // number of labels

    // create matrix used to store trace message labels
    label = new char*[rows];
    if (rows) {
        label[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            label[i] = label[0] + i * cols;
    }

    // initialize trace message labels
    label[INFO] =       "\e[1;32mINFO:    \e[m";        // bold green font
    label[WARN] =       "\e[1;33mWARNING: \e[m";        // bold yellow font
    label[ERROR] =      "\e[1;31mERROR:   \e[m";        // bold red font
    label[FATAL] =      "\e[1;37m\e[41mFATAL:\e[m   ";  // bold White font on red background
    label[UNLABELED] =  "";                             // no labels
}


// constructors for the class
DeBug::DeBug(bool s, bool t, bool p) {

    serial = s;         // flag to turn on/off serial trace messages
    telnet = t;         // flag to turn on/off telnet trace messages
    preamble = p;       // flag to turn on/off preamble for trace messages
    cols = COLS;        // max characters in labels
    rows = ROWS;        // number of labels

    // create matrix used to store trace message labels
    label = new char*[rows];
    if (rows) {
        label[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            label[i] = label[0] + i * cols;
    }

    // initialize trace message labels
    label[INFO] =       "\e[1;32mINFO:    \e[m";        // bold green font
    label[WARN] =       "\e[1;33mWARNING: \e[m";        // bold yellow font
    label[ERROR] =      "\e[1;31mERROR:   \e[m";        // bold red font
    label[FATAL] =      "\e[1;37m\e[41mFATAL:\e[m   ";  // bold White font on red background
    label[UNLABELED] =  "";                             // no labels

}


// destructors for the class
DeBug::~DeBug(void) {
    // delete array used to store trace message labels
    if (rows) delete [] label[0];
    delete [] label;
};



// ------------------------------ Private Methods ------------------------------

// print status of Debug object
void DeBug::printStatus(void) {

    Serial.printf("Serial print flag = %s\r\n", serial ? "true" : "false");
    Serial.printf("Telnet print flag = %s\r\n", telnet ? "true" : "false");
    Serial.printf("Preamble print flag = %s\r\n", preamble ? "true" : "false");

    TelnetStream.printf("Serial print flag = %s\r\n", serial ? "true" : "false");
    TelnetStream.printf("Telnet print flag = %s\r\n", telnet ? "true" : "false");
    TelnetStream.printf("Preamble print flag = %s\r\n", preamble ? "true" : "false");

/*    Serial.println("Serial print flag = " + String(serial));*/
    //Serial.println("Telnet print flag = " + String(telnet));
    //Serial.println("Preamble print flag = " + String(preamble));

    //TelnetStream.println("Serial print flag = " + String(serial));
    //TelnetStream.println("Telnet print flag = " + String(telnet));
    /*TelnetStream.println("Preamble print flag = " + String(preamble));*/

}


// print file name, function name, and line number
void DeBug::location(void) {
    if (serial) Serial.print("NOT IMPLEMENTED YET!: ");
    if (telnet) TelnetStream.print("NOT IMPLEMENTED YET!: ");
}



// ------------------------------- Public Methods ------------------------------

void DeBug::debugBegin(void) {

    if (telnet) {
        TelnetStream.begin();
        Serial.println("\n\rTelnetStream enabled");
        TelnetStream.println("TelnetStream enabled");
    }

    printStatus();

}


void DeBug::debugOnOff(bool flag) {
    serial = flag;
}


void DeBug::telnetOnOff(bool flag) {
    telnet = flag;
}


void DeBug::preambleOnOff(bool flag) {
    preamble = flag;
}


void DeBug::printInfo(void) {
    char string[20];

    traceMsg(INFO, "Information concerning ESP & flash memory chip:");

    sprintf(string, "%03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    traceMsg(INFO, "\tESP8266 IP address = ", string);
    traceMsg(INFO, "\tESP8266 MAC address = ", WiFi.macAddress());
    traceMsg(INFO, "\tESP8266 chip ID (HEX) = %X\n\r", ESP.getChipId(), HEX);
    traceMsg(INFO, "\tChip ID (HEX): %X\n\r", ESP.getFlashChipId(), HEX);
    traceMsg(INFO, "\tChip Real Size (from chip): %d bits\n\r", ESP.getFlashChipRealSize(), DEC);
    traceMsg(INFO, "\tChip Size (what compiler set): %d bits\n\r", ESP.getFlashChipSize(), DEC);
    traceMsg(INFO, "\tChip Speed: %d Hz\n\r", ESP.getFlashChipSpeed(), DEC);
    traceMsg(INFO, "\tChip Mode: %d\n\r", ESP.getFlashChipMode(), DEC);
    traceMsg(INFO, "\tFree Heap Memory: %d bytes\n\r", ESP.getFreeHeap(), DEC);
    traceMsg(INFO, "\tHeap Fragmentation: %d%%\n\r", ESP.getHeapFragmentation(), DEC);  // 0% is clean, more than ~50% is not harmless

}


void DeBug::TelnetHandler(void) {

    switch (TelnetStream.read()) {
        case 'R':   // reboot the esp8266
            Serial.println("\n\rRebooting ...");
            TelnetStream.println("\n\rRebooting ...");
            Serial.flush();
            TelnetStream.flush();
            Serial.end();
            TelnetStream.stop();
            delay(100);
            ESP.reset();
            break;
        case 'C':   // drop telnet connection to esp8266
            Serial.println("\n\rDropping telnet connection ... bye bye");
            TelnetStream.println("\n\rDropping telnet connection ... bye bye");
            Serial.flush();
            TelnetStream.flush();
            TelnetStream.stop();
            break;
        case 's':   // toggle on/off serial trace messages
            serial = !serial;
            printStatus();
            break;
        case 't':   // toggle on/off telnet trace messages
            telnet = !telnet;
            printStatus();
            break;
        case 'p':   // toggle on/off preamble for trace messages
            preamble = !preamble;
            printStatus();
            break;
        default:
            break;
    }

    switch (Serial.read()) {
        case 'R':   // reboot the esp8266
            Serial.println("\n\rRebooting ...");
            TelnetStream.println("\n\rRebooting ...");
            Serial.flush();
            TelnetStream.flush();
            Serial.end();
            TelnetStream.stop();
            delay(100);
            ESP.reset();
            break;
        case 'C':   // drop telnet connection to esp8266
            Serial.println("\n\rDropping telnet connection ... bye bye");
            TelnetStream.println("\n\rDropping telnet connection ... bye bye");
            Serial.flush();
            TelnetStream.flush();
            TelnetStream.stop();
            break;
        case 's':   // toggle on/off serial trace messages
            serial = !serial;
            printStatus();
            break;
        case 't':   // toggle on/off telnet trace messages
            telnet = !telnet;
            printStatus();
            break;
        case 'p':   // toggle on/off preamble for trace messages
            preamble = !preamble;
            printStatus();
            break;
        default:
            break;
    }

}


template<typename T>
void DeBug::printMsg(T var) {

    if (serial) Serial.print(var);

    if (!telnet) return;
    else TelnetStream.print(var);
}


template<typename T, typename U>
void DeBug::printMsg(T str, U var) {

    if (serial) {
        Serial.print(str);
        Serial.print(var);
    }

    if (!telnet) return;
    else {
        TelnetStream.print(str);
        TelnetStream.print(var);
    }
}


template<typename T, typename U, typename Z>
void DeBug::printMsg(T *str, U var, Z format) {

    if (serial) {
        Serial.print(str);
        Serial.print(var, format);
    }

    if (!telnet) return;
    else {
        TelnetStream.print(str);
        TelnetStream.print(var, format);
    }
}


void DeBug::traceMsg(int lev, char *str) {

    if (preamble) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.println(str);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.println(str);
        }
    } else
        printMsg(str);

}


template<typename T>
void DeBug::traceMsg(int lev, char *str, T var) {

    if (preamble) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.print(str);
            Serial.println(var);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.print(str);
            TelnetStream.println(var);
        }
    } else {
        printMsg(str);
        printMsg(var);
    }

}


template<typename T, typename U>
void DeBug::traceMsg(int lev, char *str, T var, U format) {

    if (preamble) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.printf(str, var, format);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.printf(str, var, format);
        }
    } else {
        printMsg(str);
        printMsg(var, format);
    }

}



// ------------------ Explicitly Declare All Needed Functions ------------------

template void DeBug::printMsg<int>(int);
template void DeBug::printMsg<char*>(char*);
template void DeBug::printMsg<wl_status_t>(wl_status_t);
template void DeBug::printMsg<char const*>(char const*);
template void DeBug::printMsg<unsigned int, int>(unsigned int, int);

template void DeBug::traceMsg<int>(int, char*, int);
template void DeBug::traceMsg<char*>(int, char*, char*);
template void DeBug::traceMsg<String>(int, char*, String);
template void DeBug::traceMsg<IPAddress>(int, char*, IPAddress);
template void DeBug::traceMsg<char const*>(int, char*, char const*);
template void DeBug::traceMsg<wl_status_t>(int, char*, wl_status_t);
template void DeBug::traceMsg<unsigned char>(int, char*, unsigned char);
template void DeBug::traceMsg<FlashMode_t, int>(int, char*, FlashMode_t, int);
template void DeBug::traceMsg<unsigned int, int>(int, char*, unsigned int, int);
template void DeBug::traceMsg<unsigned char, int>(int, char*, unsigned char, int);


// --------------------------- Construct DeBug Object --------------------------

// for trace messages/debugging, construct object DB as class DeBug
DeBug DB = DeBug(true, true, false);

