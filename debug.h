
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.3.0

DESCRIPTION:
Serial.println(val)
Serial.println(val, format)

val: the value to print. Allowed data types: any data type.
format: specifies the number base (for integral data types) or number of decimal places (for floating point types).

REFERENCE MATERIALS:
https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
Passing Different Types to Functions in C++ - https://www.dummies.com/programming/cpp/passing-different-types-to-functions-in-c/
Templates in C++ - https://www.dummies.com/programming/cpp/templates-in-c/
Function templates - http://www.cplusplus.com/doc/oldtutorial/templates/
Template member functions - https://blog.feabhas.com/2014/07/template-member-functions/

C++: “undefined reference to” templated class function - https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function

CREATED BY:
    jeffskinnerbox@yahoo.com

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

#define DEBUG  true    // activate trace message printing for debugging on serial
#define TELNET false   // activate trace message printing for debugging via telnet

#if DEBUG

#define COLS      30      // max characters in labels
#define ROWS       5      // number of labels (see below)
#define INFONEW    0      // flag to print information trace message
#define WARNNEW    1      // flag to print warning trace message
#define ERRORNEW   2      // flag to print error trace message
#define FATALNEW   3      // flag to print fatal trace message
#define UNFORMATED 4      // flag to print unfomated text

class DeBug {
  // private variables
  private:
    int cols = COLS;      // max characters in labels
    int rows = ROWS;      // number of labels
    char **array = NULL;  // memory array used to store labels

  public:
    // constructors for the class
    inline DeBug(void) {
        // create matrix used to store level labels
        array = new char*[rows];
        if (rows) {
            array[0] = new char[rows * cols];
            for (int i = 1; i < rows; ++i)
                array[i] = array[0] + i * cols;
        }

        // initialize labels
        array[INFONEW] =    "\e[1;32mINFO:    \e[m";        // bold green
        array[WARNNEW] =    "\e[1;33mWARNING: \e[m";        // bold yellow
        array[ERRORNEW] =   "\e[1;31mERROR:   \e[m";        // bold red
        array[FATALNEW] =   "\e[1;37m\e[41mFATAL:   \e[m";  // bold White on red background
        array[UNFORMATED] = "";                             // no formating
    };

    // destructors for the class
    inline ~DeBug(void) {
        // delete array used to store labels
        if (rows) delete [] array[0];
        delete [] array;
    };

    //----------------------------------------------

    // public methods
    template<typename T>
    inline void unformatted(T var) {
        Serial.print(var);
    }

    template<typename T, typename U>
    inline void unformatted(T str, U var) {
        Serial.print(str);
        Serial.print(var);
    }

    template<typename T, typename U, typename Z>
    inline void unformatted(T *str, U var, Z format) {
            Serial.print(str);
            Serial.print(var, format);
    }

    //----------------------------------------------

    inline void println(int lev, char *str) {
        if (lev != UNFORMATED) {
            Serial.print(array[lev]);
            Serial.println(str);
        } else {
            unformatted(str);
        }
    };

    template<typename T>
    inline void println(int lev, char *str, T var) {
        if (lev != UNFORMATED) {
            Serial.print(array[lev]);
            Serial.print(str);
            Serial.println(var);
        } else {
            unformatted(str);
            unformatted(var);
        }
    };

    template<typename T, typename U>
    inline void println(int lev, char *str, T var, U format) {
        if (lev != UNFORMATED) {
            Serial.print(array[lev]);
            Serial.print(str);
            Serial.println(var, format);
        } else {
            unformatted(str);
            unformatted(var, format);
        }
    };
};




// -----------------------------------------------------------------------------


//const char db_fatal[] = "\e[1;37m\e[41mFATAL:   \e[m";
//const char db_info[]= "\e[1;32mINFO:    \e[m";
//const char db_error[]= "\e[1;31mERROR:   \e[m";
//const char db_warn[]= "\e[1;33mWARNING: \e[m";

// debugging routings that print trace messages on serial port
// using F() to load static strings in flash memory, not RAM
//#define TEST(s)          { DB.println(s); }  // for testing only
//#define TEST(s)          { DB.println(s); }  // for testing only

