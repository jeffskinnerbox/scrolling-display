//
// Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
// Version:      0.0.1
//
// Hello World program to demonstrate the MD_Parola library
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//
//
// Connections for ESP8266 hardware SPI are:
// MAX72XX Pin    ESP8266 Pin    Notes / Comments
//     Vcc           3v3           LED matrices seem to work at 3.3V
//     GND           GND           GND
//     DIN           D7            HSPID or HMOSI
//     CS or LD      D8            HSPICS or HCS
//     CLK           D5            CLK or HCLK
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

// ESP8266 libraries
#include <SPI.h>
#include "ESP8266WiFi.h"

// Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers are for ESO8266 hardware SPI
//#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW    // values are: PAROLA, GENERIC, ICSTATION, FC16
#define MAX_DEVICES 4
#define CLK_PIN   D5    // or SCK
#define DATA_PIN  D7    // or MOSI
#define CS_PIN    D8    // or SS

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// SOFTWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


void setup(void) {
    P.begin();
    P.displayText("Hello", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    //P.displayScroll("Hello.....", PA_CENTER, PA_PRINT, 1);
}

void loop(void) {
    P.displayAnimate();
}
