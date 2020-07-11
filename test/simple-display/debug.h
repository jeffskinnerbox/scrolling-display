
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    Debugging routings that print trace messages on serial port

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

#pragma once                 // compiler to skip subsequent includes of this file

// debugging routings that print trace messages on serial port
// using F() to load strings in flash memory, not RAM
#define DEBUG true

#if DEBUG
    #define PRINT(s)       { Serial.print(F(s)); }                             // Print a string
    #define PRINTD(s, v)   { Serial.print(F(s)); Serial.println(v, DEC); }     // Print a string followed by decimal
    #define PRINTX(s, v)   { Serial.print(F(s)); Serial.println(v, HEX); }     // Print a string followed by hex
    #define PRINTB(s, v)   { Serial.print(F(s)); Serial.println(v, BIN); }     // Print a string followed by binary
    #define PRINTC(s, v)   { Serial.print(F(s)); Serial.println((char)v); }    // Print a string followed by char
    #define PRINTS(s, v)   { Serial.print(F(s)); Serial.println(v); }          // Print a string followed by string

    #define EXEC(s)        { s; }
    #define FATAL(s)       { Serial.print(F("\e[1;37m\e[41mFATAL:   \e[m")); PRINT(s) }

    #define INFO(s)        { Serial.print(F("\e[1;32mINFO:    \e[m")); PRINT(s) }
    #define ERROR(s)       { Serial.print(F("\e[1;31mERROR:   \e[m")); PRINT(s) }
    #define WARNING(s)     { Serial.print(F("\e[1;33mWARNING: \e[m")); PRINT(s) }

    #define INFOD(s, v)    { Serial.print(F("\e[1;32mINFO:    \e[m")); Serial.print(F(s)); Serial.println(v, DEC); }
    #define ERRORD(s, v)   { Serial.print(F("\e[1;31mERROR:   \e[m")); Serial.print(F(s)); Serial.println(v, DEC); }
    #define WARNINGD(s, v) { Serial.print(F("\e[1;33mWARNING: \e[m")); Serial.print(F(s)); Serial.println(v, DEC); }

    #define INFOX(s, v)    { Serial.print(F("\e[1;32mINFO:    \e[m")); Serial.print(F(s)); Serial.println(v, HEX); }
    #define ERRORX(s, v)   { Serial.print(F("\e[1;31mERROR:   \e[m")); Serial.print(F(s)); Serial.println(v, HEX); }
    #define WARNINGX(s, v) { Serial.print(F("\e[1;33mWARNING: \e[m")); Serial.print(F(s)); Serial.println(v, HEX); }

    #define INFOB(s, v)    { Serial.print(F("\e[1;32mINFO:    \e[m")); Serial.print(F(s)); Serial.println(v, BIN); }
    #define ERRORB(s, v)   { Serial.print(F("\e[1;31mERROR:   \e[m")); Serial.print(F(s)); Serial.println(v, BIN); }
    #define WARNINGB(s, v) { Serial.print(F("\e[1;33mWARNING: \e[m")); Serial.print(F(s)); Serial.println(v, BIN); }

    #define INFOC(s, v)    { Serial.print(F("\e[1;32mINFO:    \e[m")); Serial.print(F(s)); Serial.println((char)v); }
    #define ERRORC(s, v)   { Serial.print(F("\e[1;31mERROR:   \e[m")); Serial.print(F(s)); Serial.println((char)v); }
    #define WARNINGC(s, v) { Serial.print(F("\e[1;33mWARNING: \e[m")); Serial.print(F(s)); Serial.println((char)v); }

    #define INFOS(s, v)    { Serial.print(F("\e[1;32mINFO:    \e[m")); Serial.print(F(s)); Serial.println(v); }
    #define ERRORS(s, v)   { Serial.print(F("\e[1;31mERROR:   \e[m")); Serial.print(F(s)); Serial.println(v); }
    #define WARNINGS(s, v) { Serial.print(F("\e[1;33mWARNING: \e[m")); Serial.print(F(s)); Serial.println(v); }
#else
    #define PRINT(s)
    #define PRINTD(s, v)
    #define PRINTX(s, v)
    #define PRINTB(s, v)
    #define PRINTC(s, v)
    #define PRINTS(s, v)

    #define EXEC(s)
    #define FATAL(s)

    #define INFO(s)
    #define ERROR(s)
    #define WARNING(s)

    #define INFOD(s, v)
    #define ERRORD(s, v)
    #define WARNINGD(s, v)

    #define INFOX(s, v)
    #define ERRORX(s, v)
    #define WARNINGX(s, v)

    #define INFOB(s, v)
    #define ERRORB(s, v)
    #define WARNINGB(s, v)

    #define INFOC(s, v)
    #define ERRORC(s, v)
    #define WARNINGC(s, v)

    #define INFOS(s, v)
    #define ERRORS(s, v)
    #define WARNINGS(s, v)
#endif