#define PRINT(s)          { Serial.print(F(s)); }                             // Print a string without newline
//#define PRINTNL(s)        { Serial.println(F(s)); }                           // Print a string
//#define PRINTF(s, f, v)   { Serial.print(F(s)); Serial.printf(f, v); }     // Print a string followed by format & variable
//#define PRINTD(s, v)   { Serial.print(F(s)); Serial.println(v, DEC); }     // Print a string followed by decimal
//#define PRINTX(s, v)   { Serial.print(F(s)); Serial.println(v, HEX); }     // Print a string followed by hex
//#define PRINTB(s, v)   { Serial.print(F(s)); Serial.println(v, BIN); }     // Print a string followed by binary
//#define PRINTC(s, v)   { Serial.print(F(s)); Serial.println((char)v); }    // Print a string followed by char
//#define PRINTS(s, v)   { Serial.print(F(s)); Serial.println(v); }          // Print a string followed by string

#define EXEC(s)        { s; }
//#define FATAL(s)       { Serial.print(db_fatal); PRINTNL(s) }

//#define INFO(s)        { Serial.print(db_info); PRINTNL(s) }
//#define ERROR(s)       { Serial.print(db_error); PRINTNL(s) }
//#define WARNING(s)     { Serial.print(db_warn); PRINTNL(s) }

//#define INFOD(s, v)    { Serial.print(db_info); Serial.print(F(s)); Serial.println(v, DEC); }
//#define ERRORD(s, v)   { Serial.print(db_error); Serial.print(F(s)); Serial.println(v, DEC); }
//#define WARNINGD(s, v) { Serial.print(db_warn); Serial.print(F(s)); Serial.println(v, DEC); }

//#define INFOX(s, v)    { Serial.print(db_info); Serial.print(F(s)); Serial.println(v, HEX); }
//#define ERRORX(s, v)   { Serial.print(db_error); Serial.print(F(s)); Serial.println(v, HEX); }
//#define WARNINGX(s, v) { Serial.print(db_warn); Serial.print(F(s)); Serial.println(v, HEX); }

//#define INFOB(s, v)    { Serial.print(db_info); Serial.print(F(s)); Serial.println(v, BIN); }
//#define ERRORB(s, v)   { Serial.print(db_error); Serial.print(F(s)); Serial.println(v, BIN); }
//#define WARNINGB(s, v) { Serial.print(db_warn); Serial.print(F(s)); Serial.println(v, BIN); }

//#define INFOC(s, v)    { Serial.print(db_info); Serial.print(F(s)); Serial.println((char)v); }
//#define ERRORC(s, v)   { Serial.print(db_error); Serial.print(F(s)); Serial.println((char)v); }
//#define WARNINGC(s, v) { Serial.print(db_warn); Serial.print(F(s)); Serial.println((char)v); }

//#define INFOS(s, v)    { Serial.print(db_info); Serial.print(F(s)); Serial.println(v); }
//#define ERRORS(s, v)   { Serial.print(db_error); Serial.print(F(s)); Serial.println(v); }
//#define WARNINGS(s, v) { Serial.print(db_warn); Serial.print(F(s)); Serial.println(v); }

/*#else // DEBUG*/

    //#define PRINT(s)
    //#define PRINTNL(s)
    //#define PRINTD(s, v)
    //#define PRINTX(s, v)
    //#define PRINTB(s, v)
    //#define PRINTC(s, v)
    //#define PRINTS(s, v)

    //#define EXEC(s)
    //#define FATAL(s)

    //#define INFO(s)
    //#define ERROR(s)
    //#define WARNING(s)

    //#define INFOD(s, v)
    //#define ERRORD(s, v)
    //#define WARNINGD(s, v)

    //#define INFOX(s, v)
    //#define ERRORX(s, v)
    //#define WARNINGX(s, v)

    //#define INFOB(s, v)
    //#define ERRORB(s, v)
    //#define WARNINGB(s, v)

    //#define INFOC(s, v)
    //#define ERRORC(s, v)
    //#define WARNINGC(s, v)

    //#define INFOS(s, v)
    //#define ERRORS(s, v)
    //#define WARNINGS(s, v)

#endif // DEBUG

