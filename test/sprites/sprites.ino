
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    Demonstrates how to set up and use the user defined sprites for text animations.

    Speed for the display is controlled by a pot on SPEED_IN analog input.
    Digital switches used for control of text justification and invert mode.
    UI switches are normally HIGH.

PHYSICAL DESIGN:
    Hardware
        MAX7219 Dot Matrix Module 4-in-1 Display - https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
        HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E - https://www.amazon.com/gp/product/B010O1G1ES

    Wiring
        Connections for ESP8266 hardware SPI are:
        MAX72XX Pin    ESP8266 Pin    Notes / Comments
        Vcc              3v3           LED matrices seem to work at 3.3V
        GND              GND           GND
        DIN              D7            HSPID or HMOSI
        CS or LD         D8            HSPICS or HCS
        CLK              D5            CLK or HCLK

REFERENCE MATERIALS:
    * UISwitch library can be found at https://github.com/MajicDesigns/MD_UISwitch
    * MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX

SOURCE:
    Derived from  https://github.com/MajicDesigns/MD_Parola/blob/master/examples/Parola_Sprites_Library/Parola_Sprites_Library.ino

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/



// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <SPI.h>

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
//#include <MD_UISwitch.h>

// simple-display project's include files (~/src/scrolling-display/test/sprites)
#include "debug.h"
#include "sprites.h"


// Define the hardware interface and PINs used for wiring
// NOTE: These pin numbers are for ESO8266 hardware SPI
// NOTE: Four common types of matrix modules on the market: PAROLA, GENERIC, ICSTATION, FC16
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define DATA_PIN  D7    // or MOSI
#define CLK_PIN   D5    // or SCK
#define CS_PIN    D8    // or SS

// display text effects
const uint8_t MAX_DEVICES = 24;   // number of dot matrix modules
const uint8_t SCROLL_SPEED = 50;  // lower the number the faster the animation; 0 to run as fast as possible
const uint8_t FRAMEDELAY = 200;   // frame delay value min=10 (fast)  max=200 (slow)
const uint8_t INTENSITY = 0;      // set intensity of the display (0-15)
const uint8_t SPACING = 0;        // distance between the end of one message and the start of the next (0 = off display)
const uint8_t SPEED_IN = 0;       // control the speed with an external pot
const uint8_t JUSTIFY_SET = 6;    // change the justification
const uint8_t INVERSE_SET = 0;    // set/reset the display to inverse
const uint16_t PAUSE_TIME = 2000; // in milliseconds
const uint8_t SPEED_DEADBAND = 5; // in analog units

// Global variables
const char *msg[] = {
    "Parola Sprites",
    "Animation"
};

// Parola object constructor for SPI hardware interface
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                          // HARDWARE SPI


//------------------------------------------------------------------------------

void setup(void) {

    unsigned long tout;                           // time-out time

    Serial.begin(9600);
    PRINT("\n\r-------------------------------------------------------");
    INFO("Entered setup()...");
    INFO("Initializing scrolling display...");

    // initialize the display (aka Parola object)
    P.begin();                                        // initialize the display and data object
    P.setIntensity(INTENSITY);                        // set intensity of the display
    P.setTextAlignment(PA_LEFT);                      // set the text alignment (e.g. PA_LEFT)
    P.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);  // special effects when scrolling in & out (e.g. PA_SCROLL_LEFT)
    P.setScrollSpacing(SPACING);                      // columns between messages
    P.setSpeed(SCROLL_SPEED);                         // frame delay value
    P.setPause(PAUSE_TIME);                           // ms of pause after finished displaying message
    P.displayClear();                                 // clear the display

    // post to the display
    tout = 4000 + millis();                        // milliseconds of time to display message
    while (millis() < tout) {
        if (P.displayAnimate())
            P.displayText("Stand-By!!", PA_CENTER, SCROLL_SPEED, PAUSE_TIME, PA_SCROLL_UP, PA_DISSOLVE);
        yield();                                   // prevent the watchdog timer doing a reboot
    }

    P.displayText("Reset to Sprite Display", PA_CENTER, P.getSpeed(), PAUSE_TIME, PA_SPRITE, PA_SPRITE);
    P.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman2, W_PMAN2, F_PMAN2);
}


void loop(void) {

    static uint8_t curFX = 0;
    static uint8_t curString = 0;

    if (P.displayAnimate()) {
        if (curString == ARRAY_SIZE(msg)) {
            INFO("Doing P.setSpriteData()");
            P.setSpriteData(sprite[curFX].data, sprite[curFX].width, sprite[curFX].frames,  // entry sprite
                      sprite[curFX].data, sprite[curFX].width, sprite[curFX].frames);       // exit sprite

            curFX = (curFX + 1) % ARRAY_SIZE(sprite);
            curString = 0;
        }

        INFO("Doing P.setTextBuffer()");
        P.setTextBuffer(msg[curString]);
        P.displayReset();
        curString++;
    }

}
