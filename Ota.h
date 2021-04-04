
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.1

DESCRIPTION:

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#pragma once                // compiler to skip subsequent includes of this file

#define DEBUG true          // include DeBug trace message code in compile


class Ota {
  private:
    //-------------- private variables -------------
    bool ota_flag;                // if true, engage the ota handler
    unsigned long max_time;       // max time to wait for ota handler to engage
    unsigned long blink_rate;     // interval at which to blink LED (milliseconds)
    unsigned long standard_rate;  // interval at which to blink LED (milliseconds)

    //--------------- private methods --------------
    void printStatus(void);
    void blinkLED(unsigned long);
    void TelnetStreamHandler(void);

  public:
    //-- constructors & destructors for the class --
    Ota(void);
    Ota(bool, unsigned long, unsigned long, unsigned long);
    ~Ota(void);

    //--------------- public methods ---------------
    void setupOTA(void);
    void loopOTA(void);

};


// -----------------------------------------------------------------------------


