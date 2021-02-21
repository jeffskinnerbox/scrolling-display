
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.6.0

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

// found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <Arduino.h>
#include <ESP8266WiFi.h>

// found in Arduino libraries (~/Arduino/libraries)

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// for trace messages/debugging, found in this directory
#include "debug.h"

#define COLS      30        // max characters in labels
#define ROWS       5        // number of labels (see list in constructor below)


// ------------------------ Constructors & Destructors -------------------------

// constructors for the class
DeBug::DeBug(void) {

    debug = true;      // flag to turn on/off debugging trace messages
    preamble = false;  // flag to turn on/off preamble for trace messages
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
    label[FATAL] =      "\e[1;37m\e[41mFATAL:   \e[m";  // bold White font on red background
    label[UNLABELED] =  "";                             // no labels
};


// destructors for the class
DeBug::~DeBug(void) {
    // delete array used to store trace message labels
    if (rows) delete [] label[0];
    delete [] label;
};

// ------------------------------ Private Methods ------------------------------

// print file name, function name, and line number
void DeBug::location() {
    Serial.print("NOT IMPLEMENTED YET!: ");
}


// ------------------------------- Public Methods ------------------------------

void DeBug::debugOnOff(bool flag) {
    debug = flag;
}


void DeBug::preambleOnOff(bool flag) {
    preamble = flag;
}


template<typename T>
void DeBug::printMsg(T var) {
    if (!debug) return;
    Serial.print(var);
}

template<typename T, typename U>
void DeBug::printMsg(T str, U var) {
    if (!debug) return;
    Serial.print(str);
    Serial.print(var);
}

template<typename T, typename U, typename Z>
void DeBug::printMsg(T *str, U var, Z format) {
    if (!debug) return;
    Serial.print(str);
    Serial.print(var, format);
}

void DeBug::traceMsg(int lev, char *str) {
    if (!debug) return;
    if (preamble) location();
    if (lev != UNLABELED) {
        Serial.print(label[lev]);
        Serial.println(str);
    } else {
        printMsg(str);
    }
};

template<typename T>
void DeBug::traceMsg(int lev, char *str, T var) {
    if (!debug) return;
    if (preamble) location();
    if (lev != UNLABELED) {
        Serial.print(label[lev]);
        Serial.print(str);
        Serial.println(var);
    } else {
        printMsg(str);
        printMsg(var);
    }
};

template<typename T, typename U>
void DeBug::traceMsg(int lev, char *str, T var, U format) {
    if (!debug) return;
    if (preamble) location();
    if (lev != UNLABELED) {
        Serial.print(label[lev]);
        Serial.printf(str, var, format);
    } else {
        printMsg(str);
        printMsg(var, format);
    }
};

// ---------------- Explicitly Instantiate All Needed Templates ----------------

template void DeBug::printMsg<int>(int);
template void DeBug::printMsg<char const*>(char const*);
template void DeBug::printMsg<unsigned int, int>(unsigned int, int);
template void DeBug::printMsg<wl_status_t>(wl_status_t);

template void DeBug::printMsg<char*>(char*);
template void DeBug::traceMsg<int>(int, char*, int);
template void DeBug::traceMsg<char*>(int, char*, char*);
template void DeBug::traceMsg<String>(int, char*, String);
template void DeBug::traceMsg<unsigned int, int>(int, char*, unsigned int, int);
template void DeBug::traceMsg<unsigned char, int>(int, char*, unsigned char, int);
template void DeBug::traceMsg<FlashMode_t, int>(int, char*, FlashMode_t, int);
template void DeBug::traceMsg<IPAddress>(int, char*, IPAddress);
template void DeBug::traceMsg<wl_status_t>(int, char*, wl_status_t);
template void DeBug::traceMsg<unsigned char>(int, char*, unsigned char);


// for trace messages/debugging, construct object DB as class DeBug
DeBug DB = DeBug();

