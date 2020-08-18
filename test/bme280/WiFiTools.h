
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#pragma once                        // compiler to skip subsequent includes of this file


class WiFiTools {
  private:
    char *ssid = NULL;               // memory array used to store wifi ssid
    char *password = NULL;           // memory array used to store wifi password
    unsigned long timeout;           // time out for wifi access request

  public:
    // constructors & destructors for the class
    WiFiTools(void);
    ~WiFiTools(void);

    // public methods
    bool wifiConnect(char *, char *, unsigned long);
    void wifiTerminate(void);
    void wifiScan(void);
};
