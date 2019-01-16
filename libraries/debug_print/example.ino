//Prints numbers in various formats.

#define DEBUG

#include "debug_print.h"

void setup() {
  if (debug) {
    Serial.begin(9600); // opens the serial port at 9600 bps
  }
}

void loop() {
  // print labels
  debugPrintLine( // prints a carriage return after the values you pass to it
    "NO FORMAT" "\t" // prints a label and a tab
    "DEC"       "\t"
    "HEX"       "\t"
    "OCT"       "\t"
    "BIN"
  );

  for (int x{0}; x != 64; ++x) { // only numbers from 0 to 64, change to suit
    debugPrintLine(
          x , "\t\t", // prints x as an ASCII-encoded decimal - same as dec(x) - and two tabs to accommodate the label length
      dec(x), "\t"  , // prints x as an ASCII-encoded decimal                    and a tab
      hex(x), "\t"  , // prints x as an ASCII-encoded hexadecimal                and a tab
      oct(x), "\t"  , // prints x as an ASCII-encoded octal                      and a tab
      bin(x), "\t"    // prints x as an ASCII-encoded binary                     and a tab
    );
    delay(200); // delay 200 milliseconds
  }

  debugPrintLine(); // prints a carriage return
}
