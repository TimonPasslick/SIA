#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel
blinkerLinks{
  3, //Anzahl
  4, //Pin
  NEO_GRB
},
blinkerRechts{3, 8, NEO_RGB};

#define BLINKER(methodenAufruf) blinkerLinks. methodenAufruf ; blinkerRechts. methodenAufruf ;

void setup() {
  BLINKER(begin())
}

void loop() {
  for (int i = 0; i < 3; ++i) {
    BLINKER(setBrightness(0x30))
    BLINKER(setPixelColor(i, 0xF0, 0x20, 0x10))
    BLINKER(show())
    delay(200);
  }
  BLINKER(fill(0x00))
  BLINKER(show())
  delay(300);
}
