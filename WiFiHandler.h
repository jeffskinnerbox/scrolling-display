
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/


#pragma once                        // compiler to skip subsequent includes of this file


class WiFiTools {
  private:
    char *hostname = NULL;           // memory array used to store hostname
    char *ssid = NULL;               // memory array used to store wifi ssid
    char *password = NULL;           // memory array used to store wifi password
    unsigned long timeout;           // time out for wifi access request

  public:
    // constructors & destructors for the class
    WiFiTools(void);
    WiFiTools(char *, char *, char *, unsigned long);
    ~WiFiTools(void);

    // public methods
    bool wifiConnect(char *, char *, char *, unsigned long);
    //bool wifiHostname(char *);
    void wifiTerminate(void);
    void wifiScan(void);
};
