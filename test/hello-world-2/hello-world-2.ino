
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      1.0.0

DESCRIPTION:
    Demonstration program showing the the use of the MD_Parola library display
    scrolling features.

    An little bit more advanced program that does - Hello World!

MONITOR:
    screen /dev/ttyUSB0 9600,cs8
    to terminate Cntr-a :quit

SOURCES:
    Derived from https://github.com/MajicDesigns/MD_Parola/blob/master/examples/Parola_Ambulance/Parola_Ambulance.ino

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define TRUE              1
#define FALSE             0
#define ONE_SECOND        1000UL        // milliseconds in one second
#define TWO_SECOND        2000UL        // milliseconds in two second
#define ONE_MINUTE        60000UL       // milliseconds in one minute
#define ONE_HOUR          3600000UL     // milliseconds in one hour
#define ONE_DAY           85400000UL    // milliseconds in one day

// hardware coordinate mapping
// four common types of matrix modules on the market: PAROLA, GENERIC, ICSTATION, FC16
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW

// number of LED dot matrix modules in the chain
#define MAX_DEVICES 24  // number of modules connected

// NOTE: These pin numbers are for ESO8266 hardware SPI
#define DATA_PIN  D7    // or MOSI
#define CLK_PIN   D5    // or SCK
#define CS_PIN    D8    // or SS

// Software SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);  // software SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                     // hardware SPI

// messages to be displayed
const char *msg[] = {
    "Hello World!",
    "One more time .... Hello World!",
    "OK ... I'll try it another time!",
};

// display speeds, intensity, etc.
#define PAUSE_TIME    TWO_SECOND
#define SCROLL_SPEED  25    // lower the time the faster the animation; 0 to run as fast as possible
#define FRAMEDELAY    50    // frame delay value min=10 (fast)  max=200 (slow)
#define INTENSITY      0    // set intensity of the display (0-15)
#define SPACING        0    // distance between the end of one message and the start of the next (0 = off display



//------------------------------------------------------------------------------

void setup() {

    Serial.begin(9600);
    Serial.println("Entered setup()");

    P.begin();

    P.setIntensity(INTENSITY);                           // set intensity of the display
    P.setScrollSpacing(SPACING);                         // columns between messages
    P.setPause(TWO_SECOND);                              // ms of pause after finished displaying message
    P.setSpeed(SCROLL_SPEED);                            // frame delay value

    Serial.println("Exit setup()");
}


void loop() {
    static uint8_t cycle = 0;                            // message number being displayed

    const textPosition_t scrollAlign = PA_CENTER;        // how to align the text (other value: PA_LEFT)
    const textEffect_t scrollEffectIn = PA_SCROLL_LEFT;  // direction of scrolling
    const textEffect_t scrollEffectOut = PA_NO_EFFECT;   // special effects when scrolling
    const uint8_t scrollSpeed = SCROLL_SPEED;            // frame delay value
    const uint16_t scrollPause = TWO_SECOND;             // ms of pause after finished displaying message

    if (P.displayAnimate()) {
        Serial.print("Display message #"); Serial.println(cycle, DEC);

        // set up the string
        P.displayText(msg[cycle], scrollAlign, scrollSpeed, scrollPause, scrollEffectIn, scrollEffectOut);

        // prepare for next pass
        cycle = (cycle + 1) % ARRAY_SIZE(msg);
    }
}
