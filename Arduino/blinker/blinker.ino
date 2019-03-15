#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel
blinkerLinks{
  3, //Anzahl
  4, //Pin
  NEO_RGB
},
blinkerRechts{3, 8, NEO_RGB};

void setup() {
  blinkerLinks .begin();
  blinkerRechts.begin();

  blinkerLinks .show();
  blinkerRechts.show();
}

void loop() {
  for (int i = 0; i < 3; ++i) {
    delay(500);
    blinkerLinks .setPixelColor(i, 0x40, 0xFF, 0x40);
    blinkerRechts.setPixelColor(i, 0x40, 0xFF, 0x40);
  }
  blinkerLinks .fill(0x00);
  blinkerRechts.fill(0x00);
}
