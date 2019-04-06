#include <Wire.h>

namespace warnbake {

void sendeCodeNummer(const uint8_t nummer) {
  Wire.beginTransmission(0); //Uno sollte auf Slave-Adresse 0 liegen.
  Wire.write(nummer);
  Wire.endTransmission();
}

void aus() {
  sendeCodeNummer(0);
}
void wand() {
  sendeCodeNummer(1);
}
void pfeilLinks() {
  sendeCodeNummer(2);
}
void pfeilRechts() {
  sendeCodeNummer(3);
}
void blinkerBlau() {
  sendeCodeNummer(4);
}
void blinkerGelb() {
  sendeCodeNummer(5);
}

}
