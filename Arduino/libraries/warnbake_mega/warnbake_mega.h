#ifndef WARNBAKE_MEGA_H
#define WARNBAKE_MEGA_H

#ifdef CAR1
  #error Der Warnbaken-Header wurde eingebunden, obwohl das erste Auto verwendet wird, das keine Warnbake hat.
#endif

#include <Wire.h>

namespace warnbake {

void sendeCodeNummer(const byte nummer) {
  Wire.beginTransmission(8); //Uno sollte auf Slave-Adresse 0 liegen.
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
void mannGruen() {
  sendeCodeNummer(6);
}
void mannRot() {
  sendeCodeNummer(7);
}

void benutzerdefiniert() {
  sendeCodeNummer(0xFE);
}
byte passOnByte() {
  const auto theByte = Serial2.read();
  Wire.write(theByte);
  return theByte;
}
void benutzerdefiniertNeu() {
  //leite Animationsdaten von App an Uno weiter
  Wire.beginTransmission(8);
  Wire.write(0xFF);
  const size_t length = 120 * size_t(passOnByte()) << 8 + passOnByte();
  for (size_t i = 0; i < length; ++i) {
    passOnByte();
  }
  Wire.endTransmission();
}

}

#endif
