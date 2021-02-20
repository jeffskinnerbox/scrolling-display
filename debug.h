
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.5.0

DESCRIPTION:
Serial.println(val)
Serial.println(val, format)

val: the value to print. Allowed data types: any data type.
format: specifies the number base (for integral data types) or number of decimal places (for floating point types).

The code here assumes you are using the C99 C programming language standard (see https://en.wikipedia.org/wiki/C99).
The variable argument list notation is used within macros and this is not supported in versions earlier than C99.
You'll see the "do { ... } while (0)" used here and this idiom ensures that the code acts like a statement (i.e. function call).
The "if (DEBUG)" use of the macro code ensures that the compiler always checks that your debug code is valid — but the optimizer will remove the code when DEBUG is 0.

REFERENCE MATERIALS:
https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
Passing Different Types to Functions in C++ - https://www.dummies.com/programming/cpp/passing-different-types-to-functions-in-c/
Templates in C++ - https://www.dummies.com/programming/cpp/templates-in-c/
Function templates - http://www.cplusplus.com/doc/oldtutorial/templates/
Template member functions - https://blog.feabhas.com/2014/07/template-member-functions/
#define macro for debug printing in C? - https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c

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

#pragma once              // compiler to skip subsequent includes of this file

#define DEBUG  true       // activate trace message printing for debugging on serial
#define TELNET false      // activate trace message printing for debugging via telnet

#define COLS      30      // max characters in labels
#define ROWS       5      // number of labels (see list below)
#define INFO       0      // index into labels for printing information trace message
#define WARN       1      // index into labels for printing warning trace message
#define ERROR      2      // index into labels for printing error trace message
#define FATAL      3      // index into labels for printing fatal trace message
#define UNFORMATED 4      // index into labels for printing unformatted text

class DeBug {
  // private variables
  private:
    bool debug = true;    // flag to turn on/off debugging trace messages
    bool preamble = false; // flag to turn on/off preamble for trace messages
    int cols = COLS;      // max characters in labels
    int rows = ROWS;      // number of labels
    char **label = NULL;  // memory array used to store labels

  public:
    // constructors for the class
    inline DeBug(void) {
        // create matrix used to store trace message labels
        label = new char*[rows];
        if (rows) {
            label[0] = new char[rows * cols];
            for (int i = 1; i < rows; ++i)
                label[i] = label[0] + i * cols;
        }

        // initialize trace message labels
        label[INFO] =       "\e[1;32mINFO:    \e[m";        // bold green font
        label[WARN] =       "\e[1;33mWARNING: \e[m";        // bold yellow font
        label[ERROR] =      "\e[1;31mERROR:   \e[m";        // bold red font
        label[FATAL] =      "\e[1;37m\e[41mFATAL:   \e[m";  // bold White font on red background
        label[UNFORMATED] = "";                             // unformatted
    };

    // destructors for the class
    inline ~DeBug(void) {
        // delete array used to store trace message labels
        if (rows) delete [] label[0];
        delete [] label;
    };

    // private methods
    inline void location() { Serial.printf("%s, %s, %d: \t", __FILE__, __FUNCTION__, __LINE__); };

    // public methods
    inline void OnOff(bool flag) { debug = flag; }

    //----------------------------------------------

    template<typename T>
    inline void print(T var) {
        if (!debug) return;
        Serial.print(var);
    }

    template<typename T, typename U>
    inline void print(T str, U var) {
        if (!debug) return;
        Serial.print(str);
        Serial.print(var);
    }

    template<typename T, typename U, typename Z>
    inline void print(T *str, U var, Z format) {
        if (!debug) return;
        Serial.print(str);
        Serial.print(var, format);
    }

    //----------------------------------------------

    inline void traceMsg(int lev, char *str) {
        if (!debug) return;
        if (preamble) location();
        if (lev != UNFORMATED) {
            Serial.print(label[lev]);
            Serial.println(str);
        } else {
            print(str);
        }
    };

    template<typename T>
    inline void traceMsg(int lev, char *str, T var) {
        if (!debug) return;
        if (preamble) location();
        if (lev != UNFORMATED) {
            Serial.print(label[lev]);
            Serial.print(str);
            Serial.println(var);
        } else {
            print(str);
            print(var);
        }
    };

    template<typename T, typename U>
    inline void traceMsg(int lev, char *str, T var, U format) {
        if (!debug) return;
        if (preamble) location();
        if (lev != UNFORMATED) {
            Serial.print(label[lev]);
            Serial.print(str);
            Serial.println(var, format);
        } else {
            print(str);
            print(var, format);
        }
    };
};



// -----------------------------------------------------------------------------

/*#if DEBUG*/
//#define PRINT(s)  { Serial.print(F(s)); }                             // Print a string without newline
//#define EXEC(s)   { s; }
//#else // DEBUG
//#define PRINT(s)
//#define EXEC(s)
/*#endif // DEBUG*/

#define DEBUGPRINT(lev, ...) \
    do { if (DEBUG) DB.traceMsg(lev, __VA_ARGS__); } while (0)

