#ifndef HARDWARE_H
#define HARDWARE_H

#include <Servo.h>
#include "radio_receiver.h"
#include "LiquidCrystal.h"

#if defined(CAR1) == defined (CAR2)
#error Definiere CAR1 oder CAR2!
#endif

Servo motor, frontAxis, rearAxis;

radio_receiver controllerSignalReceiver;

#ifdef CAR1
  constexpr int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

  //23 ok, 24 down, 25 up
  constexpr int buttonPins[3] {23, 24, 25};
#endif

#endif
