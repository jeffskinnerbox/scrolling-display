
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.5.0

DESCRIPTION:

REFERENCE MATERIALS:
    The ANSI escape sequences set screen attributes, such as bold text,
    and color of foreground and background.

    Standard Colors
        'e[0;30m'           # Black
        'e[0;31m'           # Red
        'e[0;32m'           # Green
        'e[0;33m'           # Yellow
        'e[0;34m'           # Blue
        'e[0;35m'           # Purple
        'e[0;36m'           # Cyan
        'e[0;37m'           # White

    Bold Colors
        'e[1;30m'           # Bold Black
        'e[1;31m'           # Bold Red
        'e[1;32m'           # Bold Green
        'e[1;33m'           # Bold Yellow
        'e[1;34m'           # Bold Blue
        'e[1;35m'           # Bold Purple
        'e[1;36m'           # Bold Cyan
        'e[1;37m'           # Bold White

    Underline
        'e[4;30m'           # Underline Black
        'e[4;31m'           # Underline Red
        'e[4;32m'           # Underline Green
        'e[4;33m'           # Underline Yellow
        'e[4;34m'           # Underline Blue
        'e[4;35m'           # Underline Purple
        'e[4;36m'           # Underline Cyan
        'e[4;37m'           # Underline White

    Background Colors
        'e[40m'             # Background Black
        'e[41m'             # Background Red
        'e[42m'             # Background Green
        'e[43m'             # Background Yellow
        'e[44m'             # Background Blue
        'e[45m'             # Background Purple
        'e[46m'             # Background Cyan
        'e[47m'             # Background White

    Other
        '\e[m'              # Color Reset
        '\e[1;37m\e[41m'    # Bold White on Red Background

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <Arduino.h>
#include <ESP8266WiFi.h>

// Arduino libraries (~/Arduino/libraries)

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

#include "debug.h"


// ------------------------ Constructors & Destructors -------------------------

// constructors for the class
DeBug::DeBug(void) {
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
    label[UNFORMATED] = "";                             // unformatted
};


// destructors for the class
DeBug::~DeBug(void) {
    // delete array used to store trace message labels
    if (rows) delete [] label[0];
    delete [] label;
};

// ------------------------------ Private Methods ------------------------------

// ------------------------------- Public Methods ------------------------------

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
    if (lev != UNFORMATED) {
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
    if (lev != UNFORMATED) {
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
    if (lev != UNFORMATED) {
        Serial.print(label[lev]);
        Serial.printf(str, var, format);
    } else {
        printMsg(str);
        printMsg(var, format);
    }
};


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
