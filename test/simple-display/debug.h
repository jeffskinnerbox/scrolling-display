
//
// Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
// Version:      0.1.0
//
// debugging routings that print trace messages on serial port
//


#pragma once                 // compiler to skip subsequent includes of this file

// debugging routings that print trace messages on serial port
// using F() to load strings in flash memory, not RAM
#define DEBUG true

#if DEBUG
    #define PRINT(s)      { Serial.print(F(s)); }                             // Print a string
    #define PRINTD(s, v)  { Serial.print(F(s)); Serial.println(v, DEC); }     // Print a string followed by decimal
    #define PRINTX(s, v)  { Serial.print(F(s)); Serial.println(v, HEX); }     // Print a string followed by hex
    #define PRINTB(s, v)  { Serial.print(F(s)); Serial.println(v, BIN); }     // Print a string followed by binary
    #define PRINTC(s, v)  { Serial.print(F(s)); Serial.println((char)v); }    // Print a string followed by char
    #define PRINTS(s, v)  { Serial.print(F(s)); Serial.println(v); }          // Print a string followed by string
#else
    #define PRINT(s)
    #define PRINTD(s, v)
    #define PRINTX(s, v)
    #define PRINTB(s, v)
    #define PRINTC(s, v)
    #define PRINTS(s, v)
#endif


