
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      1.0.0

DESCRIPTION:
    Demonstration program showing the the use of the MD_Parola library display
    scrolling features.

    Now using more advanced text scrolling features.

MONITOR:
    screen /dev/ttyUSB0 9600,cs8
    to terminate Cntr-a :quit

REFERENCE MATERIALS:
    For textEffect parameters - https://github.com/MajicDesigns/MD_Parola/blob/master/src/MD_Parola.h

SOURCES:
    Derived from https://github.com/MajicDesigns/MD_Parola/blob/master/examples/Parola_Ambulance/Parola_Ambulance.ino

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// define units of time
#define ONE_SECOND     1000UL        // milliseconds in one second
#define TWO_SECOND     2000UL        // milliseconds in two second
#define Three_SECOND   3000UL        // milliseconds in three second
#define ONE_MINUTE     60000UL       // milliseconds in one minute
#define ONE_HOUR       3600000UL     // milliseconds in one hour
#define ONE_DAY        85400000UL    // milliseconds in one day

// debugging routings print trace messages on serial port, enable with value 'true'
#define DEBUG true
#if DEBUG
#define PRINT(s, v)   { Serial.print(F(s)); Serial.println(v); }          // Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.println(v, HEX); }     // Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.println(v, BIN); }     // Print a string followed by a value (binary)
#define PRINTC(s, v)  { Serial.print(F(s)); Serial.println(ln(char)v); }  // Print a string followed by a value (char)
#define PRINTS(s)     { Serial.println(F(s)); }                           // Print a string
#else
#define PRINT(s, v)   // Print a string followed by a value (decimal)
#define PRINTX(s, v)  // Print a string followed by a value (hex)
#define PRINTB(s, v)  // Print a string followed by a value (binary)
#define PRINTC(s, v)  // Print a string followed by a value (char)
#define PRINTS(s)     // Print a string
#endif

// NOTE: These pin numbers are for ESO8266 hardware SPI
#define DATA_PIN       D7    // or MOSI
#define CLK_PIN        D5    // or SCK
#define CS_PIN         D8    // or SS

// display speeds, intensity, etc.
#define PAUSE_TIME    TWO_SECOND
#define SCROLL_SPEED  50    // lower the number the faster the animation; 0 to run as fast as possible
#define FRAMEDELAY   200    // frame delay value min=10 (fast)  max=200 (slow)
#define INTENSITY      0    // set intensity of the display (0-15)
#define SPACING        0    // distance between the end of one message and the start of the next (0 = off display)

// hardware coordinate mapping
// four common types of matrix modules on the market: PAROLA, GENERIC, ICSTATION, FC16
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW

// number of LED dot matrix modules in the chain
#define MAX_DEVICES    24  // number of modules connected

// Software SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);  // software SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                     // hardware SPI

// messages to be displayed
const char *msg[] = {
    "0 1 2 3 4 5 6 7 8 9",
    "Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm",
    "Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz",
    "! @ # $ % ^ & * ( ) _ + - = [ { ] }",
    "\\ | ; : ' \" , < . > / ? ` ~",
};

// display text effects
const textPosition_t scrollAlign = PA_CENTER;        // how to align the text (e.g. PA_LEFT)
const textEffect_t scrollEffectIn = PA_SCROLL_UP;    // direction of scrolling (e.g. PA_SCROLL_LEFT)
const textEffect_t scrollEffectOut = PA_DISSOLVE;    // special effects when scrolling (e.g. PA_RANDOM)
const uint8_t intensity = INTENSITY;                 // intensity of matrix LEDs
const uint8_t scrollSpeed = SCROLL_SPEED;            // frame delay value
const uint16_t scrollPause = TWO_SECOND;             // ms of pause after finished displaying message



//------------------------------------------------------------------------------

void setup() {
    int i;

    Serial.begin(9600);
    PRINTS("Entered setup()");

//    PRINTS("Text to be displayed:");
//    for (i = 0; i < ARRAY_SIZE(msg); ++i) {
//        PRINTS(msg[i]);
//    }

    P.begin();                                           // initialize the display and data object
    P.setIntensity(intensity);                           // set intensity of the display
    P.setTextAlignment(scrollAlign);                     // set the text alignment
    P.setTextEffect(scrollEffectIn, scrollEffectOut);    // special effects when scrolling
    P.setScrollSpacing(SPACING);                         // columns between messages
    P.setSpeed(scrollSpeed);                             // frame delay value
    P.setPause(scrollPause);                             // ms of pause after finished displaying message
    P.displayClear();                                    // clear the display

    PRINTS("Exit setup()");
}


void loop() {
    static uint8_t cycle = 0;                            // message number being displayed

    if (P.displayAnimate()) {
        PRINT("Displaying message #", cycle);

        // send the string to the display
        P.displayText(msg[cycle], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);

        // prepare index into msg[] for next pass
        cycle = (cycle + 1) % ARRAY_SIZE(msg);
    }
}
